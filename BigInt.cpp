#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
using namespace std;

class BigInt {
    string number;    
    bool isNegative; 

   
    void removeLeadingZeros() {
        while (number.length() > 1 && number[0] == '0') {
        number.erase(0, 1);
    }

    if (number == "0") {
        isNegative = false;
    }
    
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

    // Multiply two non-negative magnitude strings (schoolbook long multiplication)
    static string mulMagnitudes(const string& a, const string& b) {
        if (a == "0" || b == "0") {
            return "0";
        }

        vector<int> result(a.length() + b.length(), 0);

        for (int i = (int)a.length() - 1; i >= 0; i--) {
            int digitA = a[i] - '0';
            for (int j = (int)b.length() - 1; j >= 0; j--) {
                int digitB = b[j] - '0';
                int sum = result[i + j + 1] + digitA * digitB;
                result[i + j + 1] = sum % 10;
                result[i + j] += sum / 10;
            }
        }

        string resultStr;
        for (int digit : result) {
            resultStr.push_back(char('0' + digit));
        }

        size_t firstNonZero = resultStr.find_first_not_of('0');
        if (firstNonZero == string::npos) {
            return "0";
        }
        return resultStr.substr(firstNonZero);
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
    BigInt result = *this;

    // Don't make zero negative
    if (result.number != "0") {
        result.isNegative = !result.isNegative;
    }

    return result;
}

// Unary plus operator (+x)
BigInt operator+() const {
    return *this;
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

        // If either number is zero
        if (number == "0" || other.number == "0") {

            number = "0";
            isNegative = false;

            return *this;
        }
        // Result size
        vector<int> result(
            number.length() + other.number.length(),
            0
        );

        // Long multiplication
        for (int i = number.length() - 1; i >= 0; i--) {

            for (int j = other.number.length() - 1; j >= 0; j--) {

                int digit1 = number[i] - '0';
                int digit2 = other.number[j] - '0';

                int product = digit1 * digit2;

                int position1 = i + j + 1;
                int position2 = i + j;

                int sum = product + result[position1];

                result[position1] = sum % 10;
                result[position2] += sum / 10;
            }
        }

        string resultString = "";

        int i = 0;

        // Skip leading zeros
        while (i < result.size() && result[i] == 0) {
            i++;
        }

        while (i < result.size()) {
            resultString += char(result[i] + '0');
            i++;
        }

        number = resultString;

        // XOR sign rule
        isNegative = isNegative != other.isNegative;

        removeLeadingZeros();

        return *this;
    }


    // Division assignment operator (x /= y)
    BigInt& operator/=(const BigInt& other) {

        // Division by zero
        if (other.number == "0") {
            throw runtime_error("Division by zero");
        }

        // If this is zero
        if (number == "0") {
            return *this;
        }

        // Save result sign
        bool resultNegative = isNegative != other.isNegative;

        // Work with positive numbers
        BigInt dividend(number);
        BigInt divisor(other.number);

        string quotient = "";

        BigInt current(0);

        // Long division digit by digit
        for (char digit : dividend.number) {

            // current = current * 10
            current *= BigInt(10);

            // current = current + current digit
            current += BigInt(digit - '0');

            int qDigit = 0;

            // Find how many times divisor fits
            for (int d = 9; d >= 1; d--) {

                BigInt candidate(0);

                // candidate = divisor * d
                for (int k = 0; k < d; k++) {
                    candidate += divisor;
                }

                if (candidate.compareMagnitude(current) <= 0) {

                    qDigit = d;
                    current -= candidate;

                    break;
                }
            }

            quotient += char(qDigit + '0');
        }

        number = quotient;
        isNegative = resultNegative;

        removeLeadingZeros();

        return *this;
    }


    // Modulus assignment operator (x %= y)
    BigInt& operator%=(const BigInt& other) {

        // Division by zero
        if (other.number == "0") {
            throw runtime_error("Division by zero");
        }

        // remainder = this - (this / other) * other

        BigInt quotient = *this;
        quotient /= other;

        BigInt product = quotient;
        product *= other;

        *this -= product;

        // Sign of remainder must match dividend
        // Our subtraction already produces the correct sign
        // when result is not zero.

        removeLeadingZeros();

        return *this;
    }


    // Pre-increment operator (++x)
    BigInt& operator++() {

        *this += BigInt(1);

        return *this;
    }


    // Post-increment operator (x++)
    BigInt operator++(int) {

        BigInt temp(*this);

        ++(*this);

        return temp;
    }


    // Pre-decrement operator (--x)
    BigInt& operator--() {

        *this -= BigInt(1);

        return *this;
    }


    // Post-decrement operator (x--)
    BigInt operator--(int) {

        BigInt temp(*this);

        --(*this);

        return temp;
    }


    // Convert BigInt to string representation
    string toString() const {

        if (isNegative && number != "0") {
            return "-" + number;
        }

        return number;
    }


    // Output stream operator
    friend ostream& operator<<(ostream& os, const BigInt& num) {

        if (num.isNegative && num.number != "0") {
            os << "-";
        }

        os << num.number;

        return os;
    }


    // Input stream operator
    friend istream& operator>>(istream& is, BigInt& num) {

        string input;

        is >> input;

        num = BigInt(input);

        return is;
    }


    // Friend declarations for comparison operators
    friend bool operator==(const BigInt& lhs, const BigInt& rhs);

    friend bool operator<(const BigInt& lhs, const BigInt& rhs);
};


// Binary addition operator
BigInt operator+(BigInt lhs, const BigInt& rhs) {

    lhs += rhs;

    return lhs;
}


// Binary subtraction operator
BigInt operator-(BigInt lhs, const BigInt& rhs) {

    lhs -= rhs;

    return lhs;
}


// Binary multiplication operator
BigInt operator*(BigInt lhs, const BigInt& rhs) {

    lhs *= rhs;

    return lhs;
}


// Binary division operator
BigInt operator/(BigInt lhs, const BigInt& rhs) {

    lhs /= rhs;

    return lhs;
}


// Binary modulus operator
BigInt operator%(BigInt lhs, const BigInt& rhs) {

    lhs %= rhs;

    return lhs;
}


// Equality comparison operator
bool operator==(const BigInt& lhs, const BigInt& rhs) {

    return lhs.isNegative == rhs.isNegative &&
           lhs.number == rhs.number;
}


// Inequality comparison operator
bool operator!=(const BigInt& lhs, const BigInt& rhs) {

    return !(lhs == rhs);
}


// Less-than comparison operator
bool operator<(const BigInt& lhs, const BigInt& rhs) {

    // Negative < Positive
    if (lhs.isNegative && !rhs.isNegative) {
        return true;
    }

    // Positive > Negative
    if (!lhs.isNegative && rhs.isNegative) {
        return false;
    }

    // Both positive
    if (!lhs.isNegative && !rhs.isNegative) {

        return lhs.compareMagnitude(rhs) < 0;
    }

    // Both negative
    // Bigger magnitude means smaller number
    return lhs.compareMagnitude(rhs) > 0;
}


// Less-than-or-equal comparison operator
bool operator<=(const BigInt& lhs, const BigInt& rhs) {

    return (lhs < rhs) || (lhs == rhs);
}


// Greater-than comparison operator
bool operator>(const BigInt& lhs, const BigInt& rhs) {

    return !(lhs <= rhs);
}


// Greater-than-or-equal comparison operator
bool operator>=(const BigInt& lhs, const BigInt& rhs) {

    return !(lhs < rhs);
}


int main() {

    cout << "=== BigInt Class Test Program ===" << endl << endl;

    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl << endl;



    // Test 1: Constructors and basic output

    cout << "1. Constructors and output:" << endl;

    BigInt a(12345);
    BigInt b("-67890");
    BigInt c("0");
    BigInt d = a;

    cout << "a (from int): " << a << endl;
    cout << "b (from string): " << b << endl;
    cout << "c (zero): " << c << endl;
    cout << "d (copy of a): " << d << endl << endl;


    // Test 2: Arithmetic operations

    cout << "2. Arithmetic operations:" << endl;

    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * b = " << a * b << endl;
    cout << "b / a = " << b / a << endl;
    cout << "a % 100 = " << a % BigInt(100) << endl << endl;


    // Test 3: Relational operators

    cout << "3. Relational operators:" << endl;

    cout << "a == d: " << (a == d) << endl;
    cout << "a != b: " << (a != b) << endl;
    cout << "a < b: " << (a < b) << endl;
    cout << "a > b: " << (a > b) << endl;
    cout << "c == 0: " << (c == BigInt(0)) << endl << endl;


    // Test 4: Unary operators and increments

    cout << "4. Unary operators and increments:" << endl;

    cout << "-a: " << -a << endl;
    cout << "++a: " << ++a << endl;
    cout << "a--: " << a-- << endl;
    cout << "a after decrement: " << a << endl << endl;


    // Test 5: Large number operations

    cout << "5. Large number operations:" << endl;

    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");

    cout << "Very large addition: "
         << num1 + num2 << endl;

    cout << "Very large multiplication: "
         << num1 * num2 << endl << endl;


    // Test 6: Edge cases and error handling

    cout << "6. Edge cases:" << endl;

    BigInt zero(0);
    BigInt one(1);

    try {

        BigInt result = one / zero;

        cout << "Division by zero succeeded (unexpected)"
             << endl;

    }
    catch (const runtime_error& e) {

        cout << "Division by zero correctly threw error: "
             << e.what() << endl;
    }

    cout << "Multiplication by zero: "
         << one * zero << endl;

    cout << "Negative multiplication: "
         << BigInt(-5) * BigInt(3) << endl;

    cout << "Negative division: "
         << BigInt(-10) / BigInt(3) << endl;

    cout << "Negative modulus: "
         << BigInt(-10) % BigInt(3) << endl;
    


    return 0;
}
