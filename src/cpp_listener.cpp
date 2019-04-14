#include "ros/ros.h"
#include "std_msgs/String.h"
#include "performance_tests/SuperAwesome.h"

#include <math.h>
#include <iostream>
#include <deque>
#include <boost/thread/thread.hpp>

using namespace std;

ros::Time oldListenerTime;

//
double lhz = 0.0;


void chatterCallback(const performance_tests::SuperAwesome msg)
{
    // Compare time and hertz
    ros::Time nt = ros::Time::now();
    ros::Duration lPeriodT = nt - oldListenerTime;          // listener period
    oldListenerTime = nt;

    lhz = 1.0 / lPeriodT.toSec();            // listener rate (hz)
}

void pub_result()
{
    ros::NodeHandle n;
    ros::Rate loop_rate(5);

    while (ros::ok())
    {
        ROS_INFO("cpp result hertz : %f", lhz);
        loop_rate.sleep();
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "cpp_listener");

    boost::thread thread_result(pub_result);

    ros::NodeHandle n;

    // Initialize base time as current time
    oldListenerTime = ros::Time::now();

    ros::Subscriber sub = n.subscribe("pt_msg", 1, chatterCallback);


    ros::spin();

    thread_result.join();

    return 0;
}
