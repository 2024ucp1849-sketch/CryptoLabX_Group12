cat > src/verification.cpp <<'EOF'
#include "../include/verification.h"
#include "../include/vigenere.h"

using namespace std;

bool verify(
    const string& originalCiphertext,
    const string& plaintext,
    const string& key)
{
    string encrypted =
        vigenere_encrypt(
            plaintext,
            key
        );

    return encrypted ==
           originalCiphertext;
}
EOF
