#ifndef enums_h
#define enums_h

#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <iomanip>
#include <vector>
#include <utility>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

using namespace std;

/**
 * @brief Typy budynków, używany w różnych logikach.
 * 
 */
enum class TypBudynku {
    NIEZNANY = 0,
    ENERGY = 1,
    FARM = 2,
    HOUSING = 3,
    PRODUCER = 4,
    TERR = 5,
};

/**
 * @brief Typ ekranu.
 * 
 */
enum class TypEkranu {MAIN_MENU,GAME,MENU_GAME,SETTINGS,CREDITS,LOAD};

/**
 * @brief Zawiera w sobie wszystkie parametry danego budynku wczytane z gamedata.txt
 * 
 */
struct BuildingInfo {
    string nazwa="";
    string type="";
    
    // Koszty
    int kKamien=0;
    int kTytan=0;
    int reqEnergy=0;
    int workers=0;      
    
    // Produkcja
    int genKamien=0;
    int genTytan=0;  
    int genInne=0;  
    int lvlTerr=0;
    int x=0;     
    string opis="";
};

/**
 * @brief Struct do przekazywania wyników budowania / a może i wszystkich błędów itp
 * 
 */
struct BuildResult{
    bool czy=false;
    int kamien=0;
    int tytan=0;
    int workers=0;
    string nazwa;
    int act_ruch=0;
    bool ruch=false;
};

/**
 * @brief Struct do przekazywania wyników burzenia.
 * 
 */
struct DestroyResult{
    bool czy=false;
    int brakLudzi=0;
    string nazwa="";
    pair <int,int> sur={0,0};
};

/**
 * @brief Struct do przekazywania wyników przejscia do kolejnej rundy
 * 
 */
struct NextResult{
    bool czy=false;
    bool food=false;
    bool energy=false;
    bool terr=false;
    string tekst="";
    int c_food=0;
    int c_stone=0;
    int c_titan=0;
    int c_terr=0;
};


// ==========================================
// FUNKCJE POMOCNICZE - UNIWERSALNE
// ==========================================

/**
 * @brief Stara funkcja do czyszczenia liczb - można zamienić na to_string czy cos
 * //FIXME
 * @param liczba 
 * @return string 
 */
inline string cleanNum(int liczba) {
    stringstream ss;
    ss << liczba;
    return ss.str();
}

/**
 * @brief Funkcja do usuwania '_' z tekstu.
 * 
 * @param tekst string do usuniecia '_'
 * @return string tekst bez '_'
 */
inline string cleanString(string tekst){
        string tekst_ = tekst;
        std::replace(tekst_.begin(),tekst_.end(),'_',' ');
        return tekst_;
}


/**
 * @brief Funkcja wyswietlajaca jeden wiersz tabeli - ImGUI
 * 
 * @param etykieta nazwa danego parametru
 * @param wartosc  wartosc parametru
 */
inline void rysujWierszTooltip(const string& etykieta, const string& wartosc) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn(); ImGui::Text("%s", etykieta.c_str());
    ImGui::TableNextColumn(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", wartosc.c_str());
}

/**
 * @brief Funkcja rysująca tabelę (uniwersalna)
 * 
 * @param nazwa nazwa nagłówka
 * @param dane wektor par danych do wyswietlenia w tabeli - vector<pair<string, string>>
 * @param opis opis dłuższy napis, pod tabelą
 */
inline void prntTooltipTablica(const string& nazwa, const vector<pair<string, string>>& dane) {
    float window_width = ImGui::GetWindowWidth();
    float text_width = ImGui::CalcTextSize(nazwa.c_str()).x;

    string nazwa_ = cleanString(nazwa);

    ImGui::SetCursorPosX((window_width - text_width) * 0.5f);  
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "%s", nazwa_.c_str());
    ImGui::Separator();
    
   
    if (!dane.empty()) {
        if (ImGui::BeginTable("StatsTooltip", 2, ImGuiTableFlags_BordersInnerH)) {
            

            for (const auto& para : dane) {
                rysujWierszTooltip(para.first, para.second);
            }
            
            ImGui::EndTable();
        }
    }
    
}

/**
 * @brief Funkcja wyświetlająca opis danego budynku.
 * 
 * @param nazwa wskaznik do nazwy budynku z '_' i z różnymi literami
 * @param bazaDanych wskaznik do bazy danych budynków map<std::string, BuildingInfo>
 */
inline void prntOpis(const std::string &nazwa, const  map<std::string, BuildingInfo>& bazaDanych){
        string nazwa_=nazwa;
        for(auto &c : nazwa_) c = tolower(c);
        ImGui::Separator();
        ImGui::PushTextWrapPos(300.0f); 
        
        if (bazaDanych.count(nazwa_)) {
            ImGui::Text("%s", bazaDanych.at(nazwa_).opis.c_str());
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Brak opisu.");
        }
        ImGui::PopTextWrapPos();    
}

/**
 * @brief Funckja rysująca tekst i nazwe
 * 
 * @param nazwa tytuł nagłówka
 * @param tekst tekst zawijany
 */
inline void prntOpis(const std::string &nazwa, string &tekst){
        string nazwa_=nazwa;
        for(auto &c : nazwa_) c = tolower(c);
        ImGui::Separator();
        ImGui::PushTextWrapPos(300.0f); 
        
        if (!tekst.empty()) {
            ImGui::Text("%s", tekst.c_str());
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Brak opisu.");
        }
        ImGui::PopTextWrapPos();    
}

/**
 * @brief Funckja rysująca tekst
 * 
 * @param tekst tekst zawijany
 */
inline void prntOpis(string &tekst){

        ImGui::PushTextWrapPos(300.0f); 
        
        if (!tekst.empty()) {
            ImGui::Text("%s", tekst.c_str());
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Brak opisu.");
        }
        ImGui::PopTextWrapPos();    
}

#endif
