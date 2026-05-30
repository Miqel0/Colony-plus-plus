#include "buildingsGrid.h"
#include "game.h"

#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

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
    if(WczytajGrafike("assets/grafiki_siatka.png",atlas_budynkow)){
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
 * @brief Konstruktor siatki budynków.
 * 
 */
BuildingsGrid::BuildingsGrid():siatka_x(1400),siatka_y(700),siatka_begin_x(400),siatka_begin_y(200),kafelek_x(200),kafelek_y(100),siatka_size_x(12),siatka_size_y(7),poz_mysz(-1,-1){
    siatka.resize(siatka_size_x,vector<DaneKafelek>(siatka_size_y));
   
    siatka[6][2].typ=TypKafelka::ZAJETY;
    siatka[6][2].nazwa="Pole_Ziemniakow";
    siatka[6][3].typ=TypKafelka::ZAJETY;
    siatka[6][3].nazwa="Odkrywka_Kamienia";
    siatka[6][4].typ=TypKafelka::ZAJETY;
    siatka[6][4].nazwa="Maly_Wiatrak";
    siatka[6][5].typ=TypKafelka::ZAJETY;
    siatka[6][5].nazwa="Stacja_Badawcza";
    siatka[6][1].typ=TypKafelka::ZAJETY;
    siatka[6][1].nazwa="Barak_Robotniczy";
    for (int x = 0; x < siatka_size_x; ++x) {
        for (int y = 0; y < siatka_size_y; ++y) {
            
            if((x % 2 != 0 && y == siatka_size_y - 1)||x == siatka_size_x - 1){
                siatka[x][y].typ = TypKafelka::BRAK;
                siatka[x][y].nazwa="brak";
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
void BuildingsGrid::prntSiatka(sf::RenderWindow& window,ImVec2& poz,const map<string, BuildingInfo>& bazaDanych,bool czyBudowanie, string nazwaTrzymanego ){
    sf::Sprite kafelek_budynek(atlas_budynkow);
    kafelek_budynek.setOrigin(sf::Vector2f(kafelek_x / 2.0f, kafelek_y / 2.0f));

    sf::Vector2i pozycjaPix = sf::Mouse::getPosition(window);
    sf::Vector2f pozycjaSwiata = window.mapPixelToCoords(pozycjaPix);
    ImVec2 Pozycja(pozycjaSwiata.x, pozycjaSwiata.y);

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

                //koordynaty textury w atlasie
                int t_X=0;
                int t_Y=0;
                //Czy najechane
                if(i == poz_mysz.first && j == poz_mysz.second){
                    t_Y+=100;
                } 

                if(siatka[i][j].typ==TypKafelka::PUSTY){
                    t_X=800;
                } else{
                    auto iterator_tekstury = bazaDanych.find(nazwa_kafelka);
                    
                    if (iterator_tekstury != bazaDanych.end()){
                        
                        t_X=bazaDanych.at(nazwa_kafelka).X;
                        t_Y+=bazaDanych.at(nazwa_kafelka).Y;
                    } else{
                        cout<<"Brakuje "<<nazwa_kafelka<<"!"<<endl;
                    }
                }

                kafelek_budynek.setTextureRect(sf::IntRect({t_X, t_Y}, {kafelek_x, kafelek_y}));
                kafelek_budynek.setPosition(sf::Vector2f(X, Y));
                window.draw(kafelek_budynek);

                //Rysowanie tego budowaniowego
                if (czyBudowanie && i == poz_mysz.first && j == poz_mysz.second) {
                    string nazw=nazwaTrzymanego;
                    for(auto &a:nazw) a =tolower(a);

                    sf::Sprite hologram(atlas_budynkow); 
                    hologram.setOrigin(sf::Vector2f(kafelek_x / 2.0f, kafelek_y / 2.0f));
                    hologram.setPosition(sf::Vector2f(X, Y));
    
                    auto iterator_bazy = bazaDanych.find(nazwaTrzymanego);
                    if (iterator_bazy != bazaDanych.end()) {
                        int h_X = iterator_bazy->second.X;
                        int h_Y = iterator_bazy->second.Y;
            
                        hologram.setTextureRect(sf::IntRect({h_X,h_Y},{kafelek_x, kafelek_y}));

                        if (siatka[i][j].typ == TypKafelka::PUSTY) {
                            // Można budować: Niebieski i przezroczysty 
                            hologram.setColor(sf::Color(100, 150, 255, 180)); 
                        } else {
                            // Zajęte lub niedostępne: Czerwony
                            hologram.setColor(sf::Color(255, 100, 100, 180)); 
                        }
            
                        window.draw(hologram);
                    }
                }
            }
        }
    }
}


pair<int,int> BuildingsGrid::getPozMysz() const { return poz_mysz; }