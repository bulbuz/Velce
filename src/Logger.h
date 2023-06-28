#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

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
        // Get the current time
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        // Convert the current time to a local time struct
        std::tm* localTime = std::localtime(&currentTime);

        // Extract hours, minutes, and seconds
        int hours = localTime->tm_hour;
        int minutes = localTime->tm_min;
        int seconds = localTime->tm_sec;

        // Output the current time
        buffer() << std::setfill('0');
        buffer() << "[" << std::setw(2) << hours << ":" << std::setw(2) << minutes << ":" << std::setw(2) << seconds << "] " << type << message << "\n";

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
