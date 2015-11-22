#include <random>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

double modPow(double a, double b, double mod)// функция a^b(mod mod)
{
	double res = 1;
	for (double i = b; i > 0; i--)
	{
		res = res * a;
		if (res > mod)
		{
			res = fmod(res, mod);
		}
	}
	return res;
}

static bool MillerRabdoubleest(double n, double k)//Тест Миллера — Рабина (проверка числа на простоту)
{
	if (n <= 1)
		return false;
	if (n == 2)
		return true;
	if (fmod(n,2) == 0)
		return false;
	double s = 0, d = n - 1;
	while (fmod(d,2) == 0)
	{
		d /= 2;
		s++;
	}

	double randNum = rand() + 2;

	for (double i = 0; i < k; i++)
	{
		long a = fmod(randNum,(n - 1));
		double x = modPow(a, d, n);
		if (x == 1 || x == n - 1)
			continue;
		for (double j = 0; j < s - 1; j++)
		{
			x = fmod((x*x),n);
			if (x == 1)
				return false;
			if (x == n - 1)
				break;
		}
		if (x != n - 1)
			return false;
	}
	return true;
}

double gen_rand(double min, double max)//рандом
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

double gen_prime(double min, double max)//генератор простых чисел
{
	double randNum;
	bool test;
	do{
		randNum = gen_rand(min, max);
		double k = log2(randNum);
		test = MillerRabdoubleest(randNum, k);
	} while (!test);
	return randNum;
}

double generator(double p) { //вычисление первообразного корня по модулю p
	vector<double> fact;
	double phi = p - 1, n = phi;
	for (double i = 2; i*i <= n; ++i)
		if (fmod(n,i) == 0) {
			fact.push_back(i);
			while (fmod(n,i) == 0)
				n /= i;
		}
	if (n > 1)
		fact.push_back(n);

	for (double res = 2; res <= p; ++res) {
		bool ok = true;
		for (size_t i = 0; i<fact.size() && ok; ++i)
			ok &= modPow(res, phi / fact[i], p) != 1;
		if (ok)  return res;
	}
	return -1;
}

double gcd(double x, double y)//взаимная простота
{
	return y ? gcd(y, fmod(x,y)) : x;
}