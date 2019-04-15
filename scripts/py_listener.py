#!/usr/bin/env python

import rospy
from performance_tests.msg import SuperAwesome
from threading import Thread
import math

oldListenerTime = None
lhz = 0.0
thz = 0.0

def callback(msg):
    global oldListenerTime
    global lhz
    global thz

    # Compare time and hertz : local time vs. header time
    nt = rospy.Time.now()
    localPeriodT = nt - oldListenerTime
    oldListenerTime = nt

    thz = msg.hertz
    lhz = 1.0 / localPeriodT.to_sec()


def pub_result():
    global lhz
    global thz

    rate = rospy.Rate(1000)

    while not rospy.is_shutdown():
        sumdhz = 0.0
        for i in list(range(1000)):
            diffhz = math.fabs(lhz - thz)
            sumdhz += diffhz
            rate.sleep()
        avrdhz = sumdhz / 500.0

        rospy.loginfo('py result hertz diff : %f', avrdhz)


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
	
