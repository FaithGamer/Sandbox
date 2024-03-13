#pragma once
#include "Sandbox/Sandbox.h"
#include "Components/Init.h"

using namespace Sandbox;
struct GameStartSignal
{

};
struct ThreadSyncSignal
{
	float delta;
};
class GameManager : public System
{
public:
	GameManager();
	void OnStart() override;
	void OnUpdate(Time delta) override;
	void OnFixedUpdate(Time delta) override;
	void StartGame();
	
	///@brief Only destroy entity this way, for thread safety
	void DestroyEntity(Entity entity);
	///@brief Only create entity this way, for thread safety
	void CreateEntity(sptr<Init> init);

	void QueuePhysicsTask(sptr<OpaqueTask> task);
	void QueueAITask(sptr<OpaqueTask> task);
public:
	
	SignalSender<GameStartSignal> gameStartSignal;
	SignalSender<ThreadSyncSignal> threadSyncSignal;

private:
	void CreateMap();
	void CreateEntities();


	//Threading
	WorkerThread m_aiThread;
	WorkerThread m_physicsThread;

	float m_threadAccumulator;

	std::unordered_set<EntityId> m_entityDestroy;
	std::vector<sptr<Init>> m_entityCreate;

	std::mutex m_createMutex;
	std::mutex m_destroyMutex;


};
