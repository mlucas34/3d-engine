#pragma once
#include <VtxEngine/Resource/Resource.h>

namespace Vtx {
	class Texture : public Resource
	{
	public:
		Texture(const wchar_t *fullPath, ResourceManager *manager);
		Texture(const std::array<const wchar_t *, 6> &faces, ResourceManager *manager);
		~Texture();

		const Texture2DPtr &GetTexture();

	private:
		Texture2DPtr m_texture;

		friend class Material;
	};
}

