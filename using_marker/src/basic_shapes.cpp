#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

int main ( int argc, char** argv )
{
   ros::init(argc, argv, "basic_shapes");
   ros::NodeHandle n;
   ros::Rate r(100);
   ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
 
   // Set our initial shape type to be a cube
   uint32_t shape = visualization_msgs::Marker::SPHERE;
   float xx=0.0;
   int xxx=0;
   while (ros::ok()&&xxx<101)
   {
     visualization_msgs::Marker marker;
     // Set the frame ID and timestamp.  See the TF tutorials for information on these.
     marker.header.frame_id = "/world";
     marker.header.stamp = ros::Time::now();
     // Set the namespace and id for this marker.  This serves to create a unique ID
     // Any marker sent with the same namespace and id will overwrite the old one
     marker.ns = "basic_shapes";
     marker.id = 0;
     // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
     marker.type = shape;
     // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
     marker.action = visualization_msgs::Marker::ADD;
     // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
     marker.pose.position.x = -0.251075226015-0.09+xx;
     marker.pose.position.y = -1.03561116624+0.3-xx;
     marker.pose.position.z = -0.0953591102715;
     xx=xx+0.01;
     xxx=xxx+1;
     marker.pose.orientation.x = 0.0;
     marker.pose.orientation.y = 0.0;
     marker.pose.orientation.z = 0.0;
     marker.pose.orientation.w = 1.0;
     marker.scale.x = 0.2;
     marker.scale.y = 0.1;
     marker.scale.z = 0.1;
     marker.color.r = 0.0f;
     marker.color.g = 1.0f;
     marker.color.b = 0.0f;
     marker.color.a = 1.0;
     marker.lifetime = ros::Duration();
     while (marker_pub.getNumSubscribers() < 1)
     {
       if (!ros::ok())
       {
         return 0;
       }
       ROS_WARN_ONCE("Please create a subscriber to the marker");
       sleep(1);
     }
     
     marker_pub.publish(marker);
    r.sleep();
   }
 }