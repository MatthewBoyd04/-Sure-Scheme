import serial 

import time 

class SerialSender: 
    def __init__(self): 
        self.arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)

    def write_read(self, x): 
        self.arduino.write(bytes(x,'utf-8'))
        time.sleep(0.0001)
        arduinoKey = ""
        data = []
        while (arduinoKey != "AWAIT"):
            if (arduinoKey != ""):
                data.append(arduinoKey)
            arduinoKey = self.arduino.readline().decode().strip()
            time.sleep(0.0001)
        print("----------")
        print("Recieved AWAIT")
        return data


S = SerialSender()


while True: 
    num = input("Input a number: ")
    value = S.write_read(num)
    print("-----------")
    for line in value:
        print(line)
    print("-----------")