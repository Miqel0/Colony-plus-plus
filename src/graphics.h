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
        
        TypEkranu ekran;
        //Czcionki
        ImFont* fontDefault = nullptr;
        ImFont* fontHUD = nullptr;

        //Wyswietlanie itp
        bool czyhelp;
        bool czyBudynki;
        bool czyBudowanie;
        bool czyBudowanieCategory;
        bool czyBudowanieWyniki;
        bool czyNextRound;
        bool czyNextRound1;
        bool czyWyburzanie;
        bool czyWyburzanie1;
        
        bool czyGra;

        string wybranaKategoriaBudowa;
        BuildResult ostatniWynik;
        NextResult nextWynik;
        DestroyResult destroyWynik;
    public:

        Graphics();
        Graphics(unsigned int szer_,unsigned int wys_);
        ~Graphics(){};

        void prntAll(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych, Game& gra);
        void prntMenu();
        void prntStatystyki(const Colony& kolonia, const map<string, BuildingInfo>& bazaDanych);
        void prntStatystykiToolTop(const Colony& kolonia, map<string,int>& licznik, string cat,const map<string, BuildingInfo>& bazaDanych);
        void prntPomoc();
        void prntBudynki(const Colony& kolonia, const map<string, BuildingInfo>& bazaDanych,Game& gra);
        void prntBudowanie(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych,Game& gra);
        void prntBudowanieWyniki(Game& gra);
        void prntBuildCategory(const string& cat,const Colony& kolonia, const map<string, BuildingInfo>& bazaDanych,Game& gra);
        void prntNextRoundButton();
        void prntCzyNextRound(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych,Game& gra);
        void prntNextRound(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych,Game& gra);
        void prntWyburz();

        void UIBegin(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych, Game& gra);
        void prntGRA(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych, Game& gra);
        void prntMenuGlowne(Game& gra);
        void prntMenuGra(Game& gra);
        void prntUstawienia();
        void prntCredits();
    };

#endif
