#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <parser.h>
#include <scanner.h>
#include <QStandardItemModel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QDialog>
#include <QGraphicsSimpleTextItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void IterateOverTree(Node* Current);



    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rect;
    QGraphicsTextItem *textTitle;
    QGraphicsTextItem *textDetails;

    int TempX;
    int TempY;

    int W = 80;
    int H =50;

    int X;
    int Y;
};

#endif // MAINWINDOW_H
