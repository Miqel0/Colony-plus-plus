#include "buildingsGrid.h"
#include "game.h"

#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include "utils.h"

#include <optional>
#include <map>
#include <vector>
#include <iostream>
#include <cfloat>
#include <cmath>


/**
 * @brief Funckja do wczytywania grafik / testur itp!
 * 
 * @tparam T 
 * @param sciezka sciezka w ""
 * @param obiektSFML jaki obiekt zdefiniowany wczesniej
 * @return true udalo sie wczytac
 * @return false nei udalo sie
 */
template <typename T>
bool WczytajGrafike(const std::string& sciezka, T& obiektSFML) {
    std::ifstream file(sciezka, std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        std::cerr << "Blad: Nie mozna otworzyc pliku: " << sciezka << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        return obiektSFML.loadFromMemory(buffer.data(), buffer.size());
    }
    
    std::cerr << "Blad: Nie udalo sie odczytac bajtow z pliku: " << sciezka << std::endl;
    return false;
}

/**
 * @brief Wczytywanie textur przy starcie gry
 * 
 * @return true udało sie wczytac wszystkie
 * @return false nie dual osei wczytac
 */
bool BuildingsGrid::wczytajTextury(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych){
    if(WczytajGrafike("assets/grafiki_siatki_nowe.png",atlas_budynkow)){
        return true;
    }else{
        cout<<"Nie udało sie wczytac grafik budynków!"<<endl;
        return false;
    }
}

void BuildingsGrid::wczytajSiatkaDane(const Colony& kolonia,const map<string,BuildingInfo>& bazaDanych){
    if(!wczytajTextury(kolonia,bazaDanych)){
        cout<<"COS nie dziala!"<<endl;
    }
}

/**
 * @brief Wczytywanie budynków na początku gry na siatkę.!
 * 
 * @param kolonia 
 */
void BuildingsGrid::wczytajBudynki(const Colony& kolonia){
    for (int x = 0; x < siatka_size_x; ++x) {
        for (int y = 0; y < siatka_size_y; ++y) {
            
            if((x % 2 != 0 && y == siatka_size_y - 1)||x == siatka_size_x - 1){
                siatka[x][y].typ = TypKafelka::BRAK;
                siatka[x][y].nazwa="brak";
            }else{
                siatka[x][y].typ = TypKafelka::PUSTY;
                siatka[x][y].nazwa="pusty";
                siatka[x][y].X=-1;
                siatka[x][y].Y=-1;
            }
        }
    }
    vector<DaneKafelek> budynki=kolonia.getBudynki();
    for(const auto b:budynki){
        siatka[b.X][b.Y]=b;
    }
}

/**
 * @brief Konstruktor siatki budynków.
 * 
 */
BuildingsGrid::BuildingsGrid():siatka_x(1400),siatka_y(700),siatka_begin_x(400),siatka_begin_y(200),kafelek_x(200),kafelek_y(100),siatka_size_x(12),siatka_size_y(7),poz_mysz(-1,-1){
    siatka.resize(siatka_size_x,vector<DaneKafelek>(siatka_size_y));

    for (int x = 0; x < siatka_size_x; ++x) {
        for (int y = 0; y < siatka_size_y; ++y) {
            
            if((x % 2 != 0 && y == siatka_size_y - 1)||x == siatka_size_x - 1){
                siatka[x][y].typ = TypKafelka::BRAK;
                siatka[x][y].nazwa="brak";
            }else{
                siatka[x][y].typ = TypKafelka::PUSTY;
                siatka[x][y].nazwa="pusty";
                siatka[x][y].X=-1;
                siatka[x][y].Y=-1;
            }
        }
    }
}
    //X=300 do 1700 = 1400 -> 200
    //Y=100 do 800 = 700 - > 100/
    //siatka inna

/**
 * @brief Funkcja tłumacząca coordynaty myszki. 
 * (zamienia na kwadraty i odległość od środka)
 * @param poz ImVec2 myszki
 */
bool BuildingsGrid::sprawdzMysz(ImVec2& poz) {
    float MyszX = poz.x;
    float MyszY = poz.y;

    float najmniejszy_dystans = FLT_MAX; 
    int znaleziony_wiersz = -1;
    int znaleziona_kolumna = -1;

    float max_dystans = (kafelek_x / 2.0f) * (kafelek_x / 2.0f);
    for (int i = 0; i < siatka_size_x; i++) {
        for (int j = 0; j < siatka_size_y; j++) {
            
            if (siatka[i][j].typ == TypKafelka::PUSTY||siatka[i][j].typ == TypKafelka::ZAJETY) {
                
                float srodek_X = 0;
                if (i % 2 == 0) {
                    srodek_X = siatka_begin_x + j * kafelek_x;
                } else {
                    srodek_X = siatka_begin_x + (j + 0.5f) * kafelek_x;
                }
                float srodek_Y = siatka_begin_y + i * (kafelek_y / 2.0f);

                float dystansX = std::abs(MyszX - srodek_X);
                float dystansY = std::abs(MyszY - srodek_Y) * (kafelek_x / kafelek_y); 
                
                float dystans = dystansX + dystansY;

                float max_dystans = kafelek_x / 2.0f;

                if (dystans <= max_dystans && dystans < najmniejszy_dystans) {
                    najmniejszy_dystans = dystans;
                    znaleziony_wiersz = i;
                    znaleziona_kolumna = j;
                }
            }
        }
    }

    if (znaleziony_wiersz != -1) {
        poz_mysz = {znaleziony_wiersz, znaleziona_kolumna};
        return true;
    }

    return false;
}

/**
 * @brief Funkcja sprawdzajaca czy najechane i zmieniająca texturę
 * 
 * @param poz 
 */
void BuildingsGrid::czyNajechane(ImVec2& poz){
    if(!sprawdzMysz(poz)){
        poz_mysz = {-1, -1};
    }
}

/**
 * @brief Funkcja wyświetlająca siatkę budynków!
 * 
 * @param window okienko
 */
void BuildingsGrid::prntSiatka(sf::RenderWindow& window,ImVec2& poz,const map<string, BuildingInfo>& bazaDanych,bool czyBudowa, string nazwaTrzymanego ){
    sf::Sprite kafelek_budynek(atlas_budynkow);
    kafelek_budynek.setOrigin(sf::Vector2f(kafelek_x / 2.0f, kafelek_y / 2.0f));
    string nazw=nazwaTrzymanego;
    for(auto &a:nazw) a =tolower(a);

    sf::Vector2i pozycjaPix = sf::Mouse::getPosition(window);
    sf::Vector2f pozycjaSwiata = window.mapPixelToCoords(pozycjaPix);
    ImVec2 Pozycja(pozycjaSwiata.x, pozycjaSwiata.y);
    int budynek_wysokosc = 200;
    czyNajechane(Pozycja);

    for(int i=0;i<siatka_size_x;i++){
        for(int j=0;j<siatka_size_y;j++){

            string nazwa_kafelka=siatka[i][j].nazwa;
            for(auto &c : nazwa_kafelka) c = tolower(c);
            if(siatka[i][j].typ==TypKafelka::BRAK){
                continue;
            }
            else{
                float X=0;
                if(i%2==0){
                    X=siatka_begin_x+j*(kafelek_x);

                }else{
                    X=(float)(siatka_begin_x+(j+0.5)*(kafelek_x));
                }
                float Y=siatka_begin_y+i*(kafelek_y/2.0f);

                //Robienie sprite do tła
                sf::Sprite sprite_ziemi(atlas_budynkow);
                sprite_ziemi.setOrigin(sf::Vector2f(kafelek_x / 2.0f, kafelek_y / 2.0f));
                sprite_ziemi.setPosition(sf::Vector2f(X, Y));
                
                int ziemia_X = 800; 
                int ziemia_Y = 100;
                
                sprite_ziemi.setTextureRect(sf::IntRect({ziemia_X, ziemia_Y}, {kafelek_x, kafelek_y}));

                if ((i == poz_mysz.first && j == poz_mysz.second) && !czyBudowa) {
                    
                    sprite_ziemi.setColor(sf::Color(200, 200, 255, 255)); 
                }

                window.draw(sprite_ziemi);

                if (siatka[i][j].typ == TypKafelka::ZAJETY) {
                    
                    auto it_tekstury = bazaDanych.find(nazwa_kafelka);
                    if (it_tekstury != bazaDanych.end()) {
                        
                        sf::Sprite sprite_budynku(atlas_budynkow);
                

                        sprite_budynku.setOrigin(sf::Vector2f(kafelek_x / 2.0f, budynek_wysokosc - (kafelek_y / 2.0f)));
                        sprite_budynku.setPosition(sf::Vector2f(X, Y));

                        int b_X = it_tekstury->second.X;
                        int b_Y = it_tekstury->second.Y;
                        
                        sprite_budynku.setTextureRect(sf::IntRect({b_X, b_Y}, {kafelek_x, budynek_wysokosc}));

                        if ((i == poz_mysz.first && j == poz_mysz.second)  && !czyBudowa) {
                            sprite_budynku.setColor(sf::Color(200, 200, 255, 255)); 
                        } else {
                            sprite_budynku.setColor(sf::Color::White);
                        }

                        window.draw(sprite_budynku);
                    }
                }

                    if (czyBudowa && (i == poz_mysz.first && j == poz_mysz.second)) {
                    
                    auto iterator_bazy = bazaDanych.find(nazw); 
                    if (iterator_bazy != bazaDanych.end()) {
                        
                        sf::Sprite hologram(atlas_budynkow);
                        
                        
                        hologram.setOrigin(sf::Vector2f(kafelek_x / 2.0f, budynek_wysokosc - (kafelek_y / 2.0f)));
                        hologram.setPosition(sf::Vector2f(X, Y));

                        int h_X = iterator_bazy->second.X;
                        int h_Y = iterator_bazy->second.Y;
                        hologram.setTextureRect(sf::IntRect({h_X, h_Y}, {kafelek_x, budynek_wysokosc}));

                        if (siatka[i][j].typ == TypKafelka::PUSTY) {
                            hologram.setColor(sf::Color(100, 150, 255, 180));
                        } else {
                            hologram.setColor(sf::Color(255, 100, 100, 180)); //czerwone
                        }

                        window.draw(hologram);
                    }
                }
            }
        }
    }
}

TypKafelka BuildingsGrid::getTypKafelka() const{return siatka[poz_mysz.first][poz_mysz.second].typ;}
pair<int,int> BuildingsGrid::getPozMysz() const { return poz_mysz; }
int BuildingsGrid::getID(pair<int,int> n) const{return siatka[n.first][n.second].id;}
string BuildingsGrid::getName(pair<int,int> n) const{return siatka[n.first][n.second].nazwa;}