#pragma once
enum MissionType { NM, PM, DM }; // Normal, Polar, Drill
enum MissionStatus { WAITING, READY, IN_EXEC, IN_BACK, DONE, ABORTED };
#include <iostream>
using namespace std;

class Rover;
class Mission {
private:
    const int ID;
    const MissionType type;
    const int Rday;      // Request day
    const int TLOC;      // Target location (km)
    const int MDUR;      // Mission duration (days)
    int Wdays;     // Waiting days
    int Tdays;     // Total mission days
    int Fday;      // Finish day
    MissionStatus status;
	Rover* assignedRover;

public:
    Mission(int id, MissionType t, int rday, int tloc, int mdur);

    // === Getters ===
    int getID() const;
    MissionType getType() const;
    int getRday() const;
    int getTLOC() const;
    int getMDUR() const;
    int getWdays() const;
    int getTdays() const;
    int getFday() const;
	int getLocationArrivalDay(int today) const;
	Rover* getAssignedRover() const;
    MissionStatus getStatus() const;

    // === Setters ===
    void setStatus(MissionStatus s);
    void setWdays(int w);
    void setTdays(int t);
    void setFday(int f);
	void setAssignedRover(Rover* r);

    // === Utility ===
    void calculateWdays(int currentDay);
    void calculateTdays(int roverSpeed);
    void calculateFday();
};
ostream& operator<<(ostream& out, Mission* M);