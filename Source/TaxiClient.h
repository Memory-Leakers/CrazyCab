#ifndef __TAXICLIENT_H__
#define __TAXICLIENT_H__

#include "GameObject.h"

enum TaxiClientState
{
	WaitingTaxi = 0,
	OnTaxi
};


class TaxiClient : public GameObject
{
public:

	TaxiClient(std::string name, Tag tag, Application* _app);
	~TaxiClient();

	void Start() override;
	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;
	void CleanUp() override;

	void OnTriggerEnter(PhysBody3D* col) override;

	void generatePosition();

private:
	float height = 64.0f;
	float radius = 16.0f;

	
	
public:

	int serviceCount = 0;
	bool onTaxi = false;

	vec3* originPos = nullptr;
	vec3* destinationPos = nullptr;

	PhysBody3D* pBody2 = nullptr;

public:
	/*GETTERS AND SETTERS*/

	void SetHeight(float height)
	{
		this->height = height;
	}

	float GetHeight()
	{
		return height;
	}

	void SetRadius(float radius)
	{
		this->radius = radius;
	}

	float GetRadius()
	{
		return radius;
	}

};

#endif //!__TAXICLIENT_H__
