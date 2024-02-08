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