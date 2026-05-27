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

    


BuildingsGrid::BuildingsGrid():siatka_x(1400),siatka_y(700),siatka_begin_x(400),siatka_begin_y(200),kafelek_x(200),kafelek_y(100),siatka_size_x(12),siatka_size_y(7),poz_mysz(-1,-1){
    siatka.resize(siatka_size_x,vector<TypKafelka>(siatka_size_y,TypKafelka::PUSTY));
    if(WczytajGrafike("assets/kafelek.png",kafelek_tex)&&WczytajGrafike("assets/kafelek_najechany.png",kafelek_najechany_tex)){
    }else{
        cout<<"Nie udalo sie wczytac tekstury kafelka!!!!!!"<<endl;
    }
    for (int x = 0; x < siatka_size_x; ++x) {
        for (int y = 0; y < siatka_size_y; ++y) {
            
            if((x % 2 != 0 && y == siatka_size_y - 1)||x == siatka_size_x - 1){
                siatka[x][y] = TypKafelka::BRAK;
            }
        }
    }
}
    //X=300 do 1700 = 1400 -> 200
    //Y=100 do 800 = 700 - > 100/
    //siatka inna
/**
 * @brief Funkcja tłumacząca coordynaty myszki.
 * 
 * @param poz ImVec2 myszki
 */
bool BuildingsGrid::sprawdzMysz(ImVec2& poz){
    // KOREKTA POZYCJI 
    float MyszX = poz.x - (siatka_begin_x - (kafelek_x / 2.0f));
    float MyszY = poz.y - (siatka_begin_y - (kafelek_y / 2.0f));
    
    if (MyszX < 0 || MyszY < 0) {
        return false;
    }

    // 2. KROK 1: Wstępny  wybór sekcji (prostokąta o wymiarach kafelek_x na kafelek_y)
    int curr_kolumna = static_cast<int>(MyszX) / kafelek_x;
    int curr_wiersz = (static_cast<int>(MyszY) / kafelek_y) * 2;

    // 3. KROK 2: Wyliczenie pozycji lokalnej wewnątrz tego prostokąta 
    int locX = static_cast<int>(MyszX) % (int)kafelek_x;
    int locY = static_cast<int>(MyszY) % (int)kafelek_y;

    // Pomocnicze zmienne określające środek prostokąta
    int polX = kafelek_x / 2;
    int polY = kafelek_y / 2;

    // 4. KROK 3: Test czterech narożników
    if (locY < polY - (locX / 2)) {
        // Róg lewy-górny
        curr_wiersz--;
        curr_kolumna--;
    }
    else if (locY < (locX / 2) - polY) {
        // Róg prawy-górny
        curr_wiersz--;
    }
    else if (locY > polY + (locX / 2)) {
        // Róg lewy-dolny
        curr_wiersz++;
        curr_kolumna--;
    }
    else if (locY > (polY + polX) - (locX / 2)) { 
        // Róg prawy-dolny
        curr_wiersz++;
    }

    // 5. KROK 4: Walidacja granic wektora i sprawdzenie typu kafelka
    if (curr_wiersz >= 0 && curr_wiersz < siatka_size_x &&
        curr_kolumna >= 0 && curr_kolumna < siatka_size_y) 
    {
        // Jeśli kafelek nie jest zamaskowany (czyli nie ma stanu BRAK)
        if (siatka[curr_wiersz][curr_kolumna] != TypKafelka::BRAK) {
            
            poz_mysz = {curr_wiersz, curr_kolumna}; 
            return true;
        }
    }

    return false; // Myszka jest poza sprawnym obszarem mapy
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
void BuildingsGrid::prntSiatka(sf::RenderWindow& window,ImVec2& poz){
    sf::Sprite kafelek_sprt(kafelek_tex);
    sf::Sprite kafelek_najechany_sprt(kafelek_najechany_tex);
    
    kafelek_najechany_sprt.setOrigin(sf::Vector2f(kafelek_x / 2.0f, kafelek_y / 2.0f));
    kafelek_sprt.setOrigin(sf::Vector2f(kafelek_x/2,kafelek_y/2));
    czyNajechane(poz);
    for(int i=0;i<siatka_size_x;i++){
        for(int j=0;j<siatka_size_y;j++){
            if(siatka[i][j]==TypKafelka::PUSTY){
                float X=0;
                if(i%2==0){
                    X=siatka_begin_x+j*(kafelek_x);

                }else{
                    X=(float)(siatka_begin_x+(j+0.5)*(kafelek_x));
                }

                float Y=siatka_begin_y+i*(kafelek_y/2.0f);

                if(i == poz_mysz.first && j == poz_mysz.second){
                    kafelek_najechany_sprt.setPosition(sf::Vector2f(X, Y));
                    window.draw(kafelek_najechany_sprt);
                } 
                else {
                    kafelek_sprt.setPosition(sf::Vector2f(X, Y));
                    window.draw(kafelek_sprt);
                }
            } 
            else if(siatka[i][j]==TypKafelka::BRAK){
                continue;
            }
        }
    }
    
}