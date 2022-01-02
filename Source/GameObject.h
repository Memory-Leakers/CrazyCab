#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include "Point.h"
#include "Application.h"
#include "glmath.h"

#define MAX_GAMEOBJECT_TEXTURES 10

class PhysBody3D;

class GameObject 
{
public:

	GameObject();

	GameObject(std::string name = "Default", std::string tag = "None", Application* _app = nullptr);

	GameObject(GameObject& obj);

	virtual ~GameObject();

	virtual void OnCollisionEnter(PhysBody3D* col);
	//virtual void OnCollisionExit(PhysBody* col);

	//virtual void OnTriggerEnter(PhysBody* trigger, PhysBody* col);
	//virtual void OnTriggerStay(PhysBody* trigger, PhysBody* col);
	//virtual void OnTriggerExit(PhysBody* trigger, PhysBody* col);

	virtual void Start();

	virtual void PreUpdate();

	virtual void Update();

	virtual void PostUpdate();

	virtual void CleanUp();

	bool CompareTag(std::string tag);

	vec3 GetPosition() const;

private :
	iPoint position = { 0,0 };

	float rotation = 0;

protected:
	Application* _app = nullptr;

public:
	std::string name = "";

	std::string tag = "";

	PhysBody3D* pBody = nullptr;

	bool pendingToDelete = false;

	bool adjustToGrid = false;

	bool isDie = false;

	bool enable = true;
};

#endif // !__GAMEOBJECT_H__