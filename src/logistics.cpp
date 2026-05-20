#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

#include "logistics.h"
#include "energy.h"
#include "farm.h"
#include "housing.h"
#include "producer.h"
#include "terr.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Logistics::Logistics():tura(1),all_workers(10),ruch(0),demand_workers(0),nazwa_kolonii("XX"),reqEnergy(0),genEnergy(0),reqFood(20),wsp_terr(0),lvl_terr(0),food(100),titan(0),stone(100){}

// ==========================================
// NEXT ROUND
// ==========================================

/**
 * @brief Funkcja sprawdzajaca czy sie udalo przejsc do kolejnej rundy
 * 
 * @param budynki Wektor zbudowanych budynków
 * @return NextResult NextResult pakiet wyników z NextRound
 */
NextResult Logistics::UIczyNextRound(const vector<unique_ptr<Building>>& budynki){
    NextResult odp=UInextRound(budynki);//wywolywanie funkcji sprawdzajacej zaleznosci
    if(odp.czy){ 
        tura++;
        ruch=0;
    }
    return odp;
}

/**
 * @brief 
 * 
 * @param budynki wektor zbudowanych budynków
 * @return NextResult paczka z wynikami przejścia do kolejnej rundy
 */
NextResult Logistics::UInextRound(const vector<unique_ptr<Building>>& budynki){
    NextResult odp = {false,false,false,false,"",0,0,0,0};    
    if((food-reqFood)>=0){//Czy jest wystarczajaca liczba jedzenia?

            odp.food=true;
            odp.czy=true;
            food-=reqFood;
            
            if(genEnergy>=reqEnergy){//Czy jest wystarczajaca liczba produkowanego pradu?
                odp.energy=true;

                int c_food=0;
                int c_stone=0;
                int c_titan=0;
                int c_terr=0;
                int new_lvl=0;

                
                for(const auto& b: budynki){//Przejscie po kazdym zbudowanym budynku
                    int c=0;
                    c=b->work();//wywolanie funkcji work() na kazdym z budynkow
                    switch(b->getTyp()){//w zaleznosci od klasy, zwrocona wartosc jset przypisywana do czegos innego
                        case TypBudynku::FARM:
                            c_food+=c;
                            break;
                        case TypBudynku::PRODUCER:{
                            Producer* p = static_cast<Producer*>(b.get());
                            c_stone += c;
                            c_titan += p->getGenTitan(); //FIXME dodac jakas pair czy cos
                            break;
                        }
                        case TypBudynku::TERR:{
                            c_terr+=c;
                            break;
                        }
                        default:
                            break;
                    }
                }
                wsp_terr+=c_terr;
                if(sprawdzLvlTerr()){//Sprawdzenie czy nie nastapilo osiagniecie kolejnego lvl
                    odp.terr=true;
                }

                //Wyswietlenie zaktualizowanych rzeczy po nextRound
                odp.c_food=c_food;
                odp.c_stone=c_stone;
                odp.c_titan=c_titan;
                odp.c_terr=c_terr;
                
                //Dodanie wyprodukowanych surowcow do zmiennej
                food+=c_food;
                stone+=c_stone;
                titan+=c_titan;

            }else{//BRAK ENERGII - budynki nie pracuja

                if((food-reqFood)>=0){//Sprawdzenie czy jest wystarczajco jedzenie pomimo braku prądu
                    odp.food=true;
                }
            }
        }else{//BRAK JEDZENIA - KONIEC GRY
            if(genEnergy>=reqEnergy){
            odp.energy=true;
            }
            food=0;
        }
    return odp;
}


/**
 * @brief Sprawdzanie poziomu terraformacji
 * 
 * @return true 
 * @return false 
 */
bool Logistics::sprawdzLvlTerr(){
    
    if(progi[lvl_terr]<=wsp_terr){//Sprawdzanie czy po zmianie wsp. terraformacji, zmienil sie na kolejny poziom
        lvl_terr++;
        return true;
    }else{
        return false;
    }
}

// ==========================================
// AKUTALIZOWANIE DANYCH
// ==========================================

/**
 * @brief Aktualizowanie parametrów po zbudowaniu budynku
 * 
 * @param budynek nowy zbudowany budynek
 */
void Logistics::updateBudynek(Building* budynek){

    switch (budynek->getTyp())
    {
    case TypBudynku::ENERGY:{//AKTUALIZOWANIE PRODUKCJI I ZAPOTRZEBOWANIA PRĄDU 
        Energy* energy = static_cast<Energy*>(budynek);
        genEnergy+=energy->getEnergy();
        reqEnergy+=budynek->getReqEnergy();
        break;
        }
    case TypBudynku::FARM://AKTUALIZOWANIE ZAPOTRZEBOWANIA PRĄDU 
        reqEnergy+=budynek->getReqEnergy();
        break;

    case TypBudynku::HOUSING:{//AKTUALIZOWANIE ZAPOTRZEBOWANIA PRĄDU I ZYWNOSCI
        Housing* house = static_cast<Housing*>(budynek);
        reqFood+=2*house->getResidents();
        reqEnergy+=budynek->getReqEnergy();
        break;
    }
    case TypBudynku::PRODUCER://AKTUALIZOWANIE ZAPOTRZEBOWANIA PRĄDU 
        reqEnergy+=budynek->getReqEnergy();
        break;

    case TypBudynku::TERR://AKTUALIZOWANIE ZAPOTRZEBOWANIA PRĄDU 
        reqEnergy+=budynek->getReqEnergy();
        break;

    default:
        break;
    }
}


pair<int, int> Logistics::UIupdateZburzBudynek(Building* budynek){

    //Odzyskiwanie pracownikow oraz polowy surowcow
    setDWorkers(-budynek->getDemandWorkers());
    int ti =budynek->getKosztTytan();
    int st = budynek->getKosztKamien();
    
    //Odzyskuje sie polowe surowców!
    titan+=(ti-1+2)/2;
    stone+=(st-1+2)/2;

    switch (budynek->getTyp())
    {
    case TypBudynku::ENERGY:{//AKTUALIZOWANIE PRODUKCJI I ZAPOTRZEBOWANIA PRĄDU 
        Energy* energy = static_cast<Energy*>(budynek);
        genEnergy-=energy->getEnergy();
        reqEnergy-=budynek->getReqEnergy();
        break;
    }
    case TypBudynku::FARM:{//AKTUALIZOWANIE ZAPOTRZEBOWANIA PRĄDU 
        reqEnergy-=budynek->getReqEnergy();
        break;
    }
    case TypBudynku::HOUSING:{//AKTUALIZOWANIE ZAPOTRZEBOWANIA PRĄDU I ZYWNOSCI
        Housing* house = static_cast<Housing*>(budynek);
        reqFood-=2*house->getResidents();
        reqEnergy-=budynek->getReqEnergy();
        break;
    }
    case TypBudynku::PRODUCER:{//AKTUALIZOWANIE ZAPOTRZEBOWANIA PRĄDU 
        reqEnergy-=budynek->getReqEnergy();
        break;
    }
    case TypBudynku::TERR:{//AKTUALIZOWANIE ZAPOTRZEBOWANIA PRĄDU 
        reqEnergy-=budynek->getReqEnergy();
        break;
    }
    default:
        break;
    }
    return {(st-1+2)/2,(ti-1+2)/2};
}

// ==========================================
// SAVE/LOAD
// ==========================================

//ZAPISYWANIE PARAMETROW COLONY - wszystkie zmienne (oprocz budynkow)
void Logistics::save(string nazwa_plik){
    ofstream plik(nazwa_plik);
    if(plik.is_open()){
        plik<<nazwa_kolonii<<" "<<tura<<" "<<ruch<<" "<<all_workers<<" "<<demand_workers<<" "<<wsp_terr<<" "<<lvl_terr<<" "<<genEnergy<<" "<<reqEnergy<<" "<<reqFood<<" "<<food<<" "<<stone<<" "<<titan<<endl;

        plik.close();
        }
}

//WCZYTYWANIE ZAPISANYCH DANYCH
void Logistics::load(string nazwa_plik){
    ifstream plik(nazwa_plik);

    //kolejnosc zapisanych danych w pliku
    //plik<<nazwa_kolonii<<" "<<tura<<" "<<ruch<<" "<<all_workers<<" "<<demand_workers<<" "<<f_logisyka.getGenEnergy()<<" "<<f_logisyka.getReqEnergy()<<" "<<f_logisyka.getReqFood()<<" "<<f_logisyka.getFood()<<" "<<f_logisyka.getStone()<<" "<<f_logisyka.getTitan()<<endl;
    string nazwa;
    int t,aw,dw,s,ti,r,lt;
    int ge,re,rf, f,te;

    if (plik.is_open()) {
        plik>>nazwa>>t>>r>>aw>>dw>>te>>lt>>ge>>re>>rf>>f>>s>>ti;
        
        //WCZYTYWANIE PARAMETROW
        reqEnergy = re;
        genEnergy = ge;
        reqFood = rf;
        food = f;    
        stone = s;
        titan = ti;
        nazwa_kolonii=nazwa;
        demand_workers=dw;
        all_workers=aw;
        tura =t;
        ruch =r;
        wsp_terr=te;
        lvl_terr=lt;
        plik.close();
        //cout<<"Kolonia zostala wczytana"<<endl;
    }
}

// ==========================================
// SETTERY
// ==========================================

void Logistics::setTura(){tura++;}
void Logistics::setRuch(int r){ruch=r;}
void Logistics::setAWorkers(int aw){all_workers+=aw;}
void Logistics::setDWorkers(int dw){demand_workers+=dw;}
void Logistics::setStone(int s){stone=s;}
void Logistics::setTitan(int t){titan=t;}

//USTAWIANIE PARAMETROW COLONY - ustawianie testowe - cheaty - daje duza ilosc surowcow
void Logistics::setSandbox(){
    stone = 50000;
    titan = 50000;
    food = 50000;
    genEnergy = 50000; 
    all_workers = 1000; 
    demand_workers = 0;
    lvl_terr = 10; 
    wsp_terr = 1000;
}

//USTAWIANIE PARAMETROW COLONY - ustawianie testowe - cheaty - daje custom ilosc surowcow
void Logistics::setCustom(){
    string ti,st,fo,wo,lvl,gen;
    cout<<"Podaj po spacji parametry ktore chcesz ustawic, jak chcesz ich nie zmieniac, to zamiast nich napisz !"<<endl;
    cout<<"kamien tytan jedzenie workers poziom_terr gen_energy"<<endl;
    cin>>ti>>st>>fo>>wo>>lvl>>gen;

    if(ti!="-"){
        titan=stoi(ti);
    }
    if(st!="-"){
        stone=stoi(st);
    }
    if(fo!="-"){
        food=stoi(fo);
    }
    if(wo!="-"){
        all_workers=stoi(wo);
    }
    if(lvl!="-"){
        lvl_terr=stoi(lvl);
    }
    if(gen!="-"){
        genEnergy=stoi(gen);
    }
}

//USTAWIANIE NAZWY - wywolywane komenda `rename`
void Logistics::setNazwa(){
    string nazwa;
    if(nazwa_kolonii=="XX"){
        cout<<"Jak chcesz nazwac swoja nowo powstawajaca kolonie na Marsie? (nie uzywaj polskich znakow, a spacje zastap '_'!! )\n";
    }else{
        cout<<"Na co chcesz zmienic nazwe swojej kolonii? (nie uzywaj polskich znakow, a spacje zastap '_'!!)\n";
    }
    cin>>nazwa;
    if(!nazwa.empty()){
        replace(nazwa.begin(), nazwa.end(), '_', ' ');
        nazwa_kolonii=nazwa;
        cout<<"Ustawioe kolonii na "<<nazwa<<"!!\nNazwe zawsze mozesz pozniej zmienic w ustawieniach!!"<<endl;
    }else{
        cout<<"Niestety nie mozesz tak nazwac swojej kolonii! \n"<<endl;

        if(nazwa_kolonii=="XX"){
            cout<<"Ustawiono nazwe kolonii na "<<"Kolonia"<<"!!\nNazwe zawsze mozesz pozniej zmienic w ustawieniach!!"<<endl;
            nazwa_kolonii="Kolonia";
        }else{
            cout<<"Zostawiono poprzednia nazwe kolonii:"<<nazwa_kolonii<<"!!\nNazwe zawsze mozesz pozniej zmienic w ustawieniach!!"<<endl;
        }
    }
}

// ==========================================
// GETTERY
// ==========================================

int Logistics::getTura() const{return tura;}
int Logistics::getRuch() const{return ruch;}
int Logistics::getAWorkers() const{return all_workers;}
int Logistics::getDWorkers() const{return demand_workers;};

int Logistics::getReqEnergy() const{return reqEnergy;}
int Logistics::getGenEnergy() const{return genEnergy;}
int Logistics::getReqFood() const{return reqFood;}
int Logistics::getFood() const{return food;}
int Logistics::getStone() const{return stone;}
int Logistics::getTitan() const{return titan;}
string Logistics::getNazwa() const{return nazwa_kolonii;}
int Logistics::getLvlTerr() const{return lvl_terr;}   
int Logistics::getToNextLvlTerr() const{return progi[lvl_terr]-wsp_terr;}
 