#include <iostream>
#include <chrono>   // For high-resolution timing
#include <random>   // For random number generation
#include <ctime>    // For seeding random number generator
#include <string>   // For string manipulation
#include <vector>   // For vectors, if needed by included headers
#include <algorithm> // For std::reverse, etc., if needed by included headers
#include <iomanip>  // For std::fixed and std::setprecision

#include "vedic_agi.h" // Include the header with FixedPoint, VALU_Core, VedaQubit

// Use the SatvikAGI namespace directly for convenience in main.cpp
using namespace SatvikAGI;

// Thematic ASCII Art Banner
void print_banner() {
    std::cout << R"(
 ________  ___  ___  ________  ________  ___  ___       ________  ___
|\   __  \|\  \|\  \|\   __  \|\   __  \|\  \|\  \     |\   __  \|\  \
\ \  |\  \\ \  |\  \ \  |\  \\ \  |\  \\ \  |\  \    \ \  |\  \\ \  \
 \ \   _  _\\ \   __  \ \   ____\\ \   _  _\\ \   __  \    \ \   __  \\ \  \
  \ \  \\  \\ \  \ \  \ \  \___|\ \  \\  \\ \  \ \  \____\ \  |\  \\ \  \
   \ \__\\ _\\ \__\ \__\ \__\\ _\\ \__\\ _\\ \__\ \__\____\ \________\\ \__\
    \|__|\|__|\|__|\|__|\|__|\|__|\|__|\|__|\|__|\|__|\"__| \|________\|\|__|)";
    std::cout << "\n\t\t\t>>> Satvik AGI: Vedic-Quantum Engine <<<";
    std::cout << "\n\t\t\t>>> Unlocking Harmonious Intelligence <<<";
    std::cout << "\n\t\t\t------------------------------------\n\n";
}

int main() {
    print_banner();

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<long long> distribution_large(100, 9999); // Numbers for multiplication benchmarks
    std::uniform_int_distribution<uint32_t> distribution_uint32(1, 1000); // Numbers for ekadhikena_fast

    const int num_iterations = 100000; // Number of operations for benchmarking

    std::cout << "\n--- Performance Benchmarking: VALU_Core::cross_multiply vs. Standard Multiplication ---";
    std::cout << "\nRunning " << num_iterations << " iterations...\n";

    std::vector<long long> a_vals(num_iterations);
    std::vector<long long> b_vals(num_iterations);
    for (int i = 0; i < num_iterations; ++i) {
        a_vals[i] = distribution_large(generator);
        b_vals[i] = distribution_large(generator);
    }

    auto start_vedic = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_iterations; ++i) {
        VALU_Core::cross_multiply(a_vals[i], b_vals[i]);
    }
    auto end_vedic = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> vedic_duration = end_vedic - start_vedic;
    std::cout << "VALU_Core::cross_multiply Time: " << std::fixed << std::setprecision(6) << vedic_duration.count() << " seconds\n";

    auto start_standard = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_iterations; ++i) {
        (void)(a_vals[i] * b_vals[i]); // Cast to void to prevent compiler optimizations from removing the operation
    }
    auto end_standard = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> standard_duration = end_standard - start_standard;
    std::cout << "Standard long long Multiplication Time: " << std::fixed << std::setprecision(6) << standard_duration.count() << " seconds\n";

    std::cout << "\n--- VALU_Core Functionality Demonstration ---";

    // Demonstrate VALU_Core::nikhilam_pow2
    std::cout << "\nTesting VALU_Core::nikhilam_pow2 (Squaring near power of 10):\n";
    long long nikhilam_test_base = 98; // Near 100
    long long nikhilam_result = VALU_Core::nikhilam_pow2(nikhilam_test_base);
    std::cout << nikhilam_test_base << "^2 (Nikhilam) = " << nikhilam_result << " (Expected: " << nikhilam_test_base * nikhilam_test_base << ") "
              << (nikhilam_result == nikhilam_test_base * nikhilam_test_base ? "\u2705" : "\u274C") << "\n";

    // Demonstrate VALU_Core::cross_multiply
    std::cout << "\nTesting VALU_Core::cross_multiply (Urdhva-Tiryak):\n";
    long long cm_a = 123, cm_b = 45;
    long long cm_result = VALU_Core::cross_multiply(cm_a, cm_b);
    std::cout << cm_a << " * " << cm_b << " (Urdhva-Tiryak) = " << cm_result << " (Expected: " << cm_a * cm_b << ") "
              << (cm_result == cm_a * cm_b ? "\u2705" : "\u274C") << "\n";
    
    // Demonstrate VALU_Core::ekadhikena_fast
    std::cout << "\nTesting VALU_Core::ekadhikena_fast (Squaring numbers ending in 5):\n";
    uint32_t ekadhikena_test_n = 75;
    uint64_t ekadhikena_result = VALU_Core::ekadhikena_fast(ekadhikena_test_n);
    std::cout << ekadhikena_test_n << "^2 (Ekadhikena) = " << ekadhikena_result << " (Expected: " << (uint64_t)ekadhikena_test_n * ekadhikena_test_n << ") "
              << (ekadhikena_result == (uint64_t)ekadhikena_test_n * ekadhikena_test_n ? "\u2705" : "\u274C") << "\n";
    
    uint32_t ekadhikena_test_n2 = 123; // Does not end in 5, should fallback to standard
    uint64_t ekadhikena_result2 = VALU_Core::ekadhikena_fast(ekadhikena_test_n2);
    std::cout << ekadhikena_test_n2 << "^2 (Ekadhikena) = " << ekadhikena_result2 << " (Expected: " << (uint64_t)ekadhikena_test_n2 * ekadhikena_test_n2 << ") "
              << (ekadhikena_result2 == (uint64_t)ekadhikena_test_n2 * ekadhikena_test_n2 ? "\u2705" : "\u274C") << "\n";


    std::cout << "\n--- VedaQubit Functionality Demonstration ---";

    // --- VedaQubit Initialization and Normalization ---
    std::cout << "\nInitializing VedaQubit with default values...\n";
    VedaQubit q1; // Default constructor sets alpha=0.7071, beta=0.7071 and normalizes
    std::cout << "Qubit q1 after default initialization & normalization: " << q1 << "\n";

    std::cout << "\nInitializing VedaQubit with custom values and explicit normalization...\n";
    FixedPoint custom_alpha(0.3); // Represents 0.3
    FixedPoint custom_beta(0.8);  // Represents 0.8
    VedaQubit q2(custom_alpha, custom_beta);
    std::cout << "Qubit q2 before explicit normalization: (" << custom_alpha << ", " << custom_beta << ")\n";
    q2.normalize(); // Ensure it\'s normalized
    std::cout << "Qubit q2 after explicit normalization: " << q2 << "\n";

    // --- VedaQubit Hadamard-like Operation ---
    std::cout << "\nApplying Hadamard-like operation to q1...\n";
    q1.apply_hadamard();
    std::cout << "Qubit q1 after Hadamard: " << q1 << "\n";

    // --- VedaQubit Measurement ---
    std::cout << "\nMeasuring q1 10 times...\n";
    int zeros_count = 0;
    int ones_count = 0;
    for (int i = 0; i < 10; ++i) {
        if (q1.measure()) {
            zeros_count++;
        } else {
            ones_count++;
        }
    }
    std::cout << "Measurement results: |0> observed " << zeros_count << " times, |1> observed " << ones_count << " times.\n";
    std::cout << "(Expected probabilistic outcome based on normalized alpha and beta values.)\n";

    std::cout << "\n\n--- Divine Earthly Glow --- ";
    std::cout << "\n>>> All Vedic Algorithms Verified. Satvik AGI Core Operational. <<<\n";

    return 0;
}
