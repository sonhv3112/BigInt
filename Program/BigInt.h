#pragma once 

#include <iostream> 
#include <string> 
#include <cstring>
#include <cstdlib>
#include <cmath> 
#include <fstream> 
#include <algorithm>
#include <ctime> 

using namespace std;

#define BASE 16

struct BigInt {
    int nData; 
    uint16_t *pData; 

    // Constructor
    BigInt();
    BigInt(int16_t x);
    
    // Destructor
    ~BigInt(); 

    // Copy assignment operator (couldn't be friend function)
	BigInt& operator = (const BigInt &y);

    // Resize BigInt
    void resize(int x); 

    // Delete unused space at the end of dynamic array
    void fix(); 
};

//////////////////////////////////////////////////////////
// Utilities functions (1) ///////////////////////////////
//////////////////////////////////////////////////////////

bool isZero(const BigInt &x); 
bool isNegative(const BigInt &x); 
bool isPositive(const BigInt &x); 
void swap(BigInt &x, BigInt &y); 

//////////////////////////////////////////////////////////
// Comparison operator ///////////////////////////////////
//////////////////////////////////////////////////////////

bool operator == (const BigInt &x, const BigInt &y);
bool operator != (const BigInt &x, const BigInt &y);
bool operator < (const BigInt &x, const BigInt &y);
bool operator <= (const BigInt &x, const BigInt &y);
bool operator > (const BigInt &x, const BigInt &y);
bool operator >= (const BigInt &x, const BigInt &y);

//////////////////////////////////////////////////////////
// Bitwise operator //////////////////////////////////////
//////////////////////////////////////////////////////////

BigInt operator & (const BigInt &x, const BigInt &y); 
BigInt operator | (const BigInt &x, const BigInt &y); 
BigInt operator ^ (const BigInt &x, const BigInt &y); 
BigInt operator << (const BigInt &x, int nShift); 
BigInt operator >> (const BigInt &x, int nShift); 

//////////////////////////////////////////////////////////
// Arithmetic operator ///////////////////////////////////
//////////////////////////////////////////////////////////

// Find quotient of x / y (0 < quotient < 65536)
uint16_t EQuotient(const BigInt &x, const BigInt &y);

BigInt operator + (const BigInt &x, const BigInt &y); 
BigInt operator - (const BigInt &x, const BigInt &y); 
BigInt operator * (const BigInt &x, const int32_t &y);
BigInt operator * (const BigInt &x, const BigInt &y); 
BigInt operator / (const BigInt &x, const int32_t &y); 
BigInt operator / (const BigInt &x, const BigInt &y); 
BigInt operator % (const BigInt &x, const int32_t &y); 
BigInt operator % (const BigInt &x, const BigInt &y); 
BigInt operator - (const BigInt &x); 
BigInt operator ~ (const BigInt &x); 

//////////////////////////////////////////////////////////
// Assignment operator ///////////////////////////////////
//////////////////////////////////////////////////////////

void operator += (BigInt &x, const BigInt &y); 
void operator -= (BigInt &x, const BigInt &y); 
void operator *= (BigInt &x, const BigInt &y); 
void operator /= (BigInt &x, const BigInt &y); 
void operator %= (BigInt &x, const BigInt &y); 
void operator &= (BigInt &x, const BigInt &y); 
void operator |= (BigInt &x, const BigInt &y); 
void operator ^= (BigInt &x, const BigInt &y); 
void operator <<= (BigInt &x, int nShift); 
void operator >>= (BigInt &x, int nShift); 

//////////////////////////////////////////////////////////
// Arithmetic functions //////////////////////////////////
//////////////////////////////////////////////////////////

BigInt abs(const BigInt &x); 
BigInt min(const BigInt &x, const BigInt &y); 
BigInt max(const BigInt &x, const BigInt &y); 
BigInt pow(const BigInt &x, const BigInt &y); 

//////////////////////////////////////////////////////////
// Utilities functions //////////////////////////////////
//////////////////////////////////////////////////////////

int16_t to_int(const BigInt &x); 
string to_string(const BigInt &x); // BigInt to base 10
string to_base32(const BigInt &x); 
string to_base64(const BigInt &x); 
string to_base58(const BigInt &x); 
int digits(const BigInt &x, const int &base); 

//////////////////////////////////////////////////////////
// Miller-Rabin test /////////////////////////////////////
//////////////////////////////////////////////////////////

BigInt powMod(const BigInt &x, const BigInt &y, const BigInt &mod); 
BigInt Rand(const BigInt &x);
bool is_prime(const BigInt &x); 

//////////////////////////////////////////////////////////
// I/O operator //////////////////////////////////////////
//////////////////////////////////////////////////////////

// string (integer) divide functions 
string divString(string dividend, uint64_t divisor, uint16_t &remainder);
BigInt Base2ToBigInt(string str); 
BigInt Base10ToBigInt(string str);
string BigIntToBase2(const BigInt &x);

void readBinary(istream& is, BigInt &x);
void readDecimal(istream& is, BigInt &x);
void writeDecimal(ostream &os, const BigInt &x);
void writeBinary(ostream &os, const BigInt &x);