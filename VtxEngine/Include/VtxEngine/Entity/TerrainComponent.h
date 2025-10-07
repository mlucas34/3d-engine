#pragma once
#include <VtxEngine/Entity/Component.h>
#include <VtxEngine/Prerequisites.h>
#include <VtxEngine/Math/Vector3D.h>
#include <d3d11.h>


namespace Vtx
{
	class TerrainComponent : public Component
	{
	public:
		TerrainComponent();
		virtual ~TerrainComponent();

		void SetHeightMap(const TexturePtr heightMap);
		const TexturePtr &GetHeightMap();

		void SetGroundMap(const TexturePtr groundMap);
		const TexturePtr &GetGroundMap();

		void SetRockMap(const TexturePtr rockMap);
		const TexturePtr &GetRockMap();

		void SetSize(const Vector3D &size);
		Vector3D GetSize();

		float GetHeightFromWorldPoint(const Vector3D& point);

	protected:
		void GenerateTerrainMesh();
		void UpdateData(void *data, UINT size);

		virtual void OnCreateInternal();

	private:
		TexturePtr m_heightMap;
		TexturePtr m_groundMap;
		TexturePtr m_rockMap;

		Vector3D m_size = Vector3D(512, 50, 512);

		VertexBufferPtr m_vertexBuffer = nullptr;
		IndexBufferPtr m_indexBuffer = nullptr;
		ConstantBufferPtr m_constantBuffer = nullptr;

		VertexShaderPtr m_vertexShader = nullptr;
		PixelShaderPtr m_pixelShader = nullptr;

		friend class GraphicsEngine;
	};
}

