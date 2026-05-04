#ifndef graphics_h
#define graphics_h

#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "colony.h"

using namespace std;



class Graphics{
    private:
        sf::RenderWindow window;
        unsigned int szer;
        unsigned int wys;
        bool czyhelp;
        bool czyBudynki;

    public:

        Graphics();
        Graphics(unsigned int szer_,unsigned int wys_);
        ~Graphics(){};

        void prntAll(const Colony& kolonia);
        void prntMenu();
        void prntStatystyki(const Colony& kolonia);
        void prntPomoc();
        void prntBudynki(const Colony& kolonia);



};

#endif
