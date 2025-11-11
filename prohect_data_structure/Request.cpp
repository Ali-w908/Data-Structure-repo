#include "Request.h"

// Constructor for new mission

Request::Request(int id)
    :missionID(id) {
}

// Getters
int Request::getMissionID() const { return missionID; }

//Request::Request(RequestType t, char mType, int rday, int id, int tloc, int mdur)
//    : type(t), missionType(mType), Rday(rday), missionID(id), TLOC(tloc), MDUR(mdur), XD(-1) {
//}
ostream& operator<<(ostream& out, Request* R)
{
	return R->print(out);
}