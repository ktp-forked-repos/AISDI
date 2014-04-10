/** 
@file asd.cc

Plik do modyfikacji w ramach cwiczenia z AISDI.
Zawiera niekompletne implementacje metod klasy ListMap,
oraz mape podobna koncepcyjnie do tej z cwicznia 1 (SmallMap)
zaimplementowana w jescze bardziej szczatkowy sposob.
Jest tez prosta funkcja testujaca (void test()), ktora
jest wolana w funkcji main. Mozna w niej zaimplementowac
wlasne testy.
NALEZY ZMODYFIKOWAC I UZUPELNIC CIALA METOD KLASY LISTMAP.
    
@author
Pawel Cichocki, Michal Nowacki, Piotr Jastrzebski

@date  
last revision
- 2010.04.08 Piotr Jastrzebski: ukonczono
- 2006.01.06 Michal Nowacki: wersja polska
- 2005.11.17 Michal Nowacki: constructor
- 2005.11.04 Pawel Cichocki: copied comments from the header
- 2005.11.03 Pawel Cichocki: const_iterator done properly now
- 2005.10.27 Pawel Cichocki: cosmetic changes
- 2005.10.26 Michal Nowacki: removed some method bodies
- 2005.10.25 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <assert.h>
#include <algorithm>

#include <iostream>

#ifdef _SUNOS
#include "/home/common/dyd/aisdi/liniowe/ListMap.h" 
#else
#include "ListMap.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// ListMap and ListMap::iterator methods
//////////////////////////////////////////////////////////////////////////////

ListMap::ListMap()
{	
	first = new Node(P(0,""));
	first->next=first;
	first->prev=first;
 };

ListMap::ListMap( const ListMap& m )
{
	Node *tmp;

	first = new Node(P(0,""));
	first->next = first;
	first->prev = first;
	tmp = first;

	for(iterator it = m.begin(); it != m.end(); ++it)
	{
		tmp->next = new Node(*it,NULL,tmp);
		tmp = tmp->next;
	}
	tmp->next = first;
};


ListMap::~ListMap()
{
	clear();
	delete end().node;
};

// Wstawienie elementu do mapy.
// @returns Para, której komponent bool jest równy true gdy wstawienie zosta³o
//          dokonane, równy false gdy element identyfikowany przez klucz
//          ju¿ istnia³ w mapie. Iterator ustawiony jest na ten wstawiony 
//          lub istniej¹cy ju¿ w mapie element.
std::pair<ListMap::iterator, bool> ListMap::insert(const std::pair<Key, Val>& entry)
{
	iterator it;
	for(it=begin(); it!=end(); ++it)
		if(it->first==entry.first)
			return std::make_pair(it, (bool)false);
	iterator wstaw=it;
	--it;
	(it.node)->next = new  ListNode(entry,wstaw.node,it.node);
	(wstaw.node)->prev = (it.node)->next;
	++it;
  	return std::make_pair(it, (bool)true);
}

// Wstawienie elementu do mapy.
// Matoda zak³ada, ¿e w mapie nie wystêpuje element identyfikowany przez key
ListMap::iterator ListMap::unsafe_insert(const std::pair<Key, Val>& entry)
{
	iterator it=end();
	iterator end=it;
	--it;
	(it.node)->next = new  ListNode(entry,end.node,it.node);
	(end.node)->prev = (it.node)->next;
	++it;
	return first;
}

// Zwraca iterator addresuj¹cy element w mapie dla którego klucz jest równy 
// szukanemu kluczowi lub element za ostatnim gdy szukanego klucza brak w mapie.
ListMap::iterator ListMap::find(const Key& k)
{
	iterator it;
	for(it=begin(); it!=end(); ++it)
		if(it->first==k)
			return it;
	return end();
}

ListMap::const_iterator ListMap::find(const Key& k) const
{
	const_iterator it;
	for(it=begin(); it!=end(); ++it)
		if(it->first==k)
			return it;
	return end();
}

// Udostêpnia wartoœæ powi¹zan¹ z kluczem key. Wstawia element do mapy jeœli 
// nie istnia³.
// @returns Referencje do wartoœci powi¹zanej z kluczem.
ListMap::Val& ListMap::operator[](const Key& k)
{
	iterator it=find(k);
	if(it==end())
	{	
		it=unsafe_insert(std::make_pair(k,it->second));
		--it;
	}
	return it->second;
}

// Sprawdzenie czy mapa jest pusta.
bool ListMap::empty( ) const
{
	return first==first->next;
}

// Zwraca iloœæ elementów w mapie.
ListMap::size_type ListMap::size( ) const
{	
	int wynik=0;
	iterator it;
	if(!empty())
		for(it=begin();it!=end();++it)
			++wynik;
	return wynik;
}

// Zwraza iloœæ elementów skojarzonych z kluczem key.
ListMap::size_type ListMap::count(const Key& _Key) const
{
	if(find(_Key)!=end())
		return 1;  // this is not a multimap
	else
		return 0;
}

// Usuwa element z mapy.
// @returns iterator adresuj¹cy pierwszy element za usuwanym.
ListMap::iterator ListMap::erase(ListMap::iterator i)
{
	if (i!=end())
	{
		iterator x;
		((i.node)->prev)->next = (i.node)->next;
		((i.node)->next)->prev = (i.node)->prev;
		x=i.node->next;
		delete i.node;
		i=x;
	}
	return i;
}

// Usuwa zakres elementów z mapy.
// Zakres jest zdefiniowany poprzez iteratory first i last
// first jest okreœla pierwszy element do usuniêcia, a last okreœla element 
// po ostatnim usuniêtym elemencie.
// @returns iterator adresuj¹cy pierwszy element za usuwanym.
ListMap::iterator ListMap::erase(ListMap::iterator f, ListMap::iterator l)
{
	iterator tmp,i;
	for(i=f; i!=l;)
	{
		if(i==end())
			++i;
		i=erase(i);
	
	}
	return l;
}

// Usuwa element z mapy.
// @returns Iloœæ usuniêtych elementów.
//          (nie jest to multimapa, wiêæ mo¿e byæ to wartoœæ 1 lub 0 )
ListMap::size_type ListMap::erase(const Key& key)
{
	iterator it=find(key);
	if(it==end())
		return 0;
	else{
		erase(it);
		return 1;
	}
}

// Usuniêcie wszystkich elementów z mapy.
void ListMap::clear( )
{
	erase(begin(), end());
}

// Porównanie strukturalne map.
// Czy reprezentacja danych jest identyczna.
// Zwraca true jeœli wewnêtrzne struktury map s¹ identyczne.
bool ListMap::struct_eq(const ListMap& another) const
{
	if (size() != another.size())
		return false;

	for(iterator i=begin(), j=another.begin(); i!=end(); ++i, ++j)
		if (*i != *j)
			return false;
	return true;
}

// Porównanie informacyjne map.
// Czy informacje trzymane w mapach s¹ identyczne.
// Zwraca true jeœli mapy zwieraj¹ takie same pary klucz-wartoœæ.
bool ListMap::info_eq(const ListMap& another) const
{
	if(size()!=another.size())
		return false;
	iterator it, x;
	for(it=begin(); it!=end(); ++it)
	{
		x=another.find(it->first);
		if(x==another.end())
			return false;
		if(x->second!=it->second)
			return false;
	}
	return true;
}

// preincrementacja
ListMap::const_iterator& ListMap::const_iterator::operator++()
{
	node=node->next;
	return *this;
}

// postincrementacja
ListMap::const_iterator ListMap::const_iterator::operator++(int)
{
	node=node->next;
	return node->prev;
}

ListMap::const_iterator& ListMap::const_iterator::operator--()
{
	node=node->prev;
	return *this;
}

// postincrementacja
ListMap::const_iterator ListMap::const_iterator::operator--(int)
{
	node=node->prev;
	return node->next;
}

/// Zwraca iterator addresuj¹cy pierwszy element w mapie.
ListMap::iterator ListMap::begin()
{
	return first->next; 
}

/// Zwraca iterator addresuj¹cy pierwszy element w mapie.
ListMap::const_iterator ListMap::begin() const
{ 
	return first->next;
}

/// Zwraca iterator addresuj¹cy element za ostatnim w mapie.   
ListMap::iterator ListMap::end()
{ 
	return first;
}

/// Zwraca iterator addresuj¹cy element za ostatnim w mapie.   
ListMap::const_iterator ListMap::end() const
{ 
	return first;
}

//////////////////////////////////////////////////////////////////////////////
// SmallMap
//////////////////////////////////////////////////////////////////////////////

/// Przyk³ad map'y z implementacj¹ podobn¹ do std::map.
/// To jest jedynie przyk³ad!!!
/// Normalnie implementacja powinna opieraæ siê o drzewo lub tablicê haszuj¹c¹.
template <class Key, class Val>
class SmallMap
{
   std::pair<Key, Val> tab[2];
   int isOcupied[2];
public:

   SmallMap()
   {
      for(unsigned i=0; i<2; ++i)
      {
         isOcupied[i]=0;
      }
   }

   typedef std::pair<Key, Val>* iterator;   ///< Ka¿dy wskaŸnik jest te¿ iteratorem.

   iterator begin() { return tab; }
   iterator end() { return tab+2; }

   Val& operator[](const Key& k)
   {
      static Val val;
      for(unsigned i=0; i<2; ++i)
      {
         if(isOcupied[i]&&tab[i].first==k)
            return tab[i].second;
      }
      // create
      for(unsigned i=0; i<2; ++i)
      {
         if(!isOcupied[i])
         {
            tab[i].first=k;
            isOcupied[i]=1;
            return tab[i].second;
         }
      }
      //std::cout<<"Out of space! You should not put more than two Key-Value pairs into the SmallMap.\n";
      std::cerr<<"Out of space! You should not put more than two Key-Value pairs into the SmallMap.\n";
      //assert(0);
      return val;   // Ma³o sensowne, ale to jest tylko przyk³¹d
   }
};
      

//////////////////////////////////////////////////////////////////////////////
// Testy
//////////////////////////////////////////////////////////////////////////////

/// Funckcja pomocnicza do wypisania elementów.
void print(const std::pair<int, std::string>& p)
{
   std::cout<<p.first<<", "<<p.second<<std::endl;
}

#include <map>

/// Testy u¿ytkownika
void test()
{
   // A typedef used by the test.
   typedef std::map<int, std::string> TEST_MAP;
   //typedef SmallMap<int, std::string> TEST_MAP;
   //typedef ListMap TEST_MAP;


ListMap nowy,o,p;
for (ListMap::iterator it=nowy.begin(); it!=nowy.end(); ++it)
	std::cout<<"* ";
std::cout << "\tKONIEC MOICH TESTOW NA ZAJECIACH" << std::endl;

std::cout << "Testy uzytkownika" << std::endl;

	TEST_MAP m,n;  
   
	m[2] = "Merry";
	m[4] = "Jane";
	m[8] = "Korwin";
	m[4] = "Magdalena";

	n[2]="Magdalena";
	n[4]="Jane";
	n[8] = "Korwin";
	n[4] = "Magdalena";

   
 /*
1MSG : Wstawiam i sprawdzam   - insert, find, read						OK
2MSG : Wstawiam i zmieniam    - insert, operator[], find					OK
3MSG : Wstawiam i kasuje      - insert, erase, find						OK
4MSG : Porownanie pustych     - info_eq, struct_eq						OK
5MSG : Wstawiam i porownuje   - insert, operator[], info_eq, struct_eq, find	OK
6MSG : Wstawiam i przypisuje  - insert, operator=, info_eq, find			OK
*/ 

/*4*/ std::cout << o.info_eq(p) << " " << o.struct_eq(p) << std::endl;
/*1*/ o.insert(std::make_pair(17,"ola")); std::cout << o.find(17)->first << " "; for_each(o.begin(), o.end(), print );
/*6*/ p=o; std::cout << o.info_eq(p) << " " << o.find(17)->first << " " << p.find(17)->first << std::endl;
/*5*/ std::cout << o[17] << std::endl;
/*3*/ std::cout << o.erase(16) << " " << o.find(17)->second << std::endl;
   
	for_each(m.begin(), m.end(), print );
	for_each(n.begin(), n.end(), print );
	
   //system("PAUSE");
}

//////////////////////////////////////////////////////////////////////////////
// main - jest w pliku /home/common/dyd/aisdi/z2/main.cc
//////////////////////////////////////////////////////////////////////////////

//int main()
//{
//   std::cout << "AISDI cwiczenie 2: wchodze do funkcji main." << std::endl;
//   test();
//   // Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
//   Test2();
//   //system("PAUSE");
//   return EXIT_SUCCESS;
//}
