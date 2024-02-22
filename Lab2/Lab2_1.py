import random, math


def genrate_nums(p,q):
        # Вычисляем n, функцию Эйлера, e
    n = p*q
    eiler = 0
    e = 0
    d = 0.5

    eiler = (p-1)*(q-1)
    e=5
    # while math.gcd (eiler, e) != 1:
    #     e = random.randint(1, n-1)


    k=0
    while d != int(d):
        d = (k * eiler + 1) / e
        # print(d, k)
        k += 1
    d = int(d)
        
    
    # e = 0
    # k=0
    # d = 0.5
    # print(p, q, n, eiler, e, d)
    return n, eiler, e, d
        

def RSA_crypt(e, n, text):
    rus_dict = {'А': 1, 'Б': 2, 'В': 3, 'Г': 4, 'Д': 5, 'Е': 6, 'Ё': 7, 'Ж': 8, 'З': 9, 'И': 10, 'Й': 11, 'К': 12, 'Л': 13, 'М': 14, 'Н': 15, 'О': 16, 'П': 17, 'Р': 18, 'С': 19, 'Т': 20, 'У': 21, 'Ф': 22, 'Х': 23, 'Ц': 24, 'Ч': 25, 'Ш': 26, 'Щ': 27, 'Ъ': 28, 'Ы': 29, 'Ь': 30, 'Э': 31, 'Ю': 32, 'Я': 33, ' ': 34, '0': 35, '1': 36, '2': 37, '3': 38, '4': 39, '5': 40, '6': 41, '7': 42, '8': 43, '9': 44}
    text_crypt = []
    for letter in text:
        t = int(rus_dict[letter])
        c = pow(t, e, n)
        text_crypt.append(c)
    return text_crypt


def RSA_decrypt(d,n, text):
    rus_dict_swap = {1: 'А', 2: 'Б', 3: 'В', 4: 'Г', 5: 'Д', 6: 'Е', 7: 'Ё', 8: 'Ж', 9: 'З', 10: 'И', 11: 'Й', 12: 'К', 13: 'Л', 14: 'М', 15: 'Н', 16: 'О', 17: 'П', 18: 'Р', 19: 'С', 20: 'Т', 21: 'У', 22: 'Ф', 23: 'Х', 24: 'Ц', 25: 'Ч', 26: 'Ш', 27: 'Щ', 28: 'Ъ', 29: 'Ы', 30: 'Ь', 31: 'Э', 32: 'Ю', 33: 'Я', 34: ' ', 35: '0', 36: '1', 37: '2', 38: '3', 39: '4', 40: '5', 41: '6', 42: '7', 43: '8', 44: '9'}
    # text_decrypt = []
    text_decrypt = ''
    for letter in text:
        t = pow(letter, d, n)
        # let = rus_dict_swap[t]
        # text_decrypt.append(t)
        text_decrypt += rus_dict_swap[t]
    return text_decrypt

# Главный метод
if __name__ == "__main__":
    # Вводим простые числа p и q
    # (Можно добавить проверку на простое число)
    p = int(input("Press prime number p: "))
    q = int(input("Press prime number q: "))
    notcrypted = input("Press message: ")
    n, eiler, e, d = genrate_nums(p, q)
    # print(n, eiler, e, d)
    crypted = RSA_crypt(e,n, notcrypted)
    print(crypted)
    uncrypted = RSA_decrypt(d,n, crypted)
    print(uncrypted)
