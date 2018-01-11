import usbtmc
import numpy as np
from struct import unpack
import pylab


print(usbtmc.list_devices())
# test= usbtmc.list_devices()[0]
#
# id_v= usbtmc.list_devices()[0].idVendor
# id_p= usbtmc.list_devices()[0].idProduct

# print("device name = {}".format(name))
scope =  usbtmc.Instrument(1689, 921)

#
# print(scope.ask("DATa:SOUrce?"))
#
scope.write('DATA:SOU CH1')
scope.write('DATA:WIDTH 1')
scope.write('DATA:ENC RPB')

ymult = float(scope.ask('WFMPRE:YMULT?'))
yzero = float(scope.ask('WFMPRE:YZERO?'))
yoff = float(scope.ask('WFMPRE:YOFF?'))
xincr = float(scope.ask('WFMPRE:XINCR?'))


scope.write('CURVE?')
data = scope.read_raw()
headerlen = 2 + int(data[1])
header = data[:headerlen]
ADC_wave = data[headerlen:-1]

ADC_wave = np.array(unpack('%sB' % len(ADC_wave),ADC_wave))

Volts = (ADC_wave - yoff) * ymult  + yzero

Time = np.arange(0, xincr * len(Volts), xincr)

pylab.plot(Time, Volts)
pylab.show()