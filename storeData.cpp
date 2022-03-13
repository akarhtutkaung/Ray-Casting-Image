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
    else if (keyword == "vn")
    {
        if (words.size() < 4)
        {
            displayError(keyword);
        }
        else
        {
            storeVertexNormalVectors(imgInfo, words);
        }
    }
    else if (keyword == "texture")
    {
        if (words.size() < 2)
        {
            displayError(keyword);
        }
        else
        {
            storeTextureImage(imgInfo, words);
        }
    }
    else if (keyword == "vt")
    {
        if (words.size() < 3)
        {
            displayError(keyword);
        }
        else
        {
            storeTextureCoordinates(imgInfo, words);
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
    Sphere *sphere = new Sphere();
    sphere->center.x = stof(words.at(1));
    sphere->center.y = stof(words.at(2));
    sphere->center.z = stof(words.at(3));
    sphere->radius = stof(words.at(4));
    sphere->mtlcolor = imgInfo->mtlcolor;
    if (imgInfo->textureApplied)
    {
        sphere->texture = imgInfo->texture;
        sphere->textureApplied = true;
    }
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
        if (words.at(1).find("//") != std::string::npos)
        {
            string delim = "//";
            // [f   v1//vn1    v2//vn2    v3//vn3]
            if (words.at(2).find(delim) != std::string::npos && words.at(3).find(delim) != std::string::npos)
            {
                vector<string> finalWords;
                for (int i = 1; i <= 3; i++)
                {
                    size_t pos = 0;
                    string token1;
                    while ((pos = words.at(i).find(delim)) != std::string::npos)
                    {
                        token1 = words.at(i).substr(0, pos);
                        finalWords.push_back(token1);
                        words.at(i).erase(0, pos + delim.length());
                        finalWords.push_back(words.at(i));
                    }
                }
        // triangle.v1Index = stoi(finalWords.at(0)) - 1;
        // triangle.v2Index = stoi(finalWords.at(2)) - 1;
        // triangle.v3Index = stoi(finalWords.at(4)) - 1;
                triangle.v1 = imgInfo->vertices[stoi(finalWords.at(0)) - 1];
                triangle.vn1 = stoi(finalWords.at(1));
                triangle.v2 = imgInfo->vertices[stoi(finalWords.at(2)) - 1];
                triangle.vn2 = stoi(finalWords.at(3));
                triangle.v3 = imgInfo->vertices[stoi(finalWords.at(4)) - 1];
                triangle.vn3 = stoi(finalWords.at(5));
                triangle.smoothShading = true;
                if (triangle.vn1 > imgInfo->vertexNormals.size() || triangle.vn2 > imgInfo->vertexNormals.size() || triangle.vn3 > imgInfo->vertexNormals.size())
                {
                    displayError("Normal Direction");
                }
            }
            else
            {
                displayError("f");
            }
        }
        else if (words.at(1).find("/") != std::string::npos)
        {
            string delim = "/";
            size_t pos = 0;
            string token1;
            int count = 0;
            vector<string> finalWords;
            string temp = words.at(1);
            while ((pos = temp.find(delim)) != std::string::npos)
            {
                temp.erase(0, pos + delim.length());
                count++;
            }
            if (count == 1)
            {
                // [f   v1/vt1   v2/vt2   v3/vt3]
                if (words.at(2).find(delim) != std::string::npos && words.at(3).find(delim) != std::string::npos)
                {
                    vector<string> finalWords;
                    for (int i = 1; i <= 3; i++)
                    {
                        size_t pos = 0;
                        string token1;
                        while ((pos = words.at(i).find(delim)) != std::string::npos)
                        {
                            token1 = words.at(i).substr(0, pos);
                            finalWords.push_back(token1);
                            words.at(i).erase(0, pos + delim.length());
                            finalWords.push_back(words.at(i));
                        }
                    }
                    triangle.v1 = imgInfo->vertices[stoi(finalWords.at(0)) - 1];
                    triangle.vt1 = stoi(finalWords.at(1));
                    triangle.v2 = imgInfo->vertices[stoi(finalWords.at(2)) - 1];
                    triangle.vt2 = stoi(finalWords.at(3));
                    triangle.v3 = imgInfo->vertices[stoi(finalWords.at(4)) - 1];
                    triangle.vt3 = stoi(finalWords.at(5));

        // triangle.v1Index = stoi(finalWords.at(0)) - 1;
        // triangle.v2Index = stoi(finalWords.at(2)) - 1;
        // triangle.v3Index = stoi(finalWords.at(4)) - 1;
                    if (triangle.vt1 > imgInfo->vertexTextureCoordinates.size() || triangle.vt2 > imgInfo->vertexTextureCoordinates.size() || triangle.vt3 > imgInfo->vertexTextureCoordinates.size())
                    {
                        displayError("Texture coordinate");
                    }
                }
                else
                {
                    displayError("f");
                }
            }
            else if (count == 2)
            {
                // [f   v1/vt1/vn1     v2/vt2/vn2   v3/vt3/vn3]
                if (words.at(2).find(delim) != std::string::npos && words.at(3).find(delim) != std::string::npos)
                {
                    vector<string> finalWords;
                    for (int i = 1; i <= 3; i++)
                    {
                        size_t pos = 0;
                        string token1;
                        while ((pos = words.at(i).find(delim)) != std::string::npos)
                        {
                            token1 = words.at(i).substr(0, pos);
                            finalWords.push_back(token1);
                            words.at(i).erase(0, pos + delim.length());
                            finalWords.push_back(words.at(i));
                        }
                    }
                    triangle.v1 = imgInfo->vertices[stoi(finalWords.at(0)) - 1];
                    triangle.vt1 = stoi(finalWords.at(1));
                    triangle.vn1 = stoi(finalWords.at(2));
                    triangle.v2 = imgInfo->vertices[stoi(finalWords.at(3)) - 1];
                    triangle.vt2 = stoi(finalWords.at(4));
                    triangle.vn2 = stoi(finalWords.at(5));
                    triangle.v3 = imgInfo->vertices[stoi(finalWords.at(6)) - 1];
                    triangle.vt3 = stoi(finalWords.at(7));
                    triangle.vn3 = stoi(finalWords.at(8));
        // triangle.v1Index = stoi(finalWords.at(0)) - 1;
        // triangle.v2Index = stoi(finalWords.at(3)) - 1;
        // triangle.v3Index = stoi(finalWords.at(6)) - 1;
                    triangle.smoothShading = true;
                    if (triangle.vt1 > imgInfo->vertexTextureCoordinates.size() || triangle.vt2 > imgInfo->vertexTextureCoordinates.size() || triangle.vt3 > imgInfo->vertexTextureCoordinates.size())
                    {
                        displayError("Texture coordinate");
                    }
                    else if (triangle.vn1 > imgInfo->vertexNormals.size() || triangle.vn2 > imgInfo->vertexNormals.size() || triangle.vn3 > imgInfo->vertexNormals.size())
                    {
                        displayError("Normal Direction");
                    }
                }
                else
                {
                    displayError("f");
                }
            }
        }
        else
        {   
            triangle.v1 = imgInfo->vertices[stoi(words.at(1)) - 1];
            triangle.v2 = imgInfo->vertices[stoi(words.at(2)) - 1];
            triangle.v3 = imgInfo->vertices[stoi(words.at(3)) - 1];
            // triangle.v1Index = stoi(words.at(1)) - 1;
            // triangle.v2Index = stoi(words.at(2)) - 1;
            // triangle.v3Index = stoi(words.at(3)) - 1;
            
        }
        triangle.mtlcolor = imgInfo->mtlcolor;
        if (imgInfo->textureApplied)
        {
            triangle.texture = imgInfo->texture;
            triangle.textureApplied = true;
        }
        imgInfo->triangles.push_back(triangle);
    }
    catch (const std::exception &)
    {
        // catch exception if the vertices does not exists
        displayError("f");
    }
}

void storeVertexNormalVectors(Info *imgInfo, vector<string> words)
{
    Vector3 vertexNormal;
    vertexNormal.x = stof(words.at(1));
    vertexNormal.y = stof(words.at(2));
    vertexNormal.z = stof(words.at(3));
    imgInfo->vertexNormals.push_back(vertexNormal);
}

void storeTextureImage(Info *imgInfo, vector<string> words)
{
    string fileName = words.at(1);
    ifstream input_stream(fileName, ios::in);

    if (input_stream.is_open())
    {
        string line;
        vector<RGB> aLine;
        Texture *textures = new Texture();
        RGB texture;
        int currentLine = 0;
        int width = 0;
        int height = 0;
        int count = 1;

        while (getline(input_stream, line))
        {
            if (!line.empty())
            {
                vector<string> tokens;
                string word;
                istringstream split(line);

                while (split >> word)
                {
                    tokens.push_back(word);
                }

                if (currentLine == 1)
                {
                    textures->width = stoi(tokens.at(0));
                    textures->height = stoi(tokens.at(1));
                }
                if (currentLine >= 3)
                {
                    for (int i = 0; i < tokens.size(); i++)
                    {
                        if (count == 1)
                        {
                            texture.r = stoi(tokens.at(i));
                            count++;
                        }
                        else if (count == 2)
                        {
                            texture.g = stoi(tokens.at(i));
                            count++;
                        }
                        else
                        {
                            texture.b = stoi(tokens.at(i));
                            count = 1;
                            aLine.push_back(texture); // width
                            width++;
                        }
                        if (width == textures->width)
                        {
                            textures->textures.push_back(aLine); // height
                            aLine.clear();
                            height++;
                            width = 0;
                        }
                    }
                }
                currentLine++;
            }
        }
        imgInfo->texture = textures;
        imgInfo->textureApplied = true;
        input_stream.close();
    }
    else
    {
        printf("[!] Error: Texture file (%s) fail to open. \n", fileName.c_str());
        exit(EXIT_FAILURE);
    }
    // string outputfile = "test.ppm";
    // ofstream output_stream(outputfile, ios::out);
    // output_stream << "P3\n"
    //               << "512"
    //               << " "
    //               << "256"
    //               << "\n"
    //               << 255 << "\n";

    // for (uint32_t row = 0; row < imgInfo->texture.textures.size(); row++)
    // {
    //     for (uint32_t column = 0; column < imgInfo->texture.textures.at(row).size(); column++)
    //     {
    //         output_stream << imgInfo->texture.textures.at(row).at(column).r << " " << imgInfo->texture.textures.at(row).at(column).g << " " << imgInfo->texture.textures.at(row).at(column).b << "\n";
    //     }
    // }
    // output_stream.close();
}

void storeTextureCoordinates(Info *imgInfo, vector<string> words)
{
    TextureCoordinate vt;
    vt.u = stof(words.at(1));
    vt.v = stof(words.at(2));
    imgInfo->vertexTextureCoordinates.push_back(vt);
}