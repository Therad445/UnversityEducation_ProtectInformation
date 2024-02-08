#include <iostream>
#include <fstream>
#include <map>
#include <iomanip>

int main() {
    std::ifstream file("Doc1.docx", std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла." << std::endl;
        return 1;
    }

    std::map<unsigned char, int> frequencies;
    int totalBytes = 0;

    unsigned char byte;
    while (file.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        frequencies[byte]++;
        totalBytes++;
    }

    file.close();

    std::cout << "Относительные частоты появления каждого байта:" << std::endl;

    for (const auto& pair : frequencies) {
        unsigned char byte = pair.first;
        int count = pair.second;
        double frequency = static_cast<double>(count) / totalBytes;
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << ": "
                  << std::dec << std::fixed << std::setprecision(2) << frequency << std::endl;
    }

    return 0;
}
