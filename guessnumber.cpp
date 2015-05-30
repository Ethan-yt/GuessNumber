#include "guessnumber.h"
#include "dialog.h"
#include "ui_dialog.h"
using namespace std;
GuessNumber::GuessNumber(QObject *parent):QObject(parent)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void GuessNumber::readFromFile(QString fileName)
{
    goods.clear();
    QFile *file=new QFile(fileName);
    if(!file->open(QIODevice::ReadOnly|QIODevice::Text))
        throw QString("文件无法打开");
    QTextStream fin(file);
    fin>>goodsNum;
    for(int i = 0;i<goodsNum;i++)
    {
        Goods newGoods;
        fin>>newGoods.name>>newGoods.price>>newGoods.imgPath;
        if(newGoods.name.isEmpty()|| newGoods.price == 0 || newGoods.imgPath.isEmpty())
            throw QString("文件内容有问题");
        goods.push_back(newGoods);
    }
}
void GuessNumber::readFromTaobao(int num)
{
    goodsNum = num;
    goods.clear();

    QNetworkRequest request(QUrl("http://tui.taobao.com/recommend?count="+QString::number(num)+"&appid=406"));
    reply = manager->get(request);

}

int GuessNumber::checkAnswer(int index,int answer)
{
    if(answer>goods[index].price)
        return 1;
    else if(answer==goods[index].price)
        return 0;
    else return -1;
}
bool GuessNumber::allFinished()
{
    for(auto& g : goods)
        if(!g.isFinished)
        {
            return false;
        }
    return true;
}
int GuessNumber::getRightCount(){
    int count = 0;
    for(auto& g : goods)
        if(g.finalGuessedPrice == g.price)
            count++;
    return count;
}

void GuessNumber::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError json_error;
        QByteArray re = reply->readAll();
        reply->deleteLater();
        re = re.mid(7,re.length()-9);
        QTextCodec *codec=QTextCodec::codecForName("GBK");
        QJsonDocument jsonDocument = QJsonDocument::fromJson(codec->toUnicode(re).toUtf8(), &json_error);
        if(json_error.error == QJsonParseError::NoError)
        {
            if(jsonDocument.isObject())
            {
                QVariantMap result = jsonDocument.toVariant().toMap();
                foreach (QVariant l, result["result"].toList()){

                    QVariantMap m = l.toMap();
                    Goods newGoods;
                    newGoods.name = m["title"].toString();
                    newGoods.price = m["promotionPrice"].toInt();
                    newGoods.imgPath=m["pict_url"].toString();

                    if(newGoods.name.isEmpty()|| newGoods.price == 0 || newGoods.imgPath.isEmpty())
                    {
                        d->showErrorMsg("读取数据遇到问题");
                        return;
                    }
                    goods.push_back(newGoods);
                }
                currentImageNum = -1;
                disconnect(manager, SIGNAL(finished(QNetworkReply*)),
                           this, SLOT(replyFinished(QNetworkReply*)));
                connect(manager, SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(replyImageFinished(QNetworkReply*)));

                QDir *temp = new QDir;
                if(!temp->exists("./tmp"))
                    temp->mkdir("./tmp");

                replyImageFinished(nullptr);

            }
            else
                d->showErrorMsg("读取数据遇到问题");
        }
        else
            d->showErrorMsg("读取数据遇到问题");
    }
    else
        d->showErrorMsg(reply->errorString());

}

void GuessNumber::replyImageFinished(QNetworkReply *reply)
{
    if(currentImageNum != -1)
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            QString filePath = "./tmp/"+QString::number(currentImageNum)+".jpg";
            goods[currentImageNum].imgPath = filePath;
            f = new QFile(filePath);
            f->open(QIODevice::WriteOnly);
            f->write(reply->readAll());
            f->flush();
            f->close();
            reply->deleteLater();
            if(currentImageNum == goodsNum-1)
            {
                d->accept();
                return;
            }
        }
        else
        {
            d->showErrorMsg(reply->errorString());
            return;
        }

    }

    currentImageNum++;
    reply = manager->get(QNetworkRequest(QUrl("http://gtms02.alicdn.com/tps/"+goods[currentImageNum].imgPath)));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
}

void GuessNumber::downloadProgress(qint64 a,qint64 b)
{
    d->ui->progressBar->setValue(((double)a/b + currentImageNum)*1000/goodsNum);
}

GuessNumber::~GuessNumber()
{
    QDir dir("./tmp/");
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        dir.remove(fi.fileName());
    }
    dir.cdUp();
    dir.rmpath("./tmp/");
    manager->deleteLater();
}
