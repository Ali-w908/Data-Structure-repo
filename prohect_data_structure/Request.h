#pragma once

enum RequestType { NEW_MISSION='R', ABORT_MISSION='X' };
static int NOaborted = 0;
class Request {
private:
    RequestType type;
    char missionType; // 'N', 'P', 'D' (فقط في NEW_MISSION)
    int Rday;
    int missionID;
    int TLOC; // TARGET LOCATION
    int MDUR; //  MISSION DURATION
    int XD;  // DAY A MISSION SHOULD BE ABORTED
  
public:
    // Constructor for new mission
    Request(RequestType t, char mType, int rday, int id, int tloc, int mdur);

    // Constructor for abort mission
    Request(RequestType t, int  xd, int id);

    // Getters
    RequestType getType() const;
    char getMissionType() const;
    int getRday() const;
    int getMissionID() const;
    int getTLOC() const;
    int getMDUR() const;
    int getXD() const;
    int getNO_of_aborted() const;
};