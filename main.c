#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
    int adjacent_rooms[4]; //array positions are, respectively: north, east, south, west
    int enemies_inside;
    int money_pile;
}rooms;

typedef struct{
    int health_points;
    int armor_class;
    int damage_dice_quantity;
    int damage_dice;
    int damage_bonus;
    int strength;
    int dexterity;
    int constitution;
    int inteligence;
    int wisdom;
    int charisma;
}sheets;

int main(){
    sheets player={20, 15, 1, 8, 3, 15, 14, 13, 12, 10, 8};
    srand(time(NULL));
    int skeleton_quantity, id, roll_counter, damage_dealt;

    skeleton_quantity = 1;

    //this line creates a pointer to pointers, an array of pointers, pointing to a memory space that holds a 'sheets' struct
    sheets **skeletons = malloc(skeleton_quantity * sizeof(sheets *));

    for(id=0; id<skeleton_quantity; id++){
        
        //this line allocates memory for each struct 'sheets' and assigns the pointer to the array
        skeletons[id] = malloc(sizeof(sheets));
        
        skeletons[id]->health_points = rand()%8+1 + rand()%8+1 + 4; //2d8 roll + 4
        skeletons[id]->armor_class = 13;
        skeletons[id]->damage_dice_quantity = 1;
        skeletons[id]->damage_dice = 6;
        skeletons[id]->damage_bonus = 2;
        skeletons[id]->strength = rand()%20+1; //random value between 1 and 20
        skeletons[id]->dexterity = rand()%20+1;
        skeletons[id]->constitution = rand()%20+1;
        skeletons[id]->inteligence = rand()%20+1;
        skeletons[id]->wisdom = rand()%20+1;
        skeletons[id]->charisma = rand()%20+1;
    }
    while( player.health_points>0 && skeleton_quantity>0 ){
        printf("\n-- New Turn --\n");
        for(id=0; id<skeleton_quantity; id++){
            damage_dealt = 0; //each new turn, is a new damage dealt value
            printf("Player HP: %d\n", player.health_points);
            printf("Skeleton %d HP: %d\n", id+1, skeletons[id]->health_points);
            for(roll_counter=1; roll_counter<=player.damage_dice_quantity; roll_counter++){
                damage_dealt += rand()%player.damage_dice+1;
                if(roll_counter == player.damage_dice_quantity){
                    damage_dealt += player.damage_bonus;
                }
            }
            printf("Player deals %d damage to Skeleton %d\n", damage_dealt, id+1);
            if(damage_dealt >= skeletons[id]->health_points){ //if the skeleton is defeated
                skeletons[id]->health_points = 0;
                printf("Skeleton %d defeated!\n", id+1);
                skeleton_quantity--;
            }else{
                skeletons[id]->health_points -= damage_dealt;
            }
        }
    }
    //only when the combat ends, free the memory allocated
    for(id=0; id<skeleton_quantity; id++){
        free(skeletons[id]);
    }
    free(skeletons);
    
    return 0;
}