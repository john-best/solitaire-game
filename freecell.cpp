#include "freecell.h"
#include "game.h"
#include "rule.h"
#include "move.h"
#include "mainwindow.h"
#include "card.h"
#include "pile.h"


extern Game *game;
const int CARD_SPACING = 5;

game_types FreeCell::Type()
{
    return FREECELL_GAME;
}

void FreeCell::Redeal(QWidget *w)
{
    if (game)
    {
        game->Clear();
        delete game;
        game = NULL;
    }
    game = new FreeCell(w);
    game->newDeal();
}

void FreeCell::newDeal(difficulty_type h)
{
    for (int n = 0; n < 8; n++)
        tableau[n] = new TableauPile(10 + (n) * (CARD_WIDTH + CARD_SPACING + 10), 10 + CARD_HEIGHT + 15, 0, 18, parent);
    tableau[0]->AddDropRules(3,
                             new ProhibitDropSameColor,
                             new DropOneDown,
                             new DropFreeCellLimit);
    tableau[0]->AddDragRules(3,
                             new DragStackDecreasing,
                             new DragFreeCellLimit,
                             new DragAlternatingColors);
    for (int n = 0; n < 4; n++)
        freecell[n] = new FreeCellPile(10 + (n) * (CARD_WIDTH + CARD_SPACING), 10, 0, 0, parent);
    freecell[0]->AddDropRules(2,
                              new DropEmptyStack,
                              new DropSingle);
    freecell[0]->AddDragRules(0);
    for (int n = 0; n < 4; n++)
        foundation[n] = new FoundationPile(79 + (n+4) * (CARD_WIDTH + CARD_SPACING), 10, 0, 0, parent);
    foundation[0]->AddDropRules(4,
                                new AceOnEmpty,
                                new AllowDropSameSuit,
                                new DropOneUp,
                                new DropSingle);
    foundation[0]->AddDragRules(1,
                                new ProhibitDrag);

    Card* Deck[52];
    for (int v = 0; v < 52; v++)
        Deck[v] = new Card(v, parent);
    Shuffle(Deck, 52);
    int n = 0;
    while (n < 52)
    {
        int column = 0;
        while (column < 8 && n < 52)
        {
            Deck[n]->Faceup(true);
            tableau[column]->acceptCard(Deck[n++], true, false, false);
            column++;
        }
    }

}

int FreeCell::getFreeMoves()
{
    int free = 0;
    for (int n = 0; n < 4; n++)
        free += 0 == freecell[n]->Top();
    int empty = 0;
    for (int n = 0; n < 8; n++)
        empty += 0 == tableau[n]->Top();

    return (free + 1) << empty;
}

void FreeCell::PlayoffTableau(TreePile *p, Card *c)
{
    int i = 0;
    while (i < 4 && freecell[i]->Top())
        i++;
    if (i == 4)
        return;
        if (freecell[i]->checkDropRules(c))
        {
            freecell[i]->acceptCard(c);
        }
}

void FreeCell::PlayoffFreeCell(Pile *p, Card *c)
{
    OneCardPlayoff(c);
}

FreeCell::FreeCell(QWidget *parent):
    Game(parent)
{

}

FreeCell::~FreeCell()
{

}

