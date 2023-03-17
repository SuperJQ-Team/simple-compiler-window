#include "mainwindow.h"
#include "matrixinput.h"
#include "aboutwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>
#include <QLibrary>
#include <QPainter>
#include <QWindow>
#include <QMimeData>
#pragma execution_character_set("utf-8")
typedef _stdcall const char* (*Fun)(const char*); //定义函数指针

Fun Preter;
Fun FilePreter;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("一个普通的计算器");
    this->setAcceptDrops(true);//设置主界面接受拖动进来的文件
    ui->textEdit->setAcceptDrops(false);

//    setWindowFlags(Qt::FramelessWindowHint);//无边框
//    QWin::enableBlurBehindWindow(this);
    setAttribute(Qt::WA_TranslucentBackground);//窗体背景全透明
    ui->textEdit->setFrameStyle(0);
    ui->textEdit->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
//    setWindowOpacity(0.7);
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
    /*//以下这段代码可以启动亚克力效果，但据说效率上有很大缺陷，不建议使用
    //Warning: Due to the API proplem, this effect is laggy when dragging | resizing
    if(huser){
        setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(huser, "SetWindowCompositionAttribute");
        if(setWindowCompositionAttribute){
            DWORD gradientColor = DWORD(0x50F5F5F5);
            ACCENT_POLICY accent = { ACCENT_ENABLE_ACRYLICBLURBEHIND, 0, gradientColor, 0 };
            WINDOWCOMPOSITIONATTRIBDATA data;
            data.Attrib = WCA_ACCENT_POLICY;
            data.pvData = &accent;
            data.cbData = sizeof(accent);
            setWindowCompositionAttribute(hwnd, &data);
        }
    }*/

    QLibrary mylib("Calculator-debug");
    if(mylib.load()){
        Preter = (Fun)mylib.resolve("LineInterpreter");
        if(!Preter) qDebug() << "LineInterpreter函数导入失败!";
        FilePreter = (Fun)mylib.resolve("FileOpen");
        if(!FilePreter) qDebug() << "FileOpen函数导入失败!";
//      qDebug() << Preter("1+1");
    }
    else qDebug() <<"dll寄!";

//    setCentralWidget(ui->textEdit); //将txtEdit设置为中心组件，自动填充整个工作区
    ui->textEdit->setFocus();
//    ui->textEdit->append("Hello World!");
    ui->textEdit->append(QString(QLatin1String(Preter("welcome()"))));
    ui->textEdit->append(">>>");
    ui->textEdit->installEventFilter(this);//设置完后自动调用其eventFilter函数
    //光标移动到末尾
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return || k->key() == Qt::Key_Enter)
             {
                 on_send_clicked();
                 return true;
             }
        }
    }
    return QMainWindow::eventFilter(target,event);
}

void MainWindow::on_send_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    QString ans = "";
    msg = msg.mid(msg.lastIndexOf('\n') + 4);
//    qDebug() << "msg = " + msg;
    //接口
    if(!msg.compare("exit", Qt::CaseInsensitive) || !msg.compare("exit()", Qt::CaseInsensitive)) close();
    else if(!msg.compare("clear", Qt::CaseInsensitive) || !msg.compare("clear()", Qt::CaseInsensitive)){
        ui->textEdit->clear();
    }
    else {
        std::string msgtemp = msg.toLocal8Bit().data();
        ans = QString::fromLocal8Bit(Preter(msgtemp.c_str()));
//      qDebug() << "ans = " + ans;
        if(!ans.isEmpty()) ui->textEdit->append(ans);
    }
    if(ans.compare("-",Qt::CaseInsensitive)) ui->textEdit->append("---");
    else ui->textEdit->append(">>>");
    //光标移动到末尾
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::on_actionjuzhen_triggered()
{
    MatrixInput *matrixinputwindow = new MatrixInput;
    connect(matrixinputwindow,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));
    matrixinputwindow->show();
}

void MainWindow::receiveData(QString data)
{
    ui->textEdit->insertPlainText(data);
}

void MainWindow::dragEnterEvent(QDragEnterEvent*event)  //将文件拖进来的事件
{
    //如果类型是cgl文件才能接受拖动。这里的compare字符串比较函数，相等的时候返回0，所以要取反
   if(!event->mimeData()->urls()[0].fileName().right(3).compare("cgl"))
       event->acceptProposedAction();
    else
       event->ignore();//否则不接受鼠标事件
}
void MainWindow::dropEvent(QDropEvent*event)    //放下事件
{
    //可能有多个文件,可以根据需要获取指定的路径
    QList<QUrl> urlsList = event->mimeData()->urls();
    if(urlsList.isEmpty())  return;
    //必须做这一步转换,不然路径无法识别
    QString textPath= urlsList.first().toLocalFile();
    qDebug() << textPath;
    std::string path = textPath.toLocal8Bit().data();
    ui->textEdit->append(QString::fromLocal8Bit(FilePreter(path.c_str())));
    ui->textEdit->append(">>>");
}

void MainWindow::paintEvent(QPaintEvent *event){
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

void MainWindow::on_actionclose_triggered()
{
    close();
}


void MainWindow::on_actionAbout_triggered()
{
    AboutWindow *aboutwindow = new AboutWindow;
    connect(aboutwindow,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));
    aboutwindow->show();
}

