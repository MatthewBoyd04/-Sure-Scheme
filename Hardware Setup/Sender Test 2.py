import serial
import matplotlib.pyplot as plt

# Configure the serial port and baud rate (adjust as needed)
arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)

# Initialize lists to store data
micros_data = []
x_data = []
y_data = []
z_data = []

# Create a figure and axis for the plot
fig, ax = plt.subplots()

# Set labels and title
ax.set_xlabel('Micros')
ax.set_ylabel('Value')
ax.set_title('X, Y, Z vs. Micros')

# Plot styles for X, Y, Z
styles = ['b-', 'g-', 'r-']  # Blue for X, Green for Y, Red for Z

# Read and plot data continuously
while True:
    try:
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

                # Append data to lists
                micros_data.append(Micros)
                x_data.append(X)
                y_data.append(Y)
                z_data.append(Z)

                # Update plot with new data
                ax.plot(micros_data, x_data, styles[0], label='X')
                ax.plot(micros_data, y_data, styles[1], label='Y')
                ax.plot(micros_data, z_data, styles[2], label='Z')

                # Update legend
                ax.legend()

                # Pause to update plot
                plt.pause(0.001)

            except ValueError:
                print("Received data could not be converted to float/int:", dsplit)
        else:
            print("Received data does not contain exactly four values:", dsplit)

    except KeyboardInterrupt:
        print("Plotting stopped by user.")
        break

# Show the plot at the end (close plot window to exit)
plt.show()
