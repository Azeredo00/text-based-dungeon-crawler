#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int party_size;

enum directions_enum {NORTH, EAST, SOUTH, WEST, DIRECTIONS_QUANTITY};
typedef struct{
    int adjacent_rooms[DIRECTIONS_QUANTITY]; //array positions are, respectively: north, east, south, west
    int enemies_inside;
    int money_pile;
    int has_enemy;
}Room;

enum character_types_enum {PLAYER, ENEMY, CHARACTER_TYPES_QUANTITY};
typedef struct{ //this struct will be an array arranged by order of iniciative rolled
    enum character_types_enum character_type; //is it a player or a monster?
    int character_index; //inside their own struct array, which index are theirs?
    int roll;
}Iniciative;

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

enum classes_enum {BARBARIAN, ROGUE, SORCERER, CLASSES_QUANTITY};
Class classes[CLASSES_QUANTITY]={
    [BARBARIAN] = {1, 12, 12},
    [ROGUE] = {1, 8, 8},  
    [SORCERER] = {1, 6, 6}
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
    [SKELETON] = {
            {0, 13, {1, 8, 2}, 0, 10, 14, 12, 6, 8, 5},
        2, 6, 0},
    [ZOMBIE] = {
            {0, 8, {1, 6, 1}, 0, 16, 6, 16, 3, 6, 5},
        2, 8, 0},
    [GOBLIN] = {
            {0, 15, {1, 4, 3}, 0, 8, 14, 10, 10, 8, 8},
        1, 6, 0}
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
        /* for now, every character will be random
        printf("Do you want this character to be random?\n (1 = yes, 0 = no): ");
        scanf("%d", &randomness_toggle);
        */
        randomness_toggle = 1; //for now, every character will be random
        
        if(randomness_toggle==1){
            //randomly assign attributes
            
            players[id].class = &classes[rand()%3];
            players[id].race = &races[rand()%3];
            players[id].basic_stats.weapon = weapons[rand()%(WEAPONS_QUANTITY-1)+1];

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
        players[id].basic_stats.health_points = (rand() % (players[id].class->health_dice + 1)) + players[id].class->health_bonus; //initial health points roll
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
    int enemies_quantity, id, id2, roll_counter, damage_dealt, enemies_defeated, enemy_to_attack, player_to_attack, party_members_defeated, monster_type;
    
    enemies_quantity = rand()%7+1;

    enemies_defeated = 0;
    party_members_defeated = 0;

    //it will hold the index of the players and enemies in order of initiative
    int iniciative[party_size + enemies_quantity]; 
    
    monster_type = rand()%MONSTERS_QUANTITY;

    //creates a pointer to pointers, an array of pointers, pointing to a memory space that will hold a 'Monster' struct
    //because the number of enemies is dynamic, we allocate memory for the array of pointers
    //pointer enemy that points to a pointer of type 'Monster', that points to a memory space that holds a 'Monster' struct
    Monster *enemies = malloc(enemies_quantity * sizeof(Monster));
    for(id=0; id<enemies_quantity; id++){

        enemies[id] = monsters[monster_type]; //copies the monster stats to the allocated memory
        enemies[id].basic_stats.weapon = weapons[rand()%(WEAPONS_QUANTITY-1)+1];

        for(roll_counter=1; roll_counter<=enemies[id].health_dice_quantity; roll_counter++){ //total health points calculation
            enemies[id].basic_stats.health_points += rand()%enemies[id].health_dice+1;
        }
        enemies[id].basic_stats.health_points += enemies[id].health_bonus;
        
    }
    Iniciative iniciatives[party_size + enemies_quantity];
    Iniciative iniciative_buffer;
    for(id=0; id<party_size + enemies_quantity; id++){
        if(id<party_size){
            iniciatives[id].character_type = PLAYER;
            iniciatives[id].character_index = id;
            iniciatives[id].roll = (rand()%20 + 1) + modifier_calculator(players[id].basic_stats.dexterity);
        }else{
            iniciatives[id].character_type = ENEMY;
            iniciatives[id].character_index = id - party_size;
            iniciatives[id].roll = (rand()%20 + 1) + modifier_calculator(enemies[id - party_size].basic_stats.dexterity);
        }
    }
    //sorting the iniciatives array by roll value, from highest to lowest
    for(id=0; id<party_size + enemies_quantity - 1; id++){
        for(id2=id+1; id2<party_size + enemies_quantity ; id2++){ //id2 is one step ahead of id
            if(iniciatives[id].roll < iniciatives[id2].roll){ //if the roll is less than the next one, swap them
                iniciative_buffer = iniciatives[id];
                iniciatives[id] = iniciatives[id2];
                iniciatives[id2] = iniciative_buffer;
            }
        }
    }
    printf("\nA battle has started! There are a horde of %d enemies of type %d!\n", enemies_quantity, monster_type);
    for(id=0; id<party_size + enemies_quantity; id++){
        printf("Iniciative order %d: ", id+1);
        if(iniciatives[id].character_type == PLAYER){
            printf("Player %d with a roll of %d\n", iniciatives[id].character_index+1, iniciatives[id].roll);
        }else if(iniciatives[id].character_type == ENEMY){
            printf("Enemy %d with a roll of %d\n", iniciatives[id].character_index+1, iniciatives[id].roll);
        } 
    }
    while(party_members_defeated<party_size && enemies_defeated<enemies_quantity){
        printf("\nNew Round!\n");
        for(id=0; id<party_size + enemies_quantity; id++){
            damage_dealt = 0;
            if(iniciatives[id].character_type == PLAYER){
                printf("\nPlayer %d's turn!\n", iniciatives[id].character_index+1);
                if(players[iniciatives[id].character_index].basic_stats.health_points>0){
                    printf("Player %d HP: %d\n", iniciatives[id].character_index+1, players[iniciatives[id].character_index].basic_stats.health_points);
                    //player's turn
                    for(roll_counter=1; roll_counter<=players[iniciatives[id].character_index].basic_stats.weapon.damage_dice_quantity; roll_counter++){ //damage calculation
                        damage_dealt += rand()%(players[iniciatives[id].character_index].basic_stats.weapon.damage_dice + 1);
                        if(roll_counter == players[iniciatives[id].character_index].basic_stats.weapon.damage_dice_quantity){
                            damage_dealt += players[iniciatives[id].character_index].basic_stats.weapon.damage_bonus;
                        }
                    }
                    if(0){
                        printf("Choose an enemy to attack (1 to %d): ", enemies_quantity);
                        while(1){
                            scanf("%d", &enemy_to_attack);
                            enemy_to_attack -= 1; //to match array index
                            if(enemies[enemy_to_attack].basic_stats.health_points<=0){
                                printf("Enemy %d is already defeated! Choose another enemy.\n", enemy_to_attack+1);
                            }else{
                                break;
                            }
                        }
                    }else{
                        enemy_to_attack = rand()%enemies_quantity;
                        if(enemies_defeated==enemies_quantity){
                            printf("All enemies have been defeated! Players win!\n");
                        }else{
                            while(enemies[enemy_to_attack].basic_stats.health_points<=0){
                                printf("Enemy %d is already defeated! Choosing another enemy.\n", enemy_to_attack+1);
                                enemy_to_attack++;
                                if(enemy_to_attack>=enemies_quantity){
                                    enemy_to_attack = 0;
                                }
                            }
                            enemies[enemy_to_attack].basic_stats.health_points -= damage_dealt;
                            printf("Player %d attacks the enemy for %d damage!\n", iniciatives[id].character_index+1, damage_dealt);
                            if(enemies[enemy_to_attack].basic_stats.health_points <= 0){
                                printf("Enemy defeated!\n");
                                enemies_defeated++;
                            }
                        }
                    }
                }else{
                    printf("Player %d is defeated and cannot act this turn.\n", iniciatives[id].character_index+1);
                }
            }else if(iniciatives[id].character_type == ENEMY){
                printf("\nEnemy %d's turn!\n", iniciatives[id].character_index+1);
                if(enemies[iniciatives[id].character_index].basic_stats.health_points>0){
                    //enemy's turn
                    for(roll_counter=1; roll_counter<=enemies[iniciatives[id].character_index].basic_stats.weapon.damage_dice_quantity; roll_counter++){ //damage calculation
                        damage_dealt += rand()%(enemies[iniciatives[id].character_index].basic_stats.weapon.damage_dice + 1);
                        if(roll_counter == enemies[iniciatives[id].character_index].basic_stats.weapon.damage_dice_quantity){
                            damage_dealt += enemies[iniciatives[id].character_index].basic_stats.weapon.damage_bonus;
                        }
                    }
                    player_to_attack = rand()%party_size;
                    if(party_members_defeated==party_size){
                        printf("All players have been defeated! Enemies win!\n");
                    }else{
                        while(players[player_to_attack].basic_stats.health_points<=0){
                            printf("Player %d is already defeated! Choosing another player.\n", player_to_attack+1);
                            player_to_attack++;
                            if(player_to_attack>=party_size){
                                player_to_attack = 0;
                            }
                        }
                        players[player_to_attack].basic_stats.health_points -= damage_dealt;
                        printf("Enemy attacks Player %d for %d damage!\n", player_to_attack+1, damage_dealt);
                        if(players[player_to_attack].basic_stats.health_points <= 0){
                            printf("Player %d has been defeated!\n", player_to_attack+1);
                            party_members_defeated++;
                            printf("%d party members defeated so far.\n", party_members_defeated);
                        }
                    }
                }
            }
        }
    }
    //only when the battle ends, free the memory allocated
    printf("\nBattle Over!\n");
    printf("%d Enemies defeated\n", enemies_defeated);
    printf("%d Party members defeated\n", party_members_defeated);
    if(party_members_defeated==party_size){
        printf("The party has been defeated!\n");
    }else if(enemies_defeated==enemies_quantity){
        printf("The party has won the battle!\n");
    }
    free(enemies);
}

int main(){
    srand(time(NULL));
    party_creator();
    battle();
    return 0;
}