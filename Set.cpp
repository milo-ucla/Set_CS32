
#include "Set.h"


Set::Set() {
	head = &dummy;
	dummy.m_next = head;
	dummy.m_prev = head;
}// Create an empty set (i.e., one with no items).

Set::Set(const Set& set) {
	head = &dummy;
	dummy.m_next = head;
	dummy.m_prev = head;

	//now I will iterate through every item in the other set and add it to this one
	Node* p = set.dummy.m_next;
	//std::cerr << "Adding items to new Set" << std::endl;
	while (p != set.head) {
		insert(p->m_value);
		//std::cerr << "Added item " << p->m_value << " from " <<p << std::endl;
		p = p->m_next;
	}
}
Set::~Set() {
	//go through every Node item and delete it
	Node* p = dummy.m_next;
	Node* n;
	while (p != head) {
		n = p->m_next;
		delete p;
		p = n;
	}
}

Set& Set::operator= (const Set& rhs) {
	//anti-aliasing
	if(&rhs == this){
		return *this;
	}

	Node* p = dummy.m_next;
	Node* n;
	//clear this set
	while (p != head) {
		n = p->m_next;
		delete p;
		p = n;
	}
	//add every item in the other set to this set
	p = rhs.dummy.m_next;
	dummy.m_next = head;
	dummy.m_prev = head;
	while (p != rhs.head) {
		insert(p->m_value); // insert the value of the rhs to this set
		p = p->m_next;
	}
	return *this;
}


bool Set::empty() const {
	return size() == 0;
}// Return true if the set is empty, otherwise false.

int Set::size() const {
	Node* p = dummy.m_next;
	int size = 0;
	while (p != head) {
		size++;
		p = p->m_next;
	}
	return size;
}// Return the number of items in the set.

bool Set::insert(const ItemType& value) {
	Node* p = dummy.m_next;
	//check if the set contains the value already
	while (p != head) {
		if (p->m_value == value)
			return false;
		p = p->m_next;
	}
	//add new node and adjust m_nexts and m_prevs accordingly 
	Node* q = new Node;
	q->m_value = value;

	q->m_next = head;
	p->m_prev->m_next = q;
	q->m_prev = p->m_prev;
	head->m_prev = q;

	return true;

}
// Insert value into the set if it is not already present.  Return
// true if the value is actually inserted.  Leave the set unchanged
// and return false if the value was not inserted (perhaps because it
// was already in the set or because the set has a fixed capacity and
// is full).

bool Set::erase(const ItemType& value) {

	//locate the value via iteration
	Node* p = dummy.m_next;

	while (p != head) {
		//make the next node's m_prev point to the last node, and the last node's m_next point to the next node
		//delete p and return true
		if (p->m_value == value){
			p->m_prev->m_next = p->m_next;
			p->m_next->m_prev = p->m_prev;
			delete p;
			return true;
		}
		p = p->m_next;
	}
	return false;


}
// Remove the value from the set if it is present.  Return true if the
// value was removed; otherwise, leave the set unchanged and
// return false.

bool Set::contains(const ItemType& value) const {
	//iterate through every node and check if m_value == value
	Node* p = dummy.m_next;

	while (p != head) {
		if (p->m_value == value) {
			return true;
		}
		p = p->m_next;
	}
	return false;
}
// Return true if the value is in the set, otherwise false.

bool Set::get(int i, ItemType& value) const {
	int count;
	for (Node* p = dummy.m_next; p != head; p = p->m_next) { //check every item against every other item
		count = 0;
		for (Node* q = dummy.m_next; q!=head; q = q->m_next) {
			if (p->m_value > q->m_value) { //if strictly >, then increase count
				count++;
			}
		}
		if (count == i) { //compare count to desire i
			value = p->m_value;
			return true;
		}
	}
	return false;
}
// If 0 <= i < size(), copy into value the item in the set that is
// strictly greater than exactly i items in the set and return true.
// Otherwise, leave value unchanged and return false.

void Set::swap(Set& other) {
	//swap the head pointers so that the while loops still work
	Node* temp_pointer = other.head;
	other.head = head;
	head = temp_pointer;

	//swap the dummy node next and prev pointers to effectively swap the entire list
	temp_pointer = other.dummy.m_next;
	other.dummy.m_next = dummy.m_next;
	dummy.m_next = temp_pointer;

	temp_pointer = other.dummy.m_prev;
	other.dummy.m_prev = dummy.m_prev;
	dummy.m_prev = temp_pointer;

	
}
//prints debugging data to cerr
void Set::dump() const {
	Node* p = dummy.m_next;
	std::cerr << std::endl << "dump: " << std::endl;
	std::cerr << "head: " << head << std::endl << "tail: " << dummy.m_prev << std::endl;
	std::cerr << "size: " << size() << std::endl;
	while (p != head) {
		std::cerr << p << "::" << p->m_value << " points to " << p->m_next << std::endl;
		p = p->m_next;
	}
	std::cerr << std::endl;
}

void unite(const Set& s1, const Set& s2, Set& result) {
	result = s1;
	ItemType oop;
	for (int i = 0; i < s2.size(); i++) {
		s2.get(i, oop);
		result.insert(oop);
	}
}

void subtract(const Set& s1, const Set& s2, Set& result) {
	result = s1;
	ItemType x;
	for (int i = 0; i < s2.size(); i++) {
		s2.get(i, x);
		result.erase(x);
	}
}
