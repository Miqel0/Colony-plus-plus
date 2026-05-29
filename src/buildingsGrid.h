#ifndef buildingsgrid_h
#define buildingsgrid_h

#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <SFML/Graphics.hpp>
#include "colony.h"
#include "utils.h"

using namespace std;

class Game; 

/**
 * @brief Siatka budynków - graficznie przedstawienie planszy gry.
 * 
 */
class BuildingsGrid{
    private:

        vector<vector<DaneKafelek>> siatka;
        map<string,sf::Texture> texturyBudynki;
        
        //puste kafelki
        sf::Texture kafelek_tex;
        sf::Texture kafelek_najechany_tex;

        float kafelek_x;
        float kafelek_y;
        int siatka_size_x;
        int siatka_size_y;
        float siatka_x;
        float siatka_y;
        float siatka_begin_x;
        float siatka_begin_y;

        pair<int,int> poz_mysz;//koordynaty na siatce

    public:

        BuildingsGrid();
        ~BuildingsGrid(){};
        void prntSiatka(sf::RenderWindow& window,ImVec2& poz);
        bool sprawdzMysz(ImVec2& poz);
        void czyNajechane(ImVec2& poz);
        bool wczytajTextury(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych);
        void wczytajSiatkaDane(const Colony& kolonia,const map<string,BuildingInfo>& bazaDanych);
    };
#endif
