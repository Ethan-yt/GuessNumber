#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "guessnumber.h"
#include <QStandardItemModel>
#include <QMainWindow>
#include <QMouseEvent>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_4_clicked();

private:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);
    QPoint  dragPosition;

    Ui::MainWindow *ui;
    GuessNumber gn;
    int currentNum;
    int times = 0;//已经猜的次数
    int max,min;
    QStandardItemModel  *pricesModel,*goodsModel;
    void backToGoodsPage();
    bool isGameover = false;
};

#endif // MAINWINDOW_H
