#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QPainter>
#include <QWindow>
#pragma execution_character_set("utf-8")

AboutWindow::AboutWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("关于");

    setAttribute(Qt::WA_TranslucentBackground);//窗体背景全透明
    acryBackground = QColor(240, 240, 240, 150);
    acryOpacity = 50;
    hwnd = HWND(winId());
    huser = GetModuleHandle(L"user32.dll");
    //以下这段代码可以启动Aero效果
    if(huser){
        setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(huser, "SetWindowCompositionAttribute");
        if(setWindowCompositionAttribute){
            //DWORD gradientColor = DWORD(0x50FFFFFF);
            ACCENT_POLICY accent = { ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };
            WINDOWCOMPOSITIONATTRIBDATA data;
            data.Attrib = WCA_ACCENT_POLICY;
            data.pvData = &accent;
            data.cbData = sizeof(accent);
            setWindowCompositionAttribute(hwnd, &data);
        }
    }
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(QColor(0,0,0,255));  //前三个参数设定背景颜色的RGB值，最后一个参数设定颜色的透明度
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(acryBackground);
    painter.drawRect(this->rect());
}
