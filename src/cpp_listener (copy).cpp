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
    int window_size = 1000;     // maximum size to average rate
    std::deque<double> qListenerHZ;

    ros::NodeHandle n;
    ros::Publisher result_pub = n.advertise<std_msgs::String>("result_hz", 1);

    ros::Time st;
    ros::Rate loop_rate(1000);

    while (ros::ok())
    {

        st = ros::Time::now();

		// Get data during 1 sec
        while (true)
        {
            if ((ros::Time::now() - st) > ros::Duration(1.0)) break;
            if (qListenerHZ.size() >= window_size) qListenerHZ.pop_front();

            qListenerHZ.push_back(lhz);
            loop_rate.sleep();
        }

        // Get average rate
        double sum_lhz = 0.0;
        for (std::deque<double>::iterator it = qListenerHZ.begin(); it!=qListenerHZ.end(); ++it)
        {
            sum_lhz += *it;
        }
        double avr_lhz = sum_lhz / (double)qListenerHZ.size();

        // Get standard deviation
        double square_err_sum_lhz = 0.0;
        for (std::deque<double>::iterator it = qListenerHZ.begin(); it!=qListenerHZ.end(); ++it)
        {
            double diff = *it - avr_lhz;
            square_err_sum_lhz += (diff * diff);
        }
        double mean_square_err_lhz = square_err_sum_lhz / (double)qListenerHZ.size();
        double std_lhz = sqrt(mean_square_err_lhz);

        //ROS_INFO("    lhz : %f", lhz);

        std_msgs::String result_msg;
        std::stringstream ss;
        ss << "result hertz : " << avr_lhz << "    std : " << std_lhz;
        result_msg.data = ss.str();

        result_pub.publish(result_msg);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "cpp_listener");

    boost::thread thread_result(pub_result);

    ros::NodeHandle n;

    // Initialize base time as current time
    oldListenerTime = ros::Time::now();

    ros::Subscriber sub = n.subscribe("chatter", 1, chatterCallback);


    ros::spin();

    thread_result.join();

    return 0;
}
