from shift_cipher import decrypt


def load_dictionary(filename):
    with open(filename, "r") as file:
        return set(word.strip().lower() for word in file)


def dictionary_score(text, dictionary):
    words = text.lower().split()
    return sum(1 for word in words if word.strip(".,!?;:'\"") in dictionary)


def attack(ciphertext, dictionary_file):
    dictionary = load_dictionary(dictionary_file)

    best_key = 0
    best_score = -1
    best_plaintext = ""

    for key in range(26):
        plaintext = decrypt(ciphertext, key)
        score = dictionary_score(plaintext, dictionary)

        print(f"Key {key:2}: Score = {score:2} : {plaintext}")

        if score > best_score:
            best_score = score
            best_key = key
            best_plaintext = plaintext

    return best_key, best_plaintext, best_score


if __name__ == "__main__":
    ciphertext = input("Enter ciphertext: ")

    dictionary_file = "../dictionary/english_words.txt"

    key, plaintext, score = attack(ciphertext, dictionary_file)

    print("\nBest Key:", key)
    print("Best Plaintext:", plaintext)
    print("Dictionary Score:", score)
