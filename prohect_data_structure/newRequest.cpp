#include "newRequest.h"
#include "MarsStation.h"
#include "Mission.h"

newRequest::newRequest(int rday, int id, int tloc, int mdur, char mType) 
    : Request(id), Rday(rday), TLOC(tloc), MDUR(mdur), missionType(mType)
{
}

void newRequest::Operate(MarsStation* station)
{
    MissionType mType;
    if(missionType == 'N')
        mType = NM;
    else if(missionType == 'P')
        mType = PM;
    else
		mType = DM;
    Mission* m = new Mission(getMissionID(), mType, Rday, TLOC, MDUR);
    station->insertMission(m);
}

ostream& newRequest::print(ostream& out)
{
	return out << "[R," << missionType << "," << Rday << ",M" << getMissionID() <<"]";
}

int newRequest::getTLOC() const {
    return TLOC;
}

int newRequest::getMDUR() const {
    return MDUR;
}

char newRequest::getMissionType() const {
    return missionType;
}

int newRequest::getRday() const {
    return Rday;
}
