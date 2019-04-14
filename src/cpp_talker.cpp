
#include "ros/ros.h"
#include "performance_tests/SuperAwesome.h"

#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "cpp_talker");
    ros::NodeHandle n;

    int hertz = 10;
    if(!(ros::param::get("/cpp_talker/hertz", hertz)))
    {
        ROS_ERROR("Failed to get hertz. Use default(10)");
    }
    cout << "param rate : " << hertz << endl;


    ros::Publisher chatter_pub = n.advertise<performance_tests::SuperAwesome>("pt_msg", 1);

    ros::Rate loop_rate(hertz);

    performance_tests::SuperAwesome msg;
    msg.header.seq = 0;
    msg.hertz = hertz;
    msg.message = "BlueOceanRobotics";

    ros::Time startT = ros::Time::now();

    while(ros::ok())
    {
        msg.header.stamp = ros::Time::now();        

        //ROS_INFO("[%f]", msg.header.stamp.toSec());

        chatter_pub.publish(msg);

        msg.header.seq += 1;

        loop_rate.sleep();

        // measure elapsed time
        ros::Time stopT = ros::Time::now();
        ros::Duration elapsedT = stopT - startT;

        startT = stopT;

    }

    return 0;

}

