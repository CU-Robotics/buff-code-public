import numpy
import argparse
import os
import cv2
import datetime

class Analyzer:
    def __init__(self):
        """???"""
    def saveImages(self, image, saveToFilePath="../data/"):
        """
        Function: saves a single or a batch of images to a given filepath
        Parameters:
            @image image = one image
            @string saveToFilePath = file path where images will be saved
        """
        cv2.imwrite(saveToFilePath+datetime.datetime.now(),image)
       
    def analyzeImageHSV(self, image):
        """
        Function: analyzes image using HSV, generally better than BGR for lights, adds to processed image array
        Parameters: 
            @string imageName = name of the image, including filetype
                Ex: "exampleImage.jpg"
        Return:        
            @array[imageRed, imageBlue]
        """
        imageHSV = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        red_lower = (0,140,100) #hsv for red, lower boundary
        red_upper = (25,255,255) #hsv for red, upper boundary
        blue_lower = (80,100,100) #hsv for blue, lower boundary
        blue_upper = (120,255,255) #hsv for blue, upper boundary
        red_mask = cv2.inRange(imageHSV, red_lower, red_upper)
        blue_mask = cv2.inRange(imageHSV, blue_lower, blue_upper)
        red_output = cv2.bitwise_and(imageHSV, imageHSV, mask=red_mask) #image red only
        blue_output = cv2.bitwise_and(imageHSV, imageHSV, mask=blue_mask) #image blue only
        red_output = cv2.cvtColor(red_output, cv2.COLOR_HSV2BGR)
        blue_output = cv2.cvtColor(blue_output, cv2.COLOR_HSV2BGR)
        return [red_output,blue_output]
        #cv2.imshow("image", imageBGR)
        #cv2.imshow("HSV red detection: " + imagePath, red_output)
        #cv2.imshow("HSV blue detection: " + imagePath, blue_output)
        #cv2.waitKey(0)
    def analyzeImageBGR(self, image):
        """
        Function: analyzes images using BGR, adds to processed image array
        Parameters:
            @string imageName = name of the image, including filetype
                Ex: "exampleImage.jpg"
        Return:
            @array[imageRed, imageBlue]
        """
        imageBGR = image
        red_lower = (0,0,100) #bgr for red, lower boundary
        red_upper = (80,80,255) #bgr for red, upper boundary
        blue_lower = (100,0,0) #bgr for blue, lower boundary
        blue_upper = (255,80,80) #bgr for blue, upper boundary
        red_mask = cv2.inRange(imageBGR, red_lower, red_upper)
        blue_mask = cv2.inRange(imageBGR, blue_lower, blue_upper)
        red_output = cv2.bitwise_and(imageBGR, imageBGR, mask=red_mask) #image red only
        blue_output = cv2.bitwise_and(imageBGR, imageBGR, mask=blue_mask) #image blue only
        return [red_output, blue_output]
        #cv2.imshow("image", imageBGR)
        #cv2.imshow("BGR red detection: " + imagePath, red_output)
        #cv2.imshow("BGR blue detection: " + imagePath, blue_output)
        #cv2.waitKey(0)
    def display(self, images):
        """
        Function: displays all processed images
        
        """
        cv2.imshow("imagesProcessed", numpy.concatenate(images, axis=0))
        cv2.waitKey(0)

#TEST CASES----------------------------------------------------------------------------------------------------
#images = Analyzer("../data/")
#images.loadImages("../data/")
#images.analyzeImageBGR("30.jpg")
#images.display()

"""
TODO:
1. Fix the issue that some images do not stack together (concatenation only shows 2 images)
2. Fix the issue where it will not allow loading from the file path of one single image (file instead of directory)
3. Give the processed images names so that when they are saved they have names
4. Add error handlers/exceptions
"""


    
    