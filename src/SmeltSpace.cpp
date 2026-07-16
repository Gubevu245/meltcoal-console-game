#include "SmeltSpace.h"

namespace SmeltSpace
{
    // converting string to int and exiting if it fails
    int ConvertToInt(string strNum)
    {
        int Num;
        stringstream ss{strNum};
        ss >> Num;
        if(ss.fail())
        {
            cerr << "Could not convert " << strNum << " to integer " << endl;
            exit(ERR_CONV);
        }
        return Num;
    }

    // simple check to see if number is in bounds
    bool RangeCheck(int Number, int Min, int Max)
    {
        return (Number >= Min && Number <= Max);
    }

    // pausing the screen so the user can read messages
    void Pause()
    {
        cout << "Press any key to continue " << endl;
        cin.ignore(100,'\n');
        cin.get();
    }

    // throwing an error if the numbers from argv are wrong
    void Arg_Check(int Number, int Min, int Max)
    {
        if(!RangeCheck(Number, Min, Max))
        {
            cerr << "Please enter a number between " << Min << " and " << Max << " For Number " << Number << endl;
            exit(ERR_INVALID);
        }
    }

    // generating a random number
    int GenRandom(int Lowest, int Highest)
    {
        assert(Highest >= Lowest);
        int Range = Highest - Lowest + 1;
        return (rand() % Range + Lowest);
    }

    // allocating memory for the 2D array board
    TwoDarray AllocMem(int Rows, int Cols)
    {
        TwoDarray arrGame;
        arrGame = new int*[Rows];
        for(int a = 0; a < Rows; a++)
        {
            arrGame[a] = new int[Cols];
        }

        // filling it with empty spaces first
        for(int a = 0; a < Rows; a++)
        {
            for(int c = 0; c < Cols; c++)
            {
                arrGame[a][c] = EMPTY;
            }
        }
        return arrGame;
    }

    // putting random items on the board
    void PlaceFeatures(GameStructure& stcWorld, int Feature, int Count,
                       int StartRowFrom, int EndRow, int StartCol, int EndCol)
    {
        for (int a = 0; a < Count; a++)
        {
            int Ran_Row = GenRandom(StartRowFrom, EndRow);
            int Ran_Col = GenRandom(StartCol, EndCol);

            // keep looking for a spot if it is not empty
            while(stcWorld.arrGame[Ran_Row][Ran_Col] != EMPTY)
            {
                Ran_Row = GenRandom(StartRowFrom, EndRow);
                Ran_Col = GenRandom(StartCol, EndCol);
            }
            stcWorld.arrGame[Ran_Row][Ran_Col] = Feature;
        }
    }

    // initializing everything at the start of the game
    GameStructure SetUpGame(int Rows, int Cols, int PCol, int PRow, int Number_Coal)
    {
        GameStructure stcWorld;
        stcWorld.arrGame = AllocMem(Rows, Cols);
        stcWorld.intRows = Rows;
        stcWorld.intCols = Cols;
        stcWorld.PRow = PRow;
        stcWorld.PCol = PCol;
        stcWorld.GameStatus = SUCCESS;
        stcWorld.Number_Coal = Number_Coal;

        // start inventory at zero
        stcWorld.Copper_Ore = 0;
        stcWorld.Tin_Ore = 0;
        stcWorld.refined_bronze = 0;
        stcWorld.Mining_Euip = 0;

        // place player
        stcWorld.arrGame[PRow][PCol] = PLAYER;

        // place the ores
        PlaceFeatures(stcWorld, TIN_ORE, Number_Coal, 0, stcWorld.intRows - 1, 0, stcWorld.intCols - 1);
        PlaceFeatures(stcWorld, COPPER_ORE, Number_Coal, 0, stcWorld.intRows - 1, 0, stcWorld.intCols - 1);

        // place buildings and equipment
        PlaceFeatures(stcWorld, SMELTER, 1, 0, stcWorld.intRows - 1, stcWorld.intCols - 2, stcWorld.intCols - 1);
        PlaceFeatures(stcWorld, MINING_EQUI, 1, stcWorld.intRows - 2, stcWorld.intRows - 1, 0, stcWorld.intCols - 1);

        return stcWorld;
    }

    // drawing the board
    void PrintGame(GameStructure& stcWorld)
    {
        // move the cursor to the top-left instead of clearing the whole screen.
        // this stops the screen from flashing/glitching every time the player moves.
        cout << "\033[H";

        for(int a = 0; a < stcWorld.intRows; a++)
        {
            for(int c = 0; c < stcWorld.intCols; c++)
            {
                int Feature = stcWorld.arrGame[a][c];
                cout << setw(3);
                cout << chFeatures[Feature];
            }
            cout << endl;
        }

        // add a few blank lines at the bottom to ensure old text gets overwritten nicely
        cout << "                                      \n";
        cout << "                                      \n";
    }
    // handling the WASD movement and item collection
    void MovePlayer(GameStructure& stcWorld, char Direction, bool& blnContinue)
    {
        int nextRow = stcWorld.PRow;
        int nextCol = stcWorld.PCol;

        // figure out where the user wants to step next
        switch(toupper(Direction))
        {
            case 'W': nextRow--; break;
            case 'S': nextRow++; break;
            case 'A': nextCol--; break;
            case 'D': nextCol++; break;
            case 'Q':
                blnContinue = false;
                return; // quit the loop
            default:
                return; // ignore wrong keys
        }

        // checking to make sure they don't walk off the array
        if(nextRow < 0 || nextRow >= stcWorld.intRows || nextCol < 0 || nextCol >= stcWorld.intCols)
        {
            return; // just do nothing if they hit the border
        }

        // see what is currently sitting in the next spot
        int currentFeature = stcWorld.arrGame[nextRow][nextCol];

        // add stuff to inventory if they walk over it
        if(currentFeature == TIN_ORE)
        {
            stcWorld.Tin_Ore++;
        }
        else if(currentFeature == COPPER_ORE)
        {
            stcWorld.Copper_Ore++;
        }
        else if(currentFeature == MINING_EQUI)
        {
            stcWorld.Mining_Euip++;
        }
        else if(currentFeature == SMELTER)
        {
            // convert ores to bronze if we have both
            if(stcWorld.Tin_Ore > 0 && stcWorld.Copper_Ore > 0)
            {
                stcWorld.refined_bronze++;
                stcWorld.Tin_Ore--;
                stcWorld.Copper_Ore--;
            }
            // don't let them walk over the smelter and delete it, just return
            return;
        }

        // leave the old spot empty
        stcWorld.arrGame[stcWorld.PRow][stcWorld.PCol] = EMPTY;

        // update to the new spot
        stcWorld.PRow = nextRow;
        stcWorld.PCol = nextCol;

        // draw the player in the new spot
        stcWorld.arrGame[stcWorld.PRow][stcWorld.PCol] = PLAYER;
    }

    // freeing up memory at the end
    void DeallocMem(TwoDarray arrGame, int Rows)
    {
        assert(arrGame != nullptr);
        for (int a = 0; a < Rows; a++)
        {
            delete[] arrGame[a];
        }
        delete[] arrGame;
        arrGame = nullptr;
    }
}
