#pragma once
#include "Request.h"
class MarsStation;
class abortRequest : public Request {
private:
	const int XD;  // DAY A MISSION SHOULD BE ABORTED
public:
	abortRequest(int xday, int id);
	void Operate(MarsStation* station);
	ostream& print(ostream& out);
	int getXD() const;
};