#include "map_reduce.h"


/* Intoarce true daca exista un numar care, ridicat la puterea e, este egal cu n;
altfel, false */
bool is_power_of(int n, int e);

/* Realizeaza operatiile de mapper pentru problema aflarii numerelor
de tip putere perfecta */
void mapper_perfect_powers(void *arg, string file);

/* Realizeaza operatiile de reducer pentru problema aflarii numerelor
de tip putere perfecta */
void reducer_perfect_powers(void *arg, list<void*> L);
