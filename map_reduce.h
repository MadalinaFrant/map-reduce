#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <queue>
#include <map>
#include <vector>
#include <list>
#include <set>

using namespace std;

/* tipuri de functii ce vor fi date ca parametri functiilor de mapper,
respectiv reducer, pentru ca acestea sa execute orice tip de operatii */
typedef void (*mapper_ops)(void *arg, string file);
typedef void (*reducer_ops)(void *arg, list<void*> L);

/* structura definita pentru a trimite parametrii necesari functiei de thread */
template <typename T> struct Arg {
	int thread_id; // id-ul (unic) al thread-ului curent
	int *M, *R; // numarul de thread-uri mapper, respectiv reducer
	queue<string> *files; // coada fisierelor din care citesc thread-urile mapper
	map<int, vector<list<T>>> *Map; // map pentru stocare rezultate partiale
	pthread_mutex_t *mutex;
	pthread_barrier_t *barrier;
	mapper_ops opM; // operatii executate de fiecare mapper
	reducer_ops opR; // operatii executate de fiecare reducer
};

/* citeste numarul de thread-uri mapper si reducer si fisierul din care se citesc
fisierele din care vor citi thread-urile mapper */
void get_args(int argc, char **argv, int *M, int *R, string *file_name);

/* functia de thread; in cadrul acesteia se executa mai intai operatiile 
corespunzatoare thread-urilor mapper, apoi cele ale thread-urilor reducer */
void *thread_function(void *arg);

/* realizeaza operatiile corespunzatoare mapper-ului */
void mapper(void *arg, mapper_ops op);

/* realizeaza operatiile corespunzatoare reducer-ului */
void reducer(void *arg, reducer_ops op);
