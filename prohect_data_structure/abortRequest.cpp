#include "abortRequest.h"
#include "MarsStation.h"

abortRequest::abortRequest(int xday, int id) :Request(id), XD(xday)
{
}

void abortRequest::Operate(MarsStation* station)
{
    // Abort is only for Normal Missions
    station->abortMission(getMissionID());
}
ostream& abortRequest::print(ostream& out)
{
	return out << "[X," << XD << ",M" << getMissionID() << "]";
}
int abortRequest::getXD() const
{
	return XD;
}
