import serial 
import time
from enum import Enum

class arduinos(Enum):
    DATA_ARDUINO = 1
    MOTOR_ARDUINO = 2

class codes(Enum):
    READY = 1
    START = 2
    STOP = 3
    READ = 4

try:
    data_arduino = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=1)
    motor_arduino = serial.Serial(port='/dev/ttyACM1', baudrate=9600, timeout=1)
    time.sleep(2)
except serial.SerialException as e:
    print(f"Error opening serial ports: {e}")
    exit(1)

def write_to_arduino(code, arduino):
    if code == codes.READY:
        msg = "READY" 
    elif code == codes.START: 
        msg = "START"
    elif code == codes.STOP: 
        msg = "STOP"    
    elif code == codes.READ: 
        msg = "READ"
    else:
        print("Invalid Code, Refusing to write")
        return

    if (arduino == arduinos.MOTOR_ARDUINO):
        try:
            motor_arduino.write(f"{msg}\n".encode())
            print(f"Sent Code: {msg} To Motor Arduino")
        except Exception as e:
            print(f"Failed to send to Motor Arduino: {e}")

    elif (arduino == arduinos.DATA_ARDUINO):
        try:
            data_arduino.write(f"{msg}\n".encode())
            print(f"Sent Code: {msg} To Data Arduino")
        except Exception as e:
            print(f"Failed to send to Data Arduino: {e}")

    time.sleep(0.1)  # Add a small delay after writing to serial

    return 


write_to_arduino(codes.READY, arduinos.DATA_ARDUINO) 
write_to_arduino(codes.START, arduinos.MOTOR_ARDUINO) 

data_ready = False
motor_ready = False
while (data_ready == False) & (motor_ready == False):
    data = data_arduino.readline().decode('utf-8', errors='ignore').strip()
    motor = motor_arduino.readline().decode('utf-8', errors='ignore').strip()

    




