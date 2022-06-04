#! /usr/bin/env python3
import os
import cv2
import sys
import time
import rospy
import numpy as np
from cv_bridge import CvBridge
from sensor_msgs.msg import Image
from std_msgs.msg import String, Float64MultiArray

"""
	Project pixel coordinates to the world
	
"""

class Projector:
	def __init__(self, name):
		# camera heading
		self.phi = 0.0
		self.psi = 0.0
		self.a = rospy.get_param(f'{name}/A')
		self.m = rospy.get_param(f'{name}/M')

		self.FOV = rospy.get_param('/buffbot/CAMERA/FOV')
		image_res = rospy.get_param('/buffbot/CAMERA/RESOLUTION')
		self.image_size = np.array([image_res, image_res, 3])

		self.init_ros()

	def init_ros(self):
		rospy.init_node('projector', anonymous=True)
		self.rate = rospy.Rate(rospy.get_param('/buffbot/CAMERA/FPS'))
		
		self.debug = rospy.get_param('/buffbot/DEBUG')
		topics = rospy.get_param('/buffbot/TOPICS')

		self.detect_sub = rospy.Subscriber(
			topics['DETECTION_PIXEL'], Float64MultiArray, self.detection_callback, queue_size=5)

		self.gimbal_sub = rospy.Subscriber(
		 	topics['GIMBAL_STATE'], Float64MultiArray, self.gimbal_callback, queue_size=1)

		self.red_pub = rospy.Publisher(
			topics['DETECTION_RED'], Float64MultiArray, queue_size=1)

		self.blue_pub = rospy.Publisher(
			topics['DETECTION_BLUE'], Float64MultiArray, queue_size=1)

		self.project_pub = rospy.Publisher(
			topics['DETECTION_WORLD'], Float64MultiArray, queue_size=1)


	def detection_callback(self, msg):
		"""
		Parse a detection msg
		PARAMS:
			msg: Float64MultiArray, detection msg, data=[x,y,w,h,cf,cl]
		"""
		# for now. need to figure out how to get accurate time between messages?
		# Build a custom message that has a timestamp
		t = time.time()
		# do projector stuff
		r = self.project(np.array(msg.data))
		if not r is None and len(r) > 0:
			msg = Float64MultiArray(data=r)
			self.project_pub.publish(msg)

	def gimbal_callback(self, msg):
		state = msg.data
		self.psi = state[0]
		self.phi = state[1]

	def height_2_distance(self, h):
		return (self.a * h) + (self.m / h)

	def project(self, detections):
		"""
		Projects a detection into the world frame
		PARAMS:
			pose: Float64MultiArray.data, [x1,y1,x2,y2,cf,cl] (detection msg)
		RETURNS:
			vector (x,y): body frame position of the detection
		"""
		reds = []
		blues = []
		poses = []

		for detection in detections.reshape((round(len(detections)/5), 5)):
			x, y, w, h, cl = detection

			if cl == 0:
				blues = np.concatenate([blues, [x,y,w,h]])
			elif cl == 1:
				reds = np.concatenate([reds, [x,y,w,h]])

			d = self.height_2_distance(h)
			alpha = np.radians((0.5 - (x / self.image_size[0])) * self.FOV / 2)
			poses = np.concatenate([poses, [cl, d * np.cos(self.phi) * np.cos(self.psi + alpha), d * np.cos(self.phi) * np.sin(self.psi + alpha)]])

		if len(reds) > 1:
			msg = Float64MultiArray(data=reds)
			self.red_pub.publish(msg)
		elif len(blues) > 1:
			msg = Float64MultiArray(data=blues)
			self.blue_pub.publish(msg)

		return poses

def main(name):
	projector = Projector(name)

	try:
		while not rospy.is_shutdown():
			# for sim
			projector.rate.sleep()

	except KeyboardInterrupt as e:
		print('Projector killed')

	except Exception as e:
		print(e)


if __name__ == '__main__':
	if len(sys.argv) < 2:
		print(f'No Data: Projector exiting ...')

	elif '/buffbot' in sys.argv[1]:
		main(sys.argv[1])




