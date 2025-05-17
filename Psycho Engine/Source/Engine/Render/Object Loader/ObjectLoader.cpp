#include "pch.h"
#include "ObjectLoader.h"
#include <iostream>

namespace Psycho
{
    bool ObjectLoader::LoadObjFile(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Failed to open OBJ file: " << filename << std::endl;
            return false;
        }

        std::vector<DirectX::XMFLOAT3> positions;
        std::vector<DirectX::XMFLOAT3> normals;
        std::vector<DirectX::XMFLOAT2> texCoords;

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "v")
            {
                DirectX::XMFLOAT3 pos;
                ss >> pos.x >> pos.y >> pos.z;
                positions.push_back(pos);
            }
            else if (prefix == "vt")
            {
                DirectX::XMFLOAT2 tex;
                ss >> tex.x >> tex.y;
                texCoords.push_back(tex);
            }
            else if (prefix == "vn")
            {
                DirectX::XMFLOAT3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
            else if (prefix == "f")
            {
                uint32_t vIdx[3] = { 0 }, tIdx[3] = { 0 }, nIdx[3] = { 0 };
                char slash;

                for (int i = 0; i < 3; ++i)
                {
                    ss >> vIdx[i];
                    if (ss.peek() == '/') {
                        ss.ignore();
                        if (ss.peek() != '/') ss >> tIdx[i];
                        if (ss.peek() == '/') {
                            ss.ignore();
                            ss >> nIdx[i];
                        }
                    }

                    // Convert to zero-based index
                    vIdx[i]--;
                    if (tIdx[i] > 0) tIdx[i]--;
                    if (nIdx[i] > 0) nIdx[i]--;

                    // Validate indices
                    if (vIdx[i] >= positions.size() || vIdx[i] < 0)
                    {
                        std::cerr << "Invalid vertex index: " << vIdx[i] << std::endl;
                        return false;
                    }
                }
                DirectX::XMFLOAT4 rainbowColors[] = {
    {1.0f, 0.0f, 0.0f, 1.0f}, // Red
    {1.0f, 0.5f, 0.0f, 1.0f}, // Orange
    {1.0f, 1.0f, 0.0f, 1.0f}, // Yellow
    {0.0f, 1.0f, 0.0f, 1.0f}, // Green
    {0.0f, 0.0f, 1.0f, 1.0f}, // Blue
    {0.29f, 0.0f, 0.51f, 1.0f}, // Indigo
    {0.93f, 0.51f, 0.93f, 1.0f} // Violet
                };

                srand(static_cast<unsigned int>(time(NULL)));
                // Store vertices
                for (int i = 0; i < 3; ++i)
                {
                    Vertex vertex;
                    vertex.position = positions[vIdx[i]];
                    int random = rand() % 7;
                    vertex.color = rainbowColors[random];
                    vertices.push_back(vertex);
                    indices.push_back(static_cast<uint32_t>(indices.size()));
                }
            }
        }

        file.close();
        return true;
    }
}