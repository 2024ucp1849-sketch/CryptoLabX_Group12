
import string
from collections import Counter


# ============================================================
# 1. clean_ciphertext()
# Purpose: Remove spaces/special characters and normalize text
# ============================================================

def clean_ciphertext(ciphertext):
    return ''.join(
        ch for ch in ciphertext.upper()
        if 'A' <= ch <= 'Z'
    )


# ============================================================
# 2. find_repeated_patterns()
# Purpose: Identify repeated sequences in ciphertext
# ============================================================

def find_repeated_patterns(ciphertext, min_length=3, max_length=5):

    patterns = {}

    for length in range(min_length, max_length + 1):

        for i in range(len(ciphertext) - length + 1):

            pattern = ciphertext[i:i + length]

            if pattern not in patterns:
                patterns[pattern] = []

            patterns[pattern].append(i)

    # Keep only repeated patterns
    repeated = {
        pattern: positions
        for pattern, positions in patterns.items()
        if len(positions) > 1
    }

    return repeated


# ============================================================
# 3. calculate_distances()
# Purpose: Find distances between repeated occurrences
# ============================================================

def calculate_distances(repeated_patterns):

    distances = {}

    for pattern, positions in repeated_patterns.items():

        pattern_distances = []

        for i in range(len(positions)):

            for j in range(i + 1, len(positions)):

                distance = positions[j] - positions[i]

                pattern_distances.append(distance)

        distances[pattern] = pattern_distances

    return distances


# ============================================================
# 4. find_factors()
# Purpose: Find factors of a distance
# ============================================================

def find_factors(number, max_factor=20):

    factors = []

    for i in range(2, min(number, max_factor) + 1):

        if number % i == 0:
            factors.append(i)

    return factors


# ============================================================
# 5. kasiski_analysis()
# Purpose: Use repeated patterns and distances to suggest
#          candidate key lengths
# ============================================================

def kasiski_analysis(ciphertext):

    repeated_patterns = find_repeated_patterns(ciphertext)

    distances = calculate_distances(repeated_patterns)

    factor_counts = Counter()

    for pattern in distances:

        for distance in distances[pattern]:

            factors = find_factors(distance)

            for factor in factors:

                factor_counts[factor] += 1

    candidates = factor_counts.most_common()

    return repeated_patterns, distances, factor_counts, candidates


# ============================================================
# 6. calculate_ic()
# Purpose: Calculate Index of Coincidence
# Bonus technique for estimating key length
# ============================================================

def calculate_ic(text):

    N = len(text)

    if N <= 1:
        return 0.0

    frequencies = Counter(text)

    numerator = 0

    for count in frequencies.values():

        numerator += count * (count - 1)

    denominator = N * (N - 1)

    return numerator / denominator


# ============================================================
# 7. split_into_groups()
# Purpose: Divide ciphertext according to key length
# ============================================================

def split_into_groups(ciphertext, key_length):

    groups = []

    for i in range(key_length):

        group = ciphertext[i::key_length]

        groups.append(group)

    return groups


# ============================================================
# 8. frequency_analysis()
# Purpose: Calculate A-Z frequency for each group
# ============================================================

def frequency_analysis(group):

    frequencies = Counter(group)

    result = {}

    for letter in string.ascii_uppercase:

        result[letter] = frequencies.get(letter, 0)

    return result


# ============================================================
# English letter frequencies
# ============================================================

ENGLISH_FREQUENCIES = {
    'A': 0.08167,
    'B': 0.01492,
    'C': 0.02782,
    'D': 0.04253,
    'E': 0.12702,
    'F': 0.02288,
    'G': 0.02015,
    'H': 0.06094,
    'I': 0.06966,
    'J': 0.00153,
    'K': 0.00772,
    'L': 0.04025,
    'M': 0.02406,
    'N': 0.06749,
    'O': 0.07507,
    'P': 0.01929,
    'Q': 0.00095,
    'R': 0.05987,
    'S': 0.06327,
    'T': 0.09056,
    'U': 0.02758,
    'V': 0.00978,
    'W': 0.02360,
    'X': 0.00150,
    'Y': 0.01974,
    'Z': 0.00074
}


# ============================================================
# 9. find_shift()
# Purpose: Estimate the Caesar shift for one group
# Uses Chi-Square analysis
# ============================================================

def find_shift(group):

    N = len(group)

    if N == 0:
        return 0

    frequencies = frequency_analysis(group)

    best_shift = 0
    lowest_chi_square = float('inf')

    # Try all 26 possible Caesar shifts
    for shift in range(26):

        chi_square = 0.0

        for i, letter in enumerate(string.ascii_uppercase):

            # Decrypt this letter using the current shift
            decrypted_index = (i - shift) % 26

            decrypted_letter = string.ascii_uppercase[
                decrypted_index
            ]

            observed = frequencies[letter]

            expected = (
                ENGLISH_FREQUENCIES[decrypted_letter] * N
            )

            if expected > 0:

                chi_square += (
                    (observed - expected) ** 2
                ) / expected

        if chi_square < lowest_chi_square:

            lowest_chi_square = chi_square
            best_shift = shift

    return best_shift


# ============================================================
# 10. find_key()
# Purpose: Combine shifts to obtain probable Vigenère key
# ============================================================

def find_key(groups):

    key = ""

    for group in groups:

        shift = find_shift(group)

        key_letter = string.ascii_uppercase[shift]

        key += key_letter

    return key


# ============================================================
# 11. vigenere_decrypt()
# Purpose: Decrypt ciphertext using recovered key
# ============================================================

def vigenere_decrypt(ciphertext, key):

    plaintext = ""

    key = key.upper()

    key_index = 0

    for char in ciphertext:

        if 'A' <= char <= 'Z':

            cipher_value = ord(char) - ord('A')

            key_value = (
                ord(key[key_index % len(key)])
                - ord('A')
            )

            plain_value = (
                cipher_value - key_value
            ) % 26

            plaintext += chr(
                plain_value + ord('A')
            )

            key_index += 1

    return plaintext


# ============================================================
# 12. vigenere_encrypt()
# Purpose: Re-encrypt plaintext for verification
# ============================================================

def vigenere_encrypt(plaintext, key):

    ciphertext = ""

    key = key.upper()

    key_index = 0

    for char in plaintext:

        if 'A' <= char <= 'Z':

            plain_value = ord(char) - ord('A')

            key_value = (
                ord(key[key_index % len(key)])
                - ord('A')
            )

            cipher_value = (
                plain_value + key_value
            ) % 26

            ciphertext += chr(
                cipher_value + ord('A')
            )

            key_index += 1

    return ciphertext


# ============================================================
# 13. verify()
# Purpose: Check whether re-encryption produces original
#          ciphertext
# ============================================================

def verify(original_ciphertext, re_encrypted_ciphertext):

    return original_ciphertext == re_encrypted_ciphertext


# ============================================================
# IC KEY LENGTH ANALYSIS
# Bonus: Test possible key lengths using IC
# ============================================================

def ic_key_length_analysis(ciphertext, max_key_length=20):

    results = []

    for key_length in range(1, max_key_length + 1):

        groups = split_into_groups(
            ciphertext,
            key_length
        )

        ic_values = []

        for group in groups:

            if len(group) > 1:

                ic = calculate_ic(group)

                ic_values.append(ic)

        if len(ic_values) > 0:

            average_ic = (
                sum(ic_values) / len(ic_values)
            )

            results.append(
                (key_length, average_ic)
            )

    # Higher IC is a better candidate
    results.sort(
        key=lambda x: x[1],
        reverse=True
    )

    return results


# ============================================================
# MAIN PROGRAM
# ============================================================

def main():

    print("=" * 60)
    print("VIGENERE CIPHER CRYPTANALYSIS")
    print("Kasiski + Frequency Analysis + IC Bonus")
    print("=" * 60)


    # --------------------------------------------------------
    # STEP 1: INPUT
    # --------------------------------------------------------

    print("\nEnter ciphertext.")
    print("Paste the complete ciphertext.")
    print("After pasting, press Ctrl+D.")
    print()

    ciphertext = ""

    while True:

        try:

            line = input()

            ciphertext += line

        except EOFError:

            break


    # --------------------------------------------------------
    # PREPROCESSING
    # --------------------------------------------------------

    ciphertext = clean_ciphertext(ciphertext)

    print("\n" + "=" * 60)
    print("PREPROCESSED CIPHERTEXT")
    print("=" * 60)

    print(ciphertext)

    print("\nCiphertext Length:", len(ciphertext))


    # ========================================================
    # STEP 2: KASISKI ANALYSIS
    # ========================================================

    print("\n" + "=" * 60)
    print("KASISKI ANALYSIS")
    print("=" * 60)


    repeated_patterns, distances, factor_counts, candidates = (
        kasiski_analysis(ciphertext)
    )


    print("\nRepeated Patterns:")

    if len(repeated_patterns) == 0:

        print("No repeated patterns found.")

    else:

        for pattern, positions in repeated_patterns.items():

            print(
                pattern,
                "->",
                positions
            )


    print("\nDistances:")

    if len(distances) == 0:

        print("No distances found.")

    else:

        for pattern, values in distances.items():

            print(
                pattern,
                "->",
                values
            )


    print("\nFactor Counts:")

    if len(factor_counts) == 0:

        print("No factors found.")

    else:

        for factor, count in factor_counts.most_common():

            print(
                "Key Length:",
                factor,
                "Count:",
                count
            )


    print("\nCandidate Key Lengths:")

    if len(candidates) == 0:

        print("No candidate key lengths found.")

    else:

        for length, score in candidates:

            print(
                "Length:",
                length,
                "Score:",
                score
            )


    # --------------------------------------------------------
    # Required Kasiski estimated key length
    # --------------------------------------------------------

    if len(candidates) > 0:

        estimated_key_length = candidates[0][0]

    else:

        estimated_key_length = None


    if estimated_key_length is not None:

        print(
            "\nEstimated Key Length using Kasiski:",
            estimated_key_length
        )

    else:

        print(
            "\nKasiski could not estimate the key length."
        )


    # ========================================================
    # BONUS: INDEX OF COINCIDENCE
    # ========================================================

    print("\n" + "=" * 60)
    print("INDEX OF COINCIDENCE (BONUS)")
    print("=" * 60)


    ic_results = ic_key_length_analysis(
        ciphertext,
        20
    )


    print("\nKey Length\tAverage IC")
    print("-" * 35)

    for key_length, average_ic in ic_results:

        print(
            f"{key_length}\t\t{average_ic:.4f}"
        )


    if len(ic_results) > 0:

        ic_estimated_length = ic_results[0][0]

        print(
            "\nIC Estimated Key Length:",
            ic_estimated_length
        )


    # ========================================================
    # STEP 3: DIVIDE CIPHERTEXT INTO GROUPS
    # ========================================================

    print("\n" + "=" * 60)
    print("CIPHERTEXT GROUPS")
    print("=" * 60)


    # Kasiski is the required method
    if estimated_key_length is None:

        print(
            "\nCannot continue because Kasiski "
            "did not find a key length."
        )

        return


    groups = split_into_groups(
        ciphertext,
        estimated_key_length
    )


    for i, group in enumerate(groups):

        print(
            f"\nGroup {i + 1}:"
        )

        print(group)


    # ========================================================
    # STEP 4: FREQUENCY ANALYSIS
    # ========================================================

    print("\n" + "=" * 60)
    print("FREQUENCY ANALYSIS")
    print("=" * 60)


    for i, group in enumerate(groups):

        frequencies = frequency_analysis(group)

        print(
            f"\nGroup {i + 1}"
        )

        print(
            "Letter\tFrequency"
        )

        print(
            "-" * 25
        )

        for letter, count in frequencies.items():

            print(
                f"{letter}\t{count}"
            )


    # ========================================================
    # STEP 5: DETERMINE PROBABLE KEY
    # ========================================================

    print("\n" + "=" * 60)
    print("KEY RECOVERY")
    print("=" * 60)


    recovered_key = find_key(groups)


    for i, group in enumerate(groups):

        shift = find_shift(group)

        key_letter = string.ascii_uppercase[shift]

        print(
            f"Group {i + 1}: "
            f"Shift = {shift}, "
            f"Key Letter = {key_letter}"
        )


    print(
        "\nRecovered Key:",
        recovered_key
    )


    # ========================================================
    # STEP 6: DECRYPT
    # ========================================================

    plaintext = vigenere_decrypt(
        ciphertext,
        recovered_key
    )


    print("\n" + "=" * 60)
    print("RECOVERED PLAINTEXT")
    print("=" * 60)

    print(plaintext)


    # ========================================================
    # STEP 8: RE-ENCRYPT AND VERIFY
    # ========================================================

    re_encrypted_ciphertext = vigenere_encrypt(
        plaintext,
        recovered_key
    )


    print("\n" + "=" * 60)
    print("VERIFICATION")
    print("=" * 60)


    print("\nOriginal Ciphertext:")
    print(ciphertext)


    print("\nRe-encrypted Ciphertext:")
    print(re_encrypted_ciphertext)


    if verify(
        ciphertext,
        re_encrypted_ciphertext
    ):

        print(
            "\nVerification Successful!"
        )

        print(
            "The re-encrypted ciphertext "
            "is identical to the original ciphertext."
        )

    else:

        print(
            "\nVerification Failed!"
        )

        print(
            "The ciphertexts are different."
        )


# ============================================================
# PROGRAM START
# ============================================================

if __name__ == "__main__":
    main()

