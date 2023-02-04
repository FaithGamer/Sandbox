#pragma once

#include "iInput.h"

namespace Sandbox
{
	class TextualInput 
	{
	public:
		InputType GetType() const { return InputType::textual; }

		struct Data
		{
			char text[SDL_TEXTINPUTEVENT_TEXT_SIZE];
		};

		/// State
	private:
		Data m_data;

	};
}