#include <iostream>
#include "DisplayT.h"

using namespace std;

const bool DEBUG_MODE = false;

void DisplayT::PrintTurn(MapT & map) {
    if (DEBUG_MODE) {
        cout << "called PrintTurn" << endl;
    }

    int i;

    PrintHeader(map);
    PrintWidthLabel(map);

    for (i = 0; i < map.GetHeight(); i++) {
        PrintMapRow(map, i);
    }

    PrintPressEnter();
}

void DisplayT::PrintHeader(MapT & map) {
    if (DEBUG_MODE) {
        cout << "called PrintHeader" << endl;
    }

    cout << "Turn " << map.GetTurn() << endl;

    cout << "    Fish (f): " <<
            map.GetFishCount() << endl;
    cout << "    Sharks (S): " <<
            map.GetSharkCount() << endl;
}

void DisplayT::PrintWidthLabel(MapT & map) {
    if (DEBUG_MODE) {
        cout << "called PrintWidthLabel" << endl;
    }

    int i;

    cout << "  ";

    for (i = 0; i < map.GetWidth(); i++) {
        cout << i << " ";
    }

    cout << endl;
}

void DisplayT::PrintMapRow(MapT & map, int row) {
    if (DEBUG_MODE) {
        cout << "called PrintMapRow" << endl;
    }

    CoordinateT location;
    int i;

    cout << row << " ";

    for (i = 0; i < map.GetWidth(); i++) {
        if (DEBUG_MODE) {
            cout << "called i loop" << i << endl;
            cout << "row = " << row << endl;
        }
        location.x = i;
        location.y = row;
        cout << map.GetCreatureSymbol(location) << " ";
    }

    cout << endl;
}

void DisplayT::PrintPressEnter() {
    if (DEBUG_MODE) {
        cout << "called PrintPressEnter" << endl;
    }

    string trash;

    cout << "press enter to continue: ";
    getline(cin, trash);
}