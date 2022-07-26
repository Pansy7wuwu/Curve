#include "curvefont.h"
#include "qcustomplot.h"
#include "ui_curvefont.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QSettings>
#include <QTranslator>
#include <QTextStream>
#include <qcustomplot.h>
#include <QTabWidget>
#include <QVector>
QTranslator tran;

CurveFont::CurveFont(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CurveFont)
{
    ui->setupUi(this);

    tran.load(":/language/language_cn.qm");
    qApp->installTranslator(&tran);
    ui->retranslateUi(this);

    QFile file(":/qss/Menubar.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();

    ui->actionHistoryFile0->setVisible(false);
    ui->actionHistoryFile1->setVisible(false);
    ui->actionHistoryFile2->setVisible(false);
    ui->actionHistoryFile3->setVisible(false);
    ui->actionHistoryFile4->setVisible(false);
    ui->actionHistoryFile5->setVisible(false);
    ui->actionHistoryFile6->setVisible(false);
    ui->actionHistoryFile7->setVisible(false);
    ui->actionHistoryFile8->setVisible(false);
    ui->actionHistoryFile9->setVisible(false);

    QFile file_history("./historyFile.txt");
    file_history.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream s(&file_history);
    QString strs = s.readLine();
    while(!strs.isNull())
    {
        historyFile.push_back(strs);
        strs = s.readLine();
    }
    file_history.close();
    updateHistoryFile();
    data_reader.resize(20);
    currentPage.resize(20);
    Time.resize(20);
    Data.resize(20);
    min.resize(20);
    max.resize(20);
    color = new QColor[36];

    for(int i=0 ; i<36 ; i++)
    {
        QColor c(rand()%256,rand()%256,rand()%256);
        color[i] = c;
    }

    QCustomPlot* Page = new QCustomPlot();
    ui->tabWidget->addTab(Page,"Curve 1");
    numOfPage = 1;

    curveName = new QButtonGroup;
    btnColor = new QButtonGroup;
    int index = 0;
    curveName->addButton(ui->checkBox_1,index++);
    curveName->addButton(ui->checkBox_2,index++);
    curveName->addButton(ui->checkBox_3,index++);
    curveName->addButton(ui->checkBox_4,index++);
    curveName->addButton(ui->checkBox_5,index++);
    curveName->addButton(ui->checkBox_6,index++);
    curveName->addButton(ui->checkBox_7,index++);
    curveName->addButton(ui->checkBox_8,index++);
    curveName->addButton(ui->checkBox_9,index++);
    curveName->addButton(ui->checkBox_10,index++);
    curveName->addButton(ui->checkBox_11,index++);
    curveName->addButton(ui->checkBox_12,index++);
    curveName->addButton(ui->checkBox_13,index++);
    curveName->addButton(ui->checkBox_14,index++);
    curveName->addButton(ui->checkBox_15,index++);
    curveName->addButton(ui->checkBox_16,index++);
    curveName->addButton(ui->checkBox_17,index++);
    index = 0;
    btnColor->addButton(ui->color_1,index++);
    btnColor->addButton(ui->color_2,index++);
    btnColor->addButton(ui->color_3,index++);
    btnColor->addButton(ui->color_4,index++);
    btnColor->addButton(ui->color_5,index++);
    btnColor->addButton(ui->color_6,index++);
    btnColor->addButton(ui->color_7,index++);
    btnColor->addButton(ui->color_8,index++);
    btnColor->addButton(ui->color_9,index++);
    btnColor->addButton(ui->color_10,index++);
    btnColor->addButton(ui->color_11,index++);
    btnColor->addButton(ui->color_12,index++);
    btnColor->addButton(ui->color_13,index++);
    btnColor->addButton(ui->color_14,index++);
    btnColor->addButton(ui->color_15,index++);
    btnColor->addButton(ui->color_16,index++);
    btnColor->addButton(ui->color_17,index++);
    editData.resize(17);
    editTime.resize(17);
    editData[0] = ui->lineEdit_1;
    editData[1] = ui->lineEdit_2;
    editData[2] = ui->lineEdit_3;
    editData[3] = ui->lineEdit_4;
    editData[4] = ui->lineEdit_5;
    editData[5] = ui->lineEdit_6;
    editData[6] = ui->lineEdit_7;
    editData[7] = ui->lineEdit_8;
    editData[8] = ui->lineEdit_9;
    editData[9] = ui->lineEdit_10;
    editData[10] = ui->lineEdit_11;
    editData[11] = ui->lineEdit_12;
    editData[12] = ui->lineEdit_13;
    editData[13] = ui->lineEdit_14;
    editData[14] = ui->lineEdit_15;
    editData[15] = ui->lineEdit_16;
    editData[16] = ui->lineEdit_17;
    editTime[0] = ui->Time_1;
    editTime[1] = ui->Time_2;
    editTime[2] = ui->Time_3;
    editTime[3] = ui->Time_4;
    editTime[4] = ui->Time_5;
    editTime[5] = ui->Time_6;
    editTime[6] = ui->Time_7;
    editTime[7] = ui->Time_8;
    editTime[8] = ui->Time_9;
    editTime[9] = ui->Time_10;
    editTime[10] = ui->Time_11;
    editTime[11] = ui->Time_12;
    editTime[12] = ui->Time_13;
    editTime[13] = ui->Time_14;
    editTime[14] = ui->Time_15;
    editTime[15] = ui->Time_16;
    editTime[16] = ui->Time_17;
    curveName->setExclusive(false);
    for(int i = 0 ; i<17 ; i++)
    {
        qDebug()<<1;
        editTime[i]->setVisible(false);
        curveName->button(i)->setVisible(false);
//        curveName.button(i)->setChecked(false);
        editData[i]->setVisible(false);
        btnColor->button(i)->setVisible(false);
    }
//    ui->scrollAreaWidgetContents->setMinimumHeight(100);
}

CurveFont::~CurveFont()
{
    delete ui;
}


void CurveFont::on_actionExit_triggered()
{
    this->close();
}

void CurveFont::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this," 提示 "," 是否退出程序 ",
                                   QMessageBox::Yes|QMessageBox::No);
    if(button == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
    QFile f("./historyFile.txt");
    f.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Append);
    QTextStream s(&f);
    QQueue<QString> historyFileTemp;
    QString strs;
    while(!historyFile.isEmpty())
    {
        strs = historyFile.front();
        historyFile.pop_front();
        historyFileTemp.push_back(strs);
        s<<strs<<'\n';
    }
    f.close();
    historyFile = historyFileTemp;
    return;
}

void CurveFont::on_actionOpen_triggered()
{
    QFileDialog* fd = new QFileDialog(this);//创建打开文件对话框
    QString fileName = fd->getOpenFileName(this,tr("Open File"),"C:\\Users\\Administrator\\Desktop\\Data",
                                           tr("(*.kdt *.KDT *.DAT *.dat *.SDR *.sdr *.tdt *.TDT)"));
    if(fileName == "")
          return;
    QDir dir = QDir::current();
    QString path = dir.filePath(fileName);
    QString PATH="";
    for(int i=0;i<path.size();i++){
        if(path[i]=='/'){
            PATH+="\\";
        }
        else{
            PATH+=path[i];
        }
    }
    QString Path = PATH;
    Path = Path.toLower();
    int length = Path.length();
    length -= 3;
    QString strs= Path.mid(length);
    if(strs=="kdt" || strs=="dat" || strs=="sdr" || strs=="tdt")
    {
        if(historyFile.size()<10)
        {
            historyFile.push_back(PATH);
        }
        else
        {
            historyFile.pop_front();
            historyFile.push_back(PATH);
        }
    }
    updateHistoryFile();
    if(isFirst)
    {
        ui->tabWidget->removeTab(0);
        numOfPage=0;
        data_reader[0].setFilePath(PATH);
        data_reader[0].readToMap();
        data_reader[0].dealData();
        draw();
        tracer = new QCPItemTracer(currentPage[0]);
        tracer->setVisible(true);
        tracer->setPen(QPen(Qt::DashLine));
        tracer->setStyle(QCPItemTracer::tsCrosshair);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
    }
    else
    {
        int index = numOfPage-1;
        index++;
        data_reader[index].setFilePath(PATH);
        data_reader[index].readToMap();
        data_reader[index].dealData();
        draw();
    }
}


void CurveFont::on_actionChinese_triggered()
{
    tran.load(":/language/language_cn.qm");
    qApp->installTranslator(&tran);
    ui->retranslateUi(this);

    QFile file(":/qss/Menubar.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();
}


void CurveFont::on_actionEnglish_triggered()
{
    tran.load(":/language/language_en.qm");
    qApp->installTranslator(&tran);
    ui->retranslateUi(this);

    QFile file(":/qss/en_Menubar.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();
}


void CurveFont::on_actionTchinese_triggered()
{
    tran.load(":/language/language_tra_cn.qm");
    qApp->installTranslator(&tran);
    ui->retranslateUi(this);

    QFile file(":/qss/Menubar.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();
}

void CurveFont::updateHistoryFile()
{
    QQueue<QString> histroyFileTemp;
    QString strs;
    numOfHistoryFile = 0;
    while(!historyFile.isEmpty())
    {
        strs = historyFile.front();
        historyFile.pop_front();
        histroyFileTemp.push_back(strs);
        switch(numOfHistoryFile)
        {
        case 0:
        {
            if(ui->actionHistoryFile9->isVisible())
                ui->actionHistoryFile9->setText(strs);
            else
            {
                ui->actionHistoryFile9->setVisible(true);
                ui->actionHistoryFile9->setText(strs);
            }
            numOfHistoryFile++;
            break;
        }
        case 1:
        {
            if(ui->actionHistoryFile8->isVisible())
                ui->actionHistoryFile8->setText(strs);
            else
            {
                ui->actionHistoryFile8->setVisible(true);
                ui->actionHistoryFile8->setText(strs);
            }
            numOfHistoryFile++;
            break;
        }
        case 2:
        {
            if(ui->actionHistoryFile7->isVisible())
                ui->actionHistoryFile7->setText(strs);
            else
            {
                ui->actionHistoryFile7->setVisible(true);
                ui->actionHistoryFile7->setText(strs);
            }
            numOfHistoryFile++;
            break;
        }
        case 3:
        {
            if(ui->actionHistoryFile6->isVisible())
                ui->actionHistoryFile6->setText(strs);
            else
            {
                ui->actionHistoryFile6->setVisible(true);
                ui->actionHistoryFile6->setText(strs);
            }
            numOfHistoryFile++;
            break;
        }
        case 4:
        {
            if(ui->actionHistoryFile5->isVisible())
                ui->actionHistoryFile5->setText(strs);
            else
            {
                ui->actionHistoryFile5->setVisible(true);
                ui->actionHistoryFile5->setText(strs);
            }
            numOfHistoryFile++;
            break;
        }
        case 5:
        {
            if(ui->actionHistoryFile4->isVisible())
                ui->actionHistoryFile4->setText(strs);
            else
            {
                ui->actionHistoryFile4->setVisible(true);
                ui->actionHistoryFile4->setText(strs);
            }
            numOfHistoryFile++;
            break;
        }
        case 6:
        {
            if(ui->actionHistoryFile3->isVisible())
                ui->actionHistoryFile3->setText(strs);
            else
            {
                ui->actionHistoryFile3->setVisible(true);
                ui->actionHistoryFile3->setText(strs);
            }
            numOfHistoryFile++;
            break;
        }
        case 7:
        {
            if(ui->actionHistoryFile2->isVisible())
                ui->actionHistoryFile2->setText(strs);
            else
            {
                ui->actionHistoryFile2->setVisible(true);
                ui->actionHistoryFile2->setText(strs);
            }
            numOfHistoryFile++;
            break;
        }
        case 8:
        {
            if(ui->actionHistoryFile1->isVisible())
                ui->actionHistoryFile1->setText(strs);
            else
            {
                ui->actionHistoryFile1->setVisible(true);
                ui->actionHistoryFile1->setText(strs);
            }
            numOfHistoryFile++;
            break;
        }
        case 9:
        {
            if(ui->actionHistoryFile0->isVisible())
                ui->actionHistoryFile0->setText(strs);
            else
            {
                ui->actionHistoryFile0->setVisible(true);
                ui->actionHistoryFile0->setText(strs);
            }
            numOfHistoryFile++;
            break;
        }
        }
    }
    historyFile = histroyFileTemp;
}

void CurveFont::draw()
{
    QCustomPlot* Page = new QCustomPlot();
    numOfPage++;
    currentPage[numOfPage-1] = Page;
    QString CurveName = "Curve"+QString::number(numOfPage);
    ui->tabWidget->addTab(Page,CurveName);
//    if(isFirst)
//    {
//        numOfPage++;
//        currentPage[numOfPage-1] = Page;
//        QString CurveName = "Curve"+QString::number(numOfPage);
//        ui->tabWidget->addTab(Page,CurveName);
//    }
//    else
//    {
//        numOfPage++;
//        currentPage[numOfPage-1] = Page;
//        QString CurveName = "Curve"+QString::number(numOfPage);
//        ui->tabWidget->addTab(Page,CurveName);
//    }
    switch (data_reader[numOfPage-1].fileType) {
    case 0:
    {
        Page->yAxis->setLabel("Temperature(°C)");
        Page->addGraph(Page->xAxis,Page->yAxis);
        for(int i=1 ; i<data_reader[numOfPage-1].numOfDataGroup ; i++)
        {
            if(data_reader[numOfPage-1].Track_Info[i]=="Humidity(RH%)")
            {
                Page->yAxis2->setVisible(true);
                Page->yAxis2->setLabel("Humidity(RH%)");
                Page->yAxis2->setRange(0,100);
                QList<QCPAxis*> axes;
                axes << Page->yAxis2 << Page->xAxis2 << Page->yAxis << Page->xAxis;
                Page->axisRect()->setRangeZoomAxes(axes);
                Page->axisRect()->setRangeDragAxes(axes);
                Page->addGraph(Page->xAxis,Page->yAxis2);
            }
            else
                Page->addGraph(Page->xAxis,Page->yAxis);
        }
        Data[numOfPage-1].resize(data_reader[numOfPage-1].numOfDataGroup);
        int size = data_reader[numOfPage-1].Processed_Data.size()/data_reader[numOfPage-1].numOfDataGroup;
        Time[numOfPage-1].resize(size);
        qDebug()<<data_reader[numOfPage-1].Processed_Data.size()<<data_reader[numOfPage-1].numOfDataGroup<<size<<data_reader[numOfPage-1].Processed_Time.size();
        for(int i=0 ; i<data_reader[numOfPage-1].numOfDataGroup ; i++)
        {
            Data[numOfPage-1][i].resize(size);
        }
        for(int i=0 ; i<data_reader[numOfPage-1].Processed_Time.size() ; i++)
        {
            Time[numOfPage-1][i] = data_reader[numOfPage-1].Processed_Time[i];
        }
        int INDEX = 0;
        int _INDEX = 0;
        min[numOfPage-1]=data_reader[numOfPage-1].Processed_Data[0];
        max[numOfPage-1]=data_reader[numOfPage-1].Processed_Data[0];
        for(int i=0 ; i<data_reader[numOfPage-1].Processed_Data.size() ; i++)
        {
            if(data_reader[numOfPage-1].Processed_Data[i]>max[numOfPage-1])
                max[numOfPage-1] = data_reader[numOfPage-1].Processed_Data[i];
            if(data_reader[numOfPage-1].Processed_Data[i]<min[numOfPage-1])
                min[numOfPage-1] = data_reader[numOfPage-1].Processed_Data[i];
            Data[numOfPage-1][INDEX][_INDEX] = data_reader[numOfPage-1].Processed_Data[i];
            INDEX++;
            if(INDEX==data_reader[numOfPage-1].numOfDataGroup)
            {
                _INDEX++;
                INDEX=0;
            }
        }
        for(int i=0 ; i<data_reader[numOfPage-1].numOfDataGroup ; i++)
        {
            Page->graph(i)->setPen(color[i]);
            Page->graph(i)->addData(Time[numOfPage-1],Data[numOfPage-1][i]);
            Page->graph(i)->rescaleAxes();
        }
//        tracer = new QCPItemTracer(Page);
//        tracer->setVisible(true);
//        tracer->setPen(QPen(Qt::DashLine));
//        tracer->setStyle(QCPItemTracer::tsCrosshair);
        Page->xAxis->setRange(0,Time[numOfPage-1][size-1]);
        Page->yAxis->setRange(min[numOfPage-1],max[numOfPage-1]);
        Page->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectPlottables);
        Page->replot();
        ui->tabWidget->setCurrentIndex(numOfPage-1);
        if(isFirst)
            isFirst=false;
        break;
    }
    default:
        break;
    }
}

void CurveFont::mousemove(QMouseEvent* e)
{
    int cnt = ui->tabWidget->currentIndex();
    double x = currentPage[cnt]->xAxis->pixelToCoord(e->pos().x());
//    tracer->setGraph(currentPage[cnt]->graph(0));
//    tracer->setGraphKey(x);
    if(!isFirst)
    {
        tracer->setGraph(currentPage[cnt]->graph(0));
        tracer->setGraphKey(x);
        tracer->setInterpolating(true);
        tracer->updatePosition();
    }
    currentPage[cnt]->replot();
}

void CurveFont::on_tabWidget_currentChanged(int index)
{
    if(index>=0&&!isFirst)
    {
        tracer->setVisible(false);
        tracer = new QCPItemTracer(currentPage[index]);
        tracer->setPen(QPen(Qt::DashLine));
        tracer->setStyle(QCPItemTracer::tsCrosshair);
        ui->tabWidget->setCurrentIndex(index);
        connect(currentPage[index],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[index]->replot();
    }
}

