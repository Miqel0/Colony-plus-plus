#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
#include "building.h"

//ID budynku
int Building::next_id=1;

// ==========================================
// KONSTRUKTORY
// ==========================================

Building:: Building():kosztEnergii(0),kosztKamien(0),kosztTytan(0),type(TypBudynku::NIEZNANY),name("Budynek"),id(next_id++),workers(0),residents(0){}
Building:: Building(string n,TypBudynku t, double kE,double kK, double kT,int w):kosztEnergii(kE),kosztKamien(kK),kosztTytan(kT),type(t),name(n),id(next_id++),workers(w), residents(0){}

// ==========================================
// VIRTUALNE
// ==========================================

//WYSWIETLANIE INFORMACJI O DANYM BUDYNKU
void Building:: prnt(int il)const{}

void Building:: UIprnt(int il)const{}

//WYKONYWANIE PRACY PRZEZ DANY BUDYNEK PODCZAS nextRound
double Building:: work(){return 0;}

//ZAPISYWANIE DANYCH O BUDYNKU DO PLIKU
void Building::save(ofstream& plik)const{
    plik<<static_cast<int>(type)<<" "<<name<<" "<<id<<" "<<kosztEnergii<<" "<<kosztKamien<<" "<<kosztTytan<<" "<<workers;
}

// ==========================================
// LICZNIKI
// ==========================================

void Building:: setId(int i){id=i;}

void Building::updateLicznik(int wczytaneMaxId){
    if(wczytaneMaxId>=next_id){
        next_id=wczytaneMaxId+1;
    }
}

// ==========================================
// GETTERY
// ==========================================

string Building:: getName()const {return name;}
TypBudynku Building::getTyp() const {return type;}
int Building::getResidents() const{return residents;}
int Building::getDemandWorkers()const {return workers;}
double Building::getReqEnergy() const{return kosztEnergii;}
double Building::getKosztKamien() const{return kosztKamien;}
double Building::getKosztTytan() const{return kosztTytan;}
