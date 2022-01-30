#include "Follower.h"
#include "Application.h"
#include "ModulePhysics3D.h"

Follower::Follower(std::string name, Tag tag, Application* app, GameObject* target) : GameObject(name, tag, app)
{
	this->target = target;
}

void Follower::Start()
{
	for (int i = 0; i < followers.count(); i++)
	{
		followers[i]->Start();
	}
}

void Follower::Update()
{
	for (int i = 0; i < followers.count(); i++)
	{
		followers[i]->Update();
	}
}

void Follower::PostUpdate()
{
	for (int i = 0; i < followers.count(); i++)
	{
		followers[i]->PostUpdate();
	}
}

void Follower::CleanUp()
{
	for (int i = 0; i < followers.count(); i++)
	{
		followers[i]->CleanUp();
	}

	followers.clearPtr();
}

void Follower::AddFollower()
{
	if (followers.count() == 0)
	{
		// First Follower
		GameObject* follower = new GameObject("follower", Tag::None, _app);

		follower->InitAsCube(1, 1, 1, 0.1);

		follower->pBody->SetPos(target->GetPosition().x + 2, 1, target->GetPosition().z + 2);

		followers.add(follower);

		_app->physics->AddConstraintP2P(*target->pBody, *follower->pBody, { 0,0,0 }, {6,0,0 });
	}
	else 
	{
		GameObject* lastFollower = followers.end->data;

		GameObject* follower = new GameObject("follower", Tag::None, _app);

		follower->InitAsCube(1, 1, 1, 0.1);

		follower->pBody->SetPos(target->GetPosition().x + 2, 1, target->GetPosition().z + 2);

		followers.add(follower);

		_app->physics->AddConstraintP2P(*lastFollower->pBody, *follower->pBody, { 0,0,0 }, {4,0,0 });
	}
}
