#include "pch.h"
#include "JsonTest.h"
#include "Sandbox/Json.h"
#include "Sandbox/Engine.h"

using namespace Sandbox;

void JsonTest()
{
	Engine::Init();
	Json json;

	json["Float"] = 1.08276f;
	json["Array"] = std::vector<int>{ 1, 1, 2, 4 };
	Json object;
	object["StringArray"] = std::vector<std::string>{ "oui", "non", "peut-etre" };
	json["Object"] = object;

	/*std::ofstream file("jsontest.json");

	file << json;
	file.close();*/

	/*Json jread;

	std::ifstream filei("jsontest.json");

	filei >> jread;

	std::vector<std::string> stringArray = jread["Object"]["StringArray"];
	for (auto& str : stringArray)
	{

		std::cout << str << std::endl;
	}*/

	Config config("jsontest.json");

	std::cout << config.GetFloat("Float") << std::endl;
	std::cout << config.GetFloat("Fzloat") << std::endl;

	Config obj = config.GetConfig("Object");
	
	std::cout << obj.GetString("String") << std::endl;
	std::cout << obj.GetString("Stringz") << std::endl;


}