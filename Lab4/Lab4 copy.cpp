#include <iostream>
#include <cstdint>
#include <string>

typedef struct {
    uint32_t p[18];
    uint32_t sbox[4][256];
} BlowfishContext;


// Прототипы функций
void EncryptBlock(const BlowfishContext& ctx, uint32_t& l, uint32_t& r);
void DecryptBlock(const BlowfishContext& ctx, uint32_t& l, uint32_t& r);

// Функция F
uint32_t F(const BlowfishContext& ctx, uint32_t x) {
    uint8_t a = x >> 24;
    uint8_t b = x >> 16;
    uint8_t c = x >> 8;
    uint8_t d = x;

    uint32_t result = (ctx.sbox[0][a] + ctx.sbox[1][b]) ^ ctx.sbox[2][c];
    return result + ctx.sbox[3][d];
}

// Расширение ключа
void ExpandKey(BlowfishContext& ctx, const uint8_t* key, size_t keyLength) {
    // Инициализация P-блока и S-блоков с использованием числа PI
    for (int i = 0; i < 18; ++i) {
        ctx.p[i] = 0xFFFFFFFF;
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 256; ++j) {
            ctx.sbox[i][j] = 0xFFFFFFFF;
        }
    }

    // Копирование исходного ключа в раундовые ключи
    size_t keyIndex = 0;
    for (int i = 0; i < 18; ++i) {
        uint32_t keyPart = 0;
        for (int j = 0; j < 4; ++j) {
            keyPart = (keyPart << 8) | key[keyIndex++ % keyLength];
        }
        ctx.p[i] ^= keyPart;
    }

    // Шифрование раундовых ключей и S-блоков с помощью сети Фейстеля
    uint32_t l = 0, r = 0;
    for (int i = 0; i < 18; i += 2) {
        EncryptBlock(ctx, l, r);
        ctx.p[i] = l;
        ctx.p[i + 1] = r;
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 256; j += 2) {
            EncryptBlock(ctx, l, r);
            ctx.sbox[i][j] = l;
            ctx.sbox[i][j + 1] = r;
        }
    }
}

// Шифрование блока данных
void EncryptBlock(const BlowfishContext& ctx, uint32_t& l, uint32_t& r) {
    for (int i = 0; i < 16; ++i) {
        l ^= ctx.p[i];
        r ^= F(ctx, l);
        std::swap(l, r);
    }
    std::swap(l, r);
    r ^= ctx.p[16];
    l ^= ctx.p[17];
}

// Дешифрование блока данных
void DecryptBlock(const BlowfishContext& ctx, uint32_t& l, uint32_t& r) {
    for (int i = 17; i > 1; --i) {
        l ^= ctx.p[i];
        r ^= F(ctx, l);
        std::swap(l, r);
    }
    std::swap(l, r);
    r ^= ctx.p[1];
    l ^= ctx.p[0];
}

void StringToByteArray(const std::string& str, uint8_t* byteArray) {
    for (size_t i = 0; i < str.length(); ++i) {
        byteArray[i] = static_cast<uint8_t>(str[i]);
    }
}

std::string ByteArrayToString(const uint8_t* byteArray, size_t length) {
    std::string result;
    for (size_t i = 0; i < length; ++i) {
        if (byteArray[i] != 0) {
            result += static_cast<char>(byteArray[i]);
        }
    }
    return result;
}


int main() {
    uint8_t key[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    BlowfishContext ctx;
    ExpandKey(ctx, key, sizeof(key));

    std::string plaintext = "Hello, World!";
    uint8_t* plaintextBytes = new uint8_t[plaintext.length()];
    StringToByteArray(plaintext, plaintextBytes);

    std::cout << "Original string: " << plaintext << std::endl;

    // Шифрование
    uint32_t l = 0, r = 0;
    for (size_t i = 0; i < plaintext.length(); i += 8) {
        l = (plaintextBytes[i] << 24) | (plaintextBytes[i + 1] << 16) |
            (plaintextBytes[i + 2] << 8) | plaintextBytes[i + 3];
        r = (plaintextBytes[i + 4] << 24) | (plaintextBytes[i + 5] << 16) |
            (plaintextBytes[i + 6] << 8) | plaintextBytes[i + 7];
        EncryptBlock(ctx, l, r);
        std::cout << "Encrypted block: " << std::hex << l << " " << r << std::endl;
    }

    // Дешифрование
    for (size_t i = 0; i < plaintext.length(); i += 8) {
        DecryptBlock(ctx, l, r);
        plaintextBytes[i] = (l >> 24) & 0xFF;
        plaintextBytes[i + 1] = (l >> 16) & 0xFF;
        plaintextBytes[i + 2] = (l >> 8) & 0xFF;
        plaintextBytes[i + 3] = l & 0xFF;
        plaintextBytes[i + 4] = (r >> 24) & 0xFF;
        plaintextBytes[i + 5] = (r >> 16) & 0xFF;
        plaintextBytes[i + 6] = (r >> 8) & 0xFF;
        plaintextBytes[i + 7] = r & 0xFF;
    }

    std::string decryptedString = ByteArrayToString(plaintextBytes, plaintext.length());
    std::cout << "Decrypted string: " << decryptedString << std::endl;

    delete[] plaintextBytes;

    return 0;
}
