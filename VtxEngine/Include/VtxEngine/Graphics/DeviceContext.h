#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <VtxEngine/Prerequisites.h>


namespace Vtx {

	class DeviceContext	
	{
	public:
		DeviceContext(ID3D11DeviceContext *deviceContext, RenderSystem* system);
		void ClearRenderTargetColor(const SwapChainPtr& swapChain, float r, float g, float b, float alpha);
		void ClearDepthStencil(const SwapChainPtr& swapChain);

		/*void ClearRenderTargetColor(const TexturePtr& renderTarget, float r, float g, float b, float alpha);
		void ClearDepthStencil(const TexturePtr& depthStencil);
		void SetRenderTarget(const TexturePtr& renderTarget, const TexturePtr& depthStencil);*/

		void SetVertexBuffer(const VertexBufferPtr& vertexBuffer);
		void SetIndexBuffer(const IndexBufferPtr& indexBuffer);

		void DrawTriangleList(UINT vertexCount, UINT startIdx);
		void DrawTriangleIndexList(UINT indexCount, UINT startIdx, UINT startIdxLocation);
		void DrawTriangleStrip(UINT vertexCount, UINT startIdx);

		void SetViewportSize(float width, float height);

		void SetVertexShader(const VertexShaderPtr& vertexShader);
		void SetPixelShader(const PixelShaderPtr& pixelShader);

		void SetConstantBuffer(const ConstantBufferPtr& buffer);

		//void SetTexture(const VertexShaderPtr& vertexShader, const TexturePtr* texture, UINT numTextures);
		void SetTexture(const Texture2DPtr* texture, UINT numTextures);
		//void SetCubeMap(const PixelShaderPtr& pixelShader, const CubeMapPtr& cubemap);

		void SetDepthStencilState(ID3D11DepthStencilState* state, UINT stencilRef = 0);

		~DeviceContext();

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;

		RenderSystem* m_system = nullptr;

	private:
		friend class ConstantBuffer;
		friend class RenderSystem;
		friend class GraphicsEngine;
	};
}

