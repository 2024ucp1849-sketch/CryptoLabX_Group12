#include "../include/vigenere.h"
#include "../include/frequency.h"

using namespace std;

string find_key(const vector<string>& groups)
{
    string key;

    for (const string& group : groups)
    {
        int shift = find_shift(group);
        key += char('A' + shift);
    }

    return key;
}

string vigenere_decrypt(
    const string& ciphertext,
    const string& key)
{
    string plaintext;

    for (int i = 0; i < (int)ciphertext.size(); i++)
    {
        int c = ciphertext[i] - 'A';
        int k = key[i % key.size()] - 'A';

        int p = (c - k + 26) % 26;

        plaintext += char('A' + p);
    }

    return plaintext;
}

string vigenere_encrypt(
    const string& plaintext,
    const string& key)
{
    string ciphertext;

    for (int i = 0; i < (int)plaintext.size(); i++)
    {
        int p = plaintext[i] - 'A';
        int k = key[i % key.size()] - 'A';

        int c = (p + k) % 26;

        ciphertext += char('A' + c);
    }

    return ciphertext;
}
