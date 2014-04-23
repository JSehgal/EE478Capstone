import serial, time
from multiprocessing import Process


def getCommands():
	while True:
		command = raw_input("Enter start. stop or reset command:")
		print command, 'command received' 

def receiveData():
	while True:
		# If command has been given, execute it and then clear it
		if(command == 'start'):
			beagle.write('s')
			command = 'none'
		elif(command == 'stop'):
			beagle.write('p')
			command = 'none'
		elif(command == 'reset'):
			beagle.write('r')
			command = 'none'

		# Receive data from beaglebone
		data = beagle.readline()
		if data:
			print data

# Setup serial connection
beagle = serial.Serial('COM5', 9600, timeout=.1)
time.sleep(5)

beagle.write("Testbench connected")
command = 'none'

receiveData()


