#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <string>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    X = 0;
    Y = 0;

     // scene->addLine(50,60,120,80,line); //((w/2+x) , (h+y) , (x+w/2), (y)) if child
     //if next (x+w), (h/2) , (x) , (h/2)

}

void MainWindow::IterateOverTree(Node *Current)
{
    //colors

    QBrush cyanBrush(Qt::cyan);
    QBrush magentaBrush(Qt::magenta);
    QPen   black(Qt::black);
    QPen   line(Qt::darkYellow);
    black.setWidth(2);
    line.setWidth(3);

    //texts
    QString QTitle  = QString::fromStdString(Current->Title);
    QString QDetails  = QString::fromStdString(Current->Details);
    /*if((Current->Left == NULL) && (Current->Right == NULL) && (Current->ElsePart == NULL) && (Current->NextTo.size() == 0) )
    {
       scene->addLine((W/2+X),(H+Y),(TempX+W/2),TempY,line);
    }*/
    int bedayaX = X+40;
    int bedayaY= Y+50;
    if(Current->Type == Current->Box)
    {
        scene->addRect(X, Y,80,50,black,magentaBrush);
        scene->addText(QTitle, QFont())->setPos(X+10,Y+10);
        scene->addText(QDetails, QFont())->setPos(X+10,Y+25);
    }
    if(Current->Type == Current->Circle)
    {
        scene->addEllipse(X,Y,80,50,black,cyanBrush);
        scene->addText(QTitle, QFont())->setPos(X+10,Y+10);
        scene->addText(QDetails, QFont())->setPos(X+10,Y+25);
    }

    if (Current->Left != NULL)
    {
        scene->addLine(bedayaX,bedayaY,X+40,Y+100,line);
        Y+=100;
        TempY = Y;
        IterateOverTree(Current->Left);
        Y-=100;
    }
    if (Current->Right != NULL)
    {
        scene->addLine(bedayaX,bedayaY,X+140,Y+100,line);
        X+=100;
        Y+=100;
        TempX= X;
        TempY =Y;
        IterateOverTree(Current->Right);
        Y-=100;
    }
    if (Current->ElsePart != NULL)
    {
        scene->addLine(bedayaX,bedayaY,X+240,Y+100,line);
        X+=100;
        Y+=100;
        TempX= X;
        TempY =Y;
        IterateOverTree(Current->ElsePart);
        Y-=100;
    }
    if (Current->NextTo.size() != 0)
    {
        list<Node *> L = Current->NextTo;
        for (list<Node *>::iterator it = L.begin(); it != L.end(); ++it)
        {
            QPen   lin(Qt::magenta);
            scene->addLine(bedayaX+40,bedayaY-25,X+100,Y+25,lin);
            X+=100;
            TempX = X;
            IterateOverTree(*it);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    QString program = ui->textEdit->toPlainText();
    //qDebug() << program;

    //convert from Qstring to string
    string programinput = program.toLocal8Bit().constData();

    Scanner S;
    S.DealWithCode(programinput);

    string inputToParser = S.parserinput;

    //QString QinputToParser  = QString::fromStdString(inputToParser);
    //qDebug() << QinputToParser;

    Node* Tree = new Node();
    Parser P;
    P.cutting_parsing_input(inputToParser);
    Tree = P.prog();

    IterateOverTree(Tree); // tyb swani

}
