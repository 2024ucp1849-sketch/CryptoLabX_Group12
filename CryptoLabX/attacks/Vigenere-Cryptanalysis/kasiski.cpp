cat > src/kasiski.cpp <<'EOF'
#include "../include/kasiski.h"
#include <iostream>

using namespace std;

map<string, vector<int>>
find_repeated_patterns(
    const string& ciphertext,
    int length)
{
    map<string, vector<int>> patterns;

    if (length <= 0 ||
        ciphertext.size() < static_cast<size_t>(length))
        return patterns;

    for (int i = 0;
    int length = 3
         i <= static_cast<int>(ciphertext.size()) - length;
         i++)
    {
        string pattern =
            ciphertext.substr(i, length);

        patterns[pattern].push_back(i);
    }

    for (auto it = patterns.begin();
         it != patterns.end();)
    {
        if (it->second.size() < 2)
            it = patterns.erase(it);
        else
            ++it;
    }

    return patterns;
}

vector<int>
calculate_distances(
    const map<string, vector<int>>& patterns)
{
    vector<int> distances;

    for (const auto& entry : patterns)
    {
        const vector<int>& positions =
            entry.second;

        for (int i = 0;
             i < static_cast<int>(positions.size());
             i++)
        {
            for (int j = i + 1;
                 j < static_cast<int>(positions.size());
                 j++)
            {
                distances.push_back(
                    positions[j] - positions[i]
                );
            }
        }
    }

    return distances;
}

vector<int> find_factors(int number)
{
    vector<int> factors;

    if (number <= 0)
        return factors;

    for (int i = 2; i <= number; i++)
    {
        if (number % i == 0)
            factors.push_back(i);
    }

    return factors;
}

int kasiski_analysis(const string& ciphertext)
{
    map<int, int> factorCount;

    cout << "\n===== KASISKI ANALYSIS =====\n";

    for (int length = 3; length <= 5; length++)
    {
        auto patterns =
            find_repeated_patterns(
                ciphertext,
                length
            );

        cout << "\nRepeated patterns of length "
             << length << ":\n";

        if (patterns.empty())
        {
            cout << "No repeated patterns found.\n";
            continue;
        }

        for (const auto& entry : patterns)
        {
            cout << entry.first << " : ";

            for (int position : entry.second)
                cout << position << " ";

            cout << "\n";
        }

        vector<int> distances =
            calculate_distances(patterns);

        cout << "Distances: ";

        for (int distance : distances)
        {
            cout << distance << " ";

            vector<int> factors =
                find_factors(distance);

            for (int factor : factors)
            {
                if (factor <= 20)
                    factorCount[factor]++;
            }
        }

        cout << "\n";
    }

    cout << "\n===== POSSIBLE KEY LENGTHS =====\n";

    int bestLength = 0;
    int bestCount = 0;

    for (const auto& entry : factorCount)
    {
        cout << "Length "
             << entry.first
             << " -> "
             << entry.second
             << "\n";

        if (entry.second > bestCount)
        {
            bestCount = entry.second;
            bestLength = entry.first;
        }
    }

    return bestLength;
}
EOF
