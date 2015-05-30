#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include "guessnumber.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent = 0);
    void showErrorMsg(QString msg);
    ~Dialog();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();
    void on_pushButton_4_clicked();

private:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);

    QPoint dragPosition;

public:
    Ui::Dialog *ui;
    GuessNumber *gn;


};

#endif // DIALOG_H
