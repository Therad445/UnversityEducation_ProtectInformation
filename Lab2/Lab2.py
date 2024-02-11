import random
import math

def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def multiplicative_inverse(e, phi):
    d = 0
    x1, x2 = 0, 1
    y1, y2 = 1, 0
    temp_phi = phi
    
    while e > 0:
        temp1 = temp_phi // e
        temp2 = temp_phi - temp1 * e
        temp_phi = e
        e = temp2
        
        x = x2 - temp1 * x1
        y = y2 - temp1 * y1
        
        x2 = x1
        x1 = x
        y2 = y1
        y1 = y
    
    if temp_phi == 1:
        d = y2 + phi
    
    return d

def is_prime(num):
    if num <= 1:
        return False
    if num <= 3:
        return True
    if num % 2 == 0 or num % 3 == 0:
        return False
    i = 5
    while i * i <= num:
        if num % i == 0 or num % (i + 2) == 0:
            return False
        i += 6
    return True

def generate_keypair(p, q):
    if not (is_prime(p) and is_prime(q)):
        raise ValueError("Both numbers must be prime.")
    elif p == q:
        raise ValueError("p and q cannot be equal")
    n = p * q
    phi = (p-1) * (q-1)
    e = random.randrange(1, phi)
    g = gcd(e, phi)
    while g != 1:
        e = random.randrange(1, phi)
        g = gcd(e, phi)
    d = multiplicative_inverse(e, phi)
    return ((e, n), (d, n))

def encrypt(public_key, plaintext):
    key, n = public_key
    cipher = [pow(ord(char), key, n) for char in plaintext]
    return cipher

def decrypt(private_key, ciphertext):
    key, n = private_key
    plain = [chr(pow(char, key, n)) for char in ciphertext]
    return ''.join(plain)

if __name__ == '__main__':
    p = int(input("Enter a prime number (p): "))
    q = int(input("Enter another prime number (q): "))
    public, private = generate_keypair(p, q)
    print("Public key:", public)
    print("Private key:", private)
    message = input("Enter a message to encrypt: ")
    encrypted_msg = encrypt(public, message)
    print("Encrypted message:", ''.join(map(lambda x: str(x), encrypted_msg)))
    decrypted_msg = decrypt(private, encrypted_msg)
    print("Decrypted message:", decrypted_msg)
