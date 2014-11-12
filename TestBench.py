import serial, time
beagle = serial.Serial('COM1', 115200, timeout=.1)
time.sleep(1)
beagle.write("Testbench connected")
while True:
	data = beagle.readline()
	if data:
		print data