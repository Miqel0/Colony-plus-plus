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
#include "farm.h"

Farm:: Farm():Building("XXX",TypBudynku::FARM,0,0),foodGen(0),time(0),curr_time(0),pType(TypFarm::NIEZNANY){}
Farm::Farm(string n,double k, double f,TypFarm t,int w,int tim):Building(n,TypBudynku::FARM,k,w),foodGen(f),time(tim),curr_time(0),pType(t){}

void Farm::prnt()const{
    Building::prnt();
    cout<<YELLOW<<" - - - - - - - - - -  - Informacje FARM - - - - - - - - - "<<RESET<<endl;
    cout<<"Pod type: "<<static_cast<int>(pType)<<endl;
    cout<<"Generowane jedzenie: "<<foodGen<<endl;
    cout<<"Czas potrzebny do urośnięcia: "<<time<<endl;

}


void Farm::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<static_cast<int>(pType)<<" "<<foodGen<<" "<<time<<endl;
}


double Farm::work(){
    curr_time++;
    cout<<YELLOW<<"- - - - - - - - - - - - - - - -"<<RESET<<endl;
    cout<<curr_time<<"/"<<time<<endl;
    if(curr_time==time){

        cout<<GREEN<<"Jedzenie gotowe do zbioru!!"<<endl;
        cout<<"Wyprodukowano: "<<foodGen<<" jedzenia"<<RESET<<endl;
        curr_time=0;
        return foodGen;
    }else{
        cout<<YELLOW<<"Potrzeba jeszcze "<<time-curr_time<<" rund do wyrosniecia!"<<RESET<<endl;
        return 0;
    }
    

}