#include "header.h"

Vector3 normalizeVector(Vector3 vec)
{
    Vector3 normV;
    float length = vectorLength(vec);
    if (length == 0)
    {
        return vec;
    }
    else
    {
        normV.x = vec.x / length;
        normV.y = vec.y / length;
        normV.z = vec.z / length;
    }
    return normV;
}
Vector3 crossProduct(Vector3 vecA, Vector3 vecB)
{
    Vector3 res;
    res.x = (vecA.y * vecB.z) - (vecA.z * vecB.y);
    res.y = (vecA.z * vecB.x) - (vecA.x * vecB.z);
    res.z = (vecA.x * vecB.y) - (vecA.y * vecB.x);

    if (res.x == -0)
    {
        res.x = 0;
    }
    if (res.y == -0)
    {
        res.y = 0;
    }
    if (res.z == -0)
    {
        res.z = 0;
    }
    return res;
}

float dotProduct(Vector3 vecA, Vector3 vecB)
{
    return (vecA.x * vecB.x) + (vecA.y * vecB.y) + (vecA.z * vecB.z);
}

float distance(Vector3 a, Vector3 b)
{
    float dis = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
    return dis;
}

float vectorLength(Vector3 vec)
{
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}