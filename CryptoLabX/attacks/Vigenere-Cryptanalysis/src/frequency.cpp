#include "../include/frequency.h"
#include <cmath>
#include <limits>

using namespace std;

double calculate_ic(const string& text)
{
    int n = text.size();

    if (n <= 1)
        return 0.0;

    int frequency[26] = {0};

    for (char c : text)
        frequency[c - 'A']++;

    int numerator = 0;

    for (int i = 0; i < 26; i++)
    {
        numerator += frequency[i] * (frequency[i] - 1);
    }

    return (double)numerator / (n * (n - 1));
}

vector<string> split_into_groups(
    const string& ciphertext, int keyLength)
{
    vector<string> groups(keyLength);

    for (int i = 0; i < (int)ciphertext.size(); i++)
    {
        groups[i % keyLength] += ciphertext[i];
    }

    return groups;
}

vector<int> frequency_analysis(const string& text)
{
    vector<int> frequency(26, 0);

    for (char c : text)
    {
        if (c >= 'A' && c <= 'Z')
            frequency[c - 'A']++;
    }

    return frequency;
}

int find_shift(const string& group)
{
    double englishFrequency[26] =
    {
        0.082, 0.015, 0.028, 0.043, 0.127,
        0.022, 0.020, 0.061, 0.070, 0.0015,
        0.0077, 0.040, 0.024, 0.067, 0.075,
        0.019, 0.00095, 0.060, 0.063, 0.091,
        0.028, 0.0098, 0.024, 0.0015, 0.020,
        0.00074
    };

    int n = group.size();

    if (n == 0)
        return 0;

    double bestScore = numeric_limits<double>::max();
    int bestShift = 0;

    for (int shift = 0; shift < 26; shift++)
    {
        int observed[26] = {0};

        // Decrypt using this shift
        for (char c : group)
        {
            int p = (c - 'A' - shift + 26) % 26;
            observed[p]++;
        }

        double chiSquare = 0.0;

        for (int i = 0; i < 26; i++)
        {
            double expected = englishFrequency[i] * n;

            if (expected > 0)
            {
                double difference =
                    observed[i] - expected;

                chiSquare +=
                    (difference * difference) / expected;
            }
        }

        if (chiSquare < bestScore)
        {
            bestScore = chiSquare;
            bestShift = shift;
        }
    }

    return bestShift;
}
