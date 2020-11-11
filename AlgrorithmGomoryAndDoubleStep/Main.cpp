#include <iostream>
#include <time.h>

using namespace std;

class Fraction
{
	private:
	int _numerator;
	int _denominator;

	public:
	Fraction(): _numerator(), _denominator() {}
	Fraction(int numerator, int denominator)
	{
		CheckDenominator(denominator);
		_numerator = numerator;
		_denominator = denominator;
		Revers(*this);
	}

	Fraction& operator = (const Fraction& f)
	{
		_numerator = f._numerator;
		_denominator = f._denominator;
		return *this;
	}

	Fraction operator + (const Fraction& f)
	{
		Fraction result;
		if(_denominator == f._denominator)
		{
			result._numerator = _numerator + f._numerator;
			result._denominator = _denominator;
		}
		else
		{
			result._numerator = _numerator * f._denominator + f._numerator * _denominator;
			result._denominator = _denominator * f._denominator;
		}

		int gcd = GetGÑD(result._numerator, result._denominator);
		result._numerator /= gcd;
		result._denominator /= gcd;
		Revers(result);
		return result;
	}

	Fraction operator - (const Fraction& f)
	{
		Fraction result;
		if(_denominator == f._denominator)
		{
			result._numerator = _numerator - f._numerator;
			result._denominator = _denominator;
		}
		else
		{
			result._numerator = _numerator * f._denominator - f._numerator * _denominator;
			result._denominator = _denominator * f._denominator;
		}

		int gcd = GetGÑD(result._numerator, result._denominator);
		result._numerator /= gcd;
		result._denominator /= gcd;
		Revers(result);
		return result;
	}

	Fraction operator * (const Fraction& f1)
	{
		Fraction result(_numerator * f1._numerator, _denominator * f1._denominator);
		Revers(result);
		return result;
	}

	Fraction operator / (const Fraction& f)
	{
		Fraction result(_numerator * f._denominator, _denominator * f._numerator);
		Revers(result);
		return result;
	}

	friend ostream& operator << (ostream& out, const Fraction& f)
	{
		if(f._denominator == 1)
		{
			out << f._numerator;
		}
		else
		{
			out << f._numerator << '/' << f._denominator;
		}
		return out;
	}

	int GetNumerator()
	{
		return _numerator;
	}

	int GetDenominator()
	{
		return _denominator;
	}

	private:

	void Revers(Fraction& f)
	{
		if(f._denominator < 0)
		{
			f._numerator *= -1;
			f._denominator *= -1;
		}
	}

	int GetGÑD(int a, int b)
	{
		return (b == 0) ? a : GetGÑD(b, a % b);
	}

	void CheckDenominator(int number)
	{
		if(IsZero(number))
		{
			cout << "Çíàìåíàòåëü íå ìîæåò áûòü ðàâåí íóëþ!" << endl;
			exit(1);
		}
	}

	bool IsZero(int number)
	{
		return number == 0;
	}
};


int main()
{
	setlocale(LC_ALL, "rus");

	srand(time(0));

	Fraction a(16, 10);

	Fraction b(2, 5);
	cout << a + b << endl;
	
	return 0;
}