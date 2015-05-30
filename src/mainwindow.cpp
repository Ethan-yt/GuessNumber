#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QPainter>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QFile file(":/qss/style/style.qss");
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    file.close();

    ui->setupUi(this);

    Dialog d;
    d.gn = &gn;
    if(d.exec() != QDialog::Accepted)
        exit(0);
    //ui->label->setText(QString::number(gn.goodsNum,10));
    goodsModel = new QStandardItemModel();

    goodsModel->setColumnCount(4);
    goodsModel->setHeaderData(0,Qt::Horizontal,"商品");
    goodsModel->setHeaderData(1,Qt::Horizontal,"价格");
    goodsModel->setHeaderData(2,Qt::Horizontal,"次数");
    goodsModel->setHeaderData(3,Qt::Horizontal,"你猜的价格");
    ui->tableView->setModel(goodsModel);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableView->setColumnWidth(0,150);
    ui->tableView->setColumnWidth(1,50);
    ui->tableView->setColumnWidth(2,50);
    ui->tableView->setColumnWidth(3,75);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    for(int i = 0; i < gn.goodsNum; i++)
    {
        goodsModel->setItem(i,0,new QStandardItem(gn.goods[i].name));
        goodsModel->setItem(i,1,new QStandardItem("你猜"));
        goodsModel->setItem(i,2,new QStandardItem("--"));
        goodsModel->setItem(i,3,new QStandardItem("--"));
    }

    ui->frame->setVisible(false);
    ui->label_hint->setVisible(false);
    ui->horizontalSlider->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(isGameover)
    {
        QMessageBox::information(this,"还想再猜？","游戏已经结束了");
        return;
    }
    currentNum = index.row();
    times = 0;
    if(gn.goods[currentNum].isFinished)
    {
        QMessageBox::information(this,"还想再猜？","你已经猜过了");
        return;
    }
    max=2147483647;
    min=0;
    ui->label_chance->setText("还有10次机会");
    ui->frame->setVisible(true);

    pricesModel = new QStandardItemModel();

    pricesModel->setColumnCount(2);
    pricesModel->setHeaderData(0,Qt::Horizontal,"你猜的价格");
    pricesModel->setHeaderData(1,Qt::Horizontal,"结果");

    ui->tableView_2->setModel(pricesModel);
    ui->tableView_2->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableView_2->setColumnWidth(0,150);
    ui->tableView_2->setColumnWidth(1,100);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑

}



void MainWindow::on_pushButton_clicked()
{
    int gp = ui->lineEdit->text().toInt();//猜的价格
    if(ui->lineEdit->text() == "")//检测非法输入
    {
        QMessageBox::information(this,"提示","请输入你猜的价格");
        return;
    }
    if(gp<=0)
    {
        QMessageBox::information(this,"提示","你输错了吧！商品价格只能是正整数");
        return;
    }

    ui->label_chance->setText("还有"+QString::number(9-times)+"次机会");

    gn.goods[currentNum].finalGuessedPrice=gp;
    pricesModel->setItem(times,0,new QStandardItem(QString::number(gp)));
    ui->lineEdit->clear();

    switch(gn.checkAnswer(currentNum,gp))
    {
    case 1:
        pricesModel->setItem(times,1,new QStandardItem("高了"));
        if(gp<max)
            max = gp;
        break;
    case -1:
        pricesModel->setItem(times,1,new QStandardItem("低了"));
        if(gp>min)
            min = gp;
        break;
    case 0:
        QMessageBox::information(this,"提示","恭喜你猜对了！答案是"+QString::number(gp)+"\r\n你太厉害了仅仅用了"+QString::number(times+1)+"次");
        QString timesStr;
        if(times == 9)
            timesStr = "10";
        else
            timesStr = "0"+QString::number(times+1);
        goodsModel->setItem(currentNum,2,new QStandardItem(timesStr));
        goodsModel->setItem(currentNum,1,new QStandardItem(QString::number(gp)));
        backToGoodsPage();
        return;
    }

    if(max == 2147483647)
        ui->label_hint->setText("提示：价格大于"+QString::number(min));
    else if(min == 0)
        ui->label_hint->setText("提示：价格小于"+QString::number(max));
    else
    {
        ui->label_hint->setText("提示：价格在"+QString::number(min)+"~"+QString::number(max)+"之间");
        ui->horizontalSlider->setMinimum(min);
        ui->horizontalSlider->setMaximum(max);
        ui->horizontalSlider->setVisible(true);
    }
    ui->label_hint->setVisible(true);
    if(times == 9)
    {
        QMessageBox::information(this,"很遗憾","十次机会已经用完了");
        goodsModel->setItem(currentNum,1,new QStandardItem(QString::number(gn.goods[currentNum].price)));
        goodsModel->setItem(currentNum,2,new QStandardItem("∞"));
        backToGoodsPage();
        return;
    }
    times++;
}

inline void MainWindow::backToGoodsPage()
{
    ui->label_hint->setVisible(false);
    ui->horizontalSlider->setVisible(false);
    ui->frame->setVisible(false);
    ui->lineEdit->clear();
    delete pricesModel;
    gn.goods[currentNum].isFinished = true;
    goodsModel->setItem(currentNum,3,new QStandardItem(QString::number(gn.goods[currentNum].finalGuessedPrice)));
    goodsModel->item(currentNum,0)->setForeground(QBrush(QColor(195, 195, 195)));
    goodsModel->item(currentNum,1)->setForeground(QBrush(QColor(195, 195, 195)));
    goodsModel->item(currentNum,2)->setForeground(QBrush(QColor(195, 195, 195)));
    goodsModel->item(currentNum,3)->setForeground(QBrush(QColor(195, 195, 195)));
    if(gn.allFinished())
    {
        isGameover = true;
        QMessageBox::information(this,"提示","游戏结束!您一共猜对"+QString::number(gn.getRightCount())+"个商品价格");
        goodsModel->sort(2);
        for(int i = 0;i < gn.goodsNum;i++)
        {
            goodsModel->item(i,0)->setForeground(QBrush(QColor(0, 0, 0)));
            goodsModel->item(i,1)->setForeground(QBrush(QColor(0, 0, 0)));
            goodsModel->item(i,2)->setForeground(QBrush(QColor(0, 0, 0)));
            goodsModel->item(i,3)->setForeground(QBrush(QColor(0, 0, 0)));
        }
    }
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    QPixmap pixmap(gn.goods[index.row()].imgPath);
    QPixmap fitpixmap=pixmap.scaled(200,200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_Pic->setPixmap(fitpixmap);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lineEdit->setText(QString::number(value));
}
void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}
void MainWindow::paintEvent(QPaintEvent *event)
{

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, this->width()-20, this->height()-20);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for(int i=0; i<10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    this->showMinimized();
}
