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


Colony::Colony():tura(1),all_workers(10),demand_workers(0){}

void Colony::prnt(){
    cout<<YELLOW<<BOLD<<" - - - - - - - - - - - Informacje COLONY - - - - - - - - - "<<RESET<<endl;
    cout<<MAGENTA<<"Nr tury: "<<tura<<RESET<<endl;
    if(demand_workers==all_workers){
        cout<<BLUE<<"Pracownicy: "<<demand_workers<<"/"<<all_workers<<RESET<<endl;
    }else{
        cout<<GREEN<<"Pracownicy: "<<demand_workers<<"/"<<all_workers<<RESET<<endl;
    }
    
    f_logisyka.prnt();
}

void Colony::prntBuilding(int nr){
    if(nr>=0 && nr <= buildings.size()){
        buildings[nr]->prnt();
    }else{
        cout<<RED<<"Blad: Nie ma budynku o takim ID: "<<RESET<<nr<<endl;
        cout<<endl;
    }
    
}

void Colony::prntBuildings(){
    for(int i=0;i<buildings.size();i++){
        prntBuilding(i);
    }
}


void Colony::prntBuildingsShort(){
    cout<<YELLOW<<" - - - - - - - - - Obecnie zbudowane budynki: - - - - - - - - -"<<RESET<<endl;
    for(int i=0;i<buildings.size();i++){
        cout<<i<<". "<<buildings[i]->getName()<<endl;
    }
}


void Colony::addBuilding(unique_ptr<Building> b){
    buildings.push_back(move(b));
    //cout<<"Dodano nowy budynek!!"<<endl;
    //cout<<endl;
}


void Colony::zbudujBudynek(TypEnergy typ){
    unique_ptr<Building> nowyBudynek;
        
    switch (typ){
    case TypEnergy::NIEZNANY:
        nowyBudynek=make_unique<Energy>();
        break;
    case TypEnergy::PANELE:
        nowyBudynek=make_unique<Energy>("Panele_sloneczne", 0, 50.0,TypEnergy::PANELE,3);
        
        break;
    case TypEnergy::WIATRAK:
        nowyBudynek=make_unique<Energy>("Wiatrak", 0, 20,TypEnergy::WIATRAK,5);
        break;
    }
    if(all_workers-demand_workers-nowyBudynek->getDemandWorkers()<0){
        cout<<endl;
        cout<<"Nie mozliwe jest zbudowanie budynku, za malo dostepnych pracownikow! Brakuje: "<<-(all_workers-demand_workers-nowyBudynek->getDemandWorkers())<<" robotnikow!"<<endl;
        cout<<endl;
        
    }else{
        f_logisyka.updateBudynek(nowyBudynek.get());
        demand_workers+=nowyBudynek->getDemandWorkers();
        addBuilding(move(nowyBudynek));
    }
}

void Colony::zbudujBudynek(TypFarm typ){
    unique_ptr<Building> nowyBudynek;
        
    switch (typ){
    case TypFarm::NIEZNANY:
        nowyBudynek=make_unique<Farm>();
        break;
    case TypFarm::POLE:
        nowyBudynek=make_unique<Farm>("Pole", 3, 4,TypFarm::POLE,1,1);
        break;
    case TypFarm::SZKLARNIA:
        nowyBudynek=make_unique<Farm>("Szklarnia", 4, 3,TypFarm::SZKLARNIA,1,2);
        break;
    }
    if(all_workers-demand_workers-nowyBudynek->getDemandWorkers()<0){
        cout<<"Nie mozliwe jest zbudowanie budynku, za malo dostepnych pracownikow! Brakuje: "<<-(all_workers-demand_workers-nowyBudynek->getDemandWorkers())<<" robotnikow!"<<endl;
        
    }else{
        f_logisyka.updateBudynek(nowyBudynek.get());
        demand_workers+=nowyBudynek->getDemandWorkers();
        addBuilding(move(nowyBudynek));
    }
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
    
    f_logisyka.updateBudynek(nowyBudynek.get());
    all_workers+=nowyBudynek->getResidents();
    addBuilding(move(nowyBudynek));
}



void Colony::zbudujBudynek(TypProducer typ){
    unique_ptr<Building> nowyBudynek;
        
    switch (typ){
    case TypProducer::NIEZNANY:
        nowyBudynek=make_unique<Producer>();
        break;
    case TypProducer::KOPALNIA_KAMIENIA:
        nowyBudynek=make_unique<Producer>("Kopalnia_kamienia", 3, 5,TypProducer::KOPALNIA_KAMIENIA,4,0);
        break;
    case TypProducer::KOPALNIA_TYTANU:
        nowyBudynek=make_unique<Producer>("Kopalnia_tytanu", 4, 0,TypProducer::KOPALNIA_TYTANU,4,5);
        break;
    case TypProducer::ZAAWANSOWANA_KOPALNIA:
        nowyBudynek=make_unique<Producer>("Zaawansowana_kopalnia", 4, 5,TypProducer::ZAAWANSOWANA_KOPALNIA,6,5);
        break;
    }
    if(all_workers-demand_workers-nowyBudynek->getDemandWorkers()<0){
        cout<<RED<<"Nie mozliwe jest zbudowanie budynku, za malo dostepnych pracownikow! Brakuje: "<<-(all_workers-demand_workers-nowyBudynek->getDemandWorkers())<<" robotnikow!"<<RESET<<endl;
        
    }else{
        f_logisyka.updateBudynek(nowyBudynek.get());
        demand_workers+=nowyBudynek->getDemandWorkers();
        addBuilding(move(nowyBudynek));
    }
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
    case TypBudynku::PRODUCER:
        nowyBudynek=make_unique<Producer>();
        break;
    }
    demand_workers+=nowyBudynek->getDemandWorkers();
    addBuilding(move(nowyBudynek));
}

void Colony::zburzBudynek(int nr){
    if(nr>=0 && nr < buildings.size()){
        string dec;
        cout<<YELLOW<<"Czy na pewno chcesz wyburzyc budynek: "<<buildings[nr]->getName()<<RESET<<endl;
        cout<<YELLOW<<">>Potwierdz wpisujac TAK, albo anuluj NIE."<<RESET<<endl;
        cin>>dec;
        if(dec=="TAK"||dec=="tak"||dec=="Tak"){
            if(buildings[nr]->getTyp()==TypBudynku::HOUSING){
                if(all_workers-buildings[nr]->getResidents()<demand_workers){
                    cout<<"Niemozliwe jest zburzenie budynku: "<<buildings[nr]->getName()<<", poniewaz bedzei wtedy brakowalo "<<(demand_workers-all_workers+buildings[nr]->getResidents())<<" pracownikow."<<endl;
                    return;
                }

            }else{
                cout<<">>Budynek "<<buildings[nr]->getName()<<" zostal wyburzony."<<endl;
                demand_workers-=buildings[nr]->getDemandWorkers();
                if(static_cast<int>(buildings[nr]->getTyp())==static_cast<int>(TypBudynku::HOUSING)){
                    all_workers-=buildings[nr]->getResidents();
                }
                f_logisyka.updateZburzBudynek(buildings[nr].get());
                buildings.erase(buildings.begin()+nr);
            }
        }else{
            cout<<"Anulowano wyburzanie budynku."<<endl;
    }
    }else{
        cout<<RED<<"Blad: Nie ma budynku o takim ID: "<<nr<<RESET<<endl;
        cout<<endl;
    }
    
}


int Colony::getIloscBudynkow(){
    return buildings.size();
}

void Colony::nextRound(){
    string decyzja;
    cout<<YELLOW<<">>Czy na pewno chcesz przejsc do kolejnej tury? (TAK / NIE)"<<RESET<<endl;
    cin>>decyzja;
    if(decyzja=="TAK"||decyzja=="tak"||decyzja=="Tak"){
        cout<<YELLOW<<">>Rozpoczynanie procedury przejscia do kolejnej rundy..."<<RESET<<endl;
        cout<<endl;
        if(f_logisyka.nextRound(buildings)){
            cout<<GREEN<<BOLD<<"Udalo sie przejsc do kolejnej rundy!"<<RESET<<endl;
            tura++;
            cout<<endl;
        }else{
            cout<<RED<<BOLD<<">>>>>>> KONIEC GRY!! <<<<<<<"<<endl;
            cout<<">>>>>>> PRZEGRANA!! <<<<<<<"<<RESET<<endl;
        }
    }else{
        cout<<YELLOW<<"Anulowano przejscie do kolejnej rundy."<<RESET<<endl;
    }
}

void Colony::update(){}

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
                case TypBudynku::PRODUCER: {
                    double s;
                    double ti;
                    plik >> w_ptype  >> s>>ti;
                
                    auto producer = make_unique<Producer>(w_n, k, s, static_cast<TypProducer>(w_ptype), w,ti);

                    producer->setId(w_id); 
                    
                    nowyBudynek = move(producer);
                    demand_workers+=nowyBudynek->getDemandWorkers();
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
