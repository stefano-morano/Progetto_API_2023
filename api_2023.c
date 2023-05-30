/*
 * Project for Algoritmi e Principi dell'Informatica
 * AA 2023
 * Prof. Matteo Pradella
 * Name: CercaPercorso
 * @author Stefano Morano 10737463
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Function that adds a station to the railroad
 * Return "aggiunta" if the station is added,
 * return "non aggiunta" if the station already exists
 */
void aggiungi_stazione() {
    //TODO
}


/**
 * Function that removes a station from the railroad
 * @param distance is the id of the station to be removed
 */
void demolisci_stazione(int distance) {
    //TODO
}

void aggiungi_auto(){
    //TODO
}

void rottama_auto(){
    //TODO
}

void pianifica_percorso(){

}

int main() {
   //sum of the first five natural numbers
    int sum = 0, input;
    for (int i = 1; i <= 5; i++) {
        scanf("%d", &input);
        sum += input;
    }
    printf("The sum is: %d", sum);
    aggiungi_stazione();
}
