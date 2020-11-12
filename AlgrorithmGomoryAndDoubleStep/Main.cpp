#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>

using namespace std;

class Slitter
{
	protected:
	void Split(vector<string>& strings, const string& line, char splitter = ' ')
	{
		size_t size = line.size();
		string temp = "";
		for(size_t i = 0; i < size; i++)
		{
			if(splitter == line[i])
			{
				strings.push_back(temp);
				temp = "";
			}
			else
			{
				temp += line[i];
			}
		}
		strings.push_back(temp);
	}
};

class Fraction
{
	private:
	int _numerator;
	int _denominator;

	public:
	Fraction(): _numerator(), _denominator(1) {}

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
		Reduce(*this);
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

		Reduce(result);
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

		Reduce(result);
		Revers(result);
		return result;
	}

	Fraction operator * (const Fraction& f1)
	{
		Fraction result(_numerator * f1._numerator, _denominator * f1._denominator);
		Reduce(result);
		Revers(result);
		return result;
	}

	Fraction operator / (const Fraction& f)
	{
		Fraction result(_numerator * f._denominator, _denominator * f._numerator);
		Reduce(result);
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

	void Reduce(Fraction& f)
	{
		int gcd = GetGСD(f._numerator, f._denominator);
		f._numerator /= gcd;
		f._denominator /= gcd;
	}

	void Revers(Fraction& f)
	{
		if(f._denominator < 0)
		{
			f._numerator *= -1;
			f._denominator *= -1;
		}
	}

	int GetGСD(int a, int b)
	{
		return (b == 0) ? a : GetGСD(b, a % b);
	}

	void CheckDenominator(int number)
	{
		if(IsZero(number))
		{
			cout << "Знаменатель не может быть равен нулю!" << endl;
			exit(1);
		}
	}

	bool IsZero(int number)
	{
		return number == 0;
	}
};

class DoubleStep: Slitter
{
	private:
	vector<vector<int>> _matrixOfRestriction;
	vector<int> _targetFunction;
	string _typeOfTask;
	size_t _sizeRow;
	size_t _sizeCol;

	public:
	DoubleStep(const string fileName)
	{
		ReadFromFile(fileName);
	}

	private:
	void ReadFromFile(const string fileName)
	{
		ifstream in;
		in.open(fileName);
		if(in.is_open() == false)
		{
			cout << "Файл не найден.\n";
		}

		string line;
		getline(in, line);
		vector<string> splittedLine;

		Split(splittedLine, line);
		_typeOfTask = splittedLine.back();
		_sizeCol = splittedLine.size();

		for(size_t i = 0; i < _sizeCol - 1; i++)
		{
			_targetFunction.push_back(stoi(splittedLine[i]));
		}
		splittedLine.clear();
		int size;
		while(getline(in, line))
		{
			Split(splittedLine, line);
			size = splittedLine.size();
			_matrixOfRestriction.push_back(vector<int>(size));
			for(size_t i = 0; i < splittedLine.size(); i++)
			{
				_matrixOfRestriction.back()[i] = stoi(splittedLine[i]);
			}
			splittedLine.clear();
		}
		_sizeRow = _matrixOfRestriction.size();
		in.close();
		ShowMatrix();
	}

	void ShowMatrix()
	{
		for(size_t i = 0; i < _sizeRow; i++)
		{
			for(size_t j = 0; j < _sizeCol; j++)
			{
				cout << _matrixOfRestriction[i][j] << '\t';
			}
			cout << '\n';
		}
	}
};

class Simplex:Slitter
{
	private:
	int** _matrix;
	int _sizeRow;
	int _sizeCol;
	string _type;
	vector<string> _namesCols;
	vector<string> _namesRows;
	public:
	Simplex(string fileName/*, vector<string> namesCols, vector<string> namesRows*/)
	{
		/*_namesCols = namesCols;
		_namesRows = namesRows;*/

		ReadFromFile(fileName);
	}

	~Simplex()
	{
		for(size_t i = 0; i < _sizeRow; i++)
		{
			delete[] _matrix[i];
		}
		delete[] _matrix;
	}

	
	private:

	void ReadFromFile(const string fileName)
	{
		ifstream in;
		in.open(fileName);
		if(in.is_open() == false)
		{
			cout << "Файл не найден.\n";
			exit(0);
		}

		string line;
		getline(in, line);
		vector<string> splittedLine;

		Split(splittedLine, line);
		_sizeRow = stoi(splittedLine[0]);
		_sizeCol = stoi(splittedLine[1]);
		_type =  splittedLine[2];
		MemoryAllocation(_matrix, _sizeRow, _sizeCol);
		_namesRows = vector<string>(_sizeRow);

		getline(in, line);
		Split(_namesCols, line);

		splittedLine.clear();
		int size;
		int row = 0;
		int col = 0;
		while(getline(in, line))
		{
			Split(splittedLine, line);
			size = splittedLine.size();
			_namesRows[row] = splittedLine[0];
			for(size_t i = 1; i < _sizeCol + 1; i++)
			{
				_matrix[row][col++] = stoi(splittedLine[i]);
			}
			splittedLine.clear();
			row++;
			col = 0;
		}
		in.close();

		ShowMatrix();
	}

	void MemoryAllocation(int** & matr, int sizeRow, int cols)
	{
		matr = new int*[sizeRow];
		for(size_t i = 0; i < sizeRow; i++)
		{
			matr[i] = new int[cols];
		}
	}

	void ShowMatrix()
	{
		for(size_t i = 0; i < _namesCols.size(); i++)
		{
			cout << _namesCols[i] << '\t';
		}
		cout << '\n';
		for(size_t i = 0; i < _sizeRow; i++)
		{
			cout << _namesRows[i] << '\t';
			for(size_t j = 0; j < _sizeCol; j++)
			{
				cout << _matrix[i][j] << '\t';
			}
			cout << '\n';
		}
	}
};

int main()
{
	setlocale(LC_ALL, "rus");
	
	//DoubleStep task1("input.txt");
	Simplex s("inputSimplex.txt");
	return 0;
}