#include "pch.h"
#include "Tools/files.h"

namespace Sandbox
{
    namespace Files
    {
        std::string ifstreamToString(std::ifstream& file)
        {
            std::ostringstream stream;
            stream << file.rdbuf();
            return stream.str();
        }
    }
}