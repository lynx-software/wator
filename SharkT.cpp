#include "SharkT.h"

#include <iostream>

using namespace std;

const bool DEBUG_MODE = false;

SharkT::SharkT (int breed, int starve) {
    lastMeal = 0;
    symbol = 'S';
    sharkBreed = breed;
    sharkStarve = starve;
    movedThisTurn = false;
    creatureType = "shark";
    ageSinceLastBreed = 0;
}

bool SharkT::HasStarved() {
    bool result = false;

    if (lastMeal == sharkStarve) {
        result = true;
    }

    return result;
}

void SharkT::IncrementLastMeal() {
    lastMeal++;
    if (DEBUG_MODE) {
        cout << "called IncrementLastMeal, lastMeal = " << lastMeal << endl;
    }
}

void SharkT::ResetLastMeal() {
    if (DEBUG_MODE) {
        cout << "called ResetLastMeal" << endl;
    }
    lastMeal = 0;
}

bool SharkT::CanBreed() {
    bool result = false;

    if (DEBUG_MODE) {
        cout << '\n' << "age since last breed = " << ageSinceLastBreed << '\n' << endl;
    }

    if (ageSinceLastBreed == sharkBreed) {
        result = true;
        ageSinceLastBreed = 0;
    }

    return result;
}

int SharkT::GetBreed() {
    return sharkBreed;
}

int SharkT::GetStarve() {
    return sharkStarve;
}