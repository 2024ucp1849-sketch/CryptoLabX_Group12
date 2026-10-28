#include <iostream>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include <bcrypt.h>

using namespace std;

int main() {
    const size_t SIZE = 1048576;
    unsigned char buffer[4096];

    ofstream output("data/windows_random.bin", ios::binary);

    if (!output) {
        cerr << "Cannot create output file.\n";
        return 1;
    }

    size_t remaining = SIZE;

    while (remaining > 0) {
        size_t chunk = min(remaining, sizeof(buffer));

        NTSTATUS status = BCryptGenRandom(
            nullptr,
            buffer,
            static_cast<ULONG>(chunk),
            BCRYPT_USE_SYSTEM_PREFERRED_RNG
        );

        if (status < 0) {
            cerr << "BCryptGenRandom failed.\n";
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

    cout << "Generated 1 MiB using Windows CNG.\n";
    cout << "File: data/windows_random.bin\n";

    return 0;
}
