#include "game.h"
#include "pile.h"
#include "card.h"
#include <algorithm>
#include "move.h"
#include <QDialog>
#include <QtMultimedia/QSound>
#include <QAbstractButton>
#include "windialog.h"
#include "rule.h"
#include "mainwindow.h"

extern WinDialog *winbox;
extern MainWindow *mw;

bool Game::OneCardPlayoff(Card *c)
{
    if (c->Over())
        return false;
    TreePile *p = NULL;
    QListIterator<TreePile*> i(piles);
    while (i.hasNext())
    {
        p = i.next();
        if (p->Type() == FOUNDATION)
            if (p->checkDropRules(c))
            {
                p->acceptCard(c,c->myPile()->Type() != STOCK);
                c->Faceup(true);
                if (c->Pip() == KING)
                    CheckWin();
                return true;
            }
    }
    return false;
}

void Game::DrawStock()
{
    TreePile *p = NULL;
    QListIterator<TreePile*> i(piles);
    while (i.hasNext())
    {
        p = i.next();
        if (p->Type() == STOCK)
        {

        }
//            DealCard(p);
}
}

void Game::CheckWin()
{
    TreePile *p = NULL;
    QListIterator<TreePile*> i(piles);
    while (i.hasNext())
    {
        p = i.next();
        if (p->Type() == FOUNDATION)
            continue;
        if (p->Top()) return;
    }
    Move::NewGame();
    winbox->show();

    //    QDialog WinBox(parent,Qt::Popup | Qt::MSWindowsFixedSizeDialogHint);
    //    WinBox.setFixedSize(300, 150);
    //    WinBox.setWindowTitle("you did it");
    //QSound::play("cards/winner.wav");

}

void Game::Clear()
{
    TreePile *p = NULL;
    while (!piles.isEmpty())
    {
        p = piles.takeFirst();
        delete p;
    }
    Rule::deleteRules();
    Move::NewGame();
}

void Game::AddPile(TreePile *p)
{
    piles.append(p);
}

void Game::PlayoffAll()
{
    bool foundCard;
    QListIterator<TreePile*> i(piles);
    TreePile *p = NULL;
    do {
        foundCard = false;
        while (i.hasNext())
        {
            bool samePile = true;
            p = i.next();
            while (p->Type() != FOUNDATION && p->Top() && samePile)
                foundCard |= samePile =  OneCardPlayoff(p->Top());
        }
        i.toFront();
    } while (foundCard);
}



TreePile* Game::FindBestPile(Card *c)
{
    const int PILES = 3; // # of piles nearby dropped point to check
    QListIterator<TreePile*> i(piles);
    TreePile *p = NULL;
    TreePile *potentialPiles[PILES] = {NULL};
    uint pileDistance[PILES];
    uint temp;
    int k = 0;

    for (int j = 0; j < PILES; j++)
        pileDistance[j] = 999999;
    while (i.hasNext())
    {
        p = i.next();

        temp = p->getMDistance(c->pos());
        if (p != c->myPile())
        {
            k = 0;
            while (k < PILES)
            {
                if (temp < pileDistance[k])
                {
                    std::swap(temp, pileDistance[k]);
                    std::swap(p, potentialPiles[k]);
                }
                k++;
            }
        }
    }
    k = 0;
    while (k < PILES) {
        if (potentialPiles[k]->checkDropRules(c))
        {
            TreePile *p2 = potentialPiles[k];
            return p2->Type() == FOUNDATION?FoundationTarget(p2, c):p2;
        }
        k++;
    }
    return NULL;
}

Game::Game(QWidget *p): parent(p)
{

}

Game::~Game()
{
    Clear();
}
