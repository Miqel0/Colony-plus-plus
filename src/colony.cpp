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

// ==========================================
// KONSTRUKTOR
// ==========================================
Colony::Colony(){}

// ==========================================
// WYSWIETLANIE
// ==========================================

//WYSWIETLANIE INFORMACJI Z COLONY / LOGISTICS - colony
void Colony::prnt(){f_logisyka.prnt();}

//WYSWIETLANIE BUDYNKU O DANYM ID
void Colony::prntBuilding(int nr){
    if(nr>=0 && nr <= buildings.size()){
        buildings[nr]->prnt(getIlosc(buildings[nr]->getName()));
    }else{
        cout<<RED<<"Blad: Nie ma budynku o takim ID: "<<RESET<<nr<<endl;
        cout<<endl;
    }
}
/**
 * @brief Wyświetlanie budynku o danej nazwie @param 
 * 
 * @param nazwa budynku
 */

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

//WYSWIETLANIE WSZYSTKICH BUDYNKOW - caly wektor buildings z parametrami
void Colony::prntBuildings(){
    if(buildings.size()==0){
        cout<<endl;
        cout<<YELLOW<<BOLD<<"                      BRAK ZBUDOWANYCH BUDYNKOW"<<endl;
    }else{
        for(int i=0;i<buildings.size();i++){
        prntBuilding(i);
        }
    }
}

//WYSWIETLANIE WSZYSTKICH BUDYNKOW - caly wektor buildings SAME NAZWY
void Colony::prntBuildingsShort(){
    prntHeader("Obecnie zbudowane budynki:");
    if(buildings.size()==0){
        cout<<endl;
        cout<<YELLOW<<BOLD<<"                      BRAK ZBUDOWANYCH BUDYNKOW"<<endl;
        
    }else{
        for(int i=0;i<buildings.size();i++){
            string str=cleanNum(i)+". ";
            cout<<YELLOW<<BOLD<<left<<setw(5)<<str<<RESET<<buildings[i]->getName()<<endl;
        }
    }
}

//WYSWIETLANIE WSZYSTKICH BUDYNKOW PODSUMOWANIE - ILOSC BUDYNKOW DANEGO TYPU - show
void Colony::prntBuildingsSumm(){
    prntHeader("Obecnie zbudowane budynki");
    if(buildings.size()==0){
        cout<<endl;
        cout<<YELLOW<<BOLD<<"                      BRAK ZBUDOWANYCH BUDYNKOW"<<endl;
        return;
    }
    map<string,int> licznik;//Robienie mapy z nazwy budynku i ilosci - w wketorze buildings

    for(auto const &b: buildings){
        licznik[b->getName()]++;
    }
    const int w_n=33;
    const int w_i=34;
    const string sep=" | ";
    cout<<BLUE<<BOLD<<left<<setw(w_n)<<"NAZWA BUDYNKU"<<NO_BOLD<<YELLOW<<sep<<BOLD<<BLUE<<setw(w_i)<<"ILOSC"<<RESET<<endl;
    cout << YELLOW << string(w_n, '-') << "-+-" << string(w_i, '-') << RESET << endl;
    for(auto const &[nazwa,ilosc]:licznik){//Wyswietlanie budynkow: nazwa - ilosc
        cout<<YELLOW<<left<<setw(w_n)<<nazwa<<sep<<BOLD<<ilosc<<RESET<<endl;
    }
    cout<<endl;
}

map<string,int> Colony::UIprntBuildingsSumm()const{
    
    map<string,int> licznik;//Robienie mapy z nazwy budynku i ilosci - w wketorze buildings
    if(buildings.size()==0){
        //FIXME - dobre rzutawnie bledu czy cos
        cout<<endl;
        //cout<<YELLOW<<BOLD<<"                      BRAK ZBUDOWANYCH BUDYNKOW"<<endl;
        return licznik;
    }
    for(auto const &b: buildings){
        licznik[b->getName()]++;
    }

    return licznik;
}

/**
 * @brief Wyświetlanie budynku o danej nazwie @param 
 * 
 * @param nazwa budynku
 */

void Colony::UIprntBuilding(string bud) const{
    int nr=-1;
    for(int i=0;i<buildings.size();i++){
        string nazwa=buildings[i]->getName();
        for(auto &c : nazwa) c = tolower(c);
        if(buildings[i]->getName()==bud){
            nr=i;
        }
    }
    if(nr>=0 && nr <= buildings.size()){
        buildings[nr]->UIprnt(getIlosc(buildings[nr]->getName()));
    }else{
        cout<<RED<<"Blad: Nie ma budynku o takiej nazwie: "<<RESET<<nr<<endl;
        cout<<endl;
    }
}


// ==========================================
// BUDOWANIE
// ==========================================

// //Budowanie Energy
// bool Colony::zbudujEnergy(string n, double kE,double kK, double kT, double e,TypEnergy t,int w){
//     unique_ptr<Building> nowyBudynek = make_unique<Energy>(n,kE,kK,kT,e,t,w);
//     return buduj(move(nowyBudynek));
// }

// //Budowanie Farm
// bool Colony::zbudujFarm(string n, double kE,double kK, double kT, double f,TypFarm t,int w,int tim,int ct){
//     unique_ptr<Building> nowyBudynek = make_unique<Farm>(n,kE,kK,kT,f,t,w,tim,ct); 
//     return buduj(move(nowyBudynek));
// }

// //Budowanie Housing
// bool Colony::zbudujHousing(string n, double kE,double kK, double kT, int r,TypDomy t,int w){
//     unique_ptr<Building> nowyBudynek=make_unique<Housing>(n,kE,kK,kT,r,t,w);
//     return buduj(move(nowyBudynek));
// }

// //Budowanie Prodcuer
// bool Colony::zbudujProducer(string n, double kE,double kK, double kT, double s,TypProducer t,int w,double ti){
//     unique_ptr<Building> nowyBudynek=make_unique<Producer>(n,kE,kK,kT,s,t,w,ti);
//     return buduj(move(nowyBudynek));
// }

// //Budowanie Terr
// bool Colony::zbudujTerr(string n, double kE,double kK, double kT, double te,TypTerr t,int w){
//     unique_ptr<Building> nowyBudynek = make_unique<Terr>(n,kE,kK,kT,te,t,w); 
//     return buduj(move(nowyBudynek));
// }

//Budowanie budynku i aktalziowanie parametrow
bool Colony::buduj(unique_ptr<Building> b){
    if (b == nullptr) return false;
    
    if(czyStac(b)){
        
        //Odejmowanie materalow, ktore sa wykorzystane do budowania
        f_logisyka.setStone(f_logisyka.getStone() - b->getKosztKamien());
        f_logisyka.setTitan(f_logisyka.getTitan() - b->getKosztTytan());

        //Aktualizowanie logistyki
        f_logisyka.updateBudynek(b.get());
        
        if(b->getTyp()==TypBudynku::HOUSING){
            f_logisyka.setAWorkers(b->getResidents());
        }else{
            f_logisyka.setDWorkers(b->getDemandWorkers());
        }
        cout << YELLOW << "Dodano nowy budynek: " << BOLD << b->getName() << RESET << YELLOW << "!!" << RESET << endl;
        
        //Dodawanie ostatecznie danego budynka do listy budynkow
        addBuilding(move(b));
        return true; 
    }
    else {
        return false; 
    }
}

//Funkcja pomocniczna do budowania
void Colony::addBuilding(unique_ptr<Building> b){buildings.push_back(move(b));}

//Sprawdzanie wymaganych parametrow do zbudowania
bool Colony::czyStac(const unique_ptr<Building> &b)const{
    int czy=0;
    if(b->getKosztKamien()<=f_logisyka.getStone()&&b->getKosztTytan()<=f_logisyka.getTitan()){ // sprwdza czy jest wystarczajaca liczba kamienia i tytanu do zbudowania
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
    if(f_logisyka.getAWorkers()-f_logisyka.getDWorkers()-b->getDemandWorkers()<0){ // sprawdza czy jest wystarczajaca liczba workerow - zeby wybudowac dany budynek
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

//Wyburzanie budynku
void Colony::zburzBudynek(string nazwa){

    string nam="X";
    int nr=0;
    for(int i=0;i<buildings.size();i++){//Szukanie dowolnego budynku o danej nazwie (nie ma znaczenia ktorego sie usunie, bo i tak wyswietla sie ich suma)
        string bud=buildings[i]->getName();
        for(auto &c : bud) c = tolower(c);
        
        if(bud==nazwa){
            nam=buildings[i]->getName();
            nr=i;
        }
    }

    if(nam!="X"){//Czy nazwa zostala zmieniona?
        string dec;
        cout<<YELLOW<<"Czy na pewno chcesz wyburzyc budynek: "<<nam<<RESET<<endl;
        cout<<YELLOW<<">>Potwierdz wpisujac y, albo anuluj n."<<RESET<<endl;
        cin>>dec;
        if(dec=="y"||dec=="yes"||dec=="tak"||dec=="t"){
            if(buildings[nr]->getTyp()==TypBudynku::HOUSING){
                if(f_logisyka.getAWorkers()-buildings[nr]->getResidents()<f_logisyka.getDWorkers()){ //Sprawdzenie czy aby na pewno mozna usunac dany budynek - nie moze brakowac pracownikow!
                    cout<<"Niemozliwe jest zburzenie budynku: "<<buildings[nr]->getName()<<", poniewaz bedzei wtedy brakowalo "<<(f_logisyka.getDWorkers()-f_logisyka.getAWorkers()+buildings[nr]->getResidents())<<" pracownikow."<<endl;
                    return;
                }
            }else{//Wszystko pasuje, wiec mozna zburzyc
                cout<<YELLOW<<">>Budynek "<<buildings[nr]->getName()<<" zostal wyburzony."<<RESET<<endl;
                
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


// ==========================================
// SAVE / LOAD
// ==========================================

//Zapisywanie wszystkiego - save
void Colony::save(){
    saveBuildings("zapis_buildings.txt");
    saveColony("zapis_colony.txt");
    cout<<YELLOW<<"Gra zostala zapisana do pliku ."<<RESET<<endl;
}

//Zapisywanie danych z Logistics - parametry
void Colony::saveColony(string nazwa_plik){f_logisyka.save(nazwa_plik);}

//Zapisywanie wszystkich budynkow
void Colony::saveBuildings(string nazwa_plik){
    ofstream plik("data/"+nazwa_plik);
    if(plik.is_open()){
         for(const auto& b : buildings){
            b->save(plik); //Wykorzystanie przeciazonej funkcji virtualnej save()
        }
        plik.close();
    }

}

//Wczytywanie wszystkiego - load
void Colony::load(){
    loadBuildings("zapis_buildings.txt");
    loadColony("zapis_colony.txt");
    cout<<CLEAR_SCREEN<<endl;
    cout<<YELLOW<<"Gra zostala wczytana z pliku."<<RESET<<endl;
}

//Wczytywanie wszystkich budynkow
void Colony::loadBuildings(string nazwa_plik) {
    ifstream plik("data/"+nazwa_plik);

    if (plik.is_open()) {

        buildings.clear();

        //Parametry
        int w_type, w_id, w, maxSaved = 0;
        string w_n;
        double kE,kT,kK;
        //Format linii w pliku: TypBudynku Nazwa ID KosztEnergii KosztKamienia KosztTytanu Pracownicy ParametrySpecjalne
        //Przejscie przez plik, linijka po linijce
        while (plik >> w_type >> w_n >> w_id >> kE>>kK>>kT >> w) {
            
            TypBudynku typ = static_cast<TypBudynku>(w_type);
            unique_ptr<Building> nowyBudynek = nullptr;

            //Tworzenie konkretnego budynku na podstawie wczytynych danych, w zaleznosci od typu budynku 
            switch (typ) {
                case TypBudynku::NIEZNANY:
                    break;

                case TypBudynku::ENERGY: {
                    //Dodatkowe parametry
                    double e;
                    plik >> e;
                    
                    auto energia = make_unique<Energy>(w_n, kE,kK,kT, e, w);
                    
                    energia->setId(w_id); //Ustawianie poprawnego ID (bo psuje sie podczas wczytywania
                    nowyBudynek = move(energia);
                    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
                    break; 
                }

                case TypBudynku::FARM: {
                    //Dodatkowe parametry
                    double f;
                    int tim,ct;
                    plik >> f>>tim>>ct;
                
                    auto farm = make_unique<Farm>(w_n, kE,kK,kT, f, w,tim,ct);

                    farm->setId(w_id); 
                    nowyBudynek = move(farm);
                    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
                    break;
                }
                case TypBudynku::HOUSING:{
                    //Dodatkowe parametry
                    int r;
                    plik >> r;
    
                    auto housing = make_unique<Housing>(w_n, kE,kK,kT, r, w);
                
                    housing->setId(w_id);                
                    nowyBudynek = move(housing);
                    f_logisyka.setAWorkers(nowyBudynek->getResidents());
                    break;
                }
                case TypBudynku::PRODUCER: {
                    //Dodatkowe parametry
                    double s, ti;
                    plik >> s>>ti;
                
                    auto producer = make_unique<Producer>(w_n, kE,kK,kT, s, w,ti);

                    producer->setId(w_id); 
                    nowyBudynek = move(producer);
                    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
                    break;
                }
                case TypBudynku::TERR: {
                    //Dodatkowe parametry
                    double te;
                    plik >> te;
                
                    auto terr = make_unique<Terr>(w_n, kE,kK,kT, te, w);

                    terr->setId(w_id); 
                    nowyBudynek = move(terr);
                    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
                    break;
                }
            } 

            //Ustawienie static maxSaved na tyle ile sie wczytalo budynkow
            if (nowyBudynek != nullptr) {
                if (w_id > maxSaved) {
                    maxSaved = w_id;
                }
            }
            
            //Wrzucenie do vectora na koniec ten nowy zbudowany budynek
            buildings.push_back(move(nowyBudynek));
        } 
        Building::updateLicznik(maxSaved);
        plik.close();
    } 
} 

void Colony::loadColony(string nazwa_plik){f_logisyka.load(nazwa_plik);}

// ==========================================
// NEXT ROUND
// ==========================================

//Wywołanie funckji nextRound z Logistics
int Colony::nextRound(){return f_logisyka.czyNextRound(buildings);}

//Sprawdzenie czy istnieje dany budynek
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

bool Colony::sprawdzLvlTerr(){return f_logisyka.sprawdzLvlTerr();}

// ==========================================
// SETTERY
// ==========================================

void Colony::setNazwa(){f_logisyka.setNazwa();}

void Colony::setRuch(int r){f_logisyka.setRuch(r);}

void Colony::setSandbox(){f_logisyka.setSandbox();}
void Colony::setCustom(){f_logisyka.setCustom();}

// ==========================================
// GETTERY
// ==========================================

int Colony::getIloscBudynkow()const{return buildings.size();}
int Colony::getAllWorkers()const{return f_logisyka.getAWorkers();}
int Colony::getDemandWorkers()const{return f_logisyka.getDWorkers();}
int Colony:: getRuch()const{return f_logisyka.getRuch();}
int Colony::getLvlTerr() const{return f_logisyka.getLvlTerr();}
int Colony::getToNextLvlTerr() const{return f_logisyka.getTitan();}
int Colony::getIlosc(string name)const{//Zwracanie ilosci budynku o danej nazwie
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

string Colony::getNazwa() const{return f_logisyka.getNazwa();}
int Colony::getTura() const{return f_logisyka.getTura();}
double Colony::getReqEnergy() const{return f_logisyka.getReqEnergy();}
double Colony::getGenEnergy() const{return f_logisyka.getGenEnergy();}
double Colony::getReqFood() const{return f_logisyka.getReqFood();}
double Colony::getFood() const{return f_logisyka.getFood();}
int Colony::getStone() const{return f_logisyka.getStone();}
int Colony::getTitan() const{return f_logisyka.getTitan();}
