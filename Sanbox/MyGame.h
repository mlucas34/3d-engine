#pragma once
#include <VtxEngine/HighLevelClasses.h>

class MyGame : public Vtx::Game
{

public:
	MyGame();
	~MyGame();
protected:
	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);

private:
	Vtx::Entity *m_entity = nullptr;
	Vtx::Entity *m_skybox = nullptr;

	Vtx::Player *m_player = nullptr;

	Vtx::TerrainComponent *m_terrainComp = nullptr;

	float m_elapsedTime = 0.0f;
	float m_rotation = 0.0f;
};

