#ifndef COMMUNITY_H
#define COMMUNITY_H

#include "card.h"

class Community
{
public:
    Community() = default;

    Card card1;
    Card card2;
    Card card3;
    Card card4;
    Card card5;

    int currBet = 0;
    int pot = 0;
    int minBet = 0;
    int bigBlind = 0;
    int smallBlind = 0;

    void clear(){}
};

#endif // COMMUNITY_H
