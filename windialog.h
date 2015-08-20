#ifndef WINDIALOG_H
#define WINDIALOG_H

#include <QDialog>
#include <QAbstractButton>
namespace Ui {
class windialog;
}

class WinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WinDialog(QWidget *parent = 0);
    ~WinDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::windialog *ui;
};

#endif // WINDIALOG_H
