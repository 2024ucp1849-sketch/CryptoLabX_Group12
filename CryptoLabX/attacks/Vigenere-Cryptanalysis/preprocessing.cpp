cat > src/preprocessing.cpp <<'EOF'
#include "../include/preprocessing.h"
#include <cctype>

std::string clean_ciphertext(const std::string& ciphertext)
{
    std::string result;

    for (char c : ciphertext)
    {
        if (std::isalpha(static_cast<unsigned char>(c)))
        {
            result += static_cast<char>(
                std::toupper(static_cast<unsigned char>(c))
            );
        }
    }

    return result;
}
EOF
