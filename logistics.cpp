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
#include "logistics.h"
//moze zmienc ze w basicowym na start dostaje budynek a nie ze dostaje tak o rzeczy
//moze dodac ratusz? czy cos, co dodaje te parametry?
Logistics::Logistics():reqEnergy(0),genEnergy(0),reqFood(10),food(90){}

void Logistics::prnt(){
    cout<<YELLOW<<" - - - - - - - - - - - Informacje LOGISTICS - - - - - - - - - "<<RESET<<endl;
    if(reqEnergy>genEnergy){
        cout<<RED<<"Energia: "<<reqEnergy<<"/"<<genEnergy<<RESET<<endl;
    }else{
        cout<<GREEN<<"Energia: "<<reqEnergy<<"/"<<genEnergy<<RESET<<endl;
    }
    if(food>2*reqFood){
        cout<<GREEN<<"Posiadane jedzenie (zapotrzebowanie na runde): "<<food<<" ("<<reqFood<<")"<<RESET<<endl;
    }else{if(food>=reqFood & food<2*reqFood){
        cout<<BLUE<<"Posiadane jedzenie (zapotrzebowanie na runde): "<<food<<" ("<<reqFood<<")"<<RESET<<endl;
    }else{if(food<reqFood){
    cout<<RED<<"Posiadane jedzenie (zapotrzebowanie na runde): "<<food<<" ("<<reqFood<<")!"<<BOLD<<" Brakuje "<<reqFood-food<<" jedzenia!!!"<<RESET<<endl;
    }}}
    cout<<endl;
}



bool Logistics::nextRound(const vector<unique_ptr<Building>>& budynki){

        if(genEnergy>=reqEnergy){
            cout<<GREEN<<"Ilosc energii:"<<BOLD<< "ZGODNA"<<RESET<<endl;
            if((food-reqFood)>=0){
                cout<<GREEN<<"Ilosc jedzenia:"<<BOLD<< "ZGODNA"<<RESET<<endl;
                cout<<endl;
                food-=reqFood;
                cout<<YELLOW<<">>Sprawdzanie produkcji budynkow:..."<<RESET<<endl;
                cout<<endl;

                double c_food=0;

                //dodac pozostale surowce
                for(const auto& b: budynki){
                    double c=0;
                    c=b->work();
                    switch(b->getTyp()){
                        case TypBudynku::FARM:
                            c_food+=c;
                            break;
                        default:
                            break;
                    }
                }

                cout<<endl;
                cout<<YELLOW<<BOLD<<"- - - - - - Lacznie wyprodukowano - - - - - - "<<RESET<<endl;
                cout<<YELLOW<<"Jedzenie: "<<c_food<<RESET<<endl;
                //dodac pozostale
                food+=c_food;

                cout<<endl;
                return true;

            }else{
                cout<<RED<<"Ilosc jedzenia:"<<BOLD<< "BRAK"<<RESET<<endl;
                cout<<RED<<BOLD<<"Brak jedzenia!!"<<RESET<<endl;
                cout<<endl;
                return false;
            }
        }else{
            cout<<RED<<"Ilosc energii:"<<BOLD<< "BRAK"<<RESET<<endl;
            cout<<RED<<BOLD<<"Brak energi!!"<<RESET<<endl;
            cout<<endl;
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
    default:
        break;
    }
}
