import cv2
import os
from PIL import Image
import numpy as np
  
src='/open_train/five/'
filenames_train=os.listdir(src)

print(len(filenames_train))
for f_name in filenames_train:
    im=Image.open(src+f_name)
    # Size of the image in pixels (size of orginal image)
    # (This is not mandatory)
#    width, height = im.size
  
    # Setting the points for cropped image
    # Setting the points for cropped image
    left = 120
    top = 45
    right = 390
    bottom = 240
  
    # Cropped image of above dimension
    # (It will not change orginal image)
    #im1 = im.crop((left, top, right, bottom))
    im1=im1.resize((32, 32))
    im1.save('/open_train/five_new/'+f_name)
