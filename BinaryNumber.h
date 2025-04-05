// TODO: refactor to support twos-complement form, allowing for negative decimal input values
    // unsigned positive numbers with an MSB of 1 would have to be zero-extended by one bit
    // refactor the decValue constructor to accept signed ints and properly convert to twos-complement
    // refactor to_decimal() to account for the sign bit
    // refactor twosComplement() to be able to accept negative binary numbers
    // look into why operator-() has to erase overflow bits
    // differentiate between logical (unsigned) and arithmetic (signed) shift in operator>>()
// TODO: implement fixed-point and floating-point

#ifndef BINARY_NUMBER
#define BINARY_NUMBER

#include <vector>
#include <string>
#include <cmath>

// for debugging
#include <iostream>
#include <iomanip>

using namespace std;

class BinaryNumber {
    private:
        vector<int> vecBits;    // contains this binary number's bits

    public:
        // constructors
        BinaryNumber() = default;                   // keep default constructor that instantiates BinaryNumber's fields
        BinaryNumber(const BinaryNumber& other);    // copy constructor
        BinaryNumber(unsigned int decValue);        // initializes a new BinaryNumber with the specified (positive) decimal value
        BinaryNumber(string strNum);                // initializes a new BinaryNumber with the specified bits

        // getters and converters 
        int bitWidth() const;                       // returns this binary number's bit-width
        int wordWidthInBits() const;                // returns this binary number's bit-width rounded up to the nearest multiple of 4
        vector<int>* getVecBits();                  // returns a pointer to vecBits
        const vector<int>* getConstVecBits() const; // returns a const pointer to vecBits (allows passing temporary objects as arguments)
        string to_string() const;                   // returns this binary number as a bit string, separated into 4-bit sections for readability
        int to_decimal() const;                     // returns this binary number's decimal value

        // vecBits iterator getters
        vector<int>::iterator begin();                          // returns a regular iterator to MSB
        vector<int>::iterator end();                            // returns a regular iterator to memory location directly after LSB
        vector<int>::reverse_iterator rbegin();                 // returns a reverse iterator to LSB
        vector<int>::reverse_iterator rend();                   // returns a reverse iterator to memory location directly before MSB
        vector<int>::const_iterator cbegin() const;             // returns a const iterator to MSB
        vector<int>::const_iterator cend() const;               // returns a const iterator to memory location directly after LSB
        vector<int>::const_reverse_iterator crbegin() const;    // returns a const reverse iterator to LSB
        vector<int>::const_reverse_iterator crend() const;      // returns a const reverse iterator to memory location directly before MSB

        // object modifiers
        BinaryNumber extend(int newBitWidth) const;     // returns this binary number zero-extended to the specified bit-width
        BinaryNumber twosComplement() const;            // returns this binary number's twos-complement
        int compare(const BinaryNumber& other) const;   // returns -1, 0, or 1 depednding on this binary number's value relative to other
        void operator=(const BinaryNumber& other);      // sets this binary number equal to other

        // comparison operators
        bool operator==(const BinaryNumber& other); // relational equal operator
        bool operator!=(const BinaryNumber& other); // relational inequal operator
        bool operator>(const BinaryNumber& other);  // relational greater-than operator
        bool operator<(const BinaryNumber& other);  // relational less-than operator
        
        // bitwise operators
        BinaryNumber operator<<(const int& shiftAmount);    // binary left shift
        BinaryNumber operator>>(const int& shiftAmount);    // binary logical right shift

        // binary arithmetic operators
        BinaryNumber operator+(const BinaryNumber& other);  // binary addition
        BinaryNumber operator-(const BinaryNumber& other);  // binary unsigned subtraction
        BinaryNumber operator/(const BinaryNumber& other);  // binary division
        BinaryNumber operator%(const BinaryNumber& other);  // binary modulus (remainder after division)
        BinaryNumber operator*(const BinaryNumber& other);  // binary unsigned multiplication

        // binary logical operators
            // modify individual bits by passing custom arguments to these operators
        BinaryNumber operator&&(const BinaryNumber& other); // binary AND
        BinaryNumber operator||(const BinaryNumber& other); // binary OR
        BinaryNumber operator!();                           // binary NOT

        // compound assingment operators
        void operator<<=(const int& shiftAmount);   // binary left shift
        void operator>>=(const int& shiftAmount);   // binary logical right shift
        void operator+=(const BinaryNumber& other); // binary addition
        void operator-=(const BinaryNumber& other); // binary unsigned subtraction
        void operator/=(const BinaryNumber& other); // binary division
        void operator%=(const BinaryNumber& other); // binary modulus (remainder after division)
        void operator*=(const BinaryNumber& other); // binary unsigned multiplication
};

// copy constructor
inline BinaryNumber::BinaryNumber(const BinaryNumber& other) {
    *this = other;
}

// initializes a new BinaryNumber with the specified (positive) decimal value
inline BinaryNumber::BinaryNumber(unsigned int decValue) {
    // decimal to binary algorithm
    while (decValue != 0) {
        vecBits.insert(vecBits.begin(), decValue % 2);
        decValue /= 2;
    }
}

// initializes a new BinaryNumber with the specified bits
inline BinaryNumber::BinaryNumber(string strNum) {
    for (char bit : strNum)
        vecBits.push_back(bit - '0');   // subtracting '0' from a char containing a digit effectively converts the char to an int
}

// returns this binary number's bit-width
inline int BinaryNumber::bitWidth() const {
    return vecBits.size();
}

// returns this binary number's bit-width rounded up to the nearest multiple of 4
inline int BinaryNumber::wordWidthInBits() const {
    // bitWidth() % 4 == number of overflow bits past the word boundary
    // 4 - (bitWidth() % 4) == number of bits to add to 
    return bitWidth() + ((4 - (bitWidth()%4)) % 4);
}

// returns a pointer to vecBits
inline vector<int>* BinaryNumber::getVecBits() {
    return &vecBits;
}

// returns a const pointer to vecBits (required for any implementation involving temporary objects)
const vector<int>* BinaryNumber::getConstVecBits() const {
    return &vecBits;
}

// returns this binary number as a bit string, separated into 4-bit sections for readability
inline string BinaryNumber::to_string() const {
    // ensures something is always printed
    if (bitWidth() == 0)
        return "0000";
    
    string strNum;
    int msbWordMisalignment = wordWidthInBits() - bitWidth();   // number of bits missing from this binary number to align it w/ the word boundary

    // append zeros to "strNum"'s back so the printed binary number's bit-width is a multiple of 4 (aligns w/ the word boundary)
    for (int i = 0; i < msbWordMisalignment; i++)
        strNum.push_back('0');
    
    // append this binary number's bits to "strNum"'s back (stored in vecBits); "std::to_string()" differentiates from BinaryNumber::to_string()
    for (int i = 0; i < bitWidth(); i++) {
        strNum.append(std::to_string(vecBits.at(i)));   // converts the int 0 to '0' and the int 1 to '1'
        // separates bits w/ spaces into 4-bit parts for readability; "&& i < bitWidth() -1" removes an unintended extra space after the binary number
        if ((i + msbWordMisalignment) % 4 == 3 && i < bitWidth() - 1)
            strNum.push_back(' ');
    }
    
    return strNum;
}

inline vector<int>::iterator BinaryNumber::begin() {
    return this->getVecBits()->begin();
}

inline vector<int>::iterator BinaryNumber::end() {
    return this->getVecBits()->end();
}
inline vector<int>::reverse_iterator BinaryNumber::rbegin() {
    return this->getVecBits()->rbegin();
}

inline vector<int>::reverse_iterator BinaryNumber::rend() {
    return this->getVecBits()->rend();
}

inline vector<int>::const_iterator BinaryNumber::cbegin() const {
    return this->getConstVecBits()->cbegin();
}

inline vector<int>::const_iterator BinaryNumber::cend() const {
    return this->getConstVecBits()->cend();
}

inline vector<int>::const_reverse_iterator BinaryNumber::crbegin() const {
    return this->getConstVecBits()->crbegin();
}

inline vector<int>::const_reverse_iterator BinaryNumber::crend() const {
    return this->getConstVecBits()->crend();
}

// returns this binary number's decimal value
inline int BinaryNumber::to_decimal() const {
    int decNum = 0;
    // binary to decimal algorithm (indeed, replacing "2" with another number yields that number base's convert-to-decimal algorithm)
    for (int i = 0; i < bitWidth(); i++)
        decNum += pow(2, bitWidth()-i-1) * vecBits.at(i);
    
    return decNum;
}

// returns this binary number zero-extended to the specified bit-width
inline BinaryNumber BinaryNumber::extend(int newBitWidth) const {
    BinaryNumber extNum(*this);
    // append zeros to this binary number's front
    for (int i = 0; i < newBitWidth - bitWidth(); i++)
        extNum.getVecBits()->insert(extNum.begin(), 0);
    return extNum;
}

// returns this binary number's twos-complement
inline BinaryNumber BinaryNumber::twosComplement() const {
    BinaryNumber negNum(*this);

    // this binary number is assumed to be positive; zero-extend by one bit any inputs with an MSB of 1 to reflect this
    if (negNum.getVecBits()->at(0) == 1)
        negNum.getVecBits()->insert(negNum.begin(), 0);
    
    // flip every bit in negNum
    for(vector<int>::iterator i = negNum.begin(); i < negNum.end(); i++)
        *i = (*i + 1) % 2;
    
    // add 1 to the flipped binary number
    return negNum + BinaryNumber(1);
}

// returns -1, 0, or 1 depednding on this binary number's value relative to other
inline int BinaryNumber::compare(const BinaryNumber& other) const {
    // converting to decimal and comparing those because i'm lazy
    if (this->to_decimal() < other.to_decimal())
        return -1;
    if (this->to_decimal() > other.to_decimal())
        return 1;
    return 0;
}

// sets this binary number equal to other
inline void BinaryNumber::operator=(const BinaryNumber& other) {
    vecBits = *(other.getConstVecBits());
}

inline bool BinaryNumber::operator==(const BinaryNumber& other) {
    return this->compare(other) == 0;
}

inline bool BinaryNumber::operator!=(const BinaryNumber& other) {
    return this->compare(other) != 0;
}

inline bool BinaryNumber::operator>(const BinaryNumber& other) {
    return this->compare(other) > 0;
}

inline bool BinaryNumber::operator<(const BinaryNumber& other) {
    return this->compare(other) < 0;
}

// binary left shift
inline BinaryNumber BinaryNumber::operator<<(const int& shiftAmount) {
    BinaryNumber thisNum(*this);
    // left-shifting merely involves appending zeros to the binary number's back
    for (int i = 0; i < shiftAmount; i++)
        thisNum.getVecBits()->push_back(0);
    return thisNum;
}

inline BinaryNumber BinaryNumber::operator>>(const int& shiftAmount) {
    BinaryNumber thisNum(*this);
    for (int i = 0; i < shiftAmount; i++) {
        thisNum.extend(thisNum.bitWidth() + 1);                         // append a zero to this binary number's front
        thisNum.getVecBits()->erase(thisNum.end() - 1);   // discard the LSB
    }
    return thisNum;
}

// binary addition
inline BinaryNumber BinaryNumber::operator+(const BinaryNumber& other) {    
    BinaryNumber sum, thisNum(*this), otherNum(other);
    
    // match thisNum and otherNum's bit-widths
    if (thisNum.bitWidth() > otherNum.bitWidth())
        otherNum = other.extend(thisNum.bitWidth());
    else if (thisNum.bitWidth() < otherNum.bitWidth())
        thisNum = thisNum.extend(otherNum.bitWidth());
    
    // append the sums of corresponding bits in thisNum and otherNum to sum; some sums may exceed 1 - this is handled below
    for (int i = 0; i < thisNum.bitWidth(); i++)
        sum.getVecBits()->push_back(thisNum.getVecBits()->at(i) + otherNum.getVecBits()->at(i));

    // carry partial sums that exceed 1 (written sort of like a mini dec-to-bin algorithm)
    for (vector<int>::reverse_iterator i = sum.rbegin(); i < sum.rend()-1; i++) {
        *(i+1) += *i / 2;
        *i %= 2;
    }
    // add room for another bit if sum overflows its current bit-width
    if (*(sum.begin()) > 1) {
        sum.getVecBits()->insert(sum.begin(), sum.getVecBits()->at(0) / 2);
        *(sum.begin() + 1) = sum.getVecBits()->at(1) % 2;
    }

    return sum;
}

// retuns this binary number and other's difference
inline BinaryNumber BinaryNumber::operator-(const BinaryNumber& other) {    
    BinaryNumber thisNum(*this);

    // zero-extend thisNum if its MSB is 1 to make adding with other's twos-complement work properly
    if (thisNum.getVecBits()->at(0) == 1)
        thisNum.getVecBits()->insert(thisNum.begin(), 0);
    
    // add other's twos-complement (subtracting is synonymous with adding a negative)
    thisNum += other.extend(thisNum.bitWidth()).twosComplement();
    vector<int>::iterator thisNumBegin = thisNum.begin();
    // erase the overflow from the addition (not sure why this happens but it does happen and this fixes it)
    // NOTE
    thisNum.getVecBits()->erase(thisNumBegin, thisNumBegin + (thisNum.bitWidth() - this->bitWidth()));

    return thisNum;
}

// binary division
inline BinaryNumber BinaryNumber::operator/(const BinaryNumber& other) {
    // return 0 if this binary number is less than the divisor
    if (*this < other)
        return BinaryNumber(0);

    BinaryNumber thisNum(*this), quotient, remainder(0);
    // binary division algorithm; a more-straightforward rewrite of assembly ISAs' lower-level implementations, which involve more binary operations
    for (vector<int>::iterator i = thisNum.begin(); i < thisNum.end(); i++) {
        // append thisNum's current bit to the remainder's back (identical to left-shifting and adding the bit in question)
        remainder.getVecBits()->push_back(*i);
        
        // append a 0 or 1 to the remainder's back if the remainder is less or greater than the divisor respectively
        if (remainder < other)
            quotient.getVecBits()->push_back(0);
        // if remainder exceeds other, set remainder equal to its difference from other
        else {
            quotient.getVecBits()->push_back(1);
            remainder -= other;      
        }
    }

    return quotient;
}

// binary modulus (remainder after division)
inline BinaryNumber BinaryNumber::operator%(const BinaryNumber& other) {
    // return 0 if this binary number is less than the divisor
    if (*this < other)
        return *this;

    BinaryNumber thisNum(*this), quotient, remainder(0);
    // this method is essentially identical to operator/(), but returns remainder instead of quotient
    for (vector<int>::iterator i = thisNum.begin(); i < thisNum.end(); i++) {
        remainder.getVecBits()->push_back(*i);
        
        if (remainder < other)
            quotient.getVecBits()->push_back(0);
        else {
            quotient.getVecBits()->push_back(1);
            remainder -= other;
        }
    }

    return remainder;
}

// binary unsigned multiplication
inline BinaryNumber BinaryNumber::operator*(const BinaryNumber& other) {
    BinaryNumber product(0), partialProduct, thisNum(*this), otherNum(other);
    
    // match thisNum and otherNum's bit-widths
    if (thisNum.bitWidth() > otherNum.bitWidth())
        otherNum = other.extend(thisNum.bitWidth());
    else if (otherNum.bitWidth() < thisNum.bitWidth())
        thisNum = thisNum.extend(otherNum.bitWidth());
    
    // take each bit in otherNum, AND it with every bit in thisNum, left-shift it according to its location in otherNum, and add it to product
    for (int iOther = otherNum.bitWidth() - 1; iOther >= 0; iOther--) {
        partialProduct = BinaryNumber(0);
        for (int iThis = thisNum.bitWidth() - 1; iThis >= 0; iThis--) {
            partialProduct.getVecBits()->insert(
                partialProduct.begin(),
                otherNum.getVecBits()->at(iOther) && thisNum.getVecBits()->at(iThis)
            );
        }
        partialProduct <<= otherNum.bitWidth()-1 - iOther;
        product += partialProduct;
    }

    return product;
}

inline BinaryNumber BinaryNumber::operator&&(const BinaryNumber& other) {
    BinaryNumber thisNum(*this), otherNum(other);

    // must zero-extend specifically
    if (thisNum.bitWidth() > otherNum.bitWidth())
        otherNum = other.extend(thisNum.bitWidth());
    else if (thisNum.bitWidth() < otherNum.bitWidth())
        thisNum = thisNum.extend(otherNum.bitWidth());

    // logically AND corresponding bits
    for (int i = 0; i < thisNum.bitWidth(); i++)
        thisNum.getVecBits()->at(i) *= otherNum.getVecBits()->at(i);

    return thisNum;
}

inline BinaryNumber BinaryNumber::operator||(const BinaryNumber& other) {
    BinaryNumber thisNum(*this), otherNum(other);

    // must zero-extend specifically
    if (thisNum.bitWidth() > otherNum.bitWidth())
        otherNum = other.extend(thisNum.bitWidth());
    else if (thisNum.bitWidth() < otherNum.bitWidth())
        thisNum = thisNum.extend(otherNum.bitWidth());

    // logically OR corresponding bits
    for (int i = 0; i < thisNum.bitWidth(); i++) {
        thisNum.getVecBits()->at(i) += otherNum.getVecBits()->at(i);
        if (thisNum.getVecBits()->at(i) > 1)
            thisNum.getVecBits()->at(i) = 1;
    }

    return thisNum;
}

inline BinaryNumber BinaryNumber::operator!() {
    BinaryNumber thisNum(*this);
    
    // flip every bit in thisNum
    for (int i : *(thisNum.getVecBits()))
        i = (i + 1) % 2;
    
    return thisNum;
}

inline void BinaryNumber::operator<<=(const int& shiftAmount) {
    *this = *this << shiftAmount;
}

inline void BinaryNumber::operator>>=(const int& shiftAmount) {
    *this = *this >> shiftAmount;
}


inline void BinaryNumber::operator+=(const BinaryNumber& other) {
    *this = *this + other;
}


inline void BinaryNumber::operator-=(const BinaryNumber& other) {
    *this = *this - other;
}


inline void BinaryNumber::operator/=(const BinaryNumber& other) {
    *this = *this / other;
}


inline void BinaryNumber::operator%=(const BinaryNumber& other) {
    *this = *this % other;
}


inline void BinaryNumber::operator*=(const BinaryNumber& other) {
    *this = *this * other;
}


#endif // BINARY_NUMBER