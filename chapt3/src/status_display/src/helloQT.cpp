#include <QApllication.hpp>
#include <QLabel>
#include <QString>

int main(int argc,char** argv[])
{
    QApplication app();
    QLable* label=new QLabel();
    QString message=QString::fromStdString("HelloQt");
    label
    return 0;
}