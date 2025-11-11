#include <iostream>
#include "MarsStation.h"
using namespace std;

int main() {
	MarsStation* station= new MarsStation;
	station->readInputFile("input.txt");
	station->simulate();
    return 0;
}
