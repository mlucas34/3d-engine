#include <VtxEngine/Graphics/Texture2D.h>
#include <DirectXTex.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>
#include <VtxEngine/Graphics/RenderSystem.h>
#include <array>

namespace Vtx {
	Texture2D::Texture2D(const wchar_t* fullPath, RenderSystem* system) : m_system(system)
	{
		HRESULT res = DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, m_imageData);

		if (FAILED(res)) {
			throw std::runtime_error("Texture Creation Unsuccessful");
		}
		else {
			res = DirectX::CreateTexture(m_system->m_d3dDevice.Get(), m_imageData.GetImages(), m_imageData.GetImageCount(), m_imageData.GetMetadata(), &m_texture);

			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.Format = m_imageData.GetMetadata().format;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = m_imageData.GetMetadata().mipLevels;
			desc.Texture2D.MostDetailedMip = 0;

			D3D11_SAMPLER_DESC sampDesc = {};
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = (UINT)m_imageData.GetMetadata().mipLevels;


			res = m_system->m_d3dDevice->CreateSamplerState(&sampDesc, &m_samplerState);

			if (FAILED(res)) {
				throw std::runtime_error("Failed to create Sampler State\n");
			}

			m_system->m_d3dDevice->CreateShaderResourceView(m_texture.Get(), &desc, &m_shaderResView);
		}

		m_size = Rect(0, 0, (int)m_imageData.GetMetadata().width, (int)m_imageData.GetMetadata().height);
	}

	Texture2D::Texture2D(const std::array<const wchar_t*, 6>& faces, RenderSystem* system) : m_system(system)
	{
		std::vector<DirectX::ScratchImage> faceImages(6);

		for (int i = 0; i < 6; ++i) {
			HRESULT res = DirectX::LoadFromWICFile(faces[i], DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, faceImages[i]);

			if (FAILED(res)) {
				throw std::runtime_error("Failed to upload CubeMap face");
			}
		}

		std::vector<D3D11_SUBRESOURCE_DATA> pData(6);

		for (int i = 0; i < 6; ++i) {
			pData[i].pSysMem = faceImages[i].GetImages()[0].pixels;
			pData[i].SysMemPitch = static_cast<UINT>(faceImages[i].GetImages()[0].rowPitch);
			pData[i].SysMemSlicePitch = static_cast<UINT>(faceImages[i].GetImages()[0].slicePitch);
		}

		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = static_cast<UINT>(faceImages[0].GetMetadata().width);
		desc.Height = static_cast<UINT>(faceImages[0].GetMetadata().height);
		desc.MipLevels = static_cast<UINT>(faceImages[0].GetMetadata().mipLevels);
		desc.ArraySize = 6;
		desc.Format = faceImages[0].GetMetadata().format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		HRESULT res = m_system->m_d3dDevice->CreateTexture2D(&desc, pData.data(), &m_skyboxTexture);

		if (FAILED(res)) {
			throw std::runtime_error("Failed to create CubeMap Texture\n");
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = desc.MipLevels;
		srvDesc.TextureCube.MostDetailedMip = 0;

		res = m_system->m_d3dDevice->CreateShaderResourceView(this->m_skyboxTexture.Get(), &srvDesc, &m_shaderResView);

		if (FAILED(res)) {
			throw std::runtime_error("Failed to create CubeMap SRV\n");
		}
	}

	Texture2D::Texture2D(const Rect& size, Texture2D::Type type, RenderSystem* system) : m_system(system)
	{
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = size.width;
		texDesc.Height = size.height;

		if (type == Texture2D::Type::normal)
		{
			texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		}
		else if (type == Texture2D::Type::renderTarget)
		{
			texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		}
		else if (type == Texture2D::Type::depthStencil)
		{
			texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		}

		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.MipLevels = 1;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.MiscFlags = 0;
		texDesc.ArraySize = 1;
		texDesc.CPUAccessFlags = 0;

		HRESULT hr = m_system->m_d3dDevice->CreateTexture2D(&texDesc, nullptr, (ID3D11Texture2D**) m_texture.GetAddressOf());

		if (FAILED(hr))
		{
			throw std::exception("Couldnt create texture 2D, error in Texture.cpp\n");
		}

		if (type == Texture2D::Type::normal || type == Texture2D::Type::renderTarget)
		{
			hr = m_system->m_d3dDevice->CreateShaderResourceView(this->m_texture.Get(), nullptr, &m_shaderResView);

			if (FAILED(hr))
			{
				throw std::exception("Couldn't create shader res view in Texture.cpp\n");
			}
		}

		if (type == Texture2D::Type::renderTarget)
		{
			hr = m_system->m_d3dDevice->CreateRenderTargetView(this->m_texture.Get(), nullptr, &m_renderTargetView);

			if (FAILED(hr))
			{
				throw std::exception("Couldn't create shader res view in Texture.cpp\n");
			}

		} else if (type == Texture2D::Type::depthStencil)
		{
			hr = m_system->m_d3dDevice->CreateDepthStencilView(this->m_texture.Get(), nullptr, &m_depthStencilView);

			if (FAILED(hr))
			{
				throw std::exception("Couldn't create depth stencil view in Texture.cpp\n");
			}
		}

		m_type = type;
		m_size = size;
	}

	Texture2D::~Texture2D()
	{
	}

	Rect Texture2D::GetSize()
	{
		return m_size;
	}

	Texture2D::Type Texture2D::GetType()
	{
		return m_type;
	}

	unsigned char *Texture2D::GetPixels()
	{
		return m_imageData.GetImages()->pixels;
	}

	unsigned int Texture2D::GetBitsPerPixel()
	{
		return (unsigned int)DirectX::BitsPerPixel(m_imageData.GetMetadata().format);
	}

}
