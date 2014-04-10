/*
Plik: asdparam.h
Przeznaczenie:	Oprogramowanie dla zaj©† laboratoryjnych
			przedmiot	ALGORYTMY I STRUKTURY DANYCH

Opracowaˆ:	dr in¾. Paweˆ Grzegorzewicz
		Instytut Informatyki Politechniki Warszawskiej
		ul. Nowowiejska 15/19, 00-650 Warszawa
*/

#include <string>

//Jest to plik definiuj¥cy typy Value, Key i STRN.


class Key {		//Definicja typu klucza zapami©tywanych danych
    int i;
    char buf[12];
public:
    Key() { i = 0; buf[0]='0'; buf[1]='\0'; };
    ~Key() { };
    Key(const int& j) { i = j; sprintf(buf,"%i",i);};
    operator const char*() const { return buf; };
    const char* c_str() const { return buf; };
    bool operator==(const Key& k) const { return i == k.i; };
    bool operator!=(const Key& k) const { return i != k.i; };
    bool operator<(const Key& k) const { return i < k.i; };
    bool operator<=(const Key& k) const { return i <= k.i; };
    bool operator>(const Key& k) const { return i > k.i; };
    bool operator>=(const Key& k) const { return i >= k.i; };
    Key(const Key& k) { i = k.i; sprintf(buf,"%i",i); };
    Key& operator=(const Key& k) { i = k.i; sprintf(buf,"%i",i); return *this; };
    Key& operator=(const int j) {i = j; sprintf(buf,"%i",i); return *this;};
    operator int() const { return i; };
    bool operator==( const bool& b ) const { return b == (i!=0); };
};       

const Key k1=1;				//Wartosci do testowania
const Key k2=2;
const Key k3=3;

//--------------------------------------------------------------------

typedef std::string Value;

//------------------------------------------------------------------------

const Value v1="v1_wartosc_danej";		//Wartosci do testowania
const Value v2="v2_wartosc_danej";
const Value v3="v3_wartosc_danej";

typedef std::string STRN;

const Key ZERO_KEY=0;
const Key ONE_KEY=1;

