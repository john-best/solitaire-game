#ifndef FREECELL_H
#define FREECELL_H

#include "game.h"

class Pile;

class FreeCell : public Game
{

    Pile *tableau[8];
    Pile *freecell[4];
    Pile *foundation[4];

public:

    game_types Type();
    void Redeal(QWidget *w);
    void newDeal(difficulty_type h);
    int getFreeMoves();
    void PlayoffTableau(TreePile *p, Card *c);
    void PlayoffFreeCell(Pile *p, Card *c);
    FreeCell(QWidget *parent);
    ~FreeCell();
};

#endif // FREECELL_H
