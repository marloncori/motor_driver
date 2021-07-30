#include <ros/ros.h>
#include "motor_driver/motor_driver.h"
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <std_msgs/Int32.h>
#include <std_srvs/Trigger.h>
#include <diagnostic_msgs/DiagnosticStatus.h>
#include <diagnostic_msgs/KeyValue.h>

class MotorDriverROSWrapper
{
private:
    std::unique_ptr<MotorDriver> motor_;
    ros::Subscriber speed_command_subscriber_;
    ros::ServiceServer stop_motor_server_;
    ros::Publisher current_speed_publisher_;
    ros::Publisher motor_status_publisher_;
    ros::Timer current_speed_timer_;
    ros::Timer motor_status_timer_;
    double publish_current_speed_frequency_;
    double publish_motor_status_frequency_;
public:
    MotorDriverROSWrapper(ros::NodeHandle *nh)
    {
        int max_speed;
        if (!ros::param::get("~max_speed", max_speed))
        {
            max_speed = 8;
        }
        if (!ros::param::get("~publish_current_speed_frequency", publish_current_speed_frequency_))
        {
            publish_current_speed_frequency_ = 5.0;
        }
        if (!ros::param::get("~publish_motor_status_frequency", publish_motor_status_frequency_))
        {
            publish_motor_status_frequency_ = 1.0;
        }
        motor_.reset(new MotorDriver(max_speed));
        speed_command_subscriber_ = nh->subscribe(
            "speed_command", 10, &MotorDriverROSWrapper::callbackSpeedCommand, this);

        stop_motor_server_ = nh->advertiseService(
            "stop_motor", &MotorDriverROSWrapper::callbackStop, this);
        current_speed_publisher_ = nh->advertise<std_msgs::Int32>("current_speed", 10);
        motor_status_publisher_ = nh->advertise<diagnostic_msgs::DiagnosticStatus>("motor_status", 10);
        current_speed_timer_ = nh->createTimer(
            ros::Duration(1.0 / publish_current_speed_frequency_),
            &MotorDriverROSWrapper::publishCurrentSpeed, this);
        motor_status_timer_ = nh->createTimer(
            ros::Duration(1.0 / publish_motor_status_frequency_),
            &MotorDriverROSWrapper::publishMotorStatus, this);
    }
    void publishCurrentSpeed(const ros::TimerEvent &event)
    {
        std_msgs::Int32 msg;
        msg.data = motor_->getSpeed();
        current_speed_publisher_.publish(msg);
    }
    void publishMotorStatus(const ros::TimerEvent &event)
    {
        diagnostic_msgs::DiagnosticStatus msg;
        std::map<std::string, std::string> status = motor_->getStatus();
        std::vector<diagnostic_msgs::KeyValue> values;
        for (auto const &x : status)
        {
            diagnostic_msgs::KeyValue value;
            value.key = x.first;
            value.value = x.second;
            values.push_back(value);
        }
        msg.values = values;
        motor_status_publisher_.publish(msg);
    }
    void stop()
    {
        motor_->stop();
    }
    void callbackSpeedCommand(const std_msgs::Int32 &msg)
    {
        motor_->setSpeed(msg.data);
    }
    bool callbackStop(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res)
    {
        stop();
        res.success = true;
        res.message = "Successfully stopped motor.";
        return true;
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "motor_driver");
    ros::NodeHandle nh;
    ros::AsyncSpinner spinner(4);
    spinner.start();

    MotorDriverROSWrapper motor_driver_wrapper(&nh);
    ROS_INFO("Motor driver is now started");
    ros::waitForShutdown();
    motor_driver_wrapper.stop();
}
