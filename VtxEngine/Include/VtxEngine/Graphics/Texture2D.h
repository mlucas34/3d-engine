#pragma once
#include <VtxEngine/Prerequisites.h>
#include <d3d11.h>
#include <wrl.h>
#include <VtxEngine/Math/Rect.h>
#include <DirectXTex.h>

namespace Vtx {
	class Texture2D
	{
	public:
		enum class Type
		{
			normal = 0,
			renderTarget,
			depthStencil,
		};


	public:
		Texture2D(const wchar_t* fullPath, RenderSystem *system);
		Texture2D(const std::array<const wchar_t*, 6>& faces, RenderSystem* system);
		Texture2D(const Rect& size, Texture2D::Type type, RenderSystem* system);
		~Texture2D();

		Rect GetSize();
		Texture2D::Type GetType();

		unsigned char *GetPixels();
		unsigned int GetBitsPerPixel();

	private:
		Microsoft::WRL::ComPtr<ID3D11Resource> m_texture = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_skyboxTexture = nullptr;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResView = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView = nullptr;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState = nullptr;

		Texture2D::Type m_type = Texture2D::Type::normal;
		Rect m_size;
		DirectX::ScratchImage m_imageData;

		RenderSystem *m_system = nullptr;

	private:
		friend class DeviceContext;
	};
}

