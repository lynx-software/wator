#pragma once

#include <memory>

#include "MapT.h"

class DisplayT {
    public:
        void PrintTurn(MapT & map);

    private:
        void PrintHeader(MapT & map);
		void PrintWidthLabel(MapT & map);
		void PrintMapRow(MapT & map, int row);
		void PrintPressEnter();
};