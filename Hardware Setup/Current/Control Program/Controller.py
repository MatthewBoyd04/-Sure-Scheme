import serial 
import time
from enum import Enum

class arduinos(Enum):
    DATA_ARDUINO = 1
    MOTOR_ARDUINO = 2

class codes(Enum):
    READY = 1
    START = 2
    START_MOTOR_DATA_TRANSMISSION = 3
    END_OF_DATA_TRANSMISSION = 4

ID = "Raspberry_Pi: "

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
    elif code == codes.START_MOTOR_DATA_TRANSMISSION: 
        msg = "START_MOTOR_DATA_TRANSMISSION"
    elif code == codes.END_OF_DATA_TRANSMISSION: 
        msg = "END_OF_DATA_TRANSMISSION"
    else:
        msg = code
        print(ID + "Sending Unrecognised Code: " + str(msg))
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


#CHECK STATE 1 -> Send "READY to both arduionos" -- WORKING

write_to_arduino(codes.READY, arduinos.DATA_ARDUINO) 
write_to_arduino(codes.READY, arduinos.MOTOR_ARDUINO) 

#CHECK STATE 2 -> Awaiting "START" From Both Arduinos
data_ready = False
motor_ready = False
data = ""
motor = ""
while (data_ready == False) & (motor_ready == False):
    data = data_arduino.readline().decode('utf-8', errors='ignore').strip()
    motor = motor_arduino.readline().decode('utf-8', errors='ignore').strip()
    if data != "": 
        print(data)
        dsplit = data.split(" ")
        if dsplit[1] == "START":
            data_ready = True
    if motor != "":
        print(motor)
        msplit = motor.split(" ")
        if msplit[1] == "START":
            motor_ready = True

print(ID + "Code START recieved")

#ACTIVE STATE 1 -> Send 'START_MOTOR_DATA_TRANSMISSION' to motor, to confirm its ready
write_to_arduino(codes.START_MOTOR_DATA_TRANSMISSION, arduinos.MOTOR_ARDUINO)
print(ID + "Active State 1 Finished")

#ACTIVE STATE 2 -> waiting for 'READY_FOR_DATA' from motor ardunio
motor_ready = False
motor = ""
msplit[1] = ""
while (motor_ready == False):
    motor = motor_arduino.readline().decode('utf-8', errors='ignore').strip()
    if motor != "":
        print(motor)
        msplit = motor.split(" ")
        print(ID + msplit[1] + " Is the value of Msplit1")
        if msplit[1] == "READY_FOR_DATA":
            motor_ready = True

time.sleep(1) #Ensure Arduino is ready
print(ID + "Active State 2 Finished")

#ACTIVATE STATE 3 -> Sending Motor Data To Arduino
print(ID + "Code READY_FOR_DATA recieved")

write_to_arduino(0, arduinos.MOTOR_ARDUINO)
write_to_arduino(0, arduinos.MOTOR_ARDUINO)
write_to_arduino(0, arduinos.MOTOR_ARDUINO)
write_to_arduino(0, arduinos.MOTOR_ARDUINO)
write_to_arduino(0, arduinos.MOTOR_ARDUINO)
write_to_arduino(codes.END_OF_DATA_TRANSMISSION, arduinos.MOTOR_ARDUINO)
print(ID + "Active State 3 Finished")
    
#ACTIVE STATE 4 -> Waiting on confirmation of data recieved
motor_ready = False
while (motor_ready == False):
    motor = motor_arduino.readline().decode('utf-8', errors='ignore').strip()
    if motor != "":
        print(motor)
        msplit = data.split(" ")
        if msplit[1] == "!!PLACEHOLDER!!":
            motor_ready = True

print(ID + "Active State 3 Finished")


