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
        f_logisyka.setDWorkers(b->getDemandWorkers());
        
        cout << YELLOW << "Dodano nowy budynek: " << BOLD << b->getName() << RESET << YELLOW << "!!" << RESET << endl;
        addBuilding(move(b));
        return true; 
    }
    else {
        return false; 
    }
}

bool Colony::zbudujBudynek(TypEnergy typ){
    unique_ptr<Building> nowyBudynek = nullptr;
    switch (typ){
        case TypEnergy::NIEZNANY: 
            nowyBudynek = make_unique<Energy>(); 
            break;
        // case TypEnergy::PANELE: 
        //     nowyBudynek = make_unique<Energy>("Panele_sloneczne", 0, 10.0, 5.0, 50.0, TypEnergy::PANELE, 3); 
        //     break;
        // case TypEnergy::WIATRAK: 
        //     nowyBudynek = make_unique<Energy>("Wiatrak", 0, 20.0, 2.0, 20.0, TypEnergy::WIATRAK, 5); 
        //     break;
        case TypEnergy::MALY_WIATRAK: 
            // Konstruktor: (Nazwa, ReqEnergy, KosztKamien, KosztTytan, GenEnergy, Typ, Workers)

            nowyBudynek = make_unique<Energy>("Maly_Wiatrak", 0.0, 15.0, 0.0, 15.0, TypEnergy::MALY_WIATRAK, 1); 
            break;
        case TypEnergy::DUZY_PANEL: 
            nowyBudynek = make_unique<Energy>("Duzy_Panel", 0.0, 50.0, 10.0, 40.0, TypEnergy::DUZY_PANEL, 2); 
            break;
        case TypEnergy::REAKTOR_JADROWY: 
            nowyBudynek = make_unique<Energy>("Reaktor_Jadrowy", 0.0, 200.0, 100.0, 150.0, TypEnergy::REAKTOR_JADROWY, 5); 
            break;
        case TypEnergy::FUZJA_ZIMNA: 
            nowyBudynek = make_unique<Energy>("Fuzja_Zimna", 0.0, 500.0, 300.0, 500.0, TypEnergy::FUZJA_ZIMNA, 10); 
            break;
        default: return false;
    }
    return buduj(move(nowyBudynek));
}

bool Colony::zbudujBudynek(TypFarm typ){
    unique_ptr<Building> nowyBudynek;
    switch (typ){
        case TypFarm::NIEZNANY: 
            nowyBudynek = make_unique<Farm>(); 
            break;
        // case TypFarm::POLE: 
        //     nowyBudynek = make_unique<Farm>("Pole", 3, 5.0, 0.0, 4, TypFarm::POLE, 1, 1, 0); 
        //     break;
        // case TypFarm::SZKLARNIA: 
        //     nowyBudynek = make_unique<Farm>("Szklarnia", 4, 10.0, 5.0, 3, TypFarm::SZKLARNIA, 1, 2, 0); 
        //     break;
        case TypFarm::POLE_ZIEMNIAKOW: 
            // Konstruktor: (Nazwa, ReqEnergy, KosztKamien, KosztTytan, GenFood, Typ, Workers, Time, Lvl)
            nowyBudynek = make_unique<Farm>("Pole_Ziemniakow", 2.0, 10.0, 0.0, 10.0, TypFarm::POLE_ZIEMNIAKOW, 2, 1, 0); 
            break;
        case TypFarm::SZKLARNIA_HYDRO: 
            nowyBudynek = make_unique<Farm>("Szklarnia_Hydro", 10.0, 40.0, 10.0, 60.0, TypFarm::SZKLARNIA_HYDRO, 3, 2, 2); 
            break;
        case TypFarm::FARMA_ALG: 
            nowyBudynek = make_unique<Farm>("Farma_Alg", 30.0, 150.0, 50.0, 120.0, TypFarm::FARMA_ALG, 5, 3, 4); 
            break;
        case TypFarm::SYNTEZATOR_BIALKA: 
            nowyBudynek = make_unique<Farm>("Syntezator_Bialka", 100.0, 300.0, 150.0, 400.0, TypFarm::SYNTEZATOR_BIALKA, 2, 4, 6); 
            break;
        default: return false;
    }
   
    return buduj(move(nowyBudynek));
}


bool Colony::zbudujBudynek(TypDomy typ){
    unique_ptr<Building> nowyBudynek;
    switch (typ){
        case TypDomy::NIEZNANY:
            nowyBudynek=make_unique<Housing>();
            break;
        // case TypDomy::BARAK:
        //     nowyBudynek=make_unique<Housing>("Barak", 0, 15.0, 0.0, 5, TypDomy::BARAK, 0);
        //     break;
        // case TypDomy::REZYDENCJA:
        //     nowyBudynek=make_unique<Housing>("Rezydencja", 0, 30.0, 10.0, 8, TypDomy::REZYDENCJA, 0);
        //     break;
        case TypDomy::BARAK_ROBOTNICZY:
            // Konstruktor: (Nazwa, ReqEnergy, KosztKamien, KosztTytan, Residents, Typ, Workers)
            nowyBudynek = make_unique<Housing>("Barak_Robotniczy", 0.0, 20.0, 0.0, 4, TypDomy::BARAK_ROBOTNICZY, 0);
            break;
        case TypDomy::KWATERY_ZALOGI:
            nowyBudynek = make_unique<Housing>("Kwatery_Zalogi", 5.0, 60.0, 20.0, 10, TypDomy::KWATERY_ZALOGI, 2);
            break;
        case TypDomy::KOPULA_MIESZKALNA:
            nowyBudynek = make_unique<Housing>("Kopula_Mieszkalna", 50.0, 250.0, 100.0, 40, TypDomy::KOPULA_MIESZKALNA, 4);
            break;
        case TypDomy::METROPOLIA:
            nowyBudynek = make_unique<Housing>("Metropolia", 200.0, 800.0, 400.0, 100, TypDomy::METROPOLIA, 7);
            break;
        default:
            return false;
    }
    if(czyStac(nowyBudynek)) { 
         f_logisyka.setStone(f_logisyka.getStone() - nowyBudynek->getKosztKamien());
         f_logisyka.setTitan(f_logisyka.getTitan() - nowyBudynek->getKosztTytan());
         f_logisyka.updateBudynek(nowyBudynek.get());
         f_logisyka.setAWorkers(nowyBudynek->getResidents());
         cout << YELLOW << "Zbudowano dom: " << nowyBudynek->getName() << endl;
         addBuilding(move(nowyBudynek));
         return true;
    }
    return false;
}



bool Colony::zbudujBudynek(TypProducer typ){
    unique_ptr<Building> nowyBudynek;
    switch (typ){
        case TypProducer::NIEZNANY:
            nowyBudynek=make_unique<Producer>();
            break;
        // case TypProducer::KOPALNIA_KAMIENIA:
        //     nowyBudynek=make_unique<Producer>("Kopalnia_kamienia", 3, 20.0, 5.0, 5, TypProducer::KOPALNIA_KAMIENIA, 4, 0);
        //     break;
        // case TypProducer::KOPALNIA_TYTANU:
        //     nowyBudynek=make_unique<Producer>("Kopalnia_tytanu", 4, 30.0, 0, 0, TypProducer::KOPALNIA_TYTANU, 4, 5);
        //     break;
        // case TypProducer::ZAAWANSOWANA_KOPALNIA:
        //     nowyBudynek=make_unique<Producer>("Zaawansowana_kopalnia", 4, 50.0, 50.0, 5, TypProducer::ZAAWANSOWANA_KOPALNIA, 6, 5);
        //     break;
        case TypProducer::ODKRYWKA_KAMIENIA:
            // Konstruktor: (Nazwa, ReqEnergy, KosztKamien, KosztTytan, GenStone, Typ, Workers, GenTitan)
            nowyBudynek = make_unique<Producer>("Odkrywka_Kamienia", 3.0, 20.0, 0.0, 8.0, TypProducer::ODKRYWKA_KAMIENIA, 4, 0.0);
            break;
        case TypProducer::WIERTLO_GLEBINOWE:
            nowyBudynek = make_unique<Producer>("Wiertlo_Glebinowe", 10.0, 50.0, 20.0, 10.0, TypProducer::WIERTLO_GLEBINOWE, 6, 5.0);
            break;
        case TypProducer::KOMBINAT_GORNICZY:
            nowyBudynek = make_unique<Producer>("Kombinat_Gorniczy", 40.0, 200.0, 100.0, 30.0, TypProducer::KOMBINAT_GORNICZY, 15, 15.0);
            break;
        case TypProducer::AUTOMAT_WYDOBYWCZY:
            nowyBudynek = make_unique<Producer>("Automat_Wydobywczy", 150.0, 600.0, 300.0, 80.0, TypProducer::AUTOMAT_WYDOBYWCZY, 2, 40.0);
            break;
        default:
            return false;
    }
    return buduj(move(nowyBudynek));
}

bool Colony::zbudujBudynek(TypTerr typ){
    unique_ptr<Building> nowyBudynek = nullptr;
    switch (typ){
        case TypTerr::NIEZNANY: 
            nowyBudynek = make_unique<Terr>(); 
            break;
        // case TypTerr::cos1: 
        //     nowyBudynek = make_unique<Terr>("Panele_sloneczne", 0, 10.0, 5.0, 50.0, TypTerr::cos1, 3); 
        //     break;
        // case TypTerr::cos2: 
        //     nowyBudynek = make_unique<Terr>("Wiatrak", 0, 20.0, 2.0, 20.0, TypTerr::cos2, 5); 
        //     break;
        case TypTerr::STACJA_BADAWCZA: 
            // Konstruktor: (Nazwa, ReqEnergy, KosztKamien, KosztTytan, GenTerr, Typ, Workers)
            nowyBudynek = make_unique<Terr>("Stacja_Badawcza", 5.0, 40.0, 0.0, 2.0, TypTerr::STACJA_BADAWCZA, 2); 
            break;
        case TypTerr::KOMINY_CIEPLNE: 
            nowyBudynek = make_unique<Terr>("Kominy_Cieplne", 20.0, 120.0, 30.0, 8.0, TypTerr::KOMINY_CIEPLNE, 5); 
            break;
        case TypTerr::GENERATOR_O2: 
            nowyBudynek = make_unique<Terr>("Generator_O2", 100.0, 400.0, 200.0, 25.0, TypTerr::GENERATOR_O2, 10); 
            break;
        case TypTerr::LUSTRA_ORBITALNE: 
            nowyBudynek = make_unique<Terr>("Lustra_Orbitalne", 500.0, 1500.0, 1000.0, 100.0, TypTerr::LUSTRA_ORBITALNE, 20); 
            break;
        default: return false;
    }
    return buduj(move(nowyBudynek));
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
    default:
        return false;
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
