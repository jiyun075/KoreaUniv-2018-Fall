// proj2_2015410056.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <vector> //
#include <iostream>
#include <Windows.h>
#include <bitset>
#include <sstream> //
#include <set> //
using namespace std;

const int N = 128;
const unsigned long long Q = 15383399235709406497;

int find_xA_pr(int q, int g, int h) {
	int val = g * g;
	for (int i = 2; i < q - 1; ++i) {
		val = val % q;
		if (val == h)
			return i;
		val *= g;
	}
	return 1;
}

int find_msb(const bitset<N> &a) { //return msb
	for (int i = N - 1; i >= 0; --i) {
		if (a.test(i))
			return i;
	}
}

bitset<N> bit_subtract(bitset<N> n1, bitset<N> n2) { //n1- n2
	bitset<N> brw;
	while (n2 != 0) {
		brw = (~n1)&n2;
		n1 = n1 ^ n2;
		n2 = brw << 1;
	}
	return n1;
}

bool bit_compare(bitset<N> n1, bitset<N> n2, int msb) { //true if n1 >= n2
	for (int i = msb; i >= 0; --i) {
		if (n1[i] ^ n2[i])
			return n1[i];
	}
	return 1;
}

/////////////////////////////////////////////////////////////

unsigned long long exp_mod_q(unsigned long long base, int exp) {
	bitset<N> modulus_q = 15383399235709406497;
	bitset<N> base_bit = base;
	bitset<N> test_bit = 1;

	bitset<N> result_bit;
	bitset<N> add_operand;

	int divisor_size = find_msb(modulus_q);
	int bit;

	bitset<N> dividend;

	vector<int> bit_position;

	for (int i = 0; i <= find_msb(base_bit); ++i) {
		if (base_bit.test(i))
			bit_position.push_back(i);
	}

	vector<int>::iterator iter = bit_position.begin();
	int j;

	for (int i = 0; i < exp; ++i) {
		// multiplication --> bit addition
		for (iter = bit_position.begin(); iter != bit_position.end(); ++iter) {
			j = *iter; // 값이 1인 bit 위치에 대해
			add_operand = test_bit << j; //shift 후 addition

										 // addition
			bool carry = 0;
			bool temp = 0;
			for (int k = j; k < N; ++k) {
				temp = result_bit[k];
				result_bit[k] = result_bit[k] ^ add_operand[k] ^ carry;
				carry = (temp & add_operand[k]) | (temp & carry) | (carry & add_operand[k]);
			}
		}

		// mod operation
		dividend = result_bit;
		bit = find_msb(dividend);
		while (bit >= divisor_size) {
			if (bit_compare(dividend, (modulus_q << (bit - divisor_size)), bit))
				dividend = bit_subtract(dividend, modulus_q << (bit - divisor_size));
			else if (bit - divisor_size >= 1)
				dividend = bit_subtract(dividend, modulus_q << (bit - divisor_size - 1));
			else
				break;
			// bit = dividend의 msb update
			for (int f = bit; f >= 0; --f) {
				if (dividend.test(f)) {
					bit = f;
					break;
				}
			}
		}

		test_bit = dividend;
		result_bit.reset();
	}
	return test_bit.to_ullong();
}

unsigned long long mul_mod_q(unsigned long long a, unsigned long long b) {
	bitset<N> modulus_q = 15383399235709406497;
	bitset<N> a_bit = a;
	bitset<N> b_bit = b;

	bitset<N> result_bit;
	bitset<N> add_operand;

	int divisor_size = find_msb(modulus_q);
	int bit;

	bitset<N> dividend;

	vector<int> bit_position; //multiply operand 3^223 mod q 6504488417728282620에서 1이 나오는 위치를 저장

	for (int i = 0; i <= find_msb(b_bit); ++i) {
		if (b_bit.test(i))
			bit_position.push_back(i);
	}

	vector<int>::iterator iter = bit_position.begin();
	int j;

	// multiplication --> bit addition
	for (iter = bit_position.begin(); iter != bit_position.end(); ++iter) {
		j = *iter; // 값이 1인 bit 위치에 대해
		add_operand = a_bit << j; //shift 후 addition

								  // addition
		bool carry = 0;
		bool temp = 0;
		for (int k = j; k < N; ++k) {
			temp = result_bit[k];
			result_bit[k] = result_bit[k] ^ add_operand[k] ^ carry;
			carry = (temp & add_operand[k]) | (temp & carry) | (carry & add_operand[k]);
		}
	}

	// mod operation
	dividend = result_bit;
	bit = find_msb(dividend);
	while (bit >= divisor_size) {
		if (bit_compare(dividend, (modulus_q << (bit - divisor_size)), bit))
			dividend = bit_subtract(dividend, modulus_q << (bit - divisor_size));
		else if (bit - divisor_size >= 1)
			dividend = bit_subtract(dividend, modulus_q << (bit - divisor_size - 1));
		else
			break;
		// bit = dividend의 msb update
		for (int f = bit; f >= 0; --f) {
			if (dividend.test(f)) {
				bit = f;
				break;
			}
		}
	}

	return dividend.to_ullong();
}

unsigned long long extended_Euclidean(unsigned long long K, unsigned long long q) {
	signed long long A1 = 1;
	signed long long A2 = 0;
	unsigned long long A3 = q;
	signed long long B1 = 0;
	signed long long B2 = 1;
	unsigned long long B3 = K;
	signed long long Q;

	signed long long temp1;
	signed long long temp2;
	unsigned long long temp3;

	signed long long inverse_K;
	unsigned long long mult_inverse_K;
	int iter = 1;
	while (true) {
		if (B3 == 1) {
			inverse_K = B2;
			if (inverse_K < 0)
				mult_inverse_K = q - (-inverse_K);
			else
				mult_inverse_K = inverse_K;
			break;
		}
		if (B3 == 0) {
			cout << "there is no inverse" << endl;
			mult_inverse_K = 0;
			break;
		}

		Q = A3 / B3;

		temp1 = A1 - Q * B1;
		temp2 = A2 - Q * B2;
		temp3 = A3 - Q * B3;

		A1 = B1; A2 = B2; A3 = B3;
		B1 = temp1; B2 = temp2; B3 = temp3;

		++iter;
	}
	return mult_inverse_K;
}

/////////////////////////////////////////////////////////////////////////////////////////
// exp와 base가 주어졌을때 mod q 계산을 빨리하는 함수
// 10단위로 제곱 값을 저장한 후 자릿수만큼 곱해서 빠르게 결과값을 낸다.
unsigned long long faster_exp_mod_q(unsigned long long base, unsigned long long exp) {
	// convert xA to string
	string exp_str;
	stringstream str_strm;

	str_strm << exp;
	str_strm >> exp_str;

	// make expo vector
	vector<unsigned long long> exp_dec;
	vector<int> coef_v;

	exp_dec.push_back(base);

	int coef;

	for (int i = exp_str.size() - 1; i >= 0; --i) {
		coef = exp_str[i] - '0';
		coef_v.push_back(coef);
	}

	int size = coef_v.size();

	// c1^(10^k) 계산
	unsigned long long res_val;
	for (int i = 1; i < size; ++i) {
		res_val = exp_mod_q(exp_dec.back(), 10);
		exp_dec.push_back(res_val);
	}


	unsigned long long ten_exp = 1;
	unsigned long long fin_res = 1;
	for (int i = 0; i < size; ++i) {
		res_val = exp_mod_q(exp_dec[i], coef_v[i]);
		ten_exp *= 10;
		fin_res = mul_mod_q(fin_res, res_val);
	}

	return fin_res;
}


unsigned long long recovering_K(unsigned long long c1, unsigned long long xA) {

	cout << "************** Recovering key K *****************" << endl << endl;

	// convert xA to string
	string xA_str;
	stringstream str_strm;

	str_strm << xA;
	str_strm >> xA_str;

	// make expo vector
	vector<unsigned long long> exp_xA;
	vector<int> xA_v;

	//cout << "*** base = " << c1 << endl; 
	//cout << "*** exp = " << xA << endl << endl; 


	exp_xA.push_back(c1);

	int coef;

	for (int i = xA_str.size() - 1; i >= 0; --i) {
		coef = xA_str[i] - '0';
		xA_v.push_back(coef);
	}

	int size = xA_v.size();

	// c1^(10^k) 계산
	unsigned long long res_val;
	unsigned long long temp_val = 1; //
									 //cout << "base ^ " << temp_val << " = " << exp_xA[0] << endl; 
	for (int i = 1; i < size; ++i) {
		temp_val *= 10;
		res_val = exp_mod_q(exp_xA.back(), 10);
		exp_xA.push_back(res_val);
		//cout << "base ^ " << temp_val << " = " << exp_xA[i] << endl; 
	}
	//cout << endl;

	unsigned long long ten_exp = 1;
	unsigned long long fin_res = 1;
	for (int i = 0; i < size; ++i) {
		res_val = exp_mod_q(exp_xA[i], xA_v[i]);
		ten_exp *= 10;
		fin_res = mul_mod_q(fin_res, res_val);
		//cout << "base ^ (" << xA_v[i] << " * 10^" << i << ") = " << res_val << endl; //

	}
	cout << "******************* ANSWER **********************" << endl;
	cout << "key K is " << fin_res << endl;
	cout << "*************************************************" << endl;

	return fin_res;
}


bool extended_prt(unsigned long long test) { //extended_primitive_root test
	cout << "-----" << test << "-----" << endl << endl;
	unsigned long long exp_arr[9] = { // R(3)을 모든 prime factor로 나눈 것
		7691699617854703248, // /1
		3845849808927351624, // /2^1
		1922924904463675812, // /2^2
		961462452231837906, // /2^3
		480731226115918953, // /2^4
		5127799745236468832, // /3
		2197628462244200928, // /7
		54745193009642016, // /281
		188832 }; // 81466061026253

	long long factor_arr[9] = {
		1,2,2,2,2,3,7,281,81466061026253
	};
	unsigned long long test_exp;
	for (int i = 0; i < 9; ++i) {
		test_exp = faster_exp_mod_q(test, exp_arr[i]);
		if (test_exp == 1)
			cout << factor_arr[i] << " ";
		//if (test_exp == 1)
		//return false;
	}
	cout << endl << "**************************************" << endl;

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////

void inverse_test(unsigned long long base, unsigned long long exp_res) {
	unsigned long long base_inverse = extended_Euclidean(base, Q);
	unsigned long long minus_one = mul_mod_q(exp_res, base_inverse);
	extended_prt(minus_one);
}

int main()
{
	unsigned long long xA = 202141387003;
	unsigned long long c1 = 2695597157275121;

	unsigned long long c2 = 151188505555671261;
	unsigned long long K_inverse;
	unsigned long long M;

	cout << "< Problem #1 >" << endl << endl;
	cout << "xA prime is " << find_xA_pr(223, 3, 118) << endl;
	//result = find_xA_pr(q, g, h);
	unsigned long long K = recovering_K(c1, xA);
	K_inverse = extended_Euclidean(K, Q);
	M = mul_mod_q(c2, K_inverse);
	cout << endl;

	cout << "******************* ANSWER **********************" << endl;
	cout << "message M is " << M << endl;
	cout << "*************************************************" << endl << endl;

	////////////////////////////////////////////////////////////////////////////
	unsigned long long gxA = 3255928389273017819;
	cout << "< Problem #2 >" << endl << endl;

	cout << "step #1" << endl;
	cout << "-----------------------------------------" << endl << endl;
	extended_prt(gxA);
	cout << "-----------------------------------------" << endl << endl;
	cout << "step #2" << endl;
	cout << "-----------------------------------------" << endl << endl;
	unsigned long long test = 9;
	for (int i = 1; i <= 2; ++i) {
		cout << "*** xA - " << 2 * i << endl;
		inverse_test(test, gxA);
		test = mul_mod_q(test, 9);
	}
	cout << "-----------------------------------------" << endl << endl;
	cout << "step #3" << endl;
	cout << "-----------------------------------------" << endl << endl;
	unsigned long long new_base = mul_mod_q(extended_Euclidean(81, Q), gxA); //xA-4
	test = 10031401664608257468; //3^(281*2)
	for (int i = 1; i <= 13; ++i) {
		cout << "*** xA - 4 - " << 281 * 2 * i << endl;
		inverse_test(test, new_base);
		test = mul_mod_q(test, 10031401664608257468);
	}
	cout << "-----------------------------------------" << endl << endl;
	cout << "step #4" << endl;
	cout << "-----------------------------------------" << endl << endl;
	new_base = mul_mod_q(extended_Euclidean(7493601519836186804, Q), gxA); //xA-4-7306
	test = 7793423908512060785; // 3^(3*7*281)
	for (int i = 1; i <= 3; ++i) {
		cout << "*** xA - 4 - 7306 - " << 281 * 3 * 7 * i << endl;
		inverse_test(test, new_base);
		test = mul_mod_q(test, 7793423908512060785);
	}
	cout << "-----------------------------------------" << endl << endl;
	cout << endl;

	xA = 721530968069;
	c1 = 8312893525486221525;

	c2 = 7825868133432246571;

	K = recovering_K(c1, xA);
	K_inverse = extended_Euclidean(K, Q);
	unsigned long long M2 = mul_mod_q(c2, K_inverse);
	cout << endl;

	cout << "******************* ANSWER **********************" << endl;
	cout << "message M2 is " << M2 << endl;
	cout << "*************************************************" << endl << endl;

	////////////////////////////////////////////////////////////////////////////
	cout << "< Problem #3 - 1 >" << endl << endl;

	c1 = 4232920939787140673;
	c2 = 12594363607212086362;
	K = recovering_K(c1, xA);
	K_inverse = extended_Euclidean(K, Q);
	unsigned long long M3 = mul_mod_q(c2, K_inverse);
	cout << endl;

	cout << "******************* ANSWER **********************" << endl;
	cout << "message M3 is " << M3 << endl;
	cout << "*************************************************" << endl << endl;
	cout << "final answer --> M2 * M3 is " << mul_mod_q(M2, M3) << endl;
	cout << "*************************************************" << endl << endl;

	cout << "< Problem #3 - 2 >" << endl << endl;

	c1 = mul_mod_q(8312893525486221525, 4232920939787140673);
	c2 = mul_mod_q(7825868133432246571, 12594363607212086362);

	cout << "** C1 = " << c1 << endl;
	cout << "--> r1 * r2 = 1" << endl;
	K = gxA;
	K_inverse = extended_Euclidean(K, Q);
	unsigned long long M2_M3 = mul_mod_q(c2, K_inverse);
	cout << endl;

	cout << "******************* ANSWER **********************" << endl;
	cout << "final answer --> M2 * M3 is " << M2_M3 << endl;
	cout << "*************************************************" << endl << endl;


	system("pause");
	return 0;

}