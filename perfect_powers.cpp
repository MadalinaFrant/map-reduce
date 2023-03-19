#include "perfect_powers.h"


bool is_power_of(int n, int e) {

	if (n <= 0) {
		return false;
	}
	if (n == 1) {
		return true;
	}

	/* Pentru a determina existenta unui numar care, ridicat la puterea e, 
	este egal cu n se foloseste algoritmul de cautare binara pentru a obtine 
	o complexitate mai buna */

	int left = 2, right = n;

	while (left <= right) {
		int mid = (left + right) / 2;
		double nr = pow(mid, e);
		if (nr == n) {
			return true; // numarul cautat a fost gasit
		}
		if (nr < n) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}

	return false; // nu exista un astfel de numar
}

void mapper_perfect_powers(void *arg, string file) {
	Arg<int> argument = *((Arg<int>*) arg);
	int id = argument.thread_id;
	int E = *(argument.R) + 1;
	map<int, vector<list<int>>> *Map = argument.Map;

	int nr_values, value;

	ifstream in_file(file);
	in_file >> nr_values;

	for (int i = 0; i < nr_values; i++) {
		in_file >> value;
		for (int e = 2; e <= E; e++) {
			if (is_power_of(value, e)) {
				vector<list<int>> *v = &(Map->at(id));
				(*v).at(e - 2).push_back(value);
			}
		}
	}
	in_file.close();
}

void reducer_perfect_powers(void *arg, list<void*> L) {
	Arg<int> argument = *((Arg<int>*) arg);
	int id = argument.thread_id;
	int M = *(argument.M);
	int E = (id - M) + 2;

	list<int> List = *(list<int>*) &L;

	set<int> Set(List.begin(), List.end());
	int res = Set.size(); // numarul de elemente unice

	char file_name[10];
	sprintf(file_name, "out%d.txt", E);

	ofstream out_file(file_name);
	out_file << res;

	out_file.close();
}
