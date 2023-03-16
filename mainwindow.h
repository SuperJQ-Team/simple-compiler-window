#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WindowCompositionAttribute.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器
private slots:
    void on_send_clicked();

//    void on_lineEdit_editingFinished();

//    void on_comboBox_activated(int index);

    void on_actionjuzhen_triggered();

    void receiveData(QString data);
    void on_actionclose_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *event);
    void dragEnterEvent(QDragEnterEvent*event);//拖动进入事件
    void dropEvent(QDropEvent*event);
    HWND hwnd;
    HMODULE huser;
    pfnSetWindowCompositionAttribute setWindowCompositionAttribute;
    QColor acryBackground;	//用来控制背景颜色
    int acryOpacity;	//用来控制透明度
};
#endif // MAINWINDOW_H
