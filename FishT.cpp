#include "FishT.h"

using namespace std;

FishT::FishT (int breed) {
    symbol = 'f';
    fishBreed = breed;
    movedThisTurn = false;
    creatureType = "fish";
    ageSinceLastBreed = 0;
}

bool FishT::CanBreed() {
    bool result = false;

    if (ageSinceLastBreed == fishBreed) {
        result = true;
        ageSinceLastBreed = 0;
    }

    return result;
}

int FishT::GetBreed() {
    return fishBreed;
}