#pragma once
#ifndef _PROJECT_1_H_
#define _PROJECT_1_H_

#include <sstream>
#include <vector>
#include <stdint.h>
#include <string>
#include <algorithm>

using namespace std;


class QInt {
private:
	int32_t data[4];
#pragma region Ham ho tro cho cac cong viec nhap du lieu
	//Ham ho tro cho cac cong viec nhap du lieu
	string StrBin(string a);		    // Chuyen chuoi so nguyen ve dang nhi phan, flag = true: so dua vao la so duong
	string divide2(string &a);		    // Chia 2 mot so, dau vao la chuoi so
	string mul2(string a);              // Nhan 2 mot so, dau vao la chuoi so
	string pow2(int n);                 // tinh 2^n, tra ve chuoi
	string add2Str(string a, string b); // cong hai so, dau vao la hai chuoi so
	string sub2Str(string a, string b); // tru hai so, a la so lon hon, dau vao la hai chuoi so
	void StrToQint(string s);           // Chuyen chuoi cac bit vao QInt

	//Ham ho tro: Ham thao tac tren bit
	bool getbit(int32_t x, int i);      // Lay bit thu i tu phai sang trong so nguyen 32 bit
	void printBit(int32_t x);           // In ra day bit cua so nguyen 32 bit
	void setBit(int32_t &x, int i);     // Set on 1 bit cua so nguyen 32 bit 
	void complement2();                 // chuyen ve dang bu 2 cho so am 
#pragma endregion

public:
	QInt();
	QInt(const string& s, int type); //type la he so cua chuoi nhap vao


	void ScanQInt();
	void PrintQInt();

	//============================================

	string  Decimal(); //Tra ve chuoi trong he 10 cua so QInt
	void printBitQint();    // Print day bit cua so Qint

	static vector<string> exponential; //mang chua cac gia tri luy thua cua 2^0 -> 2^127

	//convert method
	string BinToHex(string bit);
	string HexToBin(string s);
	string DecToHex(string x);
	string BinToDec(string bit);
	string DecToBin(string x);
	string HexToDec(string x);

	//arithmetic operators
	QInt operator+(const QInt& number);
	QInt operator-(const QInt& number);
	QInt operator*(const QInt& number);
	QInt operator/(const QInt& number);
	QInt & operator=(const QInt& number);

	//comparison operators
	bool operator>(const QInt &number);
	bool operator<(const QInt &number);
	bool operator>=(const QInt &number);
	bool operator<=(const QInt &number);
	bool operator==(const QInt &number);

	//bitwise operators
	QInt operator<<(const int n);
	QInt operator>>(const int n);
	QInt operator<<(const QInt &number);
	QInt operator>>(const QInt &number);
	QInt operator &(const int64_t n);
	QInt operator |(const int64_t n);
	QInt operator ^(const int64_t n);
	QInt operator &(const QInt &n);
	QInt operator |(const QInt &n);
	QInt operator ^(const QInt &n);
	QInt operator ~();

	//left rotate, right rotate
	QInt ror(const QInt &n);
	QInt rol(const QInt &n);
};

//Ham ho tro khac
string IntToStr(int64_t a);
int64_t StrToInt(string b);

#endif