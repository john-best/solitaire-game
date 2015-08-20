#ifndef KLONDIKE_H
#define KLONDIKE_H
#include "game.h"

class Pile;


class Klondike : public Game
{
    Pile *stock;
    Pile *waste;
    Pile *foundation[4];
    Pile *tableau[7];

public:
    game_types Type();
    void Redeal(QWidget *w);
    void newDeal(difficulty_type h);
    Klondike(QWidget *parent);
    ~Klondike();
    void DealCard(Pile *p);
    void setDealNumber(int n);
    void PlayoffTableau(TreePile *p, Card *c);
};

#endif // KLONDIKE_H
