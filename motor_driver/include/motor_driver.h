#ifndef MOTOR_DRIVER_H_INCLUDED
#define MOTOR_DRIVER_H_INCLUDED

#include <map>
#include <string>

class MotorDriver{

private:
    int maxSpeed;
    int currentSpeed;
    int voltage;
    int temperature;

public:
    MotorDriver(int max_speed=10){
        maxSpeed = max_speed;
        currentSpeed = 0;
        voltage = 12;
        temperature = 47;
    }

    void setSpeed(int speed){
        if(speed <= maxSpeed){
            currentSpeed = speed;
        }
        else{
            currentSpeed = maxSpeed;
        }
    }
    void stop(){
        currentSpeed = 0;
    }

    int getSpeed(){
        return currentSpeed;
    }
    std::map<std::string, std::string> getStatus(){
        std::map<std::string, std::string> status;
        status["temperature"] = std::to_string(temperature);
        status["voltage"] = std::to_string(voltage);

        return status;
    }
};

#endif // MOTOR_DRIVER_H_INCLUDED
