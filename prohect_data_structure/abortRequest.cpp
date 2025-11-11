#include "abortRequest.h"

abortRequest::abortRequest(int xday, int id) :Request(id), XD(xday)
{
}

void abortRequest::Operate(MarsStation* station)
{
}
ostream& abortRequest::print(ostream& out)
{
	return out << "[X," << XD << ",M" << getMissionID() << "]";
}
int abortRequest::getXD() const
{
	return XD;
}
