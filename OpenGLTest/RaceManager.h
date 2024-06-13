#ifndef RACEMANAGER_CLASS_H
#define RACEMANAGER_CLASS_H

#include "Model.h"

class RaceManager {
public:
    RaceManager(Model& vehicle, Mesh& raceTrack, Mesh& raceLine1, Mesh& raceLine2)
        : vehicle_(vehicle), raceTrack_(raceTrack), raceLine1_(raceLine1), raceLine2_(raceLine2),
        timerStarted_(false), timerEnded_(false), startTime_(0.0f), endTime_(0.0f) {}

    bool StartRace = false;
    void Update(float deltaTime);
    float GetElapsedTime();
    void ResetRace();

private:
    bool isOverlapping(Model& model1, Mesh& model2);

    Model& vehicle_;
    Mesh& raceTrack_;
    Mesh& raceLine1_;
    Mesh& raceLine2_;
    bool timerStarted_;
    bool timerEnded_;
    float startTime_;
    float endTime_;
};

#endif