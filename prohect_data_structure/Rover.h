#pragma once
#include <string>

enum RoverType { NR, PR, DR }; // Normal, Polar, Drill

class Rover {
private:
    int ID;
    RoverType type;
    int speed; // km/h
    int missionsBeforeCheckup;
    int checkupDuration; // in days
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

