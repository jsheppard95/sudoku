//
//  sudoku.cpp
//  sudoku
//
//  Created by Jackson Sheppard on 8/7/16.
//  Copyright © 2016 Jackson Sheppard. All rights reserved.
//
//
// Sudoku filename: /Users/JacksonSheppard/Desktop/sudoku_input.txt
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <iomanip>
#include <locale>
using namespace std;

long int GAME_SIZE = 9;

void openFile(ifstream& infile, string filename)
{
    // Open's sudoku input file
    infile.open(filename, ios::in);
    
    // Check if file opened:
    if(infile.fail())
    {
        cout << "Could not open the file.\n";
        exit(0);
    }
}


void displayBoard(ifstream& infile, string filename)
{
    // Prints initial gameboard to console, libe by line
    openFile(infile, filename);
    //Output initial puzzle, line by line:
    cout << "Gameboard:\n";
    string line;
    while( getline(infile, line) )
    {
        cout << line << endl;
    }
    // Close file after displaying the initial puzzle:
    infile.close();
}

void fillVector(vector<vector<int>>& gameboard, vector<int>& cols, ifstream& infile, string filename)
{
    // Reads the sudoku gameboard and fills a 2D vector
    // 'gameboard', blankspaces = '0'.
    
    // Re-open file to read from beginning
    openFile(infile, filename);
    string line;
    int number;
    while( getline(infile, line) )
    {
        for(int i = 0; i < line.length(); i++)
        {
            string temp = ""; //Convert char to string to use string class
            temp += line[i];
            if(temp == " ") // Empty space designated by 0
            {
                cols.push_back(0);
            }
            else if( (temp == "-") || (temp == "|") ) // Border of gameboard
            {
                ; //Only want digits for 2D vector, ignore border
            }
            else // Character is an integer from 1-9
            {
                number = stoi(temp);
                cols.push_back(number);
            }
        }
        if( cols.size() != 0 )
        {
            // Prevents addition of empty vectors to gameboard
            gameboard.push_back(cols);
        }
        cols.clear();
    }
}

// Need to fill each blank space with valid number.
// 1-9 should appear in each row, column, and 3x3 box once.
//
// Checks each entry in a 'row' for 'poss_value'
// Returns true if 'poss_value' is in 'row' -> not a valid solution
// Returns false if 'poss_value' does not appear in 'row'
bool inRow(vector<vector<int>>& gameboard, int row, int poss_value)
{
    for(int col = 0; col < GAME_SIZE; col++) // Iterate through columns to check a row
    {
        if( gameboard[row][col] == poss_value )
        {
            return true;
        }
    }
    // possible value not found in row, therefore
    // return false
    return false;
}

// Checks each entry in a 'col' for 'poss_value'
// Returns true if 'poss_value' is in 'col' -> not a valid solution
// Returns false if 'poss_value' does not appear in 'col'
bool inCol(vector<vector<int>>& gameboard, int col, int poss_value)
{
    for(int row = 0; row < GAME_SIZE; row++) // iterate through rows to check a column
    {
        if( gameboard[row][col] == poss_value )
        {
            return true;
        }
    }
    return false;
}

// Checks a 3x3 or 2x2 box for 'poss_value'
// Returns true if 'poss_value' is in box -> not a valid solution
// Returns false if 'poss_value' does not appear in box.
// Assume inBox will be called from the top left corner of each box
bool inBox(vector<vector<int>>& gameboard, int top_left_Row, int top_left_Col, int poss_value)
{   // 3x3 Box: GAME_SIZE = 9:
    // 'top_left_Row' = 0, 'top_left_Col' = 0; 'top_left_Row' = 3, 'top_left_Col' = 6...
    // 'top_left_Row' and 'top_left_Col' = multiples of 3
    if( GAME_SIZE == 9 )
    {
        for(int i = 0; i < 3; i++) // iterates through remaining entries in box
        {
            for(int j = 0; j < 3; j++)
            {
                if( gameboard[top_left_Row + i][top_left_Col + j] == poss_value )
                {
                    return true;
                }
            }
            // If a match is not found in box,
            // function returns false
        }
        return false;
    }
    // 2x2 Box: GAME_SIZE = 4:
    // 'top_left_Row' = 0, 'top_left_Col' = 0; 'top_left_Row' = 2, 'top_left_Col' = 4...
    // 'top_left_Row' and 'top_left_Col' = multiples of 2
    else
    {
        for(int i = 0; i < 2; i++) // iterates through remaining entries in box
        {
            for(int j = 0; j < 2; j++)
            {
                if( gameboard[top_left_Row + i][top_left_Col + j] == poss_value )
                {
                    return true;
                }
            }
            // If a match is not found in the 3x3 box,
            // function returns false
        }
        return false;
    }
}

// Returns true if 'poss_value' can be placed at 'row' and 'col'
// Returns false otherwise -> not a valid solution
bool isValid(vector<vector<int>>& gameboard, int row, int col, int poss_value)
{
    // To be valid: inRow(), inCol(), and inBox() return false
    // 3x3 Box: inBox() called with 'row'/'col increments of 3 -> (integer % 3)
        // (multiples of 3) return 0: 0%3=0, 3%3=0, 6%3=0
        // (multiples of 3) + 1 return 1: 1%3=1, 4%3=1, 7%3=1
        // (multiples of 3) + 2 return 2: 2%3=2, 5%3=2, 8%3=2
        // Therefore: (integer - integer%3)
            // 0-0%3=0, 1-1%3=0, 2-2%3=0, 3-3%3=3, 4-4%3=3, 5-5%3=3, 6-6%3=6...
    // 'row' - 'row'%3 and 'col' - 'col'%3 = 'top_left_Row' and 'top_left_Col' of box
    if( GAME_SIZE == 9 )
    {
        if( (inRow(gameboard, row, poss_value) == false) && (inCol(gameboard, col, poss_value) == false) && (inBox(gameboard, row - row%3, col - col%3, poss_value) == false) )
        {
            return true; // 'poss_value' is valid to place in 'row'/'col'
        }
        else
        {
            return false;
        }
    }
    // 2x2 Box: inBox() called with 'row'/'col' increments of 2 -> (integer % 2)
        // 0%2=0, 1%2=1, 2%2=0, 3%2=1
        // (integer - integer%2)
        // 0-0%2=0, 1-1%2=0, 2-2%2=2, 3-3%2=2...
    // 'row' - 'row'%2 and 'col' - 'col'%2 = 'top_left_Row' and 'top_left_Col' of box
    else
    {
        if( (inRow(gameboard, row, poss_value) == false) && (inCol(gameboard, col, poss_value) == false) && (inBox(gameboard, row - row%2, col - col%2, poss_value) == false) )
        {
            return true; // 'poss_value' is valid to place in 'row'/'col'
        }
        else
        {
            return false;
        }
    }
}

bool find_next_Zero(vector<vector<int>>& gameboard, int& row, int& col)
{
    // Returns true if a 0 (designating blankspace) is present in gameboard
    // Returns fals if no 0's present (puzzle solved)
    // Assigns indeces of 0 to 'row' and 'col'
    for(int i = 0; i < GAME_SIZE; i++)
    {
        for(int j = 0; j < GAME_SIZE; j ++)
        {
            if( gameboard[i][j] == 0 )
            {
                row = i;
                col = j;
                return true; // Function ends, 'row'/'col' assigned
            }
        }
    }
    return false;
}

void display_poss_values(vector<vector<int>>& gameboard)
{
    // Prints the possible values for each 0 given the initial
    // setup of 'gameboard)
    for(int i = 0; i < GAME_SIZE; i++)
    {
        cout << "|   "; // Begin each line with "|" character
        for(int j = 0; j < GAME_SIZE; j++)
        {
            if( gameboard[i][j] == 0 )
            {
                cout << "( ";
                for(int k = 1; k <= GAME_SIZE; k++)
                {
                    // Output all valid entries at 'row'=i,'col'=j
                    if( isValid(gameboard, i, j, k) == true ) // Check each integer [1, GAME_SIZE] to see if valid at
                    {                                         // 'row'=i,'col'=j
                        cout << k << " ";
                        
                    }
                }
                cout << ")";
            }
            else
            {
                cout << "( " << gameboard[i][j] << " )";    // If value fixed, output value
            }
            // Organization of "|" for GAME_SIZE = 9
            if( GAME_SIZE == 9 )
            {
                if( (j == 2) || (j == 5) )
                {
                    cout << "   |   ";
                }
                if( j == 8 )
                {
                    cout << "   |" << endl;
                }
            }
            // Organization of "|" for GAME_SIZE = 4
            else
            {
                if( j == 1 )
                {
                    cout << "   |   ";
                }
                if( j == 3 )
                {
                    cout << "   |" << endl;
                }
            }
        }
        // Organization of " " for GAME_SIZE = 9
        if( GAME_SIZE == 9 )
        {
            if( (i == 2) || (i == 5) || (i == 8) )
            {
                cout << endl;
            }
        }
        // Organization of " " for GAME_SIZE = 4
        else
        {
            if( (i == 1) || (i == 3) )
            {
                cout << endl;
            }
        }
    }
}

bool SolveSudoku(vector<vector<int>>& gameboard)
{
    // Uses functions find_next_Zero() and isValid() to
    // solve a sudoku puzzle organized as a 2D vector with 0's as empty spaces
    // Returns true if 'gameboard' has a solution
    // Returns false otherwise
    int row;
    int col;
    bool curr_zero;
    
    curr_zero = find_next_Zero(gameboard, row, col); // 'row'/'col' assigned to first 0 in matrix
    if( curr_zero == false ) // No more 0's in matrix
    {
        return true; // Puzzle is fully solved
    }
    for(int num = 1; num <= GAME_SIZE; num++)
    {
        // Try integers [1, GAME_SIZE] at each 0 in matrix
        if( isValid(gameboard, row, col, num) == true )
        {
            gameboard[row][col] = num;
            if( SolveSudoku(gameboard) == true )
            { //Program tries to evaluate SolveSudoku, but will only return true if there are no more 0's
              //Continues solving puzzle until no more 0's left and puzzle fully solved
                return true;
            }
            // If solveSudoku() tries all integers [1, GAME_SIZE] on 'row'/'col', it returns false
            // The base recursive call would then return false and that 'row'/'col is reset to 0
            // the next integer, 'num', is then tried and the recursion begins again
            gameboard[row][col] = 0;
        }
    }
    // If on a zero each integer [1, GAME_SIZE] leads to a failed solution:
    // Puzzle is unsolveble and function returns false
    // 'gameboard' matrix now filled with correct values
    return false;
    
}

void output_solved_Puzzle(vector<vector<int>>& gameboard, ofstream& outfile, string outfile_name)
{
    // Outputs solved sudoku in same format as input file
    // to both console and output file
    // GAME_SIZE = 9 Version
    outfile.open(outfile_name, ios::out);
    cout << "-------------" << endl;
    outfile << "-------------" << endl; // Top border
    for(int i = 0; i < GAME_SIZE; i++)
    {
        cout << "|";
        outfile << "|"; // border at start of each line
        for(int j = 0; j < GAME_SIZE; j++)
        {
            cout << gameboard[i][j];
            outfile << gameboard[i][j];
            // Organizing "|" border
            if( (j == 2) || (j == 5) || (j == 8) )
            {
                cout << "|";
                outfile << "|";
            }
            if( j == 8 )
            {
                cout << endl;
                outfile << endl;
            }
        }
        // Organizing "-" border
        if( (i == 2) || (i == 5) || (i == 8) )
        {
            cout << "-------------" << endl;
            outfile << "-------------" << endl;
        }
    }
    outfile.close();
}

void output_4by4_puzzle(vector<vector<int>>& gameboard, ofstream& outfile, string outfile_name)
{
    // GAME_SIZE = 4 Version of output_solved_Puzzle():
    outfile.open(outfile_name, ios::out);
    cout << "-------" << endl;
    outfile << "-------" << endl;
    for(int i = 0; i < GAME_SIZE; i++)
    {
        cout << "|";
        outfile << "|";
        for(int j = 0; j < GAME_SIZE; j++)
        {
            cout << gameboard[i][j];
            outfile << gameboard[i][j];
            if( (j == 1) || (j == 3) )
            {
                cout << "|";
                outfile << "|";
            }
            if( j == 3 )
            {
                cout << endl;
                outfile << endl;
            }
        }
        if( (i == 1) || (i == 3) )
        {
            cout << "-------" << endl;
            outfile << "-------" << endl;
        }
    }
    outfile.close();
}

int main()
{
    // Open file streams
    ifstream infile;
    ofstream outfile;
    
    // Input file name:
    string filename;
    cout << "Enter filename:\n";
    cin >> filename;
    
    // Start Program clock
    clock_t t;
    t = clock();
    
    // Display initial puzzle:
    cout << "Displaying Gameboard...\n";
    displayBoard(infile, filename);
    
    // Make gameboard from two vectors
    vector<vector<int>> gameboard;
    vector<int> cols;
    ifstream infile2; // new input stream for making vectors
    fillVector(gameboard, cols, infile2, filename);
    
    // Adjust GAME_SIZE: 9 or 4
    GAME_SIZE = gameboard.size();
    
    cout << endl;
    
    // Display possible Values:
    cout << "Possible Values:\n";
    display_poss_values(gameboard);
    
    // Solve Sudoku:
    cout << "Solving sudoku...\n";
    cout << endl;
    
    if( SolveSudoku(gameboard) == true )
    {
        // Puzzle is solveable, output the 'gameboard' matrix
        cout << "Puzzle finished!\n";
        cout << "Solved sudoku:" << endl;
    }
    else // All possible solutions tried given intial values
    {
        cout << "No Solution exists" << endl;
    }
    
    // Output solution:
    string outfile_name;
    outfile_name = filename.substr(0, filename.length() - 4) + "_ANSWERS.txt";
    
    if( GAME_SIZE == 9 )
    {
        output_solved_Puzzle(gameboard, outfile, outfile_name);
    }
    else
    {
        output_4by4_puzzle(gameboard, outfile, outfile_name);
    }
    cout << endl;
    t = clock() - t; // Stop timing program
    // Display time:
    cout << "Time taken: " << (float)t/CLOCKS_PER_SEC << " seconds" << endl;
    
    // Close files still open:
    infile2.close();
    outfile.close();
    return 0;
}
