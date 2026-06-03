#ifndef graphics_h
#define graphics_h

#include <fstream>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "colony.h"
#include "buildingsGrid.h"
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

        //Siatka budynków
        BuildingsGrid siatka;
        TypEkranu ekran;
        //Czcionki
        ImFont* fontDefault = nullptr;
        ImFont* fontHUD = nullptr;
        ImFont* fontMENU = nullptr;

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
        bool czyWyburzanieKlick;

        bool czyBudynekInfo;
        
        //Zamieszanie z budowaniem!
        BuildingInfo trzymanyBudynek;
        bool czyBudowa;

        bool czyLoad;
        bool czySave;
        
        bool czyGra;

        pair <string,int> kafelek;
        string wybranaKategoriaBudowa;
        BuildResult ostatniWynik;
        NextResult nextWynik;
        DestroyResult destroyWynik;

        float czasWyswietlaniaBledu ;
        sf::Clock zegarBledu;
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
        void prntMenuGlowne(Game& gra,const Colony &kolonia);
        void prntMenuGra(Game& gra);
        void prntUstawienia(Game& gra);
        void prntCredits();
        void prntLoad(Game& gra,const Colony &kolonia);
        void prntBladBudowanie();
        void prntBudynekInfo( const map<string, BuildingInfo>& bazaDanych,const Colony &kolonia,pair<string,int> inf);

        bool getCzyBudowa();
        BuildingInfo getTrzymanyBudynek();
    };

#endif
