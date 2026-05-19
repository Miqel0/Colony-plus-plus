#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <cctype>
#include <iomanip>
#include <vector>

#include <algorithm>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <optional>

using namespace std;
#include "game.h"

// ==========================================
// KONSTRUKTOR 
// ==========================================

Game::Game(){
    //Wczytywanie parametrow budynkow
    loadGameData();
}

// ==========================================
// GRA
// ==========================================

/**
 * @brief Funkcja wczytująca grę z plików oraz zaczynająca pętlę graficzną 
 * 
 */
void Game::UIrun(){
    //kolonia.load();
    grafika.UIBegin(kolonia, bazaDanych,*this);
}


void Game::load(){
    kolonia.load();
}

/*FIXME
Do dodania z Commands:
- zmiana nazwy wcześniej (kolonia.setNazwa();)
- save/load (kolonia.save(), kolonia.load())
- cheaty (kolonia.setSandbox)
- też dodać tutorial
*/



/**
 * @brief Funkcja wywoływująca funkcję NextRound z Colony
 * 
 * @return NextResult pakiet wyników z NextRound
 */
NextResult Game::UINextRound(){return kolonia.UInextRound();}

// ==========================================
// RZECZY Z PLIKAMI (LOAD)
// ==========================================

/**
 * @brief Jakaś funkcja do wczytywania ustawień itp.
 * 
 * @return true idk
 * @return false idk
 */
bool Game::checkConfig(){ //Sprawdzanie config
    ifstream plik("data/config.txt");
    int czy;
    if (!plik.is_open()) {
        cout << "BLAD: Nie mozna otworzyc config.txt!" << endl;
        return false;
    }else{
        plik>>czy;
        plik.close();
    }
    if(czy==1){
        return true;
    }else if(czy==0){
        return false;
    }else{
        return false;
    }
}

/**
 * @brief Zapisywanie ustawień do pliku config.txt
 * 
 */
void Game::saveConfig(){ //Zapisywanie config
     ofstream plik("data/config.txt");
    if(plik.is_open()){
        plik<<1;
        plik.close();
        }
}

/**
 * @brief Wczytywanie parametrów budynków z pliku gamedata.txt - tylko na poczatku gry
 * 
 */
void Game::loadGameData(){ 
    ifstream plik("data/gamedata.txt");
    if (!plik.is_open()) {
        cout<< "BLAD: Nie mozna otworzyc gamedata.txt!" << endl;
        return;
    }

    string n, type, opis;
    int kK, kT, kE, gk, gt, gi; 
    int w, x,lt;

    while(plik>>n>>type>>kK>>kT>>kE>>w>>gk>>gt>>gi>>x>>lt>>opis){
        replace(opis.begin(), opis.end(), '_', ' ');

        BuildingInfo nowy;
        nowy.nazwa=n;
        nowy.type=type;
        nowy.kKamien=kK;
        nowy.kTytan=kT;
        nowy.reqEnergy=kE;
        nowy.workers=w;
        nowy.genKamien=gk;
        nowy.genTytan=gt;
        nowy.genInne=gi;
        nowy.x=x;
        nowy.lvlTerr=lt;
        nowy.opis=opis;

        string klucz =n;
        for(auto &c : klucz) c = tolower(c);

        bazaDanych[klucz]=nowy;
    }
    plik.close();
}

// ==========================================
// BUDOWANIE
// ==========================================

/**
 * @brief Przekazanie nazwy do zburzenia budynku.
 * 
 * @param nazwa nazwa budynku do zburzenia.
 * 
 * @return DestroyResult paczka wynikowa.
 */
DestroyResult Game::UIZburz(string nazwa){return kolonia.UIzburzBudynek(nazwa);}

/**
 * @brief Funckja budujaca budynek
 * 
 * @param info paczka z infromacja o danym budynku
 * @return BuildResult 
 */
BuildResult Game::UIbuild(BuildingInfo info) {
    BuildResult wynik={false,0,0,0,info.nazwa,0,false};
    if(kolonia.getRuch()==3){ //Sprawdzanie warunku z iloscia ruchow w turze
            wynik.ruch=true;
            return wynik;//Nie mozna budowac bo za duzo ruchow!
    }
    string nazwa_=info.nazwa;
    for(auto &c : nazwa_) c = tolower(c);
    if(info.lvlTerr>kolonia.getLvlTerr()){
        return wynik;
    }

    unique_ptr<Building> nowyBudynek = fabryka.stworzBudynek(info);

    if (nowyBudynek != nullptr) {
        wynik =kolonia.UIbuduj(move(nowyBudynek));
        if (wynik.czy) {
            kolonia.setRuch(kolonia.getRuch() + 1);
        }
    } 
    else {
        cout << "Blad: Nieznana mechanika budynku!"<< endl;
    }
    wynik.act_ruch=kolonia.getRuch();
    return wynik;
}

/**
 * @brief Przekazywanie danych do wyświetlenia nowych budynków przy osiagnieciu nowego poziomu
 * 
 */
vector<BuildingInfo> Game::UIprntNewLvlTerr(){
        //Sprawdzanie czy istnieje jakis budynek ktory sie odblokowal
        vector<BuildingInfo> pakiet;
        
        for(const auto&[klucz,info]:bazaDanych){
            if(info.lvlTerr==kolonia.getLvlTerr()){
                pakiet.push_back(info);
            }
        }
    return pakiet;
}