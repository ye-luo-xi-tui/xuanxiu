//
// Created by yezi on 2022/1/17.
//

#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/Twist.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "track_node");

  ros::NodeHandle node;
  ros::Publisher cmd_vel_pub =
      node.advertise<geometry_msgs::Twist>("/controllers/chassis_base_controller/cmd_vel", 10);

  tf2_ros::Buffer tf_buffer;
  tf2_ros::TransformListener tf_listener(tf_buffer);
  geometry_msgs::Twist vel_msg;

  ros::Rate rate(100.0);
  while (node.ok())
  {
    geometry_msgs::TransformStamped transform_stamped;
    try
    {
      transform_stamped = tf_buffer.lookupTransform("base_link", "object_4",
                                                  ros::Time(0));
    }
    catch (tf2::TransformException &ex)
    {
      ROS_WARN("%s",ex.what());
      continue;
    }

    auto now = ros::Time::now();
    if((now - transform_stamped.header.stamp).toSec() < 0.5)
    {
      vel_msg.angular.z = 4.0 * atan2(transform_stamped.transform.translation.y,transform_stamped.transform.translation.x);
      vel_msg.linear.x = 0.5 * sqrt(pow(transform_stamped.transform.translation.x, 2) + pow(transform_stamped.transform.translation.y, 2));
    }
    else
    {
      vel_msg.angular.z = 0;
      vel_msg.linear.x = 0;
    }
    cmd_vel_pub.publish(vel_msg);
    rate.sleep();
  }
  return 0;
}