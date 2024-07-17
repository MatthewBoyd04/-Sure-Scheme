import serial 

import time 


arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)

def set_motor_speed(speed):
    arduino.write(f"{speed}\n".encode())

# Example of setting different speeds
try:
    while True:
        speed = input("Enter motor speed: ")
        set_motor_speed(speed)

except KeyboardInterrupt:
    print("Exiting...")
    arduino.close()

