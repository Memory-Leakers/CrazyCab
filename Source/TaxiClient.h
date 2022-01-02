#ifndef __TAXICLIENT_H__
#define __TAXICLIENT_H__

#include "GameObject.h"

class TaxiClient : public GameObject
{
public:

	TaxiClient();
	~TaxiClient();

	void Start() override;
	void Update() override;
	void PostUpdate() override;
	void CleanUp() override;

	void Render();

private:


public:

	iPoint startPos;
	iPoint endPoint;

};

#endif //!__TAXICLIENT_H__
