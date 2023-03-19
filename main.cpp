#include "perfect_powers.h"


int main(int argc, char *argv[]) {

	int M, R;
	string file_name;

	get_args(argc, argv, &M, &R, &file_name);

	/* Se initializeaza map-ul folosit pentru stocarea rezultatelor partiale */
	map<int, vector<list<int>>> Map;
	for (int i = 0; i < M; i++) {
		vector<list<int>> emptyVector(R);
		Map.insert({i, emptyVector});
	}

	int files_nr;
	string file;
	queue<string> files;

	ifstream in_file(file_name);
	in_file >> files_nr;

	for (int i = 0; i < files_nr; i++) {
		in_file >> file;
		files.push(file);
	}
	in_file.close();

	pthread_mutex_t mutex;
	pthread_barrier_t barrier;
	pthread_mutex_init(&mutex, NULL);
	pthread_barrier_init(&barrier, NULL, (M + R));

	Arg<int> arguments[M + R];
	
	for (int i = 0; i < (M + R); i++) {
		arguments[i].thread_id = i;
		arguments[i].M = &M;
		arguments[i].R = &R;
		arguments[i].files = &files;
		arguments[i].Map = &Map;
		arguments[i].mutex = &mutex;
		arguments[i].barrier = &barrier;
		arguments[i].opM = &mapper_perfect_powers;
		arguments[i].opR = &reducer_perfect_powers;
	}

	pthread_t tid[M + R];

	// Se creeaza thread-urile
	for (int i = 0; i < (M + R); i++) {
		pthread_create(&tid[i], NULL, thread_function, &arguments[i]);
	}

	// Se asteapta thread-urile
	for (int i = 0; i < (M + R); i++) {
		pthread_join(tid[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);

	return 0;
}
