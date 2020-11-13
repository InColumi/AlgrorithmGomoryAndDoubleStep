#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>

using namespace std;

class Shower
{
	public:
	template <typename T>
	void Show(T& matr)
	{
		size_t size = matr.size();
		for(size_t i = 0; i < size; i++)
		{
			cout << matr[i] << '\n';
		}
	}

	template <typename T>
	void Show(T& matr, size_t rows, size_t colums)
	{
		for(size_t i = 0; i < rows; i++)
		{
			for(size_t j = 0; j < colums; j++)
			{
				cout << matr[i][j] << '\t';
			}
			cout << '\n';
		}
	}

	template <typename T>
	void Show(T**& matr, size_t rows, size_t colums)
	{
		for(size_t i = 0; i < rows; i++)
		{
			for(size_t j = 0; j < colums; j++)
			{
				cout << matr[i][j] << '\t';
			}
			cout << '\n';
		}
	}
};

class Slitter
{
	protected:
	void Split(vector<string>& strings, string& line, char splitter = ' ')
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

	Fraction operator * (const Fraction& f)
	{
		Fraction result(_numerator * f._numerator, _denominator * f._denominator);
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

	bool operator > (const Fraction& f)
	{
		if(_denominator == f._denominator)
		{
			return _numerator > f._numerator;
		}
		else if(_numerator == f._numerator)
		{
			return _denominator < f._denominator;
		}
		else
		{
			int lcm = GetLCM(_denominator, f._denominator);
			return _numerator * lcm / _denominator > f._numerator * lcm / f._denominator;
		}
	}

	bool operator >= (const Fraction& f)
	{
		return *this > f || *this == f;
	}

	bool operator <= (const Fraction& f)
	{
		return *this < f || *this == f;
	}

	bool operator < (const Fraction& f)
	{
		return (*this > f) == false;
	}

	bool operator == (const Fraction& f)
	{
		return _numerator == f._numerator && _denominator == f._denominator;
	}

	bool operator != (const Fraction& f)
	{
		return (*this == f) == false;
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

	/// <summary>
	/// Наименьшее общее кратное
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	int GetLCM(int a, int b)
	{
		return abs(a * b) / GetGСD(b, a);
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
	Simplex(Fraction**& matrix, size_t rows, size_t columns, string type, vector<string>& namesColums, vector<string>& namesRows)
	{
		_matrix = matrix;
		_rows = rows;
		_columns = columns;
		_type = type;
		_namesColums = namesColums;
		_namesRows = _namesRows;
	}

	Simplex(string fileName)
	{
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

	Fraction**& GetMatrix()
	{
		return _matrix;
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
						ShowInfoNotOptimalPlan("снизу");
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
					ShowInfoNotOptimalPlan("снизу");
				}
			}
		}
		else
		{
			cout << "Тип задачи должне быть только min или max. Проверьте файл!\n";
			exit(0);
		}
		return true;
	}

	void ShowInfoNotOptimalPlan(string text)
	{
		cout << "Оптимального решение не существует, т. к. функция f не ограничена " << text << "!\n";
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

class Cell
{
	private:
	string _name;
	size_t _indexName;
	int _value;

	public:
	Cell(): _name(), _indexName(), _value() {};
	Cell(string name, size_t indexName, int value): _name(name), _indexName(indexName), _value(value) {}
	
	friend ostream& operator << (ostream& out, const Cell& p)
	{
		out << '(' << p._name << '-' << p._indexName << '=' << p._value <<  ')';
		return out;
	}

	string GetName()
	{
		return _name;
	}

	size_t GetIndex()
	{
		return _indexName;
	}

	int GetValue()
	{
		return _value;
	}
};

class DoubleStep: Slitter, Shower
{
	private:
	const string _nameInitialVariable = "x";
	const string _nameSimpleVariable = "s";
	const string _nameArtificialVariable = "r";
	const string _nameLastRow = "Z";
	vector<vector<Fraction>> _matrix;
	vector<string> _textFromFile;
	vector<string> _namesColums;
	vector<string> _namesRows;
	vector<int> _targetFunction;
	vector<Cell> _cells;
	string _typeOfTask;
	size_t _countInitialVariable;
	size_t _countSimpleVariable;
	size_t _countArtificialVariable;
	size_t _rows;
	size_t _colums;

	public:
	DoubleStep(const string fileName)
	{
		_textFromFile = ReadFromFile(fileName);
		SetTargetFunction(_textFromFile[0]);
		SetCountInitialVariable(_textFromFile[1]);
		CheckCorrectInput();

		_rows = _textFromFile.size();
		Show(_textFromFile);
		CheckTypeOfTask();
		_countInitialVariable = _targetFunction.size();
		SetNamesColums();
		SetInitialMatrix();
		AddNamesToColumnsAndCountNewVariables();
		SetMatrix();
		Show(_matrix, _rows, _colums);
		SetNamesRows();
	}

	private:

	void SetCountInitialVariable(string line)
	{
		vector<string> splittedLine;
		Split(splittedLine, line);
		_countInitialVariable = splittedLine.size() - 2;
	}

	void SetNamesColums()
	{
		for(size_t i = 1; i <= _countInitialVariable; i++)
		{
			_namesColums.push_back(_nameInitialVariable + to_string(i));
		}
	}

	void SetInitialMatrix()
	{
		vector<string> splittedLine;
		size_t size;
		for(size_t i = 0; i < _rows; i++)
		{
			_matrix.push_back(vector<Fraction>(_countInitialVariable));
		}

		for(size_t i = 1; i < _rows; i++)
		{
			Split(splittedLine, _textFromFile[i]);

			size_t size = splittedLine.size();
			for(size_t j = 0; j < size - 2; j++)
			{
				try
				{
					_matrix[i - 1][j] = stoi(splittedLine[j]);
				}
				catch(const std::exception&)
				{
					cout << "Проверьте данные в файле!\n";
					exit(0);
				}
			}
			splittedLine.clear();
		}
	}

	void SetNamesRows()
	{
		for(size_t i = 1; i < _colums; i++)
		{
			if(IsBazis(i))
			{
				_namesRows.push_back(_namesColums[i]);
			}
		}
		_namesRows.push_back(_nameLastRow);
	}

	bool IsBazis(size_t indexCol)
	{
		Fraction number = 0;
		int countZero = 0;
		int countAnotherValue = 0;
		for(size_t i = 0; i < _rows; i++)
		{
			if(countAnotherValue == 2)
			{
				return false;
			}
			if(_matrix[i][indexCol] == 0)
			{
				countZero++;
			}
			else
			{
				number = _matrix[i][indexCol];
				countAnotherValue++;
			}
		}
		return number == 1;
	}

	void SetMatrix()
	{
		_matrix.clear();
		_colums = _countInitialVariable + _countSimpleVariable + _countArtificialVariable;
		vector<string> splittedLine;
		size_t size;
		for(size_t i = 0; i < _rows; i++)
		{
			_matrix.push_back(vector<Fraction>(_colums));
		}

		for(size_t i = 1; i < _rows; i++)
		{
			Split(splittedLine, _textFromFile[i]);

			size_t size = splittedLine.size();
			_matrix[i - 1][0] = stoi(splittedLine[size - 1]);
			for(size_t j = 1; j < size - 2; j++)
			{
				_matrix[i - 1][j] = stoi(splittedLine[j]);
			}
			splittedLine.clear();
		}

		int index;
		for(size_t i = _countInitialVariable; i < _colums; i++)
		{
			index = _cells[i - _countInitialVariable].GetIndex();
			for(size_t j = 0; j < _cells.size(); j++)
			{
				if(index == j)
				{
					_matrix[index][i] = _cells[i - _countInitialVariable].GetValue();
					break;
				}
			}
		}
	}

	void AddNamesToColumnsAndCountNewVariables()
	{
		_countSimpleVariable = 0;
		_countArtificialVariable = 0;
		vector<string> splittedLine;
		size_t size;
		string sing;
		for(size_t i = 1; i < _textFromFile.size(); i++)
		{
			Split(splittedLine, _textFromFile[i]);

			size = splittedLine.size();
			sing = splittedLine[size - 2];
			string nameNewSipmleVariable;
			string nameNewArtificialVariable;
			if(sing == "<=")
			{
				nameNewSipmleVariable = _nameSimpleVariable + to_string(++_countSimpleVariable);
				_cells.push_back(Cell(nameNewSipmleVariable, i - 1, 1));
				_namesColums.push_back(nameNewSipmleVariable);
			}
			else if(sing == ">=")
			{
				nameNewSipmleVariable = _nameSimpleVariable + to_string(++_countSimpleVariable);
				nameNewArtificialVariable = _nameArtificialVariable + to_string(++_countArtificialVariable);
				_cells.push_back(Cell(nameNewSipmleVariable, i - 1, -1));
				_cells.push_back(Cell(nameNewArtificialVariable, i - 1, 1));
				_namesColums.push_back(nameNewSipmleVariable);
				_namesColums.push_back(nameNewArtificialVariable);

			}
			else if(sing == "=")
			{
				bool isNewVariable = true;
				for(size_t k = 0; k < _countInitialVariable; k++)
				{
					if(_matrix[i - 1][k] == 1)
					{
						isNewVariable = false;
						int countOne = 0;
						for(size_t j = 0; j < _rows - 1; j++)
						{
							if(_matrix[j][k] != 0)
							{
								++countOne;
							}
							if(countOne > 1)
							{
								countOne = 0;
								break;
							}
						}

						if(countOne == 0)
						{
							isNewVariable = true;
							continue;
						}

						if(countOne == 1)
						{
							break;
						}
					}
				}
				if(isNewVariable)
				{
					nameNewArtificialVariable = _nameArtificialVariable + to_string(++_countArtificialVariable);
					_cells.push_back(Cell(nameNewArtificialVariable, i - 1, 1));
					_namesColums.push_back(nameNewArtificialVariable);
				}
			}
			else
			{
				cout << "Знак может быть только ( >=, <=, = ). Проверьте файл!\n";
				exit(0);
			}
			splittedLine.clear();
		}
	}

	void CheckTypeOfTask()
	{
		if(IsCorrectType() == false)
		{
			cout << "Тип задачи должне быть только min или max. Проверьте файл!\n";
			exit(0);
		}
	}

	bool IsCorrectType()
	{
		return _typeOfTask == "min" || _typeOfTask == "max";
	}

	void SetTargetFunction(string line)
	{
		vector<string> splittedLine;
		Split(splittedLine, line);
		_typeOfTask = splittedLine.back();

		for(size_t i = 0; i < splittedLine.size() - 1; i++)
		{
			_targetFunction.push_back(stoi(splittedLine[i]));
		}
	}

	void CheckCorrectInput()
	{
		if(_targetFunction.size() != _countInitialVariable)
		{
			cout << "Кол-во переменных в целевой функции не совпадает с кол-вом переменных в матрице. Проверьте файл!\n";
			exit(0);
		}
	}

	vector<string> ReadFromFile(const string fileName)
	{
		ifstream in;
		in.open(fileName);
		if(in.is_open() == false)
		{
			cout << "Файл не найден.\n";
			exit(0);
		}

		string line;
		vector<string> text;
		while(getline(in, line))
		{
			text.push_back(line);
		}
		in.close();
		return text;
	}
};

int main()
{
	setlocale(LC_ALL, "rus");

	DoubleStep task1("input.txt");
	//Simplex s("inputSimplex.txt");
	//s.Solve();

	/*Fraction a(3, 4);
	Fraction b(2, 4);
	cout << (a < b) << endl;*/

	return 0;
}

//9 15 min
//Базис Решение x1 x2 x3 y1 y2 y3 y4 y5 y6 r1 r2 r3 r4 r5
//y1 4000 2 3 5 1 0 0 0 0 0 0 0 0 0 0
//y2 6000 4 2 7 0 1 0 0 0 0 0 0 0 0 0
//y3 9000 6 3 2 0 0 1 0 0 0 0 0 0 0 0
//r1 200 1 0 0 0 0 0 - 1 0 0 1 0 0 0 0
//r2 200 0 1 0 0 0 0 0 - 1 0 0 1 0 0 0
//r3 150 0 0 1 0 0 0 0 0 - 1 0 0 1 0 0
//r4 0 2 - 3 0 0 0 0 0 0 0 0 0 0 1 0
//r5 0 0 5 - 2 0 0 0 0 0 0 0 0 0 0 1
//Z 550 3 3 - 1 0 0 0 - 1 - 1 - 1 0 0 0 0 0

//30 20 50 max
//2 3 5 <= 4000
//4 2 7 <= 6000
//6 3 2 <= 9000
//1 0 0 >= 200
//0 1 0 >= 200
//0 0 1 >= 150
//2 - 3 0 = 0
//0 5 - 2 = 0

//4 2 0 0 min
//3 1 0 0 = 3
//4 3 -1 0 = 6
//1 2 0 1 = 4