def encrypt(text, key):
    result = ""

    for char in text:
        if char.isalpha():
            base = ord('A') if char.isupper() else ord('a')
            result += chr((ord(char) - base + key) % 26 + base)
        else:
            result += char

    return result


def decrypt(text, key):
    return encrypt(text, -key)


if __name__ == "__main__":
    plaintext = input("Enter plaintext: ")
    key = int(input("Enter key (0-25): "))

    ciphertext = encrypt(plaintext, key)

    print("Ciphertext:", ciphertext)
    print("Decrypted:", decrypt(ciphertext, key))
