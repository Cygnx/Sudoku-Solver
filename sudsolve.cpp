#include <iostream>
#include <vector>
#include <stdlib.h>
#include <set>
#include <list>
#include <stack> 
#include <time.h>
#include <stdio.h>
#include <string.h>

using namespace std;
struct point
{
	int x;
	int y;
};
class cell
{

private:
public:
	char cCurrent;
	set<int> sChoices;
	int nCurrent;
	cell() : cCurrent(0), nCurrent(0){}

	void fill_choices()
	{
		if (nCurrent == 0)
		for (int i = 1; i < 10; i++)
			sChoices.insert(i);
	}

	void print_choices()
	{
		for (set<int>::iterator it = sChoices.begin(); it != sChoices.end(); ++it)
			cout << *it;
	}

	void parse_input()
	{
		if (cCurrent == '.')
			nCurrent = 0;
		else
			nCurrent = cCurrent - '0';
	}

	void eliminate_choice(set<int> sEliminate)
	{
		for (set<int>::iterator it = sEliminate.begin(); it != sEliminate.end(); ++it)
			sChoices.erase(*it);
	}

	bool promote_possible_choice()
	{
		if (sChoices.size() == 1 && nCurrent == 0) // promote single choice to nCurrent if current is a .
		{
			nCurrent = *(sChoices.begin());
			sChoices.clear();
			return true;
		}
		return false;
	}
};


class puzzle
{

private:
public:
	vector< vector<cell> > puzzle_cells;
	puzzle() : puzzle_cells(9, vector<cell>(9))
	{
	}

	void get_puzzle()
	{
		char c_input[85];

		for (int i = 0; i < 81; i++)
		{
			c_input[i] = cin.get();
			if (c_input[i] == EOF)
			{
				cout << "ERROR: expected <value> saw <eof>" << endl;
				exit(0);
			}
			else if (c_input[i] == '\n')
			{
				cout << "ERROR: expected <value> saw \\n" << endl;
				exit(0);
			}
			else if (!(c_input[i] > '0' && c_input[i] <= '9') && c_input[i] != '.')
			{
				cout << "ERROR: expected <value> saw ";
				if (c_input[i] >= ' ')
					cout << c_input[i] << endl;
				else
				{
					cout << "\\x";
					cout.width(2);
					cout.fill('0');
					cout << hex << (int)c_input[i] << endl;
				}
				exit(0);
			}

		}
		c_input[81] = getchar();
		if (c_input[81] != '\n') // new line is assumed
		{
			cout << "ERROR: expected \\n saw ";
			if (c_input[81] == EOF)
				cout << "<eof>" << endl;
			else
			{
				if (c_input[81] >= ' ')
					cout << c_input[81] << endl;
				else
				{
					cout << "\\x";
					cout.width(2);
					cout.fill('0');
					cout << hex << (int)c_input[81] << endl;
				}
			}
			exit(0);
		}
		c_input[82] = getchar();
		if (c_input[82] != EOF)
		{
			cout << "ERROR: expected <eof> saw ";
			if (c_input[82] == '\n')
				cout << "\\n" << endl;
			else
			{
				if (c_input[82] >= ' ')
					cout << c_input[82] << endl;
				else
				{
					cout << "\\x";
					cout.width(2);
					cout.fill('0');
					cout << hex << (int)c_input[82] << endl;
				}
			}
			exit(0);
		}

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				puzzle_cells[i][j].cCurrent = c_input[i * 9 + j];
				puzzle_cells[i][j].parse_input();
				puzzle_cells[i][j].fill_choices();
			}
		}

	}

	void print_puzzle()
	{
		//	cout << "//-------------------------------------------------------------------\\" << endl;

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				cout << puzzle_cells[i][j].nCurrent;
				puzzle_cells[i][j].print_choices();
				//			cout << " ";
			}
			//	cout << endl << endl;
		}
		cout << endl;
		//		cout << "//--------------------------------------------------------------------\\" << endl << endl;
	}

	bool canPromote()
	{
		//------ Checks to see if any cell only has 1 choice, if so return true
		for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		if (puzzle_cells[i][j].promote_possible_choice())
			return true;
		return false;
	}

	bool simplify_puzzle()
	{
		set<int> sCurrent;
		//cout << "Simplify_puzzle()" << endl;

		//------ simplify across
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (puzzle_cells[i][j].nCurrent != 0)
					sCurrent.insert(puzzle_cells[i][j].nCurrent);
				if (j == 8)// if we're at the end of the colum
				for (int k = 0; k < 9; k++)
					puzzle_cells[i][k].eliminate_choice(sCurrent);
			}
			sCurrent.clear();
		}

		//------ simplify in a line
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (puzzle_cells[j][i].nCurrent != 0)
					sCurrent.insert(puzzle_cells[j][i].nCurrent);
				if (j == 8)// if we're at the end of the row
				for (int k = 0; k < 9; k++)
					puzzle_cells[k][i].eliminate_choice(sCurrent);
			}
			sCurrent.clear();
		}

		//------ simplify in a box
		int nBound[] = { 0, 3, 6 }; // Defines the boundaries for each box. The permutation of 0,3,6 w/ it self will form the pts of the upper left coordinate of each box
		int x, y;

		for (int nX = 0; nX < 3; nX++)
		{
			for (int nY = 0; nY < 3; nY++)
			{//----------------------------------------------- This does the permutation for the boundaries and sets x and y per loop
				x = nBound[nX];
				y = nBound[nY];

				sCurrent.clear();

				for (int i = x; i < x + 3; i++)
				{
					for (int j = y; j < y + 3; j++)
					{
						if (puzzle_cells[i][j].nCurrent != 0)
							sCurrent.insert(puzzle_cells[i][j].nCurrent);
						if (i == x + 2 && j == y + 2)// if we're at the end of the box
						for (int k = x; k < x + 3; k++)
						for (int b = y; b < y + 3; b++)
							puzzle_cells[k][b].eliminate_choice(sCurrent);
					}
				}
			}
		}
		//	print_puzzle();
		return canPromote();
	}

	bool hidden_singles()
	{
		//	cout << "hidden_singles()" << endl;

		//horizontally
		for (int i = 0; i < 9; i++)
		{
			int buffer[10] = { 0 };

			for (int j = 0; j < 9; j++)
			{
				if (puzzle_cells[i][j].nCurrent == 0)
				for (set<int>::iterator it = puzzle_cells[i][j].sChoices.begin(); it != puzzle_cells[i][j].sChoices.end(); ++it)
					buffer[*it]++;
				if (j == 8)// if we're at the end of the colum
				{
					for (int b = 0; b < 10; b++)
					if (buffer[b] == 1)
					for (int k = 0; k < 9; k++)
					if (puzzle_cells[i][k].sChoices.count(b))
					{
						puzzle_cells[i][k].sChoices.clear();
						puzzle_cells[i][k].sChoices.insert(b);
					}

				}
			}
		}

		// vertically
		for (int i = 0; i < 9; i++)
		{
			int buffer[10] = { 0 };

			for (int j = 0; j < 9; j++)
			{
				if (puzzle_cells[j][i].nCurrent == 0)
				for (set<int>::iterator it = puzzle_cells[j][i].sChoices.begin(); it != puzzle_cells[j][i].sChoices.end(); ++it)
					buffer[*it]++;
				if (j == 8)// if we're at the end of the colum
				{
					for (int b = 0; b < 10; b++)
					if (buffer[b] == 1)
					for (int k = 0; k < 9; k++)
					if (puzzle_cells[k][i].sChoices.count(b))
					{
						puzzle_cells[k][i].sChoices.clear();
						puzzle_cells[k][i].sChoices.insert(b);
					}

				}
			}
		}

		// boxes
		int nBound[] = { 0, 3, 6 }; // Defines the boundaries for each box. The permutation of 0,3,6 w/ it self will form the pts of the upper left coordinate of each box
		int x, y;

		for (int nX = 0; nX < 3; nX++)
		{
			for (int nY = 0; nY < 3; nY++)
			{//----------------------------------------------- This does the permutation for the boundaries and sets x and y per loop
				x = nBound[nX];
				y = nBound[nY];

				int buffer[10] = { 0 };

				for (int i = x; i < x + 3; i++)
				{
					for (int j = y; j < y + 3; j++)
					{
						if (puzzle_cells[i][j].nCurrent == 0)
						for (set<int>::iterator it = puzzle_cells[i][j].sChoices.begin(); it != puzzle_cells[i][j].sChoices.end(); ++it)
							buffer[*it]++;

						if (i == x + 2 && j == y + 2)// if we're at the end of the box
						for (int a = 0; a < 10; a++)
						if (buffer[a] == 1)
						{
							for (int k = x; k < x + 3; k++)
							for (int b = y; b < y + 3; b++)
							if (puzzle_cells[k][b].sChoices.count(a))
							{
								puzzle_cells[k][b].sChoices.clear();
								puzzle_cells[k][b].sChoices.insert(a);
							}
						}
					}
				}
			}
		}

		return canPromote();
	}
	bool isSolved()
	{
		for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		if (puzzle_cells[i][j].nCurrent == 0)
			return false;
		return true;
	}
	void solve()
	{
		if (simplify_puzzle())
			solve();
	}
	bool can_simplify()
	{
		for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		if (puzzle_cells[i][j].nCurrent == 0)
			return true;
		return false;
	}
	point first_available_cell()
	{
		point available_cell;
		available_cell.x = -1;
		int smallest = 9; 
		for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		if (puzzle_cells[i][j].nCurrent == 0 && puzzle_cells[i][j].sChoices.size() != 0)
		{
			if (available_cell.x = -1)
			{
				available_cell.x = i;
				available_cell.y = j;
				smallest = puzzle_cells[i][j].sChoices.size();
			}
			else if (puzzle_cells[i][j].sChoices.size() < smallest)
			{
				available_cell.x = i;
				available_cell.y = j;
				return available_cell;
				smallest = puzzle_cells[i][j].sChoices.size();
			}
		}

		return available_cell;
	}
	void guess_available_cell(point guess_point)
	{
		puzzle_cells[guess_point.x][guess_point.y].nCurrent = *(puzzle_cells[guess_point.x][guess_point.y].sChoices.begin());
		puzzle_cells[guess_point.x][guess_point.y].sChoices.clear();

	}
	void guess_available_alternative(point guess_point)
	{
		puzzle_cells[guess_point.x][guess_point.y].sChoices.erase(*(puzzle_cells[guess_point.x][guess_point.y].sChoices.begin()));
	}
	bool noZeroContradiction()
	{
		for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		if (puzzle_cells[i][j].nCurrent == 0 && puzzle_cells[i][j].sChoices.size() == 0)
		{
			return false;
		}
		return true;
	}
};

bool guess(puzzle* myPuzzle, puzzle* Alternative)
{
	*Alternative = *myPuzzle;
	point guess_point = myPuzzle->first_available_cell();
	if (guess_point.x != -1)
	{
		myPuzzle->guess_available_cell(guess_point);
		Alternative->guess_available_alternative(guess_point);
	}
	return myPuzzle->noZeroContradiction();
}
int main(int argc, char *argv[])
{
	list<puzzle> solutions;
	stack<puzzle> alternatives;

	puzzle myPuzzle;
	myPuzzle.get_puzzle();

	alternatives.push(myPuzzle);

	bool simplificationFound;

	while (!alternatives.empty())
	{
		myPuzzle = alternatives.top();
		alternatives.pop();

		myPuzzle.solve();
		simplificationFound = true;
		while (myPuzzle.can_simplify() && simplificationFound)
		{
			simplificationFound = myPuzzle.hidden_singles();

			if (!simplificationFound)
			{
				puzzle alternative;
				if (simplificationFound = guess(&myPuzzle, &alternative)) // if the guessed puzzle can be simplified
				{
					alternatives.push(alternative);
				}
			}
			if (simplificationFound)
			{
				myPuzzle.solve();
			}
		}

		if (myPuzzle.isSolved())
		{
			solutions.push_back(myPuzzle);
		}
	}
	if (solutions.size() != 0) {
		for (list<puzzle>::iterator it = solutions.begin(); it != solutions.end(); ++it)
		{
			it->print_puzzle();
		}
		return 0;
	}
	else
	{
		cout << "No solutions.\n";
	}
	return 0;
}
