#include "../include/preprocessing.h"
#include <cctype>

using namespace std;

string clean_ciphertext(const string& ciphertext)
{
    string cleaned;

    for (char c : ciphertext)
    {
        if (isalpha(static_cast<unsigned char>(c)))
        {
            cleaned += toupper(static_cast<unsigned char>(c));
        }
    }

    return cleaned;
}
