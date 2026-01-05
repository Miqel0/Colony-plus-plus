#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#include "producer.h"

Producer:: Producer():Building("XXX",TypBudynku::PRODUCER,0,0,0,0),stoneGen(0),titanGen(0),pType(TypProducer::NIEZNANY){}
Producer::Producer(string n, double kE,double kK, double kT, double s,TypProducer t,int w,double ti):Building(n,TypBudynku::PRODUCER,kE,kK,kT,w),stoneGen(s),titanGen(ti),pType(t){}

void Producer::prnt(int il)const{
    //Building::prnt(il);
    //prntTablica1("Informacje PRODUCER","Generowany kamien: ",cleanNum(stoneGen),"Generowany tytan: ",cleanNum(titanGen));
    prntTablica(name,"Ilosc: ",cleanNum(il),"Koszt energii: ",cleanNum(kosztEnergii),"Pracownicy: ",cleanNum(workers)," "," ","Generowany kamien: ",cleanNum(stoneGen),"Generowany tytan: ",cleanNum(titanGen));
}


void Producer::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<static_cast<int>(pType)<<" "<<stoneGen<<" "<<titanGen<<endl; //pilnowac przy kolejnym doawaniu
}


double Producer::work(){
    cout<<YELLOW<<"- - - - - - - - - - - - - - - -"<<RESET<<endl;
    
    switch(pType){
        case TypProducer::ZAAWANSOWANA_KOPALNIA:
            cout<<"Wygenerowany kamien: "<<stoneGen<<endl;
            cout<<"Wygenerowany tytan: "<<titanGen<<endl;
            return stoneGen;
        case TypProducer::KOPALNIA_KAMIENIA:
            cout<<GREEN<<"Wygenerowany kamien: "<<stoneGen<<RESET<<endl;
            return stoneGen;
        case TypProducer::KOPALNIA_TYTANU:
            cout<<GREEN<<"Wygenerowany tytan: "<<titanGen<<RESET<<endl;
            return titanGen;
        default:
        return 0;

}
}
int Producer::getPType() const{return static_cast<int>(pType);}