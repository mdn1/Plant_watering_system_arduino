import serial
from datetime import datetime
from getkey import getkey
from sys import exit


arduino_port = "/dev/ttyUSB0" #serial port of Arduino
baud = 9600 #arduino uno runs at 9600 baud
fileName="analog-data.csv" #name of the CSV file generated

ser = serial.Serial(arduino_port, baud)
print("Connected to Arduino port:" + arduino_port)
file = open(fileName, "a")
print("Created file")



samples = 5 #how many samples to collect each iteration
print_labels = True
line = 1 #start at 0 because our header is 0 (not real data)

try:
	while True:
		key_pressed = getkey(blocking=False)
		if key_pressed == "q":
			print("BKF")
			break
		line = 1

		while line <= samples:
			# incoming = ser.read(9999)
			# if len(incoming) > 0:

			getData=str(ser.readline())
			data=getData[0:][:-2]
			print(data)

			file = open(fileName, "a")
			now = datetime.now()
			dt_string = now.strftime("%d/%m/%Y %H:%M:%S")

			file.write(dt_string + ",") #write data with a newline
			file.write(data + "\n") #write data with a newline
			line = line+1
		print("Saving Checkpoint... ")
		file.close()

	print("Data collection complete!")
	file.close()

except KeyboardInterrupt:
	print("ctrl+c pressed")
	print("Data collection complete!")
	file.close()

