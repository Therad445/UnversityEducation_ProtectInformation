from Crypto.Cipher import Blowfish
from Crypto import Random

def encrypt_message(message, key):
    # Генерация случайного вектора инициализации
    iv = Random.new().read(Blowfish.block_size)
    cipher = Blowfish.new(key, Blowfish.MODE_CBC, iv)
    # Дополнение сообщения до кратности блоку шифрования
    length = Blowfish.block_size - (len(message) % Blowfish.block_size)
    message += bytes([length]) * length
    return iv + cipher.encrypt(message)

def decrypt_message(encrypted_message, key):
    iv = encrypted_message[:Blowfish.block_size]
    cipher = Blowfish.new(key, Blowfish.MODE_CBC, iv)
    decrypted_message = cipher.decrypt(encrypted_message[Blowfish.block_size:])
    # Удаление дополнения
    padding_length = decrypted_message[-1]
    return decrypted_message[:-padding_length]

# Пример использования
key = b'P@ssw0rd'
message = b'Hello, World!'
encrypted_message = encrypt_message(message, key)
print("Encrypted message:", encrypted_message)
decrypted_message = decrypt_message(encrypted_message, key)
print("Decrypted message:", decrypted_message.decode())


# Ошибка в представлении сети Фейстеля может быть вызвана различными проблемами в реализации, включая, но не ограничиваясь:

# Ошибка в ключе: Неправильное управление ключом может привести к небезопасной работе шифра. Например, использование слабого ключа или неправильная обработка ключей может угрожать безопасности.

# Недостаточное количество раундов: Сеть Фейстеля требует нескольких раундов, чтобы обеспечить хорошую криптографическую стойкость. Недостаточное количество раундов может ослабить безопасность.

# Неправильное управление дополнения: Если в схеме используется дополнение, его обработка должна быть правильной и безопасной.

# Утечка информации: Некорректное управление временем выполнения или обработка ошибок может привести к утечке информации о ключе или сообщении.