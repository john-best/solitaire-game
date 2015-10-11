#include "pyramid.h"
#include "game.h"
#include "rule.h"
#include "move.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "card.h"
#include "pile.h"
#include "windialog.h"
#include <QMessageBox>
extern WinDialog *winbox;
extern Game *game;
extern Ui::MainWindow* myui;
Pyramid::Pyramid(QWidget *parent) : Game(parent), redrawLimit(2), redealLimit(2)
{

}

Pyramid::~Pyramid()
{    myui->PyramidDealButton->hide();
     myui->PyramidDrawButton->hide();
}

game_types Pyramid::Type()
{
    return PYRAMID_GAME;
}

void Pyramid::Redeal(QWidget *w)
{
    Pyramid* pgame = dynamic_cast<Pyramid*>(game);
    int deals = 2;
    if (game && pgame)
    {
        deals = --redealLimit;
        if (deals == -1) {
            //TODO: clear scores
            deals = 2;
            QMessageBox GameOver;
            GameOver.setText("Game over! How disappointing.");
            GameOver.exec();

        }
    }
    if (game) {
        game->Clear();
        delete game;
        game = NULL;
    }
    game = new Pyramid(w);
    game->newDeal();
    redealLimit = deals;
    myui->PyramidDealLabel->setText(QString("Deals left: " + QString::number(redealLimit)));
}

void Pyramid::RedealLimited(QWidget *w)
{
    if (!redealLimit)
    {
        myui->PyramidDealButton->hide();
    }
    else {
        Redeal(parent);       
    }
}

void Pyramid::newDeal(difficulty_type h)
{
    myui->PyramidDrawButton->show();
    stock = new StockPile(249, 420, 0, 0, parent);
    stock->AddDropRules(1,
                        new TCDropSum13);
    stock->AddDragRules(1,
                        new TCDragTop);

    waste = new WastePile(373, 420, 0, 0, parent);
    waste->AddDropRules(1,
                        new TCDropSum13);
    waste->AddDragRules(1,
                        new TCDragTop);


    tableau = new PyramidPile(311, 10, 50, 32, parent);
    tableau->AddDropRules(1,
                          new TCDropSum13);
    tableau->AddDragRules(1,
                          new TCDragTop);

    foundation = new FoundationPile(1000,0,0,10,parent);
    foundation->AddDropRules(1,
                             new ProhibitDrop);
    foundation->AddDragRules(1,
                             new ProhibitDrag);

    for (int v = 0; v < 52; v++)
        Deck[v] = new TreeCard(v, parent);
    Shuffle((Card**)Deck, 52);

    int n = 0;
    while (n < 28)
    {
        Deck[n]->Position(n+1);
        tableau->acceptCard(Deck[n++], true);
    }
    while (n < 52)
        stock->acceptCard(Deck[n++]);

}

void Pyramid::DealCard(Pile *p)
{
    if (stock->Top())
    {
        waste->acceptCard(stock->Top(), false, true);
    } else {
        if (!redrawLimit)
        {
            myui->PyramidDrawButton->hide();
            if (redealLimit)
                myui->PyramidDealButton->show();
            else {
                //TODO: lose game
            }
            return;
        }
        redrawLimit--;
        int k = 0;
        while (waste->Top())
        {
            k++;
            stock->acceptCard(waste->Top(), false);
//            stock->Top()->Faceup(true);
        }
        if (k > 1)
            new Move(k);
    }

}

bool Pyramid::FindBestCard(TreeCard *c)
{
    const int CARDS = 3;
    TreeCard *potentialCards[CARDS] = {(TreeCard*)stock->Top(),
                                       (TreeCard*)waste->Top(),NULL};
    uint cardDistance[CARDS] = {stock->Top()?c->getMDistance(stock->Top()->pos()):99999,
                               waste->Top()?c->getMDistance(waste->Top()->pos()):99999, 99999};
    uint temp;
    int k = 0;

    for (int i = 0; i < 29; i++)
    {
        TreeCard *tempc = tableau->FindPos(i);


        if (tempc && tempc != c && !tempc->Over() && !tempc->ROver())
        {
            temp = tempc->getMDistance(c->pos());
            k = 0;
            while (k < CARDS)
            {
                if (temp < cardDistance[k])
                {
                    std::swap(temp, cardDistance[k]);
                    std::swap(tempc, potentialCards[k]);
                }
                k++;
            }
        }
    }
    k = 0;
    while (k < CARDS) {
        if (potentialCards[k]->myPile()->checkDropRules(potentialCards[k],c))
        {
            foundation->acceptCard(potentialCards[k]);
            foundation->acceptCard(c);
            new Move(2);
            return true;
        }
        k++;
    }
    return false;
}

bool Pyramid::OneCardPlayoff(Card *c)
{
    if (c->Pip() == KING)
    {
        foundation->acceptCard(c);
        return true;
    }
    return false;
}

void Pyramid::CheckWin()
{
    if(!tableau->Bottom())
    {
        Move::NewGame();
        winbox->show();
    }
}

void Pyramid::Clear()
{
    for (int i = 0; i < 52; i++)
       delete Deck[i];
    waste->NullTop();
    stock->NullTop();
    foundation->NullTop();
    tableau->Bottom(NULL);
    TreePile *p = NULL;
    while (!piles.isEmpty())
    {
        p = piles.takeFirst();
        delete p;
    }
    Rule::deleteRules();
    Move::NewGame();
}
