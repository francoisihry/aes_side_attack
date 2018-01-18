import serial
import time
import csv
import os
import numpy as np
import time
from aes import aes_128
from driver.tektronik_dpo_2002b_scope import Scope

print("Initializing the scope...")
scope = Scope()
scope.set_data_width(2)
scope.set_resolution_reduced()

print("Connecting to the target...")
target = serial.Serial('/dev/ttyUSB0', baudrate=9600)
time.sleep(1)
r = target.read_all()
print(r)
key = [33,77,15,21,79,90,11,22,33,44,55,66,77,88,18,16]

T_csv = open(os.path.join('data','T.csv'), 'w+')
T_writter = csv.writer(T_csv, delimiter=',',
                        quotechar='|', quoting=csv.QUOTE_MINIMAL)
M_csv = open(os.path.join('data','M.csv'), 'w+')
M_writter = csv.writer(M_csv, delimiter=',',
                        quotechar='|', quoting=csv.QUOTE_MINIMAL)



expected_nb_point = 10368
# On teste 100 fois avec des valeurs de cle/plaintext aleatoires:
print("Starting...")
for i in range(0, 10000):
    # 'p' + plaintext[16]: Send plaintext.
    target.write(b'p')
    plaintext = list(np.random.randint(255, size=16))

    target.write(bytearray(plaintext))
    # 'k' + key[16]: Send key.
    target.write(b'k')

    target.write(bytearray(key))

    # On lance l'acquisition
    scope.set_scale(0.01)
    scope.acquisition_single()
    # 'g' Go, execute AES encryption
    target.write(b'g')
    # time.sleep(0.5)
    # On stop l'acquisition
    # scope.acquisition_stop()


    Volts, Time = scope.get_trace()
    if len(Volts)==expected_nb_point: 
        T_writter.writerow(plaintext)
        M_writter.writerow((Volts*10e16).astype(int).tolist())
    else: print("unexpected nb points : {}".format(len(Volts)))
    # pylab.plot(Time, Volts)
    # pylab.show()

    # 'c' Get ciphertext, expect 16 bytes corresponding to the ciphertext.
    target.write(b'c')
    ciphertext = []
    for i in range(0, 16):
        ciphertext.append(int.from_bytes(target.read(), byteorder='little'))
    assert (ciphertext == aes_128(plaintext, key))

T_csv.close()
M_csv.close()
