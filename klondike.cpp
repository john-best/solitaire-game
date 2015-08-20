#include "klondike.h"
#include "pile.h"
#include <QWidget>
#include "card.h"
#include "game.h"
#include "rule.h"
#include "move.h"
#include "mainwindow.h"

extern Game *game;
int dealNumber = 3;
const int CARD_SPACING = 5;
game_types Klondike::Type()
{
    return KLONDIKE;
}

void Klondike::Redeal(QWidget *w)
{
    if (game)
    {
        game->Clear();
        delete game;
        game = NULL;
    }
    game = new Klondike(w);
    game->newDeal();
}

void Klondike::newDeal(difficulty_type h)
{
    stock = new StockPile(10,10,0,0,parent);
    stock->AddDropRules(1,
                        new ProhibitDrop);
    stock->AddDragRules(1,
                        new ProhibitDrag);


    waste = new WastePile(10 + CARD_WIDTH + 5, 10, 16, 0, parent);
    waste->AddDropRules(1,
                        new ProhibitDrop);
    waste->AddDragRules(1,
                        new DragTopOnly);


    for (int n = 0; n < 4; n++)
        foundation[n] = new FoundationPile(10 + (n+3) * (CARD_WIDTH + CARD_SPACING), 10, 0, 0, parent);
    foundation[0]->AddDropRules(4,
                                new AceOnEmpty,
                                new AllowDropSameSuit,
                                new DropOneUp,
                                new DropSingle);
    foundation[0]->AddDragRules(1,
                                new ProhibitDragKing);

    for (int n = 0; n < 7; n++)
        tableau[n] = new TableauPile(10 + n * (CARD_WIDTH + CARD_SPACING), 10 + CARD_HEIGHT + 15, 0, 18, parent);
    tableau[0]->AddDropRules(3,
                             new ProhibitDropSameColor,
                             new DropOneDown,
                             new KingOnEmpty);
    tableau[0]->AddDragRules(1,
                             new DragFaceUpOnly);
    Card* Deck[52];
    for (int v = 0; v < 52; v++)
        Deck[v] = new Card(v, parent);
    Shuffle(Deck, 52);
    int n = 0;
    for (int row = 0; row < 7; row++)
        for (int column = row; column < 7; column++)
        {
            if (row == column)
                Deck[n]->Faceup(true);
            tableau[column]->acceptCard(Deck[n++], false, false, false);
        }
    while (n < 52)
        stock->acceptCard(Deck[n++], false, false, false);
}

Klondike::Klondike(QWidget *parent):
    Game(parent)
{

}

Klondike::~Klondike()
{

}

void Klondike::DealCard(Pile *p)
{
    if (p->Top())
    {
        int i = 0;
        while (i < dealNumber && p->Top())
        {
            Card *temp = p->Top();
            waste->acceptCard(p->Top(), false, i == 0);
            temp->Faceup(true);
            i++;
        }
        if (i > 1)
            new Move(i);
    } else
    {
        int k = 0;
        while (waste->Top())
        {
            k++;

            stock->acceptCard(waste->Top(), false);
            stock->Top()->Faceup(false);
        }
        if (k > 1)
            new Move(k);
    }
}

void Klondike::setDealNumber(int n)
{
    dealNumber = n;
}

void Klondike::PlayoffTableau(TreePile *p, Card *c)
{
    OneCardPlayoff(c);
}

