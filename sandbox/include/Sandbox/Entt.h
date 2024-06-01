#pragma once
#include "Sandbox/Core/Log.h"
#define ENTT_ASSERT(condition, msg) if(condition==0){Sandbox::Log::Instance()->GetLogger()->error(msg);Sandbox::Log::Instance()->GetLogger()->flush();} assert(condition);
#include <entt/entt.hpp>