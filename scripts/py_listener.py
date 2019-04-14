#!/usr/bin/env python

import rospy
from performance_tests.msg import SuperAwesome
from threading import Thread
import time

oldListenerTime = None
lhz = 0.0

def callback(msg):
    global oldListenerTime
    global lhz

    # Compare time and hertz : local time vs. header time
    nt = rospy.Time.now()
    localPeriodT = nt - oldListenerTime
    oldListenerTime = nt

    lhz = 1.0 / localPeriodT.to_sec()


def pub_result():
    global lhz

    rate = rospy.Rate(5)
    while not rospy.is_shutdown():
        rospy.loginfo('py result hertz : ', lhz)
        rate.sleep()


def listener():
    global oldListenerTime

    rospy.init_node('py_listener', anonymous=True)

    pub_thread = Thread(target=pub_result)
    pub_thread.start()


    # Initialize base time as current time
    oldListenerTime = rospy.Time.now()

    rospy.Subscriber("pt_msg", SuperAwesome, callback)

    rospy.spin()
	
if __name__ == '__main__':
    listener()
	
