# # from Crypto.Cipher import Blowfish
# # from Crypto import Random

# # def encrypt_message(message, key):
# #     # Генерация случайного вектора инициализации
# #     iv = Random.new().read(Blowfish.block_size)
# #     cipher = Blowfish.new(key, Blowfish.MODE_CBC, iv)
# #     # Дополнение сообщения до кратности блоку шифрования
# #     length = Blowfish.block_size - (len(message) % Blowfish.block_size)
# #     message += bytes([length]) * length
# #     return iv + cipher.encrypt(message)

# # def decrypt_message(encrypted_message, key):
# #     iv = encrypted_message[:Blowfish.block_size]
# #     cipher = Blowfish.new(key, Blowfish.MODE_CBC, iv)
# #     decrypted_message = cipher.decrypt(encrypted_message[Blowfish.block_size:])
# #     # Удаление дополнения
# #     padding_length = decrypted_message[-1]
# #     return decrypted_message[:-padding_length]

# # # Пример использования
# # key = b'P@ssw0rd'
# # message = b'Hello, World!'
# # encrypted_message = encrypt_message(message, key)
# # print("Encrypted message:", encrypted_message)
# # decrypted_message = decrypt_message(encrypted_message, key)
# # print("Decrypted message:", decrypted_message.decode())


# # # Ошибка в представлении сети Фейстеля может быть вызвана различными проблемами в реализации, включая, но не ограничиваясь:

# # # Ошибка в ключе: Неправильное управление ключом может привести к небезопасной работе шифра. Например, использование слабого ключа или неправильная обработка ключей может угрожать безопасности.

# # # Недостаточное количество раундов: Сеть Фейстеля требует нескольких раундов, чтобы обеспечить хорошую криптографическую стойкость. Недостаточное количество раундов может ослабить безопасность.

# # # Неправильное управление дополнения: Если в схеме используется дополнение, его обработка должна быть правильной и безопасной.

# # # Утечка информации: Некорректное управление временем выполнения или обработка ошибок может привести к утечке информации о ключе или сообщении.



# import struct

# # S-блоки (матрицы подстановки)
# S_BOXES = [
#     [0x243f6a88, 0x85a308d3, 0x13198a2e, 0x03707344, 0xa4093822, 0x299f31d0, 0x082efa98, 0xec4e6c89,
#      0x452821e6, 0x38d01377, 0xbe5466cf, 0x34e90c6c, 0xc0ac29b7, 0xc97c50dd, 0x3f84d5b5, 0xb5470917,
#      0x9216d5d9, 0x8979fb1b],
#     [0xd1310ba6, 0x98dfb5ac, 0x2ffd72db, 0xd01adfb7, 0xb8e1afed, 0x6a267e96, 0xba7c9045, 0xf12c7f99,
#      0x24a19947, 0xb3916cf7, 0x0801f2e2, 0x858efc16, 0x636920d8, 0x71574e69, 0xa458fea3, 0xf4933d7e,
#      0x0d95748f, 0x728eb658],
#     [0x718bcd58, 0x82154aee, 0x7b54a41d, 0xc25a59b5, 0x9c30d539, 0x2af26013, 0xc5d1b023, 0x286085f0,
#      0xca417918, 0xb8db38ef, 0x8e79dcb0, 0x603a180e, 0x6c9e0e8b, 0xb01e8a3e, 0xd71577c1, 0xbd314b27,
#      0x78af2fda, 0x55605c60],
#     [0xe65525f3, 0xaa55ab94, 0x57489862, 0x63e81440, 0x55ca396a, 0x2aab10b6, 0xb4cc5c34, 0x1141e8ce,
#      0xa15486af, 0x7c72e993, 0xb3ee1411, 0x636fbc2a, 0x2ba9c55d, 0x741831f6, 0xce5c3e16, 0x9b87931e,
#      0xafd6ba33, 0x6c24cf5c],
#     [0x7a325381, 0x28958677, 0x3b8f4898, 0x6b4bb9af, 0xc4bfe81b, 0x66282193, 0x61d809cc, 0xfb21a991,
#      0x487cac60, 0x5dec8032, 0xef845d5d, 0xe98575b1, 0xdc262302, 0xeb651b88, 0x23893e81, 0xd396acc5,
#      0x0f6d6ff3, 0x83f44239],
#     [0x2e0b4482, 0xa4842004, 0x69c8f04a, 0x9e1f9b5e, 0x21c66842, 0xf6e96c9a, 0x670c9c61, 0xabd388f0,
#      0x6a51a0d2, 0xd8542f68, 0x960fa728, 0xab5133a3, 0x6eef0b6c, 0x137a3be4, 0xba3bf050, 0x7efb2a98,
#      0xa1f1651d, 0x39af0176],
#     [0x66ca593e, 0x82430e88, 0x8cee8619, 0x456f9fb4, 0x7d84a5c3, 0x3b8b5ebe, 0xe06f75d8, 0x85c12073,
#      0x401a449f, 0x56c16aa6, 0x4ed3aa62, 0x363f7706, 0x1bfedf72, 0x429b023d, 0x37d0d724, 0xd00a1248,
#      0xdb0fead3, 0x49f1c09b],
#     [0x075372c9, 0x80991b7b, 0x25d479d8, 0xf6e8def7, 0xe3fe501a, 0xb6794c3b, 0x976ce0bd, 0x04c006ba,
#      0xc1a94fb6, 0x409f60c4, 0x5e5c9ec2, 0x196a2463, 0x68fb6faf, 0x3e6c53b5, 0x1339b2eb, 0x3b52ec6f,
#      0x6dfc511f, 0x9b30952c],
#     [0xcc814544, 0xaf5ebd09, 0xbee3d004, 0xde334afd, 0x660f2807, 0x192e4bb3, 0xc0cba857, 0x45c8740f,
#      0xd20b5f39, 0xb9d3fbdb, 0x5579c0bd, 0x1a60320a, 0xd6a100c6, 0x402c7279, 0x679f25fe, 0xfb1fa3cc,
#      0x8ea5e9f8, 0xdb3222f8],
#     [0x3c7516df, 0xfd616b15, 0x2f501ec8, 0xad0552ab, 0x323db5fa, 0xfd238760, 0x53317b48, 0x3e00df82,
#      0x9e5c57bb, 0xca6f8ca0, 0x1a87562e, 0xdf1769db, 0xd542a8f6, 0x287effc3, 0xac6732c6, 0x8c4f5573,
#      0x695b27b0, 0xbbca58c8],
#     [0xe1ffa35d, 0xb8f011a0, 0x10fa3d98, 0xfd2183b8, 0x4afcb56c, 0x2dd1d35b, 0x9a53e479, 0xb6f84565,
#      0xd28e49bc, 0x4bfb9790, 0xe1ddf2da, 0xa4cb7e33, 0x62fb1341, 0xcee4c6e8, 0xef20cada, 0x36774c01,
#      0xd07e9efe, 0x2bf11fb4],
#     [0x95dbda4d, 0xae909198, 0xeaad8e71, 0x6b93d5a0, 0xd08ed1d0, 0xafc725e0, 0x8e3c5b2f, 0x8e7594b7,
#      0x8ff6e2fb, 0xf2122b64, 0x8888b812, 0x900df01c, 0x4fad5ea0, 0x688fc31c, 0xd1cff191, 0xb3a8c1ad,
#      0x2f2f2218, 0xbe0e1777],
#     [0xea752dfe, 0x8b021fa1, 0xe5a0cc0f, 0xb56f74e8, 0x18acf3d6, 0xce89e299, 0xb4a84fe0, 0xfd13e0b7,
#      0x7cc43b81, 0xd2ada8d9, 0x165fa266, 0x80957705, 0x93cc7314, 0x211a1477, 0xe6ad2065, 0x77b5fa86,
#      0xc75442f5, 0xfb9d35cf],
#     [0xebcdaf0c, 0x7b3e89a0, 0xd6411bd3, 0xae1e7e49, 0x00250e2d, 0x2071b35e, 0x226800bb, 0x57b8e0af,
#      0x2464369b, 0xf009b91e, 0x5563911d, 0x59dfa6aa, 0x78c14389, 0xd95a537f, 0x207d5ba2, 0x02e5b9c5,
#      0x83260376, 0x6295cfa9],
#     [0x11c81968, 0x80fa9b69, 0x3eb5a8c0, 0xe3f82d0d, 0x513742ab, 0x7e3f36d2, 0x83146cb9, 0x845840df,
#      0xbafd6bc5, 0xd1b1e24e, 0xabcfaebc, 0x3a66125f, 0xe6c9ea10, 0xdbefff82, 0xd5bfdfae, 0x5cb0a9dc,
#      0x9b9e9ab8, 0x8fc5d85c],
#     [0xa54ff53a, 0x5f1d36f1, 0x585cde95, 0x9c30d539, 0x77d4ef4d, 0x89e5ae6b, 0xcfea2660, 0x71283e1f,
#      0x6a9b4afe, 0xbaf8f190, 0x6c6a9b0b, 0x5a13057d, 0x50b88f4b, 0x364a35a7, 0x5e1a8a5c, 0x8e81d340,
#      0xad2b0e99, 0x89d7e07f],
#     [0xffef47b6, 0x3d048736, 0x7eaea384, 0x6ff89fd5, 0x858f0dd1, 0x654f3b1d, 0x3d1a3d21, 0x8cc531bf,
#      0x60aeb767, 0x176d453b, 0x928b49a1, 0x2d5b9b55, 0x21ad1565, 0x2e5aedb2, 0x7a9f44c0, 0xd5b75a87,
#      0xf95af58c, 0x8cc531bf],
#     [0x07f58679, 0x5563b9f8, 0xa3cb9426, 0x80276e8f, 0x58efd309, 0x34bcf89d, 0x34c6ffea, 0x2fe5fbc8,
#      0x0e0d78d3, 0x536f23a0, 0xc3d4b170, 0x9a29496f, 0x8533946c, 0x40fbcf88, 0x8b0c53f7, 0x77f97c91,
#      0x207058ba, 0x21e0bd1f],
# ]

# # Раундовые ключи
# ROUND_KEYS = [0] * 18

# # Инициализация раундовых ключей
# def init_round_keys(key):
#     # Значения мантиссы числа PI
#     pi_mantissa = [0x243f6a88, 0x85a308d3, 0x13198a2e, 0x03707344, 0xa4093822, 0x299f31d0, 0x082efa98, 0xec4e6c89,
#                    0x452821e6, 0x38d01377, 0xbe5466cf, 0x34e90c6c, 0xc0ac29b7, 0xc97c50dd, 0x3f84d5b5, 0xb5470917,
#                    0x9216d5d9, 0x8979fb1b]

#     # Инициализируем раундовые ключи значениями мантиссы числа PI
#     for i in range(18):
#         ROUND_KEYS[i] ^= pi_mantissa[i % len(pi_mantissa)]

#     # XOR с исходным ключом
#     key_length = len(key)
#     for i in range(18):
#         ROUND_KEYS[i] ^= key[i % key_length]



# # Функция F для сети Фейстеля
# def F(x):
#     a = (x >> 24) & 0xFF
#     b = (x >> 16) & 0xFF
#     c = (x >> 8) & 0xFF
#     d = x & 0xFF
#     a %= len(S_BOXES[0])
#     b %= len(S_BOXES[1])
#     c %= len(S_BOXES[2])
#     d %= len(S_BOXES[3])
#     res = (S_BOXES[0][a] + S_BOXES[1][b]) % (1 << 32)
#     res ^= S_BOXES[2][c]
#     res = (res + S_BOXES[3][d]) % (1 << 32)
#     return res


# # Функция для одного раунда сети Фейстеля
# def feistel_round(left, right, round_key):
#     new_left = right
#     new_right = left ^ F(right) ^ round_key
#     return new_left, new_right


# # Функция шифрования блока данных
# def encrypt_block(block):
#     left, right = struct.unpack(">II", block)
#     for i in range(16):
#         left, right = feistel_round(left, right, ROUND_KEYS[i])
#     right, left = feistel_round(right, left, ROUND_KEYS[16])
#     return struct.pack(">II", left, right)

# # Функция дешифрования блока данных
# def decrypt_block(block):
#     left, right = struct.unpack(">II", block)
#     for i in range(16, -1, -1):
#         left, right = feistel_round(left, right, ROUND_KEYS[i])
#     return struct.pack(">II", right, left)  # Меняем местами, чтобы вернуть в исходный порядок


# # Пример использования
# if __name__ == "__main__":
#     key = b"secret"
#     data = b"test1234"

#     init_round_keys(key)

#     encrypted_data = encrypt_block(data)
#     decrypted_data = decrypt_block(encrypted_data)

#     print("Original data:", data)
#     print("Encrypted data:", encrypted_data)
#     print("Decrypted data:", decrypted_data)


# import struct

# class Blowfish:
#     def __init__(self, key):
#         self.key = key
#         self._setup()

#     def _setup(self):
#         self.P = [0x243f6a88, 0x85a308d3, 0x13198a2e, 0x03707344, 0xa4093822, 0x299f31d0,
#                   0x082efa98, 0xec4e6c89, 0x452821e6, 0x38d01377, 0xbe5466cf, 0x34e90c6c,
#                   0xc0ac29b7, 0xc97c50dd, 0x3f84d5b5, 0xb5470917, 0x9216d5d9, 0x8979fb1b]
#         self.S = self.generate_S()

#     @staticmethod
#     def generate_S():
#         S = []
#         for _ in range(4):
#             s_block = []
#             for i in range(256):
#                 s_block.append(struct.unpack('<I', struct.pack('>I', i))[0])
#             S.append(s_block)
#         return S


#     def _encrypt(self, block):
#         left, right = struct.unpack('<II', block)

#         for i in range(16):
#             left ^= self.P[i]
#             right ^= self._F(left)
#             left, right = right, left

#         right ^= self.P[16]
#         left ^= self.P[17]

#         return struct.pack('<II', left, right)

#     def _decrypt(self, block):
#         left, right = struct.unpack('<II', block)

#         for i in range(17, 1, -1):
#             left ^= self.P[i]
#             right ^= self._F(left)
#             left, right = right, left

#         right ^= self.P[1]
#         left ^= self.P[0]

#         return struct.pack('<II', left, right)


#     def _F(self, x):
#         h = self.S[0][(x >> 24) & 0xFF] + self.S[1][(x >> 16) & 0xFF]
#         return ((h ^ self.S[2][(x >> 8) & 0xFF]) + self.S[3][x & 0xFF]) % (1 << 32)

#     def encrypt(self, data):
#         # Padding the data if its length is not multiple of 8 bytes
#         if len(data) % 8 != 0:
#             data += b'\x00' * (8 - len(data) % 8)
        
#         encrypted_data = b''
#         for i in range(0, len(data), 8):
#             encrypted_block = self._encrypt(data[i:i+8])
#             encrypted_data += encrypted_block
#         return encrypted_data

#     def decrypt(self, data):
#         decrypted_data = b''
#         for i in range(0, len(data), 8):
#             decrypted_block = self._decrypt(data[i:i+8])
#             decrypted_data += decrypted_block
#         return decrypted_data


# key = b'MySecretKey'
# blowfish = Blowfish(key)

# plaintext = b'Hello, World!'
# encrypted_text = blowfish.encrypt(plaintext)
# decrypted_text = blowfish.decrypt(encrypted_text)

# print("Plaintext:", plaintext)
# print("Encrypted text:", encrypted_text)
# print("Decrypted text:", decrypted_text)



# key = b'MySecretKey'
# plaintext = b'Hello, World!'

# # Создание объекта Blowfish
# blowfish = Blowfish(key)

# # Шифрование текста
# encrypted_text = blowfish.encrypt(plaintext)

# # Дешифрование текста с использованием того же объекта Blowfish
# decrypted_text = blowfish.decrypt(encrypted_text)

# print("Plaintext:", plaintext)
# print("Encrypted text:", encrypted_text)
# print("Decrypted text:", decrypted_text)

# plaintext = b'Hello, World!'
# encrypted_text = b'\x1fI\x0e@\xcb:\x95\x919^\x0eH6\x16\xb5\xc6'
# decrypted_text = b'\xe2\xac\xf7\x1fL\xe5\xbb$P\xbb\xf7\x17N\xc9\x9bs'

# print("Plaintext:", plaintext.decode())
# print("Encrypted text:", encrypted_text.decode())
# print("Decrypted text:", decrypted_text.decode())