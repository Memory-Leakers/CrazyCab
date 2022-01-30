#ifndef __SCENEGAME_H__
#define __SCENEGAME_H__

#include "Scene.h"

class Radios;

class SceneGame :  public Scene
{
public:
	Radios* radioManager = nullptr;

public:
	SceneGame(Application* app);

	~SceneGame();

	virtual bool InitScene();

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update();

	virtual bool PostUpdate();

	virtual bool CleanUp();
};

#endif // !__SCENEGAME_H__