#include <iostream>
#include "nodo.cpp"
using namespace std;

#ifndef _HEAP_
#define _HEAP_


#define MAX 512

/* Clase Heap - implementa un montículo binario de forma estática como un
 * vector de árboles de tamaño 256. El montículo se ordena de forma eficiente
 * en función de la frecuencia de sus árboles almacenados, de tal modo que la
 * frecuencia más baja siempre será su primer elemento, y cada nodo siempre 
 * tendrá una frecuencia más baja que cualquiera de sus hijos.
 * 
 * La implementación estática se basa en almacenar los árboles de tal modo que
 * índice padre = p
 * índice hijo izquierdo = 2*p
 * índice hijo derecho = 2*p + 1
 */
class Heap{
    Nodo heap[MAX];
    int numElem;
public:

    /* Constructor */
    Heap(): numElem(0){
        
    }

    bool esVacia(){
        return this->numElem == 0;
    }

    /* Pre: true
       Post: Devuelve el primer elemento */
    Nodo cima(){
        return this->heap[0];
    }

    /* Pre: numElem < MAX
     * Post: Añade 'nuevo' al montículo y lo reordena
     */
    void push(Nodo nuevo){
        if(this->numElem == MAX){
            cout << "No ha sido posible incorporar el elemnto\n";
        }
        else{
            this->heap[this->numElem]  = nuevo;
            bool debeSubir;
            int i = this->numElem;
            this->numElem++;
            if(i > 0){
                debeSubir = this->heap[i] > this->heap[i/2];
            }else{ debeSubir = false; }
            while(debeSubir){
                Nodo aux = this->heap[i];
                this->heap[i] = this->heap[i/2];
                this->heap[i/2] = aux;
                i = i / 2;
                if (i > 0){
                    debeSubir = this->heap[i] > this->heap[i/2];
                }else{ debeSubir = false; }
            }
        }
    }

    /* Pre: numElem > 0 
     * Post: elimina el primer elemento del montículo y lo reordena
     */
    void pop(){
        int i, j;
        if((this -> numElem)>0){
            this -> numElem--;
            this->heap[0] = this->heap[this->numElem];
            i = 0;
            while(i<(this->numElem/2)){
                if( (2*i) == this->numElem || this->heap[(2*i)] > this->heap[(2*i+1)]){
                    j = 2*i;
                }else{
                    j = 2*i+1;
                }
                if(this->heap[j] > this->heap[i]){
                    Nodo aux = this->heap[i];
                    this->heap[i] = this->heap[j];
                    this->heap[j] = aux;
                    i = j;
                }else{
                    i = this->numElem;
                }
            }
        }

    }

};

#endif