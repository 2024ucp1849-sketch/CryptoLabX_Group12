#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int main() {
    const size_t SIZE = 1048576;
    char buffer[4096];

    ifstream input("/dev/urandom", ios::binary);
    ofstream output("data/linux_random.bin", ios::binary);

    if (!input || !output) {
        cerr << "Error opening files.\n";
        return 1;
    }

    size_t remaining = SIZE;

    while (remaining > 0) {
        size_t chunk = min(remaining, sizeof(buffer));

        input.read(buffer, chunk);

        if (input.gcount() != static_cast<streamsize>(chunk)) {
            cerr << "Failed to read random data.\n";
            return 1;
        }

        output.write(buffer, chunk);

        if (!output) {
            cerr << "Failed to write random data.\n";
            return 1;
        }

        remaining -= chunk;
    }

    cout << "Generated 1 MiB using Linux /dev/urandom.\n";
    return 0;
}
