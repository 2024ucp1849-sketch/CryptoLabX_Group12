#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>
#include <cmath>

using namespace std;

void analyze(const string& filename) {
    ifstream file(filename, ios::binary);

    if (!file) {
        cout << "\nFile not found: " << filename << '\n';
        return;
    }

    vector<unsigned char> data(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );

    if (data.empty()) {
        cout << "File is empty: " << filename << '\n';
        return;
    }

    array<long long, 256> frequency{};
    long long zeros = 0, ones = 0;

    for (unsigned char byte : data) {
        frequency[byte]++;

        for (int bit = 0; bit < 8; bit++) {
            if ((byte >> bit) & 1)
                ones++;
            else
                zeros++;
        }
    }

    // Byte-frequency chi-square statistic
    double expected = static_cast<double>(data.size()) / 256.0;
    double chiSquare = 0.0;

    for (int i = 0; i < 256; i++) {
        double difference = frequency[i] - expected;
        chiSquare += difference * difference / expected;
    }

    // Calculate mean
    double mean = 0.0;

    for (unsigned char byte : data)
        mean += byte;

    mean /= data.size();

    // Calculate lag-1 serial correlation
    double numerator = 0.0;
    double denominator = 0.0;

    for (size_t i = 0; i < data.size(); i++) {
        double current = data[i] - mean;
        denominator += current * current;

        if (i + 1 < data.size()) {
            numerator += current * (data[i + 1] - mean);
        }
    }

    double correlation =
        denominator == 0.0 ? 0.0 : numerator / denominator;

    long long minimum =
        *min_element(frequency.begin(), frequency.end());

    long long maximum =
        *max_element(frequency.begin(), frequency.end());

    cout << "\n====================================\n";
    cout << "File: " << filename << '\n';
    cout << "Total bytes: " << data.size() << '\n';
    cout << "Expected frequency per byte: " << expected << '\n';
    cout << "Minimum byte frequency: " << minimum << '\n';
    cout << "Maximum byte frequency: " << maximum << '\n';

    cout << "Zero bits: " << zeros << '\n';
    cout << "One bits: " << ones << '\n';

    cout << fixed << setprecision(6);

    cout << "Zero percentage: "
         << 100.0 * zeros / (zeros + ones) << "%\n";

    cout << "One percentage: "
         << 100.0 * ones / (zeros + ones) << "%\n";

    cout << "Byte-frequency chi-square: "
         << chiSquare << '\n';

    cout << "Lag-1 serial correlation: "
         << correlation << '\n';
}

int main() {
    analyze("data/linux_random.bin");
    analyze("data/openssl_random.bin");
    analyze("data/windows_random.bin");

    return 0;
}
