#include "Command.h"

int Str2IntBase10(const string &str) { 
    int ret = 0; 
    for (int i = 0; i < str.length(); ++i) 
        ret = ret * 10 + str[i] - '0'; 
    return ret; 
}

int Str2IntBase2(const string &str) { 
    int ret = 0; 
    for (int i = 0; i < str.length(); ++i) 
        ret = (ret << 1) | (str[i] - '0'); 
    return ret; 
}

bool isConvert(const string &cmd) { 
    stringstream ss(cmd); 
    string pre = "", cur = ""; 
    int count = 0; 
    while (ss >> cur) { 
        count++; 
        if (count > 3)  
            return false; 
        if (count == 1) 
            pre = cur; 
        if (count == 2 && !(pre == "2" && cur == "10") && !(pre == "10" && cur == "2"))
            return false; 
    }
    return count == 3; 
}

bool is2Operator(const string &cmd) { 
    stringstream ss(cmd); 
    string ope; 
    ss >> ope >> ope >> ope; 
    if (ope == "+") return true; 
    if (ope == "-") return true; 
    if (ope == "*") return true; 
    if (ope == "/") return true; 
    if (ope == "%") return true; 
    if (ope == "&") return true; 
    if (ope == "|") return true; 
    if (ope == "^") return true; 
    if (ope == "<<") return true; 
    if (ope == ">>") return true; 
    return false; 
}

void TConvert(ostream &os, string cmd, bool base) { 
    BigInt x; 
    if (!base) { 
        x = Base10ToBigInt(cmd); 
        writeBinary(os, x); 
    } else { 
        x = Base2ToBigInt(cmd); 
        writeDecimal(os, x); 
    }
} 

void T2Operator(ostream &os, string cmd, bool base) { 
    stringstream ss(cmd); 
    string sx, sy, ope; 
    ss >> sx >> sx >> ope >> sy; 
    BigInt x, y, res; 
    int ty; 
    if (!base) { 
        x = Base10ToBigInt(sx); 
        if (ope == "<<" || ope == ">>") 
            ty = Str2IntBase10(sy); 
        else
            y = Base10ToBigInt(sy); 
    } else { 
        x = Base2ToBigInt(sx); 
        if (ope == "<<" || ope == ">>") 
            ty = Str2IntBase2(sy); 
        else
            y = Base2ToBigInt(sy); 
    }
    if (ope == "+") res = x + y; 
    if (ope == "-") res = x - y;  
    if (ope == "*") res = x * y;  
    if (ope == "/") res = x / y;  
    if (ope == "%") res = x % y;  
    if (ope == "&") res = x & y;  
    if (ope == "|") res = x | y;  
    if (ope == "^") res = x ^ y;  
    if (ope == "<<") res = x << ty;  
    if (ope == ">>") res = x >> ty;  
    if (!base) 
        writeDecimal(os, res);  
    else { 
        string temp = BigIntToBase2(res); 
        os << temp << endl; 
    }
}

void TNot(ostream &os, string cmd, bool base) { 
    stringstream ss(cmd); 
    string sx; 
    ss >> sx >> sx >> sx; 
    BigInt x; 
    if (!base) { 
        x = Base10ToBigInt(sx);
        x = ~x;  
        writeDecimal(os, x); 
    } else { 
        x = Base2ToBigInt(sx); 
        x = ~x; 
        writeBinary(os, x); 
    }
}

// get operand inside bracket (abs, digits, to_string, to_base, is_prime)
void get1Operand(string cmd, string &x, int length) { 
    stringstream ss(cmd); 
    ss >> x >> x; 
    x.erase(0, length + 1); x.pop_back(); 
}

// get operand inside bracket (min, max, pow)
void get2Operand(string cmd, string &x, string &y, int length) { 
    stringstream ss(cmd); 
    ss >> x >> x >> y; 
    x.erase(0, length + 1); x.pop_back(); 
    y.pop_back(); 
}


void TAbs(ostream &os, string cmd, bool base) { 
    string sx; 
    get1Operand(cmd, sx, 3); 
    BigInt x; 
    if (!base) { 
        x = Base10ToBigInt(sx); 
        x = abs(x); 
        writeDecimal(os, x); 
    } else { 
        x = Base2ToBigInt(sx); 
        x = abs(x); 
        writeBinary(os, x); 
    }   
}

void TMin(ostream &os, string cmd, bool base) { 
    string sx, sy; 
    get2Operand(cmd, sx, sy, 3); 
    BigInt x, y; 
    if (!base) { 
        x = Base10ToBigInt(sx); 
        y = Base10ToBigInt(sy); 
        writeDecimal(os, min(x, y)); 
    } else { 
        x = Base2ToBigInt(sx); 
        y = Base2ToBigInt(sy); 
        writeBinary(os, min(x, y)); 
    }
}

void TMax(ostream &os, string cmd, bool base) { 
    string sx, sy; 
    get2Operand(cmd, sx, sy, 3); 
    BigInt x, y; 
    if (!base) { 
        x = Base10ToBigInt(sx); 
        y = Base10ToBigInt(sy);
        writeDecimal(os, max(x, y)); 
    } else { 
        x = Base2ToBigInt(sx); 
        y = Base2ToBigInt(sy); 
        writeBinary(os, max(x, y)); 
    }
}

void TPow(ostream &os, string cmd, bool base) { 
    string sx, sy; 
    get2Operand(cmd, sx, sy, 3); 
    BigInt x, y; 
    if (!base) { 
        x = Base10ToBigInt(sx); 
        y = Base10ToBigInt(sy); 
        writeDecimal(os, pow(x, y)); 
    } else { 
        x = Base2ToBigInt(sx); 
        y = Base2ToBigInt(sy); 
        writeBinary(os, pow(x, y)); 
    }
}

void TDigits(ostream &os, string cmd, bool base) { 
    string sx; 
    get1Operand(cmd, sx, 6); 
    BigInt x; 
    if (!base) 
        x = Base10ToBigInt(sx); 
    else 
        x = Base2ToBigInt(sx); 
    os << digits(x, !base ? 10 : 2) << endl; 
}

void TToStr(ostream &os, string cmd, bool base) { 
    string sx; 
    get1Operand(cmd, sx, 9); 
    BigInt x; 
    if (!base) 
        x = Base10ToBigInt(sx); 
    else 
        x = Base2ToBigInt(sx); 
    os << to_string(x) << endl; 
}

void TToBase32(ostream &os, string cmd, bool base) { 
    string sx; 
    get1Operand(cmd, sx, 9); 
    BigInt x; 
    if (!base) 
        x = Base10ToBigInt(sx); 
    else 
        x = Base2ToBigInt(sx); 
    os << to_base32(x) << endl; 
}

void TToBase58(ostream &os, string cmd, bool base) { 
    string sx; 
    get1Operand(cmd, sx, 9); 
    BigInt x; 
    if (!base) 
        x = Base10ToBigInt(sx); 
    else 
        x = Base2ToBigInt(sx); 
    os << to_base58(x) << endl; 
}

void TToBase64(ostream &os, string cmd, bool base) { 
    string sx; 
    get1Operand(cmd, sx, 9); 
    BigInt x; 
    if (!base) 
        x = Base10ToBigInt(sx); 
    else 
        x = Base2ToBigInt(sx);  
    os << to_base64(x) << endl; 
}

void TIsPrime(ostream &os, string cmd, bool base) { 
    string sx; 
    get1Operand(cmd, sx, 8); 
    BigInt x; 
    if (!base) 
        x = Base10ToBigInt(sx); 
    else 
        x = Base2ToBigInt(sx); 
    os << (is_prime(x) ? "true" : "false") << endl; 
}

void RunBigIntCommand(ostream &os, string cmd) { 
    stringstream ss(cmd); 
    string buffer; 

    // get base
    bool base; 
    ss >> buffer; 
    base = (buffer == "2"); 

    if (isConvert(cmd)) { 
        ss >> buffer >> buffer;
        TConvert(os, buffer, base);  
    } else if (is2Operator(cmd)) { 
        T2Operator(os, cmd, base);  
    } else { 
        string type; 
        ss >> type; 
        if (type == "~") TNot(os, cmd, base); 
        else if (type.substr(0, 3) == "abs") TAbs(os, cmd, base); 
        else if (type.substr(0, 3) == "min") TMin(os, cmd, base); 
        else if (type.substr(0, 3) == "max") TMax(os, cmd, base); 
        else if (type.substr(0, 3) == "pow") TPow(os, cmd, base); 
        else if (type[0] == 'd') TDigits(os, cmd, base); 
        else if (type.substr(0, 8) == "is_prime") TIsPrime(os, cmd, base); 
        else if (type.substr(0, 9) == "to_string") TToStr(os, cmd, base); 
        else if (type.substr(0, 9) == "to_base32") TToBase32(os, cmd, base); 
        else if (type.substr(0, 9) == "to_base58") TToBase58(os, cmd, base); 
        else if (type.substr(0, 9) == "to_base64") TToBase64(os, cmd, base); 
    }
}

void RunCommand(istream &is, ostream &os) { 
    double time_1 = clock(); 
    auto myHandle = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(myHandle, &pmc, sizeof(pmc));
    size_t mem_1 = pmc.PeakWorkingSetSize; 
    
    string cmd; 

    while (getline(is, cmd)) { 
        if (cmd == "get_info") { 
            GetProcessMemoryInfo(myHandle, &pmc, sizeof(pmc));
            os << "Excution time: " << (clock() - time_1) / CLOCKS_PER_SEC << "s\n"; 
            os << "Memory usage: " << pmc.PeakWorkingSetSize - mem_1 << " Bytes\n"; 
        } else { 
            RunBigIntCommand(os, cmd); 
        }
    }
}