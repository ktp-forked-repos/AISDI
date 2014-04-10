/** 
@file asd.cc
 
Plik do modyfikacji w w ramach cwiczenia 4 z AISDI.
Zawiera niekompletne implementacje metod klasy TreeMap.
Jest tez prosta funkcja testujaca (void test()), ktora
jest wolana w funkcji main. Mozna w niej zaimplementowac
wlasne testy.
NALEZY ZMODYFIKOWAC I UZUPELNIC CIALA METOD KLASY TreeMap.
 
@author
Pawel Cichocki, Michal Nowacki
 
@date  
last revision
- 2005.12.01 Pawel Cichocki: TreeNodeDetail class
- 2005.12.01 Michal Nowacki: lab #4
- 2005.11.17 Michal Nowacki: lab #3, copy operator and constructor
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
#include "/home/common/dyd/aisdi/tree/TreeMap.h" 
#else
#include "TreeMap.h"
#endif
 
/// A helper class.
class TreeMapDetail //Helper
{
protected:
    friend class TreeMap;
 
    /// Stupid example of a method that modifies a protected field in 
    /// the TreeMap class. Feel free to remove this method or add new
    /// ones here.
    static void erase(TreeMap* tm, const TreeMap::Key& k)
    {
        tm->root=NULL; // we just modified a protected field in tree map
    }
};

 
void print(const std::pair<int, std::string>& p);
 
//////////////////////////////////////////////////////////////////////////////
// TreeMap and TreeMap::iterator methods
//////////////////////////////////////////////////////////////////////////////

TreeNode * last = NULL;
 
TreeMap::TreeMap()
{
	root = NULL;
};
 
/// Content of existing TreeMap object is copied into the new object. 
TreeMap::TreeMap( const TreeMap& m )
{
	root = NULL;
	*this = m;
};
 
TreeMap::~TreeMap()
{
	clear();
};
 
// Inserts an element into the map.
// @returns A pair whose bool component is true if an insertion was
//             made and false if the map already contained an element
//             associated with that key, and whose iterator component coresponds to
//             the address where a new element was inserted or where the element
//             was already located.
std::pair<TreeMap::iterator, bool> TreeMap::insert(const std::pair<Key, Val>& entry)
{
	iterator it = find(entry.first);
	if (it == end())
		return std::make_pair(unsafe_insert(entry), true);
	else
		return std::make_pair(it, false);
}
 
// Inserts an element into the map.
// This method assumes there is no value asociated with
// such a key in the map.
TreeMap::iterator TreeMap::unsafe_insert(const std::pair<Key, Val>& entry)
{
	if (!root){
		root = new Node(entry);
		return iterator(root);
	}
	Node * n = root;
	while (1){
	if (entry.first < n->data.first){
		if (n->left)
			n = n->left;
		else{
			n->left = new Node(entry, n);
			n = n->left;
			break;
		}
	}
	else{
		if (n->right)
			n = n->right;
		else{
			n->right = new Node(entry, n);
			n = n->right;
			break;
		}
	}
	}
	return iterator(n);
}
 
// Returns an iterator addressing the location of the entry in the map
// that has a key equivalent to the specified one or the location succeeding the
// last element in the map if there is no match for the key.
TreeMap::iterator TreeMap::find(const Key& k)
{
	if (!root)
		return end();
	Node * tmp = root;
	while (tmp && tmp->data.first != k){
		if (k < tmp->data.first)
			tmp = tmp->left;
		else
			tmp = tmp->right;
	}
	return iterator(tmp);
}
 
TreeMap::const_iterator TreeMap::find(const Key& k) const
{
	if (!root)
		return end();
	Node * tmp = root;
	while (tmp && tmp->data.first != k){
		if (k < tmp->data.first)
			tmp = tmp->left;
		else
			tmp = tmp->right;
	}
	return const_iterator(tmp);
}
 
// Inserts an element into a map with a specified key value
// if one with such a key value does not exist.
// @returns Reference to the value component of the element defined by the key.
TreeMap::Val& TreeMap::operator[](const Key& k)
{
    return insert(P(k, "temp")).first->second;
}
 
// Tests if a map is empty.
bool TreeMap::empty( ) const
{
	return root==NULL;
}
 
// Returns the number of elements in the map.
TreeMap::size_type TreeMap::size( ) const
{
	size_type size = 0;
	iterator it = begin();
	while (it != end()){
		++size;
		++it;
	}
	return size;
}
 
// Returns the number of elements in a map whose key matches a parameter-specified key.
TreeMap::size_type TreeMap::count(const Key& _Key) const
{
	return (find(_Key) != end()); // this is not a multimap
}
 
// Removes an element from the map.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator i)
{
	if (i == end())
		return i;
	iterator it(i);
	++it;
	if (erase(i->first))
		return it;
	return i;
}
 
// Removes a range of elements from the map.
// The range is defined by the key values of the first and last iterators
// first is the first element removed and last is the element just beyond the last elemnt removed.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator f, TreeMap::iterator l)
{
	while (f != l && f != end())
		f = erase(f);
	return f;
}
 
// Removes an element from the map.
// @returns The number of elements that have been removed from the map.
//             Since this is not a multimap itshould be 1 or 0.
TreeMap::size_type TreeMap::erase(const Key& key)
{
	if (!root)
		return 0;
	Node * tmp = root;
	while (tmp && tmp->data.first != key){
		if (key < tmp->data.first)
			tmp = tmp->left;
		else
			tmp = tmp->right;
	}
	if (!tmp)
		return 0;
	if (tmp->left && tmp->right){
		Node * tmp2 = tmp->right;
		while (tmp2->left)
			tmp2 = tmp2->left;
		if (tmp2->parent != tmp){
			tmp2->parent->left = tmp2->right;
			if (tmp2->right)
				tmp2->right->parent = tmp2->parent;
			tmp2->right = tmp->right;
			tmp2->right->parent = tmp2;
		}
		tmp2->left = tmp->left;
		tmp2->left->parent = tmp2;
		tmp2->parent = tmp->parent;
		if (tmp->parent){
			if (tmp->parent->left == tmp)
				tmp2->parent->left = tmp2;
			else
				tmp2->parent->right = tmp2;
		}
		else
			root = tmp2;
	}
	else if (tmp->left){
		if (tmp->parent){
			if (tmp->parent->left == tmp)
				tmp->parent->left = tmp->left;
			else
				tmp->parent->right = tmp->left;
			tmp->left->parent = tmp->parent;
		}
		else{
			root = tmp->left;
			root->parent = NULL;
		}
	}
	else if (tmp->right){
		if (tmp->parent){
			if (tmp->parent->left == tmp)
				tmp->parent->left = tmp->right;
			else
				tmp->parent->right = tmp->right;
			tmp->right->parent = tmp->parent;
		}
		else{
			root = tmp->right;
			root->parent = NULL;
		}
	}
	else{
		if (tmp->parent){
			if (tmp->parent->left == tmp)
				tmp->parent->left = NULL;
			else
				tmp->parent->right = NULL;
		}
		else
			root = NULL;
	}
	delete tmp;
	return 1;
}
 
// Erases all the elements of a map.
void TreeMap::clear()
{
	Node * tmp = root;
	while (tmp){
		if (tmp->left)
			tmp = tmp->left;
		else if (tmp->right)
			tmp = tmp->right;
		else{
			Node * tmp2 = tmp->parent;
			if (tmp2){
				if (tmp2->left == tmp)
					tmp2->left = NULL;
				else
					tmp2->right = NULL;
			}
			delete tmp;
			tmp = tmp2;
		}
	}
	root = NULL;
}
 
bool TreeMap::struct_eq(const TreeMap& another) const
{
	if (size() != another.size())
		return false;
	iterator tmp = begin();
	iterator tmp2 = another.begin();

	while (
	tmp != end()
	&& tmp2 != another.end()
	&& *tmp == *tmp2
	&& (!tmp.node->parent || tmp.node->parent->data == tmp2.node->parent->data)
	&& (!tmp.node->left || tmp.node->left->data == tmp2.node->left->data)
	&& (!tmp.node->right || tmp.node->right->data == tmp2.node->right->data)
	&& ((tmp.node->parent && tmp2.node->parent) || (!tmp.node->parent && !tmp2.node->parent))
	&& ((tmp.node->left && tmp2.node->left) || (!tmp.node->left && !tmp2.node->left))
	&& ((tmp.node->right && tmp2.node->right) || (!tmp.node->right && !tmp2.node->right))
	){
		tmp++;
		tmp2++;
	}
	if (tmp == end() && tmp2 == another.end())
		return true;
		
	return false;
}
 
bool TreeMap::info_eq(const TreeMap& another) const
{
	if (size() != another.size())
		return false;
	for (iterator it = begin(); it != end(); ++it){
		if (another.find(it->first) == another.end())
			return false;
	}
	return true;
}
 
// preincrement
TreeMap::const_iterator& TreeMap::const_iterator::operator ++()
{
	if (node){
		if (node->right){
			node = node->right;
			while (node->left)
				node = node->left;
		}
		else{
			last = node;
			Node * n = node->parent;
			while (n && n->right == node){
				node = n;
				n = n->parent;
			}
			node = n;
			if (node)
				last = node;
		}
	}
	return *this;
}
 
// postincrement
TreeMap::const_iterator TreeMap::const_iterator::operator++(int)
{
	iterator it = *this;
	++*this;
	return it;
}
 
// predecrement
TreeMap::const_iterator& TreeMap::const_iterator::operator--()
{
	if (node){
		if (node->left){
			node = node->left;
			while (node->right)
				node = node->right;
		}
		else{
			Node * n = node->parent;
			while (n && n->left == node){
				node = n;
				n = n->parent;
			}
			node = n;
		}
	}
	else
	{
	node = last;
	}
	return *this;
}
 
// postdecrement
TreeMap::const_iterator TreeMap::const_iterator::operator--(int)
{
	const_iterator it = *this;
	--*this;
	return it;
}
  
/// Assignment operator copy the source elements into this object.
TreeMap& TreeMap::operator=(const TreeMap& m)
{
	if (&m == this)
		return *this;
	clear();
	Node * tmp = m.root;
	while (tmp){
		unsafe_insert(tmp->data);
		if (tmp->left)
			tmp = tmp->left;
		else if (tmp->right)
			tmp = tmp->right;
		else{
			while (tmp->parent){
				if (tmp->parent->left == tmp && tmp->parent->right){
					tmp = tmp->parent->right;
					break;
				}
				else
					tmp = tmp->parent;
			}
			if (!tmp->parent)
				tmp = NULL;
		}
	}
	return *this;
}
 
/// Returns an iterator addressing the first element in the map
TreeMap::iterator TreeMap::begin()
{ 
	if (!root)
		return end();
	Node * tmp = root;
	if (tmp)
		while (tmp->left)
			tmp = tmp->left;
	return iterator(tmp);
}
 
TreeMap::const_iterator TreeMap::begin() const
{ 
	if (!root)
		return end();
	Node * tmp = root;
	if (tmp)
		while (tmp->left)
			tmp = tmp->left;
	return const_iterator(tmp);
}
 
/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::iterator TreeMap::end()
{ 
    return iterator(NULL);
}
 
/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::const_iterator TreeMap::end() const
{ 
    return const_iterator(NULL);
}
 
//////////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////////

/// A helper function that outputs a key-value pair.
void print(const std::pair<int, std::string>& p)
{
   std::cout<<p.first<<", "<<p.second<<std::endl;
}

#include <map>

/// The big mean test function ;)
void test()
{
   // A typedef used by the test.
   typedef std::map<int, std::string> TEST_MAP;
   //typedef SmallMap<int, std::string> TEST_MAP;
   //typedef TreeMap TEST_MAP;

   std::cout << "Testy uzytkownika" << std::endl;

        TEST_MAP m;  
   
   m[2] = "Merry";
   m[4] = "Jane";
   m[8] = "Korwin";
   m[4] = "Magdalena";

   for_each(m.begin(), m.end(), print );
   //system("PAUSE");

}

//////////////////////////////////////////////////////////////////////////////
// main - jest w pliku /home/common/dyd/aisdi/z2/main.cc
//////////////////////////////////////////////////////////////////////////////

//int main()
//{
//   std::cout << "AISDI cwiczenie 4: wchodze do funkcji main." << std::endl;
//   test();
//   // Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
//   Test2();
//   //system("PAUSE");
//   return EXIT_SUCCESS;
//}
