/*
Plik: asdtests.cc
Przeznaczenie:	Oprogramowanie dla zajc† laboratoryjnych
		przedmiot ALGORYTMY I STRUKTURY DANYCH

Opracowaˆ:	dr inz. Pawel Grzegorzewicz
		Instytut Informatyki Politechniki Warszawskiej
		ul. Nowowiejska 15/19, 00-650 Warszawa
*/


//Jest to plik z testami dla abstrakcyjnego typu slownik (ang. dictionary).
//Plik nalezy wlaczyc przy pomocy #include w kod klasy testowanej, 
//przy czym przed #include nalezy zdefiniowac stala:
//#define TSTCLASS ... np DictArray

#define AXIOM(text) if(test_level>=2) printf("aksjomat: %s\n",text);AXIOMTEXT=text;

//#define TST(preparation,val1,val2)     preparation;\
//    if(!((int)(val1==val2) && (int)(val2==val1))) {                \
//      printf("BLAD DLA AKSJOMATU: %s\nTEST: %s\nWartosc pierwsza: %s\nWartosc druga:    %s\n",AXIOMTEXT, #preparation,(char*)val1,(char*)val2); \
//      errors++;						\
//    } else if(test_level>=3) \
//      printf("test: %s,%s,%s\n", #preparation, "#val1", "#val2");
#define TST(preparation,val1,val2)     preparation;\
    if(!((val1==val2) && (val2==val1))) {                \
      printf("BLAD DLA AKSJOMATU: %s\nTEST: %s\n",AXIOMTEXT, #preparation); \
      errors++;						\
    } else if(test_level>=3) \
      printf("test: %s,%s,%s\n", #preparation, "#val1", "#val2");

public:
bool self_test(int test_level) {

    TSTCLASS d1,d2,d3;				//Zmienne do testowania klasy
    int errors = 0;

    printf("TESTOWANIE KLASY\n");
    
    char *AXIOMTEXT;
    
    AXIOM("Dict { insert(Key k, Value v) } == { del(k); unsafeins(k,v) }")
    
    TST({d1.init_dict();d2=d1;d1.insert(k1,v1);d2.del(k1);d2.insert(k1,v1);},d1,d2)
    TST({d1.init_dict();d1.insert(k1,v1);d2=d1;d1.insert(k1,v1);d2.del(k1);d2.insert(k1,v1);},d1,d2)
    TST({d1.init_dict();d1.insert(k2,v2);d2=d1;d1.insert(k1,v1);d2.del(k1);d2.insert(k1,v1);},d1,d2)
						//Operacja wstawiania usuwa i
						//wstawia bezwarunkowo-fizycznie
    AXIOM("Dict { exists(Key k) } == { }")	//exists nie zmienia stanu obiektu
    
    TST({d1.init_dict();d2=d1;d2.exists(k1);},d1,d2)
    TST({d1.init_dict();d1.insert(k1,v1);d2=d1;d2.exists(k1);},d1,d2)
    TST({d1.init_dict();d1.insert(k2,v2);d2=d1;d2.exists(k1);},d1,d2)

    AXIOM("bool { Dict(); return exists(Key k) } == false")
						//W pustym slowniku nie ma nic
    TST({d1.init_dict();},d1.exists(k1),Key(0))
    
    AXIOM("bool { unsafeins(Key k, Value v); return exists(k) } == TRUE")
						//Jesli ostatnio wstawiono, to nalezy
    TST({d1.init_dict();d1.insert(k1,v1);},d1.exists(k1),Key(1))
    TST({d1.init_dict();d1.insert(k2,v2);d1.insert(k1,v1);},d1.exists(k1),Key(1))
    					
    AXIOM("bool { unsafeins(Key k1, Value v); return exists(Key k2) } ==if (k1 != k2) { return exists(k2) }")
						//Jesli ostatnio nie wstawiono, to 
						//szukaj we wczesniejszych wstawieniach
    TST({d1.init_dict();d2=d1;d1.insert(k1,v1);}, d1.exists(k2), d2.exists(k2))
    TST({d1.init_dict();d1.insert(k3,v3);d2=d1;d1.insert(k1,v1);}, d1.exists(k2), d2.exists(k2))
    
    
    AXIOM("Dict { read(Key k) } == { }")	//read nie zmienia stanu obiektu
    TST({d1.init_dict();d2=d1;d2.read(k1);},d1,d2)
    TST({d1.init_dict();d1.insert(k1,v1);d2=d1;d2.read(k1);},d1,d2)
    TST({d1.init_dict();d1.insert(k2,v2);d2=d1;d2.read(k1);},d1,d2)

    AXIOM("Value { Dict(); return read(Key k) } == Value()")
						//W pustym ssowniku nie ma nic
    TST({d1.init_dict();},d1.read(k1),Value())
    
    AXIOM("Value { unsafeins(Key k, Value v); return read(k) } == v")
						//Jesli ostatnio wstawiono, to wartosc
    TST({d1.init_dict();d1.insert(k1,v1);},d1.read(k1),v1)
    TST({d1.init_dict();d1.insert(k1,v1);d1.insert(k1,v1);},d1.read(k1),v1)
    TST({d1.init_dict();d1.insert(k2,v2);d1.insert(k1,v1);},d1.read(k1),v1)
    				
    AXIOM("Value { unsafeins(Key k1, Value v1); return read(Key k2) } ==if (k1 != k2) { return read(k2) }")
						//Jesli ostatnio nie wstawiono, to 
						//szukaj we wczesniejszych wstawieniach
    TST({d1.init_dict();d2=d1;d1.insert(k1,v1);}, d1.read(k2), d2.read(k2))
    TST({d1.init_dict();d1.insert(k2,v2);d2=d1;	d1.insert(k1,v1);}, d1.read(k2), d2.read(k2))
    TST({d1.init_dict();d1.insert(k3,v3);d2=d1;	d1.insert(k1,v1);}, d1.read(k2), d2.read(k2))
    
    AXIOM("Dict { Dict(); del(Key k) } == { Dict() }")
						//W pustym delete nic nie skasuje
    TST({d1.init_dict();d2=d1;d1.del(k1);},d1,d2)
    
    
    AXIOM("Dict { unsafeins(Key k1, Value v); del(Key k2) } ==if (k1 != k2) { delete(k2); unsafeins(k1,v) }")
						//Jesli ostatnio nie wstawiono, to 
						//pozostawia w slowniku i
						//szuka we wczesniejszych wstawieniach
    TST({d1.init_dict();d2=d1;			d1.insert(k1,v3);d1.del(k2);d2.del(k2);d2.insert(k1,v3);},d1,d2)
    TST({d1.init_dict();d1.insert(k1,v1);d2=d1;	d1.insert(k1,v3);d1.del(k2);d2.del(k2);d2.insert(k1,v3);},d1,d2)
    TST({d1.init_dict();d1.insert(k2,v2);d2=d1;	d1.insert(k1,v3);d1.del(k2);d2.del(k2);d2.insert(k1,v3);},d1,d2)
    
    AXIOM("Dict { convert() } == { }")		//convert nie zmienia stanu obiektu
    TST({d1.init_dict();d2=d1;					d2.convert();},d1,d2)
    TST({d1.init_dict();d1.insert(k1,v1);d2=d1;			d2.convert();},d1,d2)
    TST({d1.init_dict();d1.insert(k1,v1);d1.insert(k2,v2);d2=d1;d2.convert();},d1,d2)
    
    AXIOM("STRN { Dict(); return convert() } == STRN()")
						//W pustym slowniku nie ma nic
//    TST({d1.init_dict();},d1.convert(),STRN())	
    				
//    AXIOM("STRN { inserted(Key k, Value v); return convert() } ==return STRN(k)+", "+STRN(d)+"\n"+convert()")
						//Jesli ostatnio wstawiono, to dopisac
//  TST()		//Kolejnosci i formatu konwersji nie bedziemy tak dokladnie weryfikowac

    AXIOM("Dict { ~Dict() } == { Dict() }")	//Zniszczenie rownowazne oproznieniu
						//co odpowiada obiektom Dict nie powo-
						//lywanym operacja new
    TST({d1.init_dict();d2=d1;				d1.~TSTCLASS();d2.init_dict();},d1,d2)
    TST({d1.init_dict();d1.insert(k1,v1);d2=d1;		d1.~TSTCLASS();d2.init_dict();},d1,d2)

   if (errors > 0) {
        printf ("LICZBA BLEDOW W ZESTAWACH TESTOWYCH: %i,\n", errors);
    } else printf("Nie wykryto bledow.\n");
    return errors == 0;
};



