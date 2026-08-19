# Shift Cipher Cryptanalysis

## Purpose

Cryptanalysis of Shift Cipher using:

1. Brute Force
2. Dictionary Scoring
3. Chi-Square Analysis

## Project Structure

- `src/shift_cipher.py` - Shift Cipher encryption/decryption
- `src/brute_force_dictionary.py` - Brute-force dictionary attack
- `src/chi_square_attack.py` - Chi-Square cryptanalysis
- `dictionary/english_words.txt` - English word dictionary
- `testcases/` - Test cases
- `outputs/` - Experimental results
- `screenshots/` - Screenshots
- `reports/` - Laboratory report

## Algorithms

### Brute Force and Dictionary Scoring

All 26 possible keys are tested. The decrypted text containing the
largest number of dictionary words is selected as the most likely
plaintext.

### Chi-Square Analysis

All 26 possible keys are tested. The letter frequency of each
decrypted text is compared with standard English letter frequencies.
The key with the lowest Chi-Square score is selected.

## Results

The attacks were tested using multiple Shift Cipher test cases.
The predicted keys were compared with the actual encryption keys.

## Conclusion

The experiment demonstrates that Shift Cipher is vulnerable to
brute-force and statistical cryptanalysis because it has only
26 possible keys.
