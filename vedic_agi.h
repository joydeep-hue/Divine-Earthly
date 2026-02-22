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

} // namespace SatvikAGI

#endif // VEDIC_AGI_H
