from shift_cipher import decrypt

ENGLISH_FREQUENCY = {
    'a': 8.167,
    'b': 1.492,
    'c': 2.782,
    'd': 4.253,
    'e': 12.702,
    'f': 2.228,
    'g': 2.015,
    'h': 6.094,
    'i': 6.966,
    'j': 0.153,
    'k': 0.772,
    'l': 4.025,
    'm': 2.406,
    'n': 6.749,
    'o': 7.507,
    'p': 1.929,
    'q': 0.095,
    'r': 5.987,
    's': 6.327,
    't': 9.056,
    'u': 2.758,
    'v': 0.978,
    'w': 2.360,
    'x': 0.150,
    'y': 1.974,
    'z': 0.074
}


def chi_square_score(text):
    letters = [c.lower() for c in text if c.isalpha()]
    total = len(letters)

    if total == 0:
        return float("inf")

    counts = {letter: 0 for letter in ENGLISH_FREQUENCY}

    for letter in letters:
        counts[letter] += 1

    score = 0

    for letter in ENGLISH_FREQUENCY:
        observed = counts[letter]
        expected = (ENGLISH_FREQUENCY[letter] / 100) * total

        if expected > 0:
            score += ((observed - expected) ** 2) / expected

    return score


def attack(ciphertext):
    best_key = 0
    lowest_score = float("inf")
    best_plaintext = ""

    for key in range(26):
        plaintext = decrypt(ciphertext, key)
        score = chi_square_score(plaintext)

        print(
            f"Key {key:2}: "
            f"Chi-Square = {score:.2f} : {plaintext}"
        )

        if score < lowest_score:
            lowest_score = score
            best_key = key
            best_plaintext = plaintext

    return best_key, best_plaintext, lowest_score


if __name__ == "__main__":
    ciphertext = input("Enter ciphertext: ")

    key, plaintext, score = attack(ciphertext)

    print("\nBest Key:", key)
    print("Best Plaintext:", plaintext)
    print("Chi-Square Score:", score)
