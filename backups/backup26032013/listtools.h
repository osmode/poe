//header file 'listtools.h' containing type definitions and
//function declarations for manipulating a linked list to store
//data of any type T. 
//The implementation of the functions is given in file 'listtools.cpp'

#ifndef LISTTOOLS_H
#define LISTTOOLS_H

namespace LinkedListOM
{
	template<class T>
	class Node
	{
	public:
		Node(const T& theData, Node<T>* theLink) :
			data(theData), link(theLink) {}
		Node<T>* getLink() const { return link; }
		const T getData() const { return data; }
		void setData(const T& theData) { data = theData; }
		void setLink(Node<T>* pointer) { link = pointer; }
	private:
		Node<T> *link;
		T data;

	};

	//precondition: pointer variable 'head' points to the
	//head of a linked list
	//postcondition: a new node containing 'theData'
	//has een added at the head of the linked list
	template<class T>
	void headInsert(Node<T>*& head, const T& theData);

	//precondition: the pointer 'before' points to a node
	//that has at least one node after it in the linked list
	//postcondition: the node after the node pointed to by
	//'before' has been removed from the linke list and its
	//storage returned to the freestore
	template<class T>
	void deleteNode(Node<T>* before);

	//precondition: pointer 'head' points to the first node in
	//a linked list with at least one node.
	//postcondition: node pointed to by 'head' has been
	//removed from the linekd list and its storage returned
	//to freestore
	template<class T>
	void deleteFirstNode(Node<T>*& head);

	//precondition: pointer 'head' points to the head of a
	//linked list. The pointer variable in the last node is
	//NULL. '==' is defined for type T
	//if the list is empty, then head is NULL.
	//returns a pointer that points to the first node that
	//is equal to the target. If no node equals the target,
	//then the function returns NULL.
	template<class T>
	Node<T>* search(Node<T>* head, const T& target);

} //close namespace LinkedListOM

#endif


