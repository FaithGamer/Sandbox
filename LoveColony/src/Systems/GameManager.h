#pragma once
#include "Sandbox/Sandbox.h"

using namespace Sandbox;

class GameManager : public System
{
public:
	GameManager();
	void OnStart() override;
	void OnUpdate(Time delta) override;
	void StartGame();
	
public:
	struct GameStartSignal
	{

	};
	SignalSender<GameStartSignal> gameStartSignal;

private:
	void CreateMap();
	void CreateEntities();

};
