#include "pch.h"
#include "Sandbox/Sandbox.h"
#include "Sandbox/System/Systems.h"
#include "ImGuiSystemTest.h"
#include "entt/entt.hpp"
#include "Sandbox/Time.h"

#include <random>
#include <iostream>

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(0, 99999); // distribution in range [1, 6]

unsigned int Random(unsigned int from, unsigned int to)
{
	std::uniform_int_distribution<std::mt19937::result_type> distrubution(from, to);
	return distrubution(rng);
}

struct componentA
{
	int a = 10;
	std::string b = "Hello World";
	double pouet;
};
struct componentB
{
	std::string b = "Hello World";
};
struct componentC
{
	float a = 10;
	std::string b = "Hello World";
	char* c;
};
struct componentD
{
	int a = 10;
	std::string b = "Hello World";
	componentA comp;
};
struct componentE
{
	int a = 10;
	std::string b = "Hello World";
	componentB B;
};

void EnttSpeedTest()
{

	entt::registry registry;
	std::vector<entt::entity> entities;

	entities.reserve(100000);

	for (int i = 0; i < 100000; i++)
	{
		entt::entity e = registry.create();

		if (Random(0, 2) == 2)
		{
			registry.emplace<componentA>(e);
			entities.emplace_back(e);
		}
		if (Random(0, 2) == 2)
		{
			registry.emplace<componentB>(e);
		}
		if (Random(0, 2) == 2)
		{
			registry.emplace<componentC>(e);
		}
		if (Random(0, 2) == 2)
		{
			registry.emplace<componentD>(e);
		}
		if (Random(0, 2) == 2)
		{
			registry.emplace<componentE>(e);
		}
	}

	Sandbox::Clock T;
	for (int i = 0; i < entities.size(); i++)
	{
		componentA& c = registry.get<componentA>(entities[i]);
		c.a = 12;
	}

	std::cout << "elapsed: " << T.GetElapsed() << std::endl;
	std::cout << "entities with component:" << entities.size() << std::endl;
	std::cout << "average: " << (double)T.GetElapsed() / entities.size() << std::endl;
}