#include <VtxEngine/Resource/Texture.h>
#include <VtxEngine/Resource/ResourceManager.h>
#include <VtxEngine/Game/Game.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>
#include <VtxEngine/Graphics/RenderSystem.h>

namespace Vtx {
	Texture::Texture(const wchar_t* fullPath, ResourceManager *manager) : Resource(fullPath, manager)
	{
		m_texture = manager->GetGame()->GetGraphicsEngine()->GetRenderSystem()->CreateTexture(fullPath);
	}

	Texture::Texture(const std::array<const wchar_t *, 6> &faces, ResourceManager *manager) : Resource(faces, manager)
	{
		m_texture = manager->GetGame()->GetGraphicsEngine()->GetRenderSystem()->CreateTexture(faces);
	}

	Texture::~Texture()
	{
	}
	const Texture2DPtr &Texture::GetTexture()
	{
		return m_texture;
	}
}

