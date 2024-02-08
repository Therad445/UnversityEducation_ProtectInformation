#include <iostream>
#include <fstream>
#include <vector>

void encryptFile(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile) {
    std::ifstream input(inputFile, std::ios::binary);
    std::ifstream key(keyFile, std::ios::binary);
    std::ofstream output(outputFile, std::ios::binary);

    if (!input.is_open() || !key.is_open() || !output.is_open()) {
        std::cout << "Ошибка открытия файлов." << std::endl;
        return;
    }

    std::vector<unsigned char> substitution(256);
    unsigned char byte;
    while (key.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        substitution.push_back(byte);
    }

    while (input.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        byte = substitution[byte];
        output.write(reinterpret_cast<char*>(&byte), sizeof(byte));
    }

    input.close();
    key.close();
    output.close();

    std::cout << "Файл успешно зашифрован." << std::endl;
}

void decryptFile(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile) {
    std::ifstream input(inputFile, std::ios::binary);
    std::ifstream key(keyFile, std::ios::binary);
    std::ofstream output(outputFile, std::ios::binary);

    if (!input.is_open() || !key.is_open() || !output.is_open()) {
        std::cout << "Ошибка открытия файлов." << std::endl;
        return;
    }

    std::vector<unsigned char> substitution(256);
    unsigned char byte;
    while (key.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        substitution.push_back(byte);
    }

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            if (substitution[j] == i) {
                substitution[i] = j;
                break;
            }
        }
    }

    while (input.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        byte = substitution[byte];
        output.write(reinterpret_cast<char*>(&byte), sizeof(byte));
    }

    input.close();
    key.close();
    output.close();

    std::cout << "Файл успешно расшифрован." << std::endl;
}

int main() {
    std::string inputFile = "input.txt";
    std::string keyFile = "key.txt";
    std::string encryptedFile = "encrypted.txt";
    std::string decryptedFile = "decrypted.txt";

    encryptFile(inputFile, keyFile, encryptedFile);
    decryptFile(encryptedFile, keyFile, decryptedFile);

    return 0;
}
