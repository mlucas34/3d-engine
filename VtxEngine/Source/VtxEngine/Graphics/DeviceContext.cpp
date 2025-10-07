#include <VtxEngine/Graphics/DeviceContext.h>
#include <iostream>
#include <VtxEngine/Graphics/VertexBuffer.h>
#include <VtxEngine/Graphics/SwapChain.h>
#include <VtxEngine/Graphics/VertexShader.h>
#include <VtxEngine/Graphics/PixelShader.h>
#include <VtxEngine/Graphics/IndexBuffer.h>
#include <VtxEngine/Graphics/ConstantBuffer.h>
#include <VtxEngine/Graphics/Texture2D.h>
#include <exception>


namespace Vtx {
	DeviceContext::DeviceContext(ID3D11DeviceContext *deviceContext, RenderSystem* system): m_deviceContext(deviceContext), m_system(system)
	{

	}

	void DeviceContext::ClearRenderTargetColor(const SwapChainPtr& swapChain, float r, float g, float b, float alpha)
	{
		FLOAT clearColor[] = {r, g, b, alpha};

		auto rtv = swapChain->m_rtv.Get();
		auto dsv = swapChain->m_dsv.Get();

		m_deviceContext->ClearRenderTargetView(rtv, clearColor);
		m_deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		m_deviceContext->OMSetRenderTargets(1, &rtv, dsv);
	}

	void DeviceContext::ClearDepthStencil(const SwapChainPtr& swapChain)
	{
		auto dsv = swapChain->m_dsv.Get();

		m_deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	}

	//void DeviceContext::ClearRenderTargetColor(const TexturePtr& renderTarget, float r, float g, float b, float alpha)
	//{
	//	if (renderTarget->m_type != Texture::Type::renderTarget) return;

	//	FLOAT clearColor[] = { r, g, b, alpha };
	//	m_deviceContext->ClearRenderTargetView(renderTarget->m_renderTargetView, clearColor);
	//}

	//void DeviceContext::ClearDepthStencil(const TexturePtr& depthStencil)
	//{
	//	auto dsv = swapChain->m_dsv.Get();

	//	m_deviceContext->ClearDepthStencilView(depthStencil->m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//}

	//void DeviceContext::SetRenderTarget(const TexturePtr& renderTarget, const TexturePtr& depthStencil)
	//{
	//	if (depthStencil->m_type != Texture::Type::depthStencil) return;
	//	if (renderTarget->m_type != Texture::Type::renderTarget) return;

	//	m_deviceContext->OMSetRenderTargets(1, &renderTarget->m_renderTargetView, depthStencil->m_depthStencilView);
	//}

	void DeviceContext::SetVertexBuffer(const VertexBufferPtr& vertexBuffer)
	{
		UINT stride = vertexBuffer->m_vertexSize;
		UINT offset = 0;

		ID3D11InputLayout *layout = vertexBuffer->m_layout.Get();

		ID3D11Buffer *buffer = vertexBuffer->m_buffer.Get();

		m_deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);

		m_deviceContext->IASetInputLayout(layout);
	}

	void DeviceContext::SetIndexBuffer(const IndexBufferPtr& indexBuffer)
	{
		auto buffer = indexBuffer->m_buffer.Get();

		m_deviceContext->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void DeviceContext::DrawTriangleList(UINT vertexCount, UINT startIdx)
	{
		// determines how to inrpert the vertex data in the buffer
		// this line makes it so it is interpreted as triangles
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_deviceContext->Draw(vertexCount, startIdx);

	}

	void DeviceContext::DrawTriangleIndexList(UINT indexCount, UINT startIdx, UINT startIdxLocation)
	{
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_deviceContext->DrawIndexed(indexCount, startIdx, startIdxLocation);
	}

	void DeviceContext::SetViewportSize(float width, float height)
	{
		// creates the viewport object so it can be set for device context
		D3D11_VIEWPORT vp = {};
		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		m_deviceContext->RSSetViewports(1, &vp);
	}

	void DeviceContext::DrawTriangleStrip(UINT vertexCount, UINT startIdx)
	{
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		m_deviceContext->Draw(vertexCount, startIdx);

	}

	void DeviceContext::SetVertexShader(const VertexShaderPtr& vertexShader)
	{
		m_deviceContext->VSSetShader(vertexShader->m_vs.Get(), nullptr, 0);
	}

	void DeviceContext::SetPixelShader(const PixelShaderPtr& pixelShader)
	{
		m_deviceContext->PSSetShader(pixelShader->m_ps.Get(), nullptr, 0);
	}

	void DeviceContext::SetConstantBuffer(const ConstantBufferPtr& buffer)
	{

		ID3D11Buffer *bufferPtr = buffer->m_buffer.Get();

		m_deviceContext->VSSetConstantBuffers(0, 1, &bufferPtr);
		m_deviceContext->PSSetConstantBuffers(0, 1, &bufferPtr);
	}

	//void DeviceContext::SetTexture(const VertexShaderPtr& vertexShader, const TexturePtr* texture, UINT numTextures)
	//{
	//	ID3D11ShaderResourceView* resList[32];
	//	ID3D11SamplerState* samplerList[32];

	//	for (UINT i = 0; i < numTextures; ++i) {
	//		resList[i] = texture[i]->m_shaderResView;
	//		samplerList[i] = texture[i]->m_samplerState;
	//	}
	//	m_deviceContext->VSSetShaderResources(0, numTextures, resList);
	//	m_deviceContext->VSSetSamplers(0, numTextures, samplerList);
	//}

	void DeviceContext::SetTexture(const Texture2DPtr* texture, UINT numTextures)
	{
		ID3D11ShaderResourceView* resList[32];
		ID3D11SamplerState* samplerList[32];

		for (UINT i = 0; i < numTextures; ++i) {
			resList[i] = texture[i]->m_shaderResView.Get();
			samplerList[i] = texture[i]->m_samplerState.Get();
		}

		m_deviceContext->VSSetShaderResources(0, numTextures, resList);
		m_deviceContext->VSSetSamplers(0, numTextures, samplerList);

		m_deviceContext->PSSetShaderResources(0, numTextures, resList);
		m_deviceContext->PSSetSamplers(0, numTextures, samplerList);
	}

	//void DeviceContext::SetCubeMap(const PixelShaderPtr& pixelShader, const CubeMapPtr& cubemap)
	//{
	//	m_deviceContext->PSSetShaderResources(0, 1, &cubemap->m_shaderResView);
	//}

	void DeviceContext::SetDepthStencilState(ID3D11DepthStencilState* state, UINT stencilRef)
	{
		m_deviceContext->OMSetDepthStencilState(state, stencilRef);
	}

	DeviceContext::~DeviceContext()
	{
		std::cout << "Device Contet released\n";
	}


}