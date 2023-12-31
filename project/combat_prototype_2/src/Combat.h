// Combat.h

#pragma once
#include "raylib.h"
#include "vector"
#include <string>


// Different states of combat
enum MenuState 
{ 
    ENEMY_TURN, 
    START, 
    ATTACK, 
    DEFEND, 
    ITEMS,
    RESOLVE 
};

class Char_Stat{
    public:
        int currentHp;
        int maxHp;
        int atkDmg;
        int armor;
        bool isPlayer;
        std::string combat_texture_path; //path towards texture file that will hold the sprites used in combat
    

        Char_Stat(const std::string& path);
        Char_Stat();

        //could probably be replaced with just including 
        //Char_Stat.maxHp -= Char_Stat.atkDam + Char_Stat.armor

};

class Combat{
    public:
        //stats need to be initialized for these arrays  

        // TODO for Alton - get a better idea of json files and pulling infor from them
        // this will allow you to better optimise thhe classes for stats and inventory

        int pSize;
        int eSize;

        void initialize_combat(std::vector<std::string> party, std::vector<std::string> enemy);
        bool combat_resolved();
        bool game_over();
        void initiate_round();

        void initialize_stats(std::string path);

        // initialize pulls the stats from the 
        void initialize_stats();
        void update_stats();
        int grab_int_stat(std::string path, std::string variable);

        Combat();

        MenuState menuState;

        std::vector<Char_Stat> initiative;
};