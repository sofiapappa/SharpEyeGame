
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
#include <random>

// Constants
const size_t ROWS = 25, COLS = 100;
const int MAX_LEVELS = 4;
const int TIME_LIMIT = 5;
const int MAX_NUMBER = 1000;

// Random number generator setup: variable declarations & initializations
std::random_device rd;  // generates a random seed
std::mt19937 rng(rd()); // seed the  pseudo-random number generator (PRNG) with a truly random value

// Function to generate random numbers
std::vector<int> generateNumbers(int count)
{
    std::uniform_int_distribution<int> dist(0, MAX_NUMBER);
    std::vector<int> numbers(count);

    for (auto &num : numbers)
    {                    // num is a reference to each element
        num = dist(rng); // dist takes that random seed and maps it to a number between 1 and 100 uniformly
    }

    return numbers;
}

float calculateScore(float guess, float actual_mean)
{
    float abs_diff = std::abs(guess - actual_mean) / 500;
    return 10 / std::exp(0.5 * abs_diff);
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
    std::cout << "\033[" << text_color << "m";
}

void set_bg_color(BgColor bg_color, Color color)
{
    std::cout << "\033[" << bg_color << ";" << color << "m";
}

void reset_color() { std::cout << "\033[0m"; }

void say_at(size_t row, size_t col, std::string text)
{
    std::cout << "\033[" << row << ";" << col << "H";
    std::cout << text << std::flush;
}

void clear_screen()
{
    std::cout << "\033[2J" << std::flush;
}

void center_text_in_row(size_t row, std::string msg)
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

std::string formatDouble(double value, int precision = 2)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

void update_scores(float score, float total_score)
{
    std::string score_text = "PREVIOUS LEVEL SCORE: " + formatDouble(score);
    std::string total_score_text = "TOTAL SCORE: " + formatDouble(total_score);
    set_bg_color(bg_base, yellow);
    say_at(1, 80, score_text);
    say_at(2, 80, total_score_text);
    reset_color();
}
/ Display numbers in a formatted way void displayNumbers(const std::vector<int> &numbers)
{
    std::cout << "Your numbers are: ";
    set_bg_color(bg_red, white);
    for (size_t i = 0; i < numbers.size(); ++i)
    {
        std::cout << std::setw(4) << std::setfill('0') << numbers[i] << " ";
        if ((i + 1) % 3 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}

float getUserGuess(float &time_taken)
{
    float guess;
    auto start_time = std::chrono::steady_clock::now();

    std::cout << "Guess the mean within " << TIME_LIMIT << " seconds: ";

    // Input validation
    while (!(std::cin >> guess))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number: ";
    }

    auto end_time = std::chrono::steady_clock::now();
    time_taken = std::chrono::duration_cast<std::chrono::seconds>(
                     end_time - start_time)
                     .count();

    return guess;
}

float play(int level, float level_score, float total_score)
{
    int max_level = 4;
    clear_screen();
    print_headers();
    update_scores(level_score, total_score);
    say_at(1, 0, "LEVEL: " + std::to_string(level));

    std::map<int, int> gameLevels; // Creating a dictionary (key: int, value: int)
    gameLevels[1] = 2;
    gameLevels[2] = 3;
    gameLevels[3] = 9;
    gameLevels[4] = 12;

    std::vector<size_t> nums(gameLevels[level]);
    for (size_t i = 0; i < nums.size(); ++i)
    {
        nums[i] = rand() % 1000;
    }
    display_num(nums);

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
    std::cin >> guessed_mean;

    unsigned ts2 = time(nullptr);
    if ((ts2 - ts1) > 5)
    { // Response took longer than 5 seconds
        center_text_in_row(12, "Sorry. You took " + std::to_string(ts2 - ts1) + "s, not 5 seconds.");
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

int main()
{
    srand((unsigned)time(nullptr)); // Seed the random generator.

    size_t level = 1;
    float score = 0, total_score = 0;

    play(level, score, total_score);

    return 0;
}
