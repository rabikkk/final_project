import numpy as np
import cv2
from htpa import *
import pickle
i = 0
k = 0
dev = HTPA(0x1A)

while(True):
	if (i == 5):
		dev.measure_observed_offset()
		dev.measure_electrical_offset()

	(pixel_values, ptats) = dev.capture_image()  # Capture Image
	im = dev.temperature_compensation(pixel_values, ptats)  # thermal offset
	im = dev.offset_compensation(im)  # general offset
	if(k>5):
		im=dev.electrical_offset(im)#electrical offset
	im=dev.Vdd_Comperasition()#Vdd Comperasition
	im = dev.sensitivity_compensation(im)#Sensitivity

	# resize and scale image to make it more viewable on raspberry pi screen
	im = cv2.resize(im, None, fx=12, fy=12)	
	im -= np.min(im)
	im /= np.max(im)
	imcolor=cv2.applyColorMap(im,cv2.COLORMAP_JET)

	cv2.imshow('frame', im)
	cv2.imshow('frame1', imcolor)
	
	i += 1

	if cv2.waitKey(1) & 0xFF == ord('q'):
		break

dev.close()

cv2.destroyAllWindows()
