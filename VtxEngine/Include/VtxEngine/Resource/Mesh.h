#pragma once  
#include <VtxEngine/Resource/Resource.h>
#include <VtxEngine/Graphics/VertexBuffer.h>  
#include <VtxEngine/Graphics/IndexBuffer.h>  
#include <VtxEngine/Math/Vector2D.h>
#include <VtxEngine/Math/Vector3D.h>
#include <vector>  
#include <VtxEngine/Math/VertexMesh.h>

namespace Vtx {  
    struct MaterialSlot  
    {  
        size_t startIndex = 0;  
        size_t numIndices = 0;  
        size_t materialID = 0;  
    };  

    class Mesh : public Resource  
    {  
    public:  
        Mesh(const wchar_t* fullPath, ResourceManager *manager);

        Mesh(VertexMesh* vertexData, unsigned int vertexListSize, 
            unsigned int* indexData, unsigned int indexListSize, 
            MaterialSlot* materialsList, unsigned int materialSlotListSize, ResourceManager* manager);

        ~Mesh();

        MaterialSlot GetMaterialSlots(UINT slot);  
        size_t GetNumMaterialSlots();  

    private:  
        void ComputeTangents(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2,  
                             const Vector2D& t0, const Vector2D& t1, const Vector2D& t2,  
                             Vector3D& tangent, Vector3D& binormal);  

        VertexBufferPtr m_vertexBuffer = nullptr;  
        IndexBufferPtr m_indexBuffer = nullptr;  
        std::vector<MaterialSlot> m_materialSlots;  

    private:  
        friend class GraphicsEngine;  
    };  
}
