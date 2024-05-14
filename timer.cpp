// struct account{
//     std::string username;
//     std::string password;
//     int id;
// };

// struct settings{
//     int pomodoro;
//     int short_break;
//     int long_break;
//     int break_interval;
// };

#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

void startTimer(const std::string &timerName, int durationInSeconds) {
    std::cout << timerName << " for " << durationInSeconds / 60 << " minutes started." << std::endl;
    for (int remainingSeconds = durationInSeconds; remainingSeconds >= 0; --remainingSeconds) {
        int minutes = remainingSeconds / 60;
        int seconds = remainingSeconds % 60;
        std::cout << "\r" << std::setw(2) << std::setfill('0') << minutes << ":"
                  << std::setw(2) << std::setfill('0') << seconds << " remaining" << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << std::endl << timerName << " ended." << std::endl;
}

int main() {
    const int workDuration = 25 * 60; // 25 minutes
    const int shortBreakDuration = 5 * 60; // 5 minutes
    const int longBreakDuration = 15 * 60; // 15 minutes
    const int sessionsBeforeLongBreak = 4; // Number of work sessions before a long break

    int sessionCount = 0;

    while (true) {
        // Start the work session
        startTimer("Work session", workDuration);
        sessionCount++;

        if (sessionCount % sessionsBeforeLongBreak == 0) {
            // Long break after the specified number of work sessions
            startTimer("Long break", longBreakDuration);
        } else {
            // Short break after each work session
            startTimer("Short break", shortBreakDuration);
        }
    }

    return 0;
}
