#include "SceneGame.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "Vehicle.h"
#include "ModulePhysics3D.h"

SceneGame::SceneGame(Application* app) :Scene(app)
{
}

SceneGame::~SceneGame()
{
}

bool SceneGame::InitScene()
{
	return true;
}

bool SceneGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	this->name = "GameScene";

	_app->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	_app->camera->LookAt(vec3(0, 0, 0));


	/*GameObject* g = new GameObject("test", "Test", _app);
	Cube* cube = new Cube(10.0f, 20.0f, 30.0f);
	g->pBody = _app->physics->AddBody(*cube, 1);*/

	_app->physics->CreateCube(10, 20, 30, 40);

	Vehicle* v = new Vehicle("vehicle", "Vehicle", _app);
	gameObjects.add(v);
	//gameObjects.add(g);
	Scene::Start();

	return ret;
}

bool SceneGame::PreUpdate()
{
	Scene::PreUpdate();

	return true;
}

bool SceneGame::Update()
{
	Scene::Update();

	return true;
}

bool SceneGame::PostUpdate()
{
	Scene::PostUpdate();

	return true;
}

bool SceneGame::CleanUp()
{
	Scene::CleanUp();

	return true;
}
