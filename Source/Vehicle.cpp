#include "Vehicle.h"
#include "Primitive.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "External/Bullet/include/btBulletDynamicsCommon.h"
#include <iostream>

Vehicle::Vehicle(std::string name, Tag tag, Application* _app, btRaycastVehicle* vehicle, const VehicleInfo& info) :GameObject(name, tag, _app)
{
}

Vehicle::Vehicle(std::string name, Tag tag, Application* _app):GameObject(name, tag, _app)
{
}

Vehicle::~Vehicle()
{
	RELEASE(vehicle);
	RELEASE(info);
}

VehicleInfo::~VehicleInfo()
{
	RELEASE_ARRAY(wheels);
}

void Vehicle::Start()
{
	ObserverPos.Set(0, 6, -20);

	info = new VehicleInfo();
	// Car properties ----------------------------------------
	info->chassis_size.Set(3, 1.2f, 4);
	info->chassis_offset.Set(0, 0.6f, -0.1f);

	info->mass = 500.0f;
	info->suspensionStiffness = 15.88f;
	info->suspensionCompression = 0.83f;
	info->suspensionDamping = 0.88f;
	info->maxSuspensionTravelCm = 1000.0f;
	info->frictionSlip = 50.5;
	info->maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------
	float half_width = info->chassis_size.x * 0.5f;
	float half_length = info->chassis_size.z * 0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	info->num_wheels = 4;
	info->wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	info->wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	info->wheels[0].direction = direction;
	info->wheels[0].axis = axis;
	info->wheels[0].suspensionRestLength = suspensionRestLength;
	info->wheels[0].radius = wheel_radius;
	info->wheels[0].width = wheel_width;
	info->wheels[0].front = true;
	info->wheels[0].drive = true;
	info->wheels[0].brake = false;
	info->wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	info->wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	info->wheels[1].direction = direction;
	info->wheels[1].axis = axis;
	info->wheels[1].suspensionRestLength = suspensionRestLength;
	info->wheels[1].radius = wheel_radius;
	info->wheels[1].width = wheel_width;
	info->wheels[1].front = true;
	info->wheels[1].drive = true;
	info->wheels[1].brake = false;
	info->wheels[1].steering = true;

	// REAR-LEFT ------------------------
	info->wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	info->wheels[2].direction = direction;
	info->wheels[2].axis = axis;
	info->wheels[2].suspensionRestLength = suspensionRestLength;
	info->wheels[2].radius = wheel_radius;
	info->wheels[2].width = wheel_width;
	info->wheels[2].front = false;
	info->wheels[2].drive = false;
	info->wheels[2].brake = true;
	info->wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	info->wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	info->wheels[3].direction = direction;
	info->wheels[3].axis = axis;
	info->wheels[3].suspensionRestLength = suspensionRestLength;
	info->wheels[3].radius = wheel_radius;
	info->wheels[3].width = wheel_width;
	info->wheels[3].front = false;
	info->wheels[3].drive = false;
	info->wheels[3].brake = true;
	info->wheels[3].steering = false;

	/*vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 12, 10);*/

	btCompoundShape* comShape = new btCompoundShape();
	_app->physics->shapes.add(comShape);

	// Col
	btCollisionShape* colShape = new btBoxShape(btVector3(info->chassis_size.x * 0.5f, info->chassis_size.y * 0.5f, info->chassis_size.z * 0.5f));
	_app->physics->shapes.add(colShape);

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(info->chassis_offset.x, info->chassis_offset.y, info->chassis_offset.z));

	comShape->addChildShape(trans, colShape);
	//Col

	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 localInertia(0, 0, 0);
	comShape->calculateLocalInertia(info->mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	_app->physics->motions.add(myMotionState);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(info->mass, myMotionState, comShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	body->setActivationState(DISABLE_DEACTIVATION);
	body->setAngularFactor(btVector3(0, 1, 0));

	_app->physics->world->addRigidBody(body);

	btRaycastVehicle::btVehicleTuning tuning;
	tuning.m_frictionSlip = info->frictionSlip;
	tuning.m_maxSuspensionForce = info->maxSuspensionForce;
	tuning.m_maxSuspensionTravelCm = info->maxSuspensionTravelCm;
	tuning.m_suspensionCompression = info->suspensionCompression;
	tuning.m_suspensionDamping = info->suspensionDamping;
	tuning.m_suspensionStiffness = info->suspensionStiffness;

	vehicle = new btRaycastVehicle(tuning, body, _app->physics->vehicle_raycaster);

	vehicle->setCoordinateSystem(0, 1, 2);

	for (int i = 0; i < info->num_wheels; ++i)
	{
		btVector3 conn(info->wheels[i].connection.x, info->wheels[i].connection.y, info->wheels[i].connection.z);
		btVector3 dir(info->wheels[i].direction.x, info->wheels[i].direction.y, info->wheels[i].direction.z);
		btVector3 axis(info->wheels[i].axis.x, info->wheels[i].axis.y, info->wheels[i].axis.z);

		vehicle->addWheel(conn, dir, axis, info->wheels[i].suspensionRestLength, info->wheels[i].radius, tuning, info->wheels[i].front);
	}

	pBody = new PhysBody3D(body, this);

	body->setUserPointer(pBody);
	pBody->SetPos(100, 5, 100);

	_app->physics->bodies.add(pBody);

	_app->physics->world->addVehicle(vehicle);
}

void Vehicle::Update()
{
	if (_app->physics->debug)
	{
		vec3 v = pBody->GetPos();
		//std::cout << pBody->gameObject->name << ": x(" << v.x << ") y(" << v.y << ") z(" << v.z << ")" << std::endl;
	}
	if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		ApplyEngineForce(speed + acceleration);
	}
	if(_app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		ApplyEngineForce(-speed + acceleration);
	}
	if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP || _app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		ApplyEngineForce(0);
	}


	if (_app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -15)
		{
			turn -= rotateSpeed * _app->fps;
			Turn(turn * DEGTORAD);
		}
	}
	else if (_app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < 15)
		{
			turn += rotateSpeed * _app->fps;
			Turn(turn * DEGTORAD);
		}
	}
	else if(turn != 0.0f)
	{
		if (abs(turn) <= _app->fps)
		{
			turn = 0.0f;
		}
		turn += turn > 0 ? (-rotateSpeed * _app->fps) : (rotateSpeed * _app->fps);

		Turn(turn * DEGTORAD);
	}

	if(_app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		acceleration = 1000.0f;
	}
	else if (_app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
	{
		acceleration = 0.0f;
	}

	if (_app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		Brake(GetKmh()*2);
		//info->mass = 5000;

		//vehicle.
	}
	else if(_app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_UP)
	{
		Brake(0);
		//info->mass = 500;
	}
}

void Vehicle::PostUpdate()
{
	Render();
}

void Vehicle::CleanUp()
{
}

vec3 Vehicle::GetObserverPos()
{
	vec3 ret;

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	Cube tempCube(1,1,1);

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&tempCube.transform);

	btVector3 tempPos(ObserverPos.x, ObserverPos.y, ObserverPos.z);

	tempPos = tempPos.rotate(q.getAxis(), q.getAngle());

	tempCube.transform.M[12] += tempPos.getX();
	tempCube.transform.M[13] += tempPos.getY();
	tempCube.transform.M[14] += tempPos.getZ();

	ret.Set(tempCube.transform.M[12], tempCube.transform.M[13], tempCube.transform.M[14]);

	return ret;
}

void Vehicle::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info->wheels[0].radius;
		wheel.height = info->wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info->chassis_size.x, info->chassis_size.y, info->chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info->chassis_offset.x, info->chassis_offset.y, info->chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	chassis.Render();
}

void Vehicle::ApplyEngineForce(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info->wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

void Vehicle::Brake(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info->wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

/// <summary>
///
/// </summary>
/// <param name="degrees"> THIS IS A RAD!!!!!!!</param>
void Vehicle::Turn(float degrees)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info->wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

float Vehicle::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}
