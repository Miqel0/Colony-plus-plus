#ifndef buildingsgrid_h
#define buildingsgrid_h

#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "colony.h"

using namespace std;

class Game; 

class BuildingsGrid{
    private:
        vector<vector<TypKafelka>> siatka;
        sf::Texture kafelek_tex;
        float kafelek_x;
        float kafelek_y;
        int siatka_size_x;
        int siatka_size_y;
        float siatka_x;
        float siatka_y;
        float siatka_begin_x;
        float siatka_begin_y;

    public:

        BuildingsGrid();
        ~BuildingsGrid(){};
        void prntSiatka(sf::RenderWindow& window);
    };
#endif
