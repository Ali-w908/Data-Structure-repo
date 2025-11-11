#pragma once
using namespace std;
class MarsStation;
enum UIState { INTERACTIVE, SILENT };
class UI
{

	UIState mode;				// Interactive or Silent
	MarsStation* station;

public:

	UI(MarsStation* s);

	UIState getMode();

	void printInterface();

	void printRequests();

};
