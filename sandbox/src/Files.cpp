#include "pch.h"
#include "Sandbox/Utils/Files.h"

namespace Sandbox
{
    namespace IO
    {
        std::string IfstreamToString(std::ifstream& file)
        {
            std::ostringstream stream;
            stream << file.rdbuf();
            return stream.str();
        }
    }
}