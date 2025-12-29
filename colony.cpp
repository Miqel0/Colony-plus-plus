#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

using namespace std;

#include "colony.h"
#include "farm.h"
#include "energy.h"
#include "housing.h"
#include "producer.h"


Colony::Colony(){}

void Colony::setNazwa(){f_logisyka.setNazwa();}

void Colony::prnt(){f_logisyka.prnt();}


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
    //cout<<YELLOW<<"Dodano nowy budynek: "<<BOLD<<b->getName()<<RESET<<YELLOW<<"!!"<<RESET<<endl;
    buildings.push_back(move(b));
    //cout<<endl;
}


bool Colony::zbudujBudynek(TypEnergy typ){
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
    if(f_logisyka.getAWorkers()-f_logisyka.getDWorkers()-nowyBudynek->getDemandWorkers()<0){
        cout<<endl;
        cout<<RED<<"Nie mozliwe jest zbudowanie budynku, za malo dostepnych pracownikow! "<<BOLD<<"Brakuje: "<<-(f_logisyka.getAWorkers()-f_logisyka.getAWorkers()-nowyBudynek->getDemandWorkers())<<" robotnikow!"<<RESET<<endl;
        cout<<endl;
        return false;
        
    }else{
        f_logisyka.updateBudynek(nowyBudynek.get());
        f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
        cout<<YELLOW<<"Dodano nowy budynek: "<<BOLD<<nowyBudynek->getName()<<RESET<<YELLOW<<"!!"<<RESET<<endl;
        addBuilding(move(nowyBudynek));
        return true;
    }
}

bool Colony::zbudujBudynek(TypFarm typ){
    unique_ptr<Building> nowyBudynek;
        
    switch (typ){
    case TypFarm::NIEZNANY:
        nowyBudynek=make_unique<Farm>();
        break;
    case TypFarm::POLE:
        nowyBudynek=make_unique<Farm>("Pole", 3, 4,TypFarm::POLE,1,1,0);
        break;
    case TypFarm::SZKLARNIA:
        nowyBudynek=make_unique<Farm>("Szklarnia", 4, 3,TypFarm::SZKLARNIA,1,2,0);
        break;
    }
    if(f_logisyka.getAWorkers()-f_logisyka.getDWorkers()-nowyBudynek->getDemandWorkers()<0){
        cout<<endl;
        cout<<RED<<"Nie mozliwe jest zbudowanie budynku, za malo dostepnych pracownikow! "<<BOLD<<"Brakuje: "<<-(f_logisyka.getAWorkers()-f_logisyka.getAWorkers()-nowyBudynek->getDemandWorkers())<<" robotnikow!"<<RESET<<endl;
        cout<<endl;
        return false;
        
    }else{
        f_logisyka.updateBudynek(nowyBudynek.get());
        f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
        cout<<YELLOW<<"Dodano nowy budynek: "<<BOLD<<nowyBudynek->getName()<<RESET<<YELLOW<<"!!"<<RESET<<endl;
        addBuilding(move(nowyBudynek));
        return true;
    }
}


bool Colony::zbudujBudynek(TypDomy typ){
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
    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
    cout<<YELLOW<<"Dodano nowy budynek: "<<BOLD<<nowyBudynek->getName()<<RESET<<YELLOW<<"!!"<<RESET<<endl;
    addBuilding(move(nowyBudynek));
    return true;
}



bool Colony::zbudujBudynek(TypProducer typ){
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
    if(f_logisyka.getAWorkers()-f_logisyka.getDWorkers()-nowyBudynek->getDemandWorkers()<0){
        cout<<endl;
        cout<<RED<<"Nie mozliwe jest zbudowanie budynku, za malo dostepnych pracownikow! "<<BOLD<<"Brakuje: "<<-(f_logisyka.getAWorkers()-f_logisyka.getAWorkers()-nowyBudynek->getDemandWorkers())<<" robotnikow!"<<RESET<<endl;
        cout<<endl;
        return false;
    }else{
        f_logisyka.updateBudynek(nowyBudynek.get());
        f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
        cout<<YELLOW<<"Dodano nowy budynek: "<<BOLD<<nowyBudynek->getName()<<RESET<<YELLOW<<"!!"<<RESET<<endl;
        addBuilding(move(nowyBudynek));
        return true;
    }
}

bool Colony::zbudujBudynek(TypBudynku typ){//tylko do celu testowania

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
    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
    cout<<YELLOW<<"Dodano nowy budynek: "<<BOLD<<nowyBudynek->getName()<<RESET<<YELLOW<<"!!"<<RESET<<endl;
    addBuilding(move(nowyBudynek));
    return true;
}

void Colony::zburzBudynek(int nr){
    if(nr>=0 && nr < buildings.size()){
        string dec;
        cout<<YELLOW<<"Czy na pewno chcesz wyburzyc budynek: "<<buildings[nr]->getName()<<RESET<<endl;
        cout<<YELLOW<<">>Potwierdz wpisujac TAK, albo anuluj NIE."<<RESET<<endl;
        cin>>dec;
        if(dec=="TAK"||dec=="tak"||dec=="Tak"){
            if(buildings[nr]->getTyp()==TypBudynku::HOUSING){
                if(f_logisyka.getAWorkers()-buildings[nr]->getResidents()<f_logisyka.getDWorkers()){
                    cout<<"Niemozliwe jest zburzenie budynku: "<<buildings[nr]->getName()<<", poniewaz bedzei wtedy brakowalo "<<(f_logisyka.getDWorkers()-f_logisyka.getAWorkers()+buildings[nr]->getResidents())<<" pracownikow."<<endl;
                    return;
                }

            }else{
                cout<<YELLOW<<">>Budynek "<<buildings[nr]->getName()<<" zostal wyburzony."<<RESET<<endl;
                f_logisyka.setDWorkers(-buildings[nr]->getDemandWorkers());
                if(static_cast<int>(buildings[nr]->getTyp())==static_cast<int>(TypBudynku::HOUSING)){
                    f_logisyka.setAWorkers(-buildings[nr]->getResidents());
                }
                f_logisyka.updateZburzBudynek(buildings[nr].get());
                buildings.erase(buildings.begin()+nr);
            }
        }else{
            cout<<YELLOW<<"Anulowano wyburzanie budynku."<<RESET<<endl;
    }
    }else{
        cout<<RED<<"Blad: Nie ma budynku o takim ID: "<<nr<<RESET<<endl;
        cout<<endl;
    }
    
}



bool Colony::nextRound(){return f_logisyka.czyNextRound(buildings);}

void Colony::update(){}

void Colony::save(){
    saveBuildings("test_buildings.txt");
    saveColony("test_colony.txt");
    cout<<YELLOW<<"Gra zostala zapisana do pliku ."<<RESET<<endl;
}

void Colony::load(){
    loadBuildings("test_buildings.txt");
    loadColony("test_colony.txt");
    cout<<CLEAR_SCREEN<<endl;
    cout<<YELLOW<<"Gra zostala wczytana z pliku."<<RESET<<endl;
}

void Colony::saveColony(string nazwa_plik){f_logisyka.save(nazwa_plik);}


void Colony::saveBuildings(string nazwa_plik){
    ofstream plik(nazwa_plik);
    if(plik.is_open()){
         for(const auto& b : buildings){
            b->save(plik);
        }
        plik.close();
    }

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
                    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
                    //nowyBudynek->prnt();

                    break; 
                }

                case TypBudynku::FARM: {
                    double f;
                    int tim,ct;
                    plik >> w_ptype  >> f>>tim>>ct;
                
                    auto farm = make_unique<Farm>(w_n, k, f, static_cast<TypFarm>(w_ptype), w,tim,ct);

                    farm->setId(w_id); 
                    
                    nowyBudynek = move(farm);
                    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
                    //nowyBudynek->prnt();

                    break;
                }
                case TypBudynku::HOUSING:{
                    int r;
                    plik >> w_ptype >> r;
                
                    auto housing = make_unique<Housing>(w_n, k, r, static_cast<TypDomy>(w_ptype), w);
                
                    housing->setId(w_id); 
                    
                    nowyBudynek = move(housing);
                    
                    f_logisyka.setAWorkers(nowyBudynek->getResidents());
                    //nowyBudynek->prnt();

                    break;
                
                }
                case TypBudynku::PRODUCER: {
                    double s;
                    double ti;
                    plik >> w_ptype  >> s>>ti;
                
                    auto producer = make_unique<Producer>(w_n, k, s, static_cast<TypProducer>(w_ptype), w,ti);

                    producer->setId(w_id); 
                    
                    nowyBudynek = move(producer);
                    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
                    //nowyBudynek->prnt();

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
        plik.close();
    } 
} 

void Colony::loadColony(string nazwa_plik){f_logisyka.load(nazwa_plik);}

int Colony::getIloscBudynkow()const{return buildings.size();}
int Colony::getAllWorkers()const{return f_logisyka.getAWorkers();}
int Colony::getDemandWorkers()const{return f_logisyka.getDWorkers();}
int Colony:: getRuch()const{return f_logisyka.getRuch();}

void Colony::setRuch(int r){f_logisyka.setRuch(r);}