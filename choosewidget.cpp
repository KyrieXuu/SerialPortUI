#include "choosewidget.h"
#include "ui_choosewidget.h"

choosewidget::choosewidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::choosewidget)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);//最大化
}

choosewidget::~choosewidget()
{
    delete ui;
}

void choosewidget::on_pushButton_2_clicked()
{
    this->close();
    MainWindow *setting = new MainWindow;
    setting->show();
}

