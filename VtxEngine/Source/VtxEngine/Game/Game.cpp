#include "VtxEngine/Game/Game.h"
#include <VtxEngine/Window/Window.h>
#include <VtxEngine/Graphics/GraphicsEngine.h>
#include <VtxEngine/Resource/ResourceManager.h>
#include <VtxEngine/Game/Display.h>
#include <VtxEngine/Resource/Mesh.h>
#include <VtxEngine/Resource/Texture.h>
#include <VtxEngine/Resource/Material.h>
#include <VtxEngine/Input/InputSystem.h>
#include <VtxEngine/Game/World.h>
#include <VtxEngine/Physics/PhysicsEngine.h>

namespace Vtx {
	Game::Game()
	{
		m_inputSystem = std::make_unique<InputSystem>();
		m_physicsEngine = std::make_unique<PhysicsEngine>(this);
		m_graphicsEngine = std::make_unique<GraphicsEngine>(this);
		m_display = std::make_unique<Display>(this);
		m_resourceManager = std::make_unique<ResourceManager>(this);
		m_world = std::make_unique<World>(this);

		m_inputSystem->SetLockArea(m_display->GetClientSize());
	}

	void Game::OnInternalUpdate()
	{
		// compute time per frame
		std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsedTime = std::chrono::duration<double>();

		if (m_previousTime.time_since_epoch().count()) elapsedTime = currentTime - m_previousTime;
		m_previousTime = currentTime;

		float deltaTime = (float)elapsedTime.count();

		// handles update of internal systems such as graphics engine and render system
		m_inputSystem->Update();

		OnUpdate(deltaTime);
		m_world->Update(deltaTime);

		m_physicsEngine->Update();
		m_graphicsEngine->UpdateFrame();
;	}

	World *Game::GetWorld()
	{
		return m_world.get();
	}

	ResourceManager *Game::GetResourceManager()
	{
		return m_resourceManager.get();
	}

	InputSystem *Game::GetInputSystem()
	{
		return m_inputSystem.get();
	}

	GraphicsEngine *Game::GetGraphicsEngine()
	{
		return m_graphicsEngine.get();
	}

	void Game::OnDisplaySize(const Rect &size)
	{
		m_inputSystem->SetLockArea(m_display->GetClientSize());
		OnInternalUpdate();
	}

	Game::~Game()
	{
	}

	void Game::Quit()
	{
		isRunning = false;
	}
}
