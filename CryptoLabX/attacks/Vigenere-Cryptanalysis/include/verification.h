#ifndef VERIFICATION_H
#define VERIFICATION_H

#include <string>

bool verify(
    const std::string& originalCiphertext,
    const std::string& plaintext,
    const std::string& key
);

#endif
