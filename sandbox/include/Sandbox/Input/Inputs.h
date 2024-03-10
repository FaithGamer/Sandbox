#pragma once
#include <string>

#include "Sandbox/Input/InputMap.h"
#include "Sandbox/Internal/Singleton.h"


namespace Sandbox
{
	struct InputMapContainer
	{
		sptr<InputMap> Add(std::string name);
		sptr<InputMap> Get(std::string name);
		void Remove(std::string name);

		std::vector<sptr<InputMap>> inputs;
		std::vector<std::string> names;
	};

	class Inputs : public Singleton<Inputs>
	{
	public:
		Inputs();
		/// @brief Create an input map with a default name "InputMap_0, 1, 2..."
		/// @return shared pointer to the created input map
		static sptr<InputMap> CreateInputMap();
		/// @brief Create an input map with a name
		/// @return shared pointer to the created input map
		static sptr<InputMap> CreateInputMap(std::string name);
		/// @brief Delete an input map based on the name
		/// @param name Name on the input map
		static void DestroyInputMap(std::string name);

		static std::vector<sptr<InputMap>>& GetInputMaps();
		static sptr<InputMap> GetInputMap(std::string name);
		static std::vector<std::string> GetInputMapNameList();

	private:
		friend Engine;
		InputMapContainer m_inputMaps;
		std::vector<SDL_Joystick*> m_controllers;
		
	};
}
