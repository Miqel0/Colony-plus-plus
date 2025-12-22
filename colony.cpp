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
//moze zmienc ze w basicowym na tart dostaje budynek a nie ze dostaje tak o rzeczy
//moze dodac ratusz? czy cos, co dodaje te parametry?
Colony::Colony():tura(0),reqEnergy(0),genEnergy(0),reqFood(0),all_workers(5),demand_workers(0){}

void Colony::prnt(){
    cout<<"Nr tury: "<<tura<<endl;
    cout<<"Wymagana energia: "<<reqEnergy<<endl;
    cout<<"Produkowana energia: "<<reqEnergy<<endl;
    cout<<"Wymagane jedzenie: "<<reqFood<<endl;
    cout<<"Pracownicy: "<<demand_workers<<"/"<<all_workers<<endl;
}

void Colony::prntBuilding(int nr){
    if(nr>=0 && nr <= buildings.size()){
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
        nowyBudynek=make_unique<Energy>("Panele_sloneczne", 0, 50.0,TypEnergy::PANELE,1);
        
        break;
    case TypEnergy::WIATRAK:
        nowyBudynek=make_unique<Energy>("Wiatrak", 0, 20,TypEnergy::WIATRAK,1);
        break;
    }
    demand_workers+=nowyBudynek->getDemandWorkers();
    addBuilding(move(nowyBudynek));
}

void Colony::zbudujBudynek(TypFarm typ){
    unique_ptr<Building> nowyBudynek;
        
    switch (typ){
    case TypFarm::NIEZNANY:
        nowyBudynek=make_unique<Farm>();
        break;
    case TypFarm::POLE:
        nowyBudynek=make_unique<Farm>("Pole", 3, 4,TypFarm::POLE,1,3);
        break;
    case TypFarm::SZKLARNIA:
        nowyBudynek=make_unique<Farm>("Szklarnia", 4, 3,TypFarm::SZKLARNIA,1,2);
        break;
    }
    demand_workers+=nowyBudynek->getDemandWorkers();
    addBuilding(move(nowyBudynek));
}


void Colony::zbudujBudynek(TypDomy typ){
    unique_ptr<Building> nowyBudynek;
        
    switch (typ){
    case TypDomy::NIEZNANY:
        nowyBudynek=make_unique<Housing>();
        break;
    case TypDomy::BARAK:
        nowyBudynek=make_unique<Housing>("Barak", 0, 5,TypDomy::BARAK,0);
        break;
    case TypDomy::REZYDENCJA:
        nowyBudynek=make_unique<Housing>("Rezydencja", 0, 8,TypDomy::REZYDENCJA,0);
        break;
    }
    all_workers+=nowyBudynek->getResidents();
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
    case TypBudynku::FARM:
        nowyBudynek=make_unique<Farm>();
        break;
    case TypBudynku::HOUSING:
        nowyBudynek=make_unique<Housing>();
        break;
    }
    demand_workers+=nowyBudynek->getDemandWorkers();
    addBuilding(move(nowyBudynek));
}

void Colony::zburzBudynek(int nr){
    if(nr>=0 && nr < buildings.size()){
        string dec;
        cout<<"Czy na pewno chcesz wyburzyc budynek: "<<buildings[nr]->getName()<<endl;
        cout<<"Potwierdz wpisujac TAK, albo anuluj NIE."<<endl;
        cin>>dec;
        if(dec=="TAK"){
            cout<<"Budynek "<<buildings[nr]->getName()<<" zostal wyburzony."<<endl;
            demand_workers-=buildings[nr]->getDemandWorkers();
            if(static_cast<int>(buildings[nr]->getTyp())==static_cast<int>(TypBudynku::HOUSING)){
                all_workers-=buildings[nr]->getResidents();
            }
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



void Colony::loadBuildings(string nazwa_plik) {
    ifstream plik(nazwa_plik);

    if (plik.is_open()) {
        buildings.clear();
        


    // plik<<static_cast<int>(type)<<" "<<name<<" "<<id<<" "<<kosztEnergii<<" "<<workers;

    //farm:     plik<<" "<<static_cast<int>(pType)<<" "<<foodGen<<" "<<time<<endl;
    //energy:     plik<<" "<<static_cast<int>(pType)<<" "<<enGen<<" "<<endl;
        //uniwersalne dane

        int w_type, w_id, w, maxSaved = 0, w_ptype;
        string w_n;
        double k;


        while (plik >> w_type >> w_n >> w_id >> k >> w) {
            
            TypBudynku typ = static_cast<TypBudynku>(w_type);
            unique_ptr<Building> nowyBudynek = nullptr;
            switch (typ) {
                case TypBudynku::NIEZNANY:

                    break;

                case TypBudynku::ENERGY: {
                    double e;
                    plik >> w_ptype >> e;
                
                    auto energia = make_unique<Energy>(w_n, k, e, static_cast<TypEnergy>(w_ptype), w);
                
                    energia->setId(w_id); 
                    
                    nowyBudynek = move(energia);
                    demand_workers+=nowyBudynek->getDemandWorkers();
                    nowyBudynek->prnt();

                    break; 
                }

                case TypBudynku::FARM: {
                    double f;
                    int tim;
                    plik >> w_ptype  >> f>>tim;
                
                    auto farm = make_unique<Farm>(w_n, k, f, static_cast<TypFarm>(w_ptype), w,tim);

                    farm->setId(w_id); 
                    
                    nowyBudynek = move(farm);
                    demand_workers+=nowyBudynek->getDemandWorkers();
                    nowyBudynek->prnt();

                    break;
                }
                case TypBudynku::HOUSING:{
                    int r;
                    plik >> w_ptype >> r;
                
                    auto energia = make_unique<Housing>(w_n, k, r, static_cast<TypDomy>(w_ptype), w);
                
                    energia->setId(w_id); 
                    
                    nowyBudynek = move(energia);
                    
                    all_workers+=nowyBudynek->getResidents(); //jak dodawac te głupie zmienne
                    nowyBudynek->prnt();

                    break;
                
                }
            } 

            if (nowyBudynek != nullptr) {
                if (w_id > maxSaved) {
                    maxSaved = w_id;
                }
            }
            buildings.push_back(move(nowyBudynek));
        } 
        Building::updateLicznik(maxSaved);
    } 
    plik.close();
} 


int Colony::getAllWorkers(){return all_workers;}
int Colony::getDemandWorkers(){return demand_workers;}