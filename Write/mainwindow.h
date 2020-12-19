#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include<QHash>
#include<QMap>
#include<QString>
#include<QVector>

namespace Ui {
class MainWindow;
}

class Data
{
public:
    Data()
    {
        tmpBool =true;
        tmpInt =rand()%100;

        for(auto i =0;i<1;++i)
            tmpHash[rand()%1000] =rand()%100;

        for(auto i =0;i<3;++i)
            tmpMap[rand()%100] =rand()%100;

        for(auto i =0;i<5;++i)
            tmpvector.push_back(QString::number(rand()%100));
    }

    Data(int k)
    {
        tmpInt =100;
    }

public:
    bool tmpBool;
    int tmpInt;
    QHash<int ,int> tmpHash;
    QMap<int ,int> tmpMap;
    QVector<QString> tmpvector;
};


inline QDataStream &operator<<(QDataStream &out, const Data & tmp)
{
    out<<tmp.tmpBool;
    out<<tmp.tmpInt;

    auto size =tmp.tmpHash.size();
    out<< size;
    for(auto it=tmp.tmpHash.begin();it!= tmp.tmpHash.end();++it)
    {
         auto key =it.key();
         auto value =it.value();

         out<<key<<value;
    }

    size =tmp.tmpMap.size();
    out<< size;
    for(auto it=tmp.tmpMap.begin();it!= tmp.tmpMap.end();++it)
    {
         auto key =it.key();
         auto value =it.value();

         out<<key<<value;
    }

     size =tmp.tmpvector.size();
    out<< size;

    for(auto it=tmp.tmpvector.begin();it!= tmp.tmpvector.end();++it)
    {
         auto value =*it;
         out<<value;
    }

    return  out;
}

inline QDataStream &operator>>(QDataStream &in, Data &tmp)
{
    in>>tmp.tmpBool;
    in>>tmp.tmpInt;


    int size =-1;
    in>> size;

   for(auto i =0;i<size;++i)
   {
       int key =-1;
       int value =-1;

       in>> key>>value;
       tmp.tmpHash.insert(key,value);
  }

    in>> size;
   for(auto i =0;i<size;++i)
    {
        int key =-1;
        int value =-1;

       in>> key>>value;
        tmp.tmpMap.insert(key,value);
   }

    in>> size;

   for(auto i =0;i<size;++i)
    {
       QString value;
       in>>value;
        tmp.tmpvector.push_back(value);
   }

    return in;
}


//inline QDataStream &operator<<(QDataStream &out, const  QHash<int ,Data *> & tmp)
//{

//    int datasize =tmp.size();
//    out<<datasize;

//    for(auto i =0;i<datasize;++i)
//    {
//       auto key =i;
//       auto value =tmp[i];

//        out<<key;
//        out<<*value;
//    }
//    return  out;
//}

//inline QDataStream &operator>>(QDataStream &in,  QHash<int ,Data *> &tmp)
//{
//    in.setVersion(QDataStream::Qt_4_3); int datasize =-1;
//    in>>datasize;

//    for(auto i =0;i<datasize;++i)
//    {
//        int key;
//        Data *valu = new Data();

//       in<<key;
//       in<<*valu;
//        tmp.insert(key,valu);

//    }
//    return in;
//}

class Dog
{
public:
 Dog() { _number = 0; }

 Dog(int number, const QString &name) {
  _number = number;
  _name = name;
  age =rand()%20;
  sex = rand()%2;
 }

 void setNumber(int number) { _number = number; }
    int number() const { return _number; }

 void setName(const QString &name) { _name = name; }
    QString name() const { return _name; }

  public:

 int _number;
 QString _name;

 int age;
 bool sex ;

};

inline QDataStream &operator<<(QDataStream &out, const Dog &dog) {
    out << dog.number() << dog.name()<<dog.age<<dog.sex;
    return out;

}

inline QDataStream &operator>>(QDataStream &in, Dog &dog) {
 int number = 0;
 QString name;
 int age =0;
 bool sex =false;
 in >> number >> name>>dog.age>>dog.sex;

 dog.setNumber(number);
 dog.setName(name);

 return in;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_Clear_clicked();
    void on_pushButton_Write_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void read();
private:
    Ui::MainWindow *ui;
    QSharedMemory *sharedMemory;
    QSharedMemory *sharedReadMemory;
    QString key;

    QHash<int ,Data *> infoData;

    QHash<int ,Dog *> infoDog;

    void init();
};

#endif // MAINWINDOW_H
