#ifndef graphics_h
#define graphics_h

#include <fstream>
#include <string>
#include <map>
#include <optional>
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

        sf::Texture tlo_menu_tekstura;
        sf::Sprite tlo_menu_sprite{tlo_menu_tekstura};

        sf::Texture tlo_gra_tekstura;
        sf::Sprite tlo_gra_sprite{tlo_gra_tekstura};

        //Ikonki
        sf::Texture tekstura_ikonek;
        map <string,sf::IntRect> ikonki;
        //Siatka budynków
        BuildingsGrid siatka;
        TypEkranu ekran;
        //Czcionki
        ImFont* fontDefault = nullptr;
        ImFont* fontHUD = nullptr;
        ImFont* fontMENU = nullptr;
        //Kursory
        std::optional<sf::Cursor> kursorDomyslny;
        std::optional<sf::Cursor> kursorBudowanie;
        std::optional<sf::Cursor> kursorBurzenie;
        //Wyswietlanie itp
        bool czyhelp;
        bool czyBudynki;
        bool czyBudowa;
        bool czyBudowanie;
        bool czyBudowanieCategory;
        bool czyBudowanieWyniki;
        bool czyNextRound;
        bool czyNextRound1;
        bool czyWyburzanie;
        bool czyWyburzanie1;
        bool czyWyburzanieKlick;
        bool czyPotwierdzicWyjscie;
        bool czyDoMenu;

        bool czyBudynekInfo;
        
        //Zamieszanie z budowaniem!
        BuildingInfo trzymanyBudynek;

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
        void wychodzenie();

        void ustawStylUI();
        void UIBegin(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych, Game& gra);
        void prntGRA(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych, Game& gra);
        void prntMenuGlowne(Game& gra,const Colony &kolonia);
        void prntMenuGra(Game& gra);
        void prntUstawienia(Game& gra);
        void prntCredits();
        void prntLoad(Game& gra,const Colony &kolonia);
        void prntBladBudowanie();
        void prntBudynekInfo( const map<string, BuildingInfo>& bazaDanych,const Colony &kolonia,pair<string,int> inf);
        void prntMenuTlo(const map<string, BuildingInfo>& bazaDanych);
        bool getCzyBudowa();
        BuildingInfo getTrzymanyBudynek();

        void wczytajIkonki();
        sf::Sprite getIkonka(const std::string& nazwa);
    };

#endif
