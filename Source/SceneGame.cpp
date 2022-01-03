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
	float scale = 16.0f / 9.0f;
	for (int i = 0; i < _app->map->mapObjects.count(); i++)
	{
		GameObject* g = new GameObject("wall", "Wall", _app);
		g->InitAsCube(_app->map->mapObjects[i].dimensions[0] * scale, _app->map->mapObjects[i].dimensions[1] * 10 * scale, _app->map->mapObjects[i].dimensions[2] * scale, 1);
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

	Scene::Start();

	_app->camera->Move(v->GetPosition() + vec3{ 0,5,-15 });
	_app->camera->LookAt(v->GetPosition());

	_app->camera->SetTarget(v, vec3{ 0,5,-15 });

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
	_app->camera->RemoveTarget();

	Scene::CleanUp();

	return true;
}
