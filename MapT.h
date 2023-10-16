#pragma once

#include <vector>
#include <memory>

#include "CreatureT.h"
#include "FishT.h"
#include "SharkT.h"
#include "CoordinateT.h"

typedef std::shared_ptr<CreatureT> CreaturePtr;

class MapT {
    public:
        MapT();
        int GetHeight();
        int GetWidth();
        int GetTurn();
        int GetFishCount();
        int GetSharkCount();
        char GetCreatureSymbol(CoordinateT location);
        void IncrementTurn();
        void IncrementFishCount();
        void DecrementFishCount();
        void IncrementSharkCount();
        void DecrementSharkCount();
        void LoopThroughCreatures();
        CoordinateT RandomEmptySpace(CoordinateT creature);
        CoordinateT RandomFishSpace(CoordinateT creature);
        void MoveCreature(CoordinateT original, CoordinateT destination);
        CoordinateT Destination(CoordinateT creature);
        void RemoveCreature(CoordinateT creature);
        void ReadFromFile (int & fishBreed,
                int & sharkStarve, int & sharkBreed);
        void ResetAllCreatures();

    private:
        int turn;
        int worldWidth;
        int worldHeight;
        int fishCount;
        int sharkCount;
        std::vector<std::vector <CreaturePtr> > creatures;
};