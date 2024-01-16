#ifndef CHOOSEWIDGET_H
#define CHOOSEWIDGET_H

#include <QWidget>
#include "ui_choosewidget.h"
class MainWindow;


namespace Ui {
class choosewidget;
}

class choosewidget : public QWidget
{
    Q_OBJECT

public:
    explicit choosewidget(QWidget *parent = nullptr);
    ~choosewidget();

    void setStart(bool able);

private slots:
    void on_pushButton_2_clicked();

    void on_startButton_clicked();

private:
    Ui::choosewidget *ui;

    MainWindow *mWindow;


};

#endif // CHOOSEWIDGET_H
