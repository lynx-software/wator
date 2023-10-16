#pragma once

#include <string>

class CreatureT {
    public:
        CreatureT() = default;
        virtual ~CreatureT();
        char GetSymbol();
        virtual int GetBreed() = 0;
        bool HasMoved();
        virtual bool CanBreed() = 0;
        void SetMovedThisTurn(bool moved);
        std::string GetCreatureType();
        void Age();

    protected:
        char symbol;
        bool movedThisTurn;
        std::string creatureType;
        int ageSinceLastBreed;
};