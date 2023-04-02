#include "pch.h"
#include "Sandbox/Files.h"

namespace Sandbox
{
    namespace IO
    {
        std::string ifstreamToString(std::ifstream& file)
        {
            std::ostringstream stream;
            stream << file.rdbuf();
            return stream.str();
        }
    }
}