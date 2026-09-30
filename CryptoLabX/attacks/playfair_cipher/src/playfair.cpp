#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <iomanip>

using namespace std;


// ------------------------------------------------------------
// Generate 5 x 5 Key Matrix
// ------------------------------------------------------------
vector<vector<char>> generate_key_matrix(string keyword) {

    vector<vector<char>> matrix(5, vector<char>(5));

    string key = "";

    // Process keyword
    for (char ch : keyword) {

        ch = toupper(ch);

        if (!isalpha(ch))
            continue;

        // Combine I and J
        if (ch == 'J')
            ch = 'I';

        // Add only if not already present
        if (key.find(ch) == string::npos)
            key += ch;
    }

    // Add remaining alphabet
    for (char ch = 'A'; ch <= 'Z'; ch++) {

        if (ch == 'J')
            continue;

        if (key.find(ch) == string::npos)
            key += ch;
    }

    // Fill matrix
    int k = 0;

    for (int i = 0; i < 5; i++) {

        for (int j = 0; j < 5; j++) {

            matrix[i][j] = key[k++];
        }
    }

    return matrix;
}


// ------------------------------------------------------------
// Prepare Plaintext
// ------------------------------------------------------------
string prepare_plaintext(string plaintext) {

    string result = "";

    for (char ch : plaintext) {

        if (!isalpha(ch))
            continue;

        ch = toupper(ch);

        // Combine I and J
        if (ch == 'J')
            ch = 'I';

        result += ch;
    }

    return result;
}


// ------------------------------------------------------------
// Create Digraphs
// ------------------------------------------------------------
vector<string> create_digraphs(string plaintext) {

    vector<string> digraphs;

    int i = 0;

    while (i < plaintext.length()) {

        char first = plaintext[i];

        // Last single character
        if (i + 1 >= plaintext.length()) {

            string pair = "";

            pair += first;
            pair += 'X';

            digraphs.push_back(pair);

            i++;
        }

        else {

            char second = plaintext[i + 1];

            // Repeated letters
            if (first == second) {

                string pair = "";

                pair += first;
                pair += 'X';

                digraphs.push_back(pair);

                i++;
            }

            else {

                string pair = "";

                pair += first;
                pair += second;

                digraphs.push_back(pair);

                i += 2;
            }
        }
    }

    return digraphs;
}


// ------------------------------------------------------------
// Find position of character in matrix
// ------------------------------------------------------------
void find_position(
    const vector<vector<char>>& matrix,
    char ch,
    int& row,
    int& col
) {

    if (ch == 'J')
        ch = 'I';

    for (int i = 0; i < 5; i++) {

        for (int j = 0; j < 5; j++) {

            if (matrix[i][j] == ch) {

                row = i;
                col = j;

                return;
            }
        }
    }
}


// ------------------------------------------------------------
// Playfair Encryption
// ------------------------------------------------------------
string playfair_encrypt(
    const vector<string>& digraphs,
    const vector<vector<char>>& matrix
) {

    string ciphertext = "";

    for (const string& pair : digraphs) {

        char a = pair[0];
        char b = pair[1];

        int row1, col1;
        int row2, col2;

        find_position(matrix, a, row1, col1);
        find_position(matrix, b, row2, col2);


        // Same row
        if (row1 == row2) {

            ciphertext +=
                matrix[row1][(col1 + 1) % 5];

            ciphertext +=
                matrix[row2][(col2 + 1) % 5];
        }


        // Same column
        else if (col1 == col2) {

            ciphertext +=
                matrix[(row1 + 1) % 5][col1];

            ciphertext +=
                matrix[(row2 + 1) % 5][col2];
        }


        // Rectangle rule
        else {

            ciphertext +=
                matrix[row1][col2];

            ciphertext +=
                matrix[row2][col1];
        }
    }

    return ciphertext;
}


// ------------------------------------------------------------
// Playfair Decryption
// ------------------------------------------------------------
string playfair_decrypt(
    const string& ciphertext,
    const vector<vector<char>>& matrix
) {

    string plaintext = "";

    for (int i = 0; i < ciphertext.length(); i += 2) {

        char a = ciphertext[i];
        char b = ciphertext[i + 1];

        int row1, col1;
        int row2, col2;

        find_position(matrix, a, row1, col1);
        find_position(matrix, b, row2, col2);


        // Same row
        if (row1 == row2) {

            plaintext +=
                matrix[row1][(col1 + 4) % 5];

            plaintext +=
                matrix[row2][(col2 + 4) % 5];
        }


        // Same column
        else if (col1 == col2) {

            plaintext +=
                matrix[(row1 + 4) % 5][col1];

            plaintext +=
                matrix[(row2 + 4) % 5][col2];
        }


        // Rectangle rule
        else {

            plaintext +=
                matrix[row1][col2];

            plaintext +=
                matrix[row2][col1];
        }
    }

    return plaintext;
}


// ------------------------------------------------------------
// Digraph Frequency Analysis
// ------------------------------------------------------------
map<string, int> digraph_frequency(string ciphertext) {

    map<string, int> frequency;

    for (int i = 0; i + 1 < ciphertext.length(); i += 2) {

        string digraph =
            ciphertext.substr(i, 2);

        frequency[digraph]++;
    }

    return frequency;
}


// ------------------------------------------------------------
// Verification
// ------------------------------------------------------------
bool verify(
    const string& ciphertext,
    const string& decrypted,
    const vector<vector<char>>& matrix
) {

    vector<string> digraphs =
        create_digraphs(decrypted);

    string reencrypted =
        playfair_encrypt(digraphs, matrix);

    return reencrypted == ciphertext;
}


// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main() {

    string keyword;
    string plaintext;


    // Input
    cout << "Keyword: ";
    getline(cin, keyword);

    cout << "Plaintext: ";
    getline(cin, plaintext);


    // Generate key matrix
    vector<vector<char>> matrix =
        generate_key_matrix(keyword);


    // Display key matrix
    cout << "\nKey Matrix\n";

    for (int i = 0; i < 5; i++) {

        for (int j = 0; j < 5; j++) {

            cout << matrix[i][j];

            if (j < 4)
                cout << " ";
        }

        cout << endl;
    }


    // Prepare plaintext
    string prepared =
        prepare_plaintext(plaintext);


    // Create digraphs
    vector<string> digraphs =
        create_digraphs(prepared);


    cout << "\nPrepared Digraphs:\n";

    for (int i = 0; i < digraphs.size(); i++) {

        cout << digraphs[i];

        if (i < digraphs.size() - 1)
            cout << " ";
    }

    cout << endl;


    // Encryption
    string ciphertext =
        playfair_encrypt(digraphs, matrix);


    cout << "\nCiphertext:\n";
    cout << ciphertext << endl;


    // Decryption
    string decrypted =
        playfair_decrypt(ciphertext, matrix);


    cout << "\nDecrypted Text:\n";
    cout << decrypted << endl;


    // Verification
    bool result =
        verify(ciphertext, decrypted, matrix);


    cout << "\nVerification:\n";

    if (result)
        cout << "SUCCESS" << endl;
    else
        cout << "FAILED" << endl;


    return 0;
}
