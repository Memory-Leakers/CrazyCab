#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "List.h"
#include "glmath.h"

class GameObject;
class btRigidBody;
class Module;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	PhysBody3D(btRigidBody* body, GameObject* gameObject);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	vec3 GetPos();

private:
	btRigidBody* body = nullptr;

public:
	GameObject* gameObject = nullptr;
};

#endif // __PhysBody3D_H__