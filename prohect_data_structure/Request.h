#pragma once
#include <iostream>
using namespace std;
class MarsStation;
class Request {
private:
    const int missionID;
public:
    // Constructor
    Request(int id);
    // Getters
    int getMissionID() const;
    virtual ostream& print(ostream& out) = 0;
	virtual void Operate(MarsStation* station) = 0;
};
ostream& operator<<(ostream& out, Request* R);