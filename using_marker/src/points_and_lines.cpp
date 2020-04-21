#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <iostream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

int main( int argc, char** argv )
{
  ros::init(argc, argv, "points_and_lines");
  ros::NodeHandle n;
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);

  ros::Rate r(30);

  float f = 0.0;
  while (ros::ok())
  {

    //创建一个 visualization_msgs/Marker消息，并且初始化所有共享的数据。消息成员默认为0，仅仅设置位姿成员w。
    visualization_msgs::Marker points, line_strip, line_list;
    points.header.frame_id = line_strip.header.frame_id = line_list.header.frame_id = "/world";
    points.header.stamp = line_strip.header.stamp = line_list.header.stamp = ros::Time::now();
    points.ns = line_strip.ns = line_list.ns = "points_and_lines";
    points.action = line_strip.action = line_list.action = visualization_msgs::Marker::ADD;
    points.pose.orientation.w = line_strip.pose.orientation.w = line_list.pose.orientation.w = 1.0;


    //分配三个不同的id到三个markers。points_and_lines名称空间的使用确保彼此不会相互冲突。
    points.id = 0;
    line_strip.id = 1;
    line_list.id = 2;


    //设置marker类型到 POINTS, LINE_STRIP 和 LINE_LIST
    points.type = visualization_msgs::Marker::POINTS;
    line_strip.type = visualization_msgs::Marker::LINE_STRIP;
    line_list.type = visualization_msgs::Marker::LINE_LIST;



    // scale成员对于这些marker类型是不同的,POINTS marker分别使用x和y作为宽和高，然而LINE_STRIP和LINE_LIST marker仅仅使用x，定义为线的宽度。单位是米。
    points.scale.x = 0.2;
    points.scale.y = 0.2;

    // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
    line_strip.scale.x = 0.03;
    line_list.scale.x = 0.1;



    // 点为绿色
    points.color.g = 1.0f;
    points.color.a = 1.0;

    // Line strip 是蓝色
    line_strip.color.b = 1.0;
    line_strip.color.a = 1.0;

    // Line list 为红色
    line_list.color.r = 1.0;
    line_list.color.a = 1.0;



    //使用正弦和余弦生成螺旋结构。POINTS和LINE_STRIP markers都仅仅需要1个点作为每个顶点，然而LINE_LIST marker需要2个点 。
    // for (uint32_t i = 0; i < 30; ++i)
    // {
    //   float y = 1 * sin(f + i / 100.0f * 2 * M_PI);
    //   float z = 1 * cos(f + i / 100.0f * 2 * M_PI);
    
    float I[101][2] = {0};
    ifstream myfile("/home/xzq/ros_ws/src/using_marker/data/11.txt");
    for (int i = 0; i < 101; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			myfile >> I[i][j];
		}
	} 

    for(int i = 0; i < 101; i++)
    {
      geometry_msgs::Point p;
      p.x = -0.449104526502-I[i][0];
      p.y = -1.05241432729+I[i][1];
      p.z = -0.122631197113;
      // cout << p.x << endl;
      // cout << p.y << endl;

      points.points.push_back(p);
      line_strip.points.push_back(p);

      // The line list needs two points for each line
      line_list.points.push_back(p);
      p.z += 0.05;
      line_list.points.push_back(p);
    }
    myfile.close();

    //发布各个markers
    // marker_pub.publish(points);
    marker_pub.publish(line_strip);
    // marker_pub.publish(line_list);

    r.sleep();
  }
}