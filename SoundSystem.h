#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

// FMOD
#include "Extern/inc/fmod.hpp"
#include "Extern/inc/fmod_common.h"

bool bSoundMute;
bool bPaused;

enum class SoundCall
{
	// Main menu
	MAINMENU_BGM = 0,

	// Levels
	GAME_BGM,
	PLAYER_ATK, // Melee Attack
	PLAYER_SKILL, // Camera Flash
	PLAYER_JUMP,
	PLAYER_HIT,

	// Boss
	BOSS_BGM, // Default music
	BOSS_SFX, // Tick tock
};


class Sound
{
	static Sound* s_Instance;

	// Create system
	FMOD::System* f_system = nullptr;
	// FMOD channels
	FMOD::Channel* bgm = nullptr;
	FMOD::Channel* sfx = nullptr;
	FMOD::Channel* sfx2 = nullptr;
	// Placeholder
	FMOD::ChannelGroup* master = nullptr;
	FMOD::ChannelGroup* additional = nullptr;

	// Create different sounds
	FMOD::Sound* MenuBGM = nullptr;
	FMOD::Sound* GameBGM = nullptr;
	FMOD::Sound* BossBGM = nullptr;

	FMOD::Sound* PlayerAttack = nullptr;
	FMOD::Sound* PlayerSkill = nullptr;
	FMOD::Sound* PlayerJump = nullptr;
	FMOD::Sound* PlayerHit = nullptr;
	FMOD::Sound* BossSFX = nullptr;

public:
	// Public functions
	void Mute();
	void Pause();
	void Load();
	void Initialize(SoundCall sCase);
	void Update(SoundCall sCase);
	void Free();
	void Unload();
	static Sound* Instance();
	static void UnloadInstance();
};

#endif