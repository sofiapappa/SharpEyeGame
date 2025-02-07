// unfumble game:

// when invoked it will read a bunch of:
// 1-letter , 2-letter ..,8-letter words into vectors of words
// it will pick a random word with a length determined by game level
// jumble it and present it to the user with the prompt:
// "unjumble the above words" (the sooner you do the more points you get)
// score is determined by the amount of time the user took to unjumble the word if it s right
// using the equation: score = 20 * exp(-0.1*t) where t is the number of seconds in 3 decimal places.
#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include <ctype.h>  //for toupper
#include <unistd.h> //for sleep

using namespace std;

const size_t MIN_LEVEL = 1; // 3 letter words
const size_t MAX_LEVEL = 5; // 8 letter words

string str_to_uppercase(string &str)
{ // What does the & do? (Important for Midterm)
    for (size_t i = 0; i < str.length(); i++)
        str[i] = toupper(str[i]);
}

// reading from the file words.txt
// read a list of words from filename and stuff them into the coressponding vectors
// word vecs. Specifically word of length n goes into word_vecs[n]
// word_vecs[0] is going to be empty
// for forum discussion-talk about the & below,vector of vectors and return type of read vecs
bool read_words_from_file(string filename, vector<vector<string>> &word_vecs)
{

    // open the input file - filename - and read words into the appropriate vectors
    ifstream ifs(filename);

    if (!ifs)
        return false;

    string word;
    while (getline(ifs, word))
    {
        size_t len = word.length();
        if (len >= word_vecs.size())
            word_vecs.resize(len + 1); // discuss this corner case and make sure it's correct
        str_to_uppercase(word);        // Discuss how we are just calling the function, and not setting equal to the result
        word_vecs[len].push_back(word);
    }

    // At this point our vectors should be filled with words from the file.
    // for(size_t i = 1; i < word_vecs.size(); i++)
    //    cout << "Read " << word_vecs[i].size() << " " << i << "-letter words." << endl;

    return true;
}

string jumble()
{
}
// returns false if the level was lost, true otherwise
// Discuss in the forums the weird signature (preparation for midterm), why const, why &?
bool play_level(size_t level, const vector<vector<string>> &word_vecs)
{
    cout << "Welcome to level " << level << " of Unfumble! " << endl;
    sleep(1);
    const vector<string> &words = word_vecs[level + 2]; //(This is an alias) Discuss aliases in forum, what and why?

    const string word = words[rand() % words.size()];
    string shuffled_word = jumble(word);

    cout << "Guess the word behind the following jumbled letters: " <<
}

int main()
{
    const string WORDS_FILE = "words.txt";

    srand((unsigned)time(nullptr));

    vector<vector<string>> word_vecs;
    size_t level = 1; // starting level 1 = 3-letter words

    // In versions of this game, you should try and fall back on an
    // internal word list when the file can't be opened (even if worse quality)
    if (!read_words_from_file("words.txt", word_vecs))
    {
        cerr << "Could not read from: " << WORDS_FILE << endl;
        return -1;
    }

    for (size_t i = MIN_LEVEL; i <= MAX_LEVEL; i++)
    {
        if (play_level(i, word_vecs))
        {
            if (i == MAX_LEVEL)
            {
                cout << "No more levels" << endl;
                break; // enzo :)
            }
            cout << "Well done! On to the next level: " << (i + 1) << endl;
            sleep(1);
        }
    }

    cout << "Thank you playing unfumble!" << endl;

    return 0;
}