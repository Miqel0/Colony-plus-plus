#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <cctype>
#include <iomanip>
#include <vector>
#include <filesystem>

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

Game::Game():ostatni_zapis(""),czyFirst(0){
    //Wczytywanie dostępnych zapisów
    zapisy=pobierzZapisy();
    //Wczytywanie parametrow budynkow
    loadGameData();
    //Wczytywanie ustawień itp
    loadConfig();
}

// ==========================================
// GRA
// ==========================================

/**
 * @brief Funkcja wczytująca grę z plików oraz zaczynająca pętlę graficzną 
 * 
 */
void Game::UIrun(){
    grafika.UIBegin(kolonia, bazaDanych,*this);
}


void Game::load(const string& nazwa_zapisu ){
    kolonia.load(nazwa_zapisu);
    ostatni_zapis=nazwa_zapisu;
    saveConfig();
    
}

void Game::save(const string& nazwa_zapisu ){
    kolonia.save(nazwa_zapisu);
    saveConfig();
}

/*FIXME
Do dodania z Commands:
- cheaty (kolonia.setSandbox)
- też dodać tutorial - chyba jednak nie xD
*/

vector<string>& Game::getZapisy(){
    return zapisy;
}


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
 * @brief Zaktualizowanie listy zapisów
 * 
 */
void Game::setZapisy(){
    zapisy=pobierzZapisy();
}
/**
 * @brief Wczytywanie dostępnych zapisów z folderu z zapisami
 * 
 * @return vector<string> tablica z nazwami foledrów - czyli zapisów gry
 */
vector<string> Game::pobierzZapisy() {
    std::vector<std::string> znalezioneZapisy;
    filesystem::path sciezkaSaves = "data/saves";

    if (filesystem::exists(sciezkaSaves) && filesystem::is_directory(sciezkaSaves)) {
        for (const auto& entry : filesystem::directory_iterator(sciezkaSaves)) {
            if (entry.is_directory()) {
                znalezioneZapisy.push_back(entry.path().filename().string());
                //cout<<entry.path().filename().string()<<endl;
            }
        }
    }
    return znalezioneZapisy;
}

/**
 * @brief Jakaś funkcja do wczytywania ustawień itp.
 * 
 */
void Game::loadConfig(){ //Sprawdzanie config
    ifstream plik("data/config.txt");
    string plikk;
    int czyFirst_;
    if (!plik.is_open()) {
        cout << "BLAD: Nie mozna otworzyc config.txt!" << endl;
    }else{
        plik>>plikk>>czyFirst_;
        plik.close();
        ostatni_zapis=plikk;
        czyFirst=czyFirst_;
    }    
}

/**
 * @brief Zapisywanie ustawień do pliku config.txt
 * 
 */
void Game::saveConfig(){ //Zapisywanie config
    ofstream plik("data/config.txt");
    if(plik.is_open()){
        plik<<ostatni_zapis<<" "<<czyFirst;
        plik.close();
        //cout<<"zapisano config z: "<<ostatni_zapis<<endl;
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
    int X,Y;

    while(plik>>n>>type>>kK>>kT>>kE>>w>>gk>>gt>>gi>>x>>lt>>X>>Y>>opis){
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
        nowy.X=X;
        nowy.Y=Y;
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
 * @brief Przekazanie ID do zburzenia budynku.
 * 
 * @param ID  budynku do zburzenia.
 * 
 * @return DestroyResult paczka wynikowa.
 */
DestroyResult Game::UIZburzID(int ID){return kolonia.UIzburzBudynekID(ID);}

/**
 * @brief Funckja budujaca budynek
 * 
 * @param info paczka z infromacja o danym budynku
 * @return BuildResult 
 */
BuildResult Game::UIbuild(BuildingInfo info,pair<int,int> kliknietePole) {
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

    unique_ptr<Building> nowyBudynek = fabryka.stworzBudynek(info,kliknietePole);

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

/**
 * @brief Resetowanie reczy pod nową grę.
 * 
 */
void Game::resetToDefault(){
    kolonia.resetToDefault();
}

/**
 * @brief Zwracanie nazwy ostatniego zapisu.
 * 
 * @return string 
 */
string Game::getOstatniZapis() const{return ostatni_zapis;}
void Game::setOstatniZapis(string naz){ostatni_zapis=naz;}
int Game::getCzyFirst() const{return czyFirst;}
void Game::setCzyFirst(){czyFirst=1;}


void Game::setNazwa(string nazwa){kolonia.setNazwa(nazwa);}