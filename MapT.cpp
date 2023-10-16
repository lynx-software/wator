#include <iostream>
#include <string>
#include <stdlib.h> // for rng
#include <fstream>
#include <memory>

#include "MapT.h"

using namespace std;

const bool DEBUG_MODE = false;

MapT::MapT() {
    int i;
    int j;
    int fishBreed;
    int sharkStarve;
    int sharkBreed;
    bool placed;
    vector<CreaturePtr> v;
    int randX;
    int randY;

    srand(static_cast<unsigned int>(time(nullptr)));

    turn = 0;

    ReadFromFile(fishBreed, sharkStarve, sharkBreed);

    // make world
    // if i have time during winter maybe make this a dynamic array?
    for (i = 0; i < worldWidth; i++) {
        v.clear();
        for (j = 0; j < worldHeight; j++) {
            v.push_back(nullptr);
        }
        creatures.push_back(v);
    }

    // use templates for these
    // place fish
    for (i = 0; i < fishCount; i++) {
        // for each fish to be placed
        auto newFishPtr = make_shared<FishT>(fishBreed);

        randX = rand() % worldWidth;
        randY = rand() % worldHeight;

        // loop until a valid location is found for the new fish
        placed = false;
        while (!placed) {
            if (creatures[randX][randY] == nullptr) {
                creatures[randX][randY] = newFishPtr;
                placed = true;
            } else {
                // if randomly generated location already
                // has a creature, generate a new location
                randX = rand() % worldWidth;
                randY = rand() % worldHeight;
            }
        }
    }

    // place sharks
    for (i = 0; i < sharkCount; i++) {
        auto newSharkPtr = make_shared<SharkT>(sharkBreed, sharkStarve);

        randX = rand() % worldWidth;
        randY = rand() % worldHeight;

        placed = false;
        while (!placed) {
            if (creatures[randX][randY] == nullptr) {
                creatures[randX][randY] = newSharkPtr;
                placed = true;
            } else {
                randX = rand() % worldWidth;
                randY = rand() % worldHeight; 
            }
        }
    }
}

int MapT::GetWidth() {
    return worldWidth;
}

int MapT::GetHeight() {
    return worldHeight;
}

int MapT::GetTurn() {
    return turn;
}

int MapT::GetFishCount() {
    return fishCount;
}

int MapT::GetSharkCount() {
    return sharkCount;
}

// change this to take a CoordinateT
char MapT::GetCreatureSymbol(CoordinateT location) {
    char result = '.';

    if (creatures[location.x][location.y] != nullptr) {
        result = creatures[location.x][location.y]->GetSymbol();
    }

    return result;
}

void MapT::IncrementTurn() {
    turn++;
}
void MapT::IncrementFishCount() {
    fishCount++;
}
void MapT::DecrementFishCount() {
    fishCount--;
}
void MapT::IncrementSharkCount() {
    sharkCount++;
}
void MapT::DecrementSharkCount() {
    sharkCount--;
}

void MapT::LoopThroughCreatures() {
    int x;
    int y;
    CoordinateT destinationSpace;
    CoordinateT originalSpace;
    bool moveAvailable;

    // loop through 2d vector of creatures
    // and let each creature move
    for (x = 0; x < static_cast<int>(creatures.vector::size()); x++) {
        for (y = 0; y < static_cast<int>(creatures[x].vector::size()); y++) {
            /*if (DEBUG_MODE) {
                cout << "looping through creatures: " << x << ", " << y << endl;
            }*/
            // check if creature can take a turn
            if (creatures[x][y] != nullptr and !(creatures[x][y]->HasMoved())) {

                originalSpace.x = x;
                originalSpace.y = y;

                // decide movement type
                destinationSpace = Destination(originalSpace);

                // make sure creature is still alive to take a turn
                // maybe check for starvation after taking a turn instead of before?
                if (creatures[x][y] != nullptr) {
                    creatures[x][y]->Age();
                    creatures[x][y]->SetMovedThisTurn(true);

                    // check if there is a valid move
                    if (destinationSpace.x == -1 or destinationSpace.y == -1) {
                        moveAvailable = false;
                    } else {
                        moveAvailable = true;
                    }

                    if (DEBUG_MODE) {
                        cout << "moveAvailable: " << moveAvailable << endl;
                        cout << "moving creature to (" << destinationSpace.x << ", " << destinationSpace.y <<
                            ") from (" << originalSpace.x << ", " << originalSpace.y << ")" << endl;
                    }

                    if (moveAvailable) {
                        MoveCreature(originalSpace, destinationSpace);
                    }/*

                    if (creatures[destinationSpace.x][destinationSpace.y]->GetCreatureType() == "shark") {
                        // check if shark has starved
                        sharkPtr = dynamic_pointer_cast<SharkT>(creatures[destinationSpace.x][destinationSpace.y]);
                        if (sharkPtr -> HasStarved()) {
                            RemoveCreature(destinationSpace);
                        }
                    }*/
                }
            }
        }
    }
    
    ResetAllCreatures();
}

CoordinateT MapT::Destination(CoordinateT creature) {
    CoordinateT destinationSpace;
    CoordinateT originalSpace;
    shared_ptr<SharkT> sharkPtr;

    if (DEBUG_MODE) {
        cout << "called Destination" << endl;
    }

    originalSpace.x = creature.x;
    originalSpace.y = creature.y;

    if (creatures[creature.x][creature.y]->GetCreatureType() == "shark") {
        // check if shark has starved
        sharkPtr = dynamic_pointer_cast<SharkT>(creatures[creature.x][creature.y]);
        if (sharkPtr -> HasStarved()) {
            RemoveCreature(originalSpace);
        } else {
            destinationSpace = RandomFishSpace(originalSpace);
            // if there is no nearby fish space
            if (destinationSpace.x == -1 or destinationSpace.y == -1) {
                destinationSpace = RandomEmptySpace(originalSpace);
                sharkPtr -> IncrementLastMeal();
            } else {
                // a valid fish space was found
                if (DEBUG_MODE) {
                    cout << "ate a fish" << endl;
                }
                RemoveCreature(destinationSpace);
                sharkPtr -> ResetLastMeal();
            }
        }
        if (DEBUG_MODE) {
            cout << "shark's destination: " << destinationSpace.x << ", " << destinationSpace.y << endl;
        }
    } else {
        // movement for fish
        destinationSpace = RandomEmptySpace(originalSpace);
    }

    return destinationSpace;
}

CoordinateT MapT::RandomEmptySpace(CoordinateT currentPosition) {
    CoordinateT result;
    vector <CoordinateT> emptySpaces;
    CoordinateT space;
    int x;
    int y;
    int randNum;

    // look at surrounding positions
    // check x - 1, y
    x = currentPosition.x - 1;
    y = currentPosition.y;

    if (x < 0) {
        x = worldWidth - 1;
    }

    if (creatures[x][y] == nullptr) {
        space.x = x;
        space.y = y;
        emptySpaces.push_back(space);
    }

    // check x, y - 1
    x = currentPosition.x;
    y = currentPosition.y - 1;

    if (y < 0) {
        y = worldHeight - 1;
    }

    if (creatures[x][y] == nullptr) {
        space.x = x;
        space.y = y;
        emptySpaces.push_back(space);
    }

    // check x + 1, y
    x = currentPosition.x + 1;
    y = currentPosition.y;

    if (x >= worldWidth) {
        x = 0;
    }

    if (creatures[x][y] == nullptr) {
        space.x = x;
        space.y = y;
        emptySpaces.push_back(space);
    }

    // check x, y + 1
    x = currentPosition.x;
    y = currentPosition.y + 1;

    if (y >= worldHeight) {
        y = 0;
    }

    if (creatures[x][y] == nullptr) {
        space.x = x;
        space.y = y;
        emptySpaces.push_back(space);
    }

    // generate random empty space
    if (emptySpaces.vector::size() > 0) {
        randNum = rand() % static_cast<int>(emptySpaces.vector::size());
        result = emptySpaces[randNum];
    } else {
        // return if no empty spaces are available
        result.x = -1;
        result.y = -1;
    }

    return result;
}

CoordinateT MapT::RandomFishSpace(CoordinateT currentPosition) {
    CoordinateT result;
    CoordinateT space;
    vector <CoordinateT> fishSpaces;
    int x;
    int y;
    int randNum;

    if (DEBUG_MODE) {
        cout << "called RandomFishSpace with position "
            << currentPosition.x << ", " << currentPosition.y << endl;
    }

    // check x - 1, y
    x = currentPosition.x - 1;
    y = currentPosition.y;

    if (x < 0) {
        x = worldWidth - 1;
    }

    if (creatures[x][y] != nullptr and
            creatures[x][y]->GetCreatureType() == "fish") {
        space.x = x;
        space.y = y;
        fishSpaces.push_back(space);
    }

    // check x, y - 1
    x = currentPosition.x;
    y = currentPosition.y - 1;

    if (y < 0) {
        y = worldHeight - 1;
    }

    if (creatures[x][y] != nullptr and
            creatures[x][y]->GetCreatureType() == "fish") {
        space.x = x;
        space.y = y;
        fishSpaces.push_back(space);
    }

    // check x + 1, y
    x = currentPosition.x + 1;
    y = currentPosition.y;

    if (x >= worldWidth) {
        x = 0;
    }

    if (creatures[x][y] != nullptr and
            creatures[x][y]->GetCreatureType() == "fish") {
        space.x = x;
        space.y = y;
        fishSpaces.push_back(space);
    }

    // check x, y + 1
    x = currentPosition.x;
    y = currentPosition.y + 1;

    if (y >= worldHeight) {
        y = 0;
    }

    if (creatures[x][y] != nullptr and
            creatures[x][y]->GetCreatureType() == "fish") {
        space.x = x;
        space.y = y;
        fishSpaces.push_back(space);
    }

    // generate random fish space
    if (fishSpaces.vector::size() > 0) {
        if (DEBUG_MODE) {
            long unsigned int i;
            cout << "fish spaces: ";
            for (i = 0; i < fishSpaces.vector::size(); i++) {
                cout << fishSpaces[i].x << ", " << fishSpaces[i].y << " | ";
            }
            cout << endl;
        }
        randNum = rand() % static_cast<int>(fishSpaces.vector::size());
        result = fishSpaces[randNum];
    } else {
        // return if no fish spaces are available
        result.x = -1;
        result.y = -1;
    }

    return result;
}

// should this be a bool that returns false if move isn't valid?
void MapT::MoveCreature(CoordinateT original, CoordinateT destination) {
    shared_ptr<SharkT> sharkPtr;
    shared_ptr<FishT> fishPtr;

    if (DEBUG_MODE) {
        cout << "called MoveCreature, og: " << original.x << ", " << original.y << endl;
        cout << "destination: " << destination.x << ", " << destination.y << endl;
    }

    bool validMove = true;

    if ((destination.x < 0 or destination.y < 0) or
                (destination.x >= worldWidth or destination.y >= worldHeight)) {
        validMove = false;
    }
//creatures[destination.x][destination.y] == nullptr and 
    if (validMove) {
        creatures[destination.x][destination.y] = creatures[original.x][original.y];
        creatures[original.x][original.y] = nullptr;

        // check if creature can breed
        if (creatures[destination.x][destination.y]->GetCreatureType() == "shark") {
            sharkPtr = dynamic_pointer_cast<SharkT>(creatures[destination.x][destination.y]);
            if (sharkPtr->CanBreed()) {
                if (DEBUG_MODE) {
                    cout << "shark at " << destination.x << ", " << destination.y << " can breed at "
                        << original.x << ", " << original.y << endl;
                }
                // make a shark
                auto newSharkPtr = make_shared<SharkT>(sharkPtr->GetBreed(), sharkPtr->GetStarve());
                // put the new shark where the old shark used to be
                creatures[original.x][original.y] = newSharkPtr;
                if (DEBUG_MODE) {
                    cout << "shark placed at " << original.x << ", " << original.y << endl;
                }
                IncrementSharkCount();
            }
        } else if (creatures[destination.x][destination.y]->GetCreatureType() == "fish") {
            fishPtr = dynamic_pointer_cast<FishT>(creatures[destination.x][destination.y]);
            if (fishPtr->CanBreed()) {
                // make a new fish
                auto newFishPtr = make_shared<FishT>(fishPtr->GetBreed());
                // put the new fish where the old fish used to be
                creatures[original.x][original.y] = newFishPtr;
                if (DEBUG_MODE) {
                    cout << "fish placed at " << original.x << ", " << original.y << endl;
                }
                IncrementFishCount();
            }
        }
    }
}

void MapT::RemoveCreature(CoordinateT creature) {
    if (DEBUG_MODE) {
        cout << "RemoveCreature called" << endl;
    }

    if (creatures[creature.x][creature.y] != nullptr) {
        if (creatures[creature.x][creature.y]->GetCreatureType() == "fish") {
            DecrementFishCount();
        } else if (creatures[creature.x][creature.y]->GetCreatureType() == "shark") {
            DecrementSharkCount();
        }
    }

    creatures[creature.x][creature.y] = nullptr;
}

void MapT::ReadFromFile(int & fishBreed,
        int & sharkStarve, int & sharkBreed) {
    ifstream inFile;
    inFile.open("wator.init");

    inFile >> worldWidth;
    inFile >> worldHeight;
    inFile >> fishCount;
    inFile >> fishBreed;
    inFile >> sharkCount;
    inFile >> sharkStarve;
    inFile >> sharkBreed;

    inFile.close();
}

void MapT::ResetAllCreatures() {
    int x;
    int y;

    for (x = 0; x < static_cast<int>(creatures.vector::size()); x++) {
        for (y = 0; y < static_cast<int>(creatures.vector::size()); y++) {
            if (creatures[x][y] != nullptr) {
                creatures[x][y]->SetMovedThisTurn(false);
            }
        }
    }
}