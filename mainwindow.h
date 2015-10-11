#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <game.h>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void newKlondike();
    void newSpider(difficulty_type h);
    void newFreeCell();
    void newPyramid();
    ~MainWindow();

private slots:
    void on_actionKlondike_triggered();

    void on_actionUndo_triggered();

    void on_actionWin_Game_triggered();

    void on_actionPlayoff_Cards_triggered();

    void on_actionPlay_3_triggered();

    void on_action_About_triggered();

    void on_actionEasy_triggered();

    void on_actionMedium_triggered();

    void on_actionHard_triggered();

    void on_actionFreecell_triggered();

    void on_actionExit_triggered();

    void on_PyramidDrawButton_clicked();

    void on_actionPyramid_triggered();

    void on_PyramidDealButton_clicked();

public slots:
    void on_actionRedeal_triggered();

private:
    Ui::MainWindow *ui;
    void Spider_triggered(difficulty_type h);
};

#endif // MAINWINDOW_H
