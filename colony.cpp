#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <memory>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
#include "colony.h"

Colony::Colony():tura(0),reqEnergy(0),genEnergy(0),reqFood(0),workers(0){
}

void Colony::prnt(){
    cout<<"Nr tury: "<<tura<<endl;
    cout<<"Wymagana energia: "<<reqEnergy<<endl;
    cout<<"Produkowana energia: "<<reqEnergy<<endl;
    cout<<"Wymagane jedzenie: "<<reqFood<<endl;
    cout<<"Pracownicy: "<<workers<<endl;
}

void Colony::prntBuilding(int nr){
    if(nr>=0 && nr < buildings.size()){
        buildings[nr]->prnt();
    }else{
        cout<<"Blad: Nie ma budynku o takim ID: "<<nr<<endl;
        cout<<endl;
    }
    
}

void Colony::prntBuildings(){
    for(int i=0;i<buildings.size();i++){
        prntBuilding(i);
    }
}


void Colony::addBuilding(unique_ptr<Building> b){
    buildings.push_back(move(b));
    cout<<"Dodano nowy budynek!!"<<endl;
    cout<<endl;
}


void Colony::zbudujBudynek(TypEnergy typ){
    unique_ptr<Building> nowyBudynek;
        
    switch (typ){
    case TypEnergy::NIEZNANY:
        nowyBudynek=make_unique<Energy>();
        break;
    case TypEnergy::PANELE:
        nowyBudynek=make_unique<Energy>("Panele sloneczne", 0, 50.0,TypEnergy::PANELE,1);
        break;
    case TypEnergy::WIATRAK:
        nowyBudynek=make_unique<Energy>("Wiatrak", 0, 20,TypEnergy::WIATRAK,1);
        break;
    }
    addBuilding(move(nowyBudynek));
}

void Colony::zbudujBudynek(TypFarm typ){
    unique_ptr<Building> nowyBudynek;
        
    switch (typ){
    case TypFarm::NIEZNANY:
        nowyBudynek=make_unique<Farm>();
        break;
    case TypFarm::POLE:
        nowyBudynek=make_unique<Farm>("Pole", 3, 4,TypFarm::POLE,1);
        break;
    case TypFarm::SZKLARNIA:
        nowyBudynek=make_unique<Farm>("Szklarnia", 4, 3,TypFarm::SZKLARNIA,1);
        break;
    }
    addBuilding(move(nowyBudynek));
}

void Colony::zbudujBudynek(TypBudynku typ){
    unique_ptr<Building> nowyBudynek;
        
    switch (typ){
    case TypBudynku::NIEZNANY:
        nowyBudynek=make_unique<Building>();
        break;
    case TypBudynku::ENERGY:
        nowyBudynek=make_unique<Energy>();
        break;
    }
    addBuilding(move(nowyBudynek));
}

void Colony::zburzBudynek(int nr){
    if(nr>=0 && nr < buildings.size()){
        string dec;
        cout<<"Czy na pewno chcesz wyburzyc budynek: "<<buildings[nr]->getName()<<endl;
        cout<<"Potwierdz wpisujac TAK, albo anuluj NIE."<<endl;
        cin>>dec;
        if(dec=="TAK"){
            cout<<"Bdynek "<<buildings[nr]->getName()<<" zostal wyburzony."<<endl;
            buildings.erase(buildings.begin()+nr);
        }else{
            cout<<"Anulowano wyburzanie budynku."<<endl;
    }
    }else{
        cout<<"Blad: Nie ma budynku o takim ID: "<<nr<<endl;
        cout<<endl;
    }
    
}


int Colony::getIloscBudynkow(){
    return buildings.size();
}

void Colony::nextRound(){

}
void Colony::update(){

}

void Colony::saveBuildings(string nazwa_plik){
    ofstream plik(nazwa_plik);
    if(plik.is_open()){
         for(const auto& b : buildings){
            b->save(plik);
        }
        plik.close();
    }
   cout<<"Gra została zapisana do pliku '"<<nazwa_plik<<"'.";
}