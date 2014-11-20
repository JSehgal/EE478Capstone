import serial, time
beagle = serial.Serial('COM5', 115200, timeout=.1)
time.sleep(5)
beagle.write("Testbench connected")
while True:
	data = beagle.readline()
	if data:
		print data