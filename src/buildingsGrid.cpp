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

    


BuildingsGrid::BuildingsGrid():siatka_x(1400),siatka_y(700),siatka_begin_x(300),siatka_begin_y(100),kafelek_x(200),kafelek_y(100),siatka_size_x(13),siatka_size_y(13){
    siatka.resize(siatka_size_y,vector<TypKafelka>(siatka_size_x,TypKafelka::PUSTY));
    if(WczytajGrafike("assets/kafelek.png",kafelek_tex)){
        // kafelek_sprt.setTexture(kafelek_tex);
        // kafelek_sprt.setOrigin(sf::Vector2f(kafelek_x/2,kafelek_y/2));
    }else{
        cout<<"Nie udalo sie wczytac tekstury kafelka!!!!!!"<<endl;
    }
    for (int x = 0; x < siatka_size_x; ++x) {
        for (int y = 0; y < siatka_size_y; ++y) {
            
            if ((x + y < 6) ||(x + (siatka_size_y - 1 - y) < 6) ||((siatka_size_x - 1 - x) + y < 6) ||((siatka_size_x - 1 - x) + (siatka_size_y - 1 - y) < 6)){
                siatka[x][y] = TypKafelka::BRAK;
            } 
        }
    }
}
    //X=300 do 1700 = 1400 -> 200
    //Y=100 do 800 = 700 - > 100/
    //siatka 7x7


void BuildingsGrid::prntSiatka(sf::RenderWindow& window){
    sf::Sprite kafelek_sprt(kafelek_tex);
    kafelek_sprt.setOrigin(sf::Vector2f(kafelek_x/2,kafelek_y/2));
    for(int i=0;i<siatka_size_x;i++){
        for(int j=0;j<siatka_size_y;j++){
            if(siatka[i][j]==TypKafelka::PUSTY){
                float X = siatka_begin_x+675 + (i - j) * (kafelek_x / 2.0f);
                float Y = siatka_begin_y-225+ (i + j) * (kafelek_y / 2.0f);
                kafelek_sprt.setPosition(sf::Vector2f(X, Y));
                    
                window.draw(kafelek_sprt);
            } 
            else if(siatka[i][j]==TypKafelka::BRAK){
                continue;
            }
        }
    }
}