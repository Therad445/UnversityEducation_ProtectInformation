// #include <iostream>
// #include <fstream>
// #include <vector>

// void encryptFile(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile) {
//     std::ifstream input(inputFile, std::ios::binary);
//     std::ifstream key(keyFile, std::ios::binary);
//     std::ofstream output(outputFile, std::ios::binary);

//     if (!input.is_open() || !key.is_open() || !output.is_open()) {
//         std::cout << "Ошибка открытия файлов." << std::endl;
//         return;
//     }

//     std::vector<unsigned char> substitution(256);
//     unsigned char byte;
//     while (key.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
//         substitution.push_back(byte);
//     }

//     while (input.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
//         byte = substitution[byte];
//         output.write(reinterpret_cast<char*>(&byte), sizeof(byte));
//     }

//     input.close();
//     key.close();
//     output.close();

//     std::cout << "Файл успешно зашифрован." << std::endl;
// }

// void decryptFile(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile) {
//     std::ifstream input(inputFile, std::ios::binary);
//     std::ifstream key(keyFile, std::ios::binary);
//     std::ofstream output(outputFile, std::ios::binary);

//     if (!input.is_open() || !key.is_open() || !output.is_open()) {
//         std::cout << "Ошибка открытия файлов." << std::endl;
//         return;
//     }

//     std::vector<unsigned char> substitution(256);
//     unsigned char byte;
//     while (key.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
//         substitution.push_back(byte);
//     }

//     for (int i = 0; i < 256; i++) {
//         for (int j = 0; j < 256; j++) {
//             if (substitution[j] == i) {
//                 substitution[i] = j;
//                 break;
//             }
//         }
//     }

//     while (input.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
//         byte = substitution[byte];
//         output.write(reinterpret_cast<char*>(&byte), sizeof(byte));
//     }

//     input.close();
//     key.close();
//     output.close();

//     std::cout << "Файл успешно расшифрован." << std::endl;
// }

// int main() {
//     std::string inputFile = "input.txt";
//     std::string keyFile = "key.txt";
//     std::string encryptedFile = "encrypted.txt";
//     std::string decryptedFile = "decrypted.txt";

//     encryptFile(inputFile, keyFile, encryptedFile);
//     decryptFile(encryptedFile, keyFile, decryptedFile);

//     return 0;
// }




// #include <iostream>
// #include <fstream>
// #include <vector>

// // Функция для чтения ключа из файла
// std::vector<unsigned char> readKeyFromFile(const std::string& filename) {
//     std::ifstream file(filename, std::ios::binary);
//     if (!file) {
//         std::cerr << "Ошибка открытия файла " << filename << std::endl;
//         exit(1);
//     }

//     // Чтение ключа из файла
//     std::vector<unsigned char> key;
//     unsigned char byte;
//     while (file.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
//         key.push_back(byte);
//     }

//     return key;
// }

// // Функция для шифрования или расшифрования файла
// void processFile(const std::string& inputFile, const std::string& outputFile, const std::vector<unsigned char>& key, bool encrypt) {
//     std::ifstream inFile(inputFile, std::ios::binary);
//     if (!inFile) {
//         std::cerr << "Ошибка открытия файла " << inputFile << std::endl;
//         exit(1);
//     }

//     std::ofstream outFile(outputFile, std::ios::binary);
//     if (!outFile) {
//         std::cerr << "Ошибка открытия файла " << outputFile << std::endl;
//         exit(1);
//     }

//     unsigned char byte;
//     while (inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
//         if (encrypt) {
//             // Шифрование: замена каждого байта с использованием ключа
//             byte = key[byte];
//         } else {
//             // Расшифрование: замена каждого байта обратно с использованием ключа
//             for (size_t i = 0; i < key.size(); i++) {
//                 if (key[i] == byte) {
//                     byte = static_cast<unsigned char>(i);
//                     break;
//                 }
//             }
//         }
//         outFile.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
//     }

//     std::cout << (encrypt ? "Шифрование" : "Расшифрование") << " завершено. Результат сохранен в файле " << outputFile << std::endl;
// }

// int main() {
//     std::string inputFile, outputFile, keyFile;
//     std::cout << "Введите имя входного файла: ";
//     std::cin >> inputFile;
//     std::cout << "Введите имя выходного файла: ";
//     std::cin >> outputFile;
//     std::cout << "Введите имя файла с ключом: ";
//     std::cin >> keyFile;

//     std::vector<unsigned char> key = readKeyFromFile(keyFile);

//     char mode;
//     std::cout << "Выберите режим (e - шифрование, d - расшифрование): ";
//     std::cin >> mode;

//     bool encrypt = (mode == 'e' || mode == 'E');

//     processFile(inputFile, outputFile, key, encrypt);

//     return 0;
// }


// #include <iostream>
// #include <fstream>
// using namespace std;

// int main(int argc, char* argv[]) {
//     string text = "Hello it's PC";

// }

// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <string>

// using namespace std;

// void encryptDecryptFile(const string& inputFile, const string& outputFile, const string& keyFile, bool encryptMode) {
//     ifstream input(inputFile, ios::binary);
//     ofstream output(outputFile, ios::binary);
//     ifstream key(keyFile);

//     if (!input.is_open() || !output.is_open() || !key.is_open()) {
//         cout << "Error opening files." << endl;
//         return;
//     }

//     // Read key from file
//     vector<int> substitution;
//     int byte;
//     while (key >> byte) {
//         substitution.push_back(byte);
//     }
//     key.close();

//     // Encrypt or decrypt file
//     while (!input.eof()) {
//         char byte;
//         input.read(&byte, 1);
//         if (input.eof()) break;
        
//         if (encryptMode) {
//             output.put(substitution[static_cast<unsigned char>(byte)]);
//         } else {
//             for (size_t i = 0; i < substitution.size(); ++i) {
//                 if (substitution[i] == static_cast<unsigned char>(byte)) {
//                     output.put(i);
//                     break;
//                 }
//             }
//         }
//     }

//     input.close();
//     output.close();
// }

// int main() {
//     string inputFile, encrypteFile, outputFile, keyFile;
//     char mode;

//     // cout << "Enter input file name: ";
//     // cin >> inputFile;
//     // cout << "Enter output file name: ";
//     // cin >> outputFile;
//     // cout << "Enter key file name: ";
//     // cin >> keyFile;
//     // cout << "Enter 'e' for encryption or 'd' for decryption: ";
//     // cin >> mode;

//     // bool encryptMode = (mode == 'e' || mode == 'E');

//     // encryptDecryptFile(inputFile, outputFile, keyFile, encryptMode);

//     inputFile = "input.txt";
//     encrypteFile = "encrypted.txt";
//     outputFile = "output.txt";
//     encryptDecryptFile(inputFile, encrypteFile, keyFile, true);
//     encryptDecryptFile(encrypteFile, outputFile, keyFile, false);

//     cout << "Operation completed successfully." << endl;

//     return 0;
// }

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

void encryptFile() {
    const string inputFile = "input.txt";
    const string outputFile = "encrypted.txt";
    const string keyFile = "key.txt";

    ifstream input(inputFile, ios::binary);
    ofstream output(outputFile, ios::binary);
    ifstream key(keyFile);

    if (!input.is_open() || !output.is_open() || !key.is_open()) {
        cout << "Error opening files." << endl;
        return;
    }

    // Read key from file
    vector<int> substitution;
    int byte;
    while (key >> byte) {
        substitution.push_back(byte);
    }
    key.close();

    // Encrypt file
    while (!input.eof()) {
        char byte;
        input.read(&byte, 1);
        if (input.eof()) break;

        output.put(substitution[static_cast<unsigned char>(byte)]);
    }

    input.close();
    output.close();

    cout << "Encryption completed successfully." << endl;
}

void decryptFile() {
    const string inputFile = "encrypted.txt";
    const string outputFile = "decrypted.txt";
    const string keyFile = "key.txt";

    ifstream input(inputFile, ios::binary);
    ofstream output(outputFile, ios::binary);
    ifstream key(keyFile);

    if (!input.is_open() || !output.is_open() || !key.is_open()) {
        cout << "Error opening files." << endl;
        return;
    }

    // Read key from file
    vector<int> substitution;
    int byte;
    while (key >> byte) {
        substitution.push_back(byte);
    }
    key.close();

    // Decrypt file
    while (!input.eof()) {
        char byte;
        input.read(&byte, 1);
        if (input.eof()) break;
        
        for (size_t i = 0; i < substitution.size(); ++i) {
            if (substitution[i] == static_cast<unsigned char>(byte)) {
                output.put(i);
                break;
            }
        }
    }

    input.close();
    output.close();

    cout << "Decryption completed successfully." << endl;
}

int main() {
    char mode;

    // cout << "Enter 'e' for encryption or 'd' for decryption: ";
    // cin >> mode;

    // if (mode == 'e' || mode == 'E') {
    //     encryptFile();
    // } else if (mode == 'd' || mode == 'D') {
    //     decryptFile();
    // } else {
    //     cout << "Invalid mode. Please enter 'e' or 'd'." << endl;
    // }
    encryptFile();
    decryptFile();
    return 0;
}
