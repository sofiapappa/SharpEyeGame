
// In each level, it will present a bunch of numbers.
// and the player has to guess the mean of those numbers.
// The score is determined as follows:
//	* If the player took more than five seconds, the score is zero.
//	* Else, the score equals 10/e^-0.5x
//  In each level, the numbers displayed will progress in difficulty

#include <iostream>
#include <iomanip> // For setw() and setfill()
#include <sstream> //for ostringstream
#include <math.h>
#include <vector>
#include <chrono> //for sleep
#include <unistd.h>
#include <cstdlib>

using namespace std;

const size_t ROWS = 25, COLS = 100;

enum Color
{
    base = 0,
    black = 30,
    red,
    green,
    yellow,
    blue,
    magneta,
    cyan,
    white

};

enum BgColor
{
    bg_base = 0,
    bg_black = 40,
    bg_red,
    bg_green,
    bg_yellow,
    bg_blue,
    bg_magenta,
    bg_cyan,
    bg_white
};
void set_color(int text_color)
{
    // changes color using ansi numbers
    cout << "\033[" << text_color << "m";
}

void set_bg_color(BgColor bg_color, Color color)
{
    cout << "\033[" << bg_color << ";" << color << "m";
}

void reset_color() { cout << "\033[0m"; }

void say_at(size_t row, size_t col, string text)
{
    cout << "\033[" << row << ";" << col << "H";
    cout << text << flush;
}

void clear_screen()
{
    cout << "\033[2J" << flush;
}

void center_text_in_row(size_t row, string msg)
{
    // determine the padding on either side of msg
    size_t numspaces = COLS - msg.length();
    say_at(row, (numspaces / 2), msg);
}

void print_headers()
{
    set_bg_color(bg_base, yellow);
    center_text_in_row(4, "EENIE, MEANIE, MINEY, MOE");
    set_bg_color(bg_base, green);
    center_text_in_row(5, "Guess the numbers' mean below");
    set_bg_color(bg_base, blue);
    center_text_in_row(6, "You've got five seconds to make it go.");
}

void update_scores(float score, float total_score)
{
    string score_text = "PREVIOUS LEVEL SCORE: " + to_string(score);
    string total_score_text = "TOTAL SCORE: " + to_string(total_score);
    set_bg_color(bg_base, yellow);
    say_at(1, 80, score_text);
    say_at(2, 80, total_score_text);
    reset_color();
}

double generate_rand_nums(vector<size_t> &nums)
{
    double total = 0.0;

    // Get total of all nums in vector
    for (size_t i = 0; i < nums.size(); i++)
    {
        // nums[i] = 100; // For testing purposes
        nums[i] = rand() % 1000;
        total += nums[i];
    }

    return total;
}

void display_num(vector<size_t> nums)
{
    ostringstream oss; // Create an output string stream
    size_t row = 8;

    set_bg_color(bg_base, red);
    center_text_in_row(7, "Your numbers are: ");

    set_bg_color(bg_red, white);
    for (size_t i = 0; i < nums.size(); i++)
    {
        oss << " ";
        oss << setw(3) << setfill('0') << nums[i];
        oss << " ";

        // For levels 2 and up that use matrices
        if (nums.size() > 3 && (i + 1) % 3 == 0)
        {
            center_text_in_row(row, oss.str());
            oss.str("");
            oss.clear();
            row++;
        }
    }

    // For levels 0 and 1
    if (nums.size() < 4)
        center_text_in_row(row, oss.str());

    reset_color();
}

// play_level_0 will present the user with the level 0 screen and return
// their score to the caller
float play_level_1()
{
    size_t n1 = rand() % 1000;
    size_t n2 = rand() % 1000;

    // show the two numbers on line 5 with text centered and ask them to guess
    // research for reddit
    ostringstream oss;
    oss << "Your numbers are: " << setw(3) << setfill('0') << n1 << " " << n2;
    center_text_in_row(7, oss.str());

    float mean = ((float)n1 + n2) / 2.0;
    unsigned ts1 = time(nullptr);
    center_text_in_row(10, "Guess the mean of the above numbers within 5 seconds: ");

    float guessed_mean;
    cin >> guessed_mean;

    unsigned ts2 = time(nullptr);
    if ((ts2 - ts1) > 5)
    { // Response took longer than 5 seconds
        center_text_in_row(12, "Sorry. You took " + to_string(ts2 - ts1) + "s, not 5s.");
        return 0;
    }
    float abs_diff = fabs(double(guessed_mean - mean) / 500.0);
    float score = 10 / exp(0.5 * abs_diff);

    if (score < 0.1)
    {
        center_text_in_row(12, "Sorry! You didn't do well enough to advance to the next level");
        center_text_in_row(13, "You guessed " + to_string(guessed_mean) + " instead of " + to_string(mean));
        return 0;
    }
    if (score > 9.9)
    {
        center_text_in_row(14, "Hooray, You hit the nail on the head!");
    }

    center_text_in_row(13, "You guessed " + to_string(guessed_mean) + ". The mean was " + to_string(mean));
    center_text_in_row(14, "You earned " + to_string(score) + " points");

    sleep(3);
    return score;
}

// Same as level 0 except 9 numbers presented in a 3x3 matrix
// Same as level 0, but 3 numbers passed
float play_level_2(float level_score, float score_total)
{
    clear_screen();
    print_headers();
    update_scores(level_score, score_total);
    say_at(1, 0, "LEVEL: 2");

    size_t n1 = rand() % 1000;
    size_t n2 = rand() % 1000;
    size_t n3 = rand() % 1000;

    // Show the 3 numbers at line 5 with text centered and ask them to guess
    // (research for reddit)
    ostringstream oss;
    oss << "Your numbers are: " << setw(3) << setfill('0') << n1 << " " << n2
        << " " << n3;
    center_text_in_row(7, oss.str());

    float mean = ((float)n1 + n2 + n3) / 3.0;
    unsigned ts1 = time(nullptr);
    center_text_in_row(10, "Guess the mean of the above numbers within 5 seconds: ");

    float guessed_mean;
    cin >> guessed_mean;

    unsigned ts2 = time(nullptr);
    if ((ts2 - ts1) > 5)
    { // Response took longer than 5 seconds
        center_text_in_row(12, "Sorry. You took " + to_string(ts2 - ts1) + "s, not 5s.");
        return 0;
    }
    float abs_diff = fabs(double(guessed_mean - mean) / 500.0);
    float score = 10 / exp(0.5 * abs_diff);

    if (score < 0.1)
    {
        center_text_in_row(12, "Sorry, you guessed " + to_string(guessed_mean) + " but the actual mean was " + to_string(mean));
        center_text_in_row(12, "Sorry! You didn't do well enough to advance to the next level");
        center_text_in_row(13, "You guessed " + to_string(guessed_mean) + " instead of " + to_string(mean));
        exit(0);
    }
    if (score > 9.9)
    {
        center_text_in_row(14, "Hooray, You hit the nail on the head!");
    }

    center_text_in_row(13, "You guessed " + to_string(guessed_mean) + ". The mean was " + to_string(mean));
    center_text_in_row(14, "You earned " + to_string(score) + " points");

    sleep(3);
    return score;
}

int main()
{
    srand((unsigned)time(nullptr)); // Seed the random generator.

    // main() is going to call different play_level functions which will each
    // return the score of the player on completing that level.
    // If the score is 0 at any point or if max level has been reached,
    // main() will break out of the loop.
    size_t level = 1, max_level = 4;
    float score = 0, total_score = 0;

    while (level <= max_level)
    {
        switch (level)
        {
        case 1:
            score = play_level_1(score, total_score); // 2 numbers in 1 line
            break;
        case 2:
            score = play_level_2(score, total_score); // 3 numbers in 1 line
            break;
        case 3:
            score = play_level_3(score, total_score); // 9 numbers in a 3x3 matrix
            break;
        case 4:
            center_text_in_row(16, "Congrats. You are officially a Meanie. No more levels until CS2A students make them.");
            break;
        default:
            break;
        }

        total_score += score;
        update_scores(score, total_score);

        ++level;
    }
    return 0;
}
