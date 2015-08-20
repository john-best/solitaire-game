#include "spider.h"
#include "pile.h"
#include <QWidget>
#include "card.h"
#include <QTime>
#include "game.h"
#include "rule.h"
#include "move.h"



extern Game *game;

const int CARD_SPACING = 5;

game_types Spider::Type()
{
    return SPIDER;
}

void Spider::Redeal(QWidget *w)
{
    difficulty_type h = EASY;

    if (game)
    {
        h = difficulty;
        game->Clear();
        delete game;
        game = NULL;
    }
    game = new Spider(w);
    game->newDeal(h);
}

void Spider::newDeal(difficulty_type h)
{
    for (int n = 0; n < 5; n++)
        stock[n] = new StockPile(705 - (10 + ((n)*12)), 10+400, 0,0,parent);
    stock[0]->AddDropRules(1,
                           new ProhibitDrop);
    stock[0]->AddDragRules(1,
                           new ProhibitDrag);

    for (int n = 0; n < 8; n++)
        foundation[7-n] = new FoundationPile(10 + ((7-n)*12), 10 + 400, 0, 0, parent);
    foundation[0]->AddDropRules(1,
                                new DropWholeSuit);
    foundation[0]->AddDragRules(1,
                                new ProhibitDrag);

    for (int n = 0; n < 10; n++)
        tableau[n] = new TableauPile(10 + (n) * (CARD_WIDTH + CARD_SPACING), 10, 0, 18, parent);
    tableau[0]->AddDropRules(1,
                             new DropOneDown);
    tableau[0]->AddDragRules(3,
                             new DragFaceUpOnly,
                             new DragStackDecreasing,
                             new DragStackSameSuit);

    Card* Deck[104];
    if (h != REPEAT)
        difficulty = h;
    int min_value = 52-(SUIT_SIZE<<(difficulty-1));
    int value = min_value;
    for (int v = 0; v < 104; v++)
    {
        Deck[v] = new Card(value++, parent);
        if (value == 52) value = min_value;
    }
    Shuffle(Deck, 104);

    int n = 0;

    for (int stocks = 0; stocks < 5; stocks++)
        for (int cards = 0; cards < 10; cards ++)
        {
            stock[stocks]->acceptCard(Deck[n++], false, true, false);
        }
    while (n < 104)
    {
        int column = 0;
        while (column < 10 && n < 104)
        {
            tableau[column]->acceptCard(Deck[n++], false, false, false);
            column++;
        }
    }
    for (int i = 0; i < 10; i++)
        tableau[i]->Top()->Faceup(true);

}

void Spider::DealCard(Pile *p)
{

    int pile = 4;
    while (pile >= 0 && !stock[pile]->Top())
    {
        pile--;
    }
    if (pile < 0) return;
    int column = 0;
    while (column < 10 && stock[pile]->Top())
    {
        tableau[column]->acceptCard(stock[pile]->Top());
        tableau[column++]->Top()->Faceup(true);
    }
    new Move(10);
}


void Spider::PlayoffTableau(TreePile *p, Card *c)
{
    //    int cardPip = 0;
    //    int cardPipActual = 0;
    if (!c)
        return;
    Card *temp = c;
    int v = c->Pip() == ACE?0:c->Value()-1;
    int i = 0;
    while (i < 8 && v != (foundation[i]->Top()?foundation[i]->Top()->Value():0))
        i++;
    if (i == 8) return;

    if(foundation[i]->DropRule(0)->EnforceRule(c,foundation[i]))
    {
        int k = 0;
        while (!foundation[i]->Top() || foundation[i]->Top()->Pip() != KING)
        {
            k++;
            temp = c->Under();
            foundation[i]->acceptCard(c,true,true,true);
            c = temp;
        }
        if (k > 1) new Move(k);
    }


    //    while (cardPip < 12) {
    //        cardPip++;
    //        cardPipActual++;
    //        if (temp->Pip() == temp->Under()->Pip()-1
    //                && temp->Suit() == temp->Under()->Suit())
    //        {
    //            temp = temp->Under();
    //        }
    //        else break;
    //    }

    //    if (cardPipActual == 11)
    //    {
    //        for (int i = 0; i > 12; i++)
    //        {
    //            c->Under();
    //            foundation[i]->acceptCard(c,false,true,true);
    //        }

    //    }

}

void Spider::PlayoffAll()
{
    for (int i = 0; i < 10; i++)
    {
        if (tableau[i]->Top()) PlayoffTableau(tableau[i], tableau[i]->Top());
    }

}

Pile *Spider::FoundationTarget(Pile *p, Card *c)
{
    int v = c->Pip() == ACE?0:c->Value()-1;
    int i = 0;
    while (i < 8 && v != (foundation[i]->Top()?foundation[i]->Top()->Value():0))
        i++;
    if (i == 8) return p;
    return foundation[i];
}

Spider::Spider(QWidget *parent):
    Game(parent)
{

}

Spider::~Spider()
{

}

