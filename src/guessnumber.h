#ifndef GUESSNUMBER_H
#define GUESSNUMBER_H
#include <QString>
#include <vector>
#include <QObject>
#include <QJsonObject>
#include <QJsonParseError>
#include <QtNetwork>
#include <QFile>
#include <QTextStream>
class Dialog;
struct Goods
{
    QString name,imgPath;
    int price = 0;
    int finalGuessedPrice;
    bool isFinished = false;
};
class GuessNumber:public QObject
{
    Q_OBJECT
public:
    GuessNumber(QObject *parent = 0);
    ~GuessNumber();
    void readFromFile(QString fileName);
    void readFromTaobao(int num);
    int checkAnswer(int index,int answer);
    int getRightCount();
    bool allFinished();
    int goodsNum;
    std::vector<Goods> goods;
    Dialog *d;
private:
    int currentImageNum;
    QNetworkReply * reply;
    QNetworkAccessManager * manager;
    QFile * f;

public slots:
    void replyFinished(QNetworkReply *reply);
    void replyImageFinished(QNetworkReply *reply);
    void downloadProgress(qint64 a, qint64 b);

};

#endif // GUESSNUMBER_H
