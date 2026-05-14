#ifndef graphics_h
#define graphics_h

#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "colony.h"

using namespace std;



class Game; 
//Żeby móc korzystać z funkcji klasy Game (i wpływać na różne rzeczy)

/**
 * @brief Klasa odpowiadająca za wyświetlaniem wszystkich rzeczy.
 * 
 */
class Graphics{
    private:
        sf::VideoMode screenSize;
        sf::RenderWindow window;
        unsigned int szer;
        unsigned int wys;
        
        //Wyswietlanie itp
        bool czyhelp;
        bool czyBudynki;
        bool czyBudowanie;
        bool czyBudowanieCategory;
        string wybranaKategoriaBudowa;
    public:

        Graphics();
        Graphics(unsigned int szer_,unsigned int wys_);
        ~Graphics(){};

        void prntAll(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych, Game& gra);
        void prntMenu();
        void prntStatystyki(const Colony& kolonia);
        void prntPomoc();
        void prntBudynki(const Colony& kolonia, const map<string, BuildingInfo>& bazaDanych);
        void prntBudowanie(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych,Game& gra);
        void prntBuildCategory(const string& cat,const Colony& kolonia, const map<string, BuildingInfo>& bazaDanych,Game& gra);
};

#endif
