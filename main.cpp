#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <sstream>
#include <string>
#include "algoritmos.h"

using namespace std;

// Variables globales
Algoritmo burbuja, seleccion, bogo, heap, insercion, mergesort;
vector<int> valoresBase;
int n = 100;
int delay_ms = 15;
bool iniciar = false;
bool pausa = false;
bool datosIngresados = false;
string inputTexto = "";
bool recibiendoInput = false;

// ----------------- funciones de dibujo -----------------
void dibujarTexto(float x, float y, const string &texto, void* font = GLUT_BITMAP_HELVETICA_12) {
    glRasterPos2f(x, y);
    for (char c : texto) glutBitmapCharacter(font, c);
}

void dibujarBoton(float x, float y, float ancho, float alto, const string &texto, 
                  float r, float g, float b, bool activo = true) {
    if (activo) {
        glColor3f(r, g, b);
    } else {
        glColor3f(0.3f, 0.3f, 0.3f);
    }
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + ancho, y);
        glVertex2f(x + ancho, y + alto);
        glVertex2f(x, y + alto);
    glEnd();
    
    glColor3f(1, 1, 1);
    float textX = x + (ancho - texto.length() * 0.012f) / 2.0f;
    float textY = y + (alto - 0.02f) / 2.0f;
    dibujarTexto(textX, textY, texto);
}

void dibujarTitulo() {
    glColor3f(0.9f, 0.9f, 0.95f);
    dibujarTexto(-0.98f, 0.96f, "VISUALIZADOR DE ALGORITMOS DE ORDENAMIENTO", GLUT_BITMAP_HELVETICA_18);
    
    string estado = iniciar ? (pausa ? "PAUSADO" : "EJECUTANDO") : "LISTO";
    glColor3f(1.0f, 1.0f, 0.5f);
    dibujarTexto(0.7f, 0.96f, "Estado: " + estado, GLUT_BITMAP_HELVETICA_18);
}

void dibujarPanelControl() {
    glColor3f(0.08f, 0.08f, 0.12f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, 0.84f);
        glVertex2f(1.0f, 0.84f);
        glVertex2f(1.0f, 0.98f);
        glVertex2f(-1.0f, 0.98f);
    glEnd();
    
    glColor3f(0.3f, 0.3f, 0.5f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-1.0f, 0.84f);
        glVertex2f(1.0f, 0.84f);
        glVertex2f(1.0f, 0.98f);
        glVertex2f(-1.0f, 0.98f);
    glEnd();
    
    glColor3f(0.1f, 0.1f, 0.15f);
    glBegin(GL_QUADS);
        glVertex2f(-0.8f, 0.86f);
        glVertex2f(-0.1f, 0.86f);
        glVertex2f(-0.1f, 0.92f);
        glVertex2f(-0.8f, 0.92f);
    glEnd();
    
    glColor3f(0.5f, 0.5f, 0.6f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.8f, 0.86f);
        glVertex2f(-0.1f, 0.86f);
        glVertex2f(-0.1f, 0.92f);
        glVertex2f(-0.8f, 0.92f);
    glEnd();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    string textoMostrar = "Datos: " + inputTexto;
    if (recibiendoInput) textoMostrar += "_";
    dibujarTexto(-0.79f, 0.89f, textoMostrar);
    
    dibujarBoton(0.1f, 0.86f, 0.12f, 0.06f, "ACEPTAR", 0.0f, 0.6f, 0.0f, !iniciar);
    dibujarBoton(0.23f, 0.86f, 0.12f, 0.06f, "RANDOM", 0.0f, 0.4f, 0.8f, !iniciar);
    dibujarBoton(0.36f, 0.86f, 0.12f, 0.06f, "LIMPIAR", 0.8f, 0.2f, 0.0f, !iniciar);
    
    if (datosIngresados && !iniciar) {
        dibujarBoton(0.6f, 0.86f, 0.12f, 0.06f, "INICIAR", 0.0f, 0.7f, 0.0f);
    } else if (iniciar) {
        dibujarBoton(0.6f, 0.86f, 0.12f, 0.06f, pausa ? "CONTINUAR" : "PAUSA", 1.0f, 0.5f, 0.0f);
        dibujarBoton(0.73f, 0.86f, 0.12f, 0.06f, "REINICIAR", 0.8f, 0.1f, 0.1f);
    } else {
        dibujarBoton(0.6f, 0.86f, 0.12f, 0.06f, "INICIAR", 0.3f, 0.3f, 0.3f, false);
    }
    
    if (datosIngresados) {
        string infoDatos = "Elementos: " + to_string(n);
        glColor3f(0.7f, 0.7f, 1.0f);
        dibujarTexto(-0.98f, 0.89f, infoDatos);
    }
}

void dibujarLeyenda() {
    float startX = -0.98f;
    float startY = -0.94f;
    
    glColor3f(1.0f, 1.0f, 1.0f);
    dibujarTexto(startX, startY, "LEYENDA: ");

    glColor3f(0.3f, 0.5f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(startX + 0.12f, startY - 0.005f);
        glVertex2f(startX + 0.15f, startY - 0.005f);
        glVertex2f(startX + 0.15f, startY + 0.015f);
        glVertex2f(startX + 0.12f, startY + 0.015f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    dibujarTexto(startX + 0.16f, startY, "Normal");

    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(startX + 0.25f, startY - 0.005f);
        glVertex2f(startX + 0.28f, startY - 0.005f);
        glVertex2f(startX + 0.28f, startY + 0.015f);
        glVertex2f(startX + 0.25f, startY + 0.015f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    dibujarTexto(startX + 0.29f, startY, "Comparando");

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(startX + 0.45f, startY - 0.005f);
        glVertex2f(startX + 0.48f, startY - 0.005f);
        glVertex2f(startX + 0.48f, startY + 0.015f);
        glVertex2f(startX + 0.45f, startY + 0.015f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    dibujarTexto(startX + 0.49f, startY, "Seleccionado");

    glColor3f(0.2f, 1.0f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(startX + 0.65f, startY - 0.005f);
        glVertex2f(startX + 0.68f, startY - 0.005f);
        glVertex2f(startX + 0.68f, startY + 0.015f);
        glVertex2f(startX + 0.65f, startY + 0.015f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    dibujarTexto(startX + 0.69f, startY, "Ordenado");
}

void dibujarBarras(Algoritmo &a, float xIni, float xFin, float yIni, float yFin, float r, float g, float b) {
    if (a.datos.empty()) return;
    
    if (a.comp_a >= (int)a.datos.size()) a.comp_a = -1;
    if (a.comp_b >= (int)a.datos.size()) a.comp_b = -1;
    if (a.sel >= (int)a.datos.size()) a.sel = -1;
    
    int maxVal = *max_element(a.datos.begin(), a.datos.end());
    if (maxVal == 0) maxVal = 1;

    int elementos = (int)a.datos.size();
    
    float widthZone = xFin - xIni;
    float heightZone = yFin - yIni;
    
    float gap = max(0.002f, widthZone * 0.005f);
    float anchoBarra = max(0.001f, (widthZone / elementos) - gap);
    
    if (gap > anchoBarra * 0.8f) {
        gap = anchoBarra * 0.3f;
        anchoBarra = (widthZone - (gap * (elementos - 1))) / elementos;
    }

    glColor3f(0.1f, 0.1f, 0.15f);
    glBegin(GL_QUADS);
        glVertex2f(xIni, yIni);
        glVertex2f(xFin, yIni);
        glVertex2f(xFin, yFin);
        glVertex2f(xIni, yFin);
    glEnd();

    for (int k = 0; k < elementos; ++k) {
        float x = xIni + k * (anchoBarra + gap);
        float altura = (float)a.datos[k] / maxVal * (heightZone * 0.75f);
        float baseY = yIni + (heightZone * 0.1f);

        if (a.ordenado) {
            glColor3f(0.2f, 1.0f, 0.3f); 
        } else if (k == a.comp_a || k == a.comp_b) {
            glColor3f(0.0f, 1.0f, 1.0f);
        } else if (k == a.sel) {
            glColor3f(1.0f, 0.0f, 0.0f); 
        } else {
            glColor3f(r, g, b); 
        }

        glBegin(GL_QUADS);
            glVertex2f(x, baseY);
            glVertex2f(x + anchoBarra, baseY);
            glVertex2f(x + anchoBarra, baseY + altura);
            glVertex2f(x, baseY + altura);
        glEnd();
        
        if (k < elementos - 1) {
            glColor3f(0.0f, 0.0f, 0.0f); // Negro
            glBegin(GL_LINES);
                glVertex2f(x + anchoBarra + gap/2, baseY);
                glVertex2f(x + anchoBarra + gap/2, baseY + altura);
            glEnd();
        }
    }

    glColor3f(1, 1, 1);
    string estado = a.ordenado ? "TERMINADO" : "ORDENANDO";
    string info = a.nombre + " [" + estado + "]";
    dibujarTexto(xIni + 0.01f, yFin - 0.04f, info);
    
    string stats = "Iter: " + to_string(a.iteraciones) + " | Swap: " + to_string(a.intercambios);
    dibujarTexto(xIni + 0.01f, yFin - 0.07f, stats);
}

void reiniciarAlgoritmos() {
    if (valoresBase.empty()) return;
    
    n = (int)valoresBase.size();
    if (n <= 0) return;
    
    burbuja = {"Burbuja", valoresBase};
    seleccion = {"Seleccion", valoresBase};
    bogo = {"BogoSort", valoresBase};
    heap = {"HeapSort", valoresBase};
    insercion = {"Insercion", valoresBase};
    mergesort = {"MergeSort", valoresBase};
    
    inicializarHeapSort(heap);
    inicializarMerge(mergesort);
    
    datosIngresados = true;
    iniciar = false;
    pausa = false;
}

void generarDatosAleatorios() {
    valoresBase.clear();
    int cantidad = 100;
    valoresBase.reserve(cantidad);
    
    for (int i = 0; i < cantidad; ++i) {
        valoresBase.push_back(10 + (rng() % 490));
    }
    
    inputTexto = to_string(cantidad) + " valores aleatorios";
    reiniciarAlgoritmos();
}

void procesarInput() {
    if (inputTexto.empty()) return;
    
    valoresBase.clear();
    stringstream ss(inputTexto);
    int valor;
    
    while (ss >> valor) {
        if (valor > 0 && valor <= 1000) {
            valoresBase.push_back(valor);
        }
    }
    
    if (valoresBase.size() > 200) {
        valoresBase.resize(200);
    }
    
    if (!valoresBase.empty()) {
        reiniciarAlgoritmos();
    } else {
        datosIngresados = false;
    }
}

// ----------------- eventos -----------------
void teclado(unsigned char key, int x, int y) {
    if (recibiendoInput) {
        if (key == 13 || key == 27) {
            recibiendoInput = false;
            if (key == 13) {
                procesarInput();
            }
        } else if (key == 8) {
            if (!inputTexto.empty()) {
                inputTexto.pop_back();
            }
        } else if (key >= 32 && key <= 126) {
            if (inputTexto.length() < 40) {
                inputTexto += key;
            }
        }
        glutPostRedisplay();
    } else {
        if (key == ' ') {
            if (iniciar) {
                pausa = !pausa;
            }
        } else if (key == 'r' || key == 'R') {
            reiniciarAlgoritmos();
        }
        glutPostRedisplay();
    }
}

void clickRaton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        GLint vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
        float xN = (float)x / vp[2] * 2.0f - 1.0f;
        float yN = 1.0f - (float)y / vp[3] * 2.0f;
        
        if (yN > 0.84f && yN < 0.98f) {
            if (xN > 0.1f && xN < 0.22f && !iniciar) {
                recibiendoInput = true;
                if (inputTexto.empty()) inputTexto = " ";
            }
            else if (xN > 0.23f && xN < 0.35f && !iniciar) {
                generarDatosAleatorios();
            }
            else if (xN > 0.36f && xN < 0.48f && !iniciar) {
                inputTexto = "";
                valoresBase.clear();
                datosIngresados = false;
                iniciar = false;
            }
            else if (xN > 0.6f && xN < 0.72f) {
                if (datosIngresados) {
                    if (!iniciar) {
                        iniciar = true;
                        pausa = false;
                    } else {
                        pausa = !pausa;
                    }
                }
            }
            else if (xN > 0.73f && xN < 0.85f) {
                reiniciarAlgoritmos();
            }
        }
        glutPostRedisplay();
    }
}

// ----------------- display -----------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    dibujarTitulo();
    dibujarPanelControl();
    
    if (!datosIngresados) {
        glColor3f(1.0f, 0.8f, 0.2f);
        dibujarTexto(-0.5f, 0.5f, "INGRESE DATOS USANDO LOS BOTONES SUPERIORES", GLUT_BITMAP_HELVETICA_18);
        dibujarTexto(-0.3f, 0.4f, "O PRESIONE 'RANDOM' PARA DATOS ALEATORIOS", GLUT_BITMAP_HELVETICA_18);
        glutSwapBuffers();
        return;
    }

    // Distribución de algoritmos en 2 filas y 3 columnas
    float marginX = 0.02f;
    float marginY = 0.02f;
    float panelHeight = 0.14f;
    float availableHeight = 1.88f - panelHeight;
    float rowHeight = availableHeight / 2.0f;
    float colWidth = 2.0f / 3.0f;
    
    // Primera fila - Algoritmos eficientes
    float y1 = 0.82f - rowHeight;
    float y2 = 0.82f - marginY;
    
    dibujarBarras(heap, -1.0f + marginX, -1.0f + colWidth - marginX, y1, y2, 0.2f, 0.8f, 0.3f);
    dibujarBarras(mergesort, -1.0f + colWidth + marginX, -1.0f + 2*colWidth - marginX, y1, y2, 0.8f, 0.3f, 0.8f);
    dibujarBarras(insercion, -1.0f + 2*colWidth + marginX, 1.0f - marginX, y1, y2, 1.0f, 0.6f, 0.2f);
    
    // Segunda fila - Algoritmos básicos
    float y3 = y1 - rowHeight;
    float y4 = y1 - marginY;
    
    dibujarBarras(burbuja, -1.0f + marginX, -1.0f + colWidth - marginX, y3, y4, 0.3f, 0.5f, 1.0f);
    dibujarBarras(seleccion, -1.0f + colWidth + marginX, -1.0f + 2*colWidth - marginX, y3, y4, 1.0f, 0.3f, 0.4f);
    dibujarBarras(bogo, -1.0f + 2*colWidth + marginX, 1.0f - marginX, y3, y4, 1.0f, 0.2f, 0.8f);
    
    dibujarLeyenda();
    glutSwapBuffers();
}

// ----------------- animación -----------------
void animar(int) {
    if (iniciar && datosIngresados && !pausa) {
        if (!burbuja.datos.empty()) pasoBurbuja(burbuja);
        if (!seleccion.datos.empty()) pasoSeleccion(seleccion);
        if (!bogo.datos.empty()) pasoBogo(bogo);
        if (!heap.datos.empty()) pasoHeapSort(heap);
        if (!insercion.datos.empty()) pasoInsercion(insercion);
        if (!mergesort.datos.empty()) pasoMerge(mergesort);
    }
    glutPostRedisplay();
    glutTimerFunc(delay_ms, animar, 0);
}

// ----------------- main -----------------
int main(int argc, char **argv) {
    cout << "=== VISUALIZADOR DE ALGORITMOS DE ORDENAMIENTO ===" << endl;
    cout << "Instrucciones:" << endl;
    cout << "1. Use los botones superiores para ingresar datos" << endl;
    cout << "2. 'ACEPTAR': Ingresar datos manualmente (ej: 5 3 8 1 9)" << endl;
    cout << "3. 'RANDOM': Generar datos aleatorios" << endl;
    cout << "4. 'INICIAR': Comenzar visualizacion" << endl;
    cout << "5. 'PAUSA/CONTINUAR': Pausar o reanudar" << endl;
    cout << "6. 'REINICIAR': Volver al estado inicial" << endl;
    cout << "Controles de teclado: ESPACIO (pausa), R (reiniciar)" << endl;
    
    cout << "\nAlgoritmos implementados:" << endl;
    cout << "  HeapSort:   O(n log n) - Ordenamiento por monticulos" << endl;
    cout << "  MergeSort:  O(n log n) - Ordenamiento por mezcla" << endl;
    cout << "  Insercion:  O(n²) - Eficiente para datos pequenos" << endl;
    cout << "  Burbuja:    O(n²) - Comparaciones e intercambios" << endl;
    cout << "  Seleccion:  O(n²) - Busca el minimo en cada iteracion" << endl;
    cout << "  BogoSort:   O(n×n!) - Aleatorio puro (muy lento)" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1600, 900);
    glutCreateWindow("Visualizador Avanzado de Algoritmos de Ordenamiento");
    glClearColor(0.06f, 0.07f, 0.09f, 1.0f);
    glutDisplayFunc(display);
    glutMouseFunc(clickRaton);
    glutKeyboardFunc(teclado);
    glutTimerFunc(delay_ms, animar, 0);
    glutMainLoop();
    return 0;
}