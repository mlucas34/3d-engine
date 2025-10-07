#include <VtxEngine/Resource/Mesh.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>
#include <VtxEngine/Math/VertexMesh.h>
#include <VtxEngine/Resource/ResourceManager.h>
#include <VtxEngine/Game/Game.h>
#include <VtxEngine/Graphics/RenderSystem.h>
#include <filesystem>

namespace Vtx {
	Mesh::Mesh(const wchar_t* fullPath, ResourceManager* manager) : Resource(fullPath, manager)
	{
		tinyobj::attrib_t attribs;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warning;
		std::string err;

		std::string inputFile = std::filesystem::path(fullPath).string();

		std::string mtldir = inputFile.substr(0, inputFile.find_last_of("\\/"));

		bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warning, &err, inputFile.c_str(), mtldir.c_str());

		if (!err.empty())
		{
			VTXError("Mesh not created Successfully\n");
		}

		if (!res) { VTXError("Mesh not created Successfully\n"); }
		else {
			std::cout << "Mesh Created Successfully\n";
		}

		std::vector<VertexMesh> verticesList;
		std::vector<unsigned int> indicesList; // might want to change to signed

		size_t vertexIndexListSize = 0;

		for (size_t s = 0; s < shapes.size(); ++s) {
			vertexIndexListSize += shapes[s].mesh.indices.size();
		}

		verticesList.reserve(vertexIndexListSize);
		indicesList.reserve(vertexIndexListSize);

		m_materialSlots.resize(materials.size());

		size_t globalIdxOffset = 0;


		for (size_t material = 0; material < materials.size(); ++material) {

			m_materialSlots[material].startIndex = globalIdxOffset;
			m_materialSlots[material].materialID = material;

			const float scale = 1.0f;

			// loop through all the shapes in the vector
			for (size_t s = 0; s < shapes.size(); ++s)
			{
				size_t idxOffset = 0;

				// loop through all the faces of each shape
				for (size_t face = 0; face < shapes[s].mesh.num_face_vertices.size(); ++face)
				{
					unsigned char numFaceVertices = shapes[s].mesh.num_face_vertices[face];

					if (shapes[s].mesh.material_ids[face] != material) 
					{
						idxOffset += numFaceVertices;
						continue;
					}


					Vector3D verticesFaces[3];
					Vector2D texcoordFaces[3];

					for (unsigned char v = 0; v < numFaceVertices; ++v)
					{
						tinyobj::index_t index = shapes[s].mesh.indices[idxOffset + v];
						tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0] * scale;
						tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1] * scale;
						tinyobj::real_t vz = -attribs.vertices[index.vertex_index * 3 + 2] * scale;

						// text coordniate for textures
						tinyobj::real_t tx = 0;
						tinyobj::real_t ty = 0;

						if (attribs.texcoords.size())
						{
							tx = attribs.texcoords[index.texcoord_index * 2 + 0];
							ty = 1.0f - attribs.texcoords[index.texcoord_index * 2 + 1];
						}
						verticesFaces[v] = Vector3D(vx, vy, vz);
						texcoordFaces[v] = Vector2D(tx, ty);
					}

					Vector3D tangent, binormal;

					ComputeTangents(verticesFaces[0], verticesFaces[1], verticesFaces[2],
									texcoordFaces[0], texcoordFaces[1], texcoordFaces[2], 
									tangent, binormal);

					// loop through all the vertices of each face
					for (unsigned char v = 0; v < numFaceVertices; ++v)
					{
						// these represent the vertices and texcoords	
						tinyobj::index_t index = shapes[s].mesh.indices[idxOffset + v];
						tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0] * scale;
						tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1] * scale;
						tinyobj::real_t vz = -attribs.vertices[index.vertex_index * 3 + 2] * scale;

						// text coordniate for textures
						tinyobj::real_t tx = 0;
						tinyobj::real_t ty = 0;

						if (attribs.texcoords.size())
						{
							tx = attribs.texcoords[index.texcoord_index * 2 + 0];
							ty = 1.0f - attribs.texcoords[index.texcoord_index * 2 + 1];
						}


						// normal coordinates for lighting
						tinyobj::real_t nx = 0;
						tinyobj::real_t ny = 0;
						tinyobj::real_t nz = 0;

						if (attribs.normals.size()) 
						{
							nx = attribs.normals[index.normal_index * 3 + 0] * scale;
							ny = attribs.normals[index.normal_index * 3 + 1] * scale;
							nz = -attribs.normals[index.normal_index * 3 + 2] * scale;
						}

						Vector3D v_tangent, v_binormal;

						v_binormal = Vector3D::cross(Vector3D(nx, ny, nz), tangent);
						v_tangent = Vector3D::cross(v_binormal, Vector3D(nx, ny, nz));

						VertexMesh vertex((Vector3D(vx, vy, vz)), Vector2D(tx, ty), Vector3D(nx, ny, nz), v_tangent, v_binormal);
						verticesList.emplace_back(vertex);
						indicesList.emplace_back(globalIdxOffset + v);

					}

					idxOffset += numFaceVertices;
					globalIdxOffset += numFaceVertices;
				}
			}

			m_materialSlots[material].numIndices = globalIdxOffset - m_materialSlots[material].startIndex;
		}

		RenderSystem *renderSys = m_manager->GetGame()->GetGraphicsEngine()->GetRenderSystem();	

		m_vertexBuffer = renderSys->CreateVertexBuffer(&verticesList[0], sizeof(VertexMesh), (UINT)verticesList.size());
		m_indexBuffer = renderSys->CreateIndexBuffer(&indicesList[0], (UINT)indicesList.size());
	}

	Mesh::Mesh(VertexMesh* vertexData, unsigned int vertexListSize, unsigned int* indexData, unsigned int indexListSize, MaterialSlot* materialsList, unsigned int materialSlotListSize, ResourceManager* manager) : Resource(L"", manager)
	{
		RenderSystem* renderSys = m_manager->GetGame()->GetGraphicsEngine()->GetRenderSystem();

		m_vertexBuffer = renderSys->CreateVertexBuffer(vertexData, sizeof(VertexMesh), vertexListSize);
		m_indexBuffer = renderSys->CreateIndexBuffer(indexData, indexListSize);

		m_materialSlots.resize(materialSlotListSize);
		
		for (unsigned int i = 0; i < materialSlotListSize; ++i) {
			m_materialSlots[i] = materialsList[i];
		}
	}

	Mesh::~Mesh()
	{
	}

	MaterialSlot Mesh::GetMaterialSlots(UINT slot)
	{
		if (slot >= m_materialSlots.size()) MaterialSlot();
		return m_materialSlots[slot];
	}

	size_t Mesh::GetNumMaterialSlots()
	{
		return m_materialSlots.size();
	}

	void Mesh::ComputeTangents(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2, 
							   const Vector2D& t0, const Vector2D& t1, const Vector2D& t2, 
							   Vector3D& tangent, Vector3D& binormal)
	{
		Vector3D deltaPos1 = v1 - v0;
		Vector3D deltaPos2 = v2 - v0;

		Vector2D deltaUV1 = t1 - t0;
		Vector2D deltaUV2 = t2 - t0;

		float r = 1 / (deltaUV1.m_x * deltaUV2.m_y - deltaUV2.m_y - deltaUV1.m_x);

		tangent = ((deltaPos1 * deltaUV2.m_y) - (deltaPos2 * deltaUV1.m_y));
		tangent = Vector3D::normalize(tangent);

		binormal = ((deltaPos2 * deltaUV1.m_x) - (deltaPos1 * deltaUV2.m_x));
		binormal = Vector3D::normalize(binormal);
	}

}
