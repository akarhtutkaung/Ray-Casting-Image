#include "header.h"
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "Usage: ./rayTracing [config file] [output file]\n";
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
                Triangle *triangle = imgInfo.triangles.at(k);
                float tTemp = calTDistanceFromTriangle(rayDir, imgInfo.eye, triangle);
                if (tTemp >= 0)
                {
                    Vector3 rayIntersectionPoint = calRayIntersectObjPoint(rayDir, imgInfo.eye, tTemp);
                    Barycentric barycentricPointTemp = barycentricCalculation(triangle, rayIntersectionPoint);
                    if (barycentricPointTemp.a <= 1 && barycentricPointTemp.a >= 0 && barycentricPointTemp.b <= 1 && barycentricPointTemp.b >= 0 && barycentricPointTemp.r <= 1 && barycentricPointTemp.r >= 0 && (barycentricPointTemp.a + barycentricPointTemp.b + barycentricPointTemp.r) <= 1.001 && (barycentricPointTemp.a + barycentricPointTemp.b + barycentricPointTemp.r) >= 0)
                    {
                        // point is inside the triangle
                        float triangleDis = calDistanceFromRayEqu(rayDir, imgInfo.eye, tTemp);
                        if (minDis >= triangleDis)
                        {
                            minDis = triangleDis;
                            t = tTemp;
                            index = k;
                            barycentricPoint = barycentricPointTemp;
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
                    final = phongIllu(imgInfo.eye, imgInfo.lights, imgInfo.spheres, imgInfo.triangles, sphere->mtlcolor, sphere->texture, surfaceNormal, rayIntersectionPoint, imgInfo.viewDir, index, 's', objDif, imgInfo);
                }
                else if (shape == 't')
                {
                    Triangle *triangle = imgInfo.triangles.at(index);
                    Vector3 surfaceNormal;
                    if (triangle->smoothShading)
                    {
                        surfaceNormal = calTriangleSurfaceNormalSmooth(imgInfo.vertexNormals, triangle, barycentricPoint);
                    }
                    else
                    {
                        surfaceNormal = calTriangleSurfaceNormal(triangle);
                    }

                    RGB objDif;
                    if (triangle->textureApplied)
                    {
                        RGB originalFormat = calTriangleTextureCoordinate(triangle, barycentricPoint, triangle->texture, imgInfo.vertexTextureCoordinates);
                        objDif.r = originalFormat.r / 255.0;
                        objDif.g = originalFormat.g / 255.0;
                        objDif.b = originalFormat.b / 255.0;
                    }
                    else
                    {
                        objDif = triangle->mtlcolor.objDif;
                    }

                    final = phongIllu(imgInfo.eye, imgInfo.lights, imgInfo.spheres, imgInfo.triangles, triangle->mtlcolor, triangle->texture, surfaceNormal, rayIntersectionPoint, imgInfo.viewDir, index, 't', objDif, imgInfo);
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

    Vector3 surfaceNormal;
    Vector3 originPoint;
    Vector3 intersectPoint;
    originPoint.x = 2; originPoint.y = 3; originPoint.z = 5;
    intersectPoint.x = 3; intersectPoint.y = -1; intersectPoint.z = -3;
    surfaceNormal.x = 0; surfaceNormal.y = 1; surfaceNormal.z = 0;



    Vector3 incidenceRay = calIncidenceRay(originPoint, intersectPoint);
    printf("incidence ray: %f, %f, %f\n", incidenceRay.x, incidenceRay.y, incidenceRay.z);

    Vector3 refrection = calSpecularReflection(surfaceNormal, incidenceRay);
    printf("specularRefrection: %f, %f, %f\n", refrection.x,refrection.y,refrection.z);

    Vector3 transmittedRay = calTransmittedRay(1,1.2,surfaceNormal, incidenceRay);
    printf("transmittedRay: %f, %f, %f\n", transmittedRay.x, transmittedRay.y, transmittedRay.z);
    return 0;
}