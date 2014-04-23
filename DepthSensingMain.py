import cv2
import numpy as np
import serial, time, os
import Adafruit_BBIO.UART as UART

# Extracts depth info from stereoscopic images <img1> and <img2>, then saves disparity map as an image called <name>
def ExtractDepth(img1, img2, name):
	stereo = cv2.StereoBM(cv2.STEREO_BM_BASIC_PRESET, ndisparities=16, SADWindowSize=15)
	disparity = stereo.compute(img1, img2)
	cv2.imwrite(name, disparity)

# Setup serial connection over UART
UART.setup("UART1")
UAV = serial.Serial('/dev/ttyO1', 9600, timeout=.1)
time.sleep(1)

state = 'idle'
while True:
	# Get command from UAV
	data = UAV.readline()
	if(data):
		print data
	if(data == 's'): # Testing: Extract depth from test images and send using scp
		
		# Load test images
		img1 = cv2.imread('image802.jpg', cv2.CV_LOAD_IMAGE_GRAYSCALE)	
		img2 = cv2.imread('image803.jpg', cv2.CV_LOAD_IMAGE_GRAYSCALE)
		
		# Extract depth information from images
		ExtractDepth(img1, img2, 'depth.jpg')			

		# Transmit RGB and D info to testbench using secure copy
		os.system('scp image802.jpg depth.jpg jsehgal2@192.168.7.1:Documents/ee478')		
	
		state = 'collecting'
	elif(data == 'p'):
		state = 'transmitting'
	elif(data == 'r'):
		state = 'clearing'
	

	# Not implementable until more is known about image capture and SD card storage solutions
	#if(state == 'collecting'):
		# If SD card not full
			# Tell cameras we are collecting images
		# Else
			# Send Memory full message to UAV
		# Load in next pair of images
		# Extract depth
		# Transmit RGB and D data to UAV
	#elif(state == 'transmitting'):
		# Tell cameras not to collect images
		# If next pair of images
			# Load images
			# Extract depth
			# Transmit
		# Else return to idle state
	#elif(state == 'clearing')
		# Tell cameras not to collect images
		# Delete all images currently on SD card
		# Return to idle state
