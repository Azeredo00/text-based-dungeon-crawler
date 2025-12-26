#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
    int adjacent_rooms[4]; //array positions are, respectively: north, east, south, west
    int enemies_inside;
    int money_pile;
    int has_enemy;
}Room;

typedef struct{
    Class Class;
    int health_points;
    int armor_class;
    /*
    int damage_dice_quantity;
    int damage_dice;
    int damage_bonus;
    */
    int damage;
    int strength;
    int dexterity;
    int constitution;
    int inteligence;
    int wisdom;
    int charisma;
}Sheet;

//the health points value here does not matter, because they will be calculated at the start of the battle
//because they are random, and random values can not be assigned at compile time
Sheet players[4]; //array of 4 players
Sheet monsters[3]={ 
    {2, 8, 4, 0, 13, 1, 6, 2, 10, 14, 15, 6, 8, 5}, //skeleton
    {3, 8, 9, 0, 8, 1, 6, 1, 13, 6, 16, 3, 6, 5}, //zombie
    {2, 6, 0, 0, 15, 1, 6, 2, 8, 14, 10, 10, 8, 8} //goblin
};
typedef struct{
    int strength_bonus;
    int dexterity_bonus;
    int constitution_bonus;
    int inteligence_bonus;
    int wisdom_bonus;
    int charisma_bonus;
}Race;

Race races[3]={ //0 = human, 1 = elf, 2 = dwarf
    {1, 1, 1, 1, 1, 1}, //human
    {0, 2, 0, 0, 0, 0}, //elf
    {0, 0, 2, 0, 0, 0}  //dwarf
};

typedef struct{
    int health_dice_quantity;
    int health_dice;
    int health_bonus;
}Class;

Class classes[3]={ //0 = warrior, 1 = rogue, 2 = mage
    {1, 12, 12}, //warrior
    {1, 8, 8}, //rogue
    {1, 6, 6},  //mage
    {0, 0, 0}  //enemy
};
//created a battle function that takes, as a parameter, a pointer to a Sheet structs
void battle(){

    int enemies_quantity, id, roll_counter, damage_dealt, enemies_defeated, type_of_enemy;

    enemies_quantity = rand()%7+1;
    enemies_defeated = 0;
    type_of_enemy = rand()%3; //0 = skeleton, 1 = zombie, 2 = goblin

    //creates a pointer to pointers, an array of pointers, pointing to a memory space that will hold a 'Sheet' struct
    //because the number of enemies is dynamic, we allocate memory for the array of pointers
    Sheet **enemies = malloc(enemies_quantity * sizeof(Sheet *));    

    for(id=0; id<enemies_quantity; id++){

        enemies[id] = malloc(sizeof(Sheet)); //allocates memory for each struct 'Sheet' and assigns the pointer to the array
        *enemies[id] = monster_templates[type_of_enemy]; //copies the monster stats to the allocated memory

        for(roll_counter=1; roll_counter<=enemies[id]->health_dice_quantity; roll_counter++){ //total health points calculation
            enemies[id]->health_points += rand()%enemies[id]->health_dice+1;
        }
        enemies[id]->health_points += enemies[id]->health_bonus;
        
    }
    printf("A battle has started! There are a horde of %d enemies of type %d!\n", enemies_quantity, type_of_enemy);
    while(player.health_points>0 && enemies_defeated<enemies_quantity){

        printf("\n-- New Turn --\n");
        printf("Player HP: %d\n", player.health_points);

        for(id=0; id<enemies_quantity; id++){ //player attacks each enemy in the room

            damage_dealt = 0; //each new battle, a new damage value is dealt
            printf("Enemy %d HP: %d\n", id+1, enemies[id]->health_points);
            if(enemies[id]->health_points > 0){ //only attack if the enemy is alive

                for(roll_counter=1; roll_counter<=player.damage_dice_quantity; roll_counter++){
                    damage_dealt += rand()%player.damage_dice+1;
                    if(roll_counter == player.damage_dice_quantity){
                        damage_dealt += player.damage_bonus;
                    }
                }

                printf("Player deals %d damage to the enemy %d\n", damage_dealt, id+1);

                if(damage_dealt >= enemies[id]->health_points){ //if the enemy is defeated
                    enemies[id]->health_points = 0;
                    enemies_defeated++;
                    printf("Enemy %d defeated!\n", id+1);
                }else{
                    enemies[id]->health_points -= damage_dealt;
                }
            }else{
                printf("Enemy %d is already defeated\n", id+1);
            }
        }
    }
    //only when the battle ends, free the memory allocated
    printf("Battle ended with %d Enemies defeated\n", enemies_defeated);
    for(id=0; id<3; id++){
        free(enemies[id]);
    }
    free(enemies);
}

int main(){
    srand(time(NULL));
    int selected_class, health_given, armor_class_given, id, attributes[6], choice;

    //character creation
    for(id=0; id<4; id++){
        attributes[6]={rand()%20+1, rand()%20+1, rand()%20+1, rand()%20+1, rand()%20+1, rand()%20+1}; //ability scores rolls
        printf("Attributes rolled: ");
        for(choice=0; choice<6; choice++){
            printf("%d", attributes[choice]);
            if(choice<5){
                printf(", ");
            }else{
                printf("\n");
            }
        }
        printf("Distribute attributes for player %d:\n", id+1);
        for(id=0; id<6; id++){
            switch(choice){
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
            }
        }
        selected_class=rand()%3; //except the enemy class
        health_given = rand()%classes[selected_class].health_dice + 1 + classes[selected_class].health_bonus; //initial health points roll
        armor_class_given = 10 + players[id].dexterity; //initial armor class calculation
        players[id]={
            classes[selected_class],
            health_given,
            0,
            0,
            0,
            0,
            0, 0, 0, 0, 0, 0}; //attributes
    };
    int roll_counter;
    battle();
    return 0;
}