#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <math.h>
#include <cmath>
#include <algorithm>
#define PI 3.14159265
#define DISTANCE 30
using namespace std;

#include <cstdlib>
#include <unistd.h>

typedef struct
{
    float a;
    float b;
    float r;
} Barycentric;

typedef struct
{
    float x;
    float y;
    float z;
} Vector3;

typedef struct
{
    Vector3 direction; // direction vector
    Vector3 hit;       // viewing window hit point
    float distance;    // distance from eye to the hit pixel point
} RayType;

typedef struct
{
    int width;
    int height;
} Size; // size of output image (pixel)

typedef struct
{
    float r;
    float b;
    float g;
} RBG;

typedef struct
{
    RBG objDif;
    RBG specHighlight;
    float ambient;
    float diffuse;
    float specular;
    float specExp;
} MTLcolor;

typedef struct
{
    Vector3 center;
    float radius;
    MTLcolor mtlcolor;
} Sphere;

typedef struct
{
    Vector3 v1;
    Vector3 v2;
    Vector3 v3;
    MTLcolor mtlcolor;
} Triangle;

typedef struct
{
    Vector3 ul;
    Vector3 ur;
    Vector3 ll;
    Vector3 lr;
} ViewWindow;

typedef struct
{
    Vector3 vec;
    RBG color;
    float choice; // 0 for point light source, 1 for directional light source
    Vector3 calculatedVec;
} Light;

typedef struct
{
    Vector3 eye;       // view origin aka eye position
    Vector3 viewDir;   // viewing direction
    Vector3 upDir;     // 'up' direction
    float vFov;        // vertical field of view (degrees)
    Size imgSize;      // size of output image (pixel)
    RBG bkgcolor;      // background color
    MTLcolor mtlcolor; // material color
    vector<Sphere> spheres;
    vector<Vector3> vertices;
    vector<Triangle> triangles;
    ViewWindow viewWindow;
    vector<Light> lights;
} Info;

// Vector3.cpp
Vector3 normalizeVector(Vector3 vec);
Vector3 crossProduct(Vector3 vecA, Vector3 vecB);
float dotProduct(Vector3 vecA, Vector3 vecB);
float distance(Vector3 a, Vector3 b);
float vectorLength(Vector3 vec);

// Calculation.cpp
Vector3 calV(Vector3 u, Vector3 viewDir);
Vector3 calU(Vector3 viewDir, Vector3 upDir);
float calHeightFromDegree(float degree, float distance);
float calWidthFromRatio(float aspect_ratio, float height);
float calAspectRatio(int widthPixel, int heightPixel);
Vector3 calViewCoordinate(string keyword, Vector3 eye, Vector3 u, Vector3 v, float distance, Vector3 viewDirNorm, float width, float height);
Vector3 convertPixelToCoor(ViewWindow viewWindow, Size imgSize, int xPixel, int yPixel);
Vector3 calRayDir(Vector3 eye, Vector3 plane);
float calTDistanceFromSphere(Vector3 direction, Vector3 currentPos, Sphere sphere);
float convertColor(float n);
Vector3 calSphereSurfaceNormal(Vector3 intersect, Sphere sphere);
Vector3 calTriangleSurfaceNormal(Triangle triangle);
Vector3 calLDir(Light light);
Vector3 calH(Vector3 L, Vector3 viewDir);
RBG phongIllu(vector<Light> lights, vector<Sphere> spheres, vector<Triangle> triangles, MTLcolor mtlcolor, Vector3 surfaceNormal, Vector3 intersectCoor, Vector3 viewDir, int curIndex, char shape);
Vector3 calRayIntersectObjPoint(Vector3 rayDir, Vector3 rayOrigin, float t);
float calDistanceFromRayEqu(Vector3 rayDir, Vector3 rayOrigin, float t);
float calTDistanceFromTriangle(Vector3 rayDir, Vector3 rayOrigin, Triangle triangle);
Barycentric barycentricCalculation(Triangle triangle, Vector3 p);

// storeData.cpp
void checkData(Info *imgInfo, string line);
Info readInfo(string fileName);
void storeEye(Info *imgInfo, vector<string> words);
void storeViewDir(Info *imgInfo, vector<string> words);
void storeUpDir(Info *imgInfo, vector<string> words);
void storeVfov(Info *imgInfo, vector<string> words);
void storeImgSize(Info *imgInfo, vector<string> words);
void storeBkgColor(Info *imgInfo, vector<string> words);
void storeMtlColor(Info *imgInfo, vector<string> words);
void storeSphere(Info *imgInfo, vector<string> words);
void storeLight(Info *imgInfo, vector<string> words);
void storeVertex(Info *imgInfo, vector<string> words);
void storeTriangles(Info *imgInfo, vector<string> words);