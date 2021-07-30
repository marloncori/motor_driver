// C++ standard headers
   //http://wiki.ros.org/Robots/ARI/Joint%20Trajectory%20Controller
   2 #include <exception>
   3 #include <string>
   4
   5 // Boost headers
   6 #include <boost/shared_ptr.hpp>
   7
   8 // ROS headers
   9 #include <ros/ros.h>
  10 #include <actionlib/client/simple_action_client.h>
  11 #include <control_msgs/FollowJointTrajectoryAction.h>
  12 #include <ros/topic.h>
  13
  14
  15 // Our Action interface type for moving ARI's head, provided as a typedef for convenience
  16 typedef actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> arm_control_client;
  17 typedef boost::shared_ptr< arm_control_client>  arm_control_client_Ptr;
  18
  19
  20 // Create a ROS action client to move ARI's right arm
  21 void createArmClient(arm_control_client_Ptr& actionClient)
  22 {
  23   ROS_INFO("Creating action client to arm controller ...");
  24
  25   actionClient.reset( new arm_control_client("/arm_right_controller/follow_joint_trajectory") );
  26
  27   int iterations = 0, max_iterations = 3;
  28   // Wait for arm controller action server to come up
  29   while( !actionClient->waitForServer(ros::Duration(2.0)) && ros::ok() && iterations < max_iterations )
  30   {
  31     ROS_DEBUG("Waiting for the arm_right_controller_action server to come up");
  32     ++iterations;
  33   }
  34
  35   if ( iterations == max_iterations )
  36     throw std::runtime_error("Error in createArmClient: arm controller action server not available");
  37 }
  38
  39
  40 // Generates a simple trajectory with two waypoints to move ARI's arm
  41 void waypoints_arm_goal(control_msgs::FollowJointTrajectoryGoal& goal)
  42 {
  43   // The joint names, which apply to all waypoints
  44   goal.trajectory.joint_names.push_back("arm_right_1_joint");
  45   goal.trajectory.joint_names.push_back("arm_right_2_joint");
  46   goal.trajectory.joint_names.push_back("arm_right_3_joint");
  47   goal.trajectory.joint_names.push_back("arm_right_4_joint");
  48
  49
  50   // Two waypoints in this goal trajectory
  51   goal.trajectory.points.resize(2);
  52
  53   // First trajectory point
  54   // Positions
  55   int index = 0;
  56   goal.trajectory.points[index].positions.resize(4);
  57   goal.trajectory.points[index].positions[0] = 0.65;
  58   goal.trajectory.points[index].positions[1] = 0.35;
  59   goal.trajectory.points[index].positions[2] = 0;
  60   goal.trajectory.points[index].positions[3] = 0;
  61   // Velocities
  62   goal.trajectory.points[index].velocities.resize(4);
  63   for (int j = 0; j < 4; ++j)
  64   {
  65     goal.trajectory.points[index].velocities[j] = 1.0;
  66   }
  67   // To be reached 3 second after starting along the trajectory
  68   goal.trajectory.points[index].time_from_start = ros::Duration(3.0);
  69
  70   // Second trajectory point
  71   // Positions
  72   index += 1;
  73   goal.trajectory.points[index].positions.resize(4);
  74   goal.trajectory.points[index].positions[0] = 1.13;
  75   goal.trajectory.points[index].positions[1] = 0.17;
  76   goal.trajectory.points[index].positions[2] = 0;
  77   goal.trajectory.points[index].positions[3] = 0;
  78   // Velocities
  79   goal.trajectory.points[index].velocities.resize(4);
  80   for (int j = 0; j < 4; ++j)
  81   {
  82     goal.trajectory.points[index].velocities[j] = 0.0;
  83   }
  84   // To be reached 6 seconds after starting along the trajectory
  85   goal.trajectory.points[index].time_from_start = ros::Duration(6.0);
  86 }
  87
  88
  89 // Entry point
  90 int main(int argc, char** argv)
  91 {
  92   // Init the ROS node
  93   ros::init(argc, argv, "run_traj_control");
  94
  95   ROS_INFO("Starting run_traj_control application ...");
  96
  97   // Precondition: Valid clock
  98   ros::NodeHandle nh;
  99   if (!ros::Time::waitForValid(ros::WallDuration(10.0))) // NOTE: Important when using simulated clock
 100   {
 101     ROS_FATAL("Timed-out waiting for valid time.");
 102     return EXIT_FAILURE;
 103   }
 104
 105   // Create an arm controller action client to move the ARI's right arm
 106   arm_control_client_Ptr ArmClient;
 107   createArmClient(ArmClient);
 108
 109   // Generates the goal for the ARI's right arm
 110   control_msgs::FollowJointTrajectoryGoal arm_goal;
 111   waypoints_arm_goal(arm_goal);
 112
 113   // Sends the command to start the given trajectory 1s from now
 114   arm_goal.trajectory.header.stamp = ros::Time::now() + ros::Duration(1.0);
 115   ArmClient->sendGoal(arm_goal);
 116
 117   // Wait for trajectory execution
 118   while(!(ArmClient->getState().isDone()) && ros::ok())
 119   {
 120     ros::Duration(4).sleep(); // sleep for four seconds
 121   }
 122
 123   return EXIT_SUCCESS;
 124 }
