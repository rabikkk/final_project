import cv2
import os
from PIL import Image
import numpy as np
  
# example of random rotation image augmentation
from numpy import expand_dims
from keras.preprocessing.image import load_img
from keras.preprocessing.image import img_to_array
from keras.preprocessing.image import ImageDataGenerator
from matplotlib import pyplot

# Passing the path of the image directory
src='/data2/closem/'
path1='/data2/close1/';
filenames_train=os.listdir(src)

print(len(filenames_train))
for f_name in filenames_train:
    im=Image.open(src+f_name)
       # convert to numpy array
    data = img_to_array(im)
    # expand dimension to one sample
    samples = expand_dims(data, 0)
    # create image data augmentation generator
    datagen = ImageDataGenerator(rotation_range=70)
    # prepare iterator
    it = datagen.flow(samples, batch_size=9, save_to_dir=path1, save_prefix='close_train1', save_format='png')
