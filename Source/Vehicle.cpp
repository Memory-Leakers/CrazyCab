#include "Vehicle.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "Smoke.h"
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

	/*info->test1_size.Set(1, 1, 1);
	info->test1_offset.Set(5, 2.5f, 8.5f);
	*/

	info->mass = 1000.0f;
	info->suspensionStiffness = 15.88f;
	info->suspensionCompression = 0.83f;
	info->suspensionDamping = 0.88f;
	info->maxSuspensionTravelCm = 1000.0f;
	info->frictionSlip = 5000.5f;
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

	btCompoundShape* comShape = new btCompoundShape();
	_app->physics->shapes.add(comShape);

	// Collision shape
	btCollisionShape* colShape = new btBoxShape(btVector3(info->chassis_size.x * 0.5f, info->chassis_size.y * 0.5f, info->chassis_size.z * 0.5f));
	_app->physics->shapes.add(colShape);

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(info->chassis_offset.x, info->chassis_offset.y, info->chassis_offset.z));

	comShape->addChildShape(trans, colShape);
	// Collision shape

	/*
	btCollisionShape* colShape2 = new btBoxShape(btVector3(info->test1_size.x * 0.5f, info->test1_size.y * 0.5f, info->test1_size.z * 0.5f));
	_app->physics->shapes.add(colShape2);

	btTransform trans2;
	trans2.setIdentity();
	trans2.setOrigin(btVector3(info->test1_offset.x, info->test1_offset.y, info->test1_offset.z));

	comShape->addChildShape(trans2, colShape2);
	*/

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

	body->setFriction(1000);
	body->setRestitution(5000);

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
	UpdateRotateSpeed();

	vehicle->updateVehicle(_app->fps);

	// Brake
	if (_app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		Brake(100);

		turn = 5.0f;

		if (smokeStep <= 0)
		{
			// Calculate pos & rot
			mat4x4 tempMatrix;

			vehicle->getChassisWorldTransform().getOpenGLMatrix(&tempMatrix);
			btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
			btVector3 offset(0, 0, -3);
			offset = offset.rotate(q.getAxis(), q.getAngle());

			tempMatrix.M[12] += offset.getX();
			tempMatrix.M[13] += offset.getY();
			tempMatrix.M[14] += offset.getZ();

			vec3 pos = vec3(tempMatrix.M[12], tempMatrix.M[13], tempMatrix.M[14]);

			float r = ((rand() % 10) + 5) / 10;

			Smoke* s = new Smoke(_app, r, pos);

			smokeStep = 0.1f;
		}
	}
	else if (_app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_UP)
	{
		Brake(0);
		turn = 3.0f;
	}

	// Go ahead
	if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (GetKmh() < maxVelocity)
		{
			ApplyEngineForce(speed + acceleration);
		}
		else
		{
			ApplyEngineForce(0);
		}
	}
	// Back
	if(_app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (GetKmh() > -maxVelocity)
		{
			ApplyEngineForce(-speed);
		}		
		else
		{
			ApplyEngineForce(0);
		}
	}
	// Stop add force
	if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP || _app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		ApplyEngineForce(0);
	}

	// Rotate
	if (_app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		// Apply rotation
		Turn(-turn * DEGTORAD);
	}
	else if (_app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{		
		// Apply rotation
		Turn(turn * DEGTORAD);
	}
	else
	{
		// Reset rotation
		Turn(0);
	}

	// Accelerate
	if(_app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		acceleration = 4000.0f;
		maxVelocity = 240.0f;

		if (smokeStep <= 0);
		{
			// Calculate pos & rot
			mat4x4 tempMatrix;

			vehicle->getChassisWorldTransform().getOpenGLMatrix(&tempMatrix);
			btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
			btVector3 offset(1, 0, -3);
			offset = offset.rotate(q.getAxis(), q.getAngle());

			tempMatrix.M[12] += offset.getX();
			tempMatrix.M[13] += offset.getY();
			tempMatrix.M[14] += offset.getZ();

			vec3 pos = vec3(tempMatrix.M[12], tempMatrix.M[13], tempMatrix.M[14]);

			//
			mat4x4 tempMatrix2;

			vehicle->getChassisWorldTransform().getOpenGLMatrix(&tempMatrix2);
			btQuaternion q2 = vehicle->getChassisWorldTransform().getRotation();
			btVector3 offset2(-1, 0, -3);
			offset2 = offset2.rotate(q2.getAxis(), q2.getAngle());

			tempMatrix2.M[12] += offset2.getX();
			tempMatrix2.M[13] += offset2.getY();
			tempMatrix2.M[14] += offset2.getZ();

			vec3 pos2 = vec3(tempMatrix2.M[12], tempMatrix2.M[13], tempMatrix2.M[14]);
			//

			float r = ((rand() % 10) + 5) / 10;

			int randColor = rand() % 2;

			Color color = Color(1.0f, 0.0f, 0.0f);

			color.g = ((float)(rand() % 100) + 1) / 100.0f;

			Smoke* s = new Smoke(_app, r, pos, color);

			Smoke* s2 = new Smoke(_app, r, pos2, color);
	
			smokeStep = 0.1f;
		}
	}
	else if (_app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
	{
		acceleration = 0.0f;
		maxVelocity = 180.0f;
	}

	smokeStep -= _app->fps;
}

void Vehicle::PostUpdate()
{
	Render();

	vec3 pos = GetPosition();
	pos.z -= 3;
	vehicle->getChassisWorldTransform().getRotation();
	
	//pipe.SetPos(pos.x, pos.y, pos.z);

	//pipe.Render();
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

	wheel.color = Black;

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

	/*
	Cube chassis2(info->test1_size.x, info->test1_size.y, info->test1_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis2.transform);
	btQuaternion q2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset2(info->test1_offset.x, info->test1_offset.y, info->test1_offset.z);
	offset2 = offset2.rotate(q2.getAxis(), q2.getAngle());

	chassis2.transform.M[12] += offset2.getX();
	chassis2.transform.M[13] += offset2.getY();
	chassis2.transform.M[14] += offset2.getZ();

	chassis2.Render();
	*/
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

void Vehicle::UpdateRotateSpeed()
{
	int currentSpeed = GetKmh();

	if (abs(currentSpeed) < 50)
	{
		turn = 15;
	}
	else if (abs(currentSpeed) < 100)
	{
		turn = 3;
	}
	else
	{
		turn = 2.0f;
	}

	printf("%f\n", turn);
}
