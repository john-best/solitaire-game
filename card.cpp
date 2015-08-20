#include "card.h"
#include <QtGlobal>
#include <QMouseEvent>
#include "game.h"
#include "pile.h"
#include <cassert>
#include <QDebug>

QImage Card::faces[53];
bool Card::initialized = false;
QPoint Card::startDragPos;
QPoint Card::mouseDownOffset;

extern Game *game;


void Card::Initialize()
{
    if (initialized) return;
    initialized = true;
    char suit[]={'c','d','h','s'};
    char pip[] = {'1','2','3','4','5','6','7','8','9','a','b','c','d'};
    QString fileName = ":/cards/c1.bmp";
    int n = 0;

    for (int s = CLUBS; s <= SPADES; s++)
    {
        fileName[8] = suit[s];
        for (int p = ACE; p <= KING; p++)
        {
            fileName[9] = pip[p];
            faces[n++] = QImage(fileName);
        }
    }

    faces[n] = QImage(":/cards/zCardBack.bmp");

}

void Card::Faceup(bool makeFaceUp)
{
    faceup = makeFaceUp;
    setPixmap(QPixmap::fromImage(faces[faceup?value:52]));
}

bool Card::canBeDragged()
{
    return pile->checkDragRules(this);
}


void Card::mousePressEvent(QMouseEvent *event)
{
    switch (event->buttons())
    {
    case Qt::LeftButton:
        okToMove = pile->checkDragRules(this);
        if (okToMove)
        {
            startDragPos = pos();
        }
        mouseDownOffset = pos() - event->globalPos();

        break;
        //    case Qt::RightButton:
        //    {
        //        Card *temp = this;
        //            if (temp->faceup && !temp->rcmoved)
        //            {
        //                temp->move(x(),y()+10);
        //                temp->rcmoved = true;
        //            }
        //            else if (temp->faceup && temp->rcmoved)
        //            {.
        //                temp->move(x(),y()-10);
        //                temp->rcmoved = false;
        //            }
        //        break;
        //    }

    default:;
    }
}

void Card::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (moving)
        {
            TreePile *p = game->FindBestPile(this);
            if (p)
                p->acceptCard(this);

            else
                pile->acceptCard(this, false, false, false);
            okToMove = false;
            moving = false;
        }
        else
        {
            if ((mouseDownOffset - (pos() - event->globalPos())).manhattanLength() < 4 )
                CardClicked();
        }
    }
}

void Card::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && okToMove) {
        Card *temp = NULL; // remember to set pointers to NULL or your game will crash
        QPoint point = event->globalPos() + mouseDownOffset;
        QPoint moved = point - pos();
        if (moved.manhattanLength() > 4)
            moving = pile->checkDragRules(this);
        if (moving)
        {
            this->move(point);
            this->raise();
            if (this->over)
                temp = this->over;
            while (temp) {
                point += myPile()->Delta();
                temp->raise();
                temp->move(point);
                temp = temp->over;
            }
        }
    }
}

void Card::CardClicked()
{
    pile->Clicked(this);
}

uint Card::getMDistance(QPoint p)
{
    return (pos() - p).manhattanLength();
}

Card::Card(int c, QWidget *parent) :
    QLabel(parent),value(c),over(0),under(0),pile(0),moving(false)
{
    Faceup(false);
    pip = static_cast<pips>(value % SUIT_SIZE);
    suit = static_cast<suits>(value / SUIT_SIZE);
    cardColor = static_cast<c_colors>((suit>1)^(suit&1));
}

Card::~Card()
{

}

void Shuffle(Card* Deck[], int n)
{
    Card* temp;
    int a;
    while (n)
    {
        a = qrand() % n--;
        temp = Deck[a];
        Deck[a] = Deck[n];
        Deck[n] = temp;
        temp->raise();
    }
}


TreeCard::TreeCard(int c, QWidget *parent, int pos): Card(c, parent), rover(0), runder(0),
    position(pos)
{
    Faceup(true);
}

int TreeCard::RightUnder()
{
    int ru[29] = {0,
                  0,
                 1, 0,
                2, 3, 0,
               4, 5, 6, 0,
              7, 8, 9,10, 0,
            11,12,13,14,15,0,
           16,17,18,19,20,21,0};

    return ru[position];

}

int TreeCard::LeftUnder()
{

    int lu[29] = {0,
                  0,
                 0, 1,
                0, 2, 3,
               0, 4, 5, 6,
              0, 7, 8, 9,10,
             0,11,12,13,14,15,
            0,16,17,18,19,20,21};

    return lu[position];
}

void TreeCard::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (moving)
        {
            if (!game->FindBestCard(this))
            {
                pile->acceptCard(this, false, false, false);
                okToMove = false;
                moving = false;
            }
        }
        else
        {
            if ((mouseDownOffset - (pos() - event->globalPos())).manhattanLength() < 4 )
                CardClicked();
        }
    }

}

