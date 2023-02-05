#include "pch.h"
#include "EnttTest.h"
#include "entt_macros.h"
#include "Vec.h"
#include "Signals.h"
#include "Timing.h"
#include "Tools/Toolbox.h"

using namespace Sandbox;

struct move
{
	//we can create reliable pointers on theses components
	//but iterating theses components is gonna be much slower
	PointableComponent 
	Vec2f direction;
};

struct position
{
	//we cannot use pointers of this component
	Vec2f pos;
};

struct control
{
	//we cannot use pointers of this component
	char dummy;
};

entt::registry reg;

namespace Sandbox
{
	void TestEntt()
	{
		LOG_INFO("Test Entt");

		std::vector<entt::entity> entities;
		Clock c;

		//Creating entities and adding some components
		for (int i = 0; i < 100000; i++)
		{
			entities.emplace_back(reg.create());
			if (i % 2)
			{
				reg.emplace<control>(entities.back());
			}
			if (i % 3)
			{
				reg.emplace<position>(entities.back());
			}
			reg.emplace<move>(entities.back());
		}

		std::cout << "Created 100000 entities with some components in: " << c.GetElapsed() << "s" << std::endl;
		
		c.Restart();

		//Acting upon the move component
		reg.view<move>().each([](auto entity, move& m)
			{
				m.direction.x = 5;
			});

		std::cout << "Acted upon move component in : " << c.GetElapsed() << "s" << std::endl;

		c.Restart();

		//Acting upon the position component
		reg.view<position>().each([](auto entity, position& p)
			{
				p.pos.x = 5;
			});

		std::cout << "Acted upon position component in : " << c.GetElapsed() << "s" << std::endl;

		c.Restart();

		//Destroying entities having move and controls components
		int destroyed = 0;
		reg.view<move, control>().each([&destroyed](auto entity, move& m, control& c) {
			reg.destroy(entity);

			destroyed++;
			});

		std::cout << "Destroyed entities with move AND control components in: " << c.GetElapsed() << "s" << std::endl;

	}
}