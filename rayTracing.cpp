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
    unsigned int imgColorData[widthPixel][heightPixel][3];

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

    // Calculate all the triangle's surface normal
    // bool smoothShading = false;
    // for (int i = 0; i < imgInfo.triangles.size(); i++)
    // {
    //     if (imgInfo.triangles.at(i).smoothShading)
    //     {
    //         smoothShading = true;
    //     }
    //     imgInfo.triangles.at(i).surfaceNormal = calTriangleSurfaceNormal(imgInfo.triangles.at(i));
    // }
    // if (smoothShading)
    // {
    //     int currentIndex = 0;
    //     if (imgInfo.vertexNormals.size() != 0) // Check if the verticies surface normal were provided
    //     {
    //         currentIndex = imgInfo.vertexNormals.size() - 1;
    //     }
    //     if (currentIndex != imgInfo.vertices.size() - 1) // Calculate the verticies surface normal if all were not provided
    //     {
    //         for (; currentIndex < imgInfo.vertices.size(); currentIndex++)
    //         {
    //             int totalTriangles = 0;
    //             float x = 0, y = 0, z = 0;
    //             for (int j = 0; j < imgInfo.triangles.size(); j++)
    //             {
    //                 Triangle triangle = imgInfo.triangles.at(j);
    //                 if (triangle.v1Index == currentIndex || triangle.v2Index == currentIndex || triangle.v3Index == currentIndex)
    //                 {
    //                     x += triangle.surfaceNormal.x;
    //                     y += triangle.surfaceNormal.y;
    //                     z += triangle.surfaceNormal.z;
    //                     totalTriangles++;
    //                 }
    //             }
    //             Vector3 vertexNormal;
    //             vertexNormal.x = x / float(totalTriangles);
    //             vertexNormal.y = y / float(totalTriangles);
    //             vertexNormal.z = z / float(totalTriangles);
    //             imgInfo.vertexNormals.push_back(vertexNormal);
    //         }
    //     }
    // }

    for (int i = 0; i < heightPixel; i++)
    {
        for (int j = 0; j < widthPixel; j++)
        {
            Barycentric barycentricPoint;
            Vector3 viewCoor = convertPixelToCoor(imgInfo.viewWindow, imgInfo.imgSize, j, i); // coordinate of the assigned pixel location
            Vector3 rayDir = calRayDir(imgInfo.eye, viewCoor);                                // ray direction of assigned pixel location
            float distanceEyePlane = distance(imgInfo.eye, viewCoor);                         // distance from eye to assigned pixel location
            float minDis = distanceEyePlane, t;
            char shape = 'z';
            int index;

            for (int k = 0; k < imgInfo.spheres.size(); k++)
            { // check which sphere would displayover others
                Sphere *sphere = imgInfo.spheres.at(k);
                float tTemp = calTDistanceFromSphere(rayDir, imgInfo.eye, sphere); // distance from eye to sphere if ray intersect the sphere
                if (tTemp >= 0)
                {
                    float sphereDis = calDistanceFromRayEqu(rayDir, imgInfo.eye, tTemp);
                    if (minDis >= sphereDis)
                    {
                        minDis = sphereDis;
                        t = tTemp;
                        index = k;
                        shape = 's';
                    }
                }
            }
            for (int k = 0; k < imgInfo.triangles.size(); k++)
            {
                Triangle triangle = imgInfo.triangles.at(k);
                float tTemp = calTDistanceFromTriangle(rayDir, imgInfo.eye, triangle);
                if (tTemp >= 0)
                {
                    Vector3 rayIntersectionPoint = calRayIntersectObjPoint(rayDir, imgInfo.eye, tTemp);
                    barycentricPoint = barycentricCalculation(triangle, rayIntersectionPoint);
                    if (barycentricPoint.a <= 1 && barycentricPoint.a >= 0 && barycentricPoint.b <= 1 && barycentricPoint.b >= 0 && barycentricPoint.r <= 1 && barycentricPoint.r >= 0 && (barycentricPoint.a + barycentricPoint.b + barycentricPoint.r) <= 1.001 && (barycentricPoint.a + barycentricPoint.b + barycentricPoint.r) >= 0)
                    {
                        // point is inside the triangle
                        float triangleDis = calDistanceFromRayEqu(rayDir, imgInfo.eye, tTemp);
                        if (minDis >= triangleDis)
                        {
                            minDis = triangleDis;
                            t = tTemp;
                            index = k;
                            shape = 't';
                        }
                    }
                }
            }

            if (shape == 's' || shape == 't')
            {
                RGB final;
                Vector3 rayIntersectionPoint = calRayIntersectObjPoint(rayDir, imgInfo.eye, t);
                if (shape == 's')
                {
                    Sphere *sphere = imgInfo.spheres.at(index);
                    Vector3 surfaceNormal = calSphereSurfaceNormal(rayIntersectionPoint, sphere);
                    RGB objDif;
                    if (sphere->textureApplied)
                    {
                        RGB originalFormat = calSphereTextureCoordinate(surfaceNormal, sphere->texture);
                        objDif.r = originalFormat.r / 255.0;
                        objDif.g = originalFormat.g / 255.0;
                        objDif.b = originalFormat.b / 255.0;
                    }
                    else
                    {
                        objDif = sphere->mtlcolor.objDif;
                    }
                    final = phongIllu(imgInfo.lights, imgInfo.spheres, imgInfo.triangles, sphere->mtlcolor, sphere->texture, surfaceNormal, rayIntersectionPoint, imgInfo.viewDir, index, 's', objDif);
                }
                else if (shape == 't')
                {
                    Triangle triangle = imgInfo.triangles.at(index);
                    Vector3 surfaceNormal;
                    if (triangle.smoothShading)
                    {
                        surfaceNormal = calTriangleSurfaceNormalSmooth(imgInfo.vertexNormals, triangle, barycentricPoint);
                    }
                    else
                    {
                        surfaceNormal = calTriangleSurfaceNormal(triangle);
                    }

                    RGB objDif;
                    if (triangle.textureApplied)
                    {
                        RGB originalFormat = calTriangleTextureCoordinate(triangle, barycentricPoint, triangle.texture, imgInfo.vertexTextureCoordinates);
                        objDif.r = originalFormat.r / 255.0;
                        objDif.g = originalFormat.g / 255.0;
                        objDif.b = originalFormat.b / 255.0;
                    }
                    else
                    {
                        objDif = triangle.mtlcolor.objDif;
                    }

                    final = phongIllu(imgInfo.lights, imgInfo.spheres, imgInfo.triangles, triangle.mtlcolor, triangle.texture, surfaceNormal, rayIntersectionPoint, imgInfo.viewDir, index, 't', objDif);
                }
                imgColorData[i][j][0] = convertColor(final.r);
                imgColorData[i][j][1] = convertColor(final.g);
                imgColorData[i][j][2] = convertColor(final.b);
            }
        }
    }

    string argv2 = argv[2];
    string outputfile = argv2 + ".ppm";
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

    // Vector3 rayDir;
    // rayDir.x = 0;
    // rayDir.y = 0;
    // rayDir.z = 1;
    // Vector3 rayOrigin;
    // rayOrigin.x = 0;
    // rayOrigin.y = 0;
    // rayOrigin.z = 0;
    // Vector3 p1, p2, p0;

    // p0.x = 1;
    // p0.y = 5;
    // p0.z = 5;

    // p1.x = 0;
    // p1.y = 8;
    // p1.z = 5;

    // p2.x = -1;
    // p2.y = 5;
    // p2.z = 5;

    // Triangle c;
    // c.v1 = p0;
    // c.v2 = p1;
    // c.v3 = p2;

    // Vector3 pointP;
    // pointP.x = 1.0 / 5.0;
    // pointP.y = 2.0 / 5.0;
    // pointP.z = 2.0 / 5.0;

    // float t = calTDistanceFromTriangle(rayDir, rayOrigin, c);
    // cout << "t: " << t << endl;
    // Vector3 ret = calRayIntersectObjPoint(rayDir, rayOrigin, t);
    // printf("%f, %f, %f\n", ret.x, ret.y, ret.z);
    // Barycentric barycentricPoint = barycentricCalculation(c, ret);
    // printf("baric: %f, %f, %f\n", barycentricPoint.a, barycentricPoint.b, barycentricPoint.r);
    // if (!(barycentricPoint.a > 1 || barycentricPoint.a < 0 || barycentricPoint.b > 1 || barycentricPoint.b < 0 || barycentricPoint.r > 1 || barycentricPoint.r < 0 || barycentricPoint.a + barycentricPoint.b + barycentricPoint.r > 1 || barycentricPoint.a + barycentricPoint.b + barycentricPoint.r < 0))
    // {
    //     cout << "inside" << endl;
    // }
    // else
    // {
    //     cout << "outside" << endl;
    // }
    // Vector3 surfaceNorm = calTriangleSurfaceNormal(c);
    // printf("triangle surfaceNormal: %f, %f, %f\n", surfaceNorm.x, surfaceNorm.y, surfaceNorm.z);
    return 0;
}