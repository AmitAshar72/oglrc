#ifndef VEHICLE_CLASS_H
#define VEHICLE_CLASS_H

#include "Model.h"
#include <cmath>
#include <corecrt_math_defines.h>

struct TorqueCurvePoint 
{
    double rpm;
    double torque;
};

class Vehicle:public Model
{
public:
    std::vector<TorqueCurvePoint> torqueCurve;

    Vehicle(std::string const& path, glm::vec3& initPos, float mass, float acceleration, float deceleration, float maxSpeed)
        : Model(path, initPos), Mass(mass), Acceleration(acceleration), Deceleration(deceleration), Max_speed(maxSpeed)
    {       
        Orientation = 0.0f;
        Steer_magnitude = 5.0f;
        Max_angle = 45.0f;
        CurrentDirection = glm::vec3(0.0f, 0.0f, 1.0f);

        torqueCurve = {
            {0.0, 0.0},
            {1000.0, 50.0},
            {2000.0, 100.0},
            {3000.0, 150.0},
            {4000.0, 200.0},
            {5000.0, 250.0},
            {6000.0, 300.0}
        };

    }   
    void Draw(Shader& shader, Camera& cam) override;
    void HandleVehicleInputs(GLFWwindow* window, float deltaTime);
    double CalculateRPM(double throttle);    
    double CalculateEngineForce(double rpm, const std::vector<TorqueCurvePoint>& torqueCurve);
    void SetAcceleration(double& ac)
    {        
        Acceleration = ac/Mass;
        std::cout << "Acceleration: " << Acceleration << std::endl;
    }
    float GetAcceleration() 
    {
        return Acceleration;
    }
    // Function to update the car's state
    void update(float dt) {
        // Integrate the acceleration to get the new velocity
        Speed -= Acceleration * dt;

        // Integrate the velocity to get the new position
        //ModelPosition += Speed * dt;

        ModelPosition.x += Speed * std::sin(glm::radians(Orientation)) * dt;
        ModelPosition.z += Speed * std::cos(glm::radians(Orientation)) * dt;
        // Reset the acceleration to zero
        //Acceleration = 0.0f;
    }

private:
    
    glm::vec3 CurrentVelocity;
    glm::vec3 CurrentDirection;

    float Mass;
    float Speed;
    float Acceleration;
    float Deceleration;
    float Max_speed;
    float Orientation;
    float Steer_magnitude;
    float Max_angle;

    void UpdateVehicleMovement(bool forward, bool backward, bool left, bool right, bool brake, float deltaTime);
    void UpdateVehiclePosition(Shader& shader, Camera& cam);
};

#endif