#include "map_reduce.h"


void get_args(int argc, char **argv, int *M, int *R, string *file_name) {
	if(argc < 3) {
		exit(1);
	}

	*M = atoi(argv[1]);
	*R = atoi(argv[2]);
	*file_name = argv[3];
}

void *thread_function(void *arg) {
	Arg<void*> argument = *((Arg<void*>*) arg);
	int id = argument.thread_id;
	int M = *(argument.M);
	mapper_ops opM = argument.opM;
	reducer_ops opR = argument.opR;

	if (id < M) { // Mapper
		mapper(arg, opM);
	}

	pthread_barrier_wait(argument.barrier);

	if (id >= M) { // Reducer
		reducer(arg, opR);
	}

	pthread_exit(NULL);
}

void mapper(void *arg, mapper_ops op) {
	Arg<void*> argument = *((Arg<void*>*) arg);
	queue<string> *files = argument.files;

	while (true) {

		pthread_mutex_lock(argument.mutex);

		if (files->empty()) { // s-a terminat citirea tuturor fisierelor
			pthread_mutex_unlock(argument.mutex);
			break;
		}

		string file = files->front();
		files->pop();

		pthread_mutex_unlock(argument.mutex);

		/* aplicare operatii asupra valorilor citite din fisierul obtinut la alocare */
		op(arg, file);
	}
}

void reducer(void *arg, reducer_ops op) {
	Arg<void*> argument = *((Arg<void*>*) arg);
	int id = argument.thread_id;
	int M = *(argument.M);
	map<int, vector<list<void*>>> *Map = argument.Map;

	/* Se combina listele corespunzatoare thread-ului reducer curent */
	list<void*> L;
	for (int i = 0; i < M; i++) {
		vector<list<void*>> v = Map->at(i);
		L.splice(L.end(), v.at(id - M));
	}

	/* prelucrarea datelor obtinute si scrierea rezultatului */
	op(arg, L);
}
