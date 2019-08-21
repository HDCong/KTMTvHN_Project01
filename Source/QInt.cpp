#include "QInt.h"
#include <iostream>

using namespace std;

//constructor method
QInt::QInt() {
	for (int i = 0; i < 4; i++) this->data[i] = 0;

	if (exponential[1] != "2")
		for (int i = 1; i < 128; i++) {
			exponential[i] = mul2(exponential[i - 1]);
		}
}

QInt::QInt(const string& s, int type) {
	for (int i = 0; i < 4; i++) this->data[i] = 0;
	if (type == 2) {
		StrToQint(s);
	}
	else if (type == 10) {
		string str = s;
		bool flag = (s[0] == '-');
		if (flag) str = s.substr(1);
		StrToQint(StrBin(str));
		if (flag) this->complement2();
	}
	else if (type == 16) {
		StrToQint(HexToBin(s));
		if (s[0] == '-') this->complement2();
	}
}


//======================================================

string QInt::StrBin(string a) {
	string res = "";
	string tmp = a;

	while (tmp.compare("0") != 0) {
		int32_t n = tmp.length();
		res += ((tmp[n - 1] - '0') % 2 ? "1" : "0");
		tmp = divide2(tmp);
	}
	reverse(res.begin(), res.end());
	return res;
}

string QInt::divide2(string &a) {
	int32_t n = a.length();
	int32_t step = 10;
	string res = "";
	while (n > 0) {
		int64_t tmp;
		if (n < step) step = n;
		//cout << a << endl;
		tmp = StrToInt(a.substr(0, step));
		if (tmp == 1) break;

		a.erase(0, step);

		res += IntToStr(tmp / 2);

		n -= step;
		if (tmp % 2 == 1) {
			n += 1;
			a = "1" + a;
		}
		else { // ....00001.....
			int j = 0;
			if (n > 1)
				while (a[j] == '0') {
					j++;
					res += "0";
				}
			if (a[j] == '1') res += "0";
		}



	}
	return ((res == "") ? "0" : res);
}

string QInt::mul2(string a) {
	int n = a.length();
	int64_t tmp = 0;
	string res = "";
	for (int i = n - 1; i >= 0; i--) {
		int64_t tmp1 = StrToInt(a.substr(i, 1)) * 2 + tmp;
		res = IntToStr(tmp1 % 10) + res;
		tmp = tmp1 / 10;
	}
	if (tmp != 0) res = IntToStr(tmp) + res;
	return res;
}

string QInt::pow2(int n) {

	return exponential[n];
}

string QInt::add2Str(string a, string b) {
	string res = "";
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	int nmax = max(int(a.length()), int(b.length()));

	int64_t tmp = 0;

	for (int i = 0; i < nmax; i++) {
		int64_t tmp1;
		if (i >= int(a.length())) {
			tmp1 = StrToInt(b.substr(i, 1)) + tmp;
		}
		else if (i >= int(b.length())) {
			tmp1 = StrToInt(a.substr(i, 1)) + tmp;
		}
		else {
			tmp1 = StrToInt(a.substr(i, 1)) + StrToInt(b.substr(i, 1)) + tmp;
		}
		res += IntToStr(tmp1 % 10);
		tmp = tmp1 / 10;
	}

	while (tmp) {
		res += IntToStr(tmp % 10);
		tmp = tmp / 10;
	}
	reverse(res.begin(), res.end());
	return res;
}

string QInt::sub2Str(string a, string b) {
	string res = "";
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());

	int temp = 0;
	int n = a.length();
	for (int i = 0; i < n; i++) {
		int tmp = 0;

		int64_t aN, bN;
		aN = StrToInt(a.substr(i, 1));

		if (i < int(b.length())) bN = StrToInt(b.substr(i, 1));
		else
			bN = 0;
		if (aN < bN + temp) {
			aN += 10;
			tmp = 1;
		}

		res += IntToStr(int64_t(aN - bN - temp));
		temp = tmp;
	}
	reverse(res.begin(), res.end());
	int i = 0;
	while (res[i] == '0')
		i++;
	return res.substr(i);
}

void QInt::StrToQint(string s) {
	int j = 127;
	int len = s.length();
	for (int i = len - 1; i >= 0; i--) {
		int a = j % 32;
		int b = j / 32;
		if (s[i] == '1')
			setBit(this->data[b], 31 - a);
		j--;
	}
}


void QInt::complement2() {
	(*this) = (this -> operator~()) + QInt(string("1"), 10);
}


//work on bits
bool QInt::getbit(int32_t x, int i) {
	return ((x >> i) & 1);
}

void QInt::setBit(int32_t &x, int i) {
	x = x | (1 << i);
}

void QInt::printBit(int32_t x) {
	for (int i = 31; i >= 0; i--) {
		cout << getbit(x, i);
	}
}

//scan and print
void QInt::ScanQInt() {
	string s;
	cout << "Enter the number: ";
	cin >> s;

	bool flag = (s[0] == '-');
	if (flag) s = s.substr(1);

	StrToQint(StrBin(s));

	if (flag) this->complement2();
}

void QInt::PrintQInt() {
	cout << this->Decimal() << endl;
}

//======================================================

QInt QInt::operator~() {
	QInt res(*this);

	for (int i = 0; i < 4; i++)
		res.data[i] = ~this->data[i];

	return res;
}

QInt& QInt::operator=(const QInt&other) {
	for (int i = 0; i < 4; i++) {
		this->data[i] = other.data[i];
	}
	return *this;
}

QInt QInt::operator+(const QInt& other) {
	int j = 127;
	uint8_t temp = 0; // Bien nho

	QInt res;
	while (j >= 0) {
		int a = j % 32;
		int b = j / 32;

		uint8_t bitSum = getbit(this->data[b], 31 - a) + getbit(other.data[b], 31 - a) + temp;
		temp = bitSum / 2;

		if (bitSum % 2 == 1) res.setBit(res.data[b], 31 - a);

		j--;
	}
	return res;
}

QInt QInt::operator-(const QInt& other) {
	QInt another(other);
	another.complement2();

	return *this + another;
}

QInt QInt::operator*(const QInt &other) {
	QInt ans(*this);
	QInt another;
	bool q1 = 0;
	int k = 128;

	//algorithm 
	while (k > 0) {
		if ((ans.data[3] & 1) && q1 == 0) {
			another = another - other;
		}
		else if (((ans.data[3] & 1) == 0) && q1 == 1) {
			another = another + other;
		}

		// shift right
		q1 = (ans.data[3] & 1);
		ans = ans >> 1;
		ans.data[0] = (ans.data[0] & (~(1 << 31)));
		if (another.data[3] & 1)
			ans.setBit(ans.data[0], 31);
		another = another >> 1;
		k--;
	}

	return ans;
}

QInt QInt::operator/(const QInt &other) {
	if (!other.data[0] && !other.data[1] && !other.data[2] && !other.data[3]) {
		cout << "Devided by zero!" << endl;
	}
	QInt another;
	QInt ans(*this);
	QInt othersShadow(other);

	bool flag = false; // flag de kiem tra co phai so am khong

	//kiem tra 2 so co dong thoi la so am hay khong
	if (((othersShadow.data[0] >> 31) & 1) && ((ans.data[0] >> 31) & 1)) {
		ans.complement2();
		othersShadow.complement2();
	}
	//kiem tra 1 so co phai la so am hay ko
	else if ((ans.data[0] >> 31) & 1) {
		flag = true;
		ans.complement2();
	}
	else if ((othersShadow.data[0] >> 31) & 1) {
		flag = true;
		othersShadow.complement2();
	}

	int k = 128;

	// thuat toan chia theo slide
	while (k > 0) {
		//shift left
		another.data[0] = ((another.data[0] << 1) | getbit(another.data[1], 31));
		another.data[1] = ((another.data[1] << 1) | getbit(another.data[2], 31));
		another.data[2] = ((another.data[2] << 1) | getbit(another.data[3], 31));
		another.data[3] = ((another.data[3] << 1) | getbit(ans.data[0], 31));
		//shift left
		ans.data[0] = ((ans.data[0] << 1) | getbit(ans.data[1], 31));
		ans.data[1] = ((ans.data[1] << 1) | getbit(ans.data[2], 31));
		ans.data[2] = ((ans.data[2] << 1) | getbit(ans.data[3], 31));
		ans.data[3] = (ans.data[3] << 1);

		QInt temp = another - othersShadow;

		if (getbit(temp.data[0], 31)) {
			if (getbit(ans.data[3], 0))
				ans.data[3] = ans.data[3] - 1;
		}
		else {
			another = temp;
			ans.data[3] = ans.data[3] | 1;
		}

		k--;
	}

	if (flag) ans.complement2(); // tra ve lai so am

	return ans;
}

//======================================================

//comparison operators
bool QInt::operator>(const QInt &number) {
	for (int i = 0; i < 4; i++) {
		for (int j = 31; j >= 0; j--) {
			if (getbit(data[i], j) > getbit(number.data[i], j)) return true;
			if (getbit(data[i], j) < getbit(number.data[i], j)) return false;
		}
	}
	return false;
}

bool QInt::operator<(const QInt &number) {
	for (int i = 0; i < 4; i++) {
		for (int j = 31; j >= 0; j--) {
			if (getbit(data[i], j) < getbit(number.data[i], j)) return true;
			if (getbit(data[i], j) > getbit(number.data[i], j)) return false;
		}
	}
	return false;
}

bool QInt::operator>=(const QInt &number) {
	for (int i = 0; i < 4; i++) {
		for (int j = 31; j >= 0; j--) {
			if (getbit(data[i], j) > getbit(number.data[i], j)) return true;
			if (getbit(data[i], j) < getbit(number.data[i], j)) return false;
		}
	}
	return true;
}

bool QInt::operator<=(const QInt &number) {
	for (int i = 0; i < 4; i++) {
		for (int j = 31; j >= 0; j--) {
			if (getbit(data[i], j) < getbit(number.data[i], j)) return true;
			if (getbit(data[i], j) > getbit(number.data[i], j)) return false;
		}
	}
	return true;
}

bool QInt::operator==(const QInt &number) {
	return (data[0] == number.data[0] && data[1] == number.data[1] && data[2] == number.data[2] && data[3] == number.data[3]);
}

//======================================================

//bitwise operators

QInt QInt::operator<<(const QInt &number) {
	QInt another(*this);
	int n = number.data[3];
	if (n > 128) return QInt();
	return another << n;
}

QInt QInt::operator>>(const QInt &number) {
	QInt another(*this);
	int n = number.data[3];
	if (n > 128) return QInt();
	return another >> n;
}

QInt QInt::operator<<(const int n) {
	QInt another(*this);
	for (int i = 0; i < n; i++) {
		another.data[0] = ((another.data[0] << 1) | getbit(another.data[1], 31));
		another.data[1] = ((another.data[1] << 1) | getbit(another.data[2], 31));
		another.data[2] = ((another.data[2] << 1) | getbit(another.data[3], 31));
		another.data[3] = (another.data[3] << 1);
	}
	return another;
}

QInt QInt::operator>>(const int n) {
	QInt another(*this);

	for (int i = 0; i < n; i++) {
		another.data[3] = ((another.data[3] >> 1) & (~(1 << 31))) | ((another.data[2] & 1) << 31);
		another.data[2] = ((another.data[2] >> 1) & (~(1 << 31))) | ((another.data[1] & 1) << 31);
		another.data[1] = ((another.data[1] >> 1) & (~(1 << 31))) | ((another.data[0] & 1) << 31);
		another.data[0] = another.data[0] >> 1;
	}

	return another;
}

QInt QInt::operator &(const int64_t n) {
	QInt ans(IntToStr(n), 10);
	return (*this & ans);
}

QInt QInt::operator |(const int64_t n) {
	QInt ans(IntToStr(n), 10);
	return (*this | ans);
}

QInt QInt::operator ^(const int64_t n) {
	QInt ans(IntToStr(n), 10);
	return (*this ^ ans);
}

QInt QInt::operator &(const QInt &n) {
	QInt ans(*this);
	for (int i = 0; i < 4; i++) {
		ans.data[i] = this->data[i] & n.data[i];
	}
	return ans;
}

QInt QInt::operator |(const QInt &n) {
	QInt ans(*this);
	for (int i = 0; i < 4; i++) {
		ans.data[i] = this->data[i] | n.data[i];
	}
	return ans;
}

QInt QInt::operator ^(const QInt &n) {
	QInt ans(*this);
	for (int i = 0; i < 4; i++) {
		ans.data[i] = this->data[i] ^ n.data[i];
	}
	return ans;
}

//======================================================

//left rotate, right rotate
QInt QInt::ror(const QInt &number) {
	int n = number.data[3];
	return ((*this >> n) | (*this << (128 - n)));
}

QInt QInt::rol(const QInt &number) {
	int n = number.data[3];
	return ((*this << n) | (*this >> (128 - n)));
}

//======================================================

//convert method

string QInt::BinToHex(string bit) {
	QInt number(bit, 2);
	string ans = "";

	string temp = "";
	for (int i = 0; i < 4; i++) {
		for (int j = 31; j >= 0; j--) {
			temp += ((getbit(number.data[i], j) == 1) ? "1" : "0");
		}
	}

	for (int i = 31; i >= 0; i--) {
		int answer = 0, mu = 0;

		for (int j = i * 4 + 3; j >= i * 4; j--) {
			answer += ((temp[j] == '1') ? pow(2, mu) : 0);
			mu++;
		}

		switch (answer) {
		case 0:  ans = "0" + ans; break;
		case 1:  ans = "1" + ans; break;
		case 2:  ans = "2" + ans; break;
		case 3:  ans = "3" + ans; break;
		case 4:  ans = "4" + ans; break;
		case 5:  ans = "5" + ans; break;
		case 6:  ans = "6" + ans; break;
		case 7:  ans = "7" + ans; break;
		case 8:  ans = "8" + ans; break;
		case 9:  ans = "9" + ans; break;
		case 10: ans = "A" + ans; break;
		case 11: ans = "B" + ans; break;
		case 12: ans = "C" + ans; break;
		case 13: ans = "D" + ans; break;
		case 14: ans = "E" + ans; break;
		case 15: ans = "F" + ans; break;
		default: break;
		}
	}
	int i = 0;
	while (ans[i] == '0') i++;
	ans = ans.substr(i);
	return ans;
}

string QInt::HexToBin(string s) {
	string ans = "";
	bool flag = (s[0] == '-');
	for (int i = 0; i < s.length(); i++) {
		switch (s[i]) {
		case '0': ans += "0000"; break;
		case '1': ans += "0001"; break;
		case '2': ans += "0010"; break;
		case '3': ans += "0011"; break;
		case '4': ans += "0100"; break;
		case '5': ans += "0101"; break;
		case '6': ans += "0110"; break;
		case '7': ans += "0111"; break;
		case '8': ans += "1000"; break;
		case '9': ans += "1001"; break;
		case 'A': ans += "1010"; break;
		case 'B': ans += "1011"; break;
		case 'C': ans += "1100"; break;
		case 'D': ans += "1101"; break;
		case 'E': ans += "1110"; break;
		case 'F': ans += "1111"; break;
		default: break;
		}
	}
	/*if (flag) {
		QInt temp(ans, 2);
		temp.complement2();
		string pos = "";

		for (int i = 0; i < 4; i++) {
			for (int j = 31; j >= 0; j--) {
				if (getbit(temp.data[i], j)) pos += "1";
				else pos += "0";
			}
		}
		return pos;
	}	*/
	return ans;
}

string QInt::DecToHex(string x) {
	return BinToHex(DecToBin(x));
}

string QInt::BinToDec(string bit) {
	QInt ans(bit, 2);
	return ans.Decimal();
}

string QInt::DecToBin(string x) {
	/*string res = "";
	string tmp = a;

	while (tmp.compare("0") != 0) {
		int32_t n = tmp.length();
		res += ((tmp[n - 1] - '0') % 2 ? "1" : "0");
		tmp = divide2(tmp);
	}
	reverse(res.begin(), res.end());
	return res;*/
	string ans = "";
	QInt temp(x, 10);
	for (int i = 0; i < 4; i++) {
		for (int j = 31; j >= 0; j--) {
			ans += (getbit(temp.data[i], j) ? "1" : "0");
		}
	}
	int i = 0;
	while (ans[i] == '0') i++;
	ans = ans.substr(i);
	return ans;
	//return StrBin(x);
}

string QInt::HexToDec(string x) {
	return BinToDec(HexToBin(x));
}
//======================================================

string QInt::Decimal() {
	string res = "";
	int j = 127;
	int i = 0;
	while (j) {
		int a = j % 32;
		int b = j / 32;
		if (getbit(this->data[b], 31 - a)) res = add2Str(res, pow2(i));
		i++;
		j--;
	}
	if (getbit(this->data[0], 31)) res = '-' + sub2Str(pow2(127), res);
	return ((res.length() == 0) ? "0" : res);
}

void QInt::printBitQint() {
	for (int i = 0; i < 4; i++) {
		printBit(this->data[i]);
	}
	cout << endl;
}

//======================================================

//other method
int64_t StrToInt(string b) {
	stringstream s;
	s << b;
	int64_t a;
	s >> a;
	return a;
}

string IntToStr(int64_t a) {
	stringstream s;
	s << a;
	string res;
	s >> res;
	return res;
}

