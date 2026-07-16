#include "SmeltSpace.h"
#include <iostream>

using namespace SmeltSpace;
using namespace std;

int main(int argc, char** argv)
{
    // check if the user provided all the right command line arguments
    if(argc != 7)
    {
        cerr << "Please run application with " << argv[0] << " <TotalRows> <TotalCols> <Number of Ores> "
                "<Amount of coal> <PlayerRow> <PlayerCol>" << endl;
        exit(ERR_ARGC);
    }

    // converting all the string arguments to integers
    int NumRows = ConvertToInt(argv[1]);
    int NumCols = ConvertToInt(argv[2]);
    int NumberOfCores = ConvertToInt(argv[3]);
    int Amount_Coal = ConvertToInt(argv[4]);
    int PRow = ConvertToInt(argv[5]);
    int PCol = ConvertToInt(argv[6]);

    // making sure the numbers are within the allowed limits
    Arg_Check(NumRows, ENV_MIN, ENV_MAX);
    Arg_Check(NumCols, ENV_MIN, ENV_MAX);
    Arg_Check(NumberOfCores, MIN_ORES, MAX_ORES);

    // make sure player doesn't start outside the board
    Arg_Check(PRow, 0, NumRows - 1);
    Arg_Check(PCol, 0, NumCols - 1);

    // seed the random number generator so things change each time
    srand(time(nullptr));

    // variables for the main loop
    char Input = '\0';
    bool blnContinue = true;
    GameStructure stcGame;

    // set up everything before we start playing
    stcGame = SetUpGame(NumRows, NumCols, PCol, PRow, Amount_Coal);

    // clear the screen once before the loop starts
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    // main game loop
    do
    {
        PrintGame(stcGame);

        // show the player's inventory
        cout << "\n--- INVENTORY ---" << endl;
        cout << "Tin Ore: " << stcGame.Tin_Ore << endl;
        cout << "Copper Ore: " << stcGame.Copper_Ore << endl;
        cout << "Refined Bronze: " << stcGame.refined_bronze << " / " << Amount_Coal << endl;
        cout << "Mining Equipment: " << stcGame.Mining_Euip << endl;
        cout << "-----------------" << endl;

        // check if the user has won the game
        if (stcGame.refined_bronze >= Amount_Coal)
        {
            cout << "\nYOU WON! You successfully smelted all the bronze!" << endl;
            stcGame.GameStatus = WON;
            blnContinue = false;
            break; // exit the loop
        }

        // get the next move
        cout << "Enter move (W/A/S/D) or Q to quit: ";
        cin >> Input;

        // update the player's position based on what they pressed
        MovePlayer(stcGame, Input, blnContinue);

        // check if they quit
        if (!blnContinue)
        {
            cout << "\nYou quit the game. Better luck next time!" << endl;
            stcGame.GameStatus = QUIT;
        }

    } while(blnContinue);

    // clean up the 2D array memory before the program ends so we don't get memory leaks
    DeallocMem(stcGame.arrGame, stcGame.intRows);

    return SUCCESS;
}
