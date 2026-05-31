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

        //puste kafelki - do usunieca
        map<string,sf::Texture> texturyBudynki;
        sf::Texture kafelek_tex;
        sf::Texture kafelek_najechany_tex;

        //Textury kafelkow budynków
        sf::Texture atlas_budynkow;


        int kafelek_x;
        int kafelek_y;
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
        void prntSiatka(sf::RenderWindow& window,ImVec2& poz,const map<string, BuildingInfo>& bazaDanych,bool czyBudowanie, string nazwaTrzymanego);
        bool sprawdzMysz(ImVec2& poz);
        void czyNajechane(ImVec2& poz);

        bool wczytajTextury(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych);
        void wczytajSiatkaDane(const Colony& kolonia,const map<string,BuildingInfo>& bazaDanych);
        void wczytajBudynki(const Colony& kolonia);

        int getID(pair<int,int> n) const;
        string getName(pair<int,int> n) const;
        pair<int,int> getPozMysz() const;
        TypKafelka getTypKafelka() const;
    };
#endif
