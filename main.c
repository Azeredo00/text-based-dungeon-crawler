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
    int enemies_quantity, id, roll_counter, damage_dealt, enemies_defeated;

    enemies_quantity = 3;
    enemies_defeated = 0;

    //this line creates a pointer to pointers, an array of pointers, pointing to a memory space that holds a 'sheets' struct
    sheets **enemies = malloc(enemies_quantity * sizeof(sheets *));    

    for(id=0; id<3; id++){        
        //this line allocates memory for each struct 'sheets' and assigns the pointer to the array
        enemies[id] = malloc(sizeof(sheets));
        
        sheets skeleton = {rand()%8+1 + rand()%8+1 + 4, 13, 1, 6, 2, 10, 14, 15, 6, 8, 5};
        *enemies[id] = skeleton;
    }
    for(id=0; id<3; id++){
        
        //this line allocates memory for each struct 'sheets' and assigns the pointer to the array
        enemies[id] = malloc(sizeof(sheets));

        sheets zombie = {rand()%8+1 + rand()%8+1 + rand()%8+1 + 9, 8, 1, 6, 1, 13, 6, 16, 3, 6, 5};
        *enemies[id] = zombie;
    }

    while( player.health_points>0 && enemies_defeated<enemies_quantity ){
        printf("\n-- New Turn --\n");
        printf("Player HP: %d\n", player.health_points);
        for(id=0; id<3; id++){ //player attacks each skeleton in the room
            damage_dealt = 0; //each new battle, a new damage value is dealt
            printf("Skeleton %d HP: %d\n", id+1, enemies[id]->health_points);
            if(enemies[id]->health_points > 0){ //only attack if the skeleton is alive

                for(roll_counter=1; roll_counter<=player.damage_dice_quantity; roll_counter++){
                    damage_dealt += rand()%player.damage_dice+1;
                    if(roll_counter == player.damage_dice_quantity){
                        damage_dealt += player.damage_bonus;
                    }
                }

                printf("Player deals %d damage to Skeleton %d\n", damage_dealt, id+1);

                if(damage_dealt >= enemies[id]->health_points){ //if the skeleton is defeated
                    enemies[id]->health_points = 0;
                    enemies_defeated++;
                    printf("Skeleton %d defeated!\n", id+1);
                }else{
                    enemies[id]->health_points -= damage_dealt;
                }
            }else{
                printf("Skeleton %d is already defeated\n", id+1);
            }
        }
    }
    //only when the combat ends, free the memory allocated
    printf("Combat ended with %d Enemies defeated\n", enemies_defeated);
    for(id=0; id<3; id++){
        free(enemies[id]);
    }
    free(enemies);

    return 0;
}