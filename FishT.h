#pragma once

#include "CreatureT.h"

class FishT : public CreatureT {
    public:
        FishT (int breed);
        bool CanBreed();
        int GetBreed();

    protected:
        int fishBreed;
};