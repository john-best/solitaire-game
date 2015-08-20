#ifndef MOVE_H
#define MOVE_H
#include <QStack>

class Card;
class Pile;
class TreePile;

class Move
{
    Card *card;
    TreePile *pile;
    bool underFaceup;
    bool faceup;
    int repeat;

    static QStack<Move*> history;
    void undo();


public:
    static void Undo();
    static void NewGame();
    Move(Card *c, TreePile *p);
    Move(int n);
    ~Move();
};

#endif // MOVE_H
