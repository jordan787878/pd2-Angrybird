#include "addbird.h"
#include <cstdlib>
#include "game.h"
#define NUM_Bird 4     //#define NUM_Bird 4

#define Red_Bird 0
#define White_Bird 1
#define Yellow_Bird 2
#define Blue_Bird 3


extern Game * game;

GameItem * AddBird()
{
    int bird_type;
    bird_type = rand()%NUM_Bird;
    GameItem * ptr;

    switch (bird_type) {

     case Red_Bird:
         ptr = new RedBird(game->world);
         break;

     case White_Bird:
         ptr = new WhiteBird(game->world);
         break;

     case Blue_Bird:
         ptr = new BlueBird(game->world);
         break;
     case Yellow_Bird:
         ptr = new YellowBird(game->world);
         break;

    }

    return ptr;
}
