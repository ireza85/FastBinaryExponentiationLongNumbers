// ConsoleApplication21.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <vector>
#include <string>
#include <iostream>
#include <ctime>

using namespace std;

class BigLong
{
public:
	BigLong(string s);
	BigLong(unsigned long long x = 0) { vectorLongNumbers.push_back(x); };
	operator string() const;

	friend BigLong operator *(const BigLong&a, const BigLong&b);
	friend BigLong operator +(const BigLong&a, const BigLong&b);

private:
	vector<unsigned long long> vectorLongNumbers;
	static constexpr unsigned long long max = 1'000'000'000ull;
};


BigLong operator *(const BigLong&a, const BigLong&b)
{
	BigLong tempVector;
	for (auto i = 0; i < a.vectorLongNumbers.size(); ++i)
	{
		for (auto j = 0; j < b.vectorLongNumbers.size(); ++j)
		{
			unsigned long long v = a.vectorLongNumbers[i] * b.vectorLongNumbers[j];
			unsigned long long carry = v / BigLong::max;
			v = v % BigLong::max;
			if (i + j >= tempVector.vectorLongNumbers.size())
				tempVector.vectorLongNumbers.resize(i + j + 1, 0);
			tempVector.vectorLongNumbers[i + j] += v;
			if (carry)
			{
				if (i + j + 1 >= tempVector.vectorLongNumbers.size())
					tempVector.vectorLongNumbers.resize(i + j + 2, 0);
				tempVector.vectorLongNumbers[i + j + 1] += carry;
			}
		}
	}

	for (auto i = 0; i < tempVector.vectorLongNumbers.size() - 1; ++i)
	{
		if (tempVector.vectorLongNumbers[i] > BigLong::max)
		{
			tempVector.vectorLongNumbers[i + 1] += tempVector.vectorLongNumbers[i] / BigLong::max;
			tempVector.vectorLongNumbers[i] %= BigLong::max;
		}
	}
	return tempVector;
}

BigLong operator +(const BigLong&a, const BigLong&b)
{
	BigLong d((a.vectorLongNumbers.size() > b.vectorLongNumbers.size()) ? a : b);
	BigLong c((a.vectorLongNumbers.size() > b.vectorLongNumbers.size()) ? b : a);
	c.vectorLongNumbers.resize(d.vectorLongNumbers.size(), 0);
	unsigned long long carry = 0;
	for (auto i = 0; i < d.vectorLongNumbers.size(); ++i)
	{
		d.vectorLongNumbers[i] += c.vectorLongNumbers[i] + carry;
		carry = d.vectorLongNumbers[i] / BigLong::max;
		d.vectorLongNumbers[i] %= BigLong::max;
	}
	if (carry) d.vectorLongNumbers.push_back(carry);
	return d;
}

BigLong::BigLong(string s)
{
	BigLong tempString;
	int len = s.length() % 9;
	if (len)
	{
		string val = s.substr(0, len);
		s = s.substr(len, s.length() - len);
		tempString = stoll(val);
	}
	while (s.length())
	{
		string val = s.substr(0, 9);
		s = s.substr(9, s.length() - 9);
		tempString = tempString * BigLong::max + stoll(val);
	}
	vectorLongNumbers = std::move(tempString.vectorLongNumbers);
}

BigLong::operator string() const
{
	string s;
	for (auto i = 0; i < vectorLongNumbers.size(); ++i)
	{
		char buf[12];
		snprintf(buf, 12, "%lld", vectorLongNumbers[i]);
		s = buf + s;
	}
	return s;
}

BigLong superPow(BigLong x, unsigned long long pow)
{
	BigLong temp(1);
	while (pow)
	{
		if (pow & 1) temp = temp * x;
		pow >>= 1;
		x = x * x;
	}
	return temp;
}

BigLong karatsuba(BigLong x, unsigned long long pow) {
	auto len = x.size();
	BigLong res(2 * len);

	if (len <= 32) {
		return superPow(x, y);
	}

	auto k = len / 2;

	BigLong xR( x, x + k );
	BigLong xL( (string(x).c_str()).substr(string(x).begin() + k, string(x).end() - k));
	BigLong yR{ y.begin(), y.begin() + k };
	BigLong yL{ y.begin() + k, y.end() };

	BigLong P1 = karatsuba(xL, yL);
	BigLong P2 = karatsuba(xR, yR);

	BigLong xLR(k);
	BigLong yLR(k);

	for (int i = 0; i < k; ++i) {
		xLR[i] = xL[i] + xR[i];
		yLR[i] = yL[i] + yR[i];
	}

	std::vector<unsigned long long> P3 = karatsuba(xLR, yLR);

	for (auto i = 0; i < len; ++i) {
		P3[i] -= P2[i] + P1[i];
	}

	for (auto i = 0; i < len; ++i) {
		res[i] = P2[i];
	}

	for (auto i = len; i < 2 * len; ++i) {
		res[i] = P1[i - len];
	}

	for (auto i = k; i < len + k; ++i) {
		res[i] += P3[i - k];
	}

	return res;
}



int main(int argc, const char * argv[])
{
	clock_t start = clock();
	string string1, string2;
	getline(cin, string1);
	int i = string1.find(" ");
	string2 = string1.substr(i + 1, string1.length() - 1 - i);
	string1 = string1.substr(0, i);


	cout << string(superPow(BigLong(string1), uint16_t(stoll(string2.c_str())))) << endl;
	cout << string(karatsuba(BigLong(string1), uint16_t(stoll(string2.c_str())))) << endl;
	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	printf("The time: %f seconds\n", seconds);

}