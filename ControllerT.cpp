#include <ctime>
#include <iostream>

#include "ControllerT.h"

using namespace std;

const bool DEBUG_MODE = false;

void ControllerT::StartGame() {
    while (true) {
        display.PrintTurn(cMap);
        OneChrononPasses();
    }
}

void ControllerT::OneChrononPasses() {
    if (DEBUG_MODE) {
        cout << "called OneChrononPasses" << endl;
    }
    cMap.LoopThroughCreatures();
    cMap.IncrementTurn();
}