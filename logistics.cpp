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
Logistics::Logistics():tura(0),reqEnergy(0),genEnergy(0),reqFood(10),food(10){}

void Logistics::prnt(){
    cout<<YELLOW<<" - - - - - - - - - - - Informacje LOGISTICS - - - - - - - - - "<<RESET<<endl;
    if(reqEnergy>genEnergy){
        cout<<RED<<"Energia: "<<reqEnergy<<"/"<<genEnergy<<RESET<<endl;
    }else{
        cout<<GREEN<<"Energia: "<<reqEnergy<<"/"<<genEnergy<<RESET<<endl;
    }
    cout<<"Wymagane jedzenie: "<<reqFood<<endl;
    

}



void Logistics::nextRound(){

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
