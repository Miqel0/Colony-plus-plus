#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <map>
#include <cctype>
#include <filesystem>

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


/**
 * @brief Funkcja tworząca mapę z vectora zbudowanych budynków
 * 
 * @return map<string,int> zawiera liczbę zbudowanych kolejnych budynków.
 */
map<string,int> Colony::UIprntBuildingsSumm()const{
    
    map<string,int> licznik;//Robienie mapy z nazwy budynku i ilosci - w wketorze buildings
    if(buildings.size()==0){
        //FIXME - dobre rzutawnie bledu czy cos
        return licznik;
    }
    for(auto const &b: buildings){
        licznik[b->getName()]++;
    }

    return licznik;
}

/**
 * @brief Wyświetlanie parametrów budynku o danej nazwie @param 
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
        cout<<"Blad: Nie ma budynku o takiej nazwie: "<<nr<<endl;
        cout<<endl;
    }
}

/**
 * @brief Funkcja budująca budynek - ostateczna
 * 
 * @param b budynek do zbudowania
 * @return BuildResult wynik budowy
 */
BuildResult Colony::UIbuduj(unique_ptr<Building> b){
    if (b == nullptr) return {false, 0,0,0};
    auto wynik=UIczyStac(b);
    if(wynik.czy){
        
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
        //Dodawanie ostatecznie danego budynka do listy budynkow
        addBuilding(move(b));
    }
    return wynik;
}


/**
 * @brief Funkcja pomocniczna do budowania
 * 
 * @param b Budynek do zbudowania
 */
void Colony::addBuilding(unique_ptr<Building> b){buildings.push_back(move(b));}

/**
 * @brief Sprawdzanie wymaganych parametrow do zbudowania
 * 
 * @param b wskaznik na budowany budynek 
 * @return BuildResult paczka wynikow budowania
 */
BuildResult Colony::UIczyStac(const unique_ptr<Building> &b)const{
    BuildResult wynik ={false,0,0,0,b->getName(),0};
    if(b->getKosztKamien()<=f_logisyka.getStone()&&b->getKosztTytan()<=f_logisyka.getTitan()){ // sprawdza czy jest wystarczajaca liczba kamienia i tytanu do zbudowania
        wynik.czy=true;
    }else{
        if(b->getKosztKamien()>f_logisyka.getStone()){
            wynik.kamien=b->getKosztKamien()-f_logisyka.getStone();
        }else if(b->getKosztTytan()>f_logisyka.getTitan()){
            wynik.tytan=b->getKosztTytan()-f_logisyka.getTitan();
        }
    }
    if(f_logisyka.getAWorkers()-f_logisyka.getDWorkers()-b->getDemandWorkers()<0){ // sprawdza czy jest wystarczajaca liczba workerow - zeby wybudowac dany budynek
        wynik.czy=false;
        wynik.workers=+f_logisyka.getDWorkers()+b->getDemandWorkers()-f_logisyka.getAWorkers();
    }
    return wynik;
}

/**
 * @brief Funckaj burząca budynek i sprawdzająca wymogi
 * 
 * @param nazwa nazwa budynku do zburzenia
 * @return DestroyResult paczka wynikowa
 */
DestroyResult Colony::UIzburzBudynek(string nazwa){
    DestroyResult wynik= {false, 0, nazwa,{0,0}};
    string nazwa_=nazwa;
    string nam="X";
    int nr=0;
    for(int i=0;i<buildings.size();i++){//Szukanie dowolnego budynku o danej nazwie (nie ma znaczenia ktorego sie usunie, bo i tak wyswietla sie ich suma)
        string bud=buildings[i]->getName();
        for(auto &c : bud) c = tolower(c);
        for(auto &c : nazwa_) c = tolower(c);
        if(bud==nazwa_){
            nam=buildings[i]->getName();
            nr=i;
        }
    }

    if(nam!="X"){//Czy nazwa zostala zmieniona?
        if(buildings[nr]->getTyp()==TypBudynku::HOUSING){
            if(f_logisyka.getAWorkers()-buildings[nr]->getResidents()<f_logisyka.getDWorkers()){ //Sprawdzenie czy aby na pewno mozna usunac dany budynek - nie moze brakowac pracownikow!
                wynik.brakLudzi=f_logisyka.getDWorkers()-f_logisyka.getAWorkers()+buildings[nr]->getResidents();
                return wynik;
            }else{
                wynik.czy=true;
            }
        } else{
            wynik.czy=true;
        }
        if(wynik.czy){
            if(static_cast<int>(buildings[nr]->getTyp())==static_cast<int>(TypBudynku::HOUSING)){
                f_logisyka.setAWorkers(-buildings[nr]->getResidents());
            }
            wynik.sur=f_logisyka.UIupdateZburzBudynek(buildings[nr].get());
            buildings.erase(buildings.begin()+nr);  
        }
    }else{
        cout<<"Blad: Nie ma budynku o takiej nazwie: "<<nazwa<<endl;
        cout<<endl;
    }
    return wynik;
}


// ==========================================
// SAVE / LOAD
// ==========================================

/**
 * @brief Zapisywanie wszystkiego - save
 * 
 */
void Colony::save(const string& nazwa_zapisu ){
    filesystem::path sciezkaFolderu = filesystem::path("data/saves") / nazwa_zapisu;
    filesystem::create_directories(sciezkaFolderu);
    saveBuildings("data/saves/"+nazwa_zapisu+"/"+"zapis_buildings.txt");
    saveColony("data/saves/"+nazwa_zapisu+"/"+"zapis_colony.txt");
    cout<<"Gra zostala zapisana do pliku ."<<endl;
}

/**
 * @brief Zapisywanie danych z Logistics - parametry
 * 
 * @param nazwa_plik plik do zapisu parametrów kolonii
 */
void Colony::saveColony(string nazwa_plik){f_logisyka.save(nazwa_plik);}

/**
 * @brief Zapisywanie wszystkich budynków
 * 
 * @param nazwa_plik plik do zapisu budynków
 */
void Colony::saveBuildings(string nazwa_plik){
    ofstream plik(nazwa_plik);
    if(plik.is_open()){
         for(const auto& b : buildings){
            b->save(plik); //Wykorzystanie przeciazonej funkcji virtualnej save()
        }
        plik.close();
    }

}

/**
 * @brief Wczytywanie wszystkiego
 * 
 * @param nazwa_plik plik do zapisu parametrów kolonii
 */
void Colony::load(const string& nazwa_zapisu){
    loadBuildings("data/saves/"+nazwa_zapisu+"/"+"zapis_buildings.txt");
    loadColony("data/saves/"+nazwa_zapisu+"/"+"zapis_colony.txt");
    cout<<"Gra zostala wczytana z pliku."<<endl;
}

//Wczytywanie wszystkich budynkow
/**
 * @brief Wczytywanie wszystkich budynków
 * 
 * @param nazwa_plik plik do zapisu budynków
 */
void Colony::loadBuildings(string nazwa_plik) {
    ifstream plik(nazwa_plik);

    if (plik.is_open()) {

        buildings.clear();

        //Parametry
        int w_type, w_id, w, maxSaved = 0;
        string w_n;
        int kE,kT,kK;
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
                    int e;
                    plik >> e;
                    
                    auto energia = make_unique<Energy>(w_n, kE,kK,kT, e, w);
                    
                    energia->setId(w_id); //Ustawianie poprawnego ID (bo psuje sie podczas wczytywania
                    nowyBudynek = move(energia);
                    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
                    break; 
                }

                case TypBudynku::FARM: {
                    //Dodatkowe parametry
                    int f;
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
                    int s, ti;
                    plik >> s>>ti;
                
                    auto producer = make_unique<Producer>(w_n, kE,kK,kT, s, w,ti);

                    producer->setId(w_id); 
                    nowyBudynek = move(producer);
                    f_logisyka.setDWorkers(nowyBudynek->getDemandWorkers());
                    break;
                }
                case TypBudynku::TERR: {
                    //Dodatkowe parametry
                    int te;
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
        cout<<"Budynki zostaly wczytane"<<endl;
    }
} 

void Colony::loadColony(string nazwa_plik){f_logisyka.load(nazwa_plik);}

// ==========================================
// NEXT ROUND
// ==========================================


/**
 * @brief Funkcja wywoływująca funkcję UIczyNextRound z Logistics
 * 
 * @return NextResult pakiet wyników z NextRound
 */
NextResult Colony::UInextRound(){return f_logisyka.UIczyNextRound(buildings);}

/**
 * @brief Sprawdzanie czy nie nastąpiło przejście na kolejny poziom
 * 
 * @return true tak
 * @return false nie
 */
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

int Colony::getIloscBudynkow()const{return (int)buildings.size();}
int Colony::getAllWorkers()const{return f_logisyka.getAWorkers();}
int Colony::getDemandWorkers()const{return f_logisyka.getDWorkers();}
int Colony:: getRuch()const{return f_logisyka.getRuch();}
int Colony::getLvlTerr() const{return f_logisyka.getLvlTerr();}
int Colony::getToNextLvlTerr() const{return f_logisyka.getToNextLvlTerr();}
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
int Colony::getReqEnergy() const{return f_logisyka.getReqEnergy();}
int Colony::getGenEnergy() const{return f_logisyka.getGenEnergy();}
int Colony::getReqFood() const{return f_logisyka.getReqFood();}
int Colony::getFood() const{return f_logisyka.getFood();}
int Colony::getStone() const{return f_logisyka.getStone();}
int Colony::getTitan() const{return f_logisyka.getTitan();}
