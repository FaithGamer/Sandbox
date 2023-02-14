#include "pch.h"
#include "Render/WindowGLContext.h"
#include "Render/ShaderProgram.h"
#include "Render/Buffer.h"
#include "Render/Vertex.h"
#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Transform.h"
#include "Render/Camera.h"
#include "Render/BatchRenderer.h"
#include "Input/ButtonInput.h"
#include "Input/InputMap.h"
#include "Log.h"
#include "TypeId.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <entt/src/entt/entt.hpp>

#include "Timing.h"


#include "entt_macros.h"

#include "Tools/Toolbox.h"

#include "Signals.h"

#include "Tests/SignalTest.h"
#include "Tests/EnttTest.h"
#include "Tests/ToolboxTest.h"
#include "Tests/UniformBlockTest.h"
#include "Tests/BillboardTest.h"
#include "Tests/BatchRendererTest.h"
#include "Tests/TextureUnitTest.h"

using namespace Sandbox;


int main(int argc, char* argv[])
{

	Log::Init();
	LOG_INFO("Logger initialiazed");

	//TestSignal();
	//TestEntt();
	//TestToolbox();
	//UniformBlockTest();
	BatchRendererTest();
	//TextureUnitTest();
	return 0;

}