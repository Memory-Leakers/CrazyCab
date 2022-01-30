#ifndef __SCENEGAME_H__
#define __SCENEGAME_H__

#include "Scene.h"


class Player;

class SceneGame :  public Scene
{
public:
	Player* player = nullptr;

	int i = 0;

	

public:
	SceneGame(Application* app);

	~SceneGame();

	virtual bool InitScene();

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update();

	virtual bool PostUpdate();

	virtual bool CleanUp();

private:
	
	
};

#endif // !__SCENEGAME_H__