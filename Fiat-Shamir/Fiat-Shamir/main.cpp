#include <iostream>
#include "gen_rand.h"
#include <time.h>
using namespace std;

class Verific{
public:
	double x, e, n;
	Verific(double n)
	{
		this->n = n;
	}
	void gen_E()
	{
		this->e = gen_rand(0, 1);
	}
	void set_X(double x)
	{
		this->x = x;
	}
	bool check(double y, double v)
	{
		double ch = (this->e > 0) ? modPow(x*v, 1, this->n) : modPow(x, 1, n);
		cout << "\n\n\ty^2 mod n = " << modPow(y, 2, this->n)<<"\n\tx*V^e mod n = "<< ch;
		if (modPow(y, 2, this->n) == ch)
		{
			cout <<"\n\t"<< modPow(y, 2, this->n) << "==" << ch;
			return true;
		}
		else
		{
			cout <<"\n\n"<< modPow(y, 2, this->n) << "!=" << ch; 
			return false;
		}
	}
};

class Client{//класс описывающий клиента для обмена ключа
private:
	double s,r;
public:
	double x, n, v;
	Client(double n)
	{
		this->n = n;
		this->gen_S();
		this->gen_V();
		cout << "\n\tsecret s : " << this->s << "\n\tpublic key V = s^2 mod n : " << this->v;
	}
	void gen_V()
	{
		this->v = modPow(this->s,2,this->n);
	}
	void gen_S()
	{
		double s;
		do{
			s = gen_rand(1, (this->n - 1));
		} while (gcd(s,this->n)!=1);
		this->s = s;
	}
	void gen_X()
	{
		this->r = gen_rand(1, this->n - 1);
		this->x = modPow(this->r, 2, this->n);
		cout << "\n\tx = r^2 mod n\t" << this->x << " = " << this->r << "^2 mod " << this->n;
	}
	double calc_Y(double e)
	{
		if(e == 0) return modPow(this->r,1,this->n);
		else return modPow((this->r * this->s),1,this->n);		
	}
	void show_S()
	{
		cout << "S = " << this->s<<"\n";
	}
};

int main()
{
	srand(time(NULL));
	double p = gen_prime(100, 1000);
	double q = gen_prime(100, 1000);
	double n = p*q;
	cout << "p : " << p << "\nq : " << q << "\nn = p*q = " << n;
	cout << "\n\nAlice : ";
	Client Alice(n);					//A: S = {1,n-1}; V = S^2 mod n
	double v = Alice.v;					//Alice public key
	
	Verific Bob(n); 
	bool checkA = true;
	for (double t = 0; t < 20; t++)
	{
		cout << "\nt : " << t;
		Alice.gen_X();	                //A: r = {1,n-1} and calc X = r^2 mod n
		cout << "\n\tA=>B: x = " << Alice.x;
		Bob.set_X(Alice.x);				//A=>B: X
		Bob.gen_E();					//B=>A: e = {0,1}
		cout << "\n\tB=>A: e = " << Bob.e;
		double y = Alice.calc_Y(Bob.e);	//A=>B: y = (r * s^e) mod n
		cout << "\n\tA=>B: y = (r * s^e) mod n = " << y;
		if (!Bob.check(y, v))			//B: if y^2 mod n = x*v^e mod n => continue; else break
		{
			cout << "Verification error\n";
			checkA = false;
			break;
		}
		getchar();
	}
	if (checkA)
	{
		cout << "Verification OK!\n";
		Alice.show_S();
	}
	system("pause");
	return 0;
}