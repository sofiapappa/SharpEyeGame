
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
#include <map>
#include<random>

using namespace std;

// Constants
const size_t ROWS = 25, COLS = 100;
const int MAX_LEVELS = 4;
const int TIME_LIMIT = 5;
const int MAX_NUMBER = 1000;

// Random number generator setup: variable declarations & initializations
std::random_device rd; // generates a random seed
std::mt19937 rng(rd()); // seed the  pseudo-random number generator (PRNG) with a truly random value

// Function to generate random numbers
std::vector<int> generateNumbers(int count) {
    std::uniform_int_distribution<int> dist(0, MAX_NUMBER); 
    std::vector<int> numbers(count);

    for (auto& num : numbers){ // num is a reference to each element
        num = dist(rng); //dist takes that random seed and maps it to a number between 1 and 100 uniformly
    }

    return numbers;
}

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

string formatDouble(double value, int precision = 2)
{
    ostringstream oss;
    oss << fixed << setprecision(precision) << value;
    return oss.str();
}

float play(int level, float level_score, float total_score)
{
    int max_level = 4;
    clear_screen();
    print_headers();
    update_scores(level_score, total_score);
    say_at(1, 0, "LEVEL: " + to_string(level));

    std::map<int, int> gameLevels; // Creating a dictionary (key: int, value: int)
    gameLevels[1] = 2;
    gameLevels[2] = 3;
    gameLevels[3] = 9;
    gameLevels[4] = 12;

    vector<size_t> nums(gameLevels[level]);
    for (size_t i = 0; i < nums.size(); ++i)
    {
        nums[i] = rand() % 1000;
    }
    display_num(nums);

    /*ostringstream oss;
    center_text_in_row(16, "Your numbers are: ");
    size_t current_row = 12;
    for (size_t i = 0; i < nums.size(); ++i)
    {
        oss << setw(3) << setfill('0') << nums[i] << " ";
        if ((i + 1) % 3 == 0)
        {
            center_text_in_row(current_row, oss.str());
            oss.str(" ");
            ++current_row;
        }
    }*/

    // Calculate the actual mean:
    float total = 0.0;
    for (size_t i = 0; i < nums.size(); ++i)
    {
        total += nums[i];
    }
    float mean = total / nums.size();

    unsigned ts1 = time(nullptr);
    center_text_in_row(16, "Guess the mean of the above numbers within 5 seconds: ");

    float guessed_mean;
    cin >> guessed_mean;

    unsigned ts2 = time(nullptr);
    if ((ts2 - ts1) > 5)
    { // Response took longer than 5 seconds
        center_text_in_row(12, "Sorry. You took " + to_string(ts2 - ts1) + "s, not 5 seconds.");
        return 0;
    }

    float abs_diff = fabs(double(guessed_mean - mean) / 500.0);
    float score = 10 / exp(0.5 * abs_diff);

    center_text_in_row(12, "You guessed " + formatDouble(guessed_mean) + ". The mean was " + formatDouble(mean));
    center_text_in_row(13, "You earned " + formatDouble(score) + " points");
    total_score += score;

    if (score < 0.1)
    {
        center_text_in_row(14, "Sorry! You didn't do well enough to advance to the next level");
        // center_text_in_row(13, "You guessed " + to_string(guessed_mean) + " instead of " + to_string(mean));
        exit(0);
    }
    if (score > 1 && score < 5)
    {
        center_text_in_row(14, "Let's try again in with the same level");
        sleep(3);
        play(level, score, total_score);
    }
    else
    {
        if (score > 5 && score < 9.9 && level < max_level)
        {
            center_text_in_row(14, "We can move on to the next level!");
            sleep(3);
            play(level + 1, score, total_score);
        }
        else
        {
            center_text_in_row(14, "We reached the end of the game! Good Job!");
        }
    }
    if (score > 9.9 && level < max_level)
    {
        center_text_in_row(14, "Hooray, You hit the nail on the head! You get 2 credits as bonus! Let s move on!");
        sleep(3);
        play(level++, score + 2, total_score);
    }
    else
    {
        center_text_in_row(14, "We reached the end of the game! Fantastic Job!");
    }

    return total_score;
}

// play_level_1 will present the user with the level 1 screen and return
// their score to the caller
float play_level_1(float level_score, float score_total)
{
    clear_screen();
    print_headers();
    update_scores(level_score, score_total);
    say_at(1, 0, "LEVEL: 1");

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

// Same as level 2, except 9 numbers presented in a 3x3 matrix
float play_level_3(float level_score, float total_score)
{
    clear_screen();
    print_headers();
    update_scores(level_score, total_score);
    say_at(1, 0, "LEVEL: 3");

    vector<size_t> nums(9);
    for (size_t i = 0; i < nums.size(); ++i)
    {
        nums[i] = rand() % 1000;
    }
    // Show the 9 numbers in a 3x3 matrix centered on screen
    ostringstream oss;
    center_text_in_row(16, "Your numbers are: ");
    size_t current_row = 12;
    for (size_t i = 0; i < nums.size(); ++i)
    {
        oss << setw(3) << setfill('0') << nums[i] << " ";
        if ((i + 1) % 3 == 0)
        {
            center_text_in_row(current_row, oss.str());
            oss.str(" ");
            ++current_row;
        }
    }

    // Calculate the actual mean:
    float total = 0.0;
    for (size_t i = 0; i < nums.size(); ++i)
    {
        total += nums[i];
    }
    float mean = total / nums.size();

    unsigned ts1 = time(nullptr);
    center_text_in_row(16, "Guess the mean of the above numbers within 5 seconds: ");

    float guessed_mean;
    cin >> guessed_mean;

    unsigned ts2 = time(nullptr);
    if ((ts2 - ts1) > 5)
    { // Response took longer than 5 seconds
        center_text_in_row(12, "Sorry. You took " + to_string(ts2 - ts1) + "s, not 5 seconds.");
        return 0;
    }
    float abs_diff = fabs(double(guessed_mean - mean) / 500.0);
    float score = 10 / exp(0.5 * abs_diff);

    if (score < 0.1)
    {
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
    size_t level = 1;
    float score = 0, total_score = 0;

    play(level, score, total_score);

    /*while (level <= max_level)
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
    }*/

    return 0;
}
