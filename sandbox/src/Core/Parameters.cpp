#include "pch.h"
#include "Core/Parameters.h"

namespace Sandbox
{
	void Parameters::SetInitalParameters(const EngineParameters& parameters)
	{
		Parameters::Get()->m_initialParameters = parameters;
	}
}