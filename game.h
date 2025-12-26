#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace std;
#include "building.h"
#include "colony.h"

#ifndef game_h
#define game_h

class Game{
    Colony kolonia;
    bool running;


    public:
    Game();
    void run();
    void commands();


};

#endif
