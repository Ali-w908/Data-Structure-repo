#pragma once
#include "Request.h"
class MarsStation;
class newRequest : public Request {
private:
	const int TLOC; // TARGET LOCATION
    const int MDUR; //  MISSION DURATION
	const char missionType; // 'N', 'P', 'D' 
    const int Rday;
public:
	newRequest(int rday, int id, int tloc, int mdur, char mType);
	void Operate(MarsStation* station);
	ostream& print(ostream& out);
	int getTLOC() const;
	int getMDUR() const;
	char getMissionType() const;
	int getRday() const;
};