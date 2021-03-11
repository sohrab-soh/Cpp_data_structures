#include<iostream>
using namespace std;
class Underflow {};
template<class T>class QueueArrIter;
template<class T>class QueueLinkIter;
template<class T>class QueueListIter;
template<class T>class List;
template<class T>class QueueLink;


template<class T>
class Queue
{
public:

	virtual void makeEmpty() = 0;
	virtual int size()const = 0;
	virtual T dequeue()throw(Underflow) = 0;
	virtual void enqueue(const T&) = 0;
	virtual void print()const = 0;
};


template<class T>
class QueueArr :public Queue<T>
{

private:
	int arSize;
	T*theArray;
	int population;
	int front;
	int back;
	void increment(int&x)const;
public:

	QueueArr(int capacity = 10);
	virtual~QueueArr();
	QueueArr(const QueueArr<T>&q);
	void makeEmpty();
	bool isFull()const;
	bool isEmpty()const;
	int size()const { return population; }
	T getFront()const throw(Underflow);
	T dequeue()throw(Underflow);
	void enqueue(const T& x);
	void doubleQueue();
	void print()const;
	friend class QueueArrIter<T>;
};

template<class T>
void QueueArr<T>::increment(int& x)const
{
	x++;
	if (x == arSize)
		x = 0;
}

template<class T>
QueueArr<T>::QueueArr(int capacity) :arSize(capacity), theArray(new T[arSize])
{
	makeEmpty();
}

template<class T>
void QueueArr<T>::makeEmpty()
{
	population = 0;
	front = 0;
	back = -1;
}

template<class T>
QueueArr<T>::~QueueArr() { delete[]theArray; }

template<class T>
QueueArr<T>::QueueArr(const QueueArr<T>&q)
{
	arSize = q.arSize;
	population = q.population;
	theArray = new T[arSize];
	int frontH = q.front;
	for (int i = 0; i < population; i++, increment(frontH))
	{
		theArray[frontH] = q.theArray[frontH];
	}
	front = q.front;
	back = q.back;
}

template<class T>
bool QueueArr<T>::isEmpty()const
{
	return(population == 0);
}

template<class T>
bool QueueArr<T>::isFull()const
{
	return (population == arSize);
}

template <class T>
void QueueArr<T>::doubleQueue() {
	T * newArray;

	newArray = new T[arSize * 2];
	for (int i = 0; i<population; i++, increment(front))
		newArray[i] = theArray[front];
	delete[]theArray;
	arSize = arSize * 2;
	theArray = newArray;
	front = 0;
	back = population - 1;
}

template <class T>
T QueueArr<T>::getFront()const throw(Underflow)
{
	if (isEmpty())
		throw Underflow();
	return theArray[front];
}

template <class T>
T QueueArr<T>::dequeue()throw(Underflow)
{
	if (isEmpty())
		throw Underflow();

	population--;
	T frontItem = theArray[front];
	increment(front);
	return frontItem;
}

template <class T>
void QueueArr<T>::enqueue(const T & x)
{
	if (isFull())
		doubleQueue();
	increment(back);
	theArray[back] = x;
	population++;
}

template<class T>
void QueueArr<T>::print()const {
	cout << '<';
	int frontH = front;
	for (int i = 0; i<population; i++, increment(frontH))
		cout << theArray[frontH] << ',';
	cout << ']';
}




template<class T>
class Link
{
private:
	T info;
	Link<T>*next;
public:
	Link(T i, Link<T>*n = NULL) :info(i), next(n) {}
	friend class QueueLinkIter<T>;
	friend class QueueLink<T>;
	friend class QueueListIter<T>;
	friend class List<T>;
};





template<class T>
class QueueLink :public Queue<T>
{
private:
	Link<T>*front;
	Link<T>*back;
public:
	QueueLink() :front(NULL), back(NULL) {}
	virtual~QueueLink()
	{
		makeEmpty();
	}

	QueueLink(const QueueLink<T>& q) :front(NULL), back(NULL)
	{
		Link<T>*h = q.front;
		while (h)
		{
			enqueue(h->info);
			h = h->next;
		}
	}

	void makeEmpty()
	{
		while (front)
		{
			Link<T>*h = front->next;
			delete front;
			front = h;
		}
		back = front;
	}



	void enqueue(const T& e)
	{
		if (size() == 0)
		{
			front = back = new Link<T>(e);
		}
		else
			back = back->next = new Link<T>(e);
	}
	T dequeue()throw(Underflow)
	{
		if (size() == 0)
			throw Underflow();

		T x = front->info;
		Link<T>*temp_front = front->next;
		delete front;
		front = temp_front;
		if (!front)
			back = NULL;
		return x;
	}
	int size()const
	{
		int n = 0;
		Link<T>*h = front;
		while (h)
		{
			h = h->next;
			n++;
		}
		return n;
	}

	void print()const
	{
		cout << "<";
		Link<T>*h = front;
		while (h)
		{
			cout << h->info << ",";
			h = h->next;
		}
		cout << "]";
	}
	friend class QueueLinkIter<T>;
};


template<class T>
class List {

public:

	List() : head(NULL) {}
	~List()
	{
		while (head)
		{
			Link<T>* h = head->next;
			delete head;
			head = h;
		}
	}
	List(List<T>& L) :head(0L) {
		Link<T>* temp = L.head;
		int i = 1;
		while (temp != 0L)
		{
			insert(temp->info, i);
			i++;
			temp = temp->next;
		}
	}
	void makeEmpty()
	{
		while (head)
		{
			Link<T>*h = head->next;
			delete head;
			head = h;
		}
	}
	int size()const
	{
		int n = 0;
		Link<T>* h = head;
		while (h)
		{
			n++;
			h = h->next;
		}
		return n;
	}
	Link<T>* pos(int i)const throw(Underflow)
	{
		if ((i<1) || (i>size())) // ---> baraye size parantez baz va baste nzashte budi!
		{
			throw Underflow();
		}
		int n = 1;
		Link<T>* h = head;
		while (n<i)
		{
			h = h->next;
			n++;
		}
		return h;
	}



	List<T>& insert(T e, int p = 1)throw (Underflow)
	{
		if (p == 1)
		{
			head = new Link<T>(e, head);
		}
		else
		{
			Link<T>* h = pos(p - 1);
			h->next = new Link<T>(e, h->next);
		}
		return *this;
	}

	T retrive(int p)const throw(Underflow)
	{
		if (p<1 || p>size())
			throw Underflow();
		return pos(p)->info;
	}

	void print()const
	{
		Link<T>* h = head;
		cout << "<";
		while (h)
		{
			cout << h->info << ",";
			h = h->next;
		}
		cout << "]";
	}
	List<T>& remove(int p)throw (Underflow)
	{
		if (p<1 || p>size())
		{
			throw Underflow();
		}
		if (p == 1)
		{
			Link<T>*h = head->next;
			delete head;
			head = h;
		}
		else
		{
			Link<T>* temp = pos(p - 1);
			Link<T>* temp2 = temp->next;
			temp->next = temp2->next;
			delete temp2;
		}
		return *this;
	}
	friend class QueueListIter<T>;
private:
	Link<T>*head;
};

template<class T>
class QueueList :public Queue<T>
{
private:
	List<T>L;
public:
	virtual ~QueueList() {}
	int size()const
	{
		return L.size();
	}
	void makeEmpty()
	{
		L.makeEmpty();
	}
	void enqueue(const T& e)
	{
		L.insert(e, L.size() + 1);
	}
	T dequeue()throw(Underflow)
	{
		T x = L.retrive(1);
		L.remove(1);
		return x;
	}
	void print()const
	{
		L.print();
	}
	friend class QueueListIter<T>;
};




template<class T>
class QueueIter
{
public:
	virtual bool is_valid()const = 0;
	virtual void advance()throw(Underflow) = 0;
	virtual void operator++(int)throw (Underflow) = 0;
	virtual void operator++()throw (Underflow) = 0;
	virtual T& retrive()const = 0;
	virtual T& operator*()const throw (Underflow) = 0;
	virtual void reset() = 0;

};


template<class T>
class QueueLinkIter :public QueueIter<T>
{
private:
	QueueLink<T>&q;
	Link<T>*current;
public:
	QueueLinkIter(QueueLink<T>&qi) :q(qi), current(q.front) {}
	virtual ~QueueLinkIter() {}
	bool is_valid()const
	{
		return (current != NULL);
	}
	void advance()throw(Underflow)
	{
		if (!current)
			throw Underflow();
		current = current->next;
	}
	void operator++(int)throw(Underflow)
	{
		advance();
	}
	void operator++()throw(Underflow)
	{
		advance();
	}
	T& retrive()const throw(Underflow)
	{
		if (!current)
			throw Underflow();
		return (current->info);
	}
	T& operator*()const throw(Underflow)
	{
		return retrive();
	}
	void reset()
	{
		current = q.front;
	}
};





template<class T>
class QueueArrIter :public QueueIter<T>
{
private:
	QueueArr<T>&q;
	int current;
	int n;
public:
	QueueArrIter(QueueArr<T>&qi) :q(qi), current(q.front), n(0) {}
	virtual ~QueueArrIter() {}
	bool is_valid()const
	{
		return(n != q.population);
	}
	void advance()throw(Underflow)
	{
		if (n == q.population)
			throw Underflow();
		q.increment(current);
		n++;
	}
	void operator++(int)throw (Underflow)
	{
		advance();
	}
	void operator++()throw(Underflow)
	{
		advance();
	}
	T& retrive()const throw (Underflow)
	{
		if (n == q.population)
			throw Underflow();
		return q.theArray[current];
	}
	T& operator*()const throw (Underflow)
	{
		return retrive();
	}
	void reset()
	{
		n = 0;
		current = q.front;
	}
};



template<class T>
class QueueListIter :public QueueIter<T>
{
private:
	QueueList<T>&q;
	Link<T>*current;
public:
	QueueListIter(QueueList<T>&qi) :q(qi), current(q.L.head) {}
	virtual ~QueueListIter() {}
	bool is_valid()const
	{
		return (current != NULL);
	}
	void advance()throw(Underflow)
	{
		if (!current)
			throw Underflow();
		current = current->next;
	}
	void operator++(int)throw (Underflow)
	{
		advance();
	}
	void operator++()throw(Underflow)
	{
		advance();
	}

	T& retrive()const throw(Underflow)
	{
		if (!current)
			throw Underflow();
		return current->info;
	}
	T& operator*()const throw(Underflow)
	{
		return retrive();
	}
	void reset()
	{
		current = q.L.head;
	}

};



int main()
{
	/*
	try{

	QueueArr<int>b;
	b.enqueue(1);
	b.enqueue(2);
	b.enqueue(3);
	b.enqueue(4);
	b.enqueue(5);
	b.enqueue(6);
	b.enqueue(7);
	b.enqueue(8);
	cout << b.size() << endl;
	b.print();
	cout << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	}
	catch (...)
	{
	cout << "Underflow" << endl;
	}
	*/



	/*
	try{

	QueueLink<int>b;
	b.enqueue(1);
	b.enqueue(2);
	b.enqueue(3);
	b.enqueue(4);
	b.enqueue(5);
	b.enqueue(6);
	b.enqueue(7);
	b.enqueue(8);
	cout << b.size() << endl;
	b.print();
	cout << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	}
	catch (...)
	{
	cout << "Underflow" << endl;
	}
	*/

	/*
	try {
	/*
	QueueList<int>b;
	b.enqueue(1);
	b.enqueue(2);
	b.enqueue(3);
	b.enqueue(4);
	b.enqueue(5);
	b.enqueue(6);
	b.enqueue(7);
	b.enqueue(8);
	cout << b.size() << endl;
	b.print();
	cout << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	cout << b.dequeue() << endl;
	}
	catch (...)
	{
	cout << "Underflow" << endl;
	}
	*/
	/*
	try
	{
	QueueLink<int>b;
	b.enqueue(10);
	b.enqueue(20);
	b.enqueue(30);
	b.enqueue(40);
	b.enqueue(50);
	QueueLinkIter<int>j = b;
	double sum = 0;
	for (; j.is_valid(); j++)
	{
	sum += *j;
	}
	cout << sum << endl;
	j.reset();

	for (; j.is_valid(); j++)
	{
	sum += *j;
	}
	cout << sum << endl;
	}
	catch (...)
	{
	cout << "Underflow error";
	}
	*/

	/*
	try {
	QueueLink<int>b;
	b.enqueue(10);
	b.enqueue(20);
	b.enqueue(30);
	b.enqueue(40);
	b.enqueue(50);
	QueueLinkIter<int>j = b;
	double sum = 0;
	for (; j.is_valid(); j++)
	{
	sum += *j;
	}
	cout << sum << endl;
	j.reset();

	for (; j.is_valid(); j++)
	{
	sum += *j;
	}
	cout << sum << endl;
	}
	catch (...)
	{
	cout << "Underflow error";
	}
	*/
	/*
	try {
		QueueArr<int>b;
		b.enqueue(10);
		b.enqueue(20);
		b.enqueue(30);
		b.enqueue(40);
		b.enqueue(50);
		QueueArrIter<int>j = b;
		double sum = 0;
		for (; j.is_valid(); j++)
		{
			sum += *j;
		}
		cout << sum << endl;
		j.reset();

		for (; j.is_valid(); j++)
		{
			sum += *j;
		}
		cout << sum << endl;
	}
	catch (...)
	{
		cout << "Underflow error";
	}
	*/

	/*
	try {
	QueueList<int>b;
	b.enqueue(10);
	b.enqueue(20);
	b.enqueue(30);
	b.enqueue(40);
	b.enqueue(50);
	QueueListIter<int>j = b;
	double sum = 0;
	for (; j.is_valid(); j++)
	{
	sum += *j;
	}
	cout << sum << endl;
	j.reset();

	for (; j.is_valid(); j++)
	{
	sum += *j;
	}
	cout << sum << endl;
	}
	catch (...)
	{
	cout << "Underflow error";
	}
	*/
	system("pause");
}