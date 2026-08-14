#include <iostream>
#include <string>
#include <cstdint>
using namespace std;

class BigInt {
    string number;    // Stores the number as a string
    bool isNegative;  // True if number is negative

    // Remove unnecessary leading zeros from the number string
    void removeLeadingZeros() {
        while (number.length() > 1 && number[0] == '0') {
        number.erase(0, 1);
    }

    if (number == "0") {
        isNegative = false;
    }
        // TODO: Implement this function
    }

    // Compare absolute values of two BigInts (ignore signs)
    // Returns: 1 if |this| > |other|, 0 if equal, -1 if |this| < |other|
    int compareMagnitude(const BigInt& other) const {
        if (number.length() > other.number.length()) {
        return 1;
    }

    if (number.length() < other.number.length()) {
        return -1;
    }

    if (number > other.number) {
        return 1;
    }

    if (number < other.number) {
        return -1;
    }

        // TODO: Implement this function
        return 0;
    }

    static string addMagnitudes(const string& a, const string& b) {
        string result;
        int i = (int)a.length() - 1;
        int j = (int)b.length() - 1;
        int carry = 0;

        while (i >= 0 || j >= 0 || carry) {
            int digitA = (i >= 0) ? (a[i] - '0') : 0;
            int digitB = (j >= 0) ? (b[j] - '0') : 0;
            int sum = digitA + digitB + carry;
            carry = sum / 10;
            result.push_back(char('0' + (sum % 10)));
            i--;
            j--;
        }

        // result was built least-significant-digit first, so reverse it
        for (size_t k = 0; k < result.length() / 2; k++) {
            swap(result[k], result[result.length() - 1 - k]);
        }
        return result;
    }

    static string subMagnitudes(const string& a, const string& b) {
        string result;
        int i = (int)a.length() - 1;
        int j = (int)b.length() - 1;
        int borrow = 0;

        while (i >= 0) {
            int digitA = (a[i] - '0') - borrow;
            int digitB = (j >= 0) ? (b[j] - '0') : 0;

            if (digitA < digitB) {
                digitA += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }

            result.push_back(char('0' + (digitA - digitB)));
            i--;
            j--;
        }

        // result was built least-significant-digit first, so reverse it
        for (size_t k = 0; k < result.length() / 2; k++) {
            swap(result[k], result[result.length() - 1 - k]);
        }

        // remove any leading zeros produced by the subtraction
        size_t firstNonZero = result.find_first_not_of('0');
        if (firstNonZero == string::npos) {
            return "0";
        }
        return result.substr(firstNonZero);
    }

public:
    // Default constructor - initialize to zero
    BigInt() {
        number = "0";
        isNegative = false;
    }

    // Constructor from 64-bit integer
    BigInt(int64_t value) {
        if (value < 0) {
            isNegative = true;
            number = std::to_string(-value);
        } else {
            isNegative = false;
            number = std::to_string(value);
        }
    }

    // Constructor from string representation
    BigInt(const string& str) {
        if (str[0] == '-') {
            isNegative = true;
            number = str.substr(1);
        } else {
            isNegative = false;
            number = str;
        }

        removeLeadingZeros();
    }

    // Copy constructor
    BigInt(const BigInt& other) {
        number = other.number;
        isNegative = other.isNegative;
    }

    // Destructor
    ~BigInt() {
       
    }

    // Assignment operator
    BigInt& operator=(const BigInt& other) {
        if (this == &other) {
            return *this;
        }

        number = other.number;
        isNegative = other.isNegative;

        return *this;
    }

    // Unary negation operator (-x)
    BigInt operator-() const {
        BigInt result;
        // TODO: Implement negation logic
        return result;
    }

    // Unary plus operator (+x)
    BigInt operator+() const {
        BigInt result;
        // TODO: Implement this operator
        return result;
    }

    // Addition assignment operator (x += y)
    BigInt& operator+=(const BigInt& other) {
        if (isNegative == other.isNegative) {
            // Same sign: add magnitudes, keep the sign
            number = addMagnitudes(number, other.number);
        } else {
            // Different signs: subtract smaller magnitude from larger
            int cmp = compareMagnitude(other);
            if (cmp >= 0) {
                // |this| >= |other|: result keeps this's sign
                number = subMagnitudes(number, other.number);
            } else {
                // |other| > |this|: result takes other's sign
                number = subMagnitudes(other.number, number);
                isNegative = other.isNegative;
            }
        }

        removeLeadingZeros();
        return *this;
    }

    // Subtraction assignment operator (x -= y)
    BigInt& operator-=(const BigInt& other) {
        bool otherEffectiveSign = !other.isNegative;

        if (isNegative == otherEffectiveSign) {
            // Effective signs match: add magnitudes, keep the sign
            number = addMagnitudes(number, other.number);
        } else {
            // Effective signs differ: subtract smaller magnitude from larger
            int compare = compareMagnitude(other);
            if (compare >= 0) {
                // |this| >= |other|: result keeps this's sign
                number = subMagnitudes(number, other.number);
            } else {
                // |other| > |this|: result takes other's effective sign
                number = subMagnitudes(other.number, number);
                isNegative = otherEffectiveSign;
            }
        }

        removeLeadingZeros();
        return *this;
    }

    // Multiplication assignment operator (x *= y)
    BigInt& operator*=(const BigInt& other) {
        // TODO: Implement this operator
        return *this;
    }

    // Division assignment operator (x /= y)
    BigInt& operator/=(const BigInt& other) {
        // TODO: Implement this operator
        return *this;
    }

    // Modulus assignment operator (x %= y)
    BigInt& operator%=(const BigInt& other) {
        // TODO: Implement this operator
        return *this;
    }

    // Pre-increment operator (++x)
    BigInt& operator++() {
        // TODO: Implement this operator
        return *this;
    }

    // Post-increment operator (x++)
    BigInt operator++(int) {
        BigInt temp;
        // TODO: Implement this operator
        return temp;
    }

    // Pre-decrement operator (--x)
    BigInt& operator--() {
        // TODO: Implement this operator
        return *this;
    }

    // Post-decrement operator (x--)
    BigInt operator--(int) {
        BigInt temp;
        // TODO: Implement this operator
        return temp;
    }

    // Convert BigInt to string representation
    string toString() const {
        // TODO: Implement this function
        return "";
    }

    // Output stream operator (for printing)
    friend ostream& operator<<(ostream& os, const BigInt& num) {
        // TODO: Implement this operator
        return os;
    }

    // Input stream operator (for reading from input)
    friend istream& operator>>(istream& is, BigInt& num) {
        // TODO: Implement this operator
        return is;
    }

    // Friend declarations for comparison operators
    friend bool operator==(const BigInt& lhs, const BigInt& rhs);
    friend bool operator<(const BigInt& lhs, const BigInt& rhs);
};

// Binary addition operator (x + y)
BigInt operator+(BigInt lhs, const BigInt& rhs) {
    //BigInt result;
    lhs += rhs;
    //return result;
    return lhs;
}

// Binary subtraction operator (x - y)
BigInt operator-(BigInt lhs, const BigInt& rhs) {
    //BigInt result;
    lhs -= rhs;
    //return result;
    return lhs;
}

// Binary multiplication operator (x * y)
BigInt operator*(BigInt lhs, const BigInt& rhs) {
    BigInt result;
    // TODO: Implement this operator
    return result;
}

// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt& rhs) {
    BigInt result;
    // TODO: Implement this operator
    return result;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt& rhs) {
    BigInt result;
    // TODO: Implement this operator
    return result;
}

// Equality comparison operator (x == y)
bool operator==(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

// Inequality comparison operator (x != y)
bool operator!=(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

// Less-than comparison operator (x < y)
bool operator<(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

// Less-than-or-equal comparison operator (x <= y)
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

// Greater-than comparison operator (x > y)
bool operator>(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

// Greater-than-or-equal comparison operator (x >= y)
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return false;
}

int main() {
    cout << "=== BigInt Class Test Program ===" << endl << endl;
    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl << endl;

    /*
    // Test 1: Constructors and basic output
    cout << "1. Constructors and output:" << endl;
    BigInt a(12345);              // Should create BigInt from integer
    BigInt b("-67890");           // Should create BigInt from string
    BigInt c("0");                // Should handle zero correctly
    BigInt d = a;                 // Should use copy constructor
    cout << "a (from int): " << a << endl;        // Should print "12345"
    cout << "b (from string): " << b << endl;     // Should print "-67890"
    cout << "c (zero): " << c << endl;            // Should print "0"
    cout << "d (copy of a): " << d << endl << endl; // Should print "12345"

    // Test 2: Arithmetic operations
    cout << "2. Arithmetic operations:" << endl;
    cout << "a + b = " << a + b << endl;          // Should calculate 12345 + (-67890)
    cout << "a - b = " << a - b << endl;          // Should calculate 12345 - (-67890)
    cout << "a * b = " << a * b << endl;          // Should calculate 12345 * (-67890)
    cout << "b / a = " << b / a << endl;          // Should calculate (-67890) / 12345
    cout << "a % 100 = " << a % BigInt(100) << endl << endl; // Should calculate 12345 % 100

    // Test 3: Relational operators
    cout << "3. Relational operators:" << endl;
    cout << "a == d: " << (a == d) << endl;       // Should be true (12345 == 12345)
    cout << "a != b: " << (a != b) << endl;       // Should be true (12345 != -67890)
    cout << "a < b: " << (a < b) << endl;         // Should be false (12345 < -67890)
    cout << "a > b: " << (a > b) << endl;         // Should be true (12345 > -67890)
    cout << "c == 0: " << (c == BigInt(0)) << endl << endl; // Should be true (0 == 0)

    // Test 4: Unary operators and increments
    cout << "4. Unary operators and increments:" << endl;
    cout << "-a: " << -a << endl;                 // Should print "-12345"
    cout << "++a: " << ++a << endl;               // Should increment and print "12346"
    cout << "a--: " << a-- << endl;               // Should print "12346" then decrement
    cout << "a after decrement: " << a << endl << endl; // Should print "12345"

    // Test 5: Large number operations
    cout << "5. Large number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Very large addition: " << num1 + num2 << endl;
    cout << "Very large multiplication: " << num1 * num2 << endl << endl;

    // Test 6: Edge cases and error handling
    cout << "6. Edge cases:" << endl;
    BigInt zero(0);
    BigInt one(1);
    try {
        BigInt result = one / zero;               // Should throw division by zero error
        cout << "Division by zero succeeded (unexpected)" << endl;
    } catch (const runtime_error& e) {
        cout << "Division by zero correctly threw error: " << e.what() << endl;
    }
    cout << "Multiplication by zero: " << one * zero << endl;        // Should be "0"
    cout << "Negative multiplication: " << BigInt(-5) * BigInt(3) << endl;  // Should be "-15"
    cout << "Negative division: " << BigInt(-10) / BigInt(3) << endl;       // Should be "-3"
    cout << "Negative modulus: " << BigInt(-10) % BigInt(3) << endl;        // Should be "-1"
    */

    return 0;
}
