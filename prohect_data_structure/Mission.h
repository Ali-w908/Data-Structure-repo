#pragma once
#include <string>

enum MissionType { NM, PM, DM }; // Normal, Polar, Drill
enum MissionStatus { WAITING, READY, IN_EXEC, IN_BACK, DONE, ABORTED };

class Mission {
private:
    int ID;
    MissionType type;
    int Rday;      // Request day
    int TLOC;      // Target location (km)
    int MDUR;      // Mission duration (days)
    int Wdays;     // Waiting days
    int Tdays;     // Total mission days
    int Fday;      // Finish day
    MissionStatus status;

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
    MissionStatus getStatus() const;

    // === Setters ===
    void setStatus(MissionStatus s);
    void setWdays(int w);
    void setTdays(int t);
    void setFday(int f);

    // === Utility ===
    void calculateWdays(int currentDay);
    void calculateTdays(int roverSpeed);
    void calculateFday();
};
