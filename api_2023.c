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
    int last_visited;
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

void shift_right(station station_to_shift, int position, int to_insert){
    for (int x=station_to_shift->size; x>position; x--)
        station_to_shift->car[x]=station_to_shift->car[x-1];
    station_to_shift->car[position]=to_insert;
    station_to_shift->size++;
}

void aggiungi_stazione() {
    int distance_input;
    if (scanf("%d", &distance_input) <= 0)                                              //read the distance of the new station
        return;
    if (in_station_position(station_tree, distance_input)!=NULL) {
        printf("non aggiunta\n");
        return;
    } else {
        int num_input;
        station radix = station_tree, scorr;
        if (scanf("%d", &num_input) <= 0)                                              //read the distance of the new station
            return;                                                //read the initial number of cars in the station
        station new_station = malloc(sizeof(Node));
        new_station->distance = distance_input;
        new_station->size = 0;
        new_station->capacity = num_input;
        new_station->last_visited = 0;
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
        if (new_station->capacity != 0) {
            new_station->car = malloc(sizeof(int) * num_input);
            int position;
            for (int x = 0; x < new_station->capacity; x++) {
                if (scanf("%d", &num_input) <= 0)                                              //read the distance of the new station
                    return;
                position = binary_search_right(new_station, num_input);
                shift_right(new_station, position, num_input);
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

void delete_node(station node){
    if (node->capacity>0)
        free(node->car);
    if (node->sx==NULL && node->dx==NULL){          //if the node is a leaf
        if (node->father->sx==node)
            node->father->sx=NULL;
        else node->father->dx=NULL;
    } else {
        if (node->sx==NULL){
            if (node->father->sx==node)
                node->father->sx=node->dx;
            else node->father->dx=node->dx;
        } else {
            if (node->dx==NULL){
                if (node->father->sx==node)
                    node->father->sx=node->sx;
                else node->father->dx=node->sx;
            } else {
                station to_shift = node->dx;
                while (to_shift->sx!=NULL)
                    to_shift=to_shift->sx;
                if (to_shift->father->sx==to_shift)
                    to_shift->father->sx=NULL;
                else to_shift->father->dx=NULL;
                to_shift->father=node->father;
                to_shift->sx=node->sx;
                to_shift->dx=node->dx;
                if (node->father->sx==node)
                    node->father->sx=to_shift;
                else node->father->dx=to_shift;
            }
        }
    }
    free(node);
}

void demolisci_stazione() {
    int distance_input;
    if (scanf("%d", &distance_input) <= 0)                                              //read the distance of the new station
        return;
    station to_consider = in_station_position(station_tree, distance_input);
    if (to_consider==NULL){
        printf("non demolita\n");
    } else {
        delete_node(to_consider);
        printf("demolita\n");
    }
}

void aggiungi_auto() {
    int distance_input;
    if (scanf("%d", &distance_input) <= 0)                                              //read the distance of the new station
        return;
    station to_consider = in_station_position(station_tree, distance_input);
    if (to_consider == NULL)
        printf("non aggiunta\n");
    else {
        if (to_consider->capacity == 0) {
            to_consider->car = malloc(sizeof(int));
            to_consider->capacity = 1;
            to_consider->size = 1;
            if (scanf("%d", &to_consider->car[0]) <= 0)                                              //read the distance of the new station
                return;
            printf("aggiunta\n");
        } else {
            if (to_consider->size == to_consider->capacity) {
                to_consider->capacity *= 2;
                (to_consider->car) = (int *) realloc (to_consider->car, sizeof(int) * to_consider->capacity);
            }
            int position = binary_search_right(to_consider, distance_input), num_input;
            if (scanf("%d", &num_input) <= 0)                                              //read the distance of the new station
                return;
            shift_right(to_consider, position, num_input);
            printf("aggiunta\n");
        }
    }
}

void rottama_auto(){
    int distance_input;
    if (scanf("%d", &distance_input) <= 0)                                              //read the distance of the new station
        return;
    station to_consider = in_station_position(station_tree, distance_input);
    if (to_consider==NULL) {
        printf("non rottamata\n");
    } else {
        if (to_consider->size == 0 || to_consider->capacity == 0)
            printf("non rottamata\n");
        else {
            int num_input;
            if (scanf("%d", &num_input) <= 0)                                              //read the distance of the new station
                return;
            int position = binary_search_left(to_consider, num_input);
            if (position == -1) {
                printf("non rottamata\n");
            } else {
                shift_left(to_consider, position);
                printf("rottamata\n");
            }
        }
    }
}

station first_station(station to_search){
    while (to_search->sx!=NULL)
        to_search=to_search->sx;
    return to_search;
}

station next_station(station to_next){
    if (to_next->dx!=NULL)
        return first_station(to_next->dx);
    station temp=to_next->father;
    while (temp!=NULL && to_next == temp->dx){
        to_next=temp;
        temp=temp->father;
    }
    return temp;
}

void afterward(station station_1, station station_2){
    station temp_station = next_station(station_1), first_station=station_1;
    while (first_station->distance != station_2->distance){
        while ((first_station->car[0] + first_station->distance) >= temp_station->distance) {
            if (temp_station->last_visited == 0)
                temp_station->last_visited = first_station->distance;
            temp_station= next_station(temp_station);
        }
        first_station= next_station(first_station);
    }
    if (station_2->distance != 0){
        temp_station=station_2;
        printf("%d ", station_2->last_visited);
        do {
            temp_station = in_station_position(station_tree, temp_station->last_visited);
            printf("%d ", temp_station->last_visited);
        } while (temp_station->distance != station_1 -> distance);
        printf("\n");
    } else printf("nessun percorso\n");
}

void reset_distance(station tree_to_reset){
    if (tree_to_reset!=NULL) {
        reset_distance(tree_to_reset->sx);
        tree_to_reset->last_visited = 0;
        reset_distance(tree_to_reset->dx);
    }
}


void pianifica_percorso(){
int distance_1, distance_2;
    if (scanf("%d", &distance_1) <= 0)                                              //read the distance of the new station
        return;
    if (scanf("%d", &distance_2) <= 0)                                              //read the distance of the new station
        return;
    if (distance_1 < distance_2) {
        afterward(in_station_position(station_tree, distance_1), in_station_position(station_tree, distance_2));
        return;
    } else if (distance_1 > distance_2)
        return; /*backwards*/
    printf("%d\n", distance_1);

}

int main() {
    //FILE *file=freopen("output.txt", "w", stdout);
    while (scanf("%s", input)!=EOF){
       if (strcmp(input, "aggiungi-stazione") == 0)
           aggiungi_stazione();
       else if (strcmp(input, "demolisci-stazione") == 0)
              demolisci_stazione();
       else if (strcmp(input, "aggiungi-auto") == 0)
           aggiungi_auto();
       else if (strcmp(input, "rottama-auto") == 0)
           rottama_auto();
       else if (strcmp(input, "pianifica-percorso") == 0)
           pianifica_percorso();
       else if (strcmp(input, "stampa") == 0) {
           if (station_tree==NULL)
               printf("\nNessuna stazione presente\n");
           else print_stations(station_tree);
       }
   }
   //fclose(file);
}
