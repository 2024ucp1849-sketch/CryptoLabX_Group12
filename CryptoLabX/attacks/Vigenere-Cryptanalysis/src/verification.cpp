#include "../include/verification.h"
#include "../include/vigenere.h"

using namespace std;

bool verify(
    const string& originalCiphertext,
    const string& plaintext,
    const string& key)
{
    string reEncrypted =
        vigenere_encrypt(plaintext, key);

    return reEncrypted == originalCiphertext;
}
