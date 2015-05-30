#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QBitmap>
#include <QPainter>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QFile file(":/qss/style/style.qss");
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    file.close();

    ui->setupUi(this);
    ui->lineEdit->setText("./prices.txt");
    on_radioButton_2_clicked();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_2_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("请选择文本文件"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter("文本文件 (*.txt)");
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        ui->lineEdit->setText(path);
    }


}

void Dialog::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    if(ui->radioButton->isChecked())
    {
        try
        {
            gn->readFromFile(ui->lineEdit->text());
        }
        catch (QString e)
        {
            showErrorMsg(e);
            return;
        }
        accept();
    }
    else
    {
        try
        {
            gn->readFromTaobao(ui->spinBox->text().toInt());
        }
        catch (QString e)
        {
            showErrorMsg(e);
            return;
        }
        gn->d = this;
    }

}
void Dialog::showErrorMsg(QString msg)
{
    ui->progressBar->setValue(0);
    ui->pushButton->setEnabled(true);
    QMessageBox::about(this,"错误",msg);
}

void Dialog::on_radioButton_clicked()
{
    ui->frame->setVisible(true);
    ui->frame_2->setVisible(false);
}

void Dialog::on_radioButton_2_clicked()
{
    ui->frame->setVisible(false);
    ui->frame_2->setVisible(true);
}

void Dialog::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void Dialog::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}
void Dialog::paintEvent(QPaintEvent *event)
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

void Dialog::on_pushButton_4_clicked()
{
    this->showMinimized();
}

