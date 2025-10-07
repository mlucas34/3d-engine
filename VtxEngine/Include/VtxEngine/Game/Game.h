#pragma once
#include <VtxEngine/Prerequisites.h>
#include <VtxEngine/Math/Rect.h>
#include <chrono>

namespace Vtx {
	class Game {
	public:
		Game();
		virtual ~Game();

		void Run();
		void Quit();

		GraphicsEngine *GetGraphicsEngine();
		World *GetWorld();
		ResourceManager *GetResourceManager();
		InputSystem *GetInputSystem();

	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnQuit() {}

	private:
		void OnInternalUpdate();
		virtual void OnDisplaySize(const Rect& size);


	private:
		// order matters for deconstruction as class variables or deconstructed top down
		std::unique_ptr<InputSystem> m_inputSystem = nullptr;
		std::unique_ptr<PhysicsEngine> m_physicsEngine = nullptr;
		std::unique_ptr<GraphicsEngine> m_graphicsEngine = nullptr;
		std::unique_ptr<Display> m_display = nullptr;
		std::unique_ptr<ResourceManager> m_resourceManager = nullptr;
		std::unique_ptr<World> m_world = nullptr;

		bool isRunning = true;
		std::chrono::system_clock::time_point m_previousTime;

		friend class GraphicsEngine;
		friend class Display;
	};
}