from aes import aes_128
import unittest
import time
import serial


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


if __name__ == '__main__':
    unittest.main()