#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <sensor_msgs/NavSatFix.h>
//DONE:丢掉不必要的东西，gps信息单独做成一个node
//TODO:更多的setpoint 模式
void  pose_setting(geometry_msgs::PoseStamped &pose , const double &pose_x, const double &pose_y, const double &pose_z);

geometry_msgs::PoseStamped mission_pose;
mavros_msgs::State current_state;

int timecount=0;


//callback
void state_cb(const mavros_msgs::State::ConstPtr& msg)
{
  current_state = *msg;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "offboard_node");
  ros::NodeHandle nh;

  ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("mavros/state", 10, state_cb);

  ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>
      ("mavros/setpoint_position/local", 10);
  ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
      ("mavros/cmd/arming");
  ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
      ("mavros/set_mode");

  //the setpoint publishing rate MUST be faster than 2Hz
  // 20Hz, which is 5ms
  ros::Rate rate(20.0);

  // wait for FCU connection
  while(ros::ok() && current_state.connected)
  {
    ros::spinOnce();
    rate.sleep();
  }

  pose_setting(mission_pose, 0, 0, 5);
  //send a few setpoints before starting
  for(int i = 100; ros::ok() && i > 0; --i)
  {
    // todo: send mission pose step by step
    local_pos_pub.publish(mission_pose);
    ros::spinOnce();
    rate.sleep();
  }

  mavros_msgs::SetMode offb_set_mode;
  offb_set_mode.request.custom_mode = "OFFBOARD";

  mavros_msgs::CommandBool arm_cmd;
  arm_cmd.request.value = true;

  ros::Time last_request = ros::Time::now();
//state 1;offboard and arm and setposition 1
//TODO：可以寻找一种定时器的方式替代while循环，或者用另一个线程发送点与chek是否到达,第二种方案可取度更高
 while(ros::ok() && timecount<600)
{        
        if( current_state.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request > ros::Duration(5.0)))
        {
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent)
                 {
                ROS_INFO("Offboard enabled");
                 }
            last_request = ros::Time::now();
       } else
         {
            if( !current_state.armed &&
                (ros::Time::now() - last_request > ros::Duration(5.0)))
            {
                if( arming_client.call(arm_cmd) &&
                    arm_cmd.response.success)
                {
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            } 
         }


//setpoint and go there,
        local_pos_pub.publish(mission_pose);        
        timecount++;
        ros::spinOnce();
        rate.sleep();
}
     

    //TODO:check position if get there went back;对于set_localpoint,可以用/mavros/global_position/local监听反馈信息,这个方案被否决，y轴在飘
    //                                                                  /mavros/local_position/pose 对应可以用这一个
    //state 2; setposition 2
while(ros::ok()&&timecount<1200)
{
      pose_setting(mission_pose, 0, 5, 5);
      local_pos_pub.publish(mission_pose);
      timecount++;
      ros::spinOnce();
      rate.sleep();     
}

    //if neccessary, change into manual
    //  while(timecount>2000),导致线程不能中断结束,
/*      offb_set_mode.request.custom_mode = "MANUAL";
      if( current_state.mode != "MANUAL" && (ros::Time::now() - last_request > ros::Duration(5.0)))
      {
        if( set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent)
        {
          ROS_INFO("Offboard disabled");
        }
        last_request = ros::Time::now();
      } */

/*STABILIZED模式,在没有rc_in的情况下发生的状况不是很好，一直处于failsafe
offb_set_mode.request.custom_mode = "STABILIZED";
last_request = ros::Time::now();
while(ros::ok())
{
    if( current_state.mode != "STABILIZED" &&
            (ros::Time::now() - last_request > ros::Duration(5.0)))
        {
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent)
                 {
                ROS_INFO("STABILIZED enabled");
                 }
            last_request = ros::Time::now();
       } 
}*/
//MANUAL 模式,仍然会降落
/*offb_set_mode.request.custom_mode = "MANUAL";
last_request = ros::Time::now();
while(ros::ok())
{
    if( current_state.mode != "MANUAL" &&
            (ros::Time::now() - last_request > ros::Duration(5.0)))
        {
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent)
                 {
                ROS_INFO("MANUAL enabled");
                 }
            last_request = ros::Time::now();
       } 
}*/
//AUTO.LAND TODO:加着陆检测
offb_set_mode.request.custom_mode = "AUTO.LAND";
last_request = ros::Time::now();
while(ros::ok())
{
    if( current_state.mode != "AUTO.LAND" &&
            (ros::Time::now() - last_request > ros::Duration(5.0)))
        {
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent)
                 {
                ROS_INFO("AUTO.LAND enabled");
                 }
            last_request = ros::Time::now();
       } 
}

  timecount=0;

  return 0;
}


void  pose_setting(geometry_msgs::PoseStamped &pose, const double & pose_x, const double & pose_y, const double & pose_z)
{
  pose.pose.position.x = pose_x;
  pose.pose.position.y = pose_y;
  pose.pose.position.z = pose_z;
}
