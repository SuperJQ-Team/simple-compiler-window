#ifndef MATRIXINPUT_H
#define MATRIXINPUT_H

#include <QDockWidget>
#include "WindowCompositionAttribute.h"

namespace Ui {
class MatrixInput;
}

class MatrixInput : public QDockWidget
{
    Q_OBJECT

public:
    explicit MatrixInput(QWidget *parent = nullptr);
    ~MatrixInput();

private slots:
    void on_spinBox_Rows_valueChanged(int arg1);

    void on_spinBox_Cols_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

signals:
   void sendData(QString);

private:
    Ui::MatrixInput *ui;
    void paintEvent(QPaintEvent *event);
    HWND hwnd;
    HMODULE huser;
    pfnSetWindowCompositionAttribute setWindowCompositionAttribute;
    QColor acryBackground;	//用来控制背景颜色
    int acryOpacity;	//用来控制透明度
};

#endif // MATRIXINPUT_H
