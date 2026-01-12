#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <map>
#include <cctype>

using namespace std;

#include "colony.h"
#include "farm.h"
#include "energy.h"
#include "housing.h"
#include "producer.h"
#include "terr.h"

Colony::Colony(){}

void Colony::setNazwa(){f_logisyka.setNazwa();}

void Colony::prnt(){f_logisyka.prnt();}


void Colony::prntBuilding(int nr){
    if(nr>=0 && nr <= buildings.size()){
        buildings[nr]->prnt(getIlosc(buildings[nr]->getName()));
    }else{
        cout<<RED<<"Blad: Nie ma budynku o takim ID: "<<RESET<<nr<<endl;
        cout<<endl;
    }
    
}

void Colony::prntBuilding(string bud){
    
    int nr=-1;
    for(int i=0;i<buildings.size();i++){
        string nazwa=buildings[i]->getName();
        for(auto &c : nazwa) c = tolower(c);
        if(buildings[i]->getName()==bud){
            nr=i;
        }
    }
    if(nr>=0 && nr <= buildings.size()){
        buildings[nr]->prnt(getIlosc(buildings[nr]->getName()));
    }else{
        cout<<RED<<"Blad: Nie ma budynku o takiej nazwie: "<<RESET<<nr<<endl;
        cout<<endl;
    }
    
}

void Colony::prntBuildings(){
    if(buildings.size()==0){
        cout<<endl;
        cout<<YELLOW<<BOLD<<"                      BRAK ZBUDOWANYCH BUDYNKOW"<<endl;
        
    }else{
        for(int i=0;i<buildings.size();i++){
        prntBuilding(i);
    }}
}


void Colony::prntBuildingsShort(){
    prntHeader("Obecnie zbudowane budynki:");
    if(buildings.size()==0){
        cout<<endl;
        cout<<YELLOW<<BOLD<<"                      BRAK ZBUDOWANYCH BUDYNKOW"<<endl;
        
    }else{
        for(int i=0;i<buildings.size();i++){
            string str=cleanNum(i)+". ";
            cout<<YELLOW<<BOLD<<left<<setw(5)<<str<<RESET<<buildings[i]->getName()<<endl;
    }}
}
void Colony::prntBuildingsSumm(){
    prntHeader("Obecnie zbudowane budynki");
    if(buildings.size()==0){
        cout<<endl;
        cout<<YELLOW<<BOLD<<"                      BRAK ZBUDOWANYCH BUDYNKOW"<<endl;
        return;
    }
    map<string,int> licznik;

    for(auto const &b: buildings){
        licznik[b->getName()]++;
    }
    const int w_n=33;
    const int w_i=34;
    const string sep=" | ";
    cout<<BLUE<<BOLD<<left<<setw(w_n)<<"NAZWA BUDYNKU"<<NO_BOLD<<YELLOW<<sep<<BOLD<<BLUE<<setw(w_i)<<"ILOSC"<<RESET<<endl;
    cout << YELLOW << string(w_n, '-') << "-+-" << string(w_i, '-') << RESET << endl;
    for(auto const &[nazwa,ilosc]:licznik){
        cout<<YELLOW<<left<<setw(w_n)<<nazwa<<sep<<BOLD<<ilosc<<RESET<<endl;
    }
    cout<<endl;
}


void Colony::addBuilding(unique_ptr<Building> b){buildings.push_back(move(b));}

bool Colony::buduj(unique_ptr<Building> b){
    if (b == nullptr) return false;
    
    if(czyStac(b)){
        
        f_logisyka.setStone(f_logisyka.getStone() - b->getKosztKamien());
        f_logisyka.setTitan(f_logisyka.getTitan() - b->getKosztTytan());

        f_logisyka.updateBudynek(b.get());
        
        
        if(b->getTyp()==TypBudynku::HOUSING){
            f_logisyka.setAWorkers(b->getResidents());
        }else{
            f_logisyka.setDWorkers(b->getDemandWorkers());
        }
        cout << YELLOW << "Dodano nowy budynek: " << BOLD << b->getName() << RESET << YELLOW << "!!" << RESET << endl;
        addBuilding(move(b));
        return true; 
    }
    else {
        return false; 
    }
}

bool Colony::zbudujEnergy(string n, double kE,double kK, double kT, double e,TypEnergy t,int w){
    unique_ptr<Building> nowyBudynek = make_unique<Energy>(n,kE,kK,kT,e,t,w);
    return buduj(move(nowyBudynek));
}

bool Colony::zbudujFarm(string n, double kE,double kK, double kT, double f,TypFarm t,int w,int tim,int ct){
    unique_ptr<Building> nowyBudynek = make_unique<Farm>(n,kE,kK,kT,f,t,w,tim,ct); 
    return buduj(move(nowyBudynek));
}


bool Colony::zbudujHousing(string n, double kE,double kK, double kT, int r,TypDomy t,int w){
    unique_ptr<Building> nowyBudynek=make_unique<Housing>(n,kE,kK,kT,r,t,w);
    return buduj(move(nowyBudynek));
}



bool Colony::zbudujProducer(string n, double kE,double kK, double kT, double s,TypProducer t,int w,double ti){
    unique_ptr<Building> nowyBudynek=make_unique<Producer>(n,kE,kK,kT,s,t,w,ti);
    return buduj(move(nowyBudynek));
}

bool Colony::zbudujTerr(string n, double kE,double kK, double kT, double te,TypTerr t,int w){
    unique_ptr<Building> nowyBudynek = make_unique<Terr>(n,kE,kK,kT,te,t,w); 
    return buduj(move(nowyBudynek));
}

void Colony::zburzBudynek(int nr){
    if(nr>=0 && nr < buildings.size()){
        string dec;
        cout<<YELLOW<<"Czy na pewno chcesz wyburzyc budynek: "<<buildings[nr]->getName()<<RESET<<endl;
        cout<<YELLOW<<">>Potwierdz wpisujac y, albo anuluj n."<<RESET<<endl;
        cin>>dec;
        if(dec=="y"||dec=="yes"||dec=="tak"||dec=="t"){
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


void Colony::zburzBudynek(string nazwa){
    nazwa[0]=toupper(nazwa[0]);
    string nam="X";
    int nr=0;
    for(int i=0;i<buildings.size();i++){
        if(buildings[i]->getName()==nazwa){
            nam=buildings[i]->getName();
            nr=i;
        }
    }
    if(nam!="X"){
        string dec;
        cout<<YELLOW<<"Czy na pewno chcesz wyburzyc budynek: "<<nam<<RESET<<endl;
        cout<<YELLOW<<">>Potwierdz wpisujac y, albo anuluj n."<<RESET<<endl;
        cin>>dec;
        if(dec=="y"||dec=="yes"||dec=="tak"||dec=="t"){
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
        cout<<RED<<"Blad: Nie ma budynku o takiej nazwie: "<<BOLD<<nazwa<<endl;
        cout<<endl;
    }
}



int Colony::nextRound(){return f_logisyka.czyNextRound(buildings);}

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
        double kE,kT,kK;


        while (plik >> w_type >> w_n >> w_id >> kE>>kK>>kT >> w) {
            
            TypBudynku typ = static_cast<TypBudynku>(w_type);
            unique_ptr<Building> nowyBudynek = nullptr;
            switch (typ) {
                case TypBudynku::NIEZNANY:

                    break;

                case TypBudynku::ENERGY: {
                    double e;
                    plik >> w_ptype >> e;
                
                    auto energia = make_unique<Energy>(w_n, kE,kK,kT, e, static_cast<TypEnergy>(w_ptype), w);
                
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
                
                    auto farm = make_unique<Farm>(w_n, kE,kK,kT, f, static_cast<TypFarm>(w_ptype), w,tim,ct);

                    farm->setId(w_id); 
                    
                    nowyBudynek = move(farm);
                    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
                    //nowyBudynek->prnt();

                    break;
                }
                case TypBudynku::HOUSING:{
                    int r;
                    plik >> w_ptype >> r;
                
                    auto housing = make_unique<Housing>(w_n, kE,kK,kT, r, static_cast<TypDomy>(w_ptype), w);
                
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
                
                    auto producer = make_unique<Producer>(w_n, kE,kK,kT, s, static_cast<TypProducer>(w_ptype), w,ti);

                    producer->setId(w_id); 
                    
                    nowyBudynek = move(producer);
                    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
                    //nowyBudynek->prnt();

                    break;
                }
                case TypBudynku::TERR: {
                    double te;
                    plik >> w_ptype >> te;
                
                    auto terr = make_unique<Terr>(w_n, kE,kK,kT, te, static_cast<TypTerr>(w_ptype), w);

                    terr->setId(w_id); 
                    
                    nowyBudynek = move(terr);
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
bool Colony::czyBudynek(string bud)const{
    
    for(const auto &b:buildings){
        string nazwa=b->getName();
        for(auto &c : nazwa) c = tolower(c);
        if(nazwa==bud){
            return true;
        }
    }
    return false;
}

void Colony::setSandbox(){f_logisyka.setSandbox();}
void Colony::setCustom(){f_logisyka.setCustom();}


bool Colony::sprawdzLvlTerr(){return f_logisyka.sprawdzLvlTerr();}

bool Colony::czyStac(const unique_ptr<Building> &b)const{
    int czy=0;
    if(b->getKosztKamien()<=f_logisyka.getStone()&&b->getKosztTytan()<=f_logisyka.getTitan()){
        czy=1;
    }else{
        if(b->getKosztKamien()>f_logisyka.getStone()){
            cout<<RED<<"Brakuje "<<BOLD<<b->getKosztKamien()-f_logisyka.getStone()<<NO_BOLD<<" kamienia!"<<RESET<<endl;
        
        }else if(b->getKosztTytan()>f_logisyka.getTitan()){
            cout<<RED<<"Brakuje "<<BOLD<<b->getKosztTytan()-f_logisyka.getTitan()<<NO_BOLD<<" tytanu!"<<RESET<<endl;
        
        }else if(b->getKosztKamien()>f_logisyka.getStone()&&b->getKosztTytan()>f_logisyka.getTitan()){
            cout<<RED<<"Brakuje "<<BOLD<<b->getKosztKamien()-f_logisyka.getStone()<<NO_BOLD<<" kamienia, i "<<b->getKosztTytan()-f_logisyka.getTitan()<<NO_BOLD<<" tytanu!"<<RESET<<endl;

        }
        
        czy=0;
    }
    if(f_logisyka.getAWorkers()-f_logisyka.getDWorkers()-b->getDemandWorkers()<0){
        cout<<endl;
        cout<<RED<<"Nie mozliwe jest zbudowanie budynku, za malo dostepnych pracownikow! "<<BOLD<<"Brakuje: "<<-(f_logisyka.getAWorkers()-f_logisyka.getDWorkers()-b->getDemandWorkers())<<" robotnikow!"<<RESET<<endl;
        cout<<endl;
        czy=0;
    }
    if(czy==1){
        return true;
    }else{
        return false;
    }
}

void Colony::loadColony(string nazwa_plik){f_logisyka.load(nazwa_plik);}

int Colony::getIloscBudynkow()const{return buildings.size();}
int Colony::getAllWorkers()const{return f_logisyka.getAWorkers();}
int Colony::getDemandWorkers()const{return f_logisyka.getDWorkers();}
int Colony:: getRuch()const{return f_logisyka.getRuch();}
int Colony::getLvlTerr() const{return f_logisyka.getLvlTerr();}
int Colony::getToNextLvlTerr() const{return f_logisyka.getTitan();}

int Colony::getIlosc(string name)const{
    if(name.empty()) return 0;
    int il=0;
    name[0]=toupper(name[0]);
    for(auto const &b: buildings){
        if(name==b->getName()){
            il++;
        }
    }
    return il;
}

void Colony::setRuch(int r){f_logisyka.setRuch(r);}
