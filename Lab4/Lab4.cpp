#include <iostream>
#include <cstdint>
#include <string>
#include <cstring>

typedef struct {
    uint32_t p[18];
    uint32_t sbox[4][256];
} BlowfishContext;


// Прототипы функций
void EncryptBlock(const BlowfishContext& ctx, uint32_t& l, uint32_t& r);
void DecryptBlock(const BlowfishContext& ctx, uint32_t& l, uint32_t& r);


std::string decryptedString; // Переменная для хранения полной расшифрованной строки

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
        result += static_cast<char>(byteArray[i]);
    }
    return result;
}


int main() {
    uint8_t key[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    BlowfishContext ctx;
    ExpandKey(ctx, key, sizeof(key));

    std::string plaintext = R"TEXT(А судьи кто? – За древностию лет 
К свободной жизни их вражда непримирима,
Сужденья черпают из забытых газет
Времён Очаковских и покоренья Крыма;
Всегда готовые к журьбе,
Поют все песнь одну и ту же,
Не замечая об себе:
Что старее, то хуже.
Где? укажите нам, отечества отцы,
Которых мы должны принять за образцы?
Не эти ли, грабительством богаты?
Защиту от суда в друзьях нашли, в родстве,
Великолепные соорудя палаты,
Где разливаются в пирах и мотовстве,
И где не воскресят клиенты-иностранцы
Прошедшего житья подлейшие черты.
Да и кому в Москве не зажимали рты
Обеды, ужины и танцы?
Не тот ли, вы к кому меня ещё с пелен,
Для замыслов каких-то непонятных,
Дитёй возили на поклон?
Тот Нестор негодяев знатных,
Толпою окружённый слуг;
Усердствуя, они в часы вина и драки
И честь, и жизнь его не раз спасали: вдруг
На них он выменял борзые три собаки!!!
Или вон тот ещё, который для затей
На крепостной балет согнал на многих фурах
От матерей, отцов отторженных детей?!
Сам погружён умом в Зефирах и в Амурах,
Заставил всю Москву дивиться их красе!
Но должников не согласил к отсрочке:
Амуры и Зефиры все
Распроданы поодиночке!!!
Вот те, которые дожили до седин!
Вот уважать кого должны мы на безлюдьи!
Вот наши строгие ценители и судьи!
Теперь пускай из нас один,
Из молодых людей, найдётся – враг исканий,
Не требуя ни мест, ни повышенья в чин,
В науки он вперит ум, алчущий познаний;
Или в душе его сам бог возбудит жар
К искусствам творческим, высоким и прекрасным, –
Они тотчас: разбой! пожар!
И прослывёт у них мечтателем! опасным!! –
Мундир! один мундир! он в прежнем их быту
Когда-то укрывал, расшитый и красивый,
Их слабодушие, рассудка нищету;
И нам за ними в путь счастливый!
И в жёнах, дочерях – к мундиру та же страсть!
Я сам к нему давно ль от нежности отрёкся?!
Теперь уж в это мне ребячество не впасть;
Но кто б тогда за всеми не повлекся?
Когда из гвардии, иные от двора
Сюда на время приезжали, –
Кричали женщины: ура!
И в воздух чепчики бросали!)TEXT";
    // Дополнение строки до кратного 8
    while (plaintext.length() % 8 != 0) {
        plaintext += ' ';
    }

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



    // Дешифрование и добавление каждого блока к полной строке
    for (size_t i = 0; i < plaintext.length(); i += 8) {
        DecryptBlock(ctx, l, r);
        uint8_t decryptedBlock[8];
        decryptedBlock[0] = (l >> 24) & 0xFF;
        decryptedBlock[1] = (l >> 16) & 0xFF;
        decryptedBlock[2] = (l >> 8) & 0xFF;
        decryptedBlock[3] = l & 0xFF;
        decryptedBlock[4] = (r >> 24) & 0xFF;
        decryptedBlock[5] = (r >> 16) & 0xFF;
        decryptedBlock[6] = (r >> 8) & 0xFF;
        decryptedBlock[7] = r & 0xFF;
        decryptedString += ByteArrayToString(decryptedBlock, 8);
    }



    std::string decryptedString = ByteArrayToString(plaintextBytes, plaintext.length());
    // Удаление дополнительных символов пробелов в конце строки
    std::cout << "Decrypted string: " << decryptedString << std::endl;

    delete[] plaintextBytes;

    return 0;
}