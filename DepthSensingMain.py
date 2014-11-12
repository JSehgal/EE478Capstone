import serial, time
UAV = serial.Serial('COM1', 115200, timeout=.1)
time.sleep(1)
NotConnected = True;
while NotConnected:
	data = UAV.readline()
	if data:
		print data
		NotConnected = False;

while True:
	UAV.write("Beagle here\n")