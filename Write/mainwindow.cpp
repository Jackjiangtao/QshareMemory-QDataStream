#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QHash>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{    
    if(sharedMemory!=NULL)
    {
        if(sharedMemory->isAttached())
        {
            sharedMemory->detach();
        }
        delete sharedMemory;
    }
    delete ui;
}

void MainWindow::on_pushButton_Clear_clicked()
{
    if (!sharedReadMemory->attach())
    {
        ui->label->setText(sharedMemory->errorString());
        return;
    }

    QBuffer buffer;
    QDataStream in(&buffer);

    sharedReadMemory->lock();
    buffer.setData((char*)sharedReadMemory->constData(), sharedReadMemory->size());
    buffer.open(QBuffer::ReadOnly);

    int size = -1;
    in>>size;

     QHash<int ,Dog *> tmp;
    for(auto i =0;i<size;++i)
    {
        auto p =new Dog();

        in >> *p;
        tmp.insert(i,p);
    }


    QHash<int ,Data *> ReadData;
    in>>size;
    for(auto i =0;i<size;++i)
    {
        auto p =new Data(1);

        in >> *p;
        ReadData.insert(i,p);
    }
    sharedReadMemory->unlock();
    sharedReadMemory->detach();
}

void MainWindow::on_pushButton_Write_clicked()
{
    if (sharedMemory->isAttached())
    {
        if (!sharedMemory->detach())
        {
            ui->label->setText(sharedMemory->errorString());
            return;
        }
    }

    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);

    out<<infoDog.size();
    for(auto i =0;i<5;++i)
        out<<*infoDog[i];

    out<<infoData.size();
    for(auto i =0;i<1;++i)
        out<<*infoData[i];

    int size = buffer.size();
    if (!sharedMemory->create(size)) {
        ui->label->setText(sharedMemory->errorString());
        return;
    }
    sharedMemory->lock();
    char *to = (char*)sharedMemory->data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(sharedMemory->size(), size));
    sharedMemory->unlock();
    buffer.close();
}

void MainWindow::init()
{
    key="SharedMemory_Test";
    sharedMemory=new QSharedMemory(key);
    sharedReadMemory =new QSharedMemory(key);

    ui->comboBox->addItems(QStringList()<<"Text"<<"Image");
    ui->comboBox->setCurrentIndex(0);

    for(auto i =0;i<1;++i)
    {
        auto ptr =new Data();
        infoData.insert(i,ptr);
    }

    for(auto i =0;i<5;++i)
    {
       auto dog =new Dog(i, QString::number(i)+"momor");

       infoDog.insert(i,dog);
    }


}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->label->setVisible(false);
        ui->textEdit->setVisible(true);
        break;
    default:
        ui->label->setVisible(true);
        ui->textEdit->setVisible(false);
        break;
    }
    ui->label->clear();
    ui->textEdit->clear();
}


void MainWindow::read()
{

}
