#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "card.h"
#include <QtGlobal>
#include "game.h"
#include "klondike.h"
#include <QTime>
#include "move.h"
#include "helpdialog.h"
#include "spider.h"
#include "freecell.h"
#include "pile.h"
#include "pyramid.h"

Game *game = NULL;
Ui::MainWindow* myui = NULL;
extern HelpDialog *helpbox;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    myui = ui;
    QTime time;
    time.start();
    ui->setupUi(this);
    Card::Initialize();
    qsrand(time.msecsSinceStartOfDay());
    ui->PyramidDealButton->hide();
    ui->PyramidDrawButton->hide();

}

void MainWindow::newKlondike()
{
    if (game)
    {
        game->Clear();
        delete game;
        game = NULL;
    }

    game = new Klondike(ui->centralWidget);
    game->newDeal();
}

void MainWindow::newSpider(difficulty_type h)
{
    if (game)
    {
        game->Clear();
        delete game;
        game = NULL;
    }
    game = new Spider(ui->centralWidget);
    game->newDeal(h);
}

void MainWindow::newFreeCell()
{
    if (game)
    {
        game->Clear();
        delete game;
        game = NULL;
    }

    game = new FreeCell(ui->centralWidget);
    game->newDeal();
}

void MainWindow::newPyramid()
{
    if (game)
    {
        game->Clear();
        delete game;
        game = NULL;
    }
    ui->PyramidDealButton->hide();
    ui->PyramidDrawButton->show();
    game = new Pyramid(ui->centralWidget);
    game->newDeal();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionKlondike_triggered()
{
    setMinimumSize(546,600);
    setMaximumSize(546,600);
    resize(546,600);
    newKlondike();
    ui->actionPlayoff_Cards->setEnabled(true);
    ui->actionRedeal->setEnabled(true);
    setWindowTitle("Klondike");
}

void MainWindow::on_actionUndo_triggered()
{
    Move::Undo();
}

void MainWindow::on_actionWin_Game_triggered()
{
    game->CheckWin();
}

void MainWindow::on_actionPlayoff_Cards_triggered()
{
    game->PlayoffAll();
}

void MainWindow::on_actionPlay_3_triggered()
{
    game->setDealNumber(ui->actionPlay_3->isChecked()?3:1);
}

void MainWindow::on_action_About_triggered()
{
    helpbox->show();
}

void MainWindow::on_actionRedeal_triggered()
{
    if (game) game->Redeal(ui->centralWidget);
}

void MainWindow::Spider_triggered(difficulty_type h)
{
    setMinimumSize(775,600);
    setMaximumSize(775,600);
    resize(775,600);
    newSpider(h);
    ui->actionPlayoff_Cards->setEnabled(true);
    ui->actionRedeal->setEnabled(true);
    setWindowTitle("Spider");

}

void MainWindow::on_actionEasy_triggered()
{
    Spider_triggered(EASY);
}


void MainWindow::on_actionMedium_triggered()
{
    Spider_triggered(MEDIUM);
}

void MainWindow::on_actionHard_triggered()
{
    Spider_triggered(HARD);
}

void MainWindow::on_actionFreecell_triggered()
{
    setMinimumSize(693,600);
    setMaximumSize(693,600);
    resize(693,600);
    newFreeCell();
    ui->actionPlayoff_Cards->setEnabled(true);
    ui->actionRedeal->setEnabled(true);
    setWindowTitle("FreeCell");
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}


void MainWindow::on_PyramidDrawButton_clicked()
{
    Pyramid* temp =  dynamic_cast<Pyramid*>(game);
    if(temp) temp->DealCard(NULL);
}

void MainWindow::on_actionPyramid_triggered()
{
    setMinimumSize(693,600);
    setMaximumSize(693,600);
    resize(693,600);
    newPyramid();
    ui->actionPlayoff_Cards->setEnabled(true);
    ui->actionRedeal->setEnabled(true);
    setWindowTitle("Pyramid");
}
