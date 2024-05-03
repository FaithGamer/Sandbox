#include "pch.h"
#include "AudioTest.h"
#include "Sandbox/Engine.h"
#include "Sandbox/ECS/System.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Audio/Audio.h"

using namespace Sandbox;

class AudioSys : public System
{
public:
	
};
void AudioTest()
{
	Engine::Init();
	Systems::Push<AudioSys>();
	Audio::MakeSound("assets/audio/checkpoint.wav");
	Engine::Launch();
}