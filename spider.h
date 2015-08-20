#ifndef SPIDER_H
#define SPIDER_H

#include "game.h"

class Pile;

class Spider : public Game
{

    Pile *stock[5];
    Pile *foundation[8];
    Pile *tableau[10];
    difficulty_type difficulty;

public:
    game_types Type();
    void Redeal(QWidget *w);
    void newDeal(difficulty_type h);
    void DealCard(Pile *p);
    void PlayoffTableau(TreePile *p, Card *c);
    void PlayoffAll();
    Pile *FoundationTarget(Pile *p, Card *c);
    Spider(QWidget *parent);
    ~Spider();
};

#endif // SPIDER_H
