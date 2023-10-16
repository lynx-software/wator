#pragma once

#include "CreatureT.h"

class SharkT : public CreatureT {
    public:
        SharkT (int breed, int starve);
        bool HasStarved();
        void IncrementLastMeal();
        void ResetLastMeal();
        bool CanBreed();
        int GetBreed();
        int GetStarve();

    protected:
        int sharkBreed;
        int sharkStarve;
        int lastMeal;
};