#include "GameObject.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"

GameObject::GameObject()
{
}

//TODO: Poner Applicationi* en constructor
GameObject::GameObject(std::string name, std::string tag , Application* _app)
{
	this->name = name;
	this->_app = _app;
	this->tag = tag;
}

GameObject::GameObject(GameObject& obj)
{
	this->name = obj.name;
	this->tag = obj.tag;
	//this->pBody = obj.pBody;
}

GameObject::~GameObject()
{
}

//GameObject::~GameObject()
//{
//
//}

void GameObject::OnCollisionEnter(PhysBody3D* col)
{
}

//void GameObject::OnCollisionEnter(PhysBody* col)
//{
//}
//
//void GameObject::OnCollisionExit(PhysBody* col)
//{
//
//}
//
void GameObject::OnTriggerEnter(PhysBody3D* col)
{
}
//
//void GameObject::OnTriggerStay(PhysBody* trigger, PhysBody* col)
//{
//}
//
//void GameObject::OnTriggerExit(PhysBody* trigger, PhysBody* col)
//{
//}

void GameObject::Start()
{
	//Cargar texturas
}

void GameObject::PreUpdate()
{
}

void GameObject::Update()
{
}

void GameObject::PostUpdate()
{
	
}

void GameObject::CleanUp()
{
}

bool GameObject::CompareTag(std::string tag)
{
	if(this->tag == tag)
	{
		return true;
	}

	return false;
}