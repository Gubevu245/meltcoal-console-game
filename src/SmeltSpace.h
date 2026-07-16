#ifndef LIBSMELTSPACE_H_INCLUDED
#define LIBSMELTSPACE_H_INCLUDED

#include <iostream>
#include <cctype>
#include <cmath>
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <cassert>
#include <iomanip>

using namespace std;

namespace SmeltSpace
{
    typedef int* OneDarray;
    typedef int** TwoDarray;

    // error codes for exiting the program
    enum enERRORS
    {
        ERR_CONV = -4,
        ERR_FAIL,
        ERR_ARGC,
        ERR_INVALID
    };

    // all the items that can be on the board
    enum enFEATURES
    {
        EMPTY,
        PLAYER,
        COPPER_ORE,
        TIN_ORE,
        SMELTER,
        PLAYERORE,
        MINING_EQUI
    };

    // game states
    enum enStatus
    {
        SUCCESS,
        LOST,
        WON,
        QUIT
    };

    // constants for validating user input
    const int ENV_MIN = 4;
    const int ENV_MAX = 18;
    const int MIN_ORES = 4;
    const int MAX_ORES = 12;

    // main structure to hold all game details
    struct GameStructure
    {
        TwoDarray arrGame;
        int intRows;
        int intCols;
        int PRow;
        int PCol;
        int Number_Coal;
        int Mining_Euip;
        int Tin_Ore;
        int Copper_Ore;
        int refined_bronze;
        enStatus GameStatus;
    };

    // symbols used to draw the map
    const char chFeatures[7] = {'.','P','#','@','S','P','O'};

    // function prototypes
    int ConvertToInt(string strNum);
    bool RangeCheck(int Number, int Min, int Max);
    void Arg_Check(int Number, int Min, int Max);
    TwoDarray AllocMem(int Rows, int Cols);

    // random number generator
    int GenRandom(int Lowest, int Highest);

    void DeallocMem(TwoDarray arrGame, int Rows);
    GameStructure SetUpGame(int Rows, int Cols, int PCol, int PRow, int Number_Coal);
    void PlaceFeatures(GameStructure& stcWorld, int Feature, int Count,
                       int StartRowFrom, int EndRow, int StartCol, int EndCol);
    void PrintGame(GameStructure& stcWorld);
    void Pause();

    // function to handle the user moving around
    void MovePlayer(GameStructure& stcWorld, char Direction, bool& blnContinue);
}

#endif // LIBSMELTSPACE_H_INCLUDED
