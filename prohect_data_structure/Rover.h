#pragma once
#include <string>
#include <iostream>
using namespace std;
enum RoverType { NR, PR, DR }; // Normal, Polar, Drill

class Rover {
private:
    const int ID;
    const RoverType type;
    const int speed; // km/h
    const int missionsBeforeCheckup;
    const int checkupDuration; // in days
    int missionsCompleted;
    int availableDay;
    bool inCheckup;

public:
    Rover(int id, RoverType t, int spd, int mBeforeCheck, int checkDur);
    // === Getters ===
    int getID() const;
    RoverType getType() const;
    int getSpeed() const;
    int getAvailableDay() const;
    bool isInCheckup() const;
    int getCheckupDuration() const;
    int getMissionsCompleted() const;

    // === Setters ===
    void setAvailableDay(int day);
    void setInCheckup(bool status);
    void incrementMissions();

    // === Utility ===
    bool needsCheckup() const;
};
ostream& operator<<(ostream& out, Rover* R);
