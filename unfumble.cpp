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

using namespace std;

bool read_words_from_file(string filename, vector<vector<string>> &word_vecs)
{

    // open the input file - filename - and read words into the appropriate vectors
    ifstream ifs(filename);

    if (!ifs)
    {
        cerr << "Could not open " << filename << endl;
        return false;
    }

    string word;
    while (getline(ifs, word))
    {
        size_t len = word.length();
        if (len >= word_vecs.size())
            word_vecs.resize(len + 1); // discuss this corner case and make sure it's correct
        word_vecs[len].push_back(word);
    }

    // At this point our vectors should be filled with words from the file.
    for (size_t i = 1; i < word_vecs.size(); i++)
        cout << "Read " << word_vecs[i].size() << " " << i << "-letter words." << endl;

    return true;
}

int main()
{
    vector<vector<string>> word_vecs;

    read_words_from_file("words.txt", word_vecs);

    return 0;
}