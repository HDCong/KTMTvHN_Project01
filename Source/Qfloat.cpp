#include "Qfloat.h"
#include <assert.h>

//  Ar[0]   Ar[1]   Ar[2]   Ar[3] 
/// 0...31|32...63|64...95|96...127

//  0....Fraction......111|112..Exponent...126|127: Sign

bool Qfloat::isDenormalized()
{
	for (int i = 112; i < 127; i++)
		if (getAtBit(i)) {
			return false;
		}
	return true;
}


vector<string> Qfloat::Tokenizer(const string & p)
{

	vector<string > res;
	int n = p.find_first_of('.');
	res.push_back(p.substr(0, n));
	res.push_back(p.substr(n + 1, p.length() - n - 1));

	return res;

}

bool Qfloat::getAtBit(int pos) const
{

	assert(pos >= 0 && pos < 128);
	int t = pos / 32;
	int n = pos % 32;
	return ((Data[t] >> n) & 1);
}

void Qfloat::setAtBit(int pos, bool value)
{
	assert(pos >= 0 && pos < 128);
	int t = pos / 32;
	int n = pos % 32;
	if (value)
		Data[t] = Data[t] | (1 << n);
	else Data[t] = Data[t] & (~(1 << n));
}

int Qfloat::lastDifferentZero(const string &str, bool fromRight)
{
	if (fromRight) {
		for (int i = str.length() - 1; i >= 0; i--)
			if (str[i] != '0') return i;
		return -1;
	}
	else {
		for (int i = 0; i < str.length(); i++)
			if (str[i] != '0') return i;
		return -1;
	}
}

vector<bool> Qfloat::fractionToBin(const string & str)
{

	vector<bool> res;
	int n = str.length();
	int curSize = 0;
	int remember = 0;
	string temp = str;
	while (curSize < 112)
	{

		n = lastDifferentZero(temp, 1);
		if (n >= 0) {
			remember = 0;
			for (int i = n; i >= 0; i--)
			{
				int t = (temp[i] - '0') * 2;

				if (remember) {
					t++;
					remember = 0;
				}

				if (t >= 10) {
					t -= 10;
					remember = 1;
				}

				temp[i] = t + '0';
			}
			res.push_back(remember);
			curSize++;
		}
		else break;

	}
	if (res.size() == 0) res.push_back(0);

	return res;
}

vector<bool>  Qfloat::decimalToBin(const string & str)
{

	vector<bool> res;
	int remember = 0;
	int n, length = str.length();
	string temp = str;
	while (1)
	{
		n = lastDifferentZero(temp, 0);
		if (n >= 0) {
			res.insert(res.begin(), (temp[length - 1] - '0') % 2);
			remember = 0;
			for (int i = n; i < temp.size(); i++)
			{
				int k = temp[i] - '0';
				if (remember)
				{
					k += 10;
					remember = 0;
				}
				temp[i] = (k / 2 + '0');
				remember = k % 2;
			}

		}
		else break;

	}
	if (res.size() == 0) res.push_back(0);
	return res;
}

string Qfloat::BinToDec(vector<bool> Bin)
{
	string res = "0";
	string cur = "1";
	while (!Bin.empty())
	{
		if (Bin.back()) {
			res = add(res, cur);
		}
		cur = multiply(cur, 1);
		Bin.pop_back();
	}
	return res;
}

void Qfloat::moveBitFromFractionToDecimal(vector<bool>& fract, vector<bool>& dec, int size)
{
	while (size > 0)
	{
		bool cur = fract.front();
		dec.push_back(cur);
		fract.erase(fract.begin());
		size--;
	}
}

void Qfloat::moveBitFromDecimalToFraction(vector<bool>& fract, vector<bool>& dec, int size)
{
	while (size > 0)
	{
		bool cur = dec.back();
		fract.insert(fract.begin(), cur);
		//	fract.pop_back();
		dec.pop_back();
		size--;
	}
}

bool * Qfloat::decToBin(string  str)
{
	bool *res = new bool[128]{ 0 };
	int local;
	if (str[0] == '-') {
		res[127] = 1;
		str.erase(str.begin());
	}

	int bias = pow(2, 14) - 1; // Mean 0 in bias 

	vector<string> t = Tokenizer(str);

	vector<bool> dec = decimalToBin(t[0]);
	vector<bool> fract = fractionToBin(t[1]);

	int sizeMoved = dec.size() - 1;
	if (sizeMoved > 0)
	{
		moveBitFromDecimalToFraction(fract, dec, sizeMoved);

		bias += sizeMoved;
	}
	else if (t[0] == "0")
	{
		int i;
		for (i = 0; i < fract.size(); i++)
			if (fract[i]) break;
		++i;
		if (i > fract.size()) return res;
		moveBitFromFractionToDecimal(fract, dec, i);

		bias -= i; //999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999
	}

	if (bias > pow(2, 15)) return nullptr;

	vector<bool> exponent = decimalToBin(to_string(bias));
	local = 112;
	while (!exponent.empty())
	{
		bool val = exponent.back();
		exponent.pop_back();
		res[local++] = val;
	}
	// Write Fraction to bool array 
	local = 111;
	while (!fract.empty() && local >= 0)
	{
		bool val = fract.front();
		fract.erase(fract.begin());
		res[local--] = val;
	}
	return res;
}

string Qfloat::divide(const string & str, int time)
{
	string curResul = str;
	if (curResul.find('.') == string::npos) curResul.push_back('.');
	int residual;
	while (time)
	{
		int n = curResul.length();
		string temp;
		residual = 0;
		int i = lastDifferentZero(curResul, false);
		for (i; i < n || residual != 0; i++)
		{

			if (curResul[i] == '.') {
				temp.push_back('.');
				continue;
			}

			int getCur = curResul[i] - '0';
			if (i >= n) getCur = 0;
			if (residual) {
				getCur += 10;
				residual = 0;
			}
			temp.push_back((getCur / 2) + '0');
			residual = getCur % 2;
		}
		if (temp[0] == '.') temp.insert(temp.begin(), '0');
		curResul = temp;
		time--;
	}
	return curResul;
}

string Qfloat::multiply(const string & str, int time)
{
	string curResul = str;
	int residual;

	while (time)
	{
		int n = curResul.length();
		string temp;
		residual = 0;
		for (int i = n - 1; i >= 0 || residual != 0; i--)
		{
			if (i >= 0 && curResul[i] == '.') {
				temp.insert(temp.begin(), '.');
				continue;
			}

			int getCur;
			if (i < 0) getCur = 0;
			else getCur = curResul[i] - '0';

			getCur *= 2;
			if (residual) {
				getCur += 1;
				residual = 0;
			}
			if (getCur > 9)
			{
				getCur -= 10;
				residual = 1;
			}
			temp.insert(temp.begin(), getCur + '0');
		}
		curResul = temp;
		time--;
	}
	return curResul;
}
void Qfloat::storageData(bool * data)
{
	for (int i = 127; i >= 0; i--) {
		this->setAtBit(i, data[i]);
	}
}

string Qfloat::add(string p1, string p2)
{
	string res;
	int remember = 0;
	while (!p1.empty() && !p2.empty()) {
		int t = p1.back() + p2.back() - 2 * '0';
		p1.pop_back();
		p2.pop_back();
		if (remember) {
			t++;
			remember = 0;
		}
		if (t > 9) {
			t -= 10;
			remember = 1;
		}
		res.insert(res.begin(), t + '0');
	}
	while (!p1.empty())
	{
		int t = p1.back() - '0';
		p1.pop_back();
		if (remember) {
			t++;
			remember = 0;
		}
		if (t > 9) {
			t -= 10;
			remember = 1;
		}
		res.insert(res.begin(), t + '0');
	}
	while (!p2.empty())
	{
		int t = p2.back() - '0';
		p2.pop_back();

		if (remember) {
			t++;
			remember = 0;
		}
		if (t > 9) {
			t -= 10;
			remember = 1;
		}
		res.insert(res.begin(), t + '0');
	}
	if (remember)
		res.insert(res.begin(), '1');
	return res;
}

bool Qfloat::isZero()
{
	for (int i = 0; i < 127; i++)
		if (getAtBit(i)) {
			return false;
		}
	return true;
}

bool Qfloat::isInfinity()
{
	for (int i = 112; i < 127; i++)
		if (!getAtBit(i))
			return false;

	for (int i = 111; i >= 0; i--)
		if (getAtBit(i))
			return false;
	return true;
}

bool Qfloat::isNaN()
{
	for (int i = 112; i < 127; i++)
		if (!getAtBit(i)) {
			return false;
		}
	for (int i = 111; i >= 0; i--)
		if (getAtBit(i)) return true;
	return false;
}




int Qfloat::compareBit(const Qfloat & another, int from, int to) const
{
	for (int i = to; i >= from; i--)
	{
		if (getAtBit(i) < another.getAtBit(i)) return -1;
		else if (getAtBit(i) > another.getAtBit(i)) return 1;
	}
	return 0;
}

bool Qfloat::checkSignificandZero()
{
	for (int i = 111; i >= 0; i--)
		if (getAtBit(i)) return false;
	return true;
}


void Qfloat::rightShiftSignificand(bool val)
{
	bool *data = new bool[112];
	for (int i = 1; i < 112; i++) data[i - 1] = getAtBit(i);
	data[111] = val;
	for (int i = 0; i < 112; i++)
		setAtBit(i, data[i]);



}

bool Qfloat::leftShiftSignificand()
{
	bool *data = new bool[112];
	for (int i = 1; i < 112; i++) data[i] = getAtBit(i - 1);
	data[0] = 0;
	bool res = getAtBit(111);
	for (int i = 0; i < 112; i++)
		setAtBit(i, data[i]);
	return res;
}

void Qfloat::incrementExponent()
{
	int temp = 1, sum;
	for (int i = 112; i < 128; i++) {
		sum = getAtBit(i) + temp;
		temp = sum / 2;
		setAtBit(i, sum % 2);
	}
}

void Qfloat::decrementExponent()
{
	int subtr, re = 1;

	for (int i = 112; i < 127; i++) {
		subtr = getAtBit(i);
		if (re == 1) {
			subtr -= 1;
			re = 0;
		}
		if (subtr < 0) {
			subtr += 2;
			re = 1;
		}
		setAtBit(i, subtr % 2);
	}
}

void Qfloat::roundResult()
{

}

vector<bool> Qfloat::multiply(const Qfloat & p, Qfloat &res)
{
	Qfloat X = *this, Y = p;
	vector<bool> xData = { 1 }, yData = { 1 };
	int total;

	int i = 0;
	while (!X.getAtBit(i)) i++;
	for (int j = 111; j >= i; j--) xData.push_back(X.getAtBit(j));

	total = xData.size() - 1;

	i = 0;
	while (!Y.getAtBit(i)) i++;

	for (int j = 111; j >= i; j--) yData.push_back(Y.getAtBit(j));
	total += (yData.size() - 1);
	vector <bool > curRes = xData, temp;


	i = 1;
	yData.pop_back();

	while (!yData.empty())
	{
		if (yData.back())
		{
			temp = xData;
			for (int j = 0; j < i; j++)
			{
				curRes.insert(curRes.begin(), 0);
				temp.push_back(0);
			}
			curRes = add(curRes, temp);
		}
		i++;
		yData.pop_back();
	}
	vector<bool > vtRes;
	int n = curRes.size() - total;
	for (int i = 0; i < n; i++) {
		vtRes.push_back(curRes.front());
		curRes.erase(curRes.begin());
	}
	for (int i = 111; i >= 0 && !curRes.empty(); i--) {
		res.setAtBit(i, curRes.front());
		curRes.erase(curRes.begin());
	}
	return vtRes;
}

vector<bool> Qfloat::add(vector<bool> p1, vector<bool> p2)
{
	vector<bool > res;
	int remember = 0;
	while (!p2.empty())
	{
		int sum = p1.back() + p2.back() + remember;
		remember = sum / 2;
		res.insert(res.begin(), sum % 2);
		p1.pop_back();
		p2.pop_back();
	}
	if (remember) 	res.insert(res.begin(), 1);
	return res;
}

vector<bool> Qfloat::divide(const Qfloat & p)
{
	Qfloat X = *this, Y = p;

	vector<bool> xData = { 1 }, yData = { 1 };

	int i = 0;

	while (!X.getAtBit(i)) i++;

	for (int j = 111; j >= i; j--) xData.push_back(X.getAtBit(j));

	i = 0;

	while (!Y.getAtBit(i)) i++;

	for (int j = 111; j >= i; j--) yData.push_back(Y.getAtBit(j));
	int minimumBit = yData.size();

	vector<bool > vtRes, temp;

	for (int i = 0; i < minimumBit; i++)
	{
		if (!xData.empty()) {
			temp.push_back(xData.front());
			xData.erase(xData.begin());
		}
		else temp.push_back(0);

	}
	bool haveBitOn = false;
	int curCount = 0;
	while ((temp.size() && curCount < 113) || !xData.empty())
	{
		if (compare(temp, yData)) {
			if (!haveBitOn) haveBitOn = true;
			vtRes.push_back(1);
			temp = subtract(temp, yData);
		}
		else {
			vtRes.push_back(0);
		}
		if (!xData.empty())
		{
			temp.push_back(xData.front());
			xData.erase(xData.begin());
		}
		else temp.push_back(0);
		while (!temp.empty() && !temp.front()) temp.erase(temp.begin());
		if (haveBitOn) curCount++;

	}
	return vtRes;
	//return vtRes;
}

vector<bool> Qfloat::subtract(vector<bool> p1, vector<bool> p2) // p1 always longer or equal to p2
{
	vector<bool> res;
	int re = 0, subtr;

	while (!p2.empty())
	{
		subtr = p1.back() - p2.back();
		if (re) {
			subtr -= 1;
			re = 0;
		}
		if (subtr < 0) {
			subtr += 2;
			re = 1;
		}
		res.insert(res.begin(), subtr % 2);
		p1.pop_back();
		p2.pop_back();
	}
	while (res.size() && !res.front()) res.erase(res.begin());

	return res;
}

bool Qfloat::compare(vector<bool> p1, vector<bool> p2)
{
	if (p1.size() < p2.size()) return false;
	else if (p1.size() > p2.size()) return true;
	else {
		for (int i = 0; i < p1.size(); i++)
		{
			if (p1[i] > p2[i]) return true;
			if (p1[i] < p2[i]) return false;
		}
		return true;
	}
}

Qfloat Qfloat::initPositiveInfNumber()
{
	Qfloat res;
	for (int i = 112; i < 127; i++)
		res.setAtBit(i, 1);
	return res;
}

Qfloat Qfloat::initNegativeInfNumber()
{
	Qfloat res;
	for (int i = 112; i < 128; i++)
		res.setAtBit(i, 1);
	return res;
}

Qfloat Qfloat::initNaNnumber()
{
	Qfloat res;
	for (int i = 111; i < 127; i++)
		res.setAtBit(i, 1);
	return res;
}



/// Construction and Destruction
Qfloat::Qfloat()
{
	Data[0] = Data[1] = Data[2] = Data[3] = 0;
}

Qfloat::Qfloat(const Qfloat & p)
{
	for (int i = 0; i < 4; i++)
		Data[i] = p.Data[i];
}

Qfloat::~Qfloat()
{

}

//Public Methods	
void Qfloat::scanQfloat()
{
	cout << "Input number: ";
	string number;
	cin >> number;
	if (number.find('.') == string::npos) // Standardized string 
		number.append(".0");

	while (number[0] == '0' && number[1] != '.') number.erase(number.begin());
	while (number.back() == '0' &&number[number.length() - 2] != '.') number.pop_back();
	bool *data = decToBin(number);
	storageData(data);
}

//CString Qfloat::decToBitData(string number)
//{
//	if (number.find('.') == string::npos) // Standardized string 
//		number.append(".0");
//
//	while (number[0] == '0' && number[1] != '.') number.erase(number.begin());
//	while (number.back() == '0' &&number[number.length() - 2] != '.') number.pop_back();
//	bool *data = decToBin(number);
//	CString res;
//	int j = 0;
//	while (!data[j] && j < 111) j++;
//	for (int i = 127; i >= j; i--)
//	{
//		res += (char)(data[i] + '0');
//	}
//	return res;
//}

Qfloat Qfloat::dataFromIEEE(string number)
{
	if (number.find('.') == string::npos) // Standardized string 
		number.append(".0");

	while (number[0] == '0' && number[1] != '.') number.erase(number.begin());
	while (number.back() == '0' &&number[number.length() - 2] != '.') number.pop_back();
	bool *data = decToBin(number);
	storageData(data);
	return *this;
}

string Qfloat::printIEEE()
{
	string res;
	if (isZero()) { res = "0"; return res; }
	if (isInfinity()) {
		if (getAtBit(127)) res += "-";
		res += "Infinity\n";
		return res;
	}
	if (isNaN()) { res += "Error : NaN"; return res; }
	if (isDenormalized())
	{
		res += "Denormalized number: ";
		string total;
		string temp = "1";
		string temp2;
		int i = 0;
		while (!getAtBit(i) && i < 111) i++;
		for (int j = 111; j >= i; j--)
		{
			temp = divide(temp, 1);
			total.push_back('0');
			if (getAtBit(j))
			{
				temp2 = temp.substr(2);
				total = add(total, temp2);
			}
		}
		total = ("0." + total);
		if (getAtBit(127)) total.insert(total.begin(), '-');

		res += total;
		res += ("* 2^(-16382)");
		return res;
	}
	bool *data = new bool[128];
	for (int i = 127; i >= 0; i--) {
		data[i] = getAtBit(i);
	}
	vector<bool> getExponent;
	for (int i = 126; i > 111; i--) {
		getExponent.push_back(data[i]);
	}

	int exponentValue = atoi(BinToDec(getExponent).c_str()) - pow(2, 14) + 1;

	vector<bool> dec = { 1 }, fraction;
	res = "";
	for (int i = 111; i >= 0; i--) {
		fraction.push_back(data[i]);
		if (data[i])
			res.push_back('1');
		else res.push_back('0');
	}

	int size = lastDifferentZero(res, true) + 1;

	moveBitFromFractionToDecimal(fraction, dec, size);

	exponentValue -= size;

	res = BinToDec(dec);

	if (exponentValue > 0) res = multiply(res, exponentValue);
	else if (exponentValue < 0) res = divide(res, -exponentValue);
	while (res[0] == '0' && res[1] != '.') res.erase(res.begin());
	if (data[127]) res.insert(res.begin(), '-');

	return res;
}

void Qfloat::printQfloat()
{
	if (isZero()) { cout << "Value is: 0\n"; return; }
	if (isInfinity()) {
		if (getAtBit(127)) cout << "-";
		cout << "Infinity\n";
		return;
	}
	if (isNaN()) { cout << "Error : NaN\n"; return; }
	bool *data = new bool[128];
	for (int i = 127; i >= 0; i--) {
		data[i] = getAtBit(i);
	}
	vector<bool> getExponent;
	for (int i = 126; i > 111; i--) {
		getExponent.push_back(data[i]);
	}

	int exponentValue = atoi(BinToDec(getExponent).c_str()) - pow(2, 14) + 1;

	vector<bool> dec = { 1 }, fraction;
	string temp = "";
	for (int i = 111; i >= 0; i--) {
		fraction.push_back(data[i]);
		if (data[i])
			temp.push_back('1');
		else temp.push_back('0');
	}

	int size = lastDifferentZero(temp, true) + 1;

	moveBitFromFractionToDecimal(fraction, dec, size);

	exponentValue -= size;

	temp = BinToDec(dec);

	if (exponentValue > 0) temp = multiply(temp, exponentValue);
	else if (exponentValue < 0) temp = divide(temp, -exponentValue);
	while (temp[0] == '0' && temp[1] != '.') temp.erase(temp.begin());
	if (data[127]) temp.insert(temp.begin(), '-');

	cout << "Value is: " << temp << endl;
}

Qfloat Qfloat::binToDec(bool * bit)
{
	Qfloat res;
	for (int i = 0; i < 128; i++)
		res.setAtBit(i, bit[i]);
	return res;
}

bool * Qfloat::decToBin()
{
	bool *res = new bool[128]{ 0 };
	for (int i = 0; i < 128; i++)
		res[i] = getAtBit(i);
	return res;
}

Qfloat Qfloat::dataFromBinToQfloat(string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		setAtBit(127 - i, (str[i] - '0'));
	}
	return (*this);
}

Qfloat & Qfloat::operator=(const Qfloat & rhs)
{
	for (int i = 0; i < 4; i++)
	{
		Data[i] = rhs.Data[i];
	}
	return *this;
}

Qfloat Qfloat::operator+(const Qfloat & p)
{
	Qfloat res;
	Qfloat X = *this, Y = p;

	if (X.isZero())
		return Y;
	else
	{
		if (Y.isZero())
			return X;
		else {
			// Check exponent equal
			int val = X.compareBit(Y, 112, 126);
			// If X's Exponent is not equal
			if (val < 0 || X.compareBit(Y, 111, 0) < 0) { // abs(X) always greater than abs(Y)
				Qfloat temp = X;
				X = Y;
				Y = temp;
			}
			bool bitX = 1, bitY = 1;
			int bitTemp;

			while (X.compareBit(Y, 112, 126))
			{
				Y.rightShiftSignificand(bitY);
				bitY = 0;
				Y.incrementExponent();
				if (Y.checkSignificandZero())
					return X;
			}// Exponent equal
			// Set Exponent for result

			for (int i = 112; i < 128; i++)
				res.setAtBit(i, X.getAtBit(i));
			// Add signed significand
			int remember = 0;
			bool isAdd;
			if (X.getAtBit(127) == Y.getAtBit(127)) {
				isAdd = true;
				for (int i = 0; i < 112; i++)
				{
					int sum = X.getAtBit(i) + Y.getAtBit(i) + remember;
					remember = sum / 2;
					res.setAtBit(i, sum % 2);
				}
				bitTemp = bitX + bitY + remember;

			}
			else {
				isAdd = false;
				for (int i = 0; i < 112; i++) {
					int sub = X.getAtBit(i) - Y.getAtBit(i);
					if (remember) {
						sub--;
						remember = 0;
					}
					if (sub < 0) {
						sub += 2;
						remember = 1;
					}
					res.setAtBit(i, sub % 2);
				}
				bitTemp = bitX - bitY - remember;
			}

			// Check significand =0
			if (!bitTemp&& res.checkSignificandZero())
			{
				return Qfloat();
			}
			// Check significand overflow
			if (bitTemp > 1)
			{
				bool isFull = true; // Check if increment cause exponent overflow

				for (int i = 112; i < 127; i++)
					if (!res.getAtBit(i))
					{
						isFull = false;
						break;
					}
				if (isFull) {
					//	cout << "OVERFLOW!\n";
					if (res.getAtBit(127)) return initNegativeInfNumber();
					else return initPositiveInfNumber();
				}
				res.rightShiftSignificand(bitTemp % 2);
				res.incrementExponent();
			}
			else {
				if (!bitTemp) // Not normalized
				{
					do {

						bool isFull = true; // Check if decremnt cause exponent underflow

						for (int i = 112; i < 127; i++)
							if (res.getAtBit(i))
							{
								isFull = false;
								break;
							}
						if (isFull) {
							//		cout << "UNDERFLOW!! ";
							return Qfloat();
						}

						res.decrementExponent();

					} while (!res.leftShiftSignificand());
				}
				else res.roundResult();
			}
		}
	}
	return res;
}

Qfloat Qfloat::operator-(const Qfloat & p)
{
	Qfloat X = *this, Y = p;
	Y.setAtBit(127, !Y.getAtBit(127));
	return X + Y;
}

Qfloat Qfloat::operator*(const Qfloat & p)
{
	Qfloat res, X = *this, Y = p;
	if (X.isZero() || Y.isZero()) return res;
	else
	{
		// Add exponent
		vector<bool> exponentX, exponentY;
		for (int i = 112; i < 127; i++)
		{
			exponentX.insert(exponentX.begin(), X.getAtBit(i));
			exponentY.insert(exponentY.begin(), Y.getAtBit(i));
		}
		int totalBiased = atoi(BinToDec(exponentX).c_str()) + atoi(BinToDec(exponentY).c_str()) - (pow(2, 14) - 1);
		if (totalBiased > pow(2, 15) - 1) {
			//	cout << "OVERFLOW!!";
			if (X.getAtBit(127) == Y.getAtBit(127)) return initPositiveInfNumber();
			else return initNegativeInfNumber();
		}
		if (totalBiased < -pow(2, 14)) {
			//	cout << "UNDERFLOW";
			return Qfloat();
		}
		vector<bool> exponent = decimalToBin(to_string(totalBiased));
		int local = 112;

		while (!exponent.empty())
		{
			bool val = exponent.back();
			exponent.pop_back();
			res.setAtBit(local++, val);
		}
		vector<bool > remainder = X.multiply(Y, res);
		if (remainder.size() > 1)
		{
			res.rightShiftSignificand(remainder.back());
			remainder.pop_back();
			bool flag = true;
			for (int i = 112; i < 127; i++)
				if (res.getAtBit(i) == 0) { flag = false; break; }
			if (flag)
			{
				//	cout << "OVERFLOW\n";
				if (res.getAtBit(127)) return initNegativeInfNumber();
				else return initPositiveInfNumber();
			}
			res.incrementExponent();
		}
	}
	return res;

}

Qfloat Qfloat::operator/(const Qfloat & p)
{
	Qfloat res, X = *this, Y = p;
	if (X.isZero()) return res;
	else
	{
		if (Y.isZero()) {
			//	cout << "ERROR\nDIVIDE BY ZERO\n";
			//	assert(!Y.isZero());
			return initNaNnumber();
		}
		// Add exponent
		vector<bool> exponentX, exponentY;
		for (int i = 112; i < 127; i++)
		{
			exponentX.insert(exponentX.begin(), X.getAtBit(i));
			exponentY.insert(exponentY.begin(), Y.getAtBit(i));
		}
		int totalBiased = atoi(BinToDec(exponentX).c_str()) - atoi(BinToDec(exponentY).c_str()) + (pow(2, 14) - 1);
		if (totalBiased > pow(2, 15) - 1) {
			//	cout << "OVERFLOW!!";
			if (X.getAtBit(127) == Y.getAtBit(127)) return initPositiveInfNumber();
			else return initNegativeInfNumber();
		}
		if (totalBiased < -pow(2, 14)) {
			//	cout << "UNDERFLOW";
			return Qfloat();
		}

		vector<bool> exponent = decimalToBin(to_string(totalBiased));
		int local = 112;

		while (!exponent.empty())
		{
			bool val = exponent.back();
			exponent.pop_back();
			res.setAtBit(local++, val);
		}
		vector <bool > significand = X.divide(Y);
		int i;
		for (i = 0; i < significand.size(); i++) {
			if (significand[i]) break;
		}
		totalBiased -= i;
		if (totalBiased < -pow(2, 14)) {
			//	cout << "UNDERFLOW";
			return Qfloat();
		}
		else {
			for (int j = 0; j < i; j++) {
				res.decrementExponent();
				significand.erase(significand.begin());
			}
			significand.erase(significand.begin());
			for (int j = 111; j >= 0 && significand.size(); j--) {
				res.setAtBit(j, significand.front());
				significand.erase(significand.begin());
			}
		}

	}
	return res;
}

