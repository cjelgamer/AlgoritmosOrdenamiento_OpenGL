#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <vector>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

// ======== Estructuras ========
struct EstadoMerge {
    int tam_actual;
    int pos_actual;
    int inicio, medio, fin;
    int left, right, merge_pos;
    vector<int> buffer;
    enum Fase { BUSCAR_SIGUIENTE, COPIAR_BUFFER, MERGEANDO } fase;
};

// Declaraciones globales
extern default_random_engine rng;
extern EstadoMerge estadoMerge;   // ← debe ir DESPUÉS de definir struct EstadoMerge

struct Algoritmo {
    string nombre;
    vector<int> datos;
    int i = 0, j = 0, k = 0;
    int iteraciones = 0;
    int intercambios = 0;
    bool ordenado = false;
    int comp_a = -1;
    int comp_b = -1;
    int sel = -1;
};

// ======== Prototipos ========
bool estaOrdenado(const vector<int>& v);
void inicializarHeapSort(Algoritmo &a);
void inicializarMerge(Algoritmo &a);

// Pasos de algoritmos
void pasoBurbuja(Algoritmo &a);
void pasoSeleccion(Algoritmo &a);
void pasoBogo(Algoritmo &a);
void pasoHeapSort(Algoritmo &a);
void pasoInsercion(Algoritmo &a);
void pasoMerge(Algoritmo &a);

#endif
