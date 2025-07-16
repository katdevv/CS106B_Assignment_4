/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function prototypes */

void welcome();
void giveInstructions();
void displayBoard(Grid<char>& board);
Grid<char> getCustomBoard(int dim);
Grid<char> shuffleBoard(const string cubes[], int dim);
void playersTurn(Grid<char>& board, Lexicon& lex, Set<string>& playersGuesses);
void computersTurn(Grid<char>& board, Lexicon& lex, Set<string>& playersGuesses);

/* Main program */

int main() {
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();
    giveInstructions();

    // Ask for dimentions
    cout << "You can choose standard Boggle (4x4 grid) or Big Boggle (5x5 grid)" << endl;
    string dimSpec = getLine("Would you like standard boggle? (y/n) ");
    int dim = (dimSpec == "y") ? 4 : 5;

    // Draw empty board
    drawBoard(dim, dim);

    // Ask for board configuration
    string boardSpec = getLine("Would you like to enter a custom board? (y/n) ");

    // Current board for the game
    Grid<char> board;

    if (boardSpec == "y") {
        board = getCustomBoard(dim);
    }
    else {
        if (dim == 4) {
            board = shuffleBoard(STANDARD_CUBES, dim);
        }
        else {
            board = shuffleBoard(BIG_BOGGLE_CUBES, dim);
        }
    }

    // Display board
    displayBoard(board);


    // Create lexicon
    Lexicon lex("EnglishWords.dat");

    // Player's turn
    cout << "Ok! Take all the time you want and find all the words you can! Signal that you're finished by entering an empty line." << endl;
    
    // Player's guesses
    Set<string> playersGuesses;

    playersTurn(board, lex, playersGuesses);

    // Computer's turn
    computersTurn(board, lex, playersGuesses);

    cout << "Can't beat silicon, can ya?" << endl;

    return 0;
}

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}

//// 1. Generate Board
// Get custom board input from player
Grid<char> getCustomBoard(int dim) {
    Grid<char> customBoard(dim, dim);
    int length = dim * dim;

    cout << "Enter a " << length << "-character string to identify which letters you want on the cubes." << endl;
    cout << "The first " << dim << " letters are the cubes on the top row from left to right, the next " << dim << " letters are the second row, and so on." << endl;

    while (true) {
        string input = getLine("Enter the string: ");

        if (input.size() >= length) {
            // Fill the board
            for (int row = 0; row < dim; row++) {
                for (int col = 0; col < dim; col++) {
                    customBoard[row][col] = toupper(input[row * dim + col]);
                }
            }

            break;
        }

        cout << "String must include " << length << " characters! Try again. ";
    }
    return customBoard;
}

// Shuffle the original cubes and save the board
Grid<char> shuffleBoard(const string cubes[], int dim) {
    // Pick spots for each cube
    Vector<string> newCubes;

    int length = dim * dim;
    
    // Copy the original cubes to vec
    for (int i = 0; i < length; i++) {
        newCubes.push_back(cubes[i]);
    }
    
    // Select spots and swap
    for (int i = 0; i < length; i++) {
        int spot = randomInteger(i, length - 1);
        swap(newCubes[spot], newCubes[i]);
    }

    // Roll each cube and save the board
    Grid<char> board(dim, dim);

    for (int row = 0; row < dim; row++) {
        for (int col = 0; col < dim; col++) {
            int side = randomInteger(0, 5);
            board[row][col] = newCubes[row * dim + col][side];
        }
    }

   return board;
}

// Display rolled cubes on the board
void displayBoard(Grid<char>& board) {
    for (int row = 0; row < board.nRows; row++) {
        for (int col = 0; col < board.nCols; col++) {
            labelCube(row, col, board[row][col]);
        }
    }
}

//// 2. Player's turn
bool isValidSequenceRec(Grid<char>& board, Grid<bool>& visited, Vector<pair<int, int>>& path, int row, int col, int index, string& word) {
    // If the end of the word reached, success
    if (index == word.size()) return true;

    // If invalid or already visited location or wrong character, failure
    if (!board.inBounds(row, col) ||
        board[row][col] != word[index] ||
        visited[row][col]) return false;

    // Mark location as visited and save
    visited[row][col] = true;
    path.push_back({ row, col });

    // Check next 8 directions
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x != 0 || y != 0) {
                if (isValidSequenceRec(board, visited, path, row + x, col + y, index + 1, word)) {
                    return true;
                }
            }
        }
    }

    // Backtrack
    visited[row][col] = false;
    path.remove(path.size() - 1);

    return false;
}

// Check if the word is a valid sequence on the board
bool isValidSequence(Grid<char>& board, string& word, Vector<pair<int, int>>& path) {
    // Remeber visited locations
    Grid<bool> visited(board.numRows(), board.numCols());

    string upperCaseWord = toUpperCase(word);

    // Search starting from each character
    for (int row = 0; row < board.numRows(); row++) {
        for (int col = 0; col < board.numCols(); col++) {
            path.clear();
            if (isValidSequenceRec(board, visited, path, row, col, 0, upperCaseWord))
                return true;
        }
    }
    return false;
}

void playersTurn(Grid<char>& board, Lexicon& lex, Set<string>& playersGuesses) {
    while (true) {
        string input = getLine("Enter a word: ");
        input = toLowerCase(input);

        // End turn
        if (input.empty()) break;

        // Initialize the correct path to get the word
        Vector<pair<int, int>> path;

        // Too short
        if (input.size() < 4) {
            cout << "I'm sorry, but that word doesn't meet the minimum word length." << endl;
        }
        // Invalid character sequence
        else if (!isValidSequence(board, input, path)) {
            cout << "You can't make that word!" << endl;
        }
        // Invalid word
        else if(!lex.contains(input)){
            cout << "That's not a word!" << endl;
        }
        // Repeated word
        else if (playersGuesses.contains(input)) {
            cout << "You already guessed that!" << endl;
        }
        else {
            // Add to player's guesses
            playersGuesses.add(input);

            // Highlight the letters
            for (pair<int, int> cube : path) {
                highlightCube(cube.first, cube.second, true);
            }

            // Pause
            pause(200);

            // Remove highlight
            for (pair<int, int> cube : path) {
                highlightCube(cube.first, cube.second, false);
            }
            
            // Display points for the word
            recordWordForPlayer(input, HUMAN);
        }
    }
}

//// 3. Computer's turn
void findAllWordsRec(Grid<char>& board, Lexicon& lex, Set<string>& words, int row, int col, string& curr, Grid<bool>& visited) {
    // If invalid or already visited location, failure
    if (!board.inBounds(row, col) || visited[row][col]) return;

    // Add char to the word
    curr += tolower(board[row][col]);

    // Stop if no word in lexicon starts with this prefix
    if (!lex.containsPrefix(curr)) {
        curr.pop_back();
        return;
    }
   
    // Add word if valid
    if (curr.size() >= 4 && lex.contains(toLowerCase(curr))) {
        words.add(curr);
    }

    // Mark location as visited
    visited[row][col] = true;

    // Check next 8 directions
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x != 0 || y != 0) {
                findAllWordsRec(board, lex, words, row + x, col + y, curr, visited);
            }
        }
    }

    // Backtrack
    visited[row][col] = false;
    curr.pop_back();
}

Set<string> findAllWords(Grid<char>& board, Lexicon& lex) {
    // Find all words from the board
    Set<string> words;

    // Search starting from each character
    for (int row = 0; row < board.numRows(); row++) {
        for (int col = 0; col < board.numCols(); col++) {
            // Remeber visited locations
            Grid<bool> visited(board.numRows(), board.numCols());

            // Current word
            string curr;

            findAllWordsRec(board, lex, words, row, col, curr, visited);
        }
    }

    return words;
}

void computersTurn(Grid<char>& board, Lexicon& lex, Set<string>& playersGuesses) {
    // Generate all the words from the board
    Set<string> allWords = findAllWords(board, lex);

    for (string word : allWords) {
        if (!playersGuesses.contains(word)) {
            // Display points for the word
            recordWordForPlayer(word, COMPUTER);
        }
    }
}