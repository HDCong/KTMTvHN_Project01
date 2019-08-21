#include "QInt.h"
#include "Qfloat.h"
#include <iostream>
#include <conio.h>
#include <fstream>
#include <cstring>

vector<string> QInt::exponential = vector<string>(128, "1");

using namespace std;

int main(int argc, char *argv[]) {
	if (argc == 4) {
		string fileIn = argv[1];
		string fileOut = argv[2];
		string type = argv[3];

		if (type == "1") {
			ifstream input;
			ofstream output;
			input.open(fileIn);
			output.open(fileOut);
			while (!input.eof()) {
				string line;
				getline(input, line);
				vector <string> tokens;
				stringstream check1(line);
				string intermediate;
				while (getline(check1, intermediate, ' ')) {
					tokens.push_back(intermediate);
				}
				if (tokens.size() == 4) {
					QInt number_1(tokens[1], stoi(tokens[0]));
					QInt number_2(tokens[3], stoi(tokens[0]));
					QInt result;
					bool flag_isCompare = false, flag_result = false;
					int t = atoi(tokens[0].c_str());
					string oper = tokens[2];

					if (oper.compare("+") == 0)		   result = number_1 + number_2;
					else if (oper.compare("-") == 0)   result = number_1 - number_2;
					else if (oper.compare("*") == 0)   result = number_1 * number_2;
					else if (oper.compare("/") == 0)   result = number_1 / number_2;
					else if (oper.compare("<<") == 0) {
						QInt number_3(tokens[3], 10);
						result = number_1 << number_3;
					}
					else if (oper.compare(">>") == 0) {
						QInt number_3(tokens[3], 10);
						result = number_1 >> number_3;
					}
					else if (oper.compare("&") == 0)   result = number_1 & number_2;
					else if (oper.compare("|") == 0)   result = number_1 | number_2;
					else if (oper.compare("^") == 0)   result = number_1 ^ number_2;
					else if (oper.compare("ror") == 0) result = number_1.ror(number_2);
					else if (oper.compare("rol") == 0) result = number_1.rol(number_2);
					else if (oper.compare(">") == 0) {
						flag_isCompare = true;
						flag_result = (number_1 > number_2);
					}
					else if (oper.compare("<") == 0) {
						flag_isCompare = true;
						flag_result = (number_1 < number_2);
					}
					else if (oper.compare(">=") == 0) {
						flag_isCompare = true;
						flag_result = (number_1 >= number_2);
					}
					else if (oper.compare("<=") == 0) {
						flag_isCompare = true;
						flag_result = (number_1 <= number_2);
					}
					else if (oper.compare("==") == 0) {
						flag_isCompare = true;
						flag_result = (number_1 == number_2);
					}

					if (flag_isCompare) {
						output << ((flag_result) ? "true" : "false") << endl;
					}
					else {
						if (stoi(tokens[0]) == 2) {
							output << result.DecToBin(result.Decimal()) << endl;
						}
						else if (stoi(tokens[0]) == 10) {
							output << result.Decimal() << endl;
						}
						else if (stoi(tokens[0]) == 16) {
							output << result.DecToHex(result.Decimal()) << endl;
						}
					}
				}
				else if (tokens.size() == 3) {
					int num1 = stoi(tokens[0]);
					int num2 = stoi(tokens[1]);
					string inp = tokens[2];
					QInt ans;
					if (num1 == 2 && num2 == 10) {
						output << ans.BinToDec(inp) << endl;
					}
					else if (num1 == 2 && num2 == 16) {
						output << ans.BinToHex(inp) << endl;
					}
					else if (num1 == 10 && num2 == 2) {
						output << ans.DecToBin(inp) << endl;
					}
					else if (num1 == 10 && num2 == 16) {
						output << ans.DecToHex(inp) << endl;
					}
					else if (num1 == 16 && num2 == 2) {
						output << ans.HexToBin(inp) << endl;
					}
					else if (num1 == 16 && num2 == 10) {
						output << ans.HexToDec(inp) << endl;
					}
				}
			}

			output.close();
			input.close();
		}
		else if (type == "2") {
			ifstream input;
			ofstream output;
			input.open(fileIn);
			output.open(fileOut);
			while (!input.eof()) {
				string line;
				getline(input, line);
				vector <string> tokens;
				stringstream check1(line);
				string intermediate;
				while (getline(check1, intermediate, ' ')) {
					tokens.push_back(intermediate);
				}
				if (tokens.size() == 4) {
					Qfloat number_1;
					Qfloat number_2;
					bool isBinary = true;
					if (tokens[0] == "2") {

						number_1.dataFromBinToQfloat(tokens[1]);
						number_2.dataFromBinToQfloat(tokens[3]);
					}
					else {
						isBinary = false;
						number_1.dataFromIEEE(tokens[1]);
						number_2.dataFromIEEE(tokens[3]);

					}
					Qfloat result;
					string oper = tokens[2];

					if (oper.compare("+") == 0)		   result = number_1 + number_2;
					else if (oper.compare("-") == 0)   result = number_1 - number_2;
					else if (oper.compare("*") == 0)   result = number_1 * number_2;
					else if (oper.compare("/") == 0)   result = number_1 / number_2;
					if (isBinary) {
						bool *dat = result.decToBin();
						for (int i = 127; i >= 0; i--)
							output << dat[i];
						output << endl;
					}
					else
					{
						output << result.printIEEE() << endl;
					}
				}
				else if (tokens.size() == 3) {
					string num1 = tokens[0];
					string num2 = tokens[1];
					string inp = tokens[2];
					Qfloat ans;
					if (tokens[0] == "2") {

						ans.dataFromBinToQfloat(inp);
					}
					else {
						ans.dataFromIEEE(inp);

					}

					if (num1 == "f" && num2 == "2") { // IEEE to binary qfloat
						bool *dat = ans.decToBin();
						for (int i = 127; i >= 0; i--)
							output << dat[i];
						output << endl;
					}
					else if (num1 == "2" && num2 == "f") { // value of qfloat
						output << ans.printIEEE() << endl;
					}
				}
			}

			output.close();
			input.close();
		}
	}

	return 0;
}

