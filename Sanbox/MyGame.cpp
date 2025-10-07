#include "MyGame.h"
#include <VtxEngine/Game/Player.h>
#include <VtxEngine/HighLevelClasses.h>
#include <array>

MyGame::MyGame()
{

}

void MyGame::OnCreate()
{
	Game::OnCreate();
	m_player = GetWorld()->CreateEntity<Vtx::Player>();
	m_player->GetTransform()->SetPosition(Vtx::Vector3D(40, 0, 0));

	/*Vtx::MeshPtr mesh = GetResourceManager()->CreateResourceFromFile<Vtx::Mesh>(L"assets/meshes/house.obj");*/
	Vtx::MeshPtr box = GetResourceManager()->CreateResourceFromFile<Vtx::Mesh>(L"assets/meshes/box.obj");
	/*Vtx::MeshPtr terrain = GetResourceManager()->CreateResourceFromFile<Vtx::Mesh>(L"assets/meshes/terrain.obj");*/
	Vtx::TexturePtr grass = GetResourceManager()->CreateResourceFromFile<Vtx::Texture>(L"assets/textures/grass.jpg");
	Vtx::TexturePtr rockTexture = GetResourceManager()->CreateResourceFromFile<Vtx::Texture>(L"assets/textures/cobblestone.jpg");

	std::array<const wchar_t *, 6> skyboxFaces = {
		L"assets\\textures\\px.png", // +x
		L"assets\\textures\\nx.png", // -x
		L"assets\\textures\\py.png", // +y
		L"assets\\textures\\ny.png", // -y
		L"assets\\textures\\pz.png", // +z
		L"assets\\textures\\nz.png", // -z
	};

	Vtx::TexturePtr skytex = GetResourceManager()->CreateResourceFromFile<Vtx::Texture>(skyboxFaces);
	Vtx::MaterialPtr skyboxMat = GetResourceManager()->CreateResourceFromFile<Vtx::Material>(L"assets/shaders/skybox/skyshader.hlsl");
	skyboxMat->AddTexture(skytex);
	skyboxMat->SetCullMode(Vtx::CullMode::Front);

	//Vtx::MaterialPtr mat = GetResourceManager()->CreateResourceFromFile<Vtx::Material>(L"assets/shaders/shadertest.hlsl");
	//mat->AddTexture(grass);

	//Vtx::MaterialPtr floorMat = GetResourceManager()->CreateResourceFromFile<Vtx::Material>(L"assets/shaders/shadertest.hlsl");
	//floorMat->AddTexture(grass);

	// terrain
	Vtx::TexturePtr heightMap = GetResourceManager()->CreateResourceFromFile<Vtx::Texture>(L"assets/textures/heightmapsample.png");
	{
		Vtx::Entity *entity = GetWorld()->CreateEntity<Vtx::Entity>();
		m_terrainComp = entity->CreateComponent<Vtx::TerrainComponent>();
		m_terrainComp->SetHeightMap(heightMap);
		m_terrainComp->SetGroundMap(grass);
		m_terrainComp->SetRockMap(rockTexture);

		Vtx::TransformComponent *transform = entity->GetTransform();
		transform->SetScale(Vtx::Vector3D(100.0f, 100.0f, 100.0f));
	}

	// skybox
	{
		m_entity = GetWorld()->CreateEntity<Vtx::Entity>();
		Vtx::LightComponent *lightcomp = m_entity->CreateComponent<Vtx::LightComponent>();
		lightcomp->SetColor(Vtx::Vector4D(1, 1, 1, 1));
		m_entity->GetTransform()->SetRotation(Vtx::Vector3D(-0.7f, 0.7f, 0));
	}

	//// light
	//{
	//	Vtx::Entity *entity = GetWorld()->CreateEntity<Vtx::Entity>();
	//	Vtx::MeshComponent *meshComp = entity->CreateComponent<Vtx::MeshComponent>();
	//	meshComp->SetMesh(terrain);
	//	meshComp->AddMaterial(floorMat);
	//}

	m_skybox = GetWorld()->CreateEntity<Vtx::Entity>();
	Vtx::MeshComponent *skymeshComp = m_skybox->CreateComponent<Vtx::MeshComponent>();
	skymeshComp->SetMesh(box);
	skymeshComp->AddMaterial(skyboxMat);

	//Vtx::Entity *entity = GetWorld()->CreateEntity<Vtx::Entity>();
	//Vtx::MeshComponent *meshComp = entity->CreateComponent<Vtx::MeshComponent>();
	//meshComp->SetMesh(mesh);
	//meshComp->AddMaterial(mat);
	//meshComp->AddMaterial(mat);
	//meshComp->AddMaterial(mat);
	//meshComp->AddMaterial(mat);

	GetInputSystem()->LockCursor(true);
}

void MyGame::OnUpdate(float deltaTime)
{	
	Game::OnUpdate(deltaTime);
	//m_rotation += 1.57f * deltaTime;

	//m_entity->GetTransform()->SetRotation(Vtx::Vector3D(-0.7f, m_rotation, 0));
	Vtx::Vector3D playerPos = m_player->GetTransform()->GetPosition();
	float height = m_terrainComp->GetHeightFromWorldPoint(playerPos);

	std::cout << height << '\n';
}

MyGame::~MyGame()
{

}