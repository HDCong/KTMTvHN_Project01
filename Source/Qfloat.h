#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
using namespace std;
class Qfloat
{
private:
	// Describe 
	// Bit 0...111 : Fraction
	// Bit 112...126: Exponent
	// Bit 127: Sign

	unsigned int Data[4];
private:
	// Private Methods

	vector<string> Tokenizer(const string &p);

	bool getAtBit(int pos) const;
	void setAtBit(int pos, bool value);

	vector<bool> fractionToBin(const string &str);

	vector<bool> decimalToBin(const string &str);

	int lastDifferentZero(const string &str, bool fromRight);

	string BinToDec(vector<bool> bit);

	void moveBitFromFractionToDecimal(vector<bool> &fract, vector<bool> &dec, int size);

	void moveBitFromDecimalToFraction(vector<bool> &fract, vector<bool> &dec, int size);

	bool *decToBin(string str);

	string divide(const string &str, int time);

	string multiply(const string &str, int time);

	void storageData(bool *data);

	string add(string p1, string p2);
	bool isZero();
	bool isInfinity();
	bool isNaN();
	bool isDenormalized();

	Qfloat initPositiveInfNumber();
	Qfloat initNegativeInfNumber();
	Qfloat initNaNnumber();

	int compareBit(const Qfloat &another, int from, int to) const;
	bool checkSignificandZero();
	void rightShiftSignificand(bool val);
	bool leftShiftSignificand();
	void incrementExponent();
	void decrementExponent();
	void roundResult();
	vector<bool> multiply(const Qfloat &p, Qfloat &res);
	vector<bool> add(vector<bool> p1, vector<bool> p2); // 2 vector has same size
	vector<bool> divide(const Qfloat &p);
	vector<bool > subtract(vector<bool> p1, vector<bool> p2);
	bool compare(vector<bool> p1, vector<bool> p2);
public:
	////// Constructor and Destructor Area
	Qfloat();
	Qfloat(const Qfloat &p);
	~Qfloat();
public:
	// Public Methods
	void scanQfloat();
	void printQfloat();
	Qfloat binToDec(bool *bit);
	bool *decToBin();
	// interface 
	Qfloat dataFromBinToQfloat(string str);
	//CString decToBitData(string);
	Qfloat dataFromIEEE(string str);
	string printIEEE();
	// Operator Area (Bonus Area)

	Qfloat & operator = (const Qfloat &p);
	Qfloat operator + (const Qfloat &p);
	Qfloat operator - (const Qfloat &p);
	Qfloat operator *(const Qfloat &p);
	Qfloat operator /(const Qfloat &p);
};


