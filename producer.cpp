#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#include "producer.h"

Producer:: Producer():Building("XXX",TypBudynku::PRODUCER,0,0),stoneGen(0),titanGen(0),pType(TypProducer::NIEZNANY){}
Producer::Producer(string n,double k, double s,TypProducer t,int w,double ti):Building(n,TypBudynku::PRODUCER,k,w),stoneGen(s),titanGen(ti),pType(t){}

void Producer::prnt()const{
    Building::prnt();
    prntHeader("Informacje PRODUCER");
    cout<<"Pod type: "<<static_cast<int>(pType)<<endl;
    if(stoneGen!=0){
        cout<<"Generowany kamien: "<<stoneGen<<endl;
    }
    if(titanGen!=0){
        cout<<"Generowany tytan: "<<titanGen<<endl;
    }
    


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