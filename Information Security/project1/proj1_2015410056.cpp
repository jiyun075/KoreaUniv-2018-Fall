// IST_proj_1.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <Windows.h>
using namespace std;

const unsigned int bitsize = 65536;

/* ------------- ----------------- -------------- */
/* ------------- ----------------- -------------- */
vector <pair <unsigned int, unsigned int>> check1st16bits(void) {
	cout << "Function [Check 1st 16bits] Start" << endl;
	unsigned int input_1st[4] = { 24600, 2689, 11888, 63352 };
	unsigned int output_1st[4] = { 15045, 6444, 2861, 31653 };
	bool valid;
	vector <pair <unsigned int, unsigned int>> result;

	for (int k1 = 0; k1 < bitsize / 2; ++k1) {
		unsigned int key1 = 2 * k1 - 1;
		for (int k4 = 0; k4 < bitsize / 2; ++k4) {
			unsigned int key4 = 2 * k4 - 1;

			valid = true;
			for (int i = 0; i < 4; ++i) {
				unsigned int val1 = (input_1st[i] * key1) & (bitsize - 1);
				if (((val1 ^ key4) & (bitsize - 1)) != output_1st[i]) {
					valid = false;
					break;
				}
			}

			if (valid)
				result.push_back(make_pair(key1, key4));
		}
	}
	cout << "Function [Check 1st 16bits] Done" << endl;
	return result;
}
/* ------------- ----------------- -------------- */
vector <pair <unsigned int, unsigned int>> check4th16bits(void) {
	cout << "Function [Check 4th 16bits] Start" << endl;
	unsigned int input_4th[4] = { 33961, 55898, 17824, 19121 };
	unsigned int output_4th[4] = { 29262, 27117, 55543, 11766 };
	bool valid;
	vector <pair <unsigned int, unsigned int>> result;

	for (int k5 = 0; k5 < bitsize / 2; ++k5) {
		unsigned int key5 = 2 * k5 - 1;
		for (int k8 = 0; k8 < bitsize / 2; ++k8) {
			unsigned int key8 = 2 * k8 - 1;

			valid = true;
			for (int i = 0; i < 4; ++i) {
				unsigned int val4 = (input_4th[i] * key5) & (bitsize - 1);
				if (((val4 ^ key8) & (bitsize - 1)) != output_4th[i]) {
					valid = false;
					break;
				}
			}

			if (valid)
				result.push_back(make_pair(key5, key8));
		}
	}
	cout << "Function [Check 4th 16bits] Done" << endl;
	return result;
}
/* ------------- ----------------- -------------- */

/* ------------- ----------------- -------------- */
/* -------------- Find 8 subkeys ---------------- */
void Find8subkeys(void) {
	cout << "--*--*--*--*--*--*--*--*--*--*--*" << endl;
	cout << "Function [Find 8 Subkeys] Start" << endl;
	cout << "--*--*--*--*--*--*--*--*--*--*--*" << endl;

	const int plain[4][4] = {
		{ 24600, 58768, 64933, 33961 },
	{ 2689, 60657, 10270, 55898 },
	{ 11888, 37331, 2803, 17824 },
	{ 63352, 41760, 5207, 19121 }
	};
	const int cipher[4][4] = {
		{ 15045, 14285, 40145, 29262 },
	{ 6444, 38078, 50122, 27117 },
	{ 2861, 41780, 52591, 55543 },
	{ 31653, 22565, 21351, 11766 }
	};

	unsigned int subkeys[8 + 1];
	unsigned int val[10 + 1];

	vector <pair <unsigned int, unsigned int>> key_1_4 = check1st16bits();
	vector <pair <unsigned int, unsigned int>> key_5_8 = check4th16bits();
	vector <pair <unsigned int, unsigned int>>::iterator it_1_4;
	vector <pair <unsigned int, unsigned int>>::iterator it_5_8;
	cout << endl << "--*--*--*--*--*--*--*--*--*--*--*" << endl << endl;

	for (it_1_4 = key_1_4.begin(); it_1_4 != key_1_4.end(); ++it_1_4) {
		subkeys[1] = (*it_1_4).first;
		subkeys[4] = (*it_1_4).second;

		for (it_5_8 = key_5_8.begin(); it_5_8 != key_5_8.end(); ++it_5_8) {
			subkeys[5] = (*it_5_8).first;
			subkeys[8] = (*it_5_8).second;

			for (int k2 = 0; k2 < bitsize; ++k2) {
				if ((k2 & 5) == 5)
					continue;
				for (int k6 = 0; k6 < bitsize; ++k6) {
					if ((k2 & 5) == 0) { //case1
						if ((k6 & 5) != 5)
							continue;
					}
					else if ((k2 & 5) == 4) { //case2, case3, case4
						if ((k6 & 5) == 1)
							continue;
					}
					else { //if ((k2 & 5) == 1)
						if ((k6 & 5) == 1) //case5
							continue;
						if ((k6 & 5) == 5) //case6
							continue;
					}

					subkeys[2] = k2;
					subkeys[3] = (k2 >> 2);
					subkeys[6] = k6;
					subkeys[7] = (k6 >> 2);

					/*--------- subkeys 8개 모두 생성 ---------*/
					/*
					key1, key4, key5, key8은 valid한 것
					key2, key3, key6, key7을 아래에서 test
					*/
					bool valid = true;
					for (int i = 0; i < 4; ++i) { //4 pair of plain/cipher text
						val[1] = (plain[i][0] * subkeys[1]) & (bitsize - 1); //#1 mul_mod
						val[2] = (plain[i][3] * subkeys[5]) & (bitsize - 1); //#2 mul_mod
						val[3] = (plain[i][1] + subkeys[2]) & (bitsize - 1); //#3 add_mod
						val[4] = (plain[i][2] + subkeys[6]) & (bitsize - 1); //#4 add_mod

						val[5] = (val[1] ^ val[4]); //#5 XOR
						val[6] = (val[2] ^ val[3]); //#6 XOR
						val[7] = (val[5] * subkeys[3]) & (bitsize - 1); //#7 mul_mod;
						val[8] = (val[6] + val[7]) & (bitsize - 1); //#8 add_mod
						val[9] = (val[8] * subkeys[7]) & (bitsize - 1); //#9 mul_mod
						val[10] = (val[7] + val[9]) & (bitsize - 1); //#10 add_mod

						if ((val[3] ^ val[10]) != cipher[i][1]) {
							valid = false;
							break;
						}
						if ((val[4] ^ val[9]) != cipher[i][2]) {
							valid = false;
							break;
						}
					}
					//Print valid subkeys
					if (valid) {
						cout << "--*--*--* SUBKEYS *--*--*--*" << endl;
						for (int i = 1; i < 9; ++i)
							cout << "subkey #" << i << " -> " << subkeys[i] << endl;
						cout << endl;
					}
				}
			}
		}
	}
	cout << "--*--* FINISH PROGRAM *--*--*" << endl;
}

/* ------------- ----------------- -------------- */


/*---------------- EO TESTING ---------------- */
void EvenOdd_test(void) {
	cout << "--*--*--*--*--*--*--*--*--*--*--*" << endl;
	cout << "Subkeys Even/Odd Testing Start" << endl;
	cout << "--*--*--*--*--*--*--*--*--*--*--*" << endl;

	vector <vector <int>> input = {
		{ 0,0,1,1 },{ 1,1,0,0 },{ 0,1,1,0 }
	};
	vector <vector <int>> output = {
		{ 1,1,1,0 },{ 0,0,0,1 },{ 1,0,1,1 }
	};

	cout << "** Even: 0 / Odd: 1 **" << endl;

	vector <int> val(10 + 1);
	vector <int> subkeys(8 + 1);

	subkeys[1] = 1; //subkeys1 odd
	subkeys[4] = 1; //subkeys4 odd
	subkeys[5] = 1; //subkeys5 odd
	subkeys[8] = 1; //subkeys8 odd

	cout << "| key2\t| key3\t| key6\t| key7\t|" << endl;
	cout << "----------------------------------" << endl;

	for (int k2 = 0; k2 < 2; ++k2) {
		subkeys[2] = k2;
		for (int k3 = 0; k3 < 2; ++k3) {
			subkeys[3] = k3;
			for (int k6 = 0; k6 < 2; ++k6) {
				subkeys[6] = k6;
				for (int k7 = 0; k7 < 2; ++k7) {
					subkeys[7] = k7;
					//가능한 모든 subkeys 조합
					bool result = true;
					for (int i = 0; i < 3; ++i) { //input output pair 4개 test

						val[1] = (input[i][0] * subkeys[1]) % 2; //#1 mul_mod
																 // XOR & CHECK 1st part
						if ((val[1] ^ subkeys[4]) != output[i][0]) {
							result = false;
							break;
						}

						val[2] = (input[i][3] * subkeys[5]) % 2; //#2 mul_mod
																 // XOR & CHECK 4th part
						if ((val[2] ^ subkeys[8]) != output[i][3]) {
							result = false;
							break;
						}

						val[3] = (input[i][1] + subkeys[2]) % 2; //#3 add_mod
						val[4] = (input[i][2] + subkeys[6]) % 2; //#4 add_mod
						val[5] = (val[1] ^ val[4]); //#5 XOR
						val[6] = (val[2] ^ val[3]); //#6 XOR
						val[7] = (val[5] * subkeys[3]) % 2; //#7 mul_mod
						val[8] = (val[6] + val[7]) % 2; //#8 add_mod
						val[9] = (val[8] * subkeys[7]) % 2; //#9 mul_mod
						val[10] = (val[7] + val[9]) % 2; //#10 add_mod

														 //#13 XOR & CHECK 2nd part
						if ((val[3] ^ val[10]) != output[i][1]) {
							result = false;
							break;
						}
						//#14 XOR & CHECK 3rd part
						if ((val[4] ^ val[9]) != output[i][2]) {
							result = false;
							break;
						}
					}
					if (result) {
						cout << "| " << subkeys[2] << "\t| ";
						cout << subkeys[3] << "\t| ";
						cout << subkeys[6] << "\t| ";
						cout << subkeys[7] << "\t|" << endl;
					}
				}
			}
		}
	}
	cout << endl << endl;
}

/*---------------- ----------------- ---------------- */


int main()
{
	EvenOdd_test();
	Find8subkeys();

	system("pause");
	return 0;
}
