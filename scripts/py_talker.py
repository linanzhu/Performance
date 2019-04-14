#!/usr/bin/env python

import rospy
from performance_tests.msg import SuperAwesome

def talker():

    rospy.init_node('py_talker', anonymous=True)

    hertz = 10
    if (rospy.has_param("/py_talker/hertz")):
        hertz = rospy.get_param("/py_talker/hertz")

    pymsg_pub = rospy.Publisher('pt_msg', SuperAwesome, queue_size=1)

    loop_rate = rospy.Rate(hertz)

    msg = SuperAwesome()
    msg.header.seq = 0
    msg.hertz = hertz
    msg.message = "BlueOceanRobotics"

    startT = rospy.Time.now()

    while not rospy.is_shutdown():
        msg.header.stamp = rospy.Time.now()

        pymsg_pub.publish(msg)

        msg.header.seq += 1

        loop_rate.sleep()

        stopT = rospy.Time.now()
        elapsedT = stopT - startT

        startT = stopT

		
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
