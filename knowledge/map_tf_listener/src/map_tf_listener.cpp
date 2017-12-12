#include <ros/ros.h>
#include <tf/transform_listener.h>

int main(int argc, char const *argv[])
{
	ros::init(argc, argv, "map_tf_listener");

	ros::NodeHandle node;
	tf::TransformListener listener;
	ros::Publisher map_trans =
		node.advertise<geometry_msgs::TransformStamped>("map_tf_listener/tf", 10);

	ros::Rate rate(10.0);
  	while (node.ok()){
    	tf::StampedTransform transform;
    	try{
      		listener.lookupTransform("/base_link", "/map",
                               ros::Time(0), transform);
    	}
    	catch (tf::TransformException &ex) {
     	 	ROS_ERROR("%s",ex.what());
   		    ros::Duration(1.0).sleep();
      	continue;
    	}
    	map_trans.publish(transform);
    	rate.sleep();
    }
	return 0;
}