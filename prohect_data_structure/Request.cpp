#include "Request.h"

// Constructor for new mission
Request::Request(RequestType t, char mType, int rday, int id, int tloc, int mdur)
    : type(t), missionType(mType), Rday(rday), missionID(id), TLOC(tloc), MDUR(mdur), XD(-1) {
}

// Constructor for abort mission
Request::Request(RequestType t, int xd, int id)
    : type(t), missionID(id) , XD(xd) {
    NOaborted++;
}


// Getters
RequestType Request::getType() const { return type; }
char Request::getMissionType() const { return missionType; }
int Request::getRday() const { return Rday; }
int Request::getMissionID() const { return missionID; }
int Request::getTLOC() const { return TLOC; }
int Request::getMDUR() const { return MDUR; }
int Request::getXD() const { return XD; }
int Request::getNO_of_aborted() const{return NOaborted ;}
   