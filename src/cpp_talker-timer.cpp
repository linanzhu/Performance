
#include "ros/ros.h"
#include "performance_tests/SuperAwesome.h"

#include <iostream>
#include <sstream>

using namespace std;

unsigned int hertz = 5000;
ros::Publisher chatter_pub;
ros::Time startT;
performance_tests::SuperAwesome msg;

void pub_message(const ros::TimerEvent&)
{
    msg.header.seq = 0;
    msg.header.stamp = ros::Time::now();
    msg.hertz = hertz;
    msg.message = "I am going to BlueOceanRobotics.";

    ROS_INFO("[%f]", msg.header.stamp.toSec());

    chatter_pub.publish(msg);

    msg.header.seq += 1;

    // measure elapsed time
    ros::Time stopT = ros::Time::now();
    ros::Duration elapsedT = stopT - startT;

    startT = stopT;
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "talker");

    ros::NodeHandle n;

    chatter_pub = n.advertise<performance_tests::SuperAwesome>("chatter", 1000);

    ros::Timer timer1 = n.createTimer(ros::Duration(1.0/hertz), pub_message);

    startT = ros::Time::now();

    ros::spin();

    return 0;

}

