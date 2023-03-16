#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QWidget>
#include "WindowCompositionAttribute.h"

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();

private:
    Ui::AboutWindow *ui;
    void paintEvent(QPaintEvent *event);
    HWND hwnd;
    HMODULE huser;
    pfnSetWindowCompositionAttribute setWindowCompositionAttribute;
    QColor acryBackground;	//用来控制背景颜色
    int acryOpacity;	//用来控制透明度
};

#endif // ABOUTWINDOW_H
