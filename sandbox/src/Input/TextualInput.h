#pragma once

#include "Input.h"

namespace Sandbox
{
	class TextualInput 
	{
	public:
		InputType GetType() const { return InputType::Textual; }

		struct Data
		{
			char text[SDL_TEXTINPUTEVENT_TEXT_SIZE];
		};

	private:
		Data m_data;

	};
}