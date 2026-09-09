#include "../include/kasiski.h"
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

map<string, vector<int>> find_repeated_patterns(
    const string& ciphertext, int length)
{
    map<string, vector<int>> patterns;

    for (int i = 0; i <= (int)ciphertext.size() - length; i++)
    {
        string pattern = ciphertext.substr(i, length);
        patterns[pattern].push_back(i);
    }

    // Remove patterns that occur only once
    for (auto it = patterns.begin(); it != patterns.end(); )
    {
        if (it->second.size() < 2)
            it = patterns.erase(it);
        else
            ++it;
    }

    return patterns;
}

vector<int> calculate_distances(const vector<int>& positions)
{
    vector<int> distances;

    for (int i = 0; i < (int)positions.size(); i++)
    {
        for (int j = i + 1; j < (int)positions.size(); j++)
        {
            distances.push_back(positions[j] - positions[i]);
        }
    }

    return distances;
}

vector<int> find_factors(int number)
{
    vector<int> factors;

    for (int i = 2; i <= number; i++)
    {
        if (number % i == 0)
            factors.push_back(i);
    }

    return factors;
}

int kasiski_analysis(const string& ciphertext)
{
    map<string, vector<int>> repeatedPatterns;
    repeatedPatterns = find_repeated_patterns(ciphertext, 3);

    map<int, int> factorCount;

    cout << "\n========== KASISKI ANALYSIS ==========\n";

    if (repeatedPatterns.empty())
    {
        cout << "No repeated patterns found.\n";
        return 1;
    }

    for (const auto& entry : repeatedPatterns)
    {
        cout << "Pattern: " << entry.first << " Positions: ";

        for (int pos : entry.second)
            cout << pos << " ";

        cout << "\n";

        vector<int> distances =
            calculate_distances(entry.second);

        cout << "Distances: ";

        for (int d : distances)
        {
            cout << d << " ";

            vector<int> factors = find_factors(d);

            for (int factor : factors)
            {
                if (factor <= 20)
                    factorCount[factor]++;
            }
        }

        cout << "\n";
    }

    cout << "\nPossible Key Lengths:\n";

    for (const auto& entry : factorCount)
    {
        cout << "Length " << entry.first
             << " -> " << entry.second
             << " occurrences\n";
    }

    int bestLength = 1;
    int bestCount = 0;

    for (const auto& entry : factorCount)
    {
        if (entry.second > bestCount)
        {
            bestCount = entry.second;
            bestLength = entry.first;
        }
    }

    cout << "\nEstimated Key Length: "
         << bestLength << "\n";

    return bestLength;
}
