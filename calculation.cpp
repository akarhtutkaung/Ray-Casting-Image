#include "header.h"

Vector3 calV(Vector3 u, Vector3 viewDir)
{
    Vector3 vPrime = crossProduct(u, viewDir);
    Vector3 vNorm = normalizeVector(vPrime);
    // cout << "[calV] v: (" << v.x << ", " << v.y << ", " << v.z << ")\n"; // Debug mode
    return vNorm;
}

Vector3 calU(Vector3 viewDir, Vector3 upDir)
{
    Vector3 uPrime = crossProduct(viewDir, upDir);
    Vector3 uNorm = normalizeVector(uPrime);
    // cout << "[calU] u: (" << u.x << ", " << u.y < ", " << u.z << ")\n"; // Debug mode
    return uNorm;
}

float calHeightFromDegree(float degree, float distance)
{
    float height = 2 * distance * tan((degree / 2) * PI / 180.0);
    // cout << "[calHeightFromDegree] Height: " << height << endl;   // Debug mode
    return height;
}

float calWidthFromRatio(float aspect_ratio, float height)
{
    float width = height * aspect_ratio;
    // cout << "[calWidthFromRatio] Width: << width << endl;         // Debug mode
    return width;
}

float calAspectRatio(int widthPixel, int heightPixel)
{
    float ratio = (float)widthPixel / (float)heightPixel;
    // cout << "[calAspectRatio] Aspect Ratio: << ratio << endl; // Debug mode
    return ratio;
}

Vector3 calViewCoordinate(string keyword, Vector3 eye, Vector3 u, Vector3 v, float distance, Vector3 viewDirNorm, float width, float height)
{
    Vector3 coordinate;
    if (keyword == "ul")
    {
        coordinate.x = eye.x + (distance * viewDirNorm.x) - (((float)width / 2.0f) * u.x) + (((float)height / 2.0f) * v.x);
        coordinate.y = eye.y + (distance * viewDirNorm.y) - (((float)width / 2.0f) * u.y) + (((float)height / 2.0f) * v.y);
        coordinate.z = eye.z + (distance * viewDirNorm.z) - (((float)width / 2.0f) * u.z) + (((float)height / 2.0f) * v.z);
    }
    else if (keyword == "ur")
    {
        coordinate.x = eye.x + (distance * viewDirNorm.x) + (((float)width / 2.0f) * u.x) + (((float)height / 2.0f) * v.x);
        coordinate.y = eye.y + (distance * viewDirNorm.y) + (((float)width / 2.0f) * u.y) + (((float)height / 2.0f) * v.y);
        coordinate.z = eye.z + (distance * viewDirNorm.z) + (((float)width / 2.0f) * u.z) + (((float)height / 2.0f) * v.z);
    }
    else if (keyword == "ll")
    {
        coordinate.x = eye.x + (distance * viewDirNorm.x) - (((float)width / 2.0f) * u.x) - (((float)height / 2.0f) * v.x);
        coordinate.y = eye.y + (distance * viewDirNorm.y) - (((float)width / 2.0f) * u.y) - (((float)height / 2.0f) * v.y);
        coordinate.z = eye.z + (distance * viewDirNorm.z) - (((float)width / 2.0f) * u.z) - (((float)height / 2.0f) * v.z);
    }
    else if (keyword == "lr")
    {
        coordinate.x = eye.x + (distance * viewDirNorm.x) + (((float)width / 2.0f) * u.x) - (((float)height / 2.0f) * v.x);
        coordinate.y = eye.y + (distance * viewDirNorm.y) + (((float)width / 2.0f) * u.y) - (((float)height / 2.0f) * v.y);
        coordinate.z = eye.z + (distance * viewDirNorm.z) + (((float)width / 2.0f) * u.z) - (((float)height / 2.0f) * v.z);
    }
    else
    {
        cout << "[!] Error: viewCoordinate calculation (" << keyword << ").\n";
        exit(EXIT_FAILURE);
    }
    // cout << "[calViewCoordinate]" << keyword << ": (" << coordinate.x << ", " << coordinate.y << ", "<< coordinate.z << ")\n"; // Debug mode

    return coordinate;
}

Vector3 convertPixelToCoor(ViewWindow viewWindow, Size imgSize, int xPixel, int yPixel)
{
    Vector3 ret;
    Vector3 ul = viewWindow.ul;
    Vector3 ur = viewWindow.ur;
    Vector3 ll = viewWindow.ll;
    float width = imgSize.width;
    float height = imgSize.height;
    Vector3 deltaH, deltav, deltaCH, deltacv;

    deltaH.x = (ur.x - ul.x) / width;
    deltaH.y = (ur.y - ul.y) / width;
    deltaH.z = (ur.z - ul.z) / width;

    deltav.x = (ll.x - ul.x) / height;
    deltav.y = (ll.y - ul.y) / height;
    deltav.z = (ll.z - ul.z) / height;

    deltaCH.x = (ur.x - ul.x) / (2.0f * width);
    deltaCH.y = (ur.y - ul.y) / (2.0f * width);
    deltaCH.z = (ur.z - ul.z) / (2.0f * width);

    deltacv.x = (ll.x - ul.x) / (2.0f * height);
    deltacv.y = (ll.y - ul.y) / (2.0f * height);
    deltacv.z = (ll.z - ul.z) / (2.0f * height);

    ret.x = ul.x + (xPixel * deltaH.x) + (yPixel * deltav.x) + deltaCH.x + deltacv.x;
    ret.y = ul.y + (xPixel * deltaH.y) + (yPixel * deltav.y) + deltaCH.y + deltacv.y;
    ret.z = ul.z + (xPixel * deltaH.z) + (yPixel * deltav.z) + deltaCH.z + deltacv.z;

    // cout << ret.x << endl;
    // cout << ret.y << endl;
    // cout << ret.z << endl;
    return ret;
}

Vector3 calRayDir(Vector3 eye, Vector3 plane)
{
    Vector3 ray;
    float unitVec = sqrt(pow(plane.x - eye.x, 2) + pow(plane.y - eye.y, 2) + pow(plane.z - eye.z, 2));
    ray.x = (plane.x - eye.x) / unitVec;
    ray.y = (plane.y - eye.y) / unitVec;
    ray.z = (plane.z - eye.z) / unitVec;

    // cout << ray.x << endl;
    // cout << ray.y << endl;
    // cout << ray.z << endl;

    return ray;
}

float calDistanceFromRayEqu(Vector3 rayDir, Vector3 rayOrigin, float t)
{
    Vector3 intersectPoint = calRayIntersectObjPoint(rayDir, rayOrigin, t);
    float dis = distance(rayOrigin, intersectPoint);
    return dis;
}

float calTDistanceFromSphere(Vector3 rayDir, Vector3 rayOrigin, Sphere sphere)
{
    Vector3 sphereCenter = sphere.center;
    float radius = sphere.radius;
    float a = 1;
    float b = 2 * (rayDir.x * (rayOrigin.x - sphereCenter.x) + rayDir.y * (rayOrigin.y - sphereCenter.y) + rayDir.z * (rayOrigin.z - sphereCenter.z));
    float c = pow(rayOrigin.x - sphereCenter.x, 2) + pow(rayOrigin.y - sphereCenter.y, 2) + pow(rayOrigin.z - sphereCenter.z, 2) - pow(radius, 2);
    float t = -1;
    float b24ac = pow(b, 2) - (4 * a * c);
    if (b24ac < 0)
    {
        // no solution aka is no intersection with the object
        return -1;
    }
    else if (b24ac > 0)
    {
        float temp1 = (-b + sqrt(b24ac)) / (2 * a);
        float temp2 = (-b - sqrt(b24ac)) / (2 * a);
        float temp3 = INFINITY;
        if (temp1 < temp2)
        {
            temp3 = temp1;
        }
        else
        {
            temp3 = temp2;
        }
        if (temp3 >= 0)
        {
            t = temp3;
        }
    }
    else
    {
        t = (-b + sqrt(b24ac)) / (2 * a);
    }
    return t;
}

float convertColor(float n)
{
    return n * 255;
}

Vector3 calSphereSurfaceNormal(Vector3 intersect, Sphere sphere)
{
    Vector3 N;
    N.x = (intersect.x - sphere.center.x) / sphere.radius;
    N.y = (intersect.y - sphere.center.y) / sphere.radius;
    N.z = (intersect.z - sphere.center.z) / sphere.radius;
    // N.x = (intersect.x - sphere.center.x);
    // N.y = (intersect.y - sphere.center.y);
    // N.z = (intersect.z - sphere.center.z);
    // cout << "z: " << intersect.z<< endl;
    return normalizeVector(N);
}

Vector3 calTriangleSurfaceNormal(Triangle triangle)
{
    Vector3 e1, e2;
    Vector3 p0 = triangle.v1, p1 = triangle.v2, p2 = triangle.v3;

    e1.x = p1.x - p0.x;
    e1.y = p1.y - p0.y;
    e1.z = p1.z - p0.z;

    e2.x = p2.x - p0.x;
    e2.y = p2.y - p0.y;
    e2.z = p2.z - p0.z;

    Vector3 surfaceNormal = crossProduct(e1, e2);
    return normalizeVector(surfaceNormal);
}

Vector3 calLDir(Light light)
{
    // directional lights
    Vector3 temp;
    temp.x = -light.vec.x;
    temp.y = -light.vec.y;
    temp.z = -light.vec.z;
    if (temp.x == -0)
    {
        temp.x = 0.0;
    }
    if (temp.y == -0)
    {
        temp.y = 0.0;
    }
    if (temp.z == -0)
    {
        temp.z = 0.0;
    }
    return normalizeVector(temp);
}

Vector3 calLPoint(Light light, Vector3 intersectCoor)
{
    // point lights
    Vector3 temp;
    temp.x = light.vec.x - intersectCoor.x;
    temp.y = light.vec.y - intersectCoor.y;
    temp.z = light.vec.z - intersectCoor.z;
    return normalizeVector(temp);
}

Vector3 calH(Vector3 L, Vector3 viewDir)
{
    Vector3 temp;
    temp.x = (L.x - viewDir.x);
    temp.y = (L.y - viewDir.y);
    temp.z = (L.z - viewDir.z);
    // printf("L: %f, %f, %f\n", L.x, L.y, L.z);
    // printf("Viewdir: %f, %f, %f\n", viewDir.x, viewDir.y, viewDir.z);
    // printf("H: %f, %f, %f\n", temp.x, temp.y, temp.z);
    return normalizeVector(temp);
}

float checkShadow(vector<Sphere> spheres, vector<Triangle> triangles, Vector3 L, float lightPosDis, Vector3 intersectCoor, int curIndex, char shape)
{
    float shadow = 1.0; // nothing is blocking initially

    for (int i = 0; i < spheres.size(); i++)
    {
        if (i == curIndex && shape == 's')
        {
            continue;
        }
        float t = calTDistanceFromSphere(L, intersectCoor, spheres.at(i));
        if (t >= 0)
        {
            float dis = calDistanceFromRayEqu(L, intersectCoor, t);
            if (dis < lightPosDis)
            { // if the distance between the blocking object is less than the distance between the light point
                // something is blocking
                shadow = 0.0;
                return shadow;
            }
        }
    }
    for (int i = 0; i < triangles.size(); i++)
    {
        if (i == curIndex && shape == 't')
        {
            continue;
        }
        Triangle triangle = triangles.at(i);
        float t = calTDistanceFromTriangle(L, intersectCoor, triangles.at(i));
        if (t >= 0)
        {
            Vector3 rayIntersectionPoint = calRayIntersectObjPoint(L, intersectCoor, t);
            Barycentric barycentricPoint = barycentricCalculation(triangle, rayIntersectionPoint);

            if (barycentricPoint.a <= 1 && barycentricPoint.a >= 0 && barycentricPoint.b <= 1 && barycentricPoint.b >= 0 && barycentricPoint.r <= 1.0 && barycentricPoint.r >= 0 && (barycentricPoint.a + barycentricPoint.b + barycentricPoint.r) == 1.00)
            {
                // something is blocking

                // check if that blocking is beyond the light point
                float triangleDis = calDistanceFromRayEqu(L, rayIntersectionPoint, t);
                if (triangleDis < lightPosDis)
                { // if the distance between the blocking object is less than the distance between the light point
                    // something is blocking
                    shadow = 0.0;
                    return shadow;
                }
            }
        }
    }
    return shadow;
}

// RBG phongIllu(vector<Light> lights, Vector3 intersectCoor, Vector3 rayDir, vector<Sphere> spheres, Vector3 viewDir, int curSphIndex)
RBG phongIllu(vector<Light> lights, vector<Sphere> spheres, vector<Triangle> triangles, MTLcolor mtlcolor, Vector3 surfaceNormal, Vector3 intersectCoor, Vector3 viewDir, int curIndex, char shape)
{
    Vector3 L, H;
    RBG sum;
    sum.r = 0.0f;
    sum.b = 0.0f;
    sum.g = 0.0f;
    float I = 1.0f / (float)lights.size();
    float lightPosDis = INFINITY;

    for (int i = 0; i < lights.size(); i++)
    {
        if (lights.at(i).choice == 1.0)
        { // point light source
            L = calLPoint(lights.at(i), intersectCoor);
            lightPosDis = distance(L, intersectCoor);
        }
        else
        { // direction light
            L = lights.at(i).calculatedVec;
        }

        // Find H
        H = calH(L, viewDir);

        // calculate shadow
        // send out ray from current surface toward the light
        float shadow = checkShadow(spheres, triangles, L, lightPosDis, intersectCoor, curIndex, shape);
        // float shadow = 0.0;
        RBG diffuse, specular;

        diffuse.r = (mtlcolor.diffuse * mtlcolor.objDif.r * max((float)0, dotProduct(surfaceNormal, L)));
        diffuse.g = (mtlcolor.diffuse * mtlcolor.objDif.g * max((float)0, dotProduct(surfaceNormal, L)));
        diffuse.b = (mtlcolor.diffuse * mtlcolor.objDif.b * max((float)0, dotProduct(surfaceNormal, L)));

        specular.r = I * lights.at(i).color.r * (mtlcolor.specular * mtlcolor.specHighlight.r * pow(max((float)0, dotProduct(surfaceNormal, H)), mtlcolor.specExp));
        specular.g = I * lights.at(i).color.g * (mtlcolor.specular * mtlcolor.specHighlight.g * pow(max((float)0, dotProduct(surfaceNormal, H)), mtlcolor.specExp));
        specular.b = I * lights.at(i).color.b * (mtlcolor.specular * mtlcolor.specHighlight.b * pow(max((float)0, dotProduct(surfaceNormal, H)), mtlcolor.specExp));

        sum.r += shadow * (diffuse.r + specular.r);
        sum.g += shadow * (diffuse.g + specular.g);
        sum.b += shadow * (diffuse.b + specular.b);
    }
    // return sum;
    RBG ret;
    ret.r = (mtlcolor.ambient * mtlcolor.objDif.r) + sum.r;
    ret.g = (mtlcolor.ambient * mtlcolor.objDif.g) + sum.g;
    ret.b = (mtlcolor.ambient * mtlcolor.objDif.b) + sum.b;
    return ret;
}

Vector3 calRayIntersectObjPoint(Vector3 rayDir, Vector3 rayOrigin, float t)
{
    Vector3 intersectPoint;
    intersectPoint.x = rayOrigin.x + t * rayDir.x;
    intersectPoint.y = rayOrigin.y + t * rayDir.y;
    intersectPoint.z = rayOrigin.z + t * rayDir.z;
    return intersectPoint;
}

float calTDistanceFromTriangle(Vector3 rayDir, Vector3 rayOrigin, Triangle triangle)
{
    Vector3 e1, e2, n;
    float D, t = -1;
    Vector3 p0 = triangle.v1, p1 = triangle.v2, p2 = triangle.v3;

    e1.x = p1.x - p0.x;
    e1.y = p1.y - p0.y;
    e1.z = p1.z - p0.z;

    e2.x = p2.x - p0.x;
    e2.y = p2.y - p0.y;
    e2.z = p2.z - p0.z;

    n = crossProduct(e1, e2);
    // Vector3 n = calTriangleSurfaceNormal(triangle);
    // n's xyz is same with ABC

    float A = n.x, B = n.y, C = n.z;
    // the plane equation is, x+y+z+D = 0
    float denominator = (A * rayDir.x) + (B * rayDir.y) + (C * rayDir.z);
    // cout<<denominator<<endl;
    // sleep(5);
    // cout<<denominator<<endl;
    if (denominator == 0.0)
    {
        // no intersection
        return -1;
    }
    else
    {
        D = -((A * p0.x) + (B * p0.y) + (C * p0.z));
        t = -(D + (A * rayOrigin.x) + (B * rayOrigin.y) + (C * rayOrigin.z)) / denominator;
        // if t > 0, the intersection is ahead the ray origin
        // if t < 0, the intersection is behind the ray origin
        if (t < 0)
        {
            return -1;
        }
        else
        {
            return t;
        }
    }
}

Barycentric barycentricCalculation(Triangle triangle, Vector3 p)
{
    Vector3 e1, e2, e3, e4;
    Vector3 p0 = triangle.v1, p1 = triangle.v2, p2 = triangle.v3;

    e1.x = p1.x - p0.x;
    e1.y = p1.y - p0.y;
    e1.z = p1.z - p0.z;

    e2.x = p2.x - p0.x;
    e2.y = p2.y - p0.y;
    e2.z = p2.z - p0.z;

    e3.x = p.x - p1.x;
    e3.y = p.y - p1.y;
    e3.z = p.z - p1.z;

    e4.x = p.x - p2.x;
    e4.y = p.y - p2.y;
    e4.z = p.z - p2.z;

    float area, a, b, c;
    area = (vectorLength(crossProduct(e1, e2))) / 2.0f;

    a = (vectorLength(crossProduct(e3, e4))) / 2.0f;
    b = (vectorLength(crossProduct(e4, e2))) / 2.0f;
    c = (vectorLength(crossProduct(e1, e3))) / 2.0f;
    // area = a+b+c;

    Barycentric ret;
    ret.a = a / area;
    ret.b = b / area;
    ret.r = c / area;
    return ret;
}
// if alpha > 1 or < 0 then it is outside
// if beta > 1 or < 0 then it is outside
// if r > 1 or < 0 then it is outside
// if beta + r + alpha > 1 or < 0then it is outside