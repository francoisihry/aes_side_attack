import usbtmc
import numpy as np
from struct import unpack
import time


class Scope:

    def __init__(self, idVendor=1689, idProduct=921):
        self.device = usbtmc.Instrument(idVendor, idProduct)

    def acquisition_run(self):
        self.device.write("ACQuire:STOPAfter RUNSTop")
        self.device.write("ACQuire:STATE RUN")

    def acquisition_stop(self):
        self.device.write("ACQuire:STATE STOP")

    def acquisition_single(self):
        self.device.write("ACQuire:STOPAfter SEQuence")
        self.device.write("ACQuire:STATE RUN")

    def set_scale(self, time):
        # time in seconds
        # specifies the range from 2 ns to 100 s, depending on the oscilloscope model
        self.device.write(("HORizontal:SCAle {}".format(time)))

    def get_trace(self):
        # On attend que l'acquitistion ait lieu..
        while self.device.ask("ACQuire:STATE?")!="0":
            time.sleep(0.01)

        self.device.write('DATA:SOU CH1')
        #self.device.write('DATA:WIDTH 1')
        #self.device.write('DATA:ENC RPB')

        ymult = float(self.device.ask('WFMPRE:YMULT?'))
        yzero = float(self.device.ask('WFMPRE:YZERO?'))
        yoff = float(self.device.ask('WFMPRE:YOFF?'))
        xincr = float(self.device.ask('WFMPRE:XINCR?'))

        self.device.write('CURVE?')
        data = self.device.read_raw()
        headerlen = 2 + int(data[1])
        header = data[:headerlen]
        ADC_wave = data[headerlen:-1]

        ADC_wave = np.array(unpack('%sB' % len(ADC_wave), ADC_wave))

        Volts = (ADC_wave - yoff) * ymult + yzero

        Time = np.arange(0, xincr * len(Volts), xincr)
        if len(Time)>len(Volts):
            pass
        elif len(Time)<len(Volts):
            pass

        return (Volts,Time)

    def set_data_width(self, nb_bytes):
        self.device.write("DATa:WIDth {}".format(nb_bytes))

    def set_resolution_full(self):
        # 249952 points
        self.device.write("DATa:RESOlution FULL")

    def set_resolution_reduced(self):
        # 10368 points
        self.device.write("DATa:RESOlution REDUced")


# print(usbtmc.list_devices())
#
# scope =  Scope(1689, 921)
#
#
#
#
# scope.acquisition_run()
# time.sleep(2)
# scope.acquisition_stop()
# scope.acquisition_run()
# time.sleep(2)
# scope.acquisition_stop()
# scope.acquisition_run()
# time.sleep(2)
# scope.acquisition_stop()
# scope.acquisition_run()
# time.sleep(2)
# scope.acquisition_stop()
#
# Volts, Time = scope.get_trace()
#
# print("len volts : {} len time : {}".format(len(Volts),len(Time)))
#
#
# pylab.plot(Time, Volts)
# pylab.show()
#
# scope.acquisition_run()
# time.sleep(2)
# scope.acquisition_stop()
#
# Volts, Time = scope.get_trace()
#
# print("len volts : {} len time : {}".format(len(Volts),len(Time)))
#
#
# pylab.plot(Time, Volts)
# pylab.show()
#
# scope.acquisition_run()
# time.sleep(2)
# scope.acquisition_stop()
#
# Volts, Time = scope.get_trace()
#
# print("len volts : {} len time : {}".format(len(Volts),len(Time)))
#
#
# pylab.plot(Time, Volts)
# pylab.show()