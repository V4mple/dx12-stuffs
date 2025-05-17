#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <DirectXMath.h>
#include "RenderDataTypes.h"

namespace Psycho
{
    class ObjectLoader
    {
    public:
        ObjectLoader() = default;
        ~ObjectLoader() = default;

        bool LoadObjFile(const std::string& filename);

        const std::vector<Vertex>& GetVertices() const { return vertices; }
        const std::vector<uint32_t>& GetIndices() const { return indices; }

    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };
}