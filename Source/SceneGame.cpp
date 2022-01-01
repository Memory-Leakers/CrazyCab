#include "SceneGame.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModulePhysics3D.h"
#include "Vehicle.h"
#include "ModuleMap.h"

SceneGame::SceneGame(Application* app) :Scene(app)
{
}

SceneGame::~SceneGame()
{
}

bool SceneGame::InitScene()
{
	for (int i = 0; i < _app->map->mapObjects.count(); i++)
	{
		GameObject* g = new GameObject("wall", "Wall", _app);
		g->pBody = _app->physics->CreateCube(_app->map->mapObjects[i].dimensions[0], _app->map->mapObjects[i].dimensions[1], _app->map->mapObjects[i].dimensions[2], 9999);
		g->pBody->SetPos(_app->map->mapObjects[i].position.x, 1, _app->map->mapObjects[i].position.y);

		gameObjects.add(g);
	}
	return true;
}

bool SceneGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	this->name = "GameScene";

	_app->map->Load("Map.tmx");

	InitScene();

	_app->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	_app->camera->LookAt(vec3(0, 0, 0));

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
