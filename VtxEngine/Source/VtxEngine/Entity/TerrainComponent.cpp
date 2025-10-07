#include <VtxEngine/Entity/TerrainComponent.h>
#include <VtxEngine/Entity/TransformComponent.h>
#include <VtxEngine/Entity/Entity.h>
#include <VtxEngine/Game/World.h>
#include <VtxEngine/Game/Game.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>
#include <VtxEngine/Graphics/RenderSystem.h>
#include <VtxEngine/Graphics/ConstantBuffer.h>
#include <VtxEngine/Math/VertexMesh.h>
#include <VtxEngine/Math/Functions.h>
#include <VtxEngine/Resource/Texture.h>

namespace Vtx
{
	TerrainComponent::TerrainComponent()
	{
	}

	TerrainComponent::~TerrainComponent()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
	}

	void TerrainComponent::SetHeightMap(const TexturePtr heightMap)
	{
		m_heightMap = heightMap;
	}

	const TexturePtr &TerrainComponent::GetHeightMap()
	{
		return m_heightMap;
	}

	void TerrainComponent::SetGroundMap(const TexturePtr groundMap)
	{
		m_groundMap = groundMap;
	}

	const TexturePtr &TerrainComponent::GetGroundMap()
	{
		return m_groundMap;
	}

	void TerrainComponent::SetRockMap(const TexturePtr rockMap)
	{
		m_rockMap = rockMap;
	}

	const TexturePtr &TerrainComponent::GetRockMap()
	{
		return m_rockMap;
	}

	void TerrainComponent::SetSize(const Vector3D &size)
	{
		m_size = size;
	}

	Vector3D TerrainComponent::GetSize()
	{
		return m_size;
	}

	float TerrainComponent::GetHeightFromWorldPoint(const Vector3D &point)
	{
		auto GetPixelFromTexcoord = [&](const Vector2D &texcoord)
			{
				Rect mapSize = m_heightMap->GetTexture()->GetSize();
				unsigned char *pixel = m_heightMap->GetTexture()->GetPixels();
				unsigned int bytesPerPixel = m_heightMap->GetTexture()->GetBitsPerPixel() / 8;
				auto index = (unsigned int)(((mapSize.width * bytesPerPixel) * texcoord.m_y) + texcoord.m_x * bytesPerPixel);
				auto height = (float)pixel[index];
				height /= 255.0f;

				return height;
			};


		Rect mapSize = m_heightMap->GetTexture()->GetSize();
		Vector3D scaleSize = m_size;
		Vector3D worldPos = m_entity->GetTransform()->GetPosition();
		Vector3D scale = Vector3D(scaleSize.m_x / mapSize.width, 0, scaleSize.m_z / mapSize.height);
		Vector3D tempPoint = point;
		tempPoint = Vector3D((tempPoint.m_x - worldPos.m_x) / scale.m_x, 
				0, 
				(tempPoint.m_z - worldPos.m_z) / scale.m_z);

		if (tempPoint.m_x < 0 || tempPoint.m_z < 0) return 0;

		float x = (float)(unsigned int)tempPoint.m_x;
		float y = (float)(unsigned int)tempPoint.m_z;

		float deltaX = x - (int)x;
		float deltaY = y - (int)y;

		float height0 = GetPixelFromTexcoord(Vector2D(x, y));
		float height1 = GetPixelFromTexcoord(Vector2D(x + 1, y));

		float height2 = GetPixelFromTexcoord(Vector2D(x, y + 1));
		float height3 = GetPixelFromTexcoord(Vector2D(x + 1, y + 1));

		float heightX1 = Math::Lerp(height0, height1, deltaX);
		float heightX2 = Math::Lerp(height2, height3, deltaX);

		float height = Math::Lerp(heightX1, heightX2, deltaY);

		return height * scaleSize.m_y;
	}

	void TerrainComponent::GenerateTerrainMesh()
	{
		const int width = 1024;
		const int height = 1024;
		
		const int widthIndices = width - 1;
		const int heightIndices = height - 1;

		// create an array of vertexMesh and int 
		VertexMesh *terrainMeshVertices = new VertexMesh[width * height];
		int *terrainMeshIndices = new int[widthIndices * heightIndices * 6];

		int i = 0;
		for (int x = 0; x < width; ++x) 
		{
			for (int y = 0; y < height; ++y)
			{
				terrainMeshVertices[y * width + x] = {
					Vector3D((float)x / (float)widthIndices, 0, (float)y / (float)heightIndices),
					Vector2D((float)x / (float)widthIndices, (float)y / (float)heightIndices),
					Vector3D(),
					Vector3D(),
					Vector3D()
				};

				if (x < widthIndices && y < heightIndices)
				{
					terrainMeshIndices[i] = (y + 1) * width + x;
					terrainMeshIndices[i + 1] = (y) * width + x;
					terrainMeshIndices[i + 2] = (y) * width + (x + 1);

					terrainMeshIndices[i + 3] = (y) * width + (x + 1);
					terrainMeshIndices[i + 4] = (y + 1)*width + (x + 1);
					terrainMeshIndices[i + 5] = (y + 1) * width + (x);
					i += 6;
				}
			}
		}

		RenderSystem *renderSystem = m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->GetRenderSystem();
		m_vertexBuffer = renderSystem->CreateVertexBuffer(terrainMeshVertices, sizeof(VertexMesh), width * height);
		m_indexBuffer = renderSystem->CreateIndexBuffer(terrainMeshIndices, widthIndices * heightIndices * 6);

		m_vertexShader = renderSystem->CreateVertexShader(L"assets/shaders/terrain.hlsl", "vsmain");
		m_pixelShader = renderSystem->CreatePixelShader(L"assets/shaders/terrain.hlsl", "psmain");
	}

	void TerrainComponent::UpdateData(void *data, UINT size)
	{
		RenderSystem *renderSystem = m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->GetRenderSystem();
		
		if (!m_constantBuffer)
		{
			m_constantBuffer = renderSystem->CreateConstantBuffer(data, size);
		}
		else m_constantBuffer->Update(renderSystem->GetImmediateDeviceContext(), data);
	}

	void TerrainComponent::OnCreateInternal()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->AddComponent(this);
		GenerateTerrainMesh();
	}
}
