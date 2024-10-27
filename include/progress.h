#ifndef PROGRESS_H
#define PROGRESS_H

#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>

namespace progress {

// Function to show progress in a terminal-friendly manner
void show_progress(std::atomic<size_t>& current, size_t total) {
    while (current < total) { // Updated condition to strictly be less than total
        int bar_width = 50;
        float progress = static_cast<float>(current) / total;

        std::cout << "[";
        int pos = static_cast<int>(bar_width * progress);
        for (int i = 0; i < bar_width; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // Ensure progress shows 100% upon completion
    int bar_width = 50;
    std::cout << "[";
    for (int i = 0; i < bar_width; ++i) {
        std::cout << "=";
    }
    std::cout << "] 100 %\n";
}

} // namespace progress

#endif // PROGRESS_H
