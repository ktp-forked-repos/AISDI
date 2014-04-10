/*
Plik: asd.cc
Przeznaczenie:	Oprogramowanie dla zajec laboratoryjnych
	   	przedmiot: ALGORYTMY I STRUKTURY DANYCH

Opracowal:	dr inz. Pawel Grzegorzewicz
		Instytut Informatyki Politechniki Warszawskiej
		ul. Nowowiejska 15/19, 00-650 Warszawa
*/

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "asdparam.h"
#include "asdlab.h"

using namespace std;
#define ARRAYDIM 55



struct KV {
  Key k;
  Value v;
  KV() { 
    k = Key(); 
    v = Value(); 
  }
  KV(const KV& kv) { k = kv.k; v = kv.v; }
  bool operator==(const KV& kv) 
  { 
    return (k == kv.k) && (v == kv.v); 
  }
  void operator=(const KV& kv) { k = kv.k; v = kv.v; };
} ;

class DictArray: public virtual Dict {
KV A[ARRAYDIM];
//bool element_found;
public:
     DictArray();			//Tworzenie pustego slownika
     ~DictArray();			//Niszczenie slownika
     DictArray(const DictArray& d);	//Konstruktor kopiujacy
     void insert(const Key& k, const Value& val);	//Wstawianie wartosci
     bool exists(const Key& k);	//Sprawdzanie, czy jest
     Value read(const Key& k) const;	//Odczyt wartosci
     void del(const Key& k);		//Kasowanie wartosci
     STRN convert() const;		//Slownik w postaci znakowej
     bool info_eq(const DictArray& DA);	//Czy informacyjnie ==
     bool struct_eq(const DictArray& DA);//Czy struktura danych ==
     bool operator==(const DictArray& DA) { return info_eq(DA); };
     					//Operacja porownania informacyjnego
     operator const char*() const;
protected:
    void unsafeins(const Key& k, const Value& val);
    void init_dict();			//Oproznia caly slownik
    
#define TSTCLASS DictArray 
#include "asdtests.cc"			//Kod funkcji testujacych. 

};

DictArray::operator const char*() const { 
  return (const char*)convert().c_str();
}

DictArray::DictArray() {		//Tworzenie pustego slownika
    for (int i=0; i<ARRAYDIM; i++) A[i]=KV();
    					//Wstawianie wartosci klucza Key()
    					//oznacza zwolnienie pozycji
};
DictArray::~DictArray() {for (int i=0; i<ARRAYDIM; i++) A[i]=KV(); delete A; }		//Niszczenie slownika
void DictArray::insert(const Key& k, const Value& val) {
  //Wstawianie wartosci
  del(k);
  unsafeins(k,val);
};
void DictArray::unsafeins(const Key& k, const Value& val) {
  int i;
  for(i=0; i<ARRAYDIM && A[i].k!=Key();i++){};
  if(i==ARRAYDIM) return;
  A[i].k=k;
  A[i].v=val;
};

void DictArray::init_dict() {
  for(int i=0;i<ARRAYDIM;i++) A[i]=KV();
};

DictArray::DictArray(const DictArray& d) {	//Konstruktor kopiujacy
    for (int i=0; i<ARRAYDIM; i++) A[i]=d.A[i];
};
bool DictArray::exists(const Key& k){		//Sprawdzanie, czy jest
  for(int i=0; i<ARRAYDIM; i++) if(A[i].k==k) return true;
  //element_found = false;
  return false;
};
Value DictArray::read(const Key& k)const{	//Odczyt wartosci
  //usleep(100); 
  for(int i=0; i<ARRAYDIM; i++) if(A[i].k==k) return A[i].v;
  return Value();
};
void DictArray::del(const Key& k){		//Kasowanie wartosci
  int i;
  for(i=0; i<ARRAYDIM; i++)
    if(A[i].k==k) {
      A[i]=KV();
      break;
    }
};
STRN DictArray::convert()const{		//Slownik w postaci znakowej
    STRN str=STRN();
    for(int i=0;i<ARRAYDIM ;i++){
      if(A[i].k==Key()) continue;
      str+=(const char*)(A[i].k);	
      str+=",";		
      str+=A[i].v;
      str+="\n";
    }
    return str;
};
bool DictArray::info_eq(const DictArray& DA){		//Czy informacyjnie ==
  //int i=0,j=0;
  for(int i=0;i<ARRAYDIM ;i++){
    if(DA.A[i].v!=read(DA.A[i].k)) return false;
  }
  for(int i=0;i<ARRAYDIM ;i++){
    if(A[i].v!=DA.read(A[i].k)) return false;
  }
  return true;
}

bool DictArray::struct_eq(const DictArray& DA){	//Czy struktura danych ==
  for(int i=0; i<ARRAYDIM; i++)
    if(A[i].k!=DA.A[i].k || A[i].v!=DA.A[i].v)
      return false;
  return true;
};

int main() { 
DictArray DA;
    cout << "start"<< endl; 
    cout << DA.convert();
    DA.insert(1,"test1");
    cout << DA.convert();
    DA.info_eq(DA);
    DA.struct_eq(DA);
    cout << DA.convert();
    DA.del(1);
    cout << DA.convert();
    cout << endl;
    return DA.self_test(1);
}
