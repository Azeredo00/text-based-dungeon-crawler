#include <stdio.h>

typedef struct{
    int adjacent_rooms[4]; //array positions are, respectively: north, east, south, west
    int enemies_inside;
    int money_pile;
}rooms;

typedef struct{
    int health_points;
    int armor_class;
    int damage_points;
    int strength;
    int dexterity;
    int constitution;
    int inteligence;
    int wisdom;
    int charisma;
}sheets;

int main(){
    srand(time(NULL));
    int skeleton_quantity;
    scanf("%d", &skeleton_quantity);

    //this line creates an array of pointers pointing to a memory space that holds a 'sheets' struct
    sheets **skeleton = malloc(skeleton_quantity * sizeof(sheets *));
    for(int i = 0; i < skeleton_quantity; i++){
        
        //this line allocates memory for each struct 'sheets' and assigns the pointer to the array
        skeleton[i] = malloc(sizeof(sheets));
        
        //example of assigning values to the struct members
        skeleton[i]->health_points = rand()%8+1 + rand()%8+1 + 4; //2d8 roll + 4
        skeleton[i]->armor_class = 13;
        skeleton[i]->damage_points = 5; //or 1d6+2
        skeleton[i]->strength = rand()%20+1; //random value between 1 and 20
        skeleton[i]->dexterity = rand()%20+1;
        skeleton[i]->constitution = rand()%20+1;
        skeleton[i]->inteligence = rand()%20+1;
        skeleton[i]->wisdom = rand()%20+1;
        skeleton[i]->charisma = rand()%20+1;
    }
    return 0;
}