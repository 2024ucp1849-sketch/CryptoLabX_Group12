#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>

using namespace std;

#define ALPHABET_SIZE 26
#define MAX_WORDS 5000
#define MAX_WORD_LENGTH 100

// ------------------------------------------------------------
// Encrypt plaintext using encryption key
//
// encryptionKey[0] = ciphertext for plaintext A
// encryptionKey[1] = ciphertext for plaintext B
// ...
// ------------------------------------------------------------
string encryptText(const string &text, const string &encryptionKey)
{
    string result = "";

    for (int i = 0; i < (int)text.length(); i++)
    {
        char ch = text[i];

        if (isalpha((unsigned char)ch))
        {
            bool lower = islower((unsigned char)ch);

            int index = toupper((unsigned char)ch) - 'A';

            char encrypted = encryptionKey[index];

            if (lower)
                encrypted = tolower((unsigned char)encrypted);

            result += encrypted;
        }
        else
        {
            result += ch;
        }
    }

    return result;
}

// ------------------------------------------------------------
// Generate Random Monoalphabetic Substitution Key
//
// Example:
// ABCDEFGHIJKLMNOPQRSTUVWXYZ
// QWERTYUIOPASDFGHJKLZXCVBNM
//
// Every letter appears exactly once.
// ------------------------------------------------------------
string generateRandomKey()
{
    string key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Seed random number generator
    srand((unsigned int)time(NULL));

    // Fisher-Yates style shuffle
    for (int i = ALPHABET_SIZE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        char temp = key[i];
        key[i] = key[j];
        key[j] = temp;
    }

    return key;
}

// ------------------------------------------------------------
// Frequency Analysis
// ------------------------------------------------------------
void frequency_analysis(const string &ciphertext)
{
    int frequency[ALPHABET_SIZE] = {0};
    int totalLetters = 0;

    for (int i = 0; i < (int)ciphertext.length(); i++)
    {
        if (isalpha((unsigned char)ciphertext[i]))
        {
            char ch = toupper((unsigned char)ciphertext[i]);

            frequency[ch - 'A']++;
            totalLetters++;
        }
    }

    cout << "\n============================================\n";
    cout << "          FREQUENCY ANALYSIS\n";
    cout << "============================================\n";

    // Create index array
    int index[ALPHABET_SIZE];

    for (int i = 0; i < ALPHABET_SIZE; i++)
        index[i] = i;

    // Manual bubble sort
    for (int i = 0; i < ALPHABET_SIZE - 1; i++)
    {
        for (int j = 0; j < ALPHABET_SIZE - i - 1; j++)
        {
            if (frequency[index[j]] < frequency[index[j + 1]])
            {
                int temp = index[j];

                index[j] = index[j + 1];
                index[j + 1] = temp;
            }
        }
    }

    cout << "Letter\tCount\tPercentage\n";

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        int pos = index[i];

        if (frequency[pos] > 0)
        {
            double percentage =
                ((double)frequency[pos] / totalLetters) * 100.0;

            cout << char('A' + pos)
                 << "\t"
                 << frequency[pos]
                 << "\t"
                 << percentage
                 << "%\n";
        }
    }

    cout << "\nMost frequent ciphertext letters:\n";

    int shown = 0;

    for (int i = 0; i < ALPHABET_SIZE && shown < 5; i++)
    {
        if (frequency[index[i]] > 0)
        {
            cout << char('A' + index[i])
                 << " (" << frequency[index[i]] << ")  ";

            shown++;
        }
    }

    cout << "\n";
}

// ------------------------------------------------------------
// Extract Words Manually
// ------------------------------------------------------------
int extractWords(const string &text,
                 string words[],
                 int maxWords)
{
    string current = "";
    int count = 0;

    for (int i = 0; i <= (int)text.length(); i++)
    {
        char ch =
            (i < (int)text.length()) ? text[i] : ' ';

        if (isalpha((unsigned char)ch))
        {
            current += tolower((unsigned char)ch);
        }
        else
        {
            if (current.length() > 0 && count < maxWords)
            {
                words[count] = current;

                count++;

                current = "";
            }
        }
    }

    return count;
}

// ------------------------------------------------------------
// Word Frequency Analysis
// ------------------------------------------------------------
void word_frequency_analysis(const string &ciphertext)
{
    string words[MAX_WORDS];

    int count =
        extractWords(ciphertext, words, MAX_WORDS);

    string uniqueWords[MAX_WORDS];

    int frequencies[MAX_WORDS];

    int uniqueCount = 0;

    // Find unique words and frequencies
    for (int i = 0; i < count; i++)
    {
        int found = -1;

        for (int j = 0; j < uniqueCount; j++)
        {
            if (words[i] == uniqueWords[j])
            {
                found = j;
                break;
            }
        }

        if (found == -1)
        {
            uniqueWords[uniqueCount] = words[i];

            frequencies[uniqueCount] = 1;

            uniqueCount++;
        }
        else
        {
            frequencies[found]++;
        }
    }

    // Sort words according to frequency
    for (int i = 0; i < uniqueCount - 1; i++)
    {
        for (int j = 0;
             j < uniqueCount - i - 1;
             j++)
        {
            if (frequencies[j] < frequencies[j + 1])
            {
                int tempFreq = frequencies[j];

                frequencies[j] =
                    frequencies[j + 1];

                frequencies[j + 1] =
                    tempFreq;

                string tempWord =
                    uniqueWords[j];

                uniqueWords[j] =
                    uniqueWords[j + 1];

                uniqueWords[j + 1] =
                    tempWord;
            }
        }
    }

    cout << "\n============================================\n";
    cout << "        WORD FREQUENCY ANALYSIS\n";
    cout << "============================================\n";

    // One-letter words
    cout << "\nOne-letter words:\n";

    for (int i = 0; i < uniqueCount; i++)
    {
        if (uniqueWords[i].length() == 1)
        {
            cout << uniqueWords[i]
                 << " -> "
                 << frequencies[i]
                 << " times\n";
        }
    }

    // Two-letter words
    cout << "\nTwo-letter words:\n";

    for (int i = 0; i < uniqueCount; i++)
    {
        if (uniqueWords[i].length() == 2)
        {
            cout << uniqueWords[i]
                 << " -> "
                 << frequencies[i]
                 << " times\n";
        }
    }

    // Three-letter words
    cout << "\nThree-letter words:\n";

    for (int i = 0; i < uniqueCount; i++)
    {
        if (uniqueWords[i].length() == 3)
        {
            cout << uniqueWords[i]
                 << " -> "
                 << frequencies[i]
                 << " times\n";
        }
    }

    // Repeated words
    cout << "\nRepeated words:\n";

    for (int i = 0; i < uniqueCount; i++)
    {
        if (frequencies[i] > 1)
        {
            cout << uniqueWords[i]
                 << " -> "
                 << frequencies[i]
                 << " times\n";
        }
    }
}

// ------------------------------------------------------------
// Pattern Analysis
//
// Example:
// HELLO -> 01223
// ------------------------------------------------------------
string getPattern(const string &word)
{
    string pattern = "";

    char letters[MAX_WORD_LENGTH];

    int letterCount = 0;

    for (int i = 0;
         i < (int)word.length();
         i++)
    {
        char ch = word[i];

        int found = -1;

        for (int j = 0;
             j < letterCount;
             j++)
        {
            if (letters[j] == ch)
            {
                found = j;
                break;
            }
        }

        if (found == -1)
        {
            letters[letterCount] = ch;

            pattern +=
                char('0' + letterCount);

            letterCount++;
        }
        else
        {
            pattern +=
                char('0' + found);
        }
    }

    return pattern;
}

// ------------------------------------------------------------
// Pattern Analysis
// ------------------------------------------------------------
void pattern_analysis(const string &ciphertext)
{
    string words[MAX_WORDS];

    int count =
        extractWords(ciphertext,
                     words,
                     MAX_WORDS);

    cout << "\n============================================\n";
    cout << "            PATTERN ANALYSIS\n";
    cout << "============================================\n";

    for (int i = 0; i < count; i++)
    {
        if (words[i].length() >= 3)
        {
            cout << words[i]
                 << " -> "
                 << getPattern(words[i])
                 << "\n";
        }
    }
}

// ------------------------------------------------------------
// Apply Complete Substitution Key
//
// decryptKey[ciphertext - 'A'] = plaintext letter
// ------------------------------------------------------------
string apply_substitution(const string &ciphertext,
                          const string &decryptKey)
{
    string plaintext = "";

    for (int i = 0;
         i < (int)ciphertext.length();
         i++)
    {
        char ch = ciphertext[i];

        if (isalpha((unsigned char)ch))
        {
            bool lower =
                islower((unsigned char)ch);

            int index =
                toupper((unsigned char)ch) - 'A';

            char decrypted =
                decryptKey[index];

            if (lower)
                decrypted =
                    tolower((unsigned char)decrypted);

            plaintext += decrypted;
        }
        else
        {
            plaintext += ch;
        }
    }

    return plaintext;
}

// ------------------------------------------------------------
// Display Substitution Key
// ------------------------------------------------------------
void displayKey(const string &decryptKey)
{
    cout << "\n============================================\n";
    cout << "          RECOVERED SUBSTITUTION KEY\n";
    cout << "============================================\n";

    cout << "Cipher : ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";

    cout << "Plain  : "
         << decryptKey
         << "\n";

    cout << "\nMappings:\n";

    for (int i = 0;
         i < ALPHABET_SIZE;
         i++)
    {
        cout << char('A' + i)
             << " -> "
             << decryptKey[i]
             << "\n";
    }
}

// ------------------------------------------------------------
// Check whether key contains every alphabet letter exactly once
// ------------------------------------------------------------
bool isValidKey(const string &key)
{
    if (key.length() != ALPHABET_SIZE)
        return false;

    bool used[ALPHABET_SIZE] = {false};

    for (int i = 0;
         i < ALPHABET_SIZE;
         i++)
    {
        if (!isalpha((unsigned char)key[i]))
            return false;

        char ch =
            toupper((unsigned char)key[i]);

        int index = ch - 'A';

        if (used[index])
            return false;

        used[index] = true;
    }

    return true;
}

// ------------------------------------------------------------
// Verification
//
// decryptKey:
// ciphertext -> plaintext
//
// Build inverse key:
// plaintext -> ciphertext
// ------------------------------------------------------------
bool verify_solution(const string &ciphertext,
                     const string &recoveredPlaintext,
                     const string &decryptKey)
{
    string encryptionKey(
        ALPHABET_SIZE, ' ');

    // Build inverse encryption key
    for (int i = 0;
         i < ALPHABET_SIZE;
         i++)
    {
        char cipherLetter =
            'A' + i;

        char plainLetter =
            toupper((unsigned char)
                    decryptKey[i]);

        int plainIndex =
            plainLetter - 'A';

        encryptionKey[plainIndex] =
            cipherLetter;
    }

    // Encrypt recovered plaintext again
    string reEncrypted =
        encryptText(recoveredPlaintext,
                    encryptionKey);

    cout << "\n============================================\n";
    cout << "              VERIFICATION\n";
    cout << "============================================\n";

    if (reEncrypted == ciphertext)
    {
        cout << "Original Ciphertext  = "
             << "Re-encrypted Plaintext\n";

        cout << "Verification Result  : SUCCESS\n";

        return true;
    }
    else
    {
        cout << "Verification Result  : FAILED\n";

        cout << "The recovered key does not "
             << "reproduce the ciphertext.\n";

        return false;
    }
}

// ------------------------------------------------------------
// MAIN
// ------------------------------------------------------------
int main()
{
    string plaintext = "";

    // --------------------------------------------------------
    // Read plaintext from input file
    // --------------------------------------------------------
    ifstream inputFile(
        "input/plaintext.txt");

    if (!inputFile)
    {
        cout << "Error: Could not open "
             << "input/plaintext.txt\n";

        return 1;
    }

    string line;

    while (getline(inputFile, line))
    {
        plaintext += line;

        plaintext += '\n';
    }

    inputFile.close();

    if (plaintext.length() == 0)
    {
        cout << "Error: plaintext.txt is empty.\n";

        return 1;
    }

    // --------------------------------------------------------
    // Generate RANDOM encryption key
    // --------------------------------------------------------
    string encryptionKey =
        generateRandomKey();

    cout << "\n============================================\n";
    cout << "      MONOALPHABETIC SUBSTITUTION CIPHER\n";
    cout << "============================================\n";

    cout << "\nRandom Encryption Key:\n";

    cout << "Plain  : "
         << "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";

    cout << "Cipher : "
         << encryptionKey
         << "\n";

    // --------------------------------------------------------
    // Encrypt plaintext
    // --------------------------------------------------------
    string ciphertext =
        encryptText(plaintext,
                    encryptionKey);

    cout << "\nOriginal Plaintext:\n";

    cout << plaintext << "\n";

    cout << "\nCiphertext:\n";

    cout << ciphertext << "\n";

    // --------------------------------------------------------
    // Cryptanalysis
    // --------------------------------------------------------
    frequency_analysis(ciphertext);

    word_frequency_analysis(ciphertext);

    pattern_analysis(ciphertext);

    // --------------------------------------------------------
    // Enter complete recovered key
    // --------------------------------------------------------
    string recoveredKey;

    cout << "\n============================================\n";
    cout << "        MANUAL KEY RECOVERY\n";
    cout << "============================================\n";

    cout << "\nEnter the complete recovered key.\n";

    cout << "The key must contain "
         << "26 unique letters.\n\n";

    cout << "Cipher : "
         << "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";

    cout << "Plain  : ";

    cin >> recoveredKey;

    // Convert key to uppercase
    for (int i = 0;
         i < (int)recoveredKey.length();
         i++)
    {
        recoveredKey[i] =
            toupper((unsigned char)
                    recoveredKey[i]);
    }

    // --------------------------------------------------------
    // Validate key
    // --------------------------------------------------------
    while (!isValidKey(recoveredKey))
    {
        cout << "\nInvalid key!\n";

        cout << "The key must contain "
             << "exactly 26 unique letters.\n";

        cout << "\nEnter key again: ";

        cin >> recoveredKey;

        for (int i = 0;
             i < (int)recoveredKey.length();
             i++)
        {
            recoveredKey[i] =
                toupper((unsigned char)
                        recoveredKey[i]);
        }
    }

    // --------------------------------------------------------
    // Display recovered key
    // --------------------------------------------------------
    displayKey(recoveredKey);

    // --------------------------------------------------------
    // Decrypt ciphertext
    // --------------------------------------------------------
    string recoveredPlaintext =
        apply_substitution(ciphertext,
                           recoveredKey);

    cout << "\n============================================\n";
    cout << "          RECOVERED PLAINTEXT\n";
    cout << "============================================\n";

    cout << recoveredPlaintext
         << "\n";

    // --------------------------------------------------------
    // Verification
    // --------------------------------------------------------
    verify_solution(
        ciphertext,
        recoveredPlaintext,
        recoveredKey
    );

    cout << "\n============================================\n";
    cout << "              PROGRAM END\n";
    cout << "============================================\n";

    return 0;
}
