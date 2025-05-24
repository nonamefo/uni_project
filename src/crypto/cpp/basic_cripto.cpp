#include "../../project_libs.h"

class basic_cripto{
    public:
        virtual std::string decode(const std::string message) = 0;
        virtual std::string encode(const std::string message) = 0;
};
