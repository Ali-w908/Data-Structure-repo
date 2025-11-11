#include "Mission.h"
#include "Rover.h"
Mission::Mission(int id, MissionType t, int rday, int tloc, int mdur)
    : ID(id), type(t), Rday(rday), TLOC(tloc), MDUR(mdur),
    Wdays(0), Tdays(0), Fday(0), status(WAITING) {
	assignedRover = nullptr;
}

int Mission::getID() const { return ID; }
MissionType Mission::getType() const { return type; }
int Mission::getRday() const { return Rday; }
int Mission::getTLOC() const { return TLOC; }
int Mission::getMDUR() const { return MDUR; }
int Mission::getWdays() const { return Wdays; }
int Mission::getTdays() const { return Tdays; }
int Mission::getFday() const { return Fday; }
int Mission::getLocationArrivalDay(int today) const
{
    if(assignedRover)
	    return today + TLOC / assignedRover->getSpeed();
	return -1;
}
Rover* Mission::getAssignedRover() const{ return assignedRover; }
MissionStatus Mission::getStatus() const { return status; }

void Mission::setStatus(MissionStatus s) { status = s; }
void Mission::setWdays(int w) { Wdays = w; }
void Mission::setTdays(int t) { Tdays = t; }
void Mission::setFday(int f) { Fday = f; }

void Mission::setAssignedRover(Rover* r)
{
	assignedRover = r;
}

void Mission::calculateWdays(int currentDay) {
    Wdays = currentDay - Rday;
}

void Mission::calculateTdays(int roverSpeed) {
    int travelTime = (2 * TLOC) / roverSpeed;
    Tdays = travelTime + MDUR;
}

void Mission::calculateFday() {
    Fday = Rday + Wdays + Tdays;
}
ostream& operator<<(ostream& out, Mission* M)
{
	out << M->getID();
    if(M->getAssignedRover())
		out << "/" << M->getAssignedRover()->getID();
	return out;
}