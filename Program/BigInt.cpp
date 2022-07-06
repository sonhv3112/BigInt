#include "BigInt.h"

/////////////////////////////////////
// functions inside struct //////////
/////////////////////////////////////

BigInt::BigInt() {
    nData = 1; 
    pData = new uint16_t [1]; 
    pData[0] = 0; 
}

BigInt::BigInt(int16_t x) { 
    nData = 1; 
    pData = new uint16_t [1]; 
    pData[0] = x; 
}

BigInt::~BigInt() { 
    if (nData && pData != NULL) 
        delete []pData; 
    nData = 0; 
    pData = NULL; 
}

BigInt& BigInt::operator = (const BigInt &y) { 
    if (nData && pData != y.pData)
        delete []pData;  
    nData = y.nData; 
	if (nData) { 
		pData = new uint16_t [nData]; 
		memcpy(pData, y.pData, sizeof(uint16_t) * nData); 
	} else 
		pData = NULL; 
    return (*this); 
}

void BigInt::resize(int x) { 
    uint16_t *pTemp = new uint16_t [x]; 
    unsigned char padding_value = 0x00; 
    padding_value = (nData && (pData[nData - 1] >> (BASE - 1) & 1)) ? 0xff : 0x00;
	memset(pTemp, padding_value, sizeof(uint16_t) * x); 
    memcpy(pTemp, pData, sizeof(uint16_t) * min(x, nData)); 
    if (nData) delete []pData;
    pData = pTemp; 
    nData = x; 
}

void BigInt::fix() {  
    if (nData <= 1) return;
    uint16_t sign = (pData[nData - 1] >> (BASE - 1) & 1);
    for (int i = nData - 1; i >= 0; --i) 
        if (pData[i] != sign * UINT16_MAX) {
            if ((pData[i] >> (BASE - 1) & 1) == sign) 
                (*this).resize(i + 1);  
            else 
                (*this).resize(i + 2); 
            return; 
        }
    (*this).resize(1); 
}

//////////////////////////////////////////////////////////
// Utilities functions (1) ///////////////////////////////
//////////////////////////////////////////////////////////

bool isZero(const BigInt &x) { 
    return x.nData == 1 && x.pData[0] == 0; 
}

bool isNegative(const BigInt &x) { 
    return x.pData[x.nData - 1] >> (BASE - 1) & 1; 
}

bool isPositive(const BigInt &x) { 
    return !isZero(x) && !isNegative(x); 
}

void swap(BigInt &x, BigInt &y) { 
    BigInt temp; 
    temp = x; 
    x = y; 
    y = temp; 
}

//////////////////////////////////////////////////////////
// Comparison operator ///////////////////////////////////
//////////////////////////////////////////////////////////

bool operator == (const BigInt &x, const BigInt &y) { 
    if (x.nData != y.nData) 
        return false; 
    for (int i = 0; i < x.nData; ++i) 
        if (x.pData[i] != y.pData[i]) 
            return false;
    return true; 
}

bool operator != (const BigInt &x, const BigInt &y) { 
    return !(x == y); 
}

bool operator < (const BigInt &x, const BigInt &y) { 
    int sign_x = isNegative(x), 
        sign_y = isNegative(y);
    if (sign_x != sign_y) 
        return sign_x > sign_y; 
    if (x.nData != y.nData) 
        return (x.nData < y.nData) ^ sign_x;
    for (int i = x.nData - 1; i >= 0; --i) 
        if (x.pData[i] != y.pData[i]) 
            return x.pData[i] < y.pData[i];
    return false;
}

bool operator <= (const BigInt &x, const BigInt &y) { 
    return x < y || x == y; 
}

bool operator > (const BigInt &x, const BigInt &y) { 
    return !(x <= y);
}

bool operator >= (const BigInt &x, const BigInt &y) { 
    return !(x < y);
}

//////////////////////////////////////////////////////////
// Bitwise operator //////////////////////////////////////
//////////////////////////////////////////////////////////

BigInt operator & (const BigInt &x, const BigInt &y) { 
    BigInt temp_x, temp_y; 
    temp_x = x; temp_y = y; 
    temp_x.resize(max(temp_y.nData, temp_x.nData) + 1); 
    temp_y.resize(temp_x.nData);
    for (int i = 0; i < temp_x.nData; ++i) 
        temp_x.pData[i] &= temp_y.pData[i]; 
    temp_x.fix(); 
    return temp_x; 
}

BigInt operator | (const BigInt &x, const BigInt &y) { 
    BigInt temp_x, temp_y; 
    temp_x = x; temp_y = y; 
    temp_x.resize(max(temp_y.nData, temp_x.nData) + 1); 
    temp_y.resize(temp_x.nData);
    for (int i = 0; i < temp_x.nData; ++i) 
        temp_x.pData[i] |= temp_y.pData[i]; 
    temp_x.fix(); 
    return temp_x; 
}

BigInt operator ^ (const BigInt &x, const BigInt &y) { 
    BigInt temp_x, temp_y; 
    temp_x = x; temp_y = y; 
    temp_x.resize(max(temp_y.nData, temp_x.nData) + 1); 
    temp_y.resize(temp_x.nData);
    for (int i = 0; i < temp_x.nData; ++i) 
        temp_x.pData[i] ^= temp_y.pData[i]; 
    temp_x.fix(); 
    return temp_x; 
}

BigInt operator << (const BigInt &x, int nShift) { 
    BigInt ret; 
    ret = x; 

    // Shift block
    int nBlock = nShift / BASE; 
    if (nBlock) { 
        ret.resize(ret.nData + nBlock); 
        for (int i = ret.nData - 1; i >= 0; --i) 
            ret.pData[i] = i >= nBlock ? ret.pData[i - nBlock] : 0;
    }

    // Shift bit
    int nBit = nShift % BASE;  
    if (nBit) { 
        ret.resize(ret.nData + 1);  
        for (int i = ret.nData - 1; i >= 0; --i) { 
            ret.pData[i] <<= nBit; 
            if (i > 0) 
                ret.pData[i] |= (ret.pData[i - 1] >> (BASE - nBit)); 
        }
        ret.fix(); 
    }

    return ret; 
}

BigInt operator >> (const BigInt &x, int nShift) {
    BigInt ret; 
    ret = x; 

    // Shift block
    int nBlock = nShift / BASE;
    if (nBlock) {  
        for (int i = 0; i < ret.nData - nBlock; ++i) 
            ret.pData[i] = ret.pData[i + nBlock]; 
        ret.resize(ret.nData - nBlock); 
    }

    // Shift bit
    int nBit = nShift % BASE;   
    if (nBit) { 
        bool negative = isNegative(ret);  
        for (int i = 0; i < ret.nData; ++i) { 
            ret.pData[i] >>= nBit; 
            if (i + 1 != ret.nData) 
                ret.pData[i] |= (ret.pData[i + 1] & ((1ll << nBit) - 1)) << (BASE - nBit); 
        }
        if (negative) 
        for (int i = 0; i < nBit; ++i) 
            ret.pData[ret.nData - 1] |= (1 << (BASE - i - 1)); 
        ret.fix();
    }

    return ret; 
}

//////////////////////////////////////////////////////////
// Arithmetic operator ///////////////////////////////////
//////////////////////////////////////////////////////////

BigInt operator - (const BigInt &x) { 
    BigInt ret;
    ret = x; 
    ret.resize(ret.nData + 1);

    for (int i = 0; i < ret.nData; ++i) 
        ret.pData[i] = ~ret.pData[i]; 

    for (int i = 0; i < ret.nData; ++i) { 
        if (ret.pData[i] != UINT16_MAX) { 
            ret.pData[i]++; 
            break; 
        }
        ret.pData[i] = 0; 
    }

    ret.fix();

    return ret; 
}

BigInt operator + (const BigInt &x, const BigInt &y) { 
    BigInt ret; 
    ret.resize(max(x.nData, y.nData) + 1); 

    int32_t carry = 0; 
    int8_t  sign_x = isNegative(x), 
            sign_y = isNegative(y);

    for (int i = 0; i < ret.nData; ++i) { 
        int32_t temp = carry; 
        temp += (int32_t)(i < x.nData ? x.pData[i] : sign_x * UINT16_MAX); 
        temp += (int32_t)(i < y.nData ? y.pData[i] : sign_y * UINT16_MAX); 
        carry = temp > UINT16_MAX; 
        ret.pData[i] = temp - (carry ? UINT16_MAX + 1 : 0); 
    }

    ret.fix(); 

    return ret; 
}

BigInt operator - (const BigInt &x, const BigInt &y) { 
    return x + (-y); 
}

BigInt operator * (const BigInt &x, const int32_t &y) { 
    /* 
        non-negative * non-negative = non-negative 
        non-negative * negative = negative 
        negative * negative = non-negative 
    */ 

    BigInt ret, temp_x; 
    int32_t temp_y; 
    temp_x = x; if (isNegative(x)) temp_x = -x;  // temp_x = abs(x)
    temp_y = abs(y); // temp_y = abs(y)

    ret.resize(temp_x.nData + 2); 
    for (int i = 0; i < temp_x.nData; ++i) {
        int64_t temp = 1ll * temp_x.pData[i] * temp_y; 
        for (int k = 0; ; ++k) { 
            temp += ret.pData[i + k]; 
            ret.pData[i + k] = temp % (UINT16_MAX + 1); 
            temp /= (UINT16_MAX + 1); 
            if (!temp) break; 
        }
    }

    if (isNegative(x) ^ (y < 0))
        ret = -ret; 
    else 
        ret.fix(); 

    return ret; 
}

BigInt operator * (const BigInt &x, const BigInt &y) { 
    /* 
        non-negative * non-negative = non-negative 
        non-negative * negative = negative 
        negative * negative = non-negative 
    */ 

    BigInt ret, temp_x, temp_y; 
    temp_x = x; if (isNegative(x)) temp_x = -x;  // temp_x = abs(x)
    temp_y = y; if (isNegative(y)) temp_y = -y;  // temp_y = abs(y)

    ret.resize(temp_x.nData + temp_y.nData + 1); 
    for (int i = 0; i < temp_x.nData; ++i) 
        for (int j = 0; j < temp_y.nData; ++j) { 
            int64_t temp = 1ll * temp_x.pData[i] * temp_y.pData[j]; 
            for (int k = 0; ; ++k) { 
                temp += ret.pData[i + j + k]; 
                ret.pData[i + j + k] = temp % (UINT16_MAX + 1); 
                temp /= (UINT16_MAX + 1); 
                if (!temp) break; 
            }
        }

    if (isNegative(x) ^ isNegative(y))
        ret = -ret; 
    else 
        ret.fix(); 

    return ret; 
}

// Heuristic: Estimate quotient of x / y (quotient < 65536) XXXXXX failure

// Find quotient of x / y (0 < quotient < 65536)
uint16_t EQuotient(const BigInt &x, const BigInt &y) { 
    uint16_t ret = 0; 
    for (int i = BASE - 1; i >= 0; --i) { 
        ret += (1 << i); 
        if (y * ret > x) 
            ret -= (1 << i); 
    }
    return ret;  
}

BigInt operator / (const BigInt &x, const int32_t &y) { 
    BigInt quotient, temp_x;
    temp_x = x; if (isNegative(x)) temp_x = -x;  // temp_x = abs(x)
    int64_t modulor = 0; 

    for (int i = temp_x.nData - 1; i >= 0; --i) { 
        quotient <<= BASE; 
        modulor <<= BASE; 
        modulor += temp_x.pData[i];  
        if (modulor < abs(y)) 
            continue;   
        quotient.pData[0] = modulor / abs(y);  
        modulor = modulor - 1ll * abs(y) * quotient.pData[0]; 
    }

    if (isNegative(x) ^ (y < 0)) 
        quotient = -quotient; 
    else 
        quotient.fix(); 

    return quotient; 
}

BigInt operator / (const BigInt &x, const BigInt &y) { 
    /* 
        non-negative / non-negative = non-negative 
        non-negative / negative = negative 
        negative / non-negative = negative 
        negative / negative = non-negative 
    */ 

    BigInt quotient, modulor, temp_x, temp_y; 
    temp_x = x; if (isNegative(x)) temp_x = -x;  // temp_x = abs(x)
    temp_y = y; if (isNegative(y)) temp_y = -y;  // temp_y = abs(y)

    for (int i = temp_x.nData - 1; i >= 0; --i) { 
        // quotient = ShiftBlockL(quotient, 1); 
        // modulor = ShiftBlockL(modulor, 1); 
        quotient <<= BASE; 
        modulor <<= BASE; 
        modulor.pData[0] = temp_x.pData[i]; 
        modulor.fix(); 
        if (modulor < temp_y) 
            continue;   
        quotient.pData[0] = EQuotient(modulor, temp_y); 
        modulor = modulor - temp_y * quotient.pData[0]; 
    }

    if (isNegative(x) ^ isNegative(y)) 
        quotient = -quotient; 
    else 
        quotient.fix(); 

    return quotient; 
}

BigInt operator % (const BigInt &x, const int32_t &y) { 
    int32_t modulor = 0, powBase = 1; 
    BigInt temp_x; 
    temp_x = x; if (isNegative(x)) temp_x = -x;  

    for (int i = 0; i < temp_x.nData; ++i) { 
        modulor = (1ll * modulor + 1ll * temp_x.pData[i] * powBase % y) % y;  
        powBase = 1ll * powBase * (1 << BASE) % y; 
    }

    BigInt ret; 
    ret.resize(2); 
    ret.pData[0] = modulor % (1 << BASE); 
    ret.pData[1] = modulor / (1 << BASE);  
    ret.fix(); 
    if (isNegative(x)) 
        ret = -ret; 
    
    return ret; 
}

BigInt operator % (const BigInt &x, const BigInt &y) { 
    /* 
        non-negative % y = non-negative 
        negative / y = negative
    */ 

    BigInt modulor, temp_x, temp_y; 
    temp_x = x; if (isNegative(x)) temp_x = -x;  // temp_x = abs(x)
    temp_y = y; if (isNegative(y)) temp_y = -y;  // temp_y = abs(y)

    for (int i = temp_x.nData - 1; i >= 0; --i) { 
        // modulor = ShiftBlockL(modulor, 1); 
        modulor <<= BASE; 
        modulor.pData[0] = temp_x.pData[i]; 
        modulor.fix(); 
        if (modulor < temp_y) 
            continue; 
        int32_t temp = EQuotient(modulor, temp_y); 
        modulor = modulor - temp_y * temp; 
    }

    if (isNegative(x)) 
        modulor = -modulor; 

    return modulor; 
}

BigInt operator ~ (const BigInt &x) { 
    BigInt ret; 
    ret = x;
    for (int i = 0; i < ret.nData; ++i) 
        ret.pData[i] = ~ret.pData[i]; 
    ret.fix(); 
    return ret; 
}

//////////////////////////////////////////////////////////
// Assignment operator ///////////////////////////////////
//////////////////////////////////////////////////////////

void operator += (BigInt &x, const BigInt &y) { x = x + y; }
void operator -= (BigInt &x, const BigInt &y) { x = x - y; }
void operator *= (BigInt &x, const BigInt &y) { x = x * y; }
void operator /= (BigInt &x, const BigInt &y) { x = x / y; }
void operator %= (BigInt &x, const BigInt &y) { x = x % y; }
void operator &= (BigInt &x, const BigInt &y) { x = x & y; }
void operator |= (BigInt &x, const BigInt &y) { x = x | y; }
void operator ^= (BigInt &x, const BigInt &y) { x = x ^ y; }
void operator <<= (BigInt &x, int nShift) { x = x << nShift; }
void operator >>= (BigInt &x, int nShift) { x = x >> nShift; } 

//////////////////////////////////////////////////////////
// Arithmetic functions //////////////////////////////////
//////////////////////////////////////////////////////////

BigInt abs(const BigInt &x) { 
    BigInt ret; 
    ret = x; 
    if (isNegative(x)) 
        ret = -ret; 
    return ret; 
}

BigInt min(const BigInt &x, const BigInt &y) { 
    BigInt temp_x, temp_y; 
    temp_x = x; temp_y = y; 
    if (temp_x > temp_y) 
        swap(temp_x, temp_y); 
    return temp_x; 
}

BigInt max(const BigInt &x, const BigInt &y) { 
    BigInt temp_x, temp_y; 
    temp_x = x; temp_y = y;  
    if (temp_x < temp_y) 
        swap(temp_x, temp_y); 
    return temp_x; 
}

BigInt pow(const BigInt &x, const BigInt &y) { 
    BigInt ret(1), temp;    
    temp = x; 
    if (isNegative(y)) return ret; 
    for (int i = 0; i < y.nData; ++i) { 
        for (int j = 0; j < BASE; ++j) { 
            if (y.pData[i] >> j & 1) 
                ret = ret * temp; 
            temp = temp * temp; 
            if (i == y.nData - 1 && !(1ll * y.pData[y.nData - 1] & ~((1ll << j + 1) - 1))) 
                break; 
        }
    }
    return ret; 
}

//////////////////////////////////////////////////////////
// Ultilities functions //////////////////////////////////
//////////////////////////////////////////////////////////

int16_t to_int(const BigInt &x) { 
    if (x.nData == 0)   
        return 0; 
    return x.pData[0]; 
}

string to_string(const BigInt &x) { 
    string ret = ""; 
    BigInt temp = abs(x); 
    while (!isZero(temp)) { 
        ret += (char)(to_int(temp % 10) + '0'); 
        temp /= 10; 
    }
    if (isNegative(x)) 
        ret += '-'; 
    reverse(ret.begin(), ret.end()); 
    if (ret.empty()) 
        ret += '0';
    return ret;
}

string to_base32(const BigInt &x) { 
    string ret = ""; 
    BigInt temp = abs(x); 
    while (!isZero(temp)) { 
        int remainder = to_int(temp % 32); 
        temp /= 32; 
        ret += (char)(remainder + (remainder < 26 ? 'A' : '2' - 26)); 
    }
    if (isNegative(x)) 
        ret += '-'; 
    reverse(ret.begin(), ret.end()); 
    if (ret.empty()) 
        ret += '0'; 
    return ret; 
}

string to_base64(const BigInt &x) { 
    string ret = ""; 
    BigInt temp = abs(x); 
    while (!isZero(temp)) { 
        int remainder = to_int(temp % 64); 
        temp /= 64; 
        if (remainder < 26)
            ret += (char)(remainder + 'A'); 
        else if (remainder < 52) 
            ret += (char)(remainder + 'a' - 26); 
        else if (remainder < 62) 
            ret += (char)(remainder + '0' - 52); 
        else if (remainder == 62) 
            ret += '+'; 
        else 
            ret += '\\';
    }
    if (isNegative(x)) 
        ret += '-'; 
    reverse(ret.begin(), ret.end()); 
    if (ret.empty()) 
        ret += '0';
    return ret; 
}

string to_base58(const BigInt &x) { 
    string ret = ""; 
    BigInt temp = abs(x); 
    while (!isZero(temp)) { 
        int remainder = to_int(temp % 58); 
        temp /= 58; 
        if (remainder < 9)
            ret += (char)(remainder + '1'); 
        else if (remainder < 16) 
            ret += (char)(remainder + 'A' - 9); 
        else if (remainder < 22) 
            ret += (char)(remainder + 'B' - 9); 
        else if (remainder < 33) 
            ret += (char)(remainder + 'C' - 9); 
        else if (remainder < 44) 
            ret += (char)(remainder + 'a' - 33); 
        else 
            ret += (char)(remainder + 'b' - 33); 
    }
    if (isNegative(x)) 
        ret += '-'; 
    reverse(ret.begin(), ret.end()); 
    if (ret.empty()) 
        ret += '1'; 
    return ret; 
}

int digits(const BigInt &x, const int &base) { 
    int ret = 0; 
    BigInt temp; 
    if (base == 2) temp = x; 
    else temp = abs(x); 
    if (base == 2) 
        return BigIntToBase2(temp).length(); 
    return to_string(temp).length();
}

//////////////////////////////////////////////////////////
// Miller-Rabin test /////////////////////////////////////
//////////////////////////////////////////////////////////

BigInt powMod(const BigInt &x, const BigInt &y, const BigInt &mod) { 
    BigInt ret(1), temp;    
    temp = x; 
    if (isNegative(y)) return ret;  
    for (int i = 0; i < y.nData; ++i) { 
        for (int j = 0; j < BASE; ++j) { 
            if (y.pData[i] >> j & 1) { 
                ret = ret * temp % mod;
            } 
            temp = temp * temp % mod; 
            if (i == y.nData - 1 && !(1ll * y.pData[y.nData - 1] & ~((1ll << j + 1) - 1))) 
                break; 
        }
    }
    return ret; 
}

BigInt Rand(const BigInt &x) { 
    BigInt ret; 
    ret = x; 
    if (isNegative(ret)) return ret; 
    for (int i = ret.nData - 1; i >= 0; --i) { 
        if (rand() % 2) { 
            ret.pData[i] = rand() % x.pData[i]; 
            for (int j = i - 1; j >= 0; --j) 
                ret.pData[j] = rand() % (1ll * UINT16_MAX + 1);  
            break; 
        } 
        ret.pData[i] = x.pData[i]; 
    }
    ret.fix(); 
    if (ret < BigInt(2)) 
        ret = BigInt(2); 
    return ret; 
}

bool check(const BigInt &x, const BigInt &y, int step) { 
    BigInt B1(1); 
    BigInt temp = Rand(y - BigInt(2)), y1 = y - B1; 
    temp = powMod(temp, x, y);
    if (temp == B1 || temp == y1) 
        return true; 
    for (int i = 0; i < step; ++i) { 
        temp = temp * temp % y; 
        if (temp == B1) 
            return false; 
        if (temp == y1) 
            return true; 
    }
    return false; 
}

bool is_prime(const BigInt &x) { 
    BigInt B1(1), B4(4); 
    if (x <= B1) return false;
    if (x < B4) return true; 
    if (x.pData[0] % 2 == 0) return false;  

    BigInt temp = x - B1;
    int step = 0;  
    while (temp.pData[0] % 2 == 0) { 
        temp /= 2; 
        step++; 
    }

    const int nTest = 10; 
    for (int i = 0; i < nTest; ++i) 
        if (!check(temp, x, step)) 
            return false; 

    return true; 
}

//////////////////////////////////////////////////////////
// I/O operator //////////////////////////////////////////
//////////////////////////////////////////////////////////

string divString(string dividend, uint64_t divisor, uint16_t &remainder) { 
    string quotient = ""; 
    uint64_t carry = 0; 
    for (char pNum: dividend) { 
        carry = carry * 10 + pNum - '0'; 
        if (quotient.empty() && carry < divisor) 
            continue; 
        quotient += (char)(carry / divisor + '0'); 
        carry %= divisor; 
    }
    remainder = carry; 
    return quotient; 
}

BigInt Base2ToBigInt(string str) { 
    BigInt ret; 
    ret.resize((str.length() - 1) / BASE + 1);
    reverse(str.begin(), str.end()); 
    for (int i = 0; i < str.length(); ++i) 
        if (str[i] == '1') 
            ret.pData[i / BASE] |= (1 << (i % BASE)); 
    if (str.back() == '1') 
        for (int i = 0; i < (BASE - str.length() % BASE) % BASE; ++i) 
            ret.pData[ret.nData - 1] |= (1 << (BASE - 1) - i);  
    return ret; 
}

BigInt Base10ToBigInt(string str) { 
    BigInt ret; 
    ret.resize(1000000); 
    bool negative = false; 
    if (str[0] == '-') {
        negative = true;  
        str.erase(str.begin()); 
    }
    ret.nData = 0; 
    while (!str.empty()) 
        str = divString(str, 1ll * UINT16_MAX + 1, ret.pData[ret.nData++]);
    ret.nData++;
    ret.resize(ret.nData);
    if (negative) 
        ret = -ret; 
    ret.fix();
    return ret; 
}

string BigIntToBase2(const BigInt &x) {  
    string ret = ""; 
    bool sign = isNegative(x), flag = true; 
    for (int i = x.nData - 1; i >= 0; --i) 
        for (int j = BASE - 1; j >= 0; --j) { 
            bool bit = (x.pData[i] >> j & 1); 
            if (flag && bit == sign) 
                continue; 
            if (flag) { 
                ret += (char)(sign + '0');
                flag = false; 
            }
            ret += (char)(bit + '0'); 
        }
    if (ret.empty()) ret += (x.nData && x.pData[0]) ? '1' : '0'; 
    return ret; 
}

void readBinary(istream& is, BigInt &x) { 
    string buffer; 
    is >> buffer; 
    x = Base2ToBigInt(buffer); 
}

void readDecimal(istream& is, BigInt &x) { 
    string buffer; 
    is >> buffer;
    x = Base10ToBigInt(buffer); 
}

void writeDecimal(ostream &os, const BigInt &x) { 
    os << to_string(x); os << endl; 
}

void writeBinary(ostream &os, const BigInt &x) {
    os << BigIntToBase2(x); os << endl; 
}
