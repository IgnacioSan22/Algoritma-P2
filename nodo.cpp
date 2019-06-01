#include <iostream>
#include <vector>

using namespace std;

#ifndef _NODO_
#define _NODO_


/*
 * Clase Nodo. Implementa un nodo del árbol de exploración que utiliza el
 * algoritmo de ramificación y poda para la resolución del problema planteado.
 * Se almacena el coste estimado del nodo, su número, el beneficio total hasta
 * este punto y un vector de ocupación. 
 * Este vetor tiene un tamaño igual al número de paradas del problema y cada
 * componente almacena el número de pasajeros de la correspondiente parada
 * en este punto del árbol de exploración.
 */
class Nodo {
public:
    int coste_estimado;
    int order;
    vector<int> ocupacion;
    int beneficio_actual;

    /* Constructor */
    Nodo(){
        
    }

    Nodo(int coste_estimado, int order, vector<int> ocupacion, int beneficio ){
        this->coste_estimado = coste_estimado;
        this->order = order;
        this->ocupacion = ocupacion;
        this->beneficio_actual = beneficio;
    }

    bool operator>(const Nodo& n){
        return this->coste_estimado > n.coste_estimado;
    }
};

#endif