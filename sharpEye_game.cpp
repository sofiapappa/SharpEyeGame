
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono> // for usleep()
// #include <unistd>
#include <math.h>

using namespace std;

// this function accepts 3 parameters (row, col, text)
// it will write text at (row,col) on the screen

void say_at(size_t row, size_t col, string text)
{
    cout << "\033[" << row << ";" << col << "H";
    cout << text << flush;
}

// same as above except it will blink for vfx and dissapear at the end
// We will flash the string on and off repeatedly n times
// a frequency of usecs microsecs between flashes
void blink_at(size_t row, size_t col, string text, size_t times, size_t usecs)
{
    for (size_t i = 0; i < times; i++)
    {
        cout << "\033[" << row << ";" << col << "H";
        cout << text << flush;
        this_thread::sleep_for(chrono::microseconds(usecs));

        cout << "\033[" << row << ";" << col << "H";
        cout << string(text.length(), ' ') << flush;
        this_thread::sleep_for(chrono::microseconds(usecs));
    }
}

void clear_screen()
{
    cout << "\033[2J" << flush;
}

int main()
{
    // what is srand?; research and report.
    // experiment by commenting it out and seeing the effect
    srand((unsigned)time(nullptr));

    const size_t ROWS = 30, COLS = 120;
    const size_t SCORE_COL = 90; // At row 0, col 90 your score: ...
    const size_t LEVEL_COL = 0;
    const size_t PREV_TGT_COL = 5, PREV_GUESS_COL = 40, PREV_SCORE_COL = 80;
    size_t level = 0;
    double total_score = 0;

    while (level < 3)
    {
        clear_screen();

        say_at(0, LEVEL_COL, "CURRENT LEVEL: ");
        say_at(0, SCORE_COL, "SCORE: ");
        say_at(3, PREV_TGT_COL, "PREVIOUS TARGET: ");
        say_at(3, PREV_GUESS_COL, "PREVIOUS GUESS: ");
        say_at(3, PREV_SCORE_COL, "PREVIOUS SCORE: ");
        say_at(25, 0, "GUESS THE COLUMN NUMBER OF THE *: ");

        // choose a random row (0 to NUM_ROWS-3) and a random column (0 to NUM_COLS-1)
        // rand is a library function that returns a random number 2 bytes long (up to 32k)
        // do some research and talk about this in the forums
        // also find out about srand which is related

        size_t row = 5 + rand() % (ROWS - 10);
        size_t col = rand() % (COLS);

        // if (level == 0) {
        //     show_beginners_grid(); // For the very first level grid of equally spaced pluses is shown
        // }

        // Hide the cursor
        cout << "\033[?25l" << flush;

        blink_at(row, col, "+", 3, 1000 * 1000); // blink 3 times with a gap of 10^6 microsecs between blinks
        say_at(ROWS - 5, 0, "Guess the column number of the + sign: ");

        // Show the cursor
        cout << "\033[?25h" << flush;

        size_t guessed_column;
        string user_input;

        getline(cin, user_input);
        istringstream(user_input) >> guessed_column;

        // if the guessed column = col, the output message should be "Hooray! You hit the nail on the head!"
        // otherwise it is as constructed below

        size_t abs_diff = fabs(guessed_column - col);
        double score = 10.0 / exp(0.5 * abs_diff);
        total_score += score;
        say_at(0, 112, to_string(total_score)); // TODO: FIX FORMATING
        say_at(0, 15, to_string(level));
        say_at(3, 20, to_string(col));
        say_at(3, 55, to_string(guessed_column));
        say_at(3, 95, to_string(score));

        if (guessed_column == col)
        {
            string output = "Hooray! You hit the nail on the head!";
            say_at(ROWS - 3, 0, output);
        }
        else
        {
            say_at(ROWS - 3, 0, "                                                     ");
            string output = "You guessed column " + to_string(guessed_column) + ", but it was at column " + to_string(col);
            say_at(ROWS - 3, 0, output);
        }
        this_thread::sleep_for(chrono::microseconds(2000 * 1000));
        return 0; //// FORCING RETURN FOR DEBUG PURPOSES
    }

    return 0;
}
