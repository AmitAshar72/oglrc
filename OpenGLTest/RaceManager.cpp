#include "RaceManager.h"

void RaceManager::Update(float deltaTime) {
    if (!timerStarted_ && isOverlapping(vehicle_, raceLine1_)) {
        timerStarted_ = true;
        startTime_ = glfwGetTime();
    }

    if (timerStarted_ && !timerEnded_ && isOverlapping(vehicle_, raceLine2_)) {
        timerEnded_ = true;
        endTime_ = glfwGetTime();
    }
}

float RaceManager::GetElapsedTime() {
    if (timerEnded_) {
        return endTime_ - startTime_;
    }
    return 0.0f;
}

void RaceManager::ResetRace()
{
    StartRace = false;
    timerStarted_ = false;
    timerEnded_ = false;
    startTime_ = 0.0f;
    endTime_ = 0.0f;
}


bool RaceManager::isOverlapping(Model& model1, Mesh& model2) {
    // Implement overlap detection using bounding boxes or a more advanced method
    // For simplicity, assume a simple bounding box overlap detection

    model1.UpdateModelBounds();
    BoundingBox mod2 = model2.GetMeshBoundingBox();

    float minX1 = model1.getMinX() + model1.ModelPosition.x;
    float maxX1 = model1.getMaxX() + model1.ModelPosition.x;
    float minY1 = model1.getMinY() + model1.ModelPosition.y;
    float maxY1 = model1.getMaxY() + model1.ModelPosition.y;
    float minZ1 = model1.getMinZ() + model1.ModelPosition.z;
    float maxZ1 = model1.getMaxZ() + model1.ModelPosition.z;

    float minX2 = mod2.getMinX() + model2.Position.x;
    float maxX2 = mod2.getMaxX() + model2.Position.x;
    float minY2 = mod2.getMinY() + model2.Position.y;
    float maxY2 = mod2.getMaxY() + model2.Position.y;
    float minZ2 = mod2.getMinZ() + model2.Position.z;
    float maxZ2 = mod2.getMaxZ() + model2.Position.z;

    return (maxX1 >= minX2 && minX1 <= maxX2) &&
        (maxY1 >= minY2 && minY1 <= maxY2) &&
        (maxZ1 >= minZ2 && minZ1 <= maxZ2);
}