#ifndef enums_h
#define enums_h

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <utility>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

using namespace std;

// ==========================================
// FORMATOWANIE TEKSTU
// ==========================================

//PODSTAWOWE
inline const string RESET     = "\033[0m";
inline const string BOLD      = "\033[1m"; 
inline const string NO_BOLD   = "\033[22m"; 
inline const string UNDERLINE = "\033[4m";

// KOLORY
inline const string BLACK   = "\033[30m";
inline const string RED     = "\033[31m";
inline const string GREEN   = "\033[32m";
inline const string YELLOW  = "\033[33m";
inline const string BLUE    = "\033[34m"; 
inline const string MAGENTA = "\033[35m"; 
inline const string CYAN    = "\033[36m"; 
inline const string WHITE   = "\033[37m"; 

// TŁA  
inline const string BG_BLACK   = "\033[40m";
inline const string BG_RED     = "\033[41m";
inline const string BG_GREEN   = "\033[42m";
inline const string BG_YELLOW  = "\033[43m";
inline const string BG_BLUE    = "\033[44m";
inline const string BG_MAGENTA = "\033[45m";
inline const string BG_CYAN    = "\033[46m";
inline const string BG_WHITE   = "\033[47m";

// INNE
inline const std::string CLEAR_SCREEN = "\033[2J\033[1;1H"; // czyszczenie ekranu 

// ==========================================
// ENUMY
// ==========================================

enum class TypBudynku {
    NIEZNANY = 0,
    ENERGY = 1,
    FARM = 2,
    HOUSING = 3,
    PRODUCER = 4,
    TERR = 5,
};

enum class TypEnergy {
    NIEZNANY = 0,
    MALY_WIATRAK = 1,
    DUZY_PANEL = 2,
    REAKTOR_JADROWY = 3,
    FUZJA_ZIMNA = 4,
};

enum class TypFarm {
    NIEZNANY = 0,
    POLE_ZIEMNIAKOW = 1,
    SZKLARNIA_HYDRO = 2,
    FARMA_ALG = 3,
    SYNTEZATOR_BIALKA = 4,
};

enum class TypDomy {
    NIEZNANY = 0,
    BARAK_ROBOTNICZY = 1,
    KWATERY_ZALOGI = 2,
    KOPULA_MIESZKALNA = 3,
    METROPOLIA = 4,
};

enum class TypProducer {
    NIEZNANY = 0,
    ODKRYWKA_KAMIENIA = 1,
    WIERTLO_GLEBINOWE = 2,
    KOMBINAT_GORNICZY = 3,
    AUTOMAT_WYDOBYWCZY = 4,
    KOPALNIA_TYTANU = 5,
};

enum class TypTerr {
    NIEZNANY = 0,
    STACJA_BADAWCZA = 1,
    KOMINY_CIEPLNE = 2,
    GENERATOR_O2 = 3,
    LUSTRA_ORBITALNE = 4,
};


//Do mapy gamedata - wczytywanie danych o budynkach - uprascza to wczytywanie i budowanie
struct BuildingInfo {
    string nazwa;
    string type;
    
    // Koszty
    double kKamien;
    double kTytan;
    double reqEnergy;
    int workers;      
    
    // Produkcja
    double genKamien;
    double genTytan;  
    double genInne;  
    double lvlTerr;
    int x;     
    string opis;
};

// ==========================================
// FUNKCJE POMOCNICZE - UNIWERSALNE
// ==========================================

//WYSWIETLANIE NAGLOWKA
inline void prntHeader(string text, string col = MAGENTA, int n = 70) {
    int len = text.length();
    
    if (len >= n - 2) { 
        cout << YELLOW << BOLD << text << RESET << endl;
        return;
    }
    
    int miejsce = n - len - 2;
    int left = miejsce / 2;
    int right = miejsce - left; 

    cout << endl;
    cout << YELLOW << string(left, '-') << " " << BOLD << col << text << YELLOW << NO_BOLD << " " << string(right, '-') << RESET << endl;
}

//TEMPLATE do czyszczenia liczb - do wyswietlania bo normlanie zamienianie na string nie dzialalo dla double

inline string cleanNum(double liczba) {
    stringstream ss;
    ss << liczba;
    return ss.str();
}

// WYSWIETLANIE TABLIC - 2 WIERSZE
inline void prntTablica(string n, string s11, string s12, string s13, string s14, string s21, string s22, string s23, string s24) {
    prntHeader(n);
    const int col = 33;
    const string sep = " | ";
    
    cout << YELLOW << BOLD;
    
    string col1 = s11 + s12;
    cout << left << setw(col) << col1 << NO_BOLD << sep << BOLD << s13 << s14 << endl;
    
    string col2 = s21 + s22;
    cout << left << setw(col) << col2 << NO_BOLD << sep << BOLD << s23 << s24 << RESET << endl << endl;
}

// WYSWIETLANIE TABLIC - 1 WIERSZ
inline void prntTablica(string n, string s11, string s12, string s13, string s14) {
    prntHeader(n);
    const int col = 33;
    const string sep = " | ";
    
    cout << YELLOW << BOLD;
    
    string col1 = s11 + s12;
    cout << left << setw(col) << col1 << NO_BOLD << sep << BOLD << s13 << s14 << RESET << endl << endl;
}

// WYSWIETLANIE TABLIC - 3 WIERSZE
inline void prntTablica(string n, string s11, string s12, string s13, string s14, string s21, string s22, string s23, string s24, string s31, string s32, string s33, string s34) {
    prntHeader(n);
    const int col = 33;
    const string sep = " | ";
    
    cout << YELLOW << BOLD;
    
    string col1 = s11 + s12;
    cout << left << setw(col) << col1 << NO_BOLD << sep << BOLD << s13 << s14 << endl;
    
    string col2 = s21 + s22;
    cout << left << setw(col) << col2 << NO_BOLD << sep << BOLD << s23 << s24 << endl;

    string col3 = s31 + s32;
    cout << left << setw(col) << col3 << NO_BOLD << sep << BOLD << s33 << s34 << RESET << endl << endl;
}





/**
 * @brief Funkcja wyswietlajaca jeden wiersz tabeli
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
inline void prntTooltipTablica(const string& nazwa, 
                               const vector<pair<string, string>>& dane, 
                               const string& opis = "") {
                               

    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "--- %s ---", nazwa.c_str());
    ImGui::Separator();
    
   
    if (!dane.empty()) {
        if (ImGui::BeginTable("StatsTooltip", 2, ImGuiTableFlags_BordersInnerH)) {
            

            for (const auto& para : dane) {
                rysujWierszTooltip(para.first, para.second);
            }
            
            ImGui::EndTable();
        }
    }
    

    if (!opis.empty()) {
        ImGui::Separator();
        ImGui::PushTextWrapPos(300.0f); 
        ImGui::Text("%s", opis.c_str());
        ImGui::PopTextWrapPos(); 
    }
}
#endif