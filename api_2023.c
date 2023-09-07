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
int *array_distance;
int array_capacity=0, array_size=0;

station in_station_position(station tree, int distance){
    if (tree==NULL || tree->distance == distance)
        return tree;
    if (distance<tree->distance)
        return in_station_position(tree->sx, distance);
    else return in_station_position(tree->dx, distance);
}

station first_station(station to_search){
    while (to_search->sx!=NULL)
        to_search=to_search->sx;
    return to_search;
}

station last_station(station to_search){
    while (to_search->dx!=NULL)
        to_search=to_search->dx;
    return to_search;
}

station previous_station(station to_previous){
    if (to_previous->sx!=NULL)
        return last_station(to_previous->sx);
    station temp=to_previous->father;
    while (temp!=NULL && to_previous == temp->sx){
        to_previous=temp;
        temp=temp->father;
    }
    return temp;
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

void reset_array(){
    if (array_capacity!=0){
        for (int x=0; x<array_size; x++)
            array_distance[x]=0;
    }
    array_size=0;
}

void insert_array(int distance){
    if (array_capacity==0){
        array_distance =(int*) malloc (sizeof(int));
        array_capacity=1;
    } else if (array_size==array_capacity){
        array_capacity*=2;
        array_distance = (int*) realloc (array_distance, sizeof(int)*array_capacity);
    }
    array_distance[array_size]=distance;
    array_size++;
}

int binary_search_right(station station_to_shift, int num_car){
    int left = 0, right = station_to_shift->size - 1, mid;
    while (left <= right) {
        mid = (left + right) / 2;
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
    if (in_station_position(station_tree, distance_input)!=NULL) {          //doesn't insert if the station already exists
        printf("non aggiunta\n");
        return;
    } else {
        int num_input;
        station radix = station_tree, scorr;
        if (scanf("%d", &num_input) <= 0)                                              //read the distance of the new station
            return;                                                                    //read the initial number of cars in the station
        station new_station = malloc(sizeof(Node));
        new_station->distance = distance_input;
        new_station->size = 0;
        new_station->capacity = num_input;
        new_station->last_visited = -1;
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
            new_station->car = (int*) malloc (sizeof(int) * num_input);
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
        printf("\nUltima stazione visitata: %d\n\n", tree_to_print->last_visited);
        print_stations(tree_to_print->dx);
    }
}

//DEBUG
void print_station(int distance){
    station tree_to_print=in_station_position(station_tree, distance);
    if (tree_to_print!=NULL){
        print_stations(tree_to_print->sx);
        printf("\nStazione alla distanza %d \nCon %d macchine ", tree_to_print->distance, tree_to_print->size);
        if (tree_to_print->size!=0)
            for (int x=0; x<tree_to_print->size; x++)
                printf("%d ", tree_to_print->car[x]);
        printf("\nUltima stazione visitata: %d\n\n", tree_to_print->last_visited);
        print_stations(tree_to_print->dx);
    }
}

int binary_search_left(station station_to_shift, int num_car) {
    int sx = 0;
    int dx = station_to_shift->size - 1;

    while (sx <= dx) {
        int mid = (sx+dx) / 2;
        if (station_to_shift->car[mid] == num_car)
            return mid;
        else {
            if (station_to_shift->car[mid] < num_car)
                dx = mid - 1;
            else sx = mid + 1;
        }
    }
    return -1;
}

void shift_left(station station_to_shift, int position){
    for (int x=position; x<station_to_shift->size-1; x++)
        station_to_shift->car[x]=station_to_shift->car[x+1];
    station_to_shift->size--;
}

void copy_array(station source, station destination){
    for (int x=0; x<source->size; x++)
        destination->car[x]=source->car[x];
}

station delete_node(station node){                                         //delete a node from the tree and return the node to free
    station x,y;
    if (node->sx == NULL || node->dx == NULL)
       y = node;
    else y = next_station(node);
    if (y->sx != NULL)
        x = y->sx;
    else x = y->dx;
    if (x != NULL)
        x->father = y->father;
    if (y->father == NULL)
        station_tree = x;
    else if (y == y->father->sx)
        y->father->sx = x;
    else y->father->dx = x;
    if (y != node) {
        if (node->capacity==0)
            node->car=(int*) malloc (sizeof(int) * y->capacity);
        else if (y->capacity>node->capacity)
            node->car=(int*) realloc (node->car, sizeof(int) * y->capacity);
        copy_array(y, node);
        node->distance = y->distance;
        node->size = y->size;
        node->capacity = y->capacity;
    }
    return y;
}

void demolisci_stazione() {
    int distance_input;
    if (scanf("%d", &distance_input) <= 0)                                                      //read the distance of the new station
        return;
    station to_consider = in_station_position(station_tree, distance_input);        //search the station in the station_tree
    if (to_consider==NULL){
        printf("non demolita\n");
    } else {
        free(delete_node(to_consider));
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
            to_consider->car = (int*) malloc(sizeof(int));
            to_consider->capacity = 1;
            to_consider->size = 1;
            if (scanf("%d", &to_consider->car[0]) <= 0)                                              //read the distance of the new station
                return;
            printf("aggiunta\n");
        } else {
            if (to_consider->size==0){
                if (scanf("%d", &to_consider->car[0]) <= 0)                                              //read the distance of the new station
                    return;
                to_consider->size=1;
                return;
            }
            if (to_consider->size == to_consider->capacity) {
                to_consider->capacity *= 2;
                (to_consider->car) = (int *) realloc (to_consider->car, sizeof(int) * to_consider->capacity);
            }
            int position, num_input;
            if (scanf("%d", &num_input) <= 0)                                              //read the distance of the car to place
                return;
            position = binary_search_right(to_consider, num_input);
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
        if (to_consider->capacity == 0 || to_consider->size == 0) {
            printf("non rottamata\n");
        } else {
            int num_input;
            if (scanf("%d", &num_input) <= 0)                                              //read the distance of the new station
                return;
            int position = binary_search_left(to_consider, num_input);
            if (position == -1) {
                printf("non rottamata\n");
            } else {
                if (to_consider->size==1) {
                    to_consider->size = 0;
                    printf("rottamata\n");
                    return;
                }
                shift_left(to_consider, position);
                printf("rottamata\n");
            }
        }
    }
}



void afterward(station station_1, station station_2){
   station temp_station = next_station(station_1), locked_station=station_1;
    do {
        while (temp_station!=NULL && locked_station->size!=0 && (locked_station->car[0] + locked_station->distance) >= temp_station->distance) {
            if (temp_station->last_visited == -1)
                temp_station->last_visited = locked_station->distance;
            temp_station = next_station(temp_station);
        }
        locked_station = next_station(locked_station);
        if (locked_station == temp_station)
            temp_station = next_station(locked_station);
    } while (locked_station->distance != station_2->distance);
    if (station_2->last_visited != -1){
        temp_station=station_2;
        insert_array(station_2->distance);
        do {
            if (temp_station->last_visited != -1)
                insert_array(temp_station->last_visited);
            else {
                printf("nessun percorso\n");
                return;
            }
            temp_station = in_station_position(station_tree, temp_station->last_visited);
        } while (temp_station->distance != station_1 -> distance);
        printf("%d", array_distance[array_size-1]);
        for (int x=array_size-2; x>=0; x--) {
            printf(" %d", array_distance[x]);
        }
        printf("\n");
    } else printf("nessun percorso\n");
}

void backward(station station_2, station station_1){
 station temp_station = next_station(station_1), locked_station=station_1;
 int last_distance=-1;
    do {
       while (temp_station!=NULL) {         //il problema è questo if
         if (temp_station->size!=0 && temp_station->distance>=last_distance && temp_station->last_visited==-1 && ((temp_station->distance - temp_station->car[0]) <= locked_station->distance)) {
                temp_station->last_visited = locked_station->distance;
               last_distance=temp_station->distance;
         }
            temp_station = next_station(temp_station);
        }
        do {
            locked_station = next_station(locked_station);
        } while (locked_station!=NULL && locked_station->distance<last_distance && locked_station->last_visited==-1);
        if (locked_station != NULL)
            temp_station = next_station(locked_station);
    } while (locked_station!=NULL && locked_station->distance != station_2->distance);
   if (station_2->last_visited != -1){
        temp_station=station_2;
        insert_array(temp_station->distance);
        do {
            if (temp_station->last_visited != -1) {
                insert_array(temp_station->last_visited);
            } else {
                printf("nessun percorso\n");
                return;
            }
            temp_station = in_station_position(station_tree, temp_station->last_visited);
        } while (temp_station!=NULL && temp_station->distance != station_1 -> distance);
        printf("%d", array_distance[0]);
        for (int x=1; x<array_size; x++){
            printf(" %d", array_distance[x]);
        }
        printf("\n");
    } else printf("nessun percorso\n");
}

void reset_distance(station tree_to_reset){
    if (tree_to_reset!=NULL) {
        reset_distance(tree_to_reset->sx);
        tree_to_reset->last_visited = -1;
        reset_distance(tree_to_reset->dx);
    }
}


void pianifica_percorso(){
    int distance_1, distance_2;
    if (scanf("%d", &distance_1) <= 0)                                              //read the distance of the new station
        return;
    if (scanf("%d", &distance_2) <= 0)                                              //read the distance of the new station
        return;
    reset_distance(station_tree);
    reset_array();
    if (distance_1 < distance_2) {
        afterward(in_station_position(station_tree, distance_1), in_station_position(station_tree, distance_2));
        //printf("\n");
        return;
    } else if (distance_1 > distance_2) {
        backward(in_station_position(station_tree, distance_1), in_station_position(station_tree, distance_2));
        //printf("\n");
        return;
    }
    printf("%d\n", distance_1);
}

int main() {
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
    }

    return 0;
}
