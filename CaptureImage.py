import cv

capture = cv.CaptureFromCAM(0)
cv.SetCaptureProperty(capture, cv.CV_CAP_PROP_FRAME_HEIGHT, 240)
cv.SetCaptureProperty(capture, cv.CV_CAP_PROP_FRAME_WIDTH, 320)
#cv.SetCaptureProperty(capture, cv.CV_CAP_PROP_FORMAT, cv.IPL_DEPTH_32F)

img = cv.QueryFrame(capture)
cv.SaveImage('test.ppm', img)