#pragma once

#include "MapT.h"
#include "DisplayT.h"

class ControllerT {
    public:
        void StartGame();
        void OneChrononPasses();

    private:
        MapT cMap;
        DisplayT display;
};