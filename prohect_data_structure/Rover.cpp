#include "Rover.h"

Rover::Rover(int id, RoverType t, int spd, int mBeforeCheck, int checkDur)
    : ID(id), type(t), speed(spd), missionsBeforeCheckup(mBeforeCheck),
    checkupDuration(checkDur), missionsCompleted(0), availableDay(0), inCheckup(false) {
}



int Rover::getID() const { return ID; }
RoverType Rover::getType() const { return type; }
int Rover::getSpeed() const { return speed; }
int Rover::getAvailableDay() const { return availableDay; }
bool Rover::isInCheckup() const { return inCheckup; }
int Rover::getCheckupDuration() const { return checkupDuration; }
int Rover::getMissionsCompleted() const { return missionsCompleted; }

void Rover::setAvailableDay(int day) { availableDay = day; }
void Rover::setInCheckup(bool status) { inCheckup = status; }
void Rover::incrementMissions() { missionsCompleted++; }

bool Rover::needsCheckup() const {
    return missionsCompleted >= missionsBeforeCheckup;
}
