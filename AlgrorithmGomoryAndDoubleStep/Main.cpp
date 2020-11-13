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
	//int _wholePart;
	public:
	Fraction(): _numerator(), _denominator(1) {}

	Fraction(int numerator): _numerator(numerator), _denominator(1) {}

	Fraction(int numerator, int denominator)
	{
		CheckDenominator(denominator);
		_numerator = numerator;
		_denominator = denominator;
		ReduceAndReves(*this);
	}

	Fraction& operator = (const Fraction& f)
	{
		_numerator = f._numerator;
		_denominator = f._denominator;
		ReduceAndReves(*this);
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

		ReduceAndReves(result);
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

		ReduceAndReves(result);
		return result;
	}

	Fraction operator - ()
	{
		return Fraction(_numerator * (-1), _denominator);
	}

	Fraction operator * (const Fraction& f1)
	{
		Fraction result(_numerator * f1._numerator, _denominator * f1._denominator);
		ReduceAndReves(result);
		return result;
	}

	Fraction operator / (const Fraction& f)
	{
		Fraction result(_numerator * f._denominator, _denominator * f._numerator);
		ReduceAndReves(result);
		return result;
	}

	Fraction operator / (const int& number)
	{
		CheckDenominator(number);
		Fraction result(_numerator * 1, _denominator * number);
		ReduceAndReves(result);
		return result;
	}

	bool operator > (const Fraction& f1)
	{
		if(_denominator == f1._denominator)
		{
			return _numerator > f1._numerator;
		}
		else if(_numerator == f1._numerator)
		{
			return _denominator < f1._denominator;
		}
		else
		{
			int lcm = GetLCM(_denominator, f1._denominator);
			return _numerator * lcm / _denominator > f1._numerator * lcm / f1._denominator;
		}
	}

	bool operator < (const Fraction& f1)
	{
		return (*this > f1) == false;
	}

	bool operator == (const Fraction& f1)
	{
		return _numerator == f1._numerator && _denominator == f1._denominator;
	}

	bool operator != (const Fraction& f1)
	{
		return (*this == f1) == false;
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

	void ReduceAndReves(Fraction& f)
	{
		Reduce(f);
		Revers(f);
	}

	void Reduce(Fraction& f)
	{
		int gcd = GetG�D(f._numerator, f._denominator);
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

	/// <summary>
	/// ���������� ����� �������
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	int GetLCM(int a, int b)
	{
		return abs(a * b) / GetG�D(b, a);
	}

	int GetG�D(int a, int b)
	{
		return (b == 0) ? a : GetG�D(b, a % b);
	}

	void CheckDenominator(int number)
	{
		if(IsZero(number))
		{
			cout << "����������� �� ����� ���� ����� ����!" << endl;
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
			cout << "���� �� ������.\n";
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
	Fraction** _matrix;
	size_t _rows;
	size_t _columns;
	string _type;
	vector<string> _namesColums;
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
		for(size_t i = 0; i < _rows; i++)
		{
			delete[] _matrix[i];
		}
		delete[] _matrix;
	}

	void Solve()
	{
		while(IsOptimalPlan() == false)
		{
			int indexColumnNewBazis = GetIndexMaxCellInZ();
			int indexRowMinValueInDeferens = GetIndexMinDeferensInRow(indexColumnNewBazis);

			_namesRows[indexRowMinValueInDeferens] = _namesColums[indexColumnNewBazis + 2];
			DivRowOn(indexRowMinValueInDeferens, _matrix[indexRowMinValueInDeferens][indexColumnNewBazis]);
			ChangeMatrix(indexColumnNewBazis, indexRowMinValueInDeferens);
		}
		ShowTable();
	}

	vector<string> GetNamesColmuns()
	{
		return _namesColums;
	}

	vector<string> GetNamesRows()
	{
		return _namesRows;
	}

	private:

	bool IsOptimalPlan()
	{
		int countInColum = 0;
		if(_type == "min")
		{
			for(size_t i = 1; i < _columns; i++)
			{
				if(_matrix[_rows - 1][i] > 0)
				{
					for(size_t j = 0; j < _rows - 1; j++)
					{
						if(_matrix[j][i] > 0)
						{
							return false;
						}
						countInColum++;
					}
					if(countInColum == _rows - 1)
					{
						ShowInfoNotOptimalPlan("�����");
					}
				}
			}
		}
		else if(_type == "max")
		{
			for(size_t i = 1; i < _columns; i++)
			{
				if(_matrix[_rows - 1][i] < 0)
				{
					for(size_t j = 0; j < _rows - 1; j++)
					{
						if(_matrix[j][i] < 0)
						{
							return false;
						}
						countInColum++;
					}
				}
				if(countInColum == _rows - 1)
				{
					ShowInfoNotOptimalPlan("�����");
				}
			}
		}
		else
		{
			cout << "��� ������ ������ ���� ������ min ��� max. ��������� ����!\n";
			exit(0);
		}
		return true;
	}

	void ShowInfoNotOptimalPlan(string text)
	{
		cout << "������������ ������� �� ����������, �. �. ������� f �� ���������� " << text << "!\n";
		exit(0);
	}

	void ChangeMatrix(int indexNewCol, int indexRow)
	{
		Fraction newValue;
		Fraction oldValue;
		for(size_t i = 0; i < _rows; i++)
		{
			if(i < indexRow || i > indexRow)
			{
				oldValue = _matrix[i][indexNewCol];
				for(size_t j = 0; j < _columns; j++)
				{
					_matrix[i][j] = -_matrix[indexRow][j] * oldValue + _matrix[i][j];
				}
			}
		}
	}

	void DivRowOn(int indexRow, Fraction currentCell)
	{
		for(size_t i = 0; i < _columns; i++)
		{
			_matrix[indexRow][i] = _matrix[indexRow][i] / currentCell;
		}
	}

	int GetIndexMinDeferensInRow(size_t indexCol)
	{
		Fraction min = _matrix[0][0] / _matrix[0][indexCol];
		int index = 0;
		Fraction value;
		for(size_t i = 1; i < _rows - 1; i++)
		{
			if(_matrix[i][indexCol].GetNumerator() > 0)
			{
				value = _matrix[i][0] / _matrix[i][indexCol];
				if(value < min)
				{
					index = i;
					min = value;
				}
			}
		}
		return index;
	}

	int GetIndexMaxCellInZ()
	{
		int lastRow = _rows - 1;
		int index = GetIndexNotZeroInZ();
		Fraction minMax = _matrix[lastRow][index];
		if(_type == "min")
		{
			for(size_t i = 2; i < _columns; i++)
			{
				if(_matrix[lastRow][i] > minMax)
				{
					minMax = _matrix[lastRow][i];
					index = i;
				}
			}
		}
		else if(_type == "max")
		{
			for(size_t i = 2; i < _columns; i++)
			{
				if(_matrix[lastRow][i] < minMax)
				{
					minMax = _matrix[lastRow][i];
					index = i;
				}
			}
		}
		else
		{
			cout << "Type must be min or max. Check file!\n";
			exit(0);
		}

		return index;
	}

	int GetIndexNotZeroInZ()
	{
		for(size_t i = 1; i < _columns; i++)
		{
			if(_matrix[_rows - 1][i] != 0)
			{
				return i;
			}
		}
	}

	bool IsBazis(int indexColumn)
	{
		return _matrix[_rows - 1][indexColumn] == 0;
	}

	void ReadFromFile(const string fileName)
	{
		ifstream in;
		in.open(fileName);
		if(in.is_open() == false)
		{
			cout << "���� �� ������.\n";
			exit(0);
		}

		string line;
		getline(in, line);
		vector<string> splittedLine;

		Split(splittedLine, line);
		_rows = stoi(splittedLine[0]);
		_columns = stoi(splittedLine[1]);
		_type = splittedLine[2];
		MemoryAllocation(_matrix, _rows, _columns);
		_namesRows = vector<string>(_rows);

		getline(in, line);
		Split(_namesColums, line);

		splittedLine.clear();
		int size;
		int row = 0;
		int col = 0;
		while(getline(in, line))
		{
			Split(splittedLine, line);
			size = splittedLine.size();
			_namesRows[row] = splittedLine[0];
			for(size_t i = 1; i < _columns + 1; i++)
			{
				_matrix[row][col++] = Fraction(stoi(splittedLine[i]));
			}
			splittedLine.clear();
			row++;
			col = 0;
		}
		in.close();

		ShowTable();
	}

	void MemoryAllocation(Fraction**& matr, size_t sizeRow, size_t cols)
	{
		matr = new Fraction * [sizeRow];
		for(size_t i = 0; i < sizeRow; i++)
		{
			matr[i] = new Fraction[cols];
		}
	}

	void ShowTable()
	{
		for(size_t i = 0; i < _namesColums.size(); i++)
		{
			cout << _namesColums[i] << '\t';
		}
		cout << '\n';
		for(size_t i = 0; i < _rows; i++)
		{
			cout << _namesRows[i] << '\t';
			for(size_t j = 0; j < _columns; j++)
			{
				cout << _matrix[i][j] << '\t';
			}
			cout << '\n';
		}
		cout << '\n';
	}
};

int main()
{
	setlocale(LC_ALL, "rus");

	//DoubleStep task1("input.txt");
	Simplex s("inputSimplex.txt");
	s.Solve();

	/*Fraction a(3, 4);
	Fraction b(2, 4);
	cout << (a < b) << endl;*/

	return 0;
}

//9 15 min
//����� ������� x1 x2 x3 y1 y2 y3 y4 y5 y6 r1 r2 r3 r4 r5
//y1 4000 2 3 5 1 0 0 0 0 0 0 0 0 0 0
//y2 6000 4 2 7 0 1 0 0 0 0 0 0 0 0 0
//y3 9000 6 3 2 0 0 1 0 0 0 0 0 0 0 0
//r1 200 1 0 0 0 0 0 - 1 0 0 1 0 0 0 0
//r2 200 0 1 0 0 0 0 0 - 1 0 0 1 0 0 0
//r3 150 0 0 1 0 0 0 0 0 - 1 0 0 1 0 0
//r4 0 2 - 3 0 0 0 0 0 0 0 0 0 0 1 0
//r5 0 0 5 - 2 0 0 0 0 0 0 0 0 0 0 1
//Z 550 3 3 - 1 0 0 0 - 1 - 1 - 1 0 0 0 0 0