// Задание
// Написать программу для сокрытия/(извлечения сокрытой) информации
// в bmp файле (используется True Color). Формат сокрытия 1 байт
// скрываемой информации скрывается в одном пикселе (по 2 бита на
// цвет(6 бит) и еще 2 бита на неиспользуемый в True Color альфа канал).
// Кодируется следующим образом:
// void hide_byte_into_pixel(RGBQUAD *pixel, uint8_t hide_byte)
// {
// pixel->rgbBlue &= (0xFC);
// pixel->rgbBlue |= (hide_byte >> 6) & 0x3;
// pixel->rgbGreen &= (0xFC);
// pixel->rgbGreen |= (hide_byte >> 4) & 0x3;
// pixel->rgbRed &= (0xFC);
// pixel->rgbRed |= (hide_byte >> 2) & 0x3;
// pixel->rgbReserved &= (0xFC);
// pixel->rgbReserved |= (hide_byte) & 0x3;
// }
// Признаком конца файла считать EOF (0xFF). Во вложении есть bmp
// файлы, в которых спрятаны txt файлы. Вариант задания определяется
// остатком от деления на 10. 

#include <stdio.h>
#include <stdint.h>

// Структура для хранения заголовка bmp файла
typedef struct {
    uint16_t type;            // Сигнатура файла, должна быть равна 'BM'
    uint32_t size;            // Размер файла в байтах
    uint16_t reserved1;       // Зарезервированное значение (0)
    uint16_t reserved2;       // Зарезервированное значение (0)
    uint32_t offset;          // Смещение до начала изображения в байтах
} BMPHeader;

// Структура для хранения информации о пикселях
typedef struct {
    uint8_t rgbBlue;          // Синий цвет компонента пикселя
    uint8_t rgbGreen;         // Зеленый цвет компонента пикселя
    uint8_t rgbRed;           // Красный цвет компонента пикселя
    uint8_t rgbReserved;      // Зарезервированное значение (неиспользуемый в True Color альфа канал)
} RGBQUAD;

// Функция для скрытия байта информации в одном пикселе
void hide_byte_into_pixel(RGBQUAD *pixel, uint8_t hide_byte) {
    pixel->rgbBlue &= (0xFC);            // Скрытие информации в синем цвете
    pixel->rgbBlue |= (hide_byte >> 6) & 0x3;
    pixel->rgbGreen &= (0xFC);           // Скрытие информации в зеленом цвете
    pixel->rgbGreen |= (hide_byte >> 4) & 0x3;
    pixel->rgbRed &= (0xFC);             // Скрытие информации в красном цвете
    pixel->rgbRed |= (hide_byte >> 2) & 0x3;
    pixel->rgbReserved &= (0xFC);        // Скрытие информации в зарезервированном значении
    pixel->rgbReserved |= (hide_byte) & 0x3;
}

// Функция для извлечения скрытой информации из одного пикселя
uint8_t extract_byte_from_pixel(RGBQUAD *pixel) {
    uint8_t hide_byte = 0;
    hide_byte |= (pixel->rgbBlue & 0x3) << 6;           // Извлечение информации из синего цвета
    hide_byte |= (pixel->rgbGreen & 0x3) << 4;          // Извлечение информации из зеленого цвета
    hide_byte |= (pixel->rgbRed & 0x3) << 2;            // Извлечение информации из красного цвета
    hide_byte |= (pixel->rgbReserved & 0x3);            // Извлечение информации из зарезервированного значения
    return hide_byte;
}

// Функция для скрытия информации в bmp файле
void hide_data_in_bmp(const char *bmp_filename, const char *data_filename) {
    FILE *bmp_file = fopen(bmp_filename, "rb+");
    if (bmp_file == NULL) {
        printf("Ошибка при открытии bmp файла\n");
        return;
    }

    FILE *data_file = fopen(data_filename, "rb");
    if (data_file == NULL) {
        printf("Ошибка при открытии файла данных\n");
        fclose(bmp_file);
        return;
    }

    // Читаем заголовок bmp файла
    BMPHeader header;
    fread(&header, sizeof(header), 1, bmp_file);

    // Перемещаем указатель в файле к началу изображения
    fseek(bmp_file, header.offset, SEEK_SET);

    // Считываем данные из файла и скрываем их в bmp изображении
    int byte;
    RGBQUAD pixel;
    while ((byte = fgetc(data_file)) != EOF) {
        fread(&pixel, sizeof(pixel), 1, bmp_file);
        hide_byte_into_pixel(&pixel, byte);
        fseek(bmp_file, -sizeof(pixel), SEEK_CUR);
        fwrite(&pixel, sizeof(pixel), 1, bmp_file);
    }

    // В конце добавляем признак конца файла (EOF)
    byte = 0xFF;
    for (int i = 0; i < 4; i++) {
        fread(&pixel, sizeof(pixel), 1, bmp_file);
        hide_byte_into_pixel(&pixel, byte);
        fseek(bmp_file, -sizeof(pixel), SEEK_CUR);
        fwrite(&pixel, sizeof(pixel), 1, bmp_file);
    }

    fclose(bmp_file);
    fclose(data_file);
}

// Функция для извлечения скрытой информации из bmp файла
void extract_data_from_bmp(const char *bmp_filename, const char *output_filename) {
    FILE *bmp_file = fopen(bmp_filename, "rb");
    if (bmp_file == NULL) {
        printf("Ошибка при открытии bmp файла\n");
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        printf("Ошибка при создании файла для извлеченных данных\n");
        fclose(bmp_file);
        return;
    }

    // Читаем заголовок bmp файла
    BMPHeader header;
    fread(&header, sizeof(header), 1, bmp_file);

    // Перемещаем указатель в файле к началу изображения
    fseek(bmp_file, header.offset, SEEK_SET);

    // Извлекаем данные из bmp файла
    uint8_t byte;
    RGBQUAD pixel;
    while (1) {
        fread(&pixel, sizeof(pixel), 1, bmp_file);
        byte = extract_byte_from_pixel(&pixel);
        if (byte == 0xFF) {
            break; // Достигнут признак конца файла
        }
        fwrite(&byte, sizeof(byte), 1, output_file);
    }

    fclose(bmp_file);
    fclose(output_file);
}

int main() {
    // Вызов функции для скрытия информации в bmp файле
    hide_data_in_bmp("image.bmp", "data.txt");

    // Вызов функции для извлечения скрытой информации из bmp файла
    extract_data_from_bmp("image.bmp", "extracted_data.txt");

    return 0;
}