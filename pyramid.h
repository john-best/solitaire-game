#ifndef PYRAMID_H
#define PYRAMID_H

#include "game.h"

class Pile;
class TreePile;

class Pyramid : public Game
{

    TreePile *tableau;
    Pile *waste;
    Pile *stock;
    Pile *foundation;
    int redrawLimit;
    TreeCard* Deck[52];
public:
    game_types Type();
    void Redeal(QWidget *w);
    void newDeal(difficulty_type h);
    void DealCard(Pile *p);
    bool FindBestCard(TreeCard *c);
    bool OneCardPlayoff(Card *c);
    void CheckWin();
    void Clear();
    Pyramid(QWidget *parent);
    ~Pyramid();
};

#endif // PYRAMID_H
