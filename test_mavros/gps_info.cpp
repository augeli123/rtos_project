#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <sensor_msgs/NavSatFix.h>
#include <iostream>

geometry_msgs::Point gps_current;  

//TODO:暂时没有加入 gps_home, gps_mission 的逻辑
//TODO:考虑其他存储方式，转化坐标系
//用一个三维的点填充[latitude,longitude,altitude] 纬度，经度，海拔  见：sensor_msgs/NavSatFix.msg
void  point_getting(geometry_msgs::Point &position,
                    const double & latitude, const double & longitude, const double & altitude)
{
  position.x = latitude; 
  position.y = longitude; 
  position.z = altitude; 
}

void gpsCallBack(const sensor_msgs::NavSatFixConstPtr& gps)
{
  point_getting(gps_current, gps->latitude, gps->longitude, gps->altitude);
  std::cout<<"纬度:[%f] "<<gps_current.x<<"经度:[%f] "<<gps_current.y<<"海拔：[%f] "<<gps_current.z<<std::endl;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "GPS_INFO");
  ros::NodeHandle nh;  
  ros::Subscriber gps_sub = nh.subscribe("mavros/global_position/raw/fix", 10, gpsCallBack);
  ros::spin();
//TODO: 加入适当的延时
  return 0;
}

//备注:home_position可以直接获取，pix4以起飞点作为home, 起飞后，订阅 /mavros/home_position/home

