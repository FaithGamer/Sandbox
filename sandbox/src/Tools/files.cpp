#include "pch.h"
#include "Tools/files.h"

namespace sandbox
{
    namespace files
    {
        std::string ifstreamToString(std::ifstream& file)
        {
            std::ostringstream stream;
            stream << file.rdbuf();
            return stream.str();
        }
    }
}