#include <stdio.h>
#include <stdlib.h>
typedef struct Nodo{
	struct Nodo *sgte;
	int dato;
}Nodo;
typedef struct Pila{
	Nodo *ultimo;
}Pila;
Nodo * crear_nodo(int dato){
	Nodo *nuevo = (Nodo *) malloc(sizeof(Nodo));
	nuevo->dato = dato;
	nuevo->sgte = NULL;
	return nuevo;
}
Pila * crear_pila(){
	Pila *pila = malloc(sizeof(Pila));
	pila->ultimo = NULL;
	return pila;
}
void push(Pila * pila, Nodo * nuevo){
	if(pila->ultimo == NULL){
		pila->ultimo  = nuevo;
	}else{
		nuevo->sgte = pila->ultimo ;
		pila->ultimo  = nuevo;
	}
}
Nodo * pop(Pila **pila){
	if((*pila)->ultimo != NULL){
		Nodo *aux = (*pila)->ultimo;
		(*pila)->ultimo = aux->sgte;
		return aux;
	}
	return NULL;
}
	
void eliminar(Pila **pila, int dato){
	if((*pila)->ultimo != NULL){
		Nodo *aux = pop(pila);
		if(aux->dato == dato){
			free(aux);
			aux = NULL;
		}else{
			eliminar(pila, dato);
			push(*pila, aux);
		}
	}
}

Nodo * busqueda(Pila **pila, int dato){
	Nodo *aux2;
	if((*pila)->ultimo != NULL){
		Nodo *aux = pop(pila);
		if(aux->dato == dato){
			push(*pila, aux);
			return aux;
		}else{
			aux2 = busqueda(pila, dato);
			push(*pila, aux);
		}
		return aux2;
	}else return NULL;
}
int main(int argc, char *argv[]) {
	Pila *pila = crear_pila();
	push(pila, crear_nodo(523));
	push(pila, crear_nodo(53));
	push(pila, crear_nodo(3));
	push(pila, crear_nodo(33));
	eliminar(&pila, 523);
	Nodo *ptr = pila->ultimo;
	while(ptr != NULL){
		printf("%d\n", ptr->dato);
		ptr = ptr->sgte;
	}
	return 0;
}
 
