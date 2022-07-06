#pragma once 

#include <sstream>
#include <windows.h>
#include <Psapi.h>
#include "BigInt.h"

// Convert string to int
int Str2IntBase10(const string &str); 
int Str2IntBase2(const string &str); 

// Find type of command
bool isConvert(const string &cmd); 
bool is2Operator(const string &cmd); 

// base = 0 -> Decimal 
// base = 1 -> Binary
void TConvert(ostream &os, string cmd, bool base); 
void T2Operator(ostream &os, string cmd, bool base);
void TNot(ostream &os, string cmd, bool base); 
void TAbs(ostream &os, string cmd, bool base); 
void TMin(ostream &os, string cmd, bool base); 
void TMax(ostream &os, string cmd, bool base); 
void TPow(ostream &os, string cmd, bool base); 
void TDigits(ostream &os, string cmd, bool base); 
void TToStr(ostream &os, string cmd, bool base); 
void TToBase32(ostream &os, string cmd, bool base); 
void TToBase58(ostream &os, string cmd, bool base); 
void TToBase64(ostream &os, string cmd, bool base); 
void TIsPrime(ostream &os, string cmd, bool base); 

void RunCommand(istream &is, ostream &os);