#include "header.h"
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "Usage: ./rayTracing [input file] [output file]\n";
        return 0;
    }

    string inputFile = argv[1];
    Info imgInfo = readInfo(inputFile);
    int widthPixel = imgInfo.imgSize.width;
    int heightPixel = imgInfo.imgSize.height;
    Vector3 eye = imgInfo.eye;
    int imgColorData[widthPixel][heightPixel][3];

    // set the background color
    for (int row = 0; row < heightPixel; row++)
    {
        for (int column = 0; column < widthPixel; column++)
        {
            imgColorData[row][column][0] = imgInfo.bkgcolor.r;
            imgColorData[row][column][1] = imgInfo.bkgcolor.g;
            imgColorData[row][column][2] = imgInfo.bkgcolor.b;
        }
    }

    Vector3 u = calU(imgInfo.viewDir, imgInfo.upDir);             // u
    Vector3 v = calV(u, imgInfo.viewDir);                         // v
    float height = calHeightFromDegree(imgInfo.vFov, DISTANCE);   // height
    float aspect_ratio = calAspectRatio(widthPixel, heightPixel); // aspect ratio
    float width = calWidthFromRatio(aspect_ratio, height);        // width
    Vector3 n = normalizeVector(imgInfo.viewDir);                 // unit vector of viewing direction

    imgInfo.viewWindow.ul = calViewCoordinate("ul", eye, u, v, DISTANCE, n, width, height);
    imgInfo.viewWindow.ur = calViewCoordinate("ur", eye, u, v, DISTANCE, n, width, height);
    imgInfo.viewWindow.ll = calViewCoordinate("ll", eye, u, v, DISTANCE, n, width, height);
    imgInfo.viewWindow.lr = calViewCoordinate("lr", eye, u, v, DISTANCE, n, width, height);

    for (int i = 0; i < heightPixel; i++)
    {
        for (int j = 0; j < widthPixel; j++)
        {
            Vector3 viewCoor = convertPixelToCoor(imgInfo.viewWindow, imgInfo.imgSize, j, i); // coordinate of the assigned pixel location
            Vector3 rayDir = calRayDir(imgInfo.eye, viewCoor);                                // ray direction of assigned pixel location
            float distanceEyePlane = calRayDis(imgInfo.eye, viewCoor);                        // distance from eye toassigned pixel location
            float minDis = distanceEyePlane;

            for (int k = 0; k < imgInfo.spheres.size(); k++)
            { // check which sphere would displayover others
                Sphere sphere = imgInfo.spheres.at(k);
                float sphereDis = calDistanceFromSphere(rayDir, imgInfo.eye, sphere.center, sphere.radius); // distance from eye to sphere if ray intersect the sphere
                if (minDis >= sphereDis)
                {
                    minDis = sphereDis;
                    RBG final;
                    Vector3 rayInterSphere = calRayIntersectSphere(rayDir, imgInfo.eye, sphereDis);

                    final = phongIllu(imgInfo.lights, rayInterSphere, rayDir, imgInfo.spheres, imgInfo.viewDir, k);
                    imgColorData[i][j][0] = convertColor(final.r);
                    imgColorData[i][j][1] = convertColor(final.g);
                    imgColorData[i][j][2] = convertColor(final.b);
                }
            }
        }
    }

    string argv2 = argv[2];
    string outputfile = argv2 + ".bmp";
    ofstream output_stream(outputfile, ios::out);
    output_stream << "P3\n"
                  << widthPixel << " "
                  << heightPixel << "\n"
                  << 255 << "\n";

    for (uint32_t row = 0; row < heightPixel; row++)
    {
        for (uint32_t column = 0; column < widthPixel; column++)
        {
            output_stream << imgColorData[row][column][0] << " " << imgColorData[row][column][1] << " " << imgColorData[row][column][2] << "\n";
        }
    }
    output_stream.close();

    Vector3 a;
    a.x = 1.0 / 3.0;
    a.y = 2.0 / 3.0;
    a.z = 2.0 / 3.0;
    Vector3 b;
    b.x = 0;
    b.y = 0;
    b.z = 0;
    Vector3 p1, p2, p0;
    p1.x = 0;
    p1.y = 1;
    p1.z = 0;
    p2.x = 0;
    p2.y = 0;
    p2.z = 1;
    p0.x = 1;
    p0.y = 0;
    p0.z = 0;
    Triangle c;
    c.p1 = p1;
    c.p2 = p2;
    c.p0 = p0;
    Vector3 ret = calRayIntersectTriangle(a, b, c);
    printf("%f, %f, %f\n", ret.x, ret.y, ret.z);
    return 0;
}