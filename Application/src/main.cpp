#include "pch.h"
#include "Sandbox/Sandbox.h"
#include "Sandbox/System/Systems.h"
#include "ImGuiSystemTest.h"

int main(int argc, char** argv)
{
	Sandbox::Systems::Push<ImGuiSystemTest>();
	Sandbox::Launch();

	return 0;
}