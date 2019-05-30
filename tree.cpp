#include <iostream>
#include <map>

using namespace std;

#ifndef _TREE_
#define _TREE_
/*
 * Clase Tree - implementa un árbol binario para el código de Huffman, de modo
 * que almacena en cada nodo un carácter y su coste_estimado .
 */
class Tree{
    string id;
    int coste_estimado;
    int LB;
    Tree* izq;
    Tree* der;

public:
    /* Constructores */

    Tree(): id(""), coste_estimado(99999){
        this->izq = nullptr;
        this->der = nullptr;
    }

    Tree(string _id, int _coste_estimado, int _LB): id(_id), coste_estimado(_coste_estimado) LB(_LB){
        this->izq = nullptr;
        this->der = nullptr;   
    }

    Tree(Tree& x, Tree& y){
        this->id = x.id + y.id;
        this->coste_estimado = x.coste_estimado + y.coste_estimado;
        if(x.coste_estimado > y.coste_estimado){
            this->izq = &y;            
            this->der = &x;            
        }else{
            this->der = &y;            
            this->izq = &x; 
        }
    }

    /* Pre: true
     * Post: devuelve un mapa de carácter - código con el código binario asociado
     *  a cada uno de los carácteres almacenados entre este nodo y sus hijos.
     */
    map<string,string> tablaHuffman(){
        map<string,string> tabla;
        string codigo = "";
        tablaINM(tabla,codigo);
        return tabla;
    }

    /* Pre: true
     * Post: asigna a 'vec' un vector en preorden de pares 
     * carácter - booleano que representa el árbol de Huffman de tal modo que
     * se asigna a los correspondientes índices del vector 0 o 1 dependiendo 
     * de si el nodo almacena un carácter o solo una suma de coste_estimados.
     */
    void preorderArray(pair<char,bool> vec[],int tam){
        int i = 0;
        preorderArrayINM(vec,tam,i);
    }

    /* Pre: true
     * Post: asigna a este nodo el árbol resultante de convertir el vector de 
     * pares catacter - booleano 'vec' en su correspondiente árbol de Huffman
     */
    void treeFromArray(pair<char,bool> vec[], int tam, int& i){
        if(i<tam){
            Tree * aux = new Tree(string(1,vec[i].first),0);
            this->izq = aux;
            if(!vec[i].second){
                i++;
                aux->treeFromArray(vec,tam,i);
            }
            i++;
            if(i<tam){
                Tree * aux2 = new Tree(string(1,vec[i].first),0);
                this->der = aux2;
                if(!vec[i].second){
                    i++;
                    aux2->treeFromArray(vec,tam,i);
                }
            }
        }
    }

    /* Pre: true
     * Post: devuelve true si la coste_estimado de este nodo es mayor a la de 'rhs'
     *  y en caso contrario devuelve falso.
     */
    bool operator > (const Tree& rhs) {
        return this->coste_estimado > rhs.coste_estimado;
    }

    /* Pre: true
     * Post: devuelve true si la coste_estimado de este nodo es menor a la de 'rhs'
     *  y en caso contrario devuelve falso.
     */
    bool operator < (const Tree& rhs) {
        return this->coste_estimado < rhs.coste_estimado;
    }

    /* Pre: true
     * Post: devuelve la coste_estimado de este nodo
     */
    int getFrec(){
        return this->coste_estimado;
    }
    
    /* Pre: true
     * Post: devuelve un puntero al hijo izquierdo de este nodo
     */
    Tree * getIzq(){
        return this->izq;
    }
    
    /* Pre: true
     * Post: devuelve un puntero al hijo derecho de este nodo
     */
    Tree * getDer(){
        return this->der;
    }

    /* Pre: true
     * Post: devuelve el id de este nodo
     */
    string getId(){
        return this->id;
    }
private:

    /* Pre: true
     * Post: asigna a 'vec' un vector en preorden de pares 
     * carácter - booleano que representa el árbol de Huffman de tal modo que
     * se asigna a los correspondientes índices del vector 0 o 1 dependiendo 
     * de si el nodo almacena un carácter o solo una suma de coste_estimados.
     */
    void preorderArrayINM(pair<char,bool> vec[], int tam, int& i){
        vec[i] = make_pair(char(this->id[0]), (this->id.length() == 1));
        if(this->izq != nullptr){
            i++;
            this->izq->preorderArrayINM(vec,tam,i);
            i++;
            this->der->preorderArrayINM(vec,tam,i);
        }
    }

    /* Pre: true
     * Post: asigna a 'tabla' el código binario asociado a cada uno de los 
     * carácteres almacenados entre este nodo y sus hijos.
     */
    void tablaINM(map<string,string>& tabla, string cod){
        if(this->izq == nullptr){
            tabla[this->id]=cod;
        }
        else{
            string aux = cod;
            aux = aux + "0";
            this->izq->tablaINM(tabla,aux);
            aux = cod;
            aux = aux +"1";
            this->der->tablaINM(tabla,aux);
        }
    }
};

#endif