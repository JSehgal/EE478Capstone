import serial, time
from multiprocessing import Process


def getCommands():
	while True:
		command = raw_input("Enter start. stop or reset command: ")
		
		# If command has been given, execute it and then clear it
		if(command == 'start'):
			print command, 'command received' 
			beagle.write('s')
			command = 'none'
		elif(command == 'stop'):
			print command, 'command received' 
			beagle.write('p')
			command = 'none'
		elif(command == 'reset'):
			print command, 'command received' 
			beagle.write('r')
			command = 'none'
		else:
			print 'command not valid'

# Setup serial connection
beagle = serial.Serial('/dev/ttyUSB0', 9600, timeout=.1)
time.sleep(1)

beagle.write("Testbench connected")
command = 'none'

getCommands()
