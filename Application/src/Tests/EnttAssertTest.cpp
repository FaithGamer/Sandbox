#include "Sandbox/Sandbox.h"
#include "Sandbox/Entt.h"
using namespace Sandbox;
struct comp
{
	int t;
};
void EnttAssertTest()
{
	Engine::Init();

	entt::registry reg;
	entt::entity ent = entt::entity(234);
	//reg.emplace<comp>(ent);
	reg.erase<comp>(ent);
	Engine::Launch();
}