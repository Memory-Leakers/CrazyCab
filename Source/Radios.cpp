#include "Radios.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"

Radios::Radios(Application* _app) :GameObject("radios",Tag::None,_app)
{

}

void Radios::Start()
{
	playLists[0].path = "Assets/Audios/PlayList1.ogg";
	playLists[0].duration = 745;

	playLists[1].path = "Assets/Audios/PlayList2.ogg";
	playLists[1].duration = 621;

	currentList = rand() % 2;

	_app->audio->PlayMusic(playLists[currentList].path.c_str());

	Mix_VolumeMusic(50);

	changeSceneSfxId = _app->audio->LoadFx("Assets/Audios/changeList.mp3",25) - 1;
}

void Radios::Update()
{
	timer += _app->fps;

	if (_app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		currentList--;

		currentList = currentList < 0 ? 1 : currentList;

		_app->audio->PlayFx(changeSceneSfxId, 0);

		ChangeList();
	}
	else if (_app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		currentList++;

		currentList = currentList > 1 ? 0 : currentList;

		_app->audio->PlayFx(changeSceneSfxId, 0);

		ChangeList();
	}
}

void Radios::OnCollisionEnter(PhysBody3D* col)
{
}

void Radios::OnTriggerEnter(PhysBody3D* col)
{
}

void Radios::ChangeList()
{
	timer = timer > playLists[currentList].duration ? (int)timer % playLists[currentList].duration : timer;

	_app->audio->PlayMusic(playLists[currentList].path.c_str(),0);

	Mix_SetMusicPosition(timer);
}
