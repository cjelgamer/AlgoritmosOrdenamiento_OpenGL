#include "algoritmos.h"
#include <ctime>

default_random_engine rng((unsigned)time(0));  // 🔹 definición real del rng
EstadoMerge estadoMerge;                      // 🔹 definición real del estado de merge

bool estaOrdenado(const vector<int>& v) {
    for (int i = 0; i < (int)v.size() - 1; ++i)
        if (v[i] > v[i + 1]) return false;
    return true;
}

void inicializarHeapSort(Algoritmo &a) {
    a.i = 1;
    a.j = 1;
    a.k = (int)a.datos.size() - 1;
}

void inicializarMerge(Algoritmo &a) {
    // Inicializa el estado global usado por pasoMerge
    estadoMerge.tam_actual = 1;
    estadoMerge.pos_actual = 0;
    estadoMerge.inicio = 0;
    estadoMerge.medio = 0;
    estadoMerge.fin = 0;
    estadoMerge.left = 0;
    estadoMerge.right = 0;
    estadoMerge.merge_pos = 0;
    estadoMerge.buffer.clear();
    estadoMerge.fase = EstadoMerge::BUSCAR_SIGUIENTE;

    // Asegurar que el algoritmo también esté en estado inicial
    a.i = a.j = a.k = 0;
    a.iteraciones = 0;
    a.intercambios = 0;
    a.ordenado = false;
    a.comp_a = a.comp_b = a.sel = -1;
}



// ----------------- burbuja -----------------
void pasoBurbuja(Algoritmo &a) {
    int n = (int)a.datos.size();
    a.comp_a = a.comp_b = a.sel = -1;
    if (a.ordenado) return;
    a.iteraciones++;
    
    if (a.i < n - 1) {
        if (a.j < n - a.i - 1) {
            a.comp_a = a.j;
            a.comp_b = a.j + 1;
            if (a.datos[a.j] > a.datos[a.j + 1]) {
                swap(a.datos[a.j], a.datos[a.j + 1]);
                a.intercambios++;
                a.sel = a.j + 1;
            }
            a.j++;
        } else { a.j = 0; a.i++; }
    } else a.ordenado = true;
}

// ----------------- selección -----------------
void pasoSeleccion(Algoritmo &a) {
    int n = (int)a.datos.size();
    a.comp_a = a.comp_b = -1;
    if (a.ordenado) return;
    a.iteraciones++;
    
    if (a.i >= n - 1) { a.ordenado = true; return; }

    if (a.j == 0) { a.sel = a.i; a.j = a.i + 1; }

    if (a.j < n) {
        a.comp_a = a.j;
        a.comp_b = a.sel;
        if (a.datos[a.j] < a.datos[a.sel]) a.sel = a.j;
        a.j++;
    } else {
        if (a.sel != a.i) {
            swap(a.datos[a.i], a.datos[a.sel]);
            a.intercambios++;
        }
        a.j = 0; a.i++;
    }
}

// ----------------- bogosort -----------------
void pasoBogo(Algoritmo &a) {
    a.comp_a = a.comp_b = a.sel = -1;
    if (a.ordenado) return;
    a.iteraciones++;
    
    shuffle(a.datos.begin(), a.datos.end(), rng);
    if (estaOrdenado(a.datos)) a.ordenado = true;
}

// ----------------- heapsort -----------------
void pasoHeapSort(Algoritmo &a) {
    int n = (int)a.datos.size();
    a.comp_a = a.comp_b = a.sel = -1;
    if (a.ordenado) return;
    
    // Fase de construcción del heap
    if (a.i < n) {
        if (a.j > 0) {
            int padre = (a.j - 1) / 2;
            a.comp_a = a.j;
            a.comp_b = padre;
            
            if (a.datos[a.j] > a.datos[padre]) {
                swap(a.datos[a.j], a.datos[padre]);
                a.intercambios++;
                a.j = padre;
            } else {
                a.i++;
                a.j = a.i;
            }
        } else {
            a.i++;
            a.j = a.i;
        }
        a.iteraciones++;
        return;
    }
    
    // Fase de extracción
    if (a.k > 0) {
        if (a.j == -1) {
            swap(a.datos[0], a.datos[a.k]);
            a.intercambios++;
            a.j = 0;
            a.k--;
            a.iteraciones++;
            return;
        }
        
        int mayor = a.j;
        int izquierdo = 2 * a.j + 1;
        int derecho = 2 * a.j + 2;
        
        a.comp_a = a.j;
        a.sel = a.j;
        
        if (izquierdo <= a.k && a.datos[izquierdo] > a.datos[mayor]) {
            mayor = izquierdo;
        }
        if (derecho <= a.k && a.datos[derecho] > a.datos[mayor]) {
            mayor = derecho;
        }
        
        if (mayor != a.j) {
            swap(a.datos[a.j], a.datos[mayor]);
            a.intercambios++;
            a.j = mayor;
        } else {
            a.j = -1;
        }
        a.iteraciones++;
    } else {
        a.ordenado = true;
    }
}

// ----------------- inserción -----------------
void pasoInsercion(Algoritmo &a) {
    int n = (int)a.datos.size();
    a.comp_a = a.comp_b = a.sel = -1;
    if (a.ordenado) return;
    a.iteraciones++;
    
    if (a.i == 0) { a.i = 1; a.j = 1; }
    if (a.i < n) {
        if (a.j > 0) {
            a.comp_a = a.j; a.comp_b = a.j - 1;
            if (a.datos[a.j - 1] > a.datos[a.j]) {
                swap(a.datos[a.j], a.datos[a.j - 1]);
                a.intercambios++;
                a.j--;
            } else { a.i++; a.j = a.i; }
        } else { a.i++; a.j = a.i; }
    } else a.ordenado = true;
}


void pasoMerge(Algoritmo &a) {
    int n = (int)a.datos.size();
    a.comp_a = a.comp_b = a.sel = -1;
    if (a.ordenado) return;
    
    if (n <= 1) {
        a.ordenado = true;
        return;
    }
    
    a.iteraciones++;
    
    switch (estadoMerge.fase) {
        case EstadoMerge::BUSCAR_SIGUIENTE:
            if (estadoMerge.pos_actual >= n) {
                estadoMerge.tam_actual *= 2;
                estadoMerge.pos_actual = 0;
                
                if (estadoMerge.tam_actual >= n) {
                    a.ordenado = true;
                    return;
                }
            }
            
            estadoMerge.inicio = estadoMerge.pos_actual;
            estadoMerge.medio = min(estadoMerge.inicio + estadoMerge.tam_actual - 1, n - 1);
            estadoMerge.fin = min(estadoMerge.inicio + 2 * estadoMerge.tam_actual - 1, n - 1);
            
            if (estadoMerge.medio >= estadoMerge.fin) {
                estadoMerge.pos_actual += 2 * estadoMerge.tam_actual;
                return;
            }
            
            estadoMerge.buffer.clear();
            estadoMerge.buffer.resize(estadoMerge.fin - estadoMerge.inicio + 1);
            estadoMerge.fase = EstadoMerge::COPIAR_BUFFER;
            estadoMerge.left = estadoMerge.inicio;
            return;
            
        case EstadoMerge::COPIAR_BUFFER:
            if (estadoMerge.left <= estadoMerge.fin) {
                estadoMerge.buffer[estadoMerge.left - estadoMerge.inicio] = a.datos[estadoMerge.left];
                estadoMerge.left++;
                return;
            }
            
            estadoMerge.left = 0;
            estadoMerge.right = estadoMerge.medio - estadoMerge.inicio + 1;
            estadoMerge.merge_pos = estadoMerge.inicio;
            estadoMerge.fase = EstadoMerge::MERGEANDO;
            return;
            
        case EstadoMerge::MERGEANDO:
            {
                int leftEnd = estadoMerge.medio - estadoMerge.inicio;
                int rightEnd = estadoMerge.fin - estadoMerge.inicio;
                
                if (estadoMerge.left <= leftEnd && estadoMerge.right <= rightEnd) {
                    a.comp_a = estadoMerge.inicio + estadoMerge.left;
                    a.comp_b = estadoMerge.inicio + estadoMerge.right;
                    
                    if (estadoMerge.buffer[estadoMerge.left] <= estadoMerge.buffer[estadoMerge.right]) {
                        a.datos[estadoMerge.merge_pos] = estadoMerge.buffer[estadoMerge.left];
                        estadoMerge.left++;
                    } else {
                        a.datos[estadoMerge.merge_pos] = estadoMerge.buffer[estadoMerge.right];
                        estadoMerge.right++;
                    }
                    a.intercambios++;
                    estadoMerge.merge_pos++;
                    return;
                    
                } else if (estadoMerge.left <= leftEnd) {
                    a.datos[estadoMerge.merge_pos] = estadoMerge.buffer[estadoMerge.left];
                    estadoMerge.left++;
                    estadoMerge.merge_pos++;
                    a.intercambios++;
                    return;
                    
                } else if (estadoMerge.right <= rightEnd) {
                    a.datos[estadoMerge.merge_pos] = estadoMerge.buffer[estadoMerge.right];
                    estadoMerge.right++;
                    estadoMerge.merge_pos++;
                    a.intercambios++;
                    return;
                    
                } else {
                    estadoMerge.pos_actual += 2 * estadoMerge.tam_actual;
                    estadoMerge.fase = EstadoMerge::BUSCAR_SIGUIENTE;
                    return;
                }
            }
            break;
    }
}