cat > src/frequency.cpp <<'EOF'
#include "../include/frequency.h"

using namespace std;

double englishFrequency[26] =
{
    0.082, 0.015, 0.028, 0.043, 0.127,
    0.022, 0.020, 0.061, 0.070, 0.0015,
    0.0077, 0.040, 0.024, 0.067, 0.075,
    0.019, 0.00095, 0.060, 0.063, 0.091,
    0.028, 0.0098, 0.024, 0.0015, 0.020,
    0.00074
};

double calculate_ic(const string& text)
{
    int n = text.size();

    if (n <= 1)
        return 0.0;

    int frequency[26] = {0};

    for (char c : text)
    {
        if (c >= 'A' && c <= 'Z')
            frequency[c - 'A']++;
    }

    int numerator = 0;

    for (int i = 0; i < 26; i++)
    {
        numerator +=
            frequency[i] *
            (frequency[i] - 1);
    }

    return (double)numerator /
           (n * (n - 1));
}

vector<string>
split_into_groups(
    const string& ciphertext,
    int keyLength)
{
    vector<string> groups(keyLength);

    for (int i = 0;
         i < static_cast<int>(ciphertext.size());
         i++)
    {
        groups[i % keyLength] +=
            ciphertext[i];
    }

    return groups;
}

vector<int>
frequency_analysis(const string& group)
{
    vector<int> frequency(26, 0);

    for (char c : group)
    {
        if (c >= 'A' && c <= 'Z')
            frequency[c - 'A']++;
    }

    return frequency;
}

int find_shift(const string& group)
{
    int n = group.size();

    if (n == 0)
        return 0;

    int bestShift = 0;
    double bestScore = 1e100;

    for (int shift = 0;
         shift < 26;
         shift++)
    {
        int observed[26] = {0};

        for (char c : group)
        {
            int value =
                (c - 'A' - shift + 26) % 26;

            observed[value]++;
        }

        double score = 0.0;

        for (int i = 0; i < 26; i++)
        {
            double expected =
                n * englishFrequency[i];

            if (expected > 0)
            {
                double difference =
                    observed[i] - expected;

                score +=
                    (difference * difference) /
                    expected;
            }
        }

        if (score < bestScore)
        {
            bestScore = score;
            bestShift = shift;
        }
    }

    return bestShift;
}
EOF
