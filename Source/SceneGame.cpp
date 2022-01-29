#include "SceneGame.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModulePhysics3D.h"
#include "Vehicle.h"
#include "TaxiClient.h"
#include "ModuleMap.h"
#include "Arrow.h"
#include "Booster.h"


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
		GameObject* g = new GameObject("wall", Tag::Wall, _app);
		g->InitAsCube(_app->map->mapObjects[i].dimensions[0] * scale * 9, _app->map->mapObjects[i].dimensions[1] * (rand() %40) + 80 * scale, _app->map->mapObjects[i].dimensions[2] * scale * 9 , 99999);
		g->pBody->SetPos(_app->map->mapObjects[i].position.x + ((_app->map->mapObjects[i].dimensions[0] / 2) * 16), 1, _app->map->mapObjects[i].position.y + ((_app->map->mapObjects[i].dimensions[2] / 2)*16));
		g->pBody->body->setRestitution(0.4f);
		Color wallColors[4] = { Color(1,1,1,1), Color(0.5f, 0.5f, 0.5f, 1), Color(0.4f, 0.4f, 0.4f, 1), Color(0.6f, 0.6f, 0.6f, 1) };
		g->primitive->color = wallColors[rand() % 4];

		gameObjects.add(g);
	}
	return true;
}

bool SceneGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	this->name = "GameScene";

	_app->camera->Move(vec3(-20.0f, 60.0f, -19.0f));
	_app->camera->LookAt(vec3(48, 0, 48));
	_app->map->Load("Map.tmx");

	InitScene();

	//_app->physics->CreateCube(10, 20, 30, 40);
	//_app->physics->CreateArea(4, 6, {10, 0,0});

	Vehicle* v = new Vehicle("vehicle", Tag::Vehicle, _app);
	TaxiClient* t = new TaxiClient("taxiClient", Tag::TaxiClient, _app);
	gameObjects.add(v);
	gameObjects.add(t);
	//gameObjects.add(g);
	Scene::Start();

	Arrow* arrowTest = new Arrow("testarrow", Tag::None, _app, v, t);
	gameObjects.add(arrowTest);

	Booster* boosterTest = new Booster("booster", Tag::Booster, _app,  vec3(27, 1, 300), vec3(45, 4, 20), true);
	gameObjects.add(boosterTest);
	Booster* boosterTest2 = new Booster("booster", Tag::Booster, _app, vec3(517, 1, 690), vec3(20, 4, 70), true);
	gameObjects.add(boosterTest2);
	Booster* boosterTest3 = new Booster("booster", Tag::Booster, _app, vec3(639, 1, 45), vec3(20, 4, 60), true);
	gameObjects.add(boosterTest3);
	Booster* boosterTest4 = new Booster("booster", Tag::Booster, _app, vec3(517, 1, 940), vec3(20, 4, 70), true);
	gameObjects.add(boosterTest4);
	Booster* boosterTest5 = new Booster("booster", Tag::Booster, _app, vec3(610, 1, 190), vec3(20, 4, 90), true);
	gameObjects.add(boosterTest5);
	Booster* boosterTest6 = new Booster("booster", Tag::Booster, _app, vec3(517, 1, 820), vec3(20, 4, 70), true);
	gameObjects.add(boosterTest6);

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

	//Plane p(0, 1, 0, 0);
	//p.color = Green;
	//p.axis = true;
	//p.Render();

	return true;
}

bool SceneGame::CleanUp()
{
	_app->camera->RemoveTarget();

	Scene::CleanUp();

	return true;
}
