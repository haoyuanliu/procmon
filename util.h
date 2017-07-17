#include <iostream>
#include <string>
#include <sstream>

class Util{
    public:
        Util();
        ~Util();
    public:
        template<typename T> static std::string transToString(const T &input) {
            std::stringstream ss;
            std::string output;
            ss << input;
            ss >> output;
            return output;
        }
};
