#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#include "farm.h"

Farm:: Farm():Building("XXX",TypBudynku::FARM,0,0,0,0),foodGen(0),time(0),curr_time(0),pType(TypFarm::NIEZNANY){}
Farm::Farm(string n, double kE,double kK, double kT, double f,TypFarm t,int w,int tim,int ct):Building(n,TypBudynku::FARM,kE,kK,kT,w),foodGen(f),time(tim),curr_time(ct),pType(t){}

void Farm::prnt(int il)const{
    //Building::prnt(il);
    prntTablica(name,"Ilosc: ",cleanNum(il),"Koszt energii: ",cleanNum(kosztEnergii),"Pracownicy: ",cleanNum(workers),"Generowane jedzenie: ",cleanNum(foodGen),"Czas potrzebny do urosniecia: ",cleanNum(time)," "," ");
}


void Farm::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<static_cast<int>(pType)<<" "<<foodGen<<" "<<time<<" "<<curr_time<<endl; 
}


double Farm::work(){
    curr_time++;
    //cout<<YELLOW<<"- - - - - - - - - - - - - - - -"<<RESET<<endl;
    //cout<<curr_time<<"/"<<time<<endl;
    if(curr_time==time){

        //cout<<GREEN<<"Jedzenie gotowe do zbioru!!"<<endl;
        //cout<<"Wyprodukowano: "<<foodGen<<" jedzenia"<<RESET<<endl;
        curr_time=0;
        return foodGen;
    }else{
        //cout<<YELLOW<<"Potrzeba jeszcze "<<time-curr_time<<" rund do wyrosniecia!"<<RESET<<endl;
        return 0;
    }
    

}

int Farm::getPType() const{return static_cast<int>(pType);}