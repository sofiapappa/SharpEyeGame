#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <limits>

// Constants
const int MAX_LEVELS = 4;
const int TIME_LIMIT = 5;
const int MAX_NUMBER = 1000;

// Random number generator setup
std::random_device rd;
std::mt19937 rng(rd());

// Function to generate random numbers
std::vector<int> generateNumbers(int count)
{
    std::uniform_int_distribution<int> dist(0, MAX_NUMBER);
    std::vector<int> numbers(count);

    for (auto &num : numbers)
    {
        num = dist(rng);
    }

    return numbers;
}

// Calculate score based on guess accuracy
float calculateScore(float guess, float actual_mean)
{
    float abs_diff = std::abs((guess - actual_mean) / 500.0);
    return 10 / std::exp(0.5 * abs_diff);
}

// Display numbers in a formatted way
void displayNumbers(const std::vector<int> &numbers)
{
    std::cout << "Your numbers are: ";
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

// Get user input with basic error handling

// Play a single level of the game
float playLevel(int level, float current_total_score)
{
    // Determine number of numbers based on level
    int num_count = level * level + 1;

    // Generate random numbers
    std::vector<int> numbers = generateNumbers(num_count);

    // Display numbers
    displayNumbers(numbers);

    // Calculate actual mean
    float actual_mean = std::accumulate(numbers.begin(), numbers.end(), 0.0) / numbers.size();

    // Get user guess
    float time_taken;
    float user_guess = getUserGuess(time_taken);

    // Check time limit
    if (time_taken > TIME_LIMIT)
    {
        std::cout << "Sorry, you took " << time_taken << " seconds. Time limit is "
                  << TIME_LIMIT << " seconds." << std::endl;
        return 0.0;
    }

    // Calculate score
    float level_score = calculateScore(user_guess, actual_mean);

    // Display results
    std::cout << "Actual mean: " << std::fixed << std::setprecision(2) << actual_mean << std::endl;
    std::cout << "Your guess: " << user_guess << std::endl;
    std::cout << "Level Score: " << level_score << std::endl;

    return level_score;
}

// Main game loop
void playGame()
{
    float total_score = 0.0;
    int current_level = 1;

    while (current_level <= MAX_LEVELS)
    {
        std::cout << "\n--- Level " << current_level << " ---" << std::endl;

        float level_score = playLevel(current_level, total_score);
        total_score += level_score;

        // Level progression logic
        if (level_score > 9.9 && current_level < MAX_LEVELS)
        {
            std::cout << "Congratulations! Moving to next level." << std::endl;
            current_level++;
        }
        else if (level_score < 0.1)
        {
            std::cout << "Sorry, you didn't perform well enough to continue." << std::endl;
            break;
        }
        else if (current_level == MAX_LEVELS)
        {
            std::cout << "You've completed all levels!" << std::endl;
            break;
        }
    }

    std::cout << "\nGame Over!" << std::endl;
    std::cout << "Total Score: " << total_score << std::endl;
}

int main()
{
    playGame();
    return 0;
}