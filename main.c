#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int party_size;

typedef struct{
    int adjacent_rooms[4]; //array positions are, respectively: north, east, south, west
    int enemies_inside;
    int money_pile;
    int has_enemy;
}Room;

typedef struct{
    int damage_dice_quantity;
    int damage_dice;
    int damage_bonus;
}Weapon;

enum weapons_enum {SWORD, BOW, STAFF, WEAPONS_QUANTITY};
Weapon weapons[WEAPONS_QUANTITY]={
    [SWORD] = {1, 8, 2},
    [BOW] = {1, 6, 1},
    [STAFF] = {1, 4, 3}
};

typedef struct{
    int strength_bonus;
    int dexterity_bonus;
    int constitution_bonus;
    int intelligence_bonus;
    int wisdom_bonus;
    int charisma_bonus;
}Race;

enum races_enum {HUMAN, ELF, DWARF, RACES_QUANTITY};
Race races[RACES_QUANTITY]={
    [HUMAN] = {1, 1, 1, 1, 1, 1},
    [ELF] = {0, 2, 0, 0, 0, 0},
    [DWARF] = {0, 0, 2, 0, 0, 0}
};

typedef struct{
    int health_dice_quantity;
    int health_dice;
    int health_bonus;
}Class;

enum classes_enum {WARRIOR, ROGUE, MAGE, CLASSES_QUANTITY};
Class classes[CLASSES_QUANTITY]={
    [WARRIOR] = {1, 12, 12},
    [ROGUE] = {1, 8, 8},  
    [MAGE] = {1, 6, 6}
};

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
//67
typedef struct{
    //players do not need to have a copy of the class and race67
    //since all the players of the same class and race would share the same stats
    //we can just have a pointer instead of copies of the structs
    Class *class;
    Base basic_stats;
    Race *race;
}Person;
Person *players;

typedef struct{
    Base basic_stats;
    int health_dice_quantity;
    int health_dice;
    int health_bonus;
}Monster;

//the health points value here does not matter, because they will be calculated at the start of the battle
//because they are random, and random values can not be assigned at compile time
enum monsters_enum {SKELETON, ZOMBIE, GOBLIN, MONSTERS_QUANTITY};
Monster monsters[MONSTERS_QUANTITY]={
};

int modifier_calculator(int attribute_score){
    int attribute_modifier;
    if(attribute_score>=10){
        attribute_modifier = (attribute_score - 10) / 2;
    }else{
        attribute_modifier = (attribute_score - 11) / 2;
    }
    return attribute_modifier;
}

void party_creator(){   
    int id, id2, id3, attributes[6], choice, roll_counter, four_roll_buffer[4], lowest_roll_index, randomness_toggle;
    
    //character creation
    printf("Enter party size: ");
    scanf("%d", &party_size);
    players = malloc(party_size * sizeof(Person));

    for(id=0; id<party_size; id++){
        
        for(id2=0; id2<6; id2++){ //rolling for attributes
            for(roll_counter=1; roll_counter<=4; roll_counter++){
                four_roll_buffer[roll_counter-1] = rand()%6 + 1;
            }

            lowest_roll_index = 0;
            for(id3=0; id3<4; id3++){
                if(four_roll_buffer[id3] < four_roll_buffer[lowest_roll_index]){
                    lowest_roll_index = id3;
                }
            }
            
            attributes[id2] = 0;
            for(id3=0; id3<4; id3++){
                attributes[id2] += four_roll_buffer[id3];
            }
            attributes[id2] -= four_roll_buffer[lowest_roll_index]; //removing the lowest roll
        }

        printf("Dice rolls: ");
        for(choice=0; choice<6; choice++){
            printf("%d", attributes[choice]);
            if(choice<5){
                printf(", ");
            }else{
                printf("\n");
            }
        }

        printf("Do you want this character to be random?\n (1 = yes, 0 = no): ");
        scanf("%d", &randomness_toggle);
        
        if(randomness_toggle==1){
            //randomly assign attributes
            
            players[id].class = &classes[rand()%3];
            players[id].race = &races[rand()%3];
            players[id].basic_stats.weapon = weapons[rand()%3];

            players[id].basic_stats.strength = attributes[rand()%6];
            players[id].basic_stats.dexterity = attributes[rand()%6];
            players[id].basic_stats.constitution = attributes[rand()%6];
            players[id].basic_stats.intelligence = attributes[rand()%6];
            players[id].basic_stats.wisdom = attributes[rand()%6];
            players[id].basic_stats.charisma = attributes[rand()%6];
            
        }else if(randomness_toggle==0){
            printf("Distribute attributes for player %d:\n", id+1);
            for(id2=0; id2<6; id2++){
                printf("0 = strength, 1 = dexterity, 2 = constitution, 3 = intelligence, 4 = wisdom, 5 = charisma\n");
                printf("Choose the attribute to assign the roll value of %d: ", attributes[id2]);
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
                        players[id].basic_stats.wisdom = attributes[id2];
                        break;
                    case 5:
                        players[id].basic_stats.charisma = attributes[id2];
                        break;
                }
            }
        }
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
    }
}

//created a battle function that takes, as a parameter, a pointer to a Sheet structs
void battle(){
    int enemies_quantity, id, roll_counter, damage_dealt, enemies_defeated, type_of_enemy;
    enemies_quantity = rand()%7+1;

    //it will hold the index of the players and enemies in order of initiative
    int iniciative[party_size + enemies_quantity]; 
    
    enemies_defeated = 0;
    monster_type = rand()%MONSTERS_QUANTITY;

    //creates a pointer to pointers, an array of pointers, pointing to a memory space that will hold a 'Monster' struct
    //because the number of enemies is dynamic, we allocate memory for the array of pointers
    //pointer enemy that points to a pointer of type 'Monster', that points to a memory space that holds a 'Monster' struct
    Monster *enemies = malloc(enemies_quantity * sizeof(Monster));
    for(id=0; id<enemies_quantity; id++){

        enemies[id] = monsters[monster_type]; //copies the monster stats to the allocated memory

        for(roll_counter=1; roll_counter<=enemies[id].health_dice_quantity; roll_counter++){ //total health points calculation
            enemies[id].basic_stats.health_points += rand()%enemies[id].health_dice+1;
        }
        enemies[id].basic_stats.health_points += enemies[id].health_bonus;
        
    }
    for(id=0; id<party_size + enemies_quantity; id++){
        iniciative[id] = ; //to be implemented
    }
    printf("A battle has started! There are a horde of %d enemies of type %d!\n", enemies_quantity, monster_type);
    while(players[id].basic_stats.health_points>0 && enemies_defeated<enemies_quantity){
    }
    //only when the battle ends, free the memory allocated
    printf("Battle ended with %d Enemies defeated\n", enemies_defeated);
    free(enemies);
}

int main(){
    srand(time(NULL));
    party_creator();
    //battle();
    return 0;
}