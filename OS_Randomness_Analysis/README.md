# OS Randomness Analysis in C++

## Objective
Evaluate operating-system random-number generation mechanisms and investigate their suitability for cryptographic applications.

## Random Number Generators
1. Linux `/dev/urandom`
2. OpenSSL `RAND_bytes()`
3. Windows CNG `BCryptGenRandom()`

Each generator produces 1 MiB (1,048,576 bytes) of random data.

## Statistical Analysis
The C++ analysis program measures:
- Byte-frequency distribution
- Zero-bit and one-bit distributions
- Byte-frequency chi-square statistic
- Lag-1 serial correlation

Repeated-pattern detection and additional statistical tests can extend the analysis.

## Project Structure
```text
OS_Randomness_Analysis/
├── src/
│   ├── urandom.cpp
│   ├── openssl_random.cpp
│   ├── windows_random.cpp
│   └── analysis.cpp
├── data/
├── results/
├── README.md
└── .gitignore
```

## Requirements
- C++ compiler supporting C++17
- OpenSSL development library on Linux
- Windows SDK for the Windows CNG implementation

## Run on Ubuntu

Install dependencies:

```bash
sudo apt update
sudo apt install g++ libssl-dev
```

Generate Linux random data:

```bash
g++ src/urandom.cpp -o urandom
./urandom
```

Generate OpenSSL random data:

```bash
g++ src/openssl_random.cpp -o openssl_random -lcrypto
./openssl_random
```

Compile and run the analysis:

```bash
g++ -std=c++17 src/analysis.cpp -o analysis
./analysis
```

## Windows CNG

Compile `src/windows_random.cpp` on Windows with the Windows SDK and link against `bcrypt.lib`. Copy the generated file into the Linux project's `data/` directory for comparison.

## Security Discussion

Statistical tests can identify some deviations from expected randomness, but passing these tests does not prove cryptographic security. Security also depends on unpredictability, secure initialization, internal-state protection, and correct implementation.
