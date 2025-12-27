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
    int health_points;
    int armor_class;
    Weapon weapon;
    int damage;
    int strength;
    int dexterity;
    int constitution;
    int intelligence;
    int wisdom;
    int charisma;
}Base;

typedef struct{
    Base basic_stats;
    int health_dice_quantity;
    int health_dice;
    int health_bonus;
}Monster;

typedef struct{
    //players do not need to have a copy of the class and race
    //since all the players of the same class and race would share the same stats
    //we can just have a pointer instead of copies of the structs
    Class *class;
    Base basic_stats;
    Race *race;
}Person;

//the health points value here does not matter, because they will be calculated at the start of the battle
//because they are random, and random values can not be assigned at compile time
Person players[4]; //array of 4 players
Monster monsters[3]={
    /* 
    {2, 8, 4, 0, 0, 13, 0, 1, 6, 2, 10, 14, 15, 6, 8, 5}, //skeleton
    {3, 8, 9, 0, 0, 8, 0, 1, 6, 1, 13, 6, 16, 3, 6, 5}, //zombie
    {2, 6, 0, 0, 0, 15, 0, 1, 6, 2, 8, 14, 10, 10, 8, 8} //goblin
    */    
};

typedef struct{
    int damage_dice_quantity;
    int damage_dice;
    int damage_bonus;
}Weapon;

Weapon weapons[3]={ //0 = sword, 1 = bow, 2 = staff
    {1, 8, 2}, //sword
    {1, 6, 1}, //bow
    {1, 4, 3}  //staff
};

typedef struct{
    int strength_bonus;
    int dexterity_bonus;
    int constitution_bonus;
    int intelligence_bonus;
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

Class classes[3]={ //0=warrior, 1=rogue, 2=mage
    {1, 12, 12}, //warrior
    {1, 8, 8}, //rogue  
    {1, 6, 6}  //mage
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
        *enemies[id] = monsters[type_of_enemy]; //copies the monster stats to the allocated memory

        for(roll_counter=1; roll_counter<=enemies[id]->health_dice_quantity; roll_counter++){ //total health points calculation
            enemies[id]->health_points += rand()%enemies[id]->health_dice+1;
        }
        enemies[id]->health_points += enemies[id]->health_bonus;
        
    }
    printf("A battle has started! There are a horde of %d enemies of type %d!\n", enemies_quantity, type_of_enemy);
    while(players[id].basic_stats.health_points>0 && enemies_defeated<enemies_quantity){

        printf("\n-- New Turn --\n");
        printf("Player HP: %d\n", players[id].basic_stats.health_points);

        for(id=0; id<enemies_quantity; id++){ //player attacks each enemy in the room

            damage_dealt = 0; //each new battle, a new damage value is dealt
            printf("Enemy %d HP: %d\n", id+1, enemies[id]->health_points);
            if(enemies[id]->health_points > 0){ //only attack if the enemy is alive

                for(roll_counter=1; roll_counter<=players[id].basic_stats.weapon.damage_dice_quantity; roll_counter++){
                    damage_dealt += rand()%players[id].basic_stats.weapon.damage_dice+1;
                    if(roll_counter == players[id].basic_stats.weapon.damage_dice_quantity){
                        damage_dealt += players[id].basic_stats.weapon.damage_bonus;
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
    int id, id2, attributes[6], choice, roll_counter;

    //character creation
    for(id=0; id<4; id++){
        attributes[6] = {rand()%20+1, rand()%20+1, rand()%20+1, rand()%20+1, rand()%20+1, rand()%20+1}; //ability scores rolls
        printf("Dice rolls: ");
        for(choice=0; choice<6; choice++){
            printf("%d", attributes[choice]);
            if(choice<5){
                printf(", ");
            }else{
                printf("\n");
            }
        }
        printf("Distribute attributes for player %d:\n", id+1);
        for(id2=0; id2<6; id2++){
            printf("Choose the attribute to assign the roll value of %d: ", attributes[id2]);
            printf("0 = strength, 1 = dexterity, 2 = constitution, 3 = intelligence, 4 = wisdom, 5 = charisma\n");
            scanf("%d", &choice);
            switch(choice){
                case 0:
                    players[id].basic_stats.strength = attributes[id2];
                    break;
                case 1:
                    players[id].basic_stats.dexterity = attributes[id2];
                    break;
                case 2:
                    players[id].basic_stats.constitution = attributes[id2];
                    break;
                case 3:
                    players[id].basic_stats.intelligence = attributes[id2];
                    break;
                case 4:
                    players[id].basic_stats.
                    wisdom = attributes[id2];
                    break;
                case 5:
                    players[id].basic_stats.charisma = attributes[id2];
                    break;
            }
        }
        players[id].class = &classes[rand()%3]; //except the enemy class
        players[id].race = &races[rand()%3];
        players[id].basic_stats.weapon = weapons[rand()%3];
        players[id].basic_stats.health_points = rand()%(players[id].class->health_dice + 1) + players[id].class->health_bonus; //initial health points roll
        players[id].basic_stats.armor_class = 10 + players[id].basic_stats.dexterity; //initial armor class calculation
        players[id].basic_stats.damage = 0;
        for(roll_counter=1; roll_counter<=players[id].basic_stats.weapon.damage_dice_quantity; roll_counter++){ //damage calculation
            players[id].basic_stats.damage += rand()%(players[id].basic_stats.weapon.damage_dice + 1);
            if(roll_counter == players[id].basic_stats.weapon.damage_dice_quantity){
                players[id].basic_stats.damage += players[id].basic_stats.weapon.damage_bonus;
            }
        }
        players[id].basic_stats.strength += players[id].race->strength_bonus;
        players[id].basic_stats.dexterity += players[id].race->dexterity_bonus;
        players[id].basic_stats.constitution += players[id].race->constitution_bonus;
        players[id].basic_stats.intelligence += players[id].race->intelligence_bonus;
        players[id].basic_stats.wisdom += players[id].race->wisdom_bonus;
        players[id].basic_stats.charisma += players[id].race->charisma_bonus;
    };
    int roll_counter;
    battle();
    return 0;
}