#include "pch.h"
#include "Sandbox/Sandbox.h"
#include "Sandbox/System/Systems.h"
#include "ImGuiSystemTest.h"
#include "entt/entt.hpp"
#include "Sandbox/Time.h"

#include <random>
#include <iostream>


int main(int argc, char** argv)
{

	Sandbox::Systems::Push<ImGuiSystemTest>();
	Sandbox::Launch();

	return 0;
}