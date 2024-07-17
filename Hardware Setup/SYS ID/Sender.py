import serial 
import csv
import time 



arduino = serial.Serial(port='/dev/ttyUSB1', baudrate=115200, timeout=.1)

def set_motor_speed(speed):
    arduino.write(f"{speed}\n".encode())


try:
    #Set Speed
    speed = input("Enter motor speed: ")
    set_motor_speed(speed)

    #Create a CSV file for data
    file_name = speed + 'rps.csv'

    # Define the data to be written to the first row
    first_row_data = ['X', 'Y', 'Z', 'Micros']

    # Open the file in write mode
    with open(file_name, mode='w', newline='') as file:
        # Create a CSV writer object
        writer = csv.writer(file)
        
        # Write the first row
        writer.writerow(first_row_data)

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
                row = [X,Y,Z,Micros]

                with open(file_name, mode='a', newline='') as file:
                    # Create a CSV writer object
                    writer = csv.writer(file)
                    # Write the first row
                    writer.writerow(row)

            except ValueError:
                print("Received data could not be converted to float/int:", dsplit)
        else:
            print("Received data does not contain exactly four values:", dsplit)


        
except KeyboardInterrupt:
    print("Exiting...")
    arduino.close()

