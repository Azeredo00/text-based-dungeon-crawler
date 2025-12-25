#include <stdio.h>

typedef struct{
    int id;
    int adjacent_rooms[4];
    int enemies_inside;
    int money_pile;
}rooms;

typedef struct{
    int id;
    int health_points;
    int armor_class;
    int damage_points;
    int strength;
    int dexterity;
    int constitution;
    int inteligence;
    int wisdom;
    int charisma;
}enemies;

int main(){
    printf("a");
    return 0;
}