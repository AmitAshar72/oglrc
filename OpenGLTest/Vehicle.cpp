#include "Vehicle.h"

void Vehicle::Draw(Shader& shader, Camera& cam)
{
    UpdateVehiclePosition(shader, cam);
    Model::Draw(shader, cam);
}

void Vehicle::HandleVehicleInputs(GLFWwindow* window, float deltaTime)
{
    // Handle input
    bool forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool brake = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

    UpdateVehicleMovement(forward, backward, left, right, brake, deltaTime);
}

void Vehicle::UpdateVehicleMovement(bool forward, bool backward, bool left, bool right, bool brake, float deltaTime)
{
    //brake
    if (brake) {
        if (Speed > 0.0f) {
            Speed -= Deceleration * deltaTime * 10.0f;
            Speed = glm::max(0.0f, Speed); // Ensure speed doesn't go negative
        }
        else if (Speed < 0.0f) {
            Speed += Deceleration * deltaTime * 10.0f;
            Speed = glm::min(0.0f, Speed); // Ensure speed doesn't go positive
        }
    }

    // Acceleration/Deceleration
    if (forward) {
        Speed -= Acceleration * deltaTime;
    }
    else if (backward) {
        Speed += Deceleration * deltaTime;
    }
    else {
        // Apply deceleration if no input
        if (Speed > 0) {
            Speed -= Deceleration * deltaTime;
        }
        else if (Speed < 0) {
            Speed += Deceleration * deltaTime;
        }
    }

    // Clamp speed within max_speed
    Speed = std::min(Speed, Max_speed);
    Speed = std::max(Speed, -Max_speed);

    // Steering
    if (left) {
        Orientation += Steer_magnitude * deltaTime;
    }
    else if (right) {
        Orientation -= Steer_magnitude * deltaTime;
    }

    // Clamp orientation within max_angle
    Orientation = std::min(Orientation, Max_angle);
    Orientation = std::max(Orientation, -Max_angle);

    ModelPosition.x += Speed * std::sin(glm::radians(Orientation)) * deltaTime;
    ModelPosition.z += Speed * std::cos(glm::radians(Orientation)) * deltaTime;
}

double Vehicle::CalculateRPM(double throttle) {

    // Calculate the engine's torque based on the throttle position
    double torque = 0.0;
    
    // Find the point in the torque curve with the highest RPM that is less than or equal to the current throttle
    auto it = std::lower_bound(torqueCurve.begin(), torqueCurve.end(), throttle, [](const TorqueCurvePoint& point, double value) {
        return point.rpm < value;
        });

    // If we found a point, calculate the RPM based on the torque and vehicle speed
    if (it != torqueCurve.end() && it->rpm <= throttle) {
        double wheelRadius = 0.3; // meters
        double gearRatio = 4.0;
        double finalDriveRatio = 3.0;
        double vehicleSpeed = 10.0; // meters per second
        double engineSpeed = (60.0 * vehicleSpeed) / (2.0 * M_PI * wheelRadius * gearRatio * finalDriveRatio);
        return engineSpeed * it->torque / 5252.0;
    }

    // If we didn't find a point, return 0 RPM
    return 0.0;
}

double Vehicle::CalculateEngineForce(double rpm, const std::vector<TorqueCurvePoint>& torqueCurve) {
    // Find the index of the closest RPM value in the torque curve
    int index = 0;
    double minDiff = std::abs(rpm - torqueCurve[0].rpm);
    for (int i = 1; i < torqueCurve.size(); i++) {
        double diff = std::abs(rpm - torqueCurve[i].rpm);
        if (diff < minDiff) {
            minDiff = diff;
            index = i;
        }
    }

    // Interpolate the torque value at the current RPM
    double torque = torqueCurve[index].torque;
    double slope = (torqueCurve[index + 1].torque - torque) / (torqueCurve[index + 1].rpm - torqueCurve[index].rpm);
    torque += slope * (rpm - torqueCurve[index].rpm);

    // Calculate the engine force
    double force = torque * 2 * M_PI * rpm / 60;

    return force;
}

void Vehicle::UpdateVehiclePosition(Shader& shader, Camera& cam)
{
    cam.UpdateCameraMatrix(shader);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, ModelPosition);
    model = glm::rotate(model, glm::radians(Orientation) + glm::radians(ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, ModelScale);
    shader.setMat4("model", model);
}