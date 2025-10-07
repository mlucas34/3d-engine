#include <VtxEngine/Graphics/GraphicsEngine.h>
#include <VtxEngine/Graphics/RenderSystem.h>
#include <VtxEngine/Graphics/DeviceContext.h>
#include <VtxEngine/Game/Game.h>
#include <VtxEngine/Game/Display.h>
#include <VtxEngine/Graphics/SwapChain.h> 
#include <VtxEngine/Resource/Mesh.h>
#include <VtxEngine/Resource/Material.h>
#include <VtxEngine/Math/Matrix4x4.h>
#include <VtxEngine/Math/Vector4D.h>
#include <VtxEngine/Resource/Texture.h>
#include <VtxEngine/Entity/MeshComponent.h>
#include <VtxEngine/Entity/TransformComponent.h>
#include <VtxEngine/Entity/CameraComponent.h>
#include <VtxEngine/Entity/LightComponent.h>
#include <VtxEngine/Entity/TerrainComponent.h>
#include <VtxEngine/Entity/Entity.h>

namespace Vtx {
	__declspec(align(16))
	struct LightData
	{
		Vector4D color;
		Vector4D direction;
	};

	__declspec(align(16))
		struct TerrainData
	{
		Vector4D size;
		float heightMapSize = 0;
	};

	__declspec(align(16))
	struct ConstantData
	{
		Matrix4x4 world;
		Matrix4x4 view;
		Matrix4x4 projection;
		Vector4D cameraPosition;
		LightData light;
		TerrainData terrain;
	};

	Vtx::GraphicsEngine::GraphicsEngine(Game* game) : m_game(game)
	{
		m_renderSystem = std::make_unique<RenderSystem>();
	}

	GraphicsEngine::~GraphicsEngine() 
	{
		
	}

	void GraphicsEngine::UpdateFrame()
	{
		SwapChainPtr swapChain = m_game->m_display->m_swapChain;

		DeviceContextPtr context = m_renderSystem->GetImmediateDeviceContext();

		context->ClearRenderTargetColor(swapChain, 1.0f, 0.3f, 0.0f, 1.0f);

		Rect windowSize = m_game->m_display->GetClientSize();
		context->SetViewportSize(windowSize.width, windowSize.height);

		ConstantData constData = {};
		constData.view.SetIdentity();
		constData.projection.SetIdentity();

		for (auto c : m_cameras)
		{
			TransformComponent *t = c->GetEntity()->GetTransform();
			constData.cameraPosition = t->GetPosition();

			c->SetScreenArea(windowSize);
			c->GetViewMatrix(constData.view);
			c->GetProjectionMatrix(constData.projection);
		}

		for (LightComponent *l : m_lights)
		{
			TransformComponent *t = l->GetEntity()->GetTransform();
			Matrix4x4 w;
			t->GetWorldMatrix(w);

			constData.light.direction = w.GetZDirection();
			constData.light.color = l->GetColor();
		}

		for (TerrainComponent *terrain : m_terrains)
		{
			TransformComponent *transform = terrain->GetEntity()->GetTransform();
			Matrix4x4 world;
			transform->GetWorldMatrix(world);
			constData.terrain.size = terrain->GetSize();

			constData.terrain.heightMapSize = terrain->GetHeightMap()->GetTexture()->GetSize().width;

			context->SetVertexBuffer(terrain->m_vertexBuffer);
			context->SetIndexBuffer(terrain->m_indexBuffer);

			m_renderSystem->SetCullMode(CullMode::Back);
			terrain->UpdateData(&constData, sizeof(constData));

			context->SetConstantBuffer(terrain->m_constantBuffer);

			context->SetVertexShader(terrain->m_vertexShader);
			context->SetPixelShader(terrain->m_pixelShader);

			Texture2DPtr terrainTextures[3];
			terrainTextures[0] = terrain->GetHeightMap()->GetTexture();
			terrainTextures[1] = terrain->GetGroundMap()->GetTexture();
			terrainTextures[2] = terrain->GetRockMap()->GetTexture();
			context->SetTexture(terrainTextures, 3);

			context->DrawTriangleIndexList((unsigned int)terrain->m_indexBuffer->GetIndexListSize(), 0, 0);
		}

		ID3D11RenderTargetView *rtv = nullptr;
		ID3D11DepthStencilView *dsv = nullptr;
		context->m_deviceContext->OMGetRenderTargets(1, &rtv, &dsv);
		if (rtv) rtv->Release();


		for (MeshComponent *m : m_meshes)
		{
			TransformComponent *transform = m->GetEntity()->GetTransform();
			transform->GetWorldMatrix(constData.world);

			Mesh *mesh = m->GetMesh().get();
			const std::vector<MaterialPtr> materials = m->GetMaterialList();

			if (!mesh->m_vertexBuffer || !mesh->m_indexBuffer) return;

			context->SetVertexBuffer(mesh->m_vertexBuffer);
			context->SetIndexBuffer(mesh->m_indexBuffer);


			for (size_t i = 0; i < mesh->GetNumMaterialSlots(); ++i)
			{
				if (i >= materials.size()) break;

				Material *mat = materials[i].get();

				m_renderSystem->SetCullMode(mat->GetCullMode());

				mat->SetData(&constData, sizeof(ConstantData));
				context->SetConstantBuffer(mat->m_constantBuffer);

				context->SetVertexShader(mat->m_vertexShader);
				context->SetPixelShader(mat->m_pixelShader);

				if (!mat->m_textureList.empty())
					context->SetTexture(mat->m_textureList.data(), (unsigned)mat->m_textureList.size());
				else {
					context->SetTexture(nullptr, 0);
				}

				MaterialSlot slot = mesh->GetMaterialSlots(i);

				context->DrawTriangleIndexList(slot.numIndices, slot.startIndex, 0);
			}
		}

		swapChain->Present(true);
	}

	RenderSystem* GraphicsEngine::GetRenderSystem()
	{
		return m_renderSystem.get();
	}

	void GraphicsEngine::AddComponent(Component *component)
	{
		if (auto c = dynamic_cast<MeshComponent*>(component))
		{
			m_meshes.emplace(c);
		}
		else if (auto c = dynamic_cast<CameraComponent *>(component))
		{
			if (!m_cameras.size()) m_cameras.emplace(c); // prevent from supporting multiple cameras for now
		}
		else if (auto c = dynamic_cast<LightComponent *>(component))
		{
			if (!m_lights.size()) m_lights.emplace(c); // prevent from supporting multiple lights for now
		}
		else if (auto t = dynamic_cast<TerrainComponent *>(component))
		{
			if (!m_terrains.size()) m_terrains.emplace(t); // prevent from supporting multiple lights for now
		}
	}

	void GraphicsEngine::RemoveComponent(Component *component)
	{
		if (auto c = dynamic_cast<MeshComponent*>(component))
		{
			m_meshes.erase(c);
		}
		else if (auto c = dynamic_cast<CameraComponent *>(component))
		{
			m_cameras.erase(c);
		}
		else if (auto c = dynamic_cast<LightComponent *>(component))
		{
			m_lights.erase(c);
		}
		else if (auto c = dynamic_cast<TerrainComponent *>(component))
		{
			m_terrains.erase(c);
		}
	}
}