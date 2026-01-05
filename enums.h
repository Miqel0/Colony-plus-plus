#ifndef enums_h
#define enums_h

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

// FORMATOWANIE TEKSTU
inline const string RESET     = "\033[0m";
inline const string BOLD      = "\033[1m"; 
inline const string NO_BOLD   = "\033[22m"; 
inline const string UNDERLINE = "\033[4m";

// KOLORY
inline const std::string BLACK   = "\033[30m";
inline const std::string RED     = "\033[31m";
inline const std::string GREEN   = "\033[32m";
inline const std::string YELLOW  = "\033[33m";
inline const std::string BLUE    = "\033[34m"; 
inline const std::string MAGENTA = "\033[35m"; 
inline const std::string CYAN    = "\033[36m"; 
inline const std::string WHITE   = "\033[37m"; 

// TŁA  
inline const std::string BG_BLACK   = "\033[40m";
inline const std::string BG_RED     = "\033[41m";
inline const std::string BG_GREEN   = "\033[42m";
inline const std::string BG_YELLOW  = "\033[43m";
inline const std::string BG_BLUE    = "\033[44m";
inline const std::string BG_MAGENTA = "\033[45m";
inline const std::string BG_CYAN    = "\033[46m";
inline const std::string BG_WHITE   = "\033[47m";

// INNE
inline const std::string CLEAR_SCREEN = "\033[2J\033[1;1H"; // czyszczenie ekranu 

// ENUMY
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
    WIATRAK = 1,
    PANELE = 2,
};

enum class TypFarm {
    NIEZNANY = 0,
    SZKLARNIA = 1,
    POLE = 2,
};

enum class TypDomy {
    NIEZNANY = 0,
    BARAK = 1,
    REZYDENCJA = 2,
};

enum class TypProducer { // moze zrobic przechowywanie danych jako tablica??
    NIEZNANY = 0,
    KOPALNIA_KAMIENIA = 1,
    KOPALNIA_TYTANU = 2,
    ZAAWANSOWANA_KOPALNIA = 3,
};

enum class TypTerr { // moze zrobic przechowywanie danych jako tablica??
    NIEZNANY = 0,
    cos1 = 1,
    cos2 = 2,
    cos3 = 3,
};


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
    double genInne;   // Prąd / Jedzenie / Mieszkańcy / WspTerr
    
    int x;     // czas wzrostu (Farmy) 
    string opis;
};

// FUNKCJE POMOCNICZE

inline void prntHeader(string text, string col = MAGENTA, int n = 70) {
    int len = text.length();
    
    // POPRAWKA: uwzględniamy margines (-2), żeby uniknąć liczb ujemnych
    if (len >= n - 2) { 
        cout << YELLOW << BOLD << text << RESET << endl;
        return;
    }
    
    int miejsce = n - len - 2;
    int left = miejsce / 2;
    int right = miejsce - left; 

    cout << endl;
    // Tutaj była główna przyczyna błędów (dziwne spacje w stringach)
    cout << YELLOW << string(left, '-') << " " << BOLD << col << text << YELLOW << NO_BOLD << " " << string(right, '-') << RESET << endl;
}

template <typename T>
inline string cleanNum(T val) {
    stringstream ss;
    ss << val;
    return ss.str();
}

// Przeciążenia funkcji prntTablica

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

inline void prntTablica(string n, string s11, string s12, string s13, string s14) {
    prntHeader(n);
    const int col = 33;
    const string sep = " | ";
    
    cout << YELLOW << BOLD;
    
    string col1 = s11 + s12;
    cout << left << setw(col) << col1 << NO_BOLD << sep << BOLD << s13 << s14 << RESET << endl << endl;
}

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

#endif