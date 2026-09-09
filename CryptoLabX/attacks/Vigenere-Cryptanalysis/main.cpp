
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

#include "include/preprocessing.h"
#include "include/kasiski.h"
#include "include/frequency.h"
#include "include/vigenere.h"
#include "include/verification.h"

using namespace std;

void display_ic_analysis(const string& ciphertext)
{
    cout << "\n===== INDEX OF COINCIDENCE =====\n";
    cout << "\nKey Length\tAverage IC\n";

    int bestLength = 1;
    double bestIC = 0.0;

    for (int keyLength = 1; keyLength <= 20; keyLength++)
    {
        vector<string> groups =
            split_into_groups(ciphertext, keyLength);

        double totalIC = 0.0;
        int count = 0;

        for (const string& group : groups)
        {
            if (group.size() > 1)
            {
                totalIC += calculate_ic(group);
                count++;
            }
        }

        double averageIC = 0.0;

        if (count > 0)
            averageIC = totalIC / count;

        cout << keyLength
             << "\t\t"
             << fixed
             << setprecision(4)
             << averageIC
             << "\n";

        if (averageIC > bestIC)
        {
            bestIC = averageIC;
            bestLength = keyLength;
        }
    }

    cout << "\nIC Estimated Key Length: "
         << bestLength << "\n";
}

int main()
{
    ifstream file("data/ciphertext.txt");

    if (!file)
    {
        cout << "Error opening data/ciphertext.txt\n";
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();

    string ciphertext = buffer.str();

    file.close();

    ciphertext = clean_ciphertext(ciphertext);

    cout << "=====================================\n";
    cout << " VIGENERE CIPHER CRYPTANALYSIS\n";
    cout << "=====================================\n";

    cout << "\nCiphertext Length: "
         << ciphertext.length()
         << "\n";

    if (ciphertext.empty())
    {
        cout << "Ciphertext is empty.\n";
        return 1;
    }

    int keyLength = kasiski_analysis(ciphertext);

    if (keyLength == 0)
    {
        cout << "\nKasiski could not determine "
                "the key length.\n";

        display_ic_analysis(ciphertext);

        return 1;
    }

    cout << "\nEstimated Key Length: "
         << keyLength
         << "\n";

    display_ic_analysis(ciphertext);

    vector<string> groups =
        split_into_groups(ciphertext, keyLength);

    cout << "\n===== CIPHERTEXT GROUPS =====\n";

    for (int i = 0; i < static_cast<int>(groups.size()); i++)
    {
        cout << "Group "
             << i + 1
             << ": "
             << groups[i]
             << "\n";
    }

    cout << "\n===== FREQUENCY ANALYSIS =====\n";

    for (int i = 0; i < static_cast<int>(groups.size()); i++)
    {
        cout << "\nGroup "
             << i + 1
             << "\n";

        cout << "Letter\tFrequency\n";

        vector<int> frequency =
            frequency_analysis(groups[i]);

        for (int j = 0; j < 26; j++)
        {
            cout << char('A' + j)
                 << "\t"
                 << frequency[j]
                 << "\n";
        }

        cout << "IC = "
             << fixed
             << setprecision(4)
             << calculate_ic(groups[i])
             << "\n";
    }

    string key = find_key(groups);

    cout << "\n===== RECOVERED KEY =====\n";

    cout << "Key: "
         << key
         << "\n";

    string plaintext =
        vigenere_decrypt(ciphertext, key);

    cout << "\n===== RECOVERED PLAINTEXT =====\n";

    cout << plaintext
         << "\n";

    bool verified =
        verify(ciphertext, plaintext, key);

    cout << "\n===== VERIFICATION =====\n";

    if (verified)
    {
        cout << "SUCCESS!\n";
        cout << "Re-encryption matches "
                "the original ciphertext.\n";
    }
    else
    {
        cout << "FAILED!\n";
        cout << "Re-encryption does not match "
                "the original ciphertext.\n";
    }

    return 0;
}
