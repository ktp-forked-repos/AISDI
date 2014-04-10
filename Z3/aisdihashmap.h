/** 
@file aisdihashmap.h

AISDIHashMap and related functions interface.
      
@author
Pawel Cichocki

@date  
last revision
- 2006.03.24 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2006 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <utility>

#include <string>
#include <iostream>
#define MAX_SIZE 64000

template <class Key>
unsigned int hashF(const std::string & k){
	unsigned int hash=5381;
	int c,i=0;
	while ( c= (int)k[i++])
		hash=(hash*33+c)%(MAX_SIZE);
	return hash;
}

/// Default keys' comparing function for AISDIHashMap - it uses
/// operator== by default.
/// @returns 0 if keys are equal, non-zero otherwise.
/// @param key1 First key to compare. 
/// @param key2 Second key to compare. 
template <class Key>   
inline int _compFunc(const Key& key1,const Key& key2)
{
   return (key1==key2);
};

/// A map with a similar interface to std::map.
template<class K, class V,
         unsigned hashF(const K&)=&hashF<K>,
         int compFunc(const K&,const K&)=&_compFunc<K> >
class AISDIHashMap
{
public:
   typedef K key_type;
   typedef V value_type;
   typedef unsigned size_type;
   typedef std::pair<key_type, value_type> P;
   struct  Node						
   {
	   Node *next, *prev;
	   P data;
	   Node(const std::pair< K, V >& d) : next(NULL), prev(NULL), data(d) {}
	   Node(const std::pair< K, V >& d, Node* n, Node*p) : prev(p), next(n), data(d) {}
   };
   
   Node *first; 
   Node *table[MAX_SIZE+1];
   
public:

	AISDIHashMap(){
		for(int i=0;i<MAX_SIZE;i++)
			table[i] = NULL;
		first = NULL;
                table[MAX_SIZE] = new Node( std::make_pair( std::string("GUARD"), V(0) ) );
	}
		
	~AISDIHashMap(){
		clear();
		delete table[MAX_SIZE]; 
	}
		   
	/*explicit*/ AISDIHashMap( AISDIHashMap<K, V, hashF, compFunc>& a){
		for(int i=0;i<MAX_SIZE; i++)
			table[i] = NULL;
		first = NULL;
		table[MAX_SIZE] = new Node( std::make_pair( std::string("Guard"), V(0) ) );
		if ( a.first == NULL )
			return;
		for ( const_iterator it = a.begin(); it != a.end(); ++it )
			insert( std::make_pair( it->first, it->second ) );
	}
           
	class const_iterator : public std::iterator< std::forward_iterator_tag, std::pair<K, V> >{
		protected:
		Node* node;
		Node **tab_ptr;
		friend class AISDIHashMap;
		const_iterator(Node* x, Node **t) : node(x), tab_ptr(t) {}

		public:
		const_iterator(){}
		const_iterator(const const_iterator& a) : node(a.node), tab_ptr(a.tab_ptr) {}

		inline const std::pair<K, V>& operator*() const { return node->data;}
		inline const std::pair<K, V>* operator->() const { return &(node->data);}

		const_iterator& operator++(){
			if ( (*this).node == NULL )
				return *this;
			if ( (*this).node == tab_ptr[MAX_SIZE] )
				return *this;
			if ( (*this).node->next == NULL ){
				unsigned nr = hashF( (*this)->first );
				for (int i=nr+1;i < MAX_SIZE+1;i++)
					if ( tab_ptr[i] != NULL ){
						(*this).node = tab_ptr[i]; 
						return *this;
					}
				return *this; // if it is the last element
			}
			(*this).node = (*this).node->next;
			return *this;
		}

		const_iterator operator++(int){
			const_iterator it = *this;
			++(*this);
			return it;
		}

		const_iterator& operator--(){
			if ( node->prev == NULL ){
				unsigned nr;
				if ( node == tab_ptr[MAX_SIZE] )
					nr = MAX_SIZE-1;
				else
					nr = hashF( (node->data.first) ) - 1;
				while( tab_ptr[nr] == NULL ){
					if( nr == 0 ) return(*this);
						nr--;
				}
				node = tab_ptr[nr];
				while( node->next != NULL )
					node = node->next;
				return *this;
			}
			else
				node = node->prev;
		return *this;
		}

		const_iterator operator--(int){
			const_iterator it = *this;
			--(*this);
			return it;
		}
		
		inline bool operator!=(const const_iterator& a) const { return node != a.node; }
		inline bool operator==(const const_iterator& a) const { return node == a.node; }
		
	};

	class iterator : public const_iterator{
		friend class AISDIHashMap;
		iterator(Node* x, Node **t) : const_iterator(x, t) {}
	
		public:
		iterator() {}
		iterator(const const_iterator& a) : const_iterator(a) {}
		iterator(const iterator& a){
			const_iterator::node = a.node;
			const_iterator::tab_ptr = a.tab_ptr;
		}
		inline std::pair<K, V>* operator->() const{ return &(const_iterator::node->data);}
		inline std::pair<K, V>& operator*() const { return const_iterator::node->data;}

		iterator& operator--(){
			--(*(const_iterator*)this);
			return (*this);
		}

		iterator operator--(int){
			iterator tmp = *this;
			--*this;
			return tmp;
		}
		
		iterator& operator++(){
			++(*(const_iterator*)this);
			return (*this);
		}
		
		iterator operator++(int){
			iterator tmp = *this;
			++*this;
			return tmp;
		}
	};
	   
	inline iterator begin(){
		if ( first == NULL )
			return end();
		return iterator( first, table );
	}
	
	inline const_iterator begin() const{
		if ( first == NULL )
			return const_iterator(end());
		return const_iterator( first, table );
	}
	
	inline iterator end(){
		return iterator( table[MAX_SIZE], table );
	}
	
	inline const_iterator end() const{
		return const_iterator( table[MAX_SIZE], table );
	}
	   
/// Inserts an element into the map.
/// @returns A pair whose bool component is true if an insertion was
///          made and false if the map already contained an element
///          associated with that key, and whose iterator component coresponds to
///          the address where a new element was inserted or where the element
///          was already located.
	std::pair<iterator, bool> insert(const std::pair<K, V>& entry){                  
		unsigned nr = hashF( entry.first );
		if (  table[nr] == NULL ){ // pusta kom
			table[nr] = new Node( std::make_pair( entry.first, entry.second ) );
			if ( first == NULL ) // pusta mapa
				first = table[nr];
			else if ( hashF(first->data.first) > hashF(entry.first) ) //nowy pierwszy
				first = table[nr];
			return std::make_pair( iterator( table[nr], table ), true );
		}
		for ( Node *wsk = table[nr]; wsk != NULL; wsk = wsk->next ) //jesli  istnieje
			if ( compFunc( wsk->data.first, entry.first) )
		return std::make_pair ( iterator(wsk, table), false );

		return std::make_pair ( unsafe_insert( entry ), true );
	}

	iterator unsafe_insert(const std::pair<K, V>& entry){
		unsigned nr = hashF( entry.first );
		if (  table[nr] == NULL ){ // pusta kom
			table[nr] = new Node( std::make_pair( entry.first, entry.second ) );
			if ( first == NULL ) // pusta mapa
				first = table[nr];
			else if ( hashF(first->data.first) > hashF(entry.first) ) //nowy pierwszy
				first = table[nr];
			return iterator( table[nr], table );
		}

		Node *preptr = table[nr];
		while ( preptr->next != NULL )
			preptr = preptr->next;

		preptr->next = new Node( std::make_pair( entry.first, entry.second ) );
		preptr->next->prev = preptr;
		return iterator( preptr->next, table );
	}
	   
/// Returns an iterator addressing the location of the entry in the map
/// that has a key equivalent to the specified one or the location succeeding the
/// last element in the map if there is no match for the key.
	iterator find(const K& k){
		unsigned nr = hashF(k);
		for (Node *wsk = table[nr]; wsk != NULL; wsk = wsk->next)
			if ( wsk->data.first == k )
				return iterator(wsk, table);
		return iterator( table[MAX_SIZE], table );
	}
	   
	const_iterator find(const K& k) const {
		unsigned nr = hashF(k);
		for ( Node *wsk = table[nr]; wsk != NULL; wsk = wsk->next )
			if ( compFunc( wsk->data.first, k ) )
				return const_iterator(wsk, table);
		return const_iterator( table[MAX_SIZE], table );
	}
	
/// Inserts an element into a map with a specified key value
/// if one with such a key value does not exist.
/// @returns Reference to the value component of the element defined by the key.
	V& operator[](const K& k){
		unsigned nr = hashF(k);
		iterator it = find(k);
		if ( it == end() )
			it = unsafe_insert( std::make_pair( k, V(0) ) );
		return it->second;
	}
	
/// Tests if a map is empty.
	bool empty( ) const { return ( first == NULL ); }
		   
  
/// Returns the number of elements in the map.
	size_type size() const {
		unsigned size = 0;
		Node *ptr;
		for ( unsigned i=0; i<MAX_SIZE; ++i ){
			ptr = table[i];
			while (ptr){
				size++;
				ptr = ptr->next;
			}
		}
		return size;
	}
	
/// Returns the number of elements in a map whose key matches a parameter-specified key.
	size_type count(const K& _Key) const {
		unsigned index = hashF( _Key );
		for (Node * ptr = table[index]; ptr != NULL; ptr = ptr->next )
			return  compFunc( ptr->data.first, _Key ); 
		return 0;
	}

/// Removes an element from the map.
/// @returns The iterator that designates the first element remaining beyond any elements removed.
	iterator erase(iterator i){
		if ( first == NULL || i==end() )
			return i;

		Node* tmp = i.node;
		unsigned k = hashF( i->first );
		if( tmp->next == NULL && tmp->prev == NULL){// if first
			if ( first == tmp)
				first = NULL; 
			delete tmp;
			table[k] = NULL;
			while( table[k]==NULL )
				k++;
			if( MAX_SIZE == k){
				first = table[MAX_SIZE];
				return iterator( table[MAX_SIZE], table );
			}
			i.node = table[k];
			if ( first == NULL )
				first = i.node;
			return iterator( i.node, table );
		}
		else if(tmp->next==NULL && tmp->prev!=NULL){
			i++;
			Node* tmp2 = tmp->prev;
			delete tmp;
			tmp2->next = NULL;
			return i;
		}
		else if(tmp->next!=NULL && tmp->prev==NULL){
			if ( first == tmp)
				first = tmp->next;
			Node* tmp2 = tmp->next;
			delete tmp;
			table[k] = tmp2;
			tmp2->prev = NULL;
			i.node=tmp2;
			return iterator(i.node, table);
		}
		else{
			Node* tmp2 = tmp->prev;
			Node* tmp3 = tmp->next;
			delete tmp;
			tmp2->next = tmp3;
			tmp3->prev = tmp2;
			i.node = tmp2;
			return iterator(i.node, table);
		}
	}
	   
/// Removes a range of elements from the map.
/// The range is defined by the key values of the first and last iterators
/// first is the first element removed and last is the element just beyond the last elemnt removed.
/// @returns The iterator that designates the first element remaining beyond any elements removed.
	iterator erase(iterator f, iterator last){
		if ( first == NULL ) // pusta mapa
			return end();
		unsigned ostatni = hashF( last->first );
		unsigned pierwszy = hashF( f->first );

		if ( pierwszy > ostatni )
			return end();
		while ( f != last )
			f = erase( f );
		return last;
	}
	   
/// Removes an element from the map.
/// @returns The number of elements that have been removed from the map.
///          Since this is not a multimap itshould be 1 or 0.
	size_type erase(const K& key){
		iterator i = find( key );
		i = erase( i );
		return i!=end();
	}

/// Erases all the elements of a map.
	void clear( ){ erase( begin(), end() ); }
};
