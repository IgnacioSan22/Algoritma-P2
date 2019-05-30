#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "heap.cpp"

using namespace std;

struct pedido {
    int inicio, fin, pasajeros;
};

bool cmpBeneficio(pedido p1, pedido p2){
    int b1 = (p1.fin - p1.inicio) * p1.pasajeros;
    int b2 = (p2.fin - p2.inicio) * p2.pasajeros;
    return b1 > b2;
}

int lowerBound(){

}

bool cabePedido(){

}

int costeEstimado(){

}

int main(int argc, char *argv[]){
    
    if(argc >= 2){
        
        ifstream entrada(argv[1]);
        if(!entrada.is_open()){
            cerr << "EL fichero de entrada no se ha podido abrir.\n";
            return -1;
        }
        
        int tren, estaciones, pedidos;
        entrada >> tren >> estaciones >> pedidos;

        cout << "Datos problema: " << tren << ", " << estaciones << ", " << pedidos << endl; 
        //struct pedido lista[pedidos];
        vector<pedido> lista(pedidos);

        for(int i = 0; i < pedidos; i++){
            entrada >> lista[i].inicio >> lista[i].fin >> lista[i].pasajeros;
        }

        sort(lista.begin(), lista.end(), cmpBeneficio);
        
        int actualLB = 0;
        Heap nodos_vivos();

        int coste, lower_bound;

        Nodo nd;
    

        lower_bound = lowerBound(lista,raiz.ocupacion,0);
        coste = costeEstimado(lista, raiz.ocupacion, 0);


        while(){

        }
        
        //for(int i = 0; i < pedidos; i++){
        //    cout << "{" << (lista[i].fin - lista[i].inicio) * lista[i].pasajeros << "}, ";
        //}

        entrada.close();

    }

    return 0;
}