#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
#include <mavros_msgs/PositionTarget.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
mavros_msgs::State current_state;
int timecount=0;
//TODO:程序中断时无人机的模式切换。
//TODO:丢掉不必要的东西，还原最基本的实现逻辑（飞单点），然后用action_lib等实现移动的逻辑
void  pose_setting(mavros_msgs::PositionTarget &pose , const double &pose_x, const double &pose_y, const double &pose_z)
{
//TODO:这个状态坐标系是怎么样子的？需要解释
  pose.position.x = pose_x;
  pose.position.y = pose_y;
  pose.position.z = pose_z;
}
void state_cb(const mavros_msgs::State::ConstPtr& msg)
{
  current_state = *msg;
}
int main(int argc, char **argv)
{ 
  ros::init(argc, argv, "offboard_node");
  ros::NodeHandle nh;

  ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("mavros/state", 10, state_cb);
  ros::Publisher local_pos_pub = nh.advertise<mavros_msgs::PositionTarget>
      ("/mavros/setpoint_raw/local", 10);
  ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
      ("mavros/cmd/arming");
  ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
      ("mavros/set_mode");

  //the setpoint publishing rate MUST be faster than 2Hz
  ros::Rate rate(20.0);
   
  // wait for FCU connection ,这一步是否需要？
   //官网给出的解释是：在发布之前，需要等待MAVROS和飞控建立连接。一旦接收到心跳包，该循环就会立即退出。
  while(ros::ok() && current_state.connected)
  {
    ros::spinOnce();
    rate.sleep();
  }
  mavros_msgs::PositionTarget pose;
  pose_setting(pose, 0, 0, 20);

  //send a few setpoints before starting
  for(int i = 100; ros::ok() && i > 0; --i)
  {
    // 经过测试，send mission pose step by step ; 这个模式下没有效果
    local_pos_pub.publish(pose);
    ros::spinOnce();
    rate.sleep();
  }

  mavros_msgs::SetMode offb_set_mode;
  offb_set_mode.request.custom_mode = "OFFBOARD";

  mavros_msgs::CommandBool arm_cmd;
  arm_cmd.request.value = true;
//TODO:一个测试，将offboard和arm模式用mavros提供的命令行模式完成,另外提供线程用于设置目标
//在请求之间间隔5秒，不至于让飞控响应不过来。在更大的系统中，创建一个新的负责周期性发送目标指令的线程往往更加有用。
  ros::Time last_request = ros::Time::now();
  while(ros::ok()&&timecount<1000)
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


//setpoint and go there,对于set_localpoint,可以用/mavros/global_position/local监听反馈信息
        local_pos_pub.publish(pose);
        timecount ++;
//TODO:切换模式，使降落。DONE
        ros::spinOnce();
        rate.sleep();
}
timecount=0;
offb_set_mode.request.custom_mode = "MANUAL";
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
}

  return 0;
}


