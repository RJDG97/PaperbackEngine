#include "SoundSystem.h"

// Declaring class variable (Singleton)
Sound* Sound::s_Instance = nullptr;
// Bool to mute sounds
extern bool bSoundMute = false;
extern bool bPaused = false;

void Sound::Mute()
{
	// Get status of sound (Playing or Paused)
	bgm->getMute(&bSoundMute);
	bSoundMute = !bSoundMute;
	// Update status of sound
	bgm->setMute(bSoundMute);
}

void Sound::Pause()
{
	// Update status of sound
	bgm->setPaused(bPaused);
}

void Sound::Load()
{
	// Create system
	FMOD::System_Create(&f_system);
	// Initialize system
	f_system->init(32, FMOD_INIT_NORMAL, f_system);
}

void Sound::Initialize(SoundCall sCase)
{
	// Load sounds
	f_system->createSound("../Assets/Music/MenuBGM.wav", FMOD_LOOP_NORMAL, 0, &MenuBGM);
	f_system->createSound("../Assets/Music/GameBGM.wav", FMOD_LOOP_NORMAL, 0, &GameBGM);
	f_system->createSound("../Assets/Music/BossBGM.wav", FMOD_LOOP_NORMAL, 0, &BossBGM);
	f_system->createSound("../Assets/Music/PlayerAttack.wav", FMOD_DEFAULT, 0, &PlayerAttack);
	f_system->createSound("../Assets/Music/PlayerHit.wav", FMOD_DEFAULT, 0, &PlayerHit);
	f_system->createSound("../Assets/Music/PlayerJump.wav", FMOD_DEFAULT, 0, &PlayerJump);
	f_system->createSound("../Assets/Music/CameraFlash.wav", FMOD_DEFAULT, 0, &PlayerSkill);
	f_system->createSound("../Assets/Music/BossSFX.wav", FMOD_LOOP_NORMAL, 0, &BossSFX);

	// Load background sounds that will loop
	switch (sCase)
	{
	case SoundCall::MAINMENU_BGM:
		bgm->setVolume(0.1f);
		f_system->playSound(MenuBGM, 0, false, &bgm); // Sound set to menu's music & allocated to bgm channel
		break;
	case SoundCall::GAME_BGM:
		f_system->playSound(GameBGM, 0, false, &bgm); // Sound set to game's music & allocated to bgm channel
		break;
	case SoundCall::BOSS_BGM:
		f_system->playSound(BossBGM, 0, false, &bgm); // Sound set to boss's music & allocated to bgm channel
		break;
	}
	bgm->setMute(bSoundMute);
}

void Sound::Update(SoundCall sCase)
{
	(void)sCase;
	// Pauses BGM soundtrack
	//if (AEInputCheckTriggered(AEVK_L))
	//{
	//	bool bSoundPaused;
	//	// Get status of sound (Playing or Paused)
	//	bgm->getPaused(&bSoundPaused);
	//	bSoundPaused = !bSoundPaused;
	//	// Update status of sound
	//	bgm->setPaused(bSoundPaused);
	//}


	//// Switch between different enum cases to play different sounds
	//if (current == GS_LEVEL1 || current == GS_LEVEL2 || current == GS_LEVEL3 || current == GS_BOSS_STAGE)
	//{
	//	switch (sCase)
	//	{
	//	case SoundCall::PLAYER_ATK:
	//		f_system->playSound(PlayerAttack, 0, false, &sfx);
	//		break;

	//	case SoundCall::PLAYER_SKILL:
	//		f_system->playSound(PlayerSkill, 0, false, &sfx);
	//		break;

	//	case SoundCall::PLAYER_JUMP:
	//		f_system->playSound(PlayerJump, 0, false, &sfx);
	//		break;

	//	case SoundCall::PLAYER_HIT:
	//		f_system->playSound(PlayerHit, 0, false, &sfx);
	//		break;

	//	case SoundCall::BOSS_SFX:
	//		f_system->playSound(BossSFX, 0, false, &sfx);
	//		break;

	//	default:
	//		break;
	//	}
	//}


	// Toggle mute
	sfx->setMute(bSoundMute);
}

void Sound::Free()
{
	// Free allocated channels to stop music
	bgm->stop();
	sfx->stop();
}

void Sound::Unload()
{
	// Free allocated tracks
	PlayerAttack->release();
	PlayerHit->release();
	PlayerJump->release();
	PlayerSkill->release();
	BossSFX->release();

	// Terminate system
	f_system->close();
	f_system->release();
}

Sound* Sound::Instance()
{
	// Create a new Sound class
	if (!s_Instance)
		s_Instance = new Sound;
	return s_Instance;
}

void Sound::UnloadInstance()
{
	// Delete Sound class
	delete s_Instance;
	s_Instance = nullptr;
}