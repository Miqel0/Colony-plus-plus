#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

#include "logistics.h"
#include "energy.h"
#include "farm.h"
#include "housing.h"
#include "producer.h"
//moze zmienc ze w basicowym na start dostaje budynek a nie ze dostaje tak o rzeczy
//moze dodac ratusz? czy cos, co dodaje te parametry?
Logistics::Logistics():reqEnergy(0),genEnergy(0),reqFood(20),food(1000),titan(0),stone(0){}

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
    if(stone!=0){
        cout<<YELLOW<<"Posiadany kamien: "<<BOLD<<stone<<RESET<<endl;
    }
    if(titan!=0){
        cout<<YELLOW<<"Posiadany tytan: "<<BOLD<<titan<<RESET<<endl;
    }
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
                double c_stone=0;
                double c_titan=0;

                //dodac pozostale surowce
                for(const auto& b: budynki){
                    double c=0;
                    c=b->work();
                    switch(b->getTyp()){
                        case TypBudynku::FARM:
                            c_food+=c;
                            break;
                        case TypBudynku::PRODUCER:{
                            int kamien = static_cast<int>(TypProducer::KOPALNIA_KAMIENIA);
                            int tytan  = static_cast<int>(TypProducer::KOPALNIA_TYTANU);
                            int zaaw   = static_cast<int>(TypProducer::ZAAWANSOWANA_KOPALNIA);
                            
                            int  a = b->getPType();

                            cout<<a<<endl;
                            if(a==kamien || a==zaaw){
                                c_stone+=c;
                            }
                            if(a==tytan||a==zaaw){
                                c_titan+=c;
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }

                cout<<endl;
                cout<<YELLOW<<BOLD<<"- - - - - - Lacznie wyprodukowano - - - - - - "<<RESET<<endl;
                if(c_food!=0){
                    cout<<YELLOW<<"Jedzenie: "<<c_food<<RESET<<endl;

                }
                if(c_stone!=0){
                    cout<<YELLOW<<"Kamien: "<<c_stone<<RESET<<endl;
                }
                if(c_titan!=0){
                    cout<<YELLOW<<"Tytan: "<<c_titan<<RESET<<endl;
                }
                //dodac pozostale
                food+=c_food;
                stone+=c_stone;
                titan+=c_titan;


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
    case TypBudynku::PRODUCER:
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
    default:
        break;
    }
}



double Logistics::getReqEnergy() const{return reqEnergy;}
double Logistics::getGenEnergy() const{return genEnergy;}
double Logistics::getReqFood() const{return reqFood;}
double Logistics::getFood() const{return food;}
int Logistics::getStone() const{return stone;}
int Logistics::getTitan() const{return titan;}

void Logistics:: load(double re,double ge,double rf,double f,int s,int ti){
    reqEnergy = re;
    genEnergy = ge;
    reqFood = rf;
    food = f;    
    stone = s;
    titan = ti;
}
