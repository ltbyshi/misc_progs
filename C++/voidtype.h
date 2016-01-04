#ifndef __VOIDTYPE_H__
#define __VOIDTYPE_H__
#define CAST_VOID(ti, psrc, pdest, stmt, defstmt) \
    if(ti == typeid(char)){char* pdest = (char*)psrc; stmt;} \
    else if(ti == typeid(unsigned char)){unsigned char* pdest = (unsigned char*)psrc; stmt;} \
    else if(ti == typeid(short)){short* pdest = (short*)psrc; stmt;} \
    else if(ti == typeid(unsigned short)){unsigned short* pdest = (unsigned short*)psrc; stmt;} \
    else if(ti == typeid(int)){int* pdest = (int*)psrc; stmt;} \
    else if(ti == typeid(unsigned int)){unsigned int* pdest = (unsigned int*)psrc; stmt;} \
    else if(ti == typeid(long)){long* pdest = (long*)psrc; stmt;} \
    else if(ti == typeid(unsigned long)){unsigned long* pdest = (unsigned long*)psrc; stmt;} \
    else if(ti == typeid(long long)){long long* pdest = (long long*)psrc; stmt;} \
    else if(ti == typeid(unsigned long long)){unsigned long long* pdest = (unsigned long long*)psrc; stmt;} \
    else if(ti == typeid(float)){float* pdest = (float*)psrc; stmt;} \
    else if(ti == typeid(double)){double* pdest = (double*)psrc; stmt;} \
    else if(ti == typeid(long double)){long double* pdest = (long double*)psrc; stmt;} \
    else if(ti == typeid(std::string)){std::string* pdest = (std::string*)psrc; stmt;} \
    else{defstmt;}
#define NEW_VOID(ti, pdest, defstmt) \
    if(ti == typeid(char)){pdest = new char;} \
    else if(ti == typeid(unsigned char)){pdest = new unsigned char;} \
    else if(ti == typeid(short)){pdest = new short;} \
    else if(ti == typeid(unsigned short)){pdest = new unsigned short;} \
    else if(ti == typeid(int)){pdest = new int;} \
    else if(ti == typeid(unsigned int)){pdest = new unsigned int;} \
    else if(ti == typeid(long)){pdest = new long;} \
    else if(ti == typeid(unsigned long)){pdest = new unsigned long;} \
    else if(ti == typeid(long long)){pdest = new long long;} \
    else if(ti == typeid(unsigned long long)){pdest = new unsigned long long;} \
    else if(ti == typeid(float)){pdest = new float;} \
    else if(ti == typeid(double)){pdest = new double;} \
    else if(ti == typeid(long double)){pdest = new long double;} \
    else if(ti == typeid(std::string)){pdest = new std::string;} \
    else{defstmt;}
#define NEW_VOID_ARRAY(ti, ptr, n, defstmt) \
    if(ti == typeid(char)){ptr = new char[n];} \
    else if(ti == typeid(unsigned char)){ptr = new unsigned char[n];} \
    else if(ti == typeid(short)){ptr = new short[n];} \
    else if(ti == typeid(unsigned short)){ptr = new unsigned short[n];} \
    else if(ti == typeid(int)){ptr = new int[n];} \
    else if(ti == typeid(unsigned int)){ptr = new unsigned int[n];} \
    else if(ti == typeid(long)){ptr = new long[n];} \
    else if(ti == typeid(unsigned long)){ptr = new unsigned long[n];} \
    else if(ti == typeid(long long)){ptr = new long long[n];} \
    else if(ti == typeid(unsigned long long)){ptr = new unsigned long long[n];} \
    else if(ti == typeid(float)){ptr = new float[n];} \
    else if(ti == typeid(double)){ptr = new double[n];} \
    else if(ti == typeid(long double)){ptr = new long double[n];} \
    else if(ti == typeid(std::string)){ptr = new std::string[n];} \
    else{defstmt;}
#define DELETE_VOID(ti, ptr, defstmt) \
    if(ti == typeid(char)){delete (char*)ptr;} \
    else if(ti == typeid(unsigned char)){delete (unsigned char*)ptr;} \
    else if(ti == typeid(short)){delete (short*)ptr;} \
    else if(ti == typeid(unsigned short)){delete (unsigned short*)ptr;} \
    else if(ti == typeid(int)){delete (int*)ptr;} \
    else if(ti == typeid(unsigned int)){delete (unsigned int*)ptr;} \
    else if(ti == typeid(long)){delete (long*)ptr;} \
    else if(ti == typeid(unsigned long)){delete (unsigned long*)ptr;} \
    else if(ti == typeid(long long)){delete (long long*)ptr;} \
    else if(ti == typeid(unsigned long long)){delete (unsigned long long*)ptr;} \
    else if(ti == typeid(float)){delete (float*)ptr;} \
    else if(ti == typeid(double)){delete (double*)ptr;} \
    else if(ti == typeid(long double)){delete (long double*)ptr;} \
    else if(ti == typeid(std::string)){delete (std::string*)ptr;} \
    else{defstmt;}
#define DELETE_VOID_ARRAY(ti, ptr, defstmt) \
    if(ti == typeid(char)){delete[] (char*)ptr;} \
    else if(ti == typeid(unsigned char)){delete[] (unsigned char*)ptr;} \
    else if(ti == typeid(short)){delete[] (short*)ptr;} \
    else if(ti == typeid(unsigned short)){delete[] (unsigned short*)ptr;} \
    else if(ti == typeid(int)){delete[] (int*)ptr;} \
    else if(ti == typeid(unsigned int)){delete[] (unsigned int*)ptr;} \
    else if(ti == typeid(long)){delete[] (long*)ptr;} \
    else if(ti == typeid(unsigned long)){delete[] (unsigned long*)ptr;} \
    else if(ti == typeid(long long)){delete[] (long long*)ptr;} \
    else if(ti == typeid(unsigned long long)){delete[] (unsigned long long*)ptr;} \
    else if(ti == typeid(float)){delete[] (float*)ptr;} \
    else if(ti == typeid(double)){delete[] (double*)ptr;} \
    else if(ti == typeid(long double)){delete[] (long double*)ptr;} \
    else if(ti == typeid(std::string)){delete[] (std::string*)ptr;} \
    else{defstmt;}
#endif
