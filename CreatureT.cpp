#include <iostream>

#include "CreatureT.h"

const bool DEBUG_MODE = false;

using namespace std;

CreatureT::~CreatureT() {}

bool CreatureT::HasMoved() {
    return movedThisTurn;
}

char CreatureT::GetSymbol() {
    if (DEBUG_MODE) {
        cout << "called GetSymbol" << endl;
    }

    return symbol;
}

void CreatureT::SetMovedThisTurn(bool moved) {
    movedThisTurn = moved;
}

// use dynamic cast instead, dingus
string CreatureT::GetCreatureType() {
    if (DEBUG_MODE) {
        cout << "called GetCreatureType" << endl;
    }
    return creatureType;
}

void CreatureT::Age() {
    ageSinceLastBreed++;
}