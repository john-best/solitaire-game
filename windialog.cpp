#include "windialog.h"
#include "ui_windialog.h"
#include "game.h"
#include "klondike.h"
#include "mainwindow.h"
extern MainWindow *mainwindow;

WinDialog::WinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::windialog)
{
    ui->setupUi(this);
}

WinDialog::~WinDialog()
{
    delete ui;
}

void WinDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton b = ui->buttonBox->standardButton(button);

    switch (b) {
    case QDialogButtonBox::Ok:
        break;
    case QDialogButtonBox::Close:
        QApplication::quit();
        break;
    case QDialogButtonBox::Retry:
        mainwindow->on_actionRedeal_triggered();
        break;
    }
}
