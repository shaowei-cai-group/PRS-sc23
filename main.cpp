#include "solve.hpp"
#include <chrono>
#include <thread> 

int main(int argc, char **argv) {
    auto startp = std::chrono::high_resolution_clock::now();
    solve(argc, argv);
    auto readf = std::chrono::high_resolution_clock::now();
    printf("c time %.3lfs\n", std::chrono::duration_cast<std::chrono::milliseconds>(readf - startp).count() / 1000.0);
    return 0;
}