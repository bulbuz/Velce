#pragma once

#include <sstream>
#include <string>
#include <type_traits>

class Logger {
public:
    enum class MODE { SYSTEM, DEBUG, INFO, ERROR };

    template<typename T>
    static void LOG(MODE mode, T message) {
        std::string type = "";
        switch (mode) {
            case MODE::DEBUG:
                type += "[DEBUG]: ";
                break;
            case MODE::SYSTEM:
                type += "[SYSTEM]: ";
                break;
            case MODE::INFO:
                type += "[INFO]: ";
                break;
            case MODE::ERROR:
                type += "[ERROR]: ";
                break;
        }
        buffer() << type << message << "\n";
    }

    static std::string GetBuffer() {
        return buffer().str();
    }

private:
    static std::stringstream& buffer() {
        static std::stringstream instance;
        return instance;
    }
};
