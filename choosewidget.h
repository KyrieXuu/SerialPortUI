#ifndef CHOOSEWIDGET_H
#define CHOOSEWIDGET_H

#include <QWidget>
#include <mainwindow.h>

namespace Ui {
class choosewidget;
}

class choosewidget : public QWidget
{
    Q_OBJECT

public:
    explicit choosewidget(QWidget *parent = nullptr);
    ~choosewidget();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::choosewidget *ui;
};

#endif // CHOOSEWIDGET_H
