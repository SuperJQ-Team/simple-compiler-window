#include "matrixinput.h"
#include "ui_matrixinput.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QWindow>
#include <QStyleFactory>
#pragma execution_character_set("utf-8")

MatrixInput::MatrixInput(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::MatrixInput)
{
    ui->setupUi(this);
    this->setWindowTitle("矩阵输入");

    setAttribute(Qt::WA_TranslucentBackground);//窗体背景全透明
    ui->tableWidget->setFrameStyle(0);
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

    ui->tableWidget->setAttribute(Qt::WA_InputMethodEnabled, false);
//    QApplication::setStyle( QStyleFactory::create("Fusion"));
    int rows = ui->spinBox_Rows->value();
    int cols = ui->spinBox_Cols->value();
    ui->tableWidget->setColumnCount(cols);
    ui->tableWidget->setRowCount(rows);
}

MatrixInput::~MatrixInput()
{
    delete ui;
}

void MatrixInput::on_spinBox_Rows_valueChanged(int arg1)
{
    ui->tableWidget->setRowCount(arg1);	// 当spinBox的值发生改变时，直接设置为TableWidget的行数
}


void MatrixInput::on_spinBox_Cols_valueChanged(int arg1)
{
    ui->tableWidget->setColumnCount(arg1);// 当spinBox的值发生改变时，直接设置为TableWidget的列数
}


void MatrixInput::on_pushButton_clicked()
{
    // 获得当前TableWidget控件的行列
    int rows = ui->tableWidget->rowCount();
    int cols = ui->tableWidget->columnCount();
    bool nullbook = false;
    QString mat = "";
    if(rows > 0 && cols > 0) mat += "{";
    for(int i = 0; i < rows; i++){
        if(cols > 1) mat += "{";
        for(int j = 0;j < cols; j++){
            QTableWidgetItem* pItem = ui->tableWidget->item(i, j);
            QString text;
            if(pItem == nullptr){
                if(!nullbook){
                    QMessageBox::warning(this, "Warning", "空输入默认为0", QMessageBox::Ok);
                    nullbook = true;
                }
                text = "0";
            }
            else text = pItem->text();
            mat += text.trimmed();
            if(j < cols - 1) mat += ",";
            else if(cols > 1) mat += "}";
        }
        if(i < rows - 1) mat += ",";
        else mat += "}";
    }
    sendData(mat);
//    qDebug()<<mat<<Qt::endl;
    close();
}

void MatrixInput::on_pushButton_2_clicked()
{
    close();
}

void MatrixInput::on_pushButton_3_clicked()
{
    QMessageBox::StandardButton box;
    box = QMessageBox::question(this, "提示", "确实要清空吗?", QMessageBox::Yes|QMessageBox::No);
    if(box==QMessageBox::No) return;
    ui->tableWidget->clearContents();
}

void MatrixInput::paintEvent(QPaintEvent *event){
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
