#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

#include "logistics.h"
#include "energy.h"
#include "farm.h"
#include "housing.h"
#include "producer.h"
#include "terr.h"


//Logistics::Logistics():tura(1),all_workers(10),ruch(0),demand_workers(0),nazwa_kolonii("XX"),reqEnergy(0),genEnergy(0),reqFood(20),wsp_terr(0),lvl_terr(0),food(1000),titan(100),stone(100){}
Logistics::Logistics():tura(1),all_workers(10),ruch(0),demand_workers(0),nazwa_kolonii("XX"),reqEnergy(0),genEnergy(0),reqFood(20),wsp_terr(0),lvl_terr(0),food(100),titan(0),stone(50){}

void Logistics::prnt(){


    if(nazwa_kolonii=="XX"){
        prntHeader("Kolonia");
    }else{
        prntHeader(nazwa_kolonii);
    }
        cout<<MAGENTA<<"Nr tury: "<<tura<<"                  Nr ruchu: "<<ruch<<"/3"<<RESET<<endl;
    if(demand_workers==all_workers){
        cout<<BLUE<<"Pracownicy: "<<demand_workers<<"/"<<all_workers<<"                   Poziom wsp. terraformacji: "<<BOLD<<lvl_terr<<RESET<<endl;
    }else{
        cout<<GREEN<<"Pracownicy: "<<demand_workers<<"/"<<all_workers<<BLUE<<"           Poziom wsp. terraformacji: "<<BOLD<<lvl_terr<<RESET<<endl;
    }

    prntHeader("Informacje LOGISTICS");
    if(reqEnergy>genEnergy){
        cout<<RED<<"Energia: "<<BOLD<<genEnergy<<"/"<<reqEnergy<<RESET<<endl;
    }else{
        cout<<GREEN<<"Energia: "<<BOLD<<genEnergy<<"/"<<reqEnergy<<RESET<<endl;
    }
    if(food>2*reqFood){
        cout<<GREEN<<"Posiadane jedzenie (zapotrzebowanie na runde): "<<BOLD<<food<<" ("<<reqFood<<")"<<RESET<<endl;
    }else{if(food>=reqFood & food<2*reqFood){
        cout<<BLUE<<"Posiadane jedzenie (zapotrzebowanie na runde): "<<BOLD<<food<<" ("<<reqFood<<") "<<NO_BOLD<<RED<<"Starczy tylko na jedna runde!"<<RESET<<endl;
    }else{if(food<reqFood){
    cout<<RED<<"Posiadane jedzenie (zapotrzebowanie na runde): "<<food<<BOLD<<" ("<<reqFood<<")!"<<BOLD<<" Brakuje "<<reqFood-food<<" jedzenia!!!"<<RESET<<endl;
    }}}
    if(stone!=0){
        cout<<YELLOW<<"Posiadany kamien: "<<BOLD<<stone<<RESET<<endl;
    }
    if(titan!=0){
        cout<<YELLOW<<"Posiadany tytan: "<<BOLD<<titan<<RESET<<endl;
    }
    cout<<endl;
}

void Logistics::prntRound(double f, double s, double t,double te){
    if(nazwa_kolonii=="XX"){
        prntHeader("Kolonia");
    }else{
        prntHeader(nazwa_kolonii);
    }
        cout<<MAGENTA<<"Nr tury: "<<tura<<CYAN<<" +1"<<MAGENTA<<"         Nr ruchu: 0/3"<<RESET<<endl;
    if(demand_workers==all_workers){
        cout<<BLUE<<"Pracownicy: "<<demand_workers<<"/"<<all_workers<<"           Poziom wsp. terraformacji: "<<BOLD<<lvl_terr<<CYAN<<" + "<<CYAN<<te<<RESET<<endl;
    }else{
        cout<<GREEN<<"Pracownicy: "<<demand_workers<<"/"<<all_workers<<BLUE<<"           Poziom wsp. terraformacji: "<<BOLD<<lvl_terr<<CYAN<<" + "<<CYAN<<te<<RESET<<endl;
    }
        prntHeader("Informacje LOGISTICS");
    if(reqEnergy>genEnergy){
        cout<<RED<<"Energia: "<<BOLD<<genEnergy<<"/"<<reqEnergy<<RESET<<endl;
    }else{
        cout<<GREEN<<"Energia: "<<BOLD<<genEnergy<<"/"<<reqEnergy<<RESET<<endl;
    }
    if((food+f)>2*reqFood){
        cout<<GREEN<<"Posiadane jedzenie (zapotrzebowanie na runde): "<<BOLD<<food<<CYAN<<" + "<<f<<GREEN<<NO_BOLD<<" ("<<BOLD<<reqFood<<NO_BOLD<<")"<<RESET<<endl;
    }else{if((food+f)>=reqFood & (food+f)<2*reqFood){
        cout<<BLUE<<"Posiadane jedzenie (zapotrzebowanie na runde): "<<BOLD<<food<<CYAN<<" + "<<f<<RED<<NO_BOLD<<" ("<<BOLD<<reqFood<<NO_BOLD<<") "<<RED<<"Starczy tylko na jedna runde!"<<RESET<<endl;
    }else{if((food+f)<reqFood){
    cout<<RED<<"Posiadane jedzenie (zapotrzebowanie na runde): "<<BOLD<<food<<CYAN<<" + "<<f<<NO_BOLD<<RED<<" ("<<BOLD<<reqFood<<NO_BOLD<<")!"<<BOLD<<" Brakuje "<<BOLD<<reqFood+f-food<<NO_BOLD<<" jedzenia!!!"<<RESET<<endl;
    }}}
    if(stone!=0){
        cout<<YELLOW<<"Posiadany kamien: "<<BOLD<<stone<<CYAN<<" + "<<s<<YELLOW<<NO_BOLD<<RESET<<endl;
    }
    if(titan!=0){
        cout<<YELLOW<<"Posiadany tytan: "<<BOLD<<titan<<CYAN<<" + "<<t<<YELLOW<<NO_BOLD<<RESET<<endl;
    }
    cout<<endl;
}

int Logistics::czyNextRound(const vector<unique_ptr<Building>>& budynki){
    string decyzja;
    cout<<YELLOW<<">>Czy na pewno chcesz przejsc do kolejnej tury? (y / n)"<<RESET<<endl;
    cin>>decyzja;
    if(decyzja=="y"||decyzja=="yes"||decyzja=="tak"||decyzja=="t"){
        cout<<YELLOW<<">>Rozpoczynanie procedury przejscia do kolejnej rundy..."<<RESET<<endl;
        cout<<endl;
        if(nextRound(budynki)){
            cout<<GREEN<<BOLD<<"Udalo sie przejsc do kolejnej rundy!"<<RESET<<endl;
            tura++;
            ruch=0;
            cout<<endl;
            return 1;
        }else if(food==0){
            cout<<RED<<"KOLONIA UMARLA, Z POWODU BRAKU JEDZENIA!!"<<RESET<<endl;

            return -1;
        } else if(genEnergy<reqEnergy){
            cout<<GREEN<<BOLD<<"Udalo sie przejsc do kolejnej rundy!"<<RED<<" Ale..."<<RESET<<endl;
            cout<<RED<<BOLD<<"Z powod braku energii zaden z budynkow nie wykonal pracy!"<<RESET<<endl;
            tura++;
            ruch=0;
            cout<<endl;
            return 1;
        } else{
            return 0;
        }
    }else{
        cout<<YELLOW<<"Anulowano przejscie do kolejnej rundy."<<RESET<<endl;
        return 0;
    }
}
bool Logistics::nextRound(const vector<unique_ptr<Building>>& budynki){
        if((food-reqFood)>=0){
            cout<<GREEN<<"Ilosc jedzenia:"<<BOLD<< "ZGODNA"<<RESET<<endl;
            cout<<endl;
            food-=reqFood;
            
            if(genEnergy>=reqEnergy){
                cout<<GREEN<<"Ilosc energii:"<<BOLD<< "ZGODNA"<<RESET<<endl;
                cout<<endl;

                cout<<YELLOW<<">>Sprawdzanie produkcji budynkow:..."<<RESET<<endl;
                cout<<endl;

                double c_food=0;
                double c_stone=0;
                double c_titan=0;
                double c_terr=0;

                //dodac pozostale surowce
                for(const auto& b: budynki){
                    double c=0;
                    c=b->work();
                    switch(b->getTyp()){
                        case TypBudynku::FARM:
                            c_food+=c;
                            break;
                        case TypBudynku::PRODUCER:{
                            Producer* p = static_cast<Producer*>(b.get());
                            TypProducer pTyp=p->getTypeProducer();
                            c_stone+=c;
                            if(pTyp==TypProducer::WIERTLO_GLEBINOWE||pTyp==TypProducer::KOMBINAT_GORNICZY||pTyp==TypProducer::AUTOMAT_WYDOBYWCZY){
                                c_titan += p->getGenTitan();
                            }
                            
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

                cout<<endl;
                //dodawac pozostale
                prntRound(c_food,c_stone,c_titan,c_terr);
                food+=c_food;
                stone+=c_stone;
                titan+=c_titan;
                wsp_terr+=c_terr;
                cout<<endl;
                return true;

            }else{

                cout<<RED<<"Ilosc energii:"<<BOLD<< "BRAK"<<RESET<<endl;
                cout<<RED<<BOLD<<"Niewystarczajaca ilosc energi!!"<<RESET<<endl;
                cout<<endl;

                if((food-reqFood)>=0){
                    cout<<GREEN<<"Ilosc jedzenia:"<<BOLD<< "ZGODNA"<<RESET<<endl;
                    cout<<endl;
                }
                return false;
            }
        }else{
            cout<<RED<<"Ilosc jedzenia:"<<BOLD<< "BRAK"<<RESET<<endl;
            cout<<RED<<BOLD<<"Brak jedzenia!!"<<RESET<<endl;
            food=0;
            cout<<endl;
            return false;
        }
}


bool Logistics::sprawdzLvlTerr(){
    vector<int> progi={10,20,50,100,200,500,1000,2000,5000};
    if(progi[lvl_terr]<wsp_terr){
        lvl_terr++;
        cout<<MAGENTA<<"Osiagnales kolejny poziom terraformacji!! \n"<<BOLD<<"Aktualny poziom "<<lvl_terr<<RESET<<endl;
        return true;
    }else{
        return false;
    }
}

void Logistics::updateBudynek(Building* budynek){

    switch (budynek->getTyp())
    {
    case TypBudynku::ENERGY:{
        Energy* energy = static_cast<Energy*>(budynek);
        genEnergy+=energy->getEnergy();
        reqEnergy+=budynek->getReqEnergy();
        break;
        }
    case TypBudynku::FARM:
        reqEnergy+=budynek->getReqEnergy();

        break;
    case TypBudynku::HOUSING:{
        Housing* house = static_cast<Housing*>(budynek);
        reqFood+=2*house->getResidents();
        reqEnergy+=budynek->getReqEnergy();
        break;
    }
    case TypBudynku::PRODUCER:
        reqEnergy+=budynek->getReqEnergy();
        break;
    case TypBudynku::TERR:
        reqEnergy+=budynek->getReqEnergy();
        break;
    default:
        break;
    }
}



void Logistics::updateZburzBudynek(Building* budynek){

    switch (budynek->getTyp())
    {
    case TypBudynku::ENERGY:{
        Energy* energy = static_cast<Energy*>(budynek);
        genEnergy-=energy->getEnergy();
        reqEnergy-=budynek->getReqEnergy();
        
        break;
        }
    case TypBudynku::FARM:
        reqEnergy-=budynek->getReqEnergy();

        break;
    case TypBudynku::HOUSING:{
        Housing* house = static_cast<Housing*>(budynek);
        reqFood-=2*house->getResidents();
        reqEnergy-=budynek->getReqEnergy();
        break;
        }
    case TypBudynku::PRODUCER:
        reqEnergy-=budynek->getReqEnergy();
        break;
    case TypBudynku::TERR:
        reqEnergy-=budynek->getReqEnergy();
        break;
    default:
        break;
    }
}

void Logistics::setTura(){tura++;}
void Logistics::setRuch(int r){ruch=r;}
void Logistics::setAWorkers(int aw){all_workers+=aw;}
void Logistics::setDWorkers(int dw){demand_workers+=dw;}
void Logistics::setStone(double s){stone=s;}
void Logistics::setTitan(double t){titan=t;}

int Logistics::getTura() const{return tura;}
int Logistics::getRuch() const{return ruch;}
int Logistics::getAWorkers() const{return all_workers;}
int Logistics::getDWorkers() const{return demand_workers;};


double Logistics::getReqEnergy() const{return reqEnergy;}
double Logistics::getGenEnergy() const{return genEnergy;}
double Logistics::getReqFood() const{return reqFood;}
double Logistics::getFood() const{return food;}
int Logistics::getStone() const{return stone;}
int Logistics::getTitan() const{return titan;}
string Logistics::getNazwa() const{return nazwa_kolonii;}
int Logistics::getLvlTerr() const{return lvl_terr;}   

    
void Logistics::setSandbox(){
    stone = 50000;
    titan = 50000;
    food = 50000;
    genEnergy = 50000; 
    all_workers = 1000; 
    demand_workers = 0;
    lvl_terr = 10; 
    wsp_terr = 1000;
    cout << MAGENTA << BOLD << "[DEBUG]: Uruchomiono tryb SANDBOX. Masz nieskonczone zasoby!" << RESET << endl;
}

void Logistics::setCustom(){
    string ti,st,fo,wo,lvl,gen;
    prntHeader("Konfiguracja niestandardowa gry");
    cout<<YELLOW<<"Podaj po spacji parametry ktore chcesz ustawic, jak chcesz ich nie zmieniac, to zamiast nich napisz "<<BOLD<<"'-' "<<NO_BOLD<<"!"<<RESET<<endl;
    cout<<"kamien tytan jedzenie workers poziom_terr gen_energy"<<endl;
    cout<<BLUE<<">>"<<RESET;
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
    cout << MAGENTA << BOLD << "[DEBUG]: Uruchomiono tryb CUSTOM." << RESET << endl;
}


void Logistics::save(string nazwa_plik){
    ofstream plik(nazwa_plik);
    if(plik.is_open()){
        plik<<nazwa_kolonii<<" "<<tura<<" "<<ruch<<" "<<all_workers<<" "<<demand_workers<<" "<<wsp_terr<<" "<<lvl_terr<<" "<<genEnergy<<" "<<reqEnergy<<" "<<reqFood<<" "<<food<<" "<<stone<<" "<<titan<<endl;

        plik.close();
        }
}


void Logistics::load(string nazwa_plik){
    ifstream plik(nazwa_plik);
    //plik<<nazwa_kolonii<<" "<<tura<<" "<<ruch<<" "<<all_workers<<" "<<demand_workers<<" "<<f_logisyka.getGenEnergy()<<" "<<f_logisyka.getReqEnergy()<<" "<<f_logisyka.getReqFood()<<" "<<f_logisyka.getFood()<<" "<<f_logisyka.getStone()<<" "<<f_logisyka.getTitan()<<endl;
    string nazwa;
    int t,aw,dw,s,ti,r,lt;
    double ge,re,rf, f,te;

    if (plik.is_open()) {
        plik>>nazwa>>t>>r>>aw>>dw>>te>>lt>>ge>>re>>rf>>f>>s>>ti;
        //wczytywanie wszsytkich parametrow
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
    }
}





void Logistics::setNazwa(){
    string nazwa;
    if(nazwa_kolonii=="XX"){
        cout<<YELLOW<<"Jak chcesz nazwac swoja nowo powstawajaca kolonie na Marsie? (nie uzywaj polskich znakow, a spacje zastap '_'!! )\n"<<BLUE<<">>"<<RESET;
    }else{
        cout<<YELLOW<<"Na co chcesz zmienic nazwe swojej kolonii? (nie uzywaj polskich znakow, a spacje zastap '_'!!)\n"<<BLUE<<">>"<<RESET;
    }
    cin>>nazwa;
    if(!nazwa.empty()){
        replace(nazwa.begin(), nazwa.end(), '_', ' ');
        nazwa_kolonii=nazwa;
        cout<<YELLOW<<"Ustawiono nazwe kolonii na "<<BOLD<<nazwa<<RESET<<YELLOW<<"!!\nNazwe zawsze mozesz pozniej zmienic w ustawieniach!!"<<RESET<<endl;
    }else{
        cout<<RED<<"Niestety nie mozesz tak nazwac swojej kolonii! \n"<<RESET<<endl;

        if(nazwa_kolonii=="XX"){
            cout<<YELLOW<<"Ustawiono nazwe kolonii na "<<BOLD<<"Kolonia"<<RESET<<YELLOW<<"!!\nNazwe zawsze mozesz pozniej zmienic w ustawieniach!!"<<RESET<<endl;
            nazwa_kolonii="Kolonia";
        }else{
            cout<<YELLOW<<"Zostawiono poprzednia nazwe kolonii:"<<BOLD<<nazwa_kolonii<<RESET<<YELLOW<<"!!\nNazwe zawsze mozesz pozniej zmienic w ustawieniach!!"<<RESET<<endl;
            
        }
      }
}