#include "../src/matOps.hpp"
#include <chrono>

int main() {

    size_t rowsA = 10000;
    size_t colsA = 100;
    size_t rowsB = 100;
    size_t colsB = 10000;
    size_t iterations = 10;

    Matrix A = Matrix::constValMatrix(rowsA, colsA, 1.0);
    Matrix B = Matrix::constValMatrix(rowsB, colsB, 2.0);

    // Warm-up multiplication to mitigate any first-run overhead
    Matrix warmup = A * B;

    std::vector<double> times(iterations);

    std::cout << "\nStarting benchmark (" << iterations << " iterations)..." << std::endl;
    for (size_t i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        Matrix result = A * B;
        auto end = std::chrono::high_resolution_clock::now();

        // Measure duration in milliseconds
        double duration = std::chrono::duration<double, std::milli>(end - start).count();
        times[i] = duration;

        std::cout << "Iteration " << (i + 1) << ": " << duration << " ms" << std::endl;
    }

    // Calculate statistics: average, minimum, maximum, and standard deviation
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double average = sum / iterations;
    double min_time = *std::min_element(times.begin(), times.end());
    double max_time = *std::max_element(times.begin(), times.end());
    double variance = 0.0;
    for (double t : times) {
        variance += (t - average) * (t - average);
    }
    variance /= iterations;
    double stddev = std::sqrt(variance);

    std::cout << "\nBenchmark Results:" << std::endl;
    std::cout << "Average time: " << average << " ms" << std::endl;
    std::cout << "Minimum time: " << min_time << " ms" << std::endl;
    std::cout << "Maximum time: " << max_time << " ms" << std::endl;
    std::cout << "Standard deviation: " << stddev << " ms" << std::endl;

    
    return 0;
}
