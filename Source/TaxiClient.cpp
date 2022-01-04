#include "TaxiClient.h"
#include "ModulePhysics3D.h"
#include "ModuleInput.h"
#include <iostream>

TaxiClient::TaxiClient(std::string name, Tag tag, Application* _app) : GameObject(name, tag, _app)
{
}

TaxiClient::~TaxiClient()
{
}

void TaxiClient::Start()
{
	generatePosition();
	pBody = _app->physics->CreateArea(radius, height, *originPos);
	pBody->gameObject = this;
	//pBody2 = _app->physics->CreateArea(radius, height, *destinationPos);
}

void TaxiClient::PreUpdate()
{
	//Gets position from physics body
	this->position = pBody->GetPos();
}

void TaxiClient::Update()
{
	if (!onTaxi) //Client remains to be picked up
	{
		if (originPos == nullptr || destinationPos == nullptr) //Client doesn't have position
		{
			generatePosition();
		}
	}
}

void TaxiClient::PostUpdate()
{
	if (_app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		generatePosition();
		std::cout << "N�" << serviceCount << std::endl;
	}
}

void TaxiClient::CleanUp()
{
	RELEASE(originPos);
	RELEASE(destinationPos);
}

void TaxiClient::OnTriggerEnter(PhysBody3D* col)
{
	if (col->gameObject->tag == Tag::Vehicle)
	{
		std::cout << "Vehicle entered ClientTaxi Area" << std::endl;

		if (!onTaxi) //On Origin
		{
			if (pBody != nullptr)
			{
				pBody->SetPos(destinationPos->x, destinationPos->y, destinationPos->z);
			}
			onTaxi = true;
		}
		else //On destination
		{
			generatePosition();
			onTaxi = false;
			serviceCount++;
			std::cout << "N�" << serviceCount << std::endl;
		}
	}
}

void TaxiClient::generatePosition()
{
	float xOrigin, zOrigin, xDestination, zDestination, distance;
	xOrigin = rand() % 500 + 1;
	zOrigin = rand() % 500 + 1;
	xDestination = rand() % 500 + 1;
	zDestination = rand() % 500 + 1;

	distance = abs(sqrt(xOrigin * xOrigin + zOrigin * zOrigin) - sqrt(xDestination * xDestination + zDestination * zDestination));

	while (distance < 100) //Prevents the destination from being to close to the origin
	{
		xDestination = rand() % 400 + 1;
		zDestination = rand() % 400 + 1;

		distance = abs(sqrt(xOrigin * xOrigin + zOrigin * zOrigin) - sqrt(xDestination * xDestination + zDestination * zDestination));
	}


	RELEASE(originPos);
	RELEASE(destinationPos);
	originPos = new vec3({xOrigin, 16.0f, zOrigin});
	destinationPos = new vec3({ xDestination, 16.0f, zDestination });

	if (pBody != nullptr)
	{
		pBody->SetPos(originPos->x, originPos->y, originPos->z);
		std::cout << "Origin: x(" << originPos->x << ") y(" << originPos->y << ") z(" << originPos->z << ")" << std::endl;
		std::cout << "Destination: x(" << destinationPos->x << ") y(" << destinationPos->y << ") z(" << destinationPos->z << ")" << std::endl;
	}

	/*if (pBody2 != nullptr)
	{
		pBody2->SetPos(destinationPos->x, destinationPos->y, destinationPos->z);
		std::cout << "Destination: x(" << destinationPos->x << ") y(" << destinationPos->y << ") z(" << destinationPos->z << ")" << std::endl;
	}*/
}