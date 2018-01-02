from aes import aes_128
import unittest
import time
import serial
import numpy as np




class TestDevice(unittest.TestCase):

    def setUp(self):
        # On se connecte au microcontroleur:
        self.ser = serial.Serial('/dev/ttyUSB0', baudrate=9600)
        # On lit le message de bienvenue:
        time.sleep(1)
        self.ser.read_all()


    def tearDown(self):
        # On se déconnecte:
        self.ser.close()


    def test_test(self):
        # 't': Test connection, return the string "OK" if the command is received.
        self.ser.write(b't')
        response= self.ser.readline()
        self.assertEqual(response, b"OK\n")


    def test_cipher_functions(self):
        # 'p' + plaintext[16]: Send plaintext.
        self.ser.write(b'p')
        plaintext = [0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34]
        self.ser.write(bytearray(plaintext))
        # 'k' + key[16]: Send key.
        self.ser.write(b'k')
        key = [0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c]
        self.ser.write(bytearray(key))
        # 'g' Go, execute AES encryption
        self.ser.write(b'g')
        # 'c' Get ciphertext, expect 16 bytes corresponding to the ciphertext.
        self.ser.write(b'c')
        ciphertext = []
        for i in range(0, 16):
            ciphertext.append(int.from_bytes( self.ser.read(), byteorder='little'))
        self.assertEqual(ciphertext, aes_128(plaintext,key))

        # On teste 100 fois avec des valeurs de clé/plaintext aleatoires:
        for i in range(0,100):
            self.ser.write(b'p')
            plaintext = list(np.random.randint(255, size=16))
            self.ser.write(bytearray(plaintext))
            self.ser.write(b'k')
            key = list(np.random.randint(255, size=16))
            self.ser.write(bytearray(key))
            self.ser.write(b'g')
            self.ser.write(b'c')
            ciphertext = []
            for i in range(0, 16):
                ciphertext.append(int.from_bytes(self.ser.read(), byteorder='little'))
            self.assertEqual(ciphertext, aes_128(plaintext, key))

    def test_fast_mode(self):
        # 'k' + key[16]: Send key.
        self.ser.write(b'k')
        key = [0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c]
        self.ser.write(bytearray(key))
        # 'f' + plaintext[16]: Fast mode, send plaintext, execute AES and give the ciphertext back. Expect 16 bytes corresponding to the ciphertext.
        self.ser.write(b'f')
        plaintext = [0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34]
        self.ser.write(bytearray(plaintext))
        ciphertext = []
        for i in range(0, 16):
            ciphertext.append(int.from_bytes(self.ser.read(), byteorder='little'))
        self.assertEqual(ciphertext, aes_128(plaintext, key))

        # On teste 100 fois avec des valeurs de clé/plaintext aleatoires:
        for i in range(0, 100):
            self.ser.write(b'k')
            key = list(np.random.randint(255, size=16))
            self.ser.write(bytearray(key))
            self.ser.write(b'f')
            plaintext = list(np.random.randint(255, size=16))
            self.ser.write(bytearray(plaintext))
            ciphertext = []
            for i in range(0, 16):
                ciphertext.append(int.from_bytes(self.ser.read(), byteorder='little'))
            self.assertEqual(ciphertext, aes_128(plaintext, key))




if __name__ == '__main__':
    unittest.main()