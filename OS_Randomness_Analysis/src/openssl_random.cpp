#include <iostream>
#include <fstream>
#include <algorithm>
#include <openssl/rand.h>

using namespace std;

int main() {
    const size_t SIZE = 1048576; // 1 MiB
    unsigned char buffer[4096];

    ofstream output("data/openssl_random.bin", ios::binary);

    if (!output) {
        cerr << "Cannot create output file.\n";
        return 1;
    }

    size_t remaining = SIZE;

    while (remaining > 0) {
        size_t chunk = min(remaining, sizeof(buffer));

        if (RAND_bytes(buffer, static_cast<int>(chunk)) != 1) {
            cerr << "OpenSSL random generation failed.\n";
            return 1;
        }

        output.write(
            reinterpret_cast<char*>(buffer),
            static_cast<streamsize>(chunk)
        );

        if (!output) {
            cerr << "Error writing random data.\n";
            return 1;
        }

        remaining -= chunk;
    }

    cout << "Generated 1 MiB using OpenSSL RAND_bytes().\n";
    cout << "File: data/openssl_random.bin\n";

    return 0;
}
