#ifndef enums_h
#define enums_h

#include <string>
using namespace std;


inline const string RESET = "\033[0m";
inline const string BOLD        = "\033[1m";  
inline const string UNDERLINE   = "\033[4m";

inline const std::string BLACK       = "\033[30m";
inline const std::string RED         = "\033[31m";
inline const std::string GREEN       = "\033[32m";
inline const std::string YELLOW      = "\033[33m"; // Często wygląda jak brązowy/ciemnożółty
inline const std::string BLUE        = "\033[34m"; // Ciemny niebieski (słabo widać na czarnym tle)
inline const std::string MAGENTA     = "\033[35m"; // Fioletowy / Purpurowy
inline const std::string CYAN        = "\033[36m"; // Błękitny / Morski
inline const std::string WHITE       = "\033[37m"; // Szary / Ciemnobiały




enum class TypBudynku{
    NIEZNANY = 0,
    ENERGY = 1,
    FARM=2,
    HOUSING=3,
    PRODUCER=4,
};


enum class TypEnergy{
    NIEZNANY=0,
    WIATRAK = 1,
    PANELE = 2,
};

enum class TypFarm{
    NIEZNANY=0,
    SZKLARNIA = 1,
    POLE = 2,
};

enum class TypDomy{
    NIEZNANY=0,
    BARAK=1,
    REZYDENCJA=2,
};

enum class TypProducer{
    NIEZNANY=0,
    KOPALNIA_KAMIENIA=1,
    KOPALNIA_TYTANU=2,
    ZAAWANSOWANA_KOPALNIA=3,
};

#endif
