import serial 
import time 
arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)

while True:
    # Read a line from the Arduino
    data = arduino.readline().decode('utf-8', errors='ignore').strip()
    # Split the data by commas
    dsplit = data.split(",")

    # Ensure there are exactly four values
    if len(dsplit) == 4:
        try:
            # Convert the values to appropriate types
            X = float(dsplit[0])
            Y = float(dsplit[1])
            Z = float(dsplit[2])
            Micros = int(dsplit[3])

            # Print the values
            formatted_output = "X: {:<10} | Y: {:<10} | Z: {:<10} | Micros: {:<12}".format(X, Y, Z, Micros)

            print(formatted_output)

        except ValueError:
            print("Received data could not be converted to float/int:", dsplit)
    else:
        print("Received data does not contain exactly four values:", dsplit)
