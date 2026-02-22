#ifndef VEDIC_AGI_H
#define VEDIC_AGI_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

namespace SatvikAGI {

// Define a scaling factor for fixed-point arithmetic
const int FIXED_POINT_SCALE = 10000; // 4 decimal places of precision

/**
 * @brief Represents a fixed-point number using long long for precision.
 * The value is stored as an integer multiplied by FIXED_POINT_SCALE.
 */
struct FixedPoint {
    long long value;

    // Default constructor
    FixedPoint() : value(0) {}

    // Constructor from long long (integer part)
    explicit FixedPoint(long long int_val) : value(int_val * FIXED_POINT_SCALE) {}

    // Constructor from double (floating point conversion)
    explicit FixedPoint(double double_val) : value(static_cast<long long>(double_val * FIXED_POINT_SCALE)) {}

    // Addition operator
    FixedPoint operator+(const FixedPoint& other) const {
        FixedPoint result;
        result.value = value + other.value;
        return result;
    }

    // Subtraction operator
    FixedPoint operator-(const FixedPoint& other) const {
        FixedPoint result;
        result.value = value - other.value;
        return result;
    }

    // Multiplication operator
    // This is the core fixed-point multiplication, handling scaling
    FixedPoint operator*(const FixedPoint& other) const {
        FixedPoint result;
        // Multiply values and then divide by scale to correct precision
        // Care must be taken to avoid overflow if intermediate product exceeds long long max
        result.value = (value * other.value) / FIXED_POINT_SCALE;
        return result;
    }

    // Division operator
    // This correctly handles fixed-point division by scaling up before dividing
    FixedPoint operator/(const FixedPoint& other) const {
        if (other.value == 0) {
            // Handle division by zero, e.g., throw an exception
            std::cerr << "Error: Division by zero in FixedPoint division." << std::endl;
            return FixedPoint(0LL); // Return zero or throw error
        }
        FixedPoint result;
        // Scale the numerator up before division to maintain precision
        result.value = (value * FIXED_POINT_SCALE) / other.value;
        return result;
    }

    // Convert to double
    double to_double() const {
        return static_cast<double>(value) / FIXED_POINT_SCALE;
    }

    // Comparison operators
    bool operator==(const FixedPoint& other) const { return value == other.value; }
    bool operator!=(const FixedPoint& other) const { return value != other.value; }
    bool operator<(const FixedPoint& other) const { return value < other.value; }
    bool operator>(const FixedPoint& other) const { return value > other.value; }
    bool operator<=(const FixedPoint& other) const { return value <= other.value; }
    bool operator>=(const FixedPoint& other) const { return value >= other.value; }
};

// Output stream operator for easy printing
std::ostream& operator<<(std::ostream& os, const FixedPoint& fp) {
    os << fp.to_double();
    return os;
}

/**
 * @brief Implements various Vedic Mathematics algorithms for high-performance computing.
 */
class VedicMath {
public:
    /**
     * @brief Implements Urdhva-Tiryak multiplication (Vertically and Crosswise).
     *
     * This method performs multiplication using a digit-by-digit approach, emulating the Vedic
     * 'Urdhva Tiryagbhyam' sutra. It is particularly efficient for hardware implementations
     * (e.g., ARM/RISC-V architectures) as it can be parallelized, avoiding traditional serial
     * multiplication loops and potentially reducing computational latency and power consumption.
     *
     * Conceptual Extension to Matrix Multiplication for ARM/RISC-V:
     * In a hardware-accelerated context, each digit-by-digit product (num1_digits[i] * num2_digits[j])
     * can be computed in parallel across multiple processing units. For matrix multiplication,
     * this extends by applying Urdhva-Tiryak on the elements of each row-column pair. For instance,
     * to compute element C[r][c] = sum(A[r][k] * B[k][c]), the individual multiplications A[r][k] * B[k][c]
     * can be performed using Urdhva-Tiryak, potentially with specialized hardware circuits for digit products.
     * The partial products would then be accumulated with carry propagation handled across these parallel units.
     * This fine-grained parallelism at the digit level, combined with element-wise parallelism for matrices,
     * offers significant efficiency gains on highly parallel architectures like ARM/RISC-V based accelerators,
     * especially when dealing with fixed-point representations for AI weights and activations.
     *
     * @param a The first number (long long).
     * @param b The second number (long long).
     * @return The product of a and b using the Urdhva-Tiryak method.
     */
    static long long urdhva_tiryak_multiply(long long a, long long b) {
        // Convert numbers to string representations to access individual digits.
        std::string s_a = std::to_string(a);
        std::string s_b = std::to_string(b);

        // Convert string digits to integer vectors for easier arithmetic operations.
        std::vector<int> num1_digits, num2_digits;
        for (char c : s_a) num1_digits.push_back(c - '0');
        for (char c : s_b) num2_digits.push_back(c - '0');

        int len1 = num1_digits.size();
        int len2 = num2_digits.size();

        // Initialize a result array. The maximum length of the product can be sum of lengths of the numbers.
        // For example, multiplying two 2-digit numbers can result in a 3 or 4-digit number.
        std::vector<int> result_array(len1 + len2, 0);

        // Perform multiplication of each digit of num1 by each digit of num2.
        // The product of digits at index i and j contributes to the position i + j + 1 in the result array.
        // This accounts for place values during multiplication.
        for (int i = 0; i < len1; ++i) {
            for (int j = 0; j < len2; ++j) {
                result_array[i + j + 1] += num1_digits[i] * num2_digits[j];
            }
        }

        // Process carries from right to left (least significant digit to most significant).
        // This simulates the 'carrying over' in manual multiplication.
        int carry = 0;
        std::string final_product_str = "";
        // Iterate from the rightmost part of the result_array (which stores products without carries).
        for (int i = result_array.size() - 1; i >= 0; --i) {
            int current_sum = result_array[i] + carry;
            final_product_str += std::to_string(current_sum % 10); // Append the last digit of the sum
            carry = current_sum / 10; // Calculate the carry for the next position
        }

        // If there's a final carry after processing all digits, append it to the string.
        if (carry > 0) {
            final_product_str += std::to_string(carry);
        }

        // The digits were appended in reverse order (from least significant to most significant),
        // so reverse the string to get the correct product.
        std::reverse(final_product_str.begin(), final_product_str.end());

        // Remove any leading zeros. For example, 00123 should become 123. If the result is 0, return 0.
        size_t first_digit = final_product_str.find_first_not_of('0');
        if (std::string::npos == first_digit) {
            return 0; // The result is 0 (e.g., 0 * 5)
        } else {
            return std::stoll(final_product_str.substr(first_digit)); // Convert the significant part to long long
        }
    }

    static long long nikhilam_multiplier(long long n1, long long n2) {
        long long base = 1;
        while (base <= n1 || base <= n2) {
            base *= 10;
        }
        long long d1 = base - n1;
        long long d2 = base - n2;
        return ((n1 - d2) * base) + (d1 * d2);
    }
};

} // namespace SatvikAGI

#endif // VEDIC_AGI_H
