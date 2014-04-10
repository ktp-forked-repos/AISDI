/*
Plik: asdlab.h
Przeznaczenie:	Oprogramowanie dla zajc† laboratoryjnych
		przedmiot	ALGORYTMY I STRUKTURY DANYCH

Opracowal:	dr inz. Pawel Grzegorzewicz
		Instytut Informatyki Politechniki Warszawskiej
		ul. Nowowiejska 15/19, 00-650 Warszawa
*/
/*
Jest to plik naglowkowy abstrakcyjnego typu slownik (ang. dictionary).
Okreslone sa nazwy operacji, arnosc operacji, opisane jest 
aksjomatycznie i slownie dzialanie podanych operacji.
Obiekty implementacji slownika przechowuja wartosci danych typu Value,
przy czym kazda wartosc jest identyfikowana kluczem typu Key.
*/
//typedef int bool;

//extern class STRN;	//Obiekty typu string
//extern class Value;	//Typ "parametryzujacy" specyfikacjc
//extern class Key;	//Typ "parametryzujacy" specyfikacjc

class Dict {					//Specyfikacja slownika:
public:
    virtual void insert(const Key& k, const Value& v)=0;//Wstawianie wartosci
    virtual bool exists(const Key& k)=0;		//Sprawdzanie, czy jest
    virtual Value read(const Key& k) const=0;		//Odczyt wartosci
    virtual void del(const Key& k)=0;		//Kasowanie wartosci
    virtual STRN convert() const=0;			//Slownik w postaci znakowej

protected:
    virtual void unsafeins(const Key& k,const Value& v)=0;	//Operacja niebezpiecznego wstawiania
    						//zaklada sie bowiem, ze w strukturze danych
    						//nie wystepuje para o takim kluczu
						//-fizyczne wstawienie wartosci
/*
Aksjomatyczny opis operacji pomijany przez kompilator:
    Dict { insert(Key k, Value v) } == { del(k); unsafeins(k,v) }
						//Operacja wstawiania usuwa i
						//wstawia bezwarunkowo-fizycznie
    Dict { exists(Key k) } == { }	//exists nie zmienia stanu obiektu
    bool { Dict(); return exists(Key k) } == FALSE
						//W pustym slowniku nie ma nic
    bool { unsafeins(Key k, Value v); return exists(k) } == TRUE
						//Jesli ostatnio wstawiono, to nalezy
    bool { unsafeins(Key k1, Value v); return exists(Key k2) } ==
			if (k1 != k2) { return exists(k2) }
						//Jesli ostatnio nie wstawiono, to 
						//szukaj we wczesniejszych wstawieniach
    Dict { read(Key k) } == { }			//read nie zmienia stanu obiektu
    Value { Dict(); return read(Key k) } == Value()
						//W pustym ssowniku nie ma nic
						//Value() oznacza pusta wartoscz
    Value { unsafeins(Key k, Value v); return read(k) } == v
						//Jesli ostatnio wstawiono, to wartosc
    Value { unsafeins(Key k1, Value v); return read(Key k2) } ==
			if (k1 != k2) { return read(k2) }
						//Jesli ostatnio nie wstawiono, to 
						//szukaj we wczesniejszych wstawieniach
    Dict { Dict(); delete(Key k) } == { Dict() }
						//W pustym delete nic nie skasuje
    Dict { unsafeins(Key k, Value v); del(k) } == { }
						//Jesli ostatnio wstawiono, to kasuje
    Dict { unsafeins(Key k1, Value v); del(Key k2) } ==
			if (k1 != k2) { del(k2); unsafeins(k1,v) }
						//Jesli ostatnio nie wstawiono, to 
						//pozostawia w slowniku i
						//szuka we wczesniejszych wstawieniach
    Dict { convert() } == { }			//convert nie zmienia stanu obiektu
    STRN { Dict(); return convert() } == STRN()
						//W pustym slowniku nie ma nic
    STRN { unsafeins(Key k, Value v); return convert() } ==
			return STRN(k)+", "+STRN(d)+"\n"+convert()
						//Jesli ostatnio wstawiono, to dopisac
    Dict { ~Dict() } == { Dict() }		//Zniszczenie rownowazne oproznieniu
						//co odpowiada obiektom Dict nie powo-
						//lywanym operacja new
*/
};



