#pragma once
#include <VtxEngine/Prerequisites.h>
#include <set>

namespace Vtx {

	class GraphicsEngine
	{
	public:
		GraphicsEngine(Game *game);
		~GraphicsEngine();

		void UpdateFrame();

	public:
		RenderSystem* GetRenderSystem(); // if errors change these to regular ptrs

		void AddComponent(Component *component);
		void RemoveComponent(Component *component);

	private:
		std::unique_ptr<RenderSystem> m_renderSystem = nullptr;
		Game *m_game = nullptr;

		std::set<MeshComponent*> m_meshes;
		std::set<CameraComponent *> m_cameras;
		std::set<LightComponent *> m_lights;
		std::set<TerrainComponent *> m_terrains;
	};
}

