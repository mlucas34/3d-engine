#pragma once
#include <VtxEngine/Resource/Resource.h>
#include <d3d.h>
#include <vector>

namespace Vtx {
	class Material : public Resource
	{
	public:
		Material(const wchar_t* fullPath, ResourceManager *manager);
		Material(const MaterialPtr& material, ResourceManager* manager);
		~Material();

		void AddTexture(const TexturePtr& texture);
		void RemoveTexture(UINT idx);

		void SetData(void *data, UINT size);

		void SetCullMode(const CullMode &mode);
		CullMode GetCullMode();

	private:
		VertexShaderPtr m_vertexShader = nullptr;
		PixelShaderPtr m_pixelShader = nullptr;
		std::vector<Texture2DPtr> m_textureList;
		ConstantBufferPtr m_constantBuffer = nullptr;
		CullMode m_cullMode = CullMode::Back;

	private:
		friend class GraphicsEngine;
	};
}

