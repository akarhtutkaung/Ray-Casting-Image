#include "header.h"

void displayError(string keyword)
{
    cout << "[!] Error: Insufficient description data (" << keyword << ").\n'";
    exit(EXIT_FAILURE);
}

void checkData(Info *imgInfo, string line)
{
    vector<string> words;
    string word;
    istringstream split(line);

    while (split >> word)
    {
        words.push_back(word);
    }

    string keyword = words.at(0);
    if (keyword == "eye")
    {
        if (words.size() < 4)
        {
            displayError(keyword);
        }
        else
        {
            storeEye(imgInfo, words);
        }
    }
    else if (keyword == "viewdir")
    {
        if (words.size() < 4)
        {
            displayError(keyword);
        }
        else
        {
            storeViewDir(imgInfo, words);
        }
    }
    else if (keyword == "updir")
    {
        if (words.size() < 4)
        {
            displayError(keyword);
        }
        else
        {
            storeUpDir(imgInfo, words);
        }
    }
    else if (keyword == "vfov")
    {
        if (words.size() < 2)
        {
            displayError(keyword);
        }
        else
        {
            storeVfov(imgInfo, words);
        }
    }
    else if (keyword == "imsize")
    {
        if (words.size() < 3)
        {
            displayError(keyword);
        }
        else
        {
            storeImgSize(imgInfo, words);
        }
    }
    else if (keyword == "bkgcolor")
    {
        if (words.size() < 4)
        {
            displayError(keyword);
        }
        else
        {
            storeBkgColor(imgInfo, words);
        }
    }
    else if (keyword == "mtlcolor")
    {
        if (words.size() < 11)
        {
            displayError(keyword);
        }
        else
        {
            storeMtlColor(imgInfo, words);
        }
    }
    else if (keyword == "sphere")
    {
        if (words.size() < 5)
        {
            displayError(keyword);
        }
        else
        {
            storeSphere(imgInfo, words);
        }
    }
    else if (keyword == "light")
    {
        if (words.size() < 8)
        {
            displayError(keyword);
        }
        else
        {
            storeLight(imgInfo, words);
        }
    }
    else if (keyword == "v")
    {
        if (words.size() < 4)
        {
            displayError(keyword);
        }
        else
        {
            storeVertex(imgInfo, words);
        }
    }
    else if (keyword == "f")
    {
        if (words.size() < 4)
        {
            displayError(keyword);
        }
        else
        {
            storeTriangles(imgInfo, words);
        }
    }
}

Info readInfo(string fileName)
{
    Info imgInfo;
    ifstream input_stream(fileName, ios::in);
    if (input_stream.is_open())
    {
        string line;
        while (getline(input_stream, line))
        {
            if (!line.empty())
            {
                checkData(&imgInfo, line);
            }
        }
    }
    else
    {
        cout << "[!] Error: File fail to open. \n";
        exit(EXIT_FAILURE);
    }
    return imgInfo;
}

void storeEye(Info *imgInfo, vector<string> words)
{
    imgInfo->eye.x = stof(words.at(1));
    imgInfo->eye.y = stof(words.at(2));
    imgInfo->eye.z = stof(words.at(3));
}

void storeViewDir(Info *imgInfo, vector<string> words)
{
    imgInfo->viewDir.x = stof(words.at(1));
    imgInfo->viewDir.y = stof(words.at(2));
    imgInfo->viewDir.z = stof(words.at(3));
}

void storeUpDir(Info *imgInfo, vector<string> words)
{
    imgInfo->upDir.x = stof(words.at(1));
    imgInfo->upDir.y = stof(words.at(2));
    imgInfo->upDir.z = stof(words.at(3));
}

void storeVfov(Info *imgInfo, vector<string> words)
{
    imgInfo->vFov = stof(words.at(1));
}

void storeImgSize(Info *imgInfo, vector<string> words)
{
    imgInfo->imgSize.width = stof(words.at(1));
    imgInfo->imgSize.height = stof(words.at(2));
}

void storeBkgColor(Info *imgInfo, vector<string> words)
{

    imgInfo->bkgcolor.r = convertColor(stof(words.at(1)));
    imgInfo->bkgcolor.g = convertColor(stof(words.at(2)));
    imgInfo->bkgcolor.b = convertColor(stof(words.at(3)));
}

void storeMtlColor(Info *imgInfo, vector<string> words)
{
    imgInfo->mtlcolor.objDif.r = (stof(words.at(1)));
    imgInfo->mtlcolor.objDif.g = (stof(words.at(2)));
    imgInfo->mtlcolor.objDif.b = (stof(words.at(3)));
    imgInfo->mtlcolor.specHighlight.r = (stof(words.at(4)));
    imgInfo->mtlcolor.specHighlight.g = (stof(words.at(5)));
    imgInfo->mtlcolor.specHighlight.b = (stof(words.at(6)));
    imgInfo->mtlcolor.ambient = (stof(words.at(7)));
    imgInfo->mtlcolor.diffuse = (stof(words.at(8)));
    imgInfo->mtlcolor.specular = (stof(words.at(9)));
    imgInfo->mtlcolor.specExp = (stof(words.at(10)));
}

void storeSphere(Info *imgInfo, vector<string> words)
{
    Sphere sphere;
    sphere.center.x = stof(words.at(1));
    sphere.center.y = stof(words.at(2));
    sphere.center.z = stof(words.at(3));
    sphere.radius = stof(words.at(4));
    sphere.mtlcolor = imgInfo->mtlcolor;
    imgInfo->spheres.push_back(sphere);
}

void storeLight(Info *imgInfo, vector<string> words)
{
    if (stof(words.at(4)) == 1.0 || stof(words.at(4)) == 0.0)
    {
        Light light;
        light.vec.x = stof(words.at(1));
        light.vec.y = stof(words.at(2));
        light.vec.z = stof(words.at(3));
        light.choice = stof(words.at(4));
        light.color.r = stof(words.at(5));
        light.color.g = stof(words.at(6));
        light.color.b = stof(words.at(7));

        if (light.choice == 0.0)
        {
            light.calculatedVec = calLDir(light);
        }

        imgInfo->lights.push_back(light);
    }
    else
    {
        cout << "[!] Error: Unknown light data [w].\n'";
        exit(EXIT_FAILURE);
    }
}

void storeVertex(Info *imgInfo, vector<string> words)
{
    Vector3 vertex;
    vertex.x = stof(words.at(1));
    vertex.y = stof(words.at(2));
    vertex.z = stof(words.at(3));
    imgInfo->vertices.push_back(vertex);
}

void storeTriangles(Info *imgInfo, vector<string> words)
{
    try
    {
        Triangle triangle;
        triangle.p0 = imgInfo->vertices[stoi(words.at(1)) - 1];
        triangle.p1 = imgInfo->vertices[stoi(words.at(2)) - 1];
        triangle.p2 = imgInfo->vertices[stoi(words.at(3)) - 1];
        triangle.mtlcolor = imgInfo->mtlcolor;
        imgInfo->triangles.push_back(triangle);
    }
    catch (const std::exception &)
    {
        displayError("f");
    }
}