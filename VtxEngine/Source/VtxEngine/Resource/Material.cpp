#include <VtxEngine/Resource/Material.h>
#include <VtxEngine/Graphics/VertexShader.h>
#include <VtxEngine/Graphics/PixelShader.h>
#include <VtxEngine/Graphics/ConstantBuffer.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>
#include <VtxEngine/Game/Game.h>
#include <VtxEngine/Graphics/RenderSystem.h>
#include <VtxEngine/Resource/ResourceManager.h>
#include <VtxEngine/Resource/Texture.h>



namespace Vtx {

	Material::Material(const wchar_t* fullPath, ResourceManager* manager) : Resource(fullPath, manager)
	{
		
		RenderSystem *system = m_manager->GetGame()->GetGraphicsEngine()->GetRenderSystem();

		m_vertexShader = system->CreateVertexShader(fullPath, "vsmain");

		if (!m_vertexShader) VTXError("Failed to create Material Vertex Shader\n");

		// you must compile and release the vertex shader before compiling the pixel shader
		m_pixelShader = system->CreatePixelShader(fullPath, "psmain");

		if (!m_vertexShader) VTXError("Failed to create Material Pixel Shader\n");
	}

	Material::Material(const MaterialPtr& material, ResourceManager *manager) : Resource(L"", manager)
	{
		m_vertexShader = material->m_vertexShader;
		m_pixelShader = material->m_pixelShader;
	}

	Material::~Material()
	{
	}

	void Material::AddTexture(const TexturePtr& texture)
	{
		m_textureList.emplace_back(texture->m_texture);
	}

	void Material::RemoveTexture(UINT idx)
	{

		if (idx >= this->m_textureList.size()) return;
		else m_textureList.erase(m_textureList.begin() + idx);
	}

	void Material::SetData(void* data, UINT size)
	{

		RenderSystem* system = m_manager->GetGame()->GetGraphicsEngine()->GetRenderSystem();

		if (!m_constantBuffer) m_constantBuffer = system->CreateConstantBuffer(data, size);
		else m_constantBuffer->Update(system->GetImmediateDeviceContext(), data);
	}	

	void Material::SetCullMode(const CullMode &mode)
	{
		m_cullMode = mode;
	}

	CullMode Material::GetCullMode()
	{
		return m_cullMode;
	}

}
