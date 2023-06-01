/*
 * Project for Algoritmi e Principi dell'Informatica
 * AA 2023
 * Prof. Matteo Pradella
 * Name: CercaPercorso
 * @author Stefano Morano 10737463
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node {
    int distance;
    int size;
    int capacity;
    struct Node *father;
    struct Node *dx;
    struct Node *sx;
    int *car;
} Node;

typedef Node *station;
station station_tree=NULL;

char input[19];

station in_station_position(station tree, int distance){
    if (tree==NULL || tree->distance == distance)
        return tree;
    if (distance<tree->distance)
        return in_station_position(tree->sx, distance);
    else return in_station_position(tree->dx, distance);
}
/**
 * Function that adds a station to the railroad
 * Return "aggiunta" if the station is added,
 * return "non aggiunta" if the station already exists
 */
void aggiungi_stazione() {
    int distance_input;
    scanf("%d", &distance_input);                                               //read the distance of the new station
    if (in_station_position(station_tree, distance_input)!=NULL) {
        printf("non aggiunta\n");
        return;
    } else {
        int num_input;
        station radix = station_tree, scorr;
        scanf("%d", &num_input);                                                   //read the initial number of cars in the station
        station new_station = malloc(sizeof(Node));
        new_station->distance = distance_input;
        new_station->size = num_input;
        new_station->capacity = num_input;
        new_station->dx = NULL;
        new_station->sx = NULL;
        new_station->father = NULL;
        if (station_tree == NULL) {
            station_tree = new_station;
        } else {
            while (station_tree != NULL) {
                scorr = station_tree;
                if (distance_input < station_tree->distance)
                    station_tree = station_tree->sx;
                else station_tree = station_tree->dx;
            }
            new_station->father = scorr;
            if (distance_input < scorr->distance)
                scorr->sx = new_station;
            else scorr->dx = new_station;
            station_tree = radix;
        }
        if (new_station->size != 0) {
            new_station->car = malloc(sizeof(int) * num_input);
            for (int x = 0; x < new_station->size; x++) {
                scanf("%d", &num_input);
                new_station->car[x] = num_input;                      //error: va in segmentation fault
            }
        }
        printf("aggiunta\n");
    }
}

//DEBUG
void print_stations(station tree_to_print){
    if (tree_to_print!=NULL){
        print_stations(tree_to_print->sx);
        printf("\nStazione alla distanza %d \nCon %d macchine ", tree_to_print->distance, tree_to_print->size);
        if (tree_to_print->size!=0)
            for (int x=0; x<tree_to_print->size; x++)
                printf("%d ", tree_to_print->car[x]);
        printf("\n\n");
        print_stations(tree_to_print->dx);
    }
}

int binary_search_left(station station_to_shift, int num_car) {
    int left = 0;
    int right = station_to_shift->size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (station_to_shift->car[mid] == num_car)
            return mid;
        else {
            if (station_to_shift->car[mid] < num_car)
                right = mid - 1;
            else left = mid + 1;
        }
    }
    return -1;
}

void shift_left(station station_to_shift, int position){
    for (int x=position; x<station_to_shift->size-1; x++)
        station_to_shift->car[x]=station_to_shift->car[x+1];
    station_to_shift->size--;
}

int binary_search_right(station station_to_shift, int num_car){
        int left = 0;
        int right = station_to_shift->size - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (station_to_shift->car[mid] == num_car)
                return mid;
            else {
                if (station_to_shift->car[mid] < num_car)
                    right = mid - 1;
                else left = mid + 1;
            }
        }
        return left;
}

void shift_right(){

}



/**
 * Function that remove a station in the railroad chosen in the line after the command "demolisci-stazione"
 */
void demolisci_stazione() {
    //TODO
}

/**
 * Function that add a car in a station chosen in the line after the command "aggiungi-auto"
 */
void aggiungi_auto(){
    int distance_input;
    scanf("%d", &distance_input);
    station to_consider = in_station_position(station_tree, distance_input);
    if (to_consider==NULL)
        printf("non aggiunta\n"); // return
    else {
        //control if size==0
        if (to_consider->size==to_consider->capacity) {
            to_consider->capacity *= 2;
            (to_consider->car) = (int*) realloc (to_consider->car, sizeof(int) * to_consider->capacity);
        }
        //binary_search
        //right shift
        to_consider->size++;
        printf("aggiunta");
    }
}

void rottama_auto(){
    int distance_input;
    scanf("%d", &distance_input);
    station to_consider = in_station_position(station_tree, distance_input);
    if (to_consider==NULL) {
        printf("non rottamata\n");
    } else {
        int num_input;
        scanf("%d", &num_input);
        int position=binary_search_left(to_consider, num_input);
        if (position == -1) {
            printf("non rottamata\n");
        } else {
            shift_left(to_consider, position);
            printf("rottamata\n");
        }
    }
}

void pianifica_percorso(){

}

int main() {
    while (scanf("%s", input)!=EOF){
       if (strcmp(input, "aggiungi-stazione") == 0)
           aggiungi_stazione();
       else if (strcmp(input, "distruggi-stazione") == 0)
           printf("distruggi stazione\n");
       else if (strcmp(input, "aggiungi-auto") == 0)
           aggiungi_auto();
       else if (strcmp(input, "rottama-auto") == 0)
           rottama_auto();
       else if (strcmp(input, "pianifica-percorso") == 0)
           printf("pianifica percorso\n");
       else if (strcmp(input, "stampa") == 0) {
           if (station_tree==NULL)
               printf("\nNessuna stazione presente\n");
           else print_stations(station_tree);
       }
   }
}
