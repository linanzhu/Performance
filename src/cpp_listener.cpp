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
double thz = 0.0;

void chatterCallback(const performance_tests::SuperAwesome msg)
{
    // Compare time and hertz
    ros::Time nt = ros::Time::now();
    ros::Duration lPeriodT = nt - oldListenerTime;          // listener period
    oldListenerTime = nt;

    thz = (double)msg.hertz;
    lhz = 1.0 / lPeriodT.toSec();            // listener rate (hz)
}

void pub_result()
{
    ros::NodeHandle n;
    ros::Rate loop_rate(1000);


    while (ros::ok())
    {
        double sumdhz = 0.0;
        for(int k = 0; k < 1000; k++)
        {
            double diffhz = fabs(lhz - thz);
            sumdhz += diffhz;
            loop_rate.sleep();
        }
        double avrdhz = sumdhz / 500.0;

        ROS_INFO("cpp result hertz diff : %f", avrdhz);
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
