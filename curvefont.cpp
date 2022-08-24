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
#include <QRegularExpression>
QTranslator tran;

CurveFont::CurveFont(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CurveFont)
{
    ui->setupUi(this);

    //    this->setWindowTitle(tr("Temak曲线软件 v1.0"));
    //    this->setWindowIcon(QIcon(":/Image/Icon/Temak.png"));
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
    NOW.resize(20);
    TimeHMS.resize(20);
    TDT_TrackUpdate.resize(20);
    SDR_TrackUpdate.resize(20);
    color = new QColor[36];

    for(int i=0 ; i<36 ; i++)
    {
        QColor c(rand()%256,rand()%256,rand()%256);
        color[i] = c;
    }
    color[1] = Qt::red;
    color[0] = Qt::green;
    color[3] = Qt::blue;
    color[2] = Qt::yellow;
    color[4] = Qt::cyan;

    QCustomPlot* Page = new QCustomPlot();
    ui->tabWidget->addTab(Page,"Curve 1");
    numOfPage = 1;

    curveName = new QButtonGroup;
    btnColor = new QButtonGroup;
    rBtnTimeStyle = new QButtonGroup;
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
    rBtnTimeStyle->addButton(ui->hms_radiobutton,0);
    rBtnTimeStyle->addButton(ui->yMdhms_radiobutton,1);
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
    for(int i=0 ; i<17 ; i++)
    {
        btnColor->button(i)->setStyleSheet("border:none");
        QPalette pal = btnColor->button(i)->palette();
        pal.setColor(QPalette::Button,color[i]);
        btnColor->button(i)->setPalette(pal);
        btnColor->button(i)->setAutoFillBackground(true);
    }
    for(int i=0 ; i<17 ; i++)
    {
        //        qDebug()<<1;
        editTime[i]->setVisible(false);
        curveName->button(i)->setVisible(false);
        //        curveName.button(i)->setChecked(false);
        editData[i]->setVisible(false);
        btnColor->button(i)->setVisible(false);
    }
    rBtnTimeStyle->button(1)->setChecked(true);
    ui->scrollAreaWidgetContents->setMinimumHeight(100);

    //    ui->centralwidget->setStyleSheet("QWidget#centralwidget{background-color:rgb(127,127,127)}");
    //    ui->All->setStyleSheet("QWidget#All{background-color:rgb(195,195,195)}");
    //    ui->Curve_Info->setStyleSheet("QTabWidget#Curve_Info{border:none}");
    //    ui->Settings->setStyleSheet("QWidget#Settings{background-color:rgb(195,195,195)}");
    //    Page->axisRect()->setBackground(Qt::black);
    //    Page->setStyleSheet("QCustomPlot#Page{background-color:rgb(195,195,195)}");
    //    ui->tabWidget->setStyleSheet("background-color:rgb(127,127,127)");
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
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    ui->statusbar->addWidget(label);
    QFileDialog* fd = new QFileDialog(this);//创建打开文件对话框
    QString fileName = fd->getOpenFileName(this,tr("Open File"),"C:\\Users\\Administrator\\Desktop\\Data",
                                           tr("(*.kdt *.KDT *.DAT *.dat *.SDR *.sdr *.tdt *.TDT)"));
    if(fileName == "")
    {
        label->setText("");
        return;
    }
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        //        tracerLabel->position->setParentAnchor(0);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
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

    if(!isFirst)
    {
        int cnt = ui->tabWidget->currentIndex();
        this->hideTrack();
        this->showTrack(cnt);
    }
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
    if(!isFirst)
    {
        int cnt = ui->tabWidget->currentIndex();
        this->hideTrack();
        this->showTrack(cnt);
    }
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
    if(!isFirst)
    {
        int cnt = ui->tabWidget->currentIndex();
        this->hideTrack();
        this->showTrack(cnt);
    }
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
    QString CurveName = data_reader[numOfPage-1].getFileName();
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
        //        qDebug()<<data_reader[numOfPage-1].Processed_Data.size()<<data_reader[numOfPage-1].numOfDataGroup<<size<<data_reader[numOfPage-1].Processed_Time.size();
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

        QDateTime now;
        QDate nowDate(data_reader[numOfPage-1].startTime[0],data_reader[numOfPage-1].startTime[0],data_reader[numOfPage-1].startTime[0]);
        QTime nowTime(data_reader[numOfPage-1].startTime[3],data_reader[numOfPage-1].startTime[4],data_reader[numOfPage-1].startTime[5]);
        now.setDate(nowDate);
        now.setTime(nowTime);
        double NOW = now.toSecsSinceEpoch();
        //        qDebug()<<NOW;
        QSharedPointer<QCPAxisTickerDateTime>  dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("yyyy-mm-dd hh:mm:ss");
        Page->xAxis->setTicker(dateTicker);
        Page->xAxis->setRange(NOW,NOW+Time[numOfPage-1][size-1]);
        Page->yAxis->setRange(min[numOfPage-1],max[numOfPage-1]);
        Page->xAxis->ticker()->setTickCount(8);
        Page->yAxis->ticker()->setTickCount(10);
        Page->selectionRect()->setPen(QPen(Qt::black,1,Qt::DashLine));//设置选框的样式：虚线
        Page->selectionRect()->setBrush(QBrush(QColor(0,0,100,50)));//设置选框的样式：半透明浅蓝
        Page->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
        Page->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        Page->replot();
        ui->tabWidget->setCurrentIndex(numOfPage-1);
        if(isFirst)
            isFirst=false;
        break;
    }
    case 1:
    {
        Page->yAxis->setLabel("Temperature(°C)");
        Page->yAxis2->setVisible(true);
        Page->yAxis2->setLabel("Humidity(RH%)");
        Page->addGraph(Page->xAxis,Page->yAxis);
        Page->addGraph(Page->xAxis,Page->yAxis);
        Page->addGraph(Page->xAxis,Page->yAxis2);
        Page->addGraph(Page->xAxis,Page->yAxis2);
        QList<QCPAxis*> axes;
        axes << Page->yAxis2 << Page->xAxis2 << Page->yAxis << Page->xAxis;
        Page->axisRect()->setRangeZoomAxes(axes);
        Page->axisRect()->setRangeDragAxes(axes);
        Data[numOfPage-1].resize(data_reader[numOfPage-1].numOfDataGroup);
        int size = data_reader[numOfPage-1].Processed_Data.size()/data_reader[numOfPage-1].numOfDataGroup;
        Time[numOfPage-1].resize(size);
        TimeHMS[numOfPage-1].resize(size);
        QDateTime now;
        QDate nowDate(data_reader[numOfPage-1].startTime[0],data_reader[numOfPage-1].startTime[1],data_reader[numOfPage-1].startTime[2]);
        QTime nowTime(data_reader[numOfPage-1].startTime[3],data_reader[numOfPage-1].startTime[4],data_reader[numOfPage-1].startTime[5]);
        now.setDate(nowDate);
        now.setTime(nowTime);
        NOW[numOfPage-1] = now.toSecsSinceEpoch();
        //        qDebug()<<data_reader[numOfPage-1].Processed_Data.size()<<data_reader[numOfPage-1].numOfDataGroup<<size<<data_reader[numOfPage-1].Processed_Time.size();
        for(int i=0 ; i<data_reader[numOfPage-1].numOfDataGroup ; i++)
        {
            Data[numOfPage-1][i].resize(size);
        }
        for(int i=0 ; i<data_reader[numOfPage-1].Processed_Data.size()/4 ; i++)
        {
            Time[numOfPage-1][i] =NOW[numOfPage-1] + data_reader[numOfPage-1].dataFrequency*i;
            TimeHMS[numOfPage-1][i] = data_reader[numOfPage-1].dataFrequency*i;
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
        //        qDebug()<<Time[numOfPage-1][0]<<Time[numOfPage-1][1]<<Data[numOfPage-1][2][0]<<Data[numOfPage-1][2][1];
        for(int i=0 ; i<data_reader[numOfPage-1].numOfDataGroup ; i++)
        {
            Page->graph(i)->setPen(color[i]);
            Page->graph(i)->addData(Time[numOfPage-1],Data[numOfPage-1][i]);
            //            if(i==0||i==1)
            //                Page->graph(i)->rescaleAxes();
            //            else
            //                Page->yAxis2->rescale(true);
        }
        //        tracer = new QCPItemTracer(Page);
        //        tracer->setVisible(true);
        //        tracer->setPen(QPen(Qt::DashLine));
        //        tracer->setStyle(QCPItemTracer::tsCrosshair);
        QSharedPointer<QCPAxisTickerDateTime>  dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("yyyy-MM-dd\nhh:mm:ss");
        Page->xAxis->setTicker(dateTicker);
        Page->xAxis->setRange(Time[numOfPage-1][0],Time[numOfPage-1][size-1]);
        min[numOfPage-1]=data_reader[numOfPage-1].minTemperature/data_reader[numOfPage-1].sdrTemperatureMultiple;
        max[numOfPage-1]=data_reader[numOfPage-1].maxTemperature/data_reader[numOfPage-1].sdrTemperatureMultiple;
        Page->yAxis->setRange(min[numOfPage-1],max[numOfPage-1]);
        Page->yAxis2->setRange(0,100);
        Page->selectionRect()->setPen(QPen(Qt::black,1,Qt::DashLine));//设置选框的样式：虚线
        Page->selectionRect()->setBrush(QBrush(QColor(0,0,100,50)));//设置选框的样式：半透明浅蓝
        Page->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
        Page->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        QString name__ = data_reader[numOfPage-1].getFileName();
        QCPTextElement* title;
        Page->plotLayout()->insertRow(0);
        title = new QCPTextElement(Page,name__);
        Page->plotLayout()->addElement(0,0,title);
        Page->replot();
        SDRupdateTrack(numOfPage-1);
        ui->tabWidget->setCurrentIndex(numOfPage-1);
        if(isFirst)
            isFirst=false;
        break;
    }
    case 2:
    {
        Page->yAxis->setLabel("Temperature(°C)");
        if(data_reader[numOfPage-1].numOfYAxis==2)
        {
            Page->yAxis2->setVisible(true);
            Page->yAxis2->setLabel(data_reader[numOfPage-1].YAxis_Info[1]);
            Page->addGraph();
            Page->addGraph(Page->xAxis,Page->yAxis2);
            QList<QCPAxis*> axes;
            axes << Page->yAxis2 << Page->xAxis2 << Page->yAxis << Page->xAxis;
            Page->axisRect()->setRangeZoomAxes(axes);
            Page->axisRect()->setRangeDragAxes(axes);
        }
        else
        {
            Page->addGraph();
        }
        Data[numOfPage-1].resize(data_reader[numOfPage-1].numOfDataGroup);
        int size = data_reader[numOfPage-1].Processed_Data.size()/data_reader[numOfPage-1].numOfDataGroup;
        Time[numOfPage-1].resize(size);
        //        qDebug()<<data_reader[numOfPage-1].Processed_Data.size()<<data_reader[numOfPage-1].numOfDataGroup<<size<<data_reader[numOfPage-1].Processed_Time.size();
        for(int i=0 ; i<data_reader[numOfPage-1].numOfDataGroup ; i++)
        {
            Data[numOfPage-1][i].resize(size);
        }
        for(int i=0 ; i<data_reader[numOfPage-1].Processed_Data.size()/data_reader[numOfPage-1].numOfDataGroup ; i++)
        {
            Time[numOfPage-1][i] = data_reader[numOfPage-1].dataFrequency*i;
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
        //        qDebug()<<Time[numOfPage-1][0]<<Time[numOfPage-1][1]<<Data[numOfPage-1][2][0]<<Data[numOfPage-1][2][1];
        for(int i=0 ; i<data_reader[numOfPage-1].numOfDataGroup ; i++)
        {
            Page->graph(i)->setPen(color[i]);
            Page->graph(i)->addData(Time[numOfPage-1],Data[numOfPage-1][i]);
            //            if(i==0||i==1)
            //                Page->graph(i)->rescaleAxes();
            //            else
            //                Page->yAxis2->rescale(true);
        }
        //        tracer = new QCPItemTracer(Page);
        //        tracer->setVisible(true);
        //        tracer->setPen(QPen(Qt::DashLine));
        //        tracer->setStyle(QCPItemTracer::tsCrosshair);
        Page->xAxis->setRange(0,Time[numOfPage-1][size-1]);
        Page->yAxis->setRange(min[numOfPage-1],max[numOfPage-1]);
        Page->yAxis2->setRange(0,100);
        Page->selectionRect()->setPen(QPen(Qt::black,1,Qt::DashLine));//设置选框的样式：虚线
        Page->selectionRect()->setBrush(QBrush(QColor(0,0,100,50)));//设置选框的样式：半透明浅蓝
        Page->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
        Page->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        Page->replot();
        ui->tabWidget->setCurrentIndex(numOfPage-1);
        if(isFirst)
            isFirst=false;
        break;
    }
    case 4:
    {
        Page->yAxis->setLabel(data_reader[numOfPage-1].YAxis_1);
        Page->yAxis2->setVisible(true);
        Page->yAxis2->setLabel(data_reader[numOfPage-1].YAxis_2);
        if(data_reader[numOfPage-1].numOfDataGroup==7)
        {
            Page->axisRect()->addAxis(QCPAxis::atRight);
            Page->axisRect()->addAxis(QCPAxis::atLeft);
            Page->axisRect()->addAxes(QCPAxis::atRight);
            Page->axisRect()->axis(QCPAxis::atRight,0)->setPadding(1);
            Page->axisRect()->axis(QCPAxis::atRight,0)->setLabelPadding(0);
            Page->axisRect()->axis(QCPAxis::atRight,1)->setPadding(1);
            Page->axisRect()->axis(QCPAxis::atRight,1)->setLabelPadding(0);
            Page->axisRect()->axis(QCPAxis::atRight,1)->setRange(0,1200);
            Page->axisRect()->axis(QCPAxis::atRight,1)->setLabel("SunShine(W/M^2)");
            Page->axisRect()->axis(QCPAxis::atLeft,0)->setPadding(1);
            Page->axisRect()->axis(QCPAxis::atLeft,0)->setLabelPadding(0);
            Page->axisRect()->axis(QCPAxis::atLeft,1)->setPadding(1);
            Page->axisRect()->axis(QCPAxis::atLeft,1)->setLabelPadding(0);
            Page->axisRect()->axis(QCPAxis::atLeft,1)->setLabel("BlkTemp(°C)");
            Page->axisRect()->axis(QCPAxis::atLeft,1)->setRange(-75,175);
            Page->axisRect()->axis(QCPAxis::atRight,2)->setPadding(1);
            Page->axisRect()->axis(QCPAxis::atRight,2)->setLabelPadding(0);
            Page->axisRect()->axis(QCPAxis::atRight,2)->setLabel("PID(%)");
            Page->axisRect()->axis(QCPAxis::atRight,2)->setRange(0,100);
            Page->axisRect()->axis(QCPAxis::atRight,1)->setVisible(false);
            Page->axisRect()->axis(QCPAxis::atLeft,1)->setVisible(false);
            Page->axisRect()->axis(QCPAxis::atRight,2)->setVisible(false);
            Page->addGraph(Page->xAxis,Page->yAxis);
            Page->addGraph(Page->xAxis,Page->yAxis);
            Page->addGraph(Page->xAxis,Page->yAxis2);
            Page->addGraph(Page->xAxis,Page->yAxis2);
            Page->addGraph(Page->xAxis,Page->yAxis2);
            Page->addGraph(Page->xAxis,Page->yAxis2);
            Page->addGraph(Page->xAxis,Page->yAxis2);
        }
        else if(data_reader[numOfPage-1].numOfDataGroup==12)
        {
            Page->axisRect()->addAxis(QCPAxis::atRight);
            Page->axisRect()->addAxis(QCPAxis::atLeft);
            Page->axisRect()->addAxes(QCPAxis::atRight);
            Page->axisRect()->axis(QCPAxis::atRight,0)->setPadding(1);
            Page->axisRect()->axis(QCPAxis::atRight,0)->setLabelPadding(0);
            Page->axisRect()->axis(QCPAxis::atRight,1)->setPadding(1);
            Page->axisRect()->axis(QCPAxis::atRight,1)->setLabelPadding(0);
            Page->axisRect()->axis(QCPAxis::atRight,1)->setRange(0,1200);
            Page->axisRect()->axis(QCPAxis::atRight,1)->setLabel("SunShine(W/M^2)");
            Page->axisRect()->axis(QCPAxis::atLeft,0)->setPadding(1);
            Page->axisRect()->axis(QCPAxis::atLeft,0)->setLabelPadding(0);
            Page->axisRect()->axis(QCPAxis::atLeft,1)->setPadding(1);
            Page->axisRect()->axis(QCPAxis::atLeft,1)->setLabelPadding(0);
            Page->axisRect()->axis(QCPAxis::atLeft,1)->setLabel("BlkTemp(°C)");
            Page->axisRect()->axis(QCPAxis::atLeft,1)->setRange(-75,175);
            Page->axisRect()->axis(QCPAxis::atRight,2)->setPadding(1);
            Page->axisRect()->axis(QCPAxis::atRight,2)->setLabelPadding(0);
            Page->axisRect()->axis(QCPAxis::atRight,2)->setLabel("PID(%)");
            Page->axisRect()->axis(QCPAxis::atRight,2)->setRange(0,100);
            Page->addGraph(Page->xAxis,Page->yAxis);
            Page->addGraph(Page->xAxis,Page->yAxis);
            Page->addGraph(Page->xAxis,Page->yAxis2);
            Page->addGraph(Page->xAxis,Page->yAxis2);
            Page->addGraph(Page->xAxis,Page->axisRect()->axis(QCPAxis::atRight,1));
            Page->addGraph(Page->xAxis,Page->axisRect()->axis(QCPAxis::atRight,1));
            Page->addGraph(Page->xAxis,Page->axisRect()->axis(QCPAxis::atLeft,1));
            Page->addGraph(Page->xAxis,Page->axisRect()->axis(QCPAxis::atLeft,1));
            Page->addGraph(Page->xAxis,Page->axisRect()->axis(QCPAxis::atRight,1));
            Page->addGraph(Page->xAxis,Page->axisRect()->axis(QCPAxis::atRight,2));
            Page->addGraph(Page->xAxis,Page->axisRect()->axis(QCPAxis::atRight,2));
            Page->addGraph(Page->xAxis,Page->axisRect()->axis(QCPAxis::atRight,2));
        }
        QList<QCPAxis*> axes;
        axes << Page->yAxis2 << Page->xAxis2 << Page->yAxis << Page->xAxis<<Page->axisRect()->axis(QCPAxis::atRight,1)<<Page->axisRect()->axis(QCPAxis::atLeft,1);
        Page->axisRect()->setRangeZoomAxes(axes);
        Page->axisRect()->setRangeDragAxes(axes);
        Data[numOfPage-1].resize(data_reader[numOfPage-1].numOfDataGroup);
        int size = data_reader[numOfPage-1].Processed_Data.size()/data_reader[numOfPage-1].numOfDataGroup;
        Time[numOfPage-1].resize(size);
        TimeHMS[numOfPage-1].resize(size);
        QDateTime now;
        QDate nowDate(data_reader[numOfPage-1].startTime[0],data_reader[numOfPage-1].startTime[1],data_reader[numOfPage-1].startTime[2]);
        QTime nowTime(data_reader[numOfPage-1].startTime[3],data_reader[numOfPage-1].startTime[4],data_reader[numOfPage-1].startTime[5]);
        now.setDate(nowDate);
        now.setTime(nowTime);
        NOW[numOfPage-1] = now.toSecsSinceEpoch();
        //        qDebug()<<data_reader[numOfPage-1].Processed_Data.size()<<data_reader[numOfPage-1].numOfDataGroup<<size<<data_reader[numOfPage-1].Processed_Time.size();
        for(int i=0 ; i<data_reader[numOfPage-1].numOfDataGroup ; i++)
        {
            Data[numOfPage-1][i].resize(size);
        }
        for(int i=0 ; i<data_reader[numOfPage-1].Processed_Data.size()/data_reader[numOfPage-1].numOfDataGroup ; i++)
        {
            Time[numOfPage-1][i] =NOW[numOfPage-1] + data_reader[numOfPage-1].dataFrequency*i;
            TimeHMS[numOfPage-1][i] = data_reader[numOfPage-1].dataFrequency*i;
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
        //        qDebug()<<Time[numOfPage-1][0]<<Time[numOfPage-1][1]<<Data[numOfPage-1][2][0]<<Data[numOfPage-1][2][1];
        for(int i=0 ; i<data_reader[numOfPage-1].numOfDataGroup ; i++)
        {
            Page->graph(i)->setPen(color[i]);
            Page->graph(i)->addData(Time[numOfPage-1],Data[numOfPage-1][i]);
            //            if(i==0||i==1)
            //                Page->graph(i)->rescaleAxes();
            //            else
            //                Page->yAxis2->rescale(true);
        }
        //        tracer = new QCPItemTracer(Page);
        //        tracer->setVisible(true);
        //        tracer->setPen(QPen(Qt::DashLine));
        //        tracer->setStyle(QCPItemTracer::tsCrosshair);
        //        qDebug()<<NOW;
        QSharedPointer<QCPAxisTickerDateTime>  dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("yyyy-MM-dd\nhh:mm:ss");
        Page->xAxis->setTicker(dateTicker);
        Page->xAxis->setRange(Time[numOfPage-1][0],Time[numOfPage-1][size-1]);
        max[numOfPage-1] = data_reader[numOfPage-1].TDT_Y_MAX;
        min[numOfPage-1] =  data_reader[numOfPage-1].TDT_Y_MIN;
        Page->yAxis->setRange(data_reader[numOfPage-1].TDT_Y_MIN,data_reader[numOfPage-1].TDT_Y_MAX);
        Page->yAxis2->setRange(0,100);
        Page->xAxis->ticker()->setTickCount(9);
        Page->yAxis->ticker()->setTickCount(10);
        Page->selectionRect()->setPen(QPen(Qt::black,1,Qt::DashLine));//设置选框的样式：虚线
        Page->selectionRect()->setBrush(QBrush(QColor(0,0,100,50)));//设置选框的样式：半透明浅蓝
        Page->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
        Page->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        QString name__ = data_reader[numOfPage-1].getFileName();
        QCPTextElement* title;
        Page->plotLayout()->insertRow(0);
        title = new QCPTextElement(Page,name__);
        Page->plotLayout()->addElement(0,0,title);
        Page->replot();
        TDTupdateTrack(numOfPage-1);
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
    double y = currentPage[cnt]->yAxis->pixelToCoord(e->pos().y());
    if(isHMS==false)
    {
        QDateTime now;
        now.setSecsSinceEpoch(x);
        QString qTime = now.toString("yyyy-MM-dd hh:mm:ss");
        for(int i=0 ; i<data_reader[cnt].numOfDataGroup ; i++)
        {
            editTime[i]->setText(qTime);
        }
    }
    else
    {
        int H = x / (60*60);
        int M = (x- (H * 60 * 60)) / 60;
        int S = (x - (H * 60 * 60)) - M * 60;
        QString hour = QString::number(H);
        if (hour.length() == 1) hour = "0" + hour;
        QString min = QString::number(M);
        if (min.length() == 1) min = "0" + min;
        QString sec = QString::number(S);
        if (sec.length() == 1) sec = "0" + sec;
        QString qTime = hour + ":" + min + ":" + sec;
        for(int i=0 ; i<data_reader[cnt].numOfDataGroup ; i++)
        {
            editTime[i]->setText(qTime);
        }
    }
    //    tracer->setGraph(currentPage[cnt]->graph(0));
    //    tracer->setGraphKey(x);
    if(!isFirst)
    {
        tracer->setGraph(currentPage[cnt]->graph(0));
        tracer->setGraphKey(x);
        tracer->setInterpolating(true);
        tracer->updatePosition();
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),8));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,-10));                        //文字距离边框几个像素
        //        tracerLabel->position->setType(Qt::AlignCenter);
        tracerLabel->position->setCoords(x,y);
    }
    QString information;
    for(int i=0 ; i<data_reader[cnt].numOfDataGroup;i++)
    {
        tracer->setGraph(currentPage[cnt]->graph(i));
        tracer->setGraphKey(x);
        double yValue = tracer->position->value();
        if(curveName->button(i)->isChecked())
        {
            information = information+data_reader[cnt].Track_Info[i]+" "+QString::number(yValue)+'\n';
        }
        editData[i]->setText(QString::number(yValue));
    }
    tracerLabel->setText(information);
    currentPage[cnt]->replot();
}

void CurveFont::on_tabWidget_currentChanged(int index)
{
    if(index>=0&&!isFirst)
    {
        hideTrack();
        tracer->setVisible(false);
        tracerLabel->setVisible(false);
        tracer = new QCPItemTracer(currentPage[index]);
        tracer->setPen(QPen(Qt::DashLine));
        tracer->setStyle(QCPItemTracer::tsCrosshair);
        tracerLabel = new QCPItemText(currentPage[index]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        ui->tabWidget->setCurrentIndex(index);
        connect(currentPage[index],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[index]->replot();
        showTrack(index);
        //        ui->yMdhms_radiobutton->setChecked(true);
        if(ui->yMdhms_radiobutton->isChecked())
        {
            if(isFirst)
            {
                QMessageBox::StandardButton result = QMessageBox::critical(this,tr("错误"),tr("当前页面没有数据!"));
                ui->yMdhms_radiobutton->setChecked(true);
                return;
            }
            isHMS=false;
            int cnt = ui->tabWidget->currentIndex();
            for(int i=0 ; i<data_reader[cnt].numOfDataGroup ; i++)
            {
                currentPage[cnt]->graph(i)->data().data()->clear();
                currentPage[cnt]->graph(i)->addData(Time[cnt],Data[cnt][i]);
            }
            QSharedPointer<QCPAxisTickerDateTime>  dateTicker(new QCPAxisTickerDateTime);
            dateTicker->setDateTimeFormat("yyyy-MM-dd\nhh:mm:ss");
            currentPage[cnt]->xAxis->setTicker(dateTicker);
            currentPage[cnt]->xAxis->setRange(Time[cnt][0],Time[cnt][(Time[cnt].size()-1)]);
            currentPage[cnt]->yAxis->setRange(min[cnt],max[cnt]);
            currentPage[cnt]->yAxis2->setRange(0,100);
            currentPage[cnt]->replot();
        }
        else if(ui->hms_radiobutton->isChecked())
        {
            if(isFirst)
            {
                QMessageBox::StandardButton result = QMessageBox::critical(this,tr("错误"),tr("当前页面没有数据!"));
                ui->yMdhms_radiobutton->setChecked(true);
                return;
            }
            isHMS=true;
            int cnt = ui->tabWidget->currentIndex();
            for(int i=0 ; i<data_reader[cnt].numOfDataGroup ; i++)
            {
                currentPage[cnt]->graph(i)->data().data()->clear();
                currentPage[cnt]->graph(i)->addData(TimeHMS[cnt],Data[cnt][i]);
            }
            QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
            timeTicker->setTimeFormat("%h:%m:%s");
            currentPage[cnt]->xAxis->setTicker(timeTicker);
            currentPage[cnt]->xAxis->setRange(TimeHMS[cnt][0],TimeHMS[cnt][(TimeHMS[cnt].size()-1)]);
            currentPage[cnt]->yAxis->setRange(min[cnt],max[cnt]);
            currentPage[cnt]->yAxis2->setRange(0,100);
            currentPage[cnt]->replot();
        }
    }
}

void CurveFont::hideTrack()
{
    for(int i = 0 ; i<17 ; i++)
    {
        //        qDebug()<<1;
        editTime[i]->setVisible(false);
        curveName->button(i)->setVisible(false);
        //        curveName.button(i)->setChecked(false);
        editData[i]->setVisible(false);
        btnColor->button(i)->setVisible(false);
    }
}

void CurveFont::showTrack(int index)
{
    switch (data_reader[index].fileType) {
    case 4:
    {
        for(int i=0 ; i<data_reader[index].numOfDataGroup ; i++)
        {
            if(TDT_TrackUpdate[index].size()==0||TDT_TrackUpdate[index].value(i)==true)
            {
                editTime[i]->setVisible(true);
                curveName->button(i)->setVisible(true);
                curveName->button(i)->setChecked(true);
                QString strs = QString::number(i+1)+". "+data_reader[index].Track_Info[i];
                curveName->button(i)->setText(strs);
                editData[i]->setVisible(true);
                btnColor->button(i)->setVisible(true);
                QPalette pal = btnColor->button(i)->palette();
                pal.setColor(QPalette::Button,color[i]);
                btnColor->button(i)->setPalette(pal);
                btnColor->button(i)->setAutoFillBackground(true);
                editTime[i]->setEnabled(true);
                editData[i]->setEnabled(true);
                currentPage[index]->graph(i)->setVisible(true);
            }
            else if(TDT_TrackUpdate[index].value(i)==false)
            {
                editTime[i]->setVisible(true);
                curveName->button(i)->setVisible(true);
                curveName->button(i)->setChecked(false);
                QString strs = QString::number(i+1)+". "+data_reader[index].Track_Info[i];
                curveName->button(i)->setText(strs);
                editData[i]->setVisible(true);
                btnColor->button(i)->setVisible(true);
                QPalette pal = btnColor->button(i)->palette();
                pal.setColor(QPalette::Button,color[i]);
                btnColor->button(i)->setPalette(pal);
                btnColor->button(i)->setAutoFillBackground(true);
                editTime[i]->setEnabled(false);
                editData[i]->setEnabled(false);
                currentPage[index]->graph(i)->setVisible(false);
            }
        }
        currentPage[index]->replot();
        ui->scrollAreaWidgetContents->setMinimumHeight(30*data_reader[index].numOfDataGroup);
        break;
    }
    case 1:
    {
        for(int i=0 ; i<data_reader[index].numOfDataGroup ; i++)
        {
            if(SDR_TrackUpdate[index].size()==0||SDR_TrackUpdate[index].value(i)==true)
            {
                editTime[i]->setVisible(true);
                curveName->button(i)->setVisible(true);
                curveName->button(i)->setChecked(true);
                QString strs = QString::number(i+1)+". "+data_reader[index].Track_Info[i];
                curveName->button(i)->setText(strs);
                editData[i]->setVisible(true);
                btnColor->button(i)->setVisible(true);
                QPalette pal = btnColor->button(i)->palette();
                pal.setColor(QPalette::Button,color[i]);
                btnColor->button(i)->setPalette(pal);
                btnColor->button(i)->setAutoFillBackground(true);
                editTime[i]->setEnabled(true);
                editData[i]->setEnabled(true);
                currentPage[index]->graph(i)->setVisible(true);
            }
            else if(SDR_TrackUpdate[index].value(i)==false)
            {
                editTime[i]->setVisible(true);
                curveName->button(i)->setVisible(true);
                curveName->button(i)->setChecked(false);
                QString strs = QString::number(i+1)+". "+data_reader[index].Track_Info[i];
                curveName->button(i)->setText(strs);
                editData[i]->setVisible(true);
                btnColor->button(i)->setVisible(true);
                QPalette pal = btnColor->button(i)->palette();
                pal.setColor(QPalette::Button,color[i]);
                btnColor->button(i)->setPalette(pal);
                btnColor->button(i)->setAutoFillBackground(true);
                editTime[i]->setEnabled(false);
                editData[i]->setEnabled(false);
                currentPage[index]->graph(i)->setVisible(false);
            }
        }
        currentPage[index]->replot();
        ui->scrollAreaWidgetContents->setMinimumHeight(30*data_reader[index].numOfDataGroup);
        break;
    }
    }
}

void CurveFont::TDTupdateTrack(int index)
{
    if(TDT_TrackUpdate[index].size()==0&&data_reader[index].numOfDataGroup==7)
    {
        TDT_TrackUpdate[index].insert(0,false);
        TDT_TrackUpdate[index].insert(1,true);
        TDT_TrackUpdate[index].insert(2,false);
        TDT_TrackUpdate[index].insert(3,true);
        TDT_TrackUpdate[index].insert(4,false);
        TDT_TrackUpdate[index].insert(5,false);
        TDT_TrackUpdate[index].insert(6,false);
    }
    else if(TDT_TrackUpdate[index].size()==0&&data_reader[index].numOfDataGroup==12)
    {
        TDT_TrackUpdate[index].insert(0,false);
        TDT_TrackUpdate[index].insert(1,true);
        TDT_TrackUpdate[index].insert(2,false);
        TDT_TrackUpdate[index].insert(3,true);
        TDT_TrackUpdate[index].insert(4,false);
        TDT_TrackUpdate[index].insert(5,false);
        TDT_TrackUpdate[index].insert(6,false);
        TDT_TrackUpdate[index].insert(7,false);
        TDT_TrackUpdate[index].insert(8,false);
        TDT_TrackUpdate[index].insert(9,false);
        TDT_TrackUpdate[index].insert(10,false);
        TDT_TrackUpdate[index].insert(11,false);
    }
}

void CurveFont::SDRupdateTrack(int index)
{
    SDR_TrackUpdate[index].insert(0,false);
    SDR_TrackUpdate[index].insert(1,true);
    SDR_TrackUpdate[index].insert(2,false);
    SDR_TrackUpdate[index].insert(3,true);
}

void CurveFont::on_checkBox_1_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_1->isChecked())
    {
        currentPage[cnt]->graph(0)->setVisible(true);
        currentPage[cnt]->replot();
        editData[0]->setEnabled(true);
        editTime[0]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(0);
            TDT_TrackUpdate[cnt].insert(0,true);
        }
    }
    else
    {
        currentPage[cnt]->graph(0)->setVisible(false);
        currentPage[cnt]->replot();
        editData[0]->setEnabled(false);
        editTime[0]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(0);
            TDT_TrackUpdate[cnt].insert(0,false);
        }
    }
}


void CurveFont::on_checkBox_2_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_2->isChecked())
    {
        currentPage[cnt]->graph(1)->setVisible(true);
        currentPage[cnt]->replot();
        editData[1]->setEnabled(true);
        editTime[1]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(1);
            TDT_TrackUpdate[cnt].insert(1,true);
        }
    }
    else
    {
        currentPage[cnt]->graph(1)->setVisible(false);
        currentPage[cnt]->replot();
        editData[1]->setEnabled(false);
        editTime[1]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(1);
            TDT_TrackUpdate[cnt].insert(1,false);
        }
    }
}


void CurveFont::on_checkBox_3_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_3->isChecked())
    {
        currentPage[cnt]->graph(2)->setVisible(true);
        currentPage[cnt]->replot();
        editData[2]->setEnabled(true);
        editTime[2]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(2);
            TDT_TrackUpdate[cnt].insert(2,true);
        }
    }
    else
    {
        currentPage[cnt]->graph(2)->setVisible(false);
        currentPage[cnt]->replot();
        editData[2]->setEnabled(false);
        editTime[2]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(2);
            TDT_TrackUpdate[cnt].insert(2,false);
        }
    }
}


void CurveFont::on_checkBox_4_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_4->isChecked())
    {
        currentPage[cnt]->graph(3)->setVisible(true);
        currentPage[cnt]->replot();
        editData[3]->setEnabled(true);
        editTime[3]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(3);
            TDT_TrackUpdate[cnt].insert(3,true);

        }
    }
    else
    {
        currentPage[cnt]->graph(3)->setVisible(false);
        currentPage[cnt]->replot();
        editData[3]->setEnabled(false);
        editTime[3]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(3);
            TDT_TrackUpdate[cnt].insert(3,false);
        }
    }
}


void CurveFont::on_checkBox_5_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_5->isChecked())
    {
        currentPage[cnt]->graph(4)->setVisible(true);
        currentPage[cnt]->replot();
        editData[4]->setEnabled(true);
        editTime[4]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(4);
            TDT_TrackUpdate[cnt].insert(4,true);
        }
    }
    else
    {
        currentPage[cnt]->graph(4)->setVisible(false);
        currentPage[cnt]->replot();
        editData[4]->setEnabled(false);
        editTime[4]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(4);
            TDT_TrackUpdate[cnt].insert(4,false);
        }
    }
}


void CurveFont::on_checkBox_6_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_6->isChecked())
    {
        currentPage[cnt]->graph(5)->setVisible(true);
        currentPage[cnt]->replot();
        editData[5]->setEnabled(true);
        editTime[5]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(5);
            TDT_TrackUpdate[cnt].insert(5,true);
        }
    }
    else
    {
        currentPage[cnt]->graph(5)->setVisible(false);
        currentPage[cnt]->replot();
        editData[5]->setEnabled(false);
        editTime[5]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(5);
            TDT_TrackUpdate[cnt].insert(5,false);
        }
    }
}


void CurveFont::on_checkBox_7_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_7->isChecked())
    {
        currentPage[cnt]->graph(6)->setVisible(true);
        currentPage[cnt]->replot();
        editData[6]->setEnabled(true);
        editTime[6]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(6);
            TDT_TrackUpdate[cnt].insert(6,true);
        }
    }
    else
    {
        currentPage[cnt]->graph(6)->setVisible(false);
        currentPage[cnt]->replot();
        editData[6]->setEnabled(false);
        editTime[6]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(6);
            TDT_TrackUpdate[cnt].insert(6,false);
        }
    }
}


void CurveFont::on_checkBox_8_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_8->isChecked())
    {
        currentPage[cnt]->graph(7)->setVisible(true);
        currentPage[cnt]->replot();
        editData[7]->setEnabled(true);
        editTime[7]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(7);
            TDT_TrackUpdate[cnt].insert(7,true);
        }
    }
    else
    {
        currentPage[cnt]->graph(7)->setVisible(false);
        currentPage[cnt]->replot();
        editData[7]->setEnabled(false);
        editTime[7]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(7);
            TDT_TrackUpdate[cnt].insert(7,false);
        }
    }
}


void CurveFont::on_checkBox_9_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_9->isChecked())
    {
        currentPage[cnt]->graph(8)->setVisible(true);
        currentPage[cnt]->replot();
        editData[8]->setEnabled(true);
        editTime[8]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(8);
            TDT_TrackUpdate[cnt].insert(8,true);
        }
    }
    else
    {
        currentPage[cnt]->graph(8)->setVisible(false);
        currentPage[cnt]->replot();
        editData[8]->setEnabled(false);
        editTime[8]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(8);
            TDT_TrackUpdate[cnt].insert(8,false);
        }
    }
}


void CurveFont::on_checkBox_10_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_10->isChecked())
    {
        currentPage[cnt]->graph(9)->setVisible(true);
        currentPage[cnt]->replot();
        editData[9]->setEnabled(true);
        editTime[9]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(9);
            TDT_TrackUpdate[cnt].insert(9,true);
        }
    }
    else
    {
        currentPage[cnt]->graph(9)->setVisible(false);
        currentPage[cnt]->replot();
        editData[9]->setEnabled(false);
        editTime[9]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(9);
            TDT_TrackUpdate[cnt].insert(9,false);
        }
    }
}


void CurveFont::on_checkBox_11_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_11->isChecked())
    {
        currentPage[cnt]->graph(10)->setVisible(true);
        currentPage[cnt]->replot();
        editData[10]->setEnabled(true);
        editTime[10]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(10);
            TDT_TrackUpdate[cnt].insert(10,true);
        }
    }
    else
    {
        currentPage[cnt]->graph(10)->setVisible(false);
        currentPage[cnt]->replot();
        editData[10]->setEnabled(false);
        editTime[10]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(10);
            TDT_TrackUpdate[cnt].insert(10,false);
        }
    }
}


void CurveFont::on_checkBox_12_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_12->isChecked())
    {
        currentPage[cnt]->graph(11)->setVisible(true);
        currentPage[cnt]->replot();
        editData[11]->setEnabled(true);
        editTime[11]->setEnabled(true);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(11);
            TDT_TrackUpdate[cnt].insert(11,true);
        }
    }
    else
    {
        currentPage[cnt]->graph(11)->setVisible(false);
        currentPage[cnt]->replot();
        editData[11]->setEnabled(false);
        editTime[11]->setEnabled(false);
        if(TDT_TrackUpdate[cnt].size()!=0)
        {
            TDT_TrackUpdate[cnt].remove(11);
            TDT_TrackUpdate[cnt].insert(11,false);
        }
    }
}


void CurveFont::on_checkBox_13_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_13->isChecked())
    {
        currentPage[cnt]->graph(12)->setVisible(true);
        currentPage[cnt]->replot();
        editData[12]->setEnabled(true);
        editTime[12]->setEnabled(true);
    }
    else
    {
        currentPage[cnt]->graph(12)->setVisible(false);
        currentPage[cnt]->replot();
        editData[12]->setEnabled(false);
        editTime[12]->setEnabled(false);
    }
}


void CurveFont::on_checkBox_14_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_14->isChecked())
    {
        currentPage[cnt]->graph(13)->setVisible(true);
        currentPage[cnt]->replot();
        editData[13]->setEnabled(true);
        editTime[13]->setEnabled(true);
    }
    else
    {
        currentPage[cnt]->graph(13)->setVisible(false);
        currentPage[cnt]->replot();
        editData[13]->setEnabled(false);
        editTime[13]->setEnabled(false);
    }
}


void CurveFont::on_checkBox_15_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_15->isChecked())
    {
        currentPage[cnt]->graph(14)->setVisible(true);
        currentPage[cnt]->replot();
        editData[14]->setEnabled(true);
        editTime[14]->setEnabled(true);
    }
    else
    {
        currentPage[cnt]->graph(14)->setVisible(false);
        currentPage[cnt]->replot();
        editData[14]->setEnabled(false);
        editTime[14]->setEnabled(false);
    }
}


void CurveFont::on_checkBox_16_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_16->isChecked())
    {
        currentPage[cnt]->graph(15)->setVisible(true);
        currentPage[cnt]->replot();
        editData[15]->setEnabled(true);
        editTime[15]->setEnabled(true);
    }
    else
    {
        currentPage[cnt]->graph(15)->setVisible(false);
        currentPage[cnt]->replot();
        editData[15]->setEnabled(false);
        editTime[15]->setEnabled(false);
    }
}


void CurveFont::on_checkBox_17_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    if(ui->checkBox_17->isChecked())
    {
        currentPage[cnt]->graph(16)->setVisible(true);
        currentPage[cnt]->replot();
        editData[16]->setEnabled(true);
        editTime[16]->setEnabled(true);
    }
    else
    {
        currentPage[cnt]->graph(16)->setVisible(false);
        currentPage[cnt]->replot();
        editData[16]->setEnabled(false);
        editTime[16]->setEnabled(false);
    }
}


void CurveFont::on_color_1_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(0)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_1->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_1->setPalette(pal);
    ui->color_1->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_2_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(1)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_2->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_2->setPalette(pal);
    ui->color_2->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_3_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(2)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_3->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_3->setPalette(pal);
    ui->color_3->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_4_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(3)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_4->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_4->setPalette(pal);
    ui->color_4->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_5_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(4)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_5->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_5->setPalette(pal);
    ui->color_5->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_6_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(5)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_6->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_6->setPalette(pal);
    ui->color_6->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_7_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(6)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_7->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_7->setPalette(pal);
    ui->color_7->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_8_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(7)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_8->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_8->setPalette(pal);
    ui->color_8->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_9_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(8)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_9->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_9->setPalette(pal);
    ui->color_9->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_10_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(9)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_10->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_10->setPalette(pal);
    ui->color_10->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_11_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(10)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_11->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_11->setPalette(pal);
    ui->color_11->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_12_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(11)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_12->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_12->setPalette(pal);
    ui->color_12->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_13_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(12)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_13->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_13->setPalette(pal);
    ui->color_13->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_14_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(13)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_14->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_14->setPalette(pal);
    ui->color_14->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_15_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(4)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_15->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_15->setPalette(pal);
    ui->color_15->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_16_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(15)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_1->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_16->setPalette(pal);
    ui->color_16->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_color_17_clicked()
{
    int cnt = ui->tabWidget->currentIndex();
    QColorDialog color;
    QColor c = color.getColor();
    currentPage[cnt]->graph(16)->setPen(QPen(QColor(c.red(),c.green(),c.blue())));
    QPalette pal = ui->color_17->palette();
    pal.setColor(QPalette::Button,c);
    ui->color_17->setPalette(pal);
    ui->color_17->setAutoFillBackground(true);
    //    ui->color_1->setFlat(true);
    currentPage[cnt]->replot();
}


void CurveFont::on_actionOpenFile_triggered()
{
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    ui->statusbar->addWidget(label);
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
}


void CurveFont::on_actionDelete_triggered()
{
    if(isFirst)
    {
        QMessageBox::StandardButton result = QMessageBox::critical(this,tr("错误"),tr("当前页面没有数据!"));
        return;
    }
    int cnt = ui->tabWidget->currentIndex();
    numOfPage--;
    if(numOfPage==0)
    {
        isFirst=true;
    }
    ui->tabWidget->removeTab(cnt);
    for(int i=cnt ; i<data_reader.size()-1 ; i++)
    {
        data_reader[i] = data_reader[i+1];
    }
    for(int i=cnt ; i<currentPage.size()-1 ; i++)
    {
        currentPage[i] = currentPage[i+1];
    }
    for(int i=cnt ; i<Time.size()-1 ; i++)
    {
        Time[i] = Time[i+1];
    }
    for(int i=cnt ; i<Data.size()-1 ; i++)
    {
        Data[i] = Data[i+1];
    }
    for(int i=cnt ; i<min.size()-1 ; i++)
    {
        min[i] = min[i+1];
    }
    for(int i=cnt ; i<max.size()-1 ; i++)
    {
        max[i] = max[i+1];
    }
    for(int i=cnt ; i<NOW.size()-1 ; i++)
    {
        NOW[i] = NOW[i+1];
    }
    for(int i=cnt ; i<TimeHMS.size()-1 ; i++)
    {
        TimeHMS[i] = TimeHMS[i+1];
    }
}


void CurveFont::on_actionSave_triggered()
{
    if(isFirst)
    {
        QMessageBox::StandardButton result = QMessageBox::critical(this,tr("错误"),tr("当前页面没有数据!"));
        return;
    }
    QFileDialog* fd = new QFileDialog(this);//创建打开文件对话框
    QString fileName = fd->getExistingDirectory(this,tr("Open File"),"./");
    if(fileName == "")
        return;
    int cnt = ui->tabWidget->currentIndex();
    QDir dir = QDir::current();
    QString path = dir.filePath(fileName);
    QString PATH="";
    for(int i=0;i<path.size();i++){
        if(path[i]=='/'){
            PATH+="\\\\";
        }
        else{
            PATH+=path[i];
        }
    }
    PATH= PATH+"\\\\Temak-"+data_reader[cnt].getFileName();
    QDir csvdir;
    if(!csvdir.exists(PATH))
    {
        csvdir.mkpath(PATH);
    }
    PATH+="\\\\";
    QString currentPath = data_reader[cnt].getFilePath();
    QString name;
    for(int i=currentPath.size()-1 ; i>0 ; i--)
    {
        if(currentPath[i]!='\\')
            name+=currentPath[i];
        else
            break;
    }
    for(int i=name.size()-1 ; i>0 ; i--)
    {
        if(name[i]!='.')
            PATH+=name[i];
        else
            break;
    }
    PATH+=".csv";
    int num = Data[cnt][0].size()/65000;
    num+=1;
    QString* SavePath = new QString[num];
    int pathindex = 1;
    if(num>1)
    {
        for(int i=0 ; i<num ; i++)
        {
            SavePath[i] = "";
            for(int j=0 ; j<PATH.size() ; j++)
            {
                SavePath[i]+=PATH[j];
                if(j<PATH.size()-2&&PATH[j+1] == '.'&&PATH[j+2]=='c')
                {
                    SavePath[i]=SavePath[i]+'-'+QString::number(pathindex);
                    pathindex++;
                }
            }
        }
        QDateTime dateTime_;
        QDate date_(data_reader[cnt].startTime[0],data_reader[cnt].startTime[1],data_reader[cnt].startTime[2]);
        QTime time_(data_reader[cnt].startTime[3],data_reader[cnt].startTime[4],data_reader[cnt].startTime[5]);
        dateTime_.setDate(date_);
        dateTime_.setTime(time_);
        QString strs = "";
        //        QTime frequency(0,0,0);
        //        QString strs_ = "";
        int DATAFREQUENCY = data_reader[cnt].dataFrequency;
        long bit = 0;
        for(int k=0 ; k<num ; k++)
        {
            static QMutex mutex;
            mutex.lock();
            QFile file(SavePath[k]);
            qDebug()<<SavePath[k];
            if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
            {
                //                qDebug()<<3;
                QTextStream in(&file);
                in<<"文件路径："<<","<<data_reader[cnt].getFilePath()<<'\n';
                if(data_reader[cnt].haveStratTime==true)
                {
                    QDateTime dateTime;
                    QDate date(data_reader[cnt].startTime[0],data_reader[cnt].startTime[1],data_reader[cnt].startTime[2]);
                    QTime time(data_reader[cnt].startTime[3],data_reader[cnt].startTime[4],data_reader[cnt].startTime[5]);
                    dateTime.setDate(date);
                    dateTime.setTime(time);
                    QString strs = dateTime.toString("yyyy-MM-dd hh:mm:ss");
                    in<<"时间段："<<","<<'\t'<<strs<<'\t'<<",";
                    int total = TimeHMS[cnt].size();
                    dateTime = dateTime.addSecs(TimeHMS[cnt][total-1]);
                    strs = "";
                    strs = dateTime.toString("yyyy-MM-dd hh:mm:ss");
                    in<<'\t'<<strs<<'\t'<<'\n';
                }
                int total = TimeHMS[cnt].size();
                long x = TimeHMS[cnt][total-1];
                int H = x / (60*60);
                int M = (x- (H * 60 * 60)) / 60;
                int S = (x - (H * 60 * 60)) - M * 60;
                QString hour = QString::number(H);
                if (hour.length() == 1) hour = "0" + hour;
                QString min = QString::number(M);
                if (min.length() == 1) min = "0" + min;
                QString sec = QString::number(S);
                if (sec.length() == 1) sec = "0" + sec;
                QString qTime = hour + ":" + min + ":" + sec;
                in<<"总用时："<<","<<qTime<<'\n';
                in<<'\n';
                in<<"Date"<<","<<"Time";
                for(int i=0 ; i<data_reader[cnt].numOfDataGroup ; i++)
                    in<<","<<data_reader[cnt].Track_Info[i];
                in<<'\n';
                for(bit; (bit/65000)<(k+1) ; bit++)
                {
                    strs = dateTime_.toString("yyyy-MM-dd hh:mm:ss");
                    //                    strs_ = frequency.toString("hh:mm:ss");
                    dateTime_  =  dateTime_.addSecs(DATAFREQUENCY);
                    //                    frequency = frequency.addSecs(DATAFREQUENCY);
                    in<<'\t'<<strs<<'\t';
                    long x_ = TimeHMS[cnt][bit];
                    int H_ = x_ / (60*60);
                    int M_ = (x_- (H_ * 60 * 60)) / 60;
                    int S_ = (x_ - (H_ * 60 * 60)) - M_ * 60;
                    QString hour = QString::number(H_);
                    if (hour.length() == 1) hour = "0" + hour;
                    QString min = QString::number(M_);
                    if (min.length() == 1) min = "0" + min;
                    QString sec = QString::number(S_);
                    if (sec.length() == 1) sec = "0" + sec;
                    QString qTime = hour + ":" + min + ":" + sec;
                    in<<","<<qTime;
                    for(int j=0 ; j<data_reader[cnt].numOfDataGroup ; j++)
                    {
                        in<<","<<Data[cnt][j][bit];
                    }
                    in<<'\n';
                }
                //                qDebug()<<1;
                file.close();
            }
            mutex.unlock();
        }
    }
    else
    {
        static QMutex mutex;
        mutex.lock();
        QFile file(PATH);
        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            QTextStream in(&file);
            in<<"文件路径："<<","<<data_reader[cnt].getFilePath()<<'\n';
            if(data_reader[cnt].haveStratTime==true)
            {
                QDateTime dateTime;
                QDate date(data_reader[cnt].startTime[0],data_reader[cnt].startTime[1],data_reader[cnt].startTime[2]);
                QTime time(data_reader[cnt].startTime[3],data_reader[cnt].startTime[4],data_reader[cnt].startTime[5]);
                dateTime.setDate(date);
                dateTime.setTime(time);
                QString strs = dateTime.toString("yyyy-MM-dd hh:mm:ss");
                in<<"时间段："<<","<<'\t'<<strs<<'\t'<<",";
                int total = TimeHMS[cnt].size();
                dateTime = dateTime.addSecs(TimeHMS[cnt][total-1]);
                strs = "";
                strs = dateTime.toString("yyyy-MM-dd hh:mm:ss");
                in<<'\t'<<strs<<'\t'<<'\n';
            }
            int total = TimeHMS[cnt].size();
            long x = TimeHMS[cnt][total-1];
            int H = x / (60*60);
            int M = (x- (H * 60 * 60)) / 60;
            int S = (x - (H * 60 * 60)) - M * 60;
            QString hour = QString::number(H);
            if (hour.length() == 1) hour = "0" + hour;
            QString min = QString::number(M);
            if (min.length() == 1) min = "0" + min;
            QString sec = QString::number(S);
            if (sec.length() == 1) sec = "0" + sec;
            QString qTime = hour + ":" + min + ":" + sec;
            in<<"总用时："<<","<<qTime<<'\n';
            in<<'\n';
            in<<"Date"<<","<<"Time";
            for(int i=0 ; i<data_reader[cnt].numOfDataGroup ; i++)
                in<<","<<data_reader[cnt].Track_Info[i];
            in<<'\n';
            QDateTime dateTime_;
            QDate date_(data_reader[cnt].startTime[0],data_reader[cnt].startTime[1],data_reader[cnt].startTime[2]);
            QTime time_(data_reader[cnt].startTime[3],data_reader[cnt].startTime[4],data_reader[cnt].startTime[5]);
            dateTime_.setDate(date_);
            dateTime_.setTime(time_);
            QString strs = "";
            QTime frequency(0,0,0);
            QString strs_ = "";
            int DATAFREQUENCY = data_reader[cnt].dataFrequency;
            for(int i=0 ; i<Data[cnt][0].size() ; i++)
            {
                strs = dateTime_.toString("yyyy-MM-dd hh:mm:ss");
                strs_ = frequency.toString("hh:mm:ss");
                dateTime_  =  dateTime_.addSecs(DATAFREQUENCY);
                frequency = frequency.addSecs(DATAFREQUENCY);
                in<<'\t'<<strs<<'\t';
                in<<","<<strs_;
                for(int j=0 ; j<data_reader[cnt].numOfDataGroup ; j++)
                {
                    in<<","<<Data[cnt][j][i];
                }
                in<<'\n';
            }
            file.close();
        }
        mutex.unlock();
    }
}


void CurveFont::on_actionReset_triggered()
{
    if(isFirst)
    {
        QMessageBox::StandardButton result = QMessageBox::critical(this,tr("错误"),tr("当前页面没有数据!"));
        return;
    }
    int cnt = ui->tabWidget->currentIndex();
    currentPage[cnt]->yAxis->setRange(min[cnt],max[cnt]);
    currentPage[cnt]->yAxis2->setRange(0,100);
    currentPage[cnt]->axisRect()->axis(QCPAxis::atRight,1)->setRange(0,1200);
    currentPage[cnt]->axisRect()->axis(QCPAxis::atLeft,1)->setRange(-75,175);
    if(isHMS==true)
    {
        currentPage[cnt]->xAxis->setRange(TimeHMS[cnt][0],TimeHMS[cnt][(TimeHMS[cnt].size()-1)]);
    }
    else
        currentPage[cnt]->xAxis->setRange(Time[cnt][0],Time[cnt][(Time[cnt].size()-1)]);
    currentPage[cnt]->replot();
}


void CurveFont::on_actionHistoryFile0_triggered()
{
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    ui->statusbar->addWidget(label);
    QString fileName = ui->actionHistoryFile0->text();
    if(fileName == "")
    {
        label->setText("");
        return;
    }
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        //        tracerLabel->position->setParentAnchor(0);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
}


void CurveFont::on_actionHistoryFile1_triggered()
{
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    ui->statusbar->addWidget(label);
    QString fileName = ui->actionHistoryFile1->text();
    if(fileName == "")
    {
        label->setText("");
        return;
    }
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        //        tracerLabel->position->setParentAnchor(0);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
}


void CurveFont::on_actionHistoryFile2_triggered()
{
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    ui->statusbar->addWidget(label);
    QString fileName = ui->actionHistoryFile2->text();
    if(fileName == "")
    {
        label->setText("");
        return;
    }
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        //        tracerLabel->position->setParentAnchor(0);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
}


void CurveFont::on_actionHistoryFile3_triggered()
{
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    ui->statusbar->addWidget(label);
    QString fileName = ui->actionHistoryFile3->text();
    if(fileName == "")
    {
        label->setText("");
        return;
    }
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        //        tracerLabel->position->setParentAnchor(0);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
}


void CurveFont::on_actionHistoryFile4_triggered()
{
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    ui->statusbar->addWidget(label);
    QString fileName = ui->actionHistoryFile4->text();
    if(fileName == "")
    {
        label->setText("");
        return;
    }
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        //        tracerLabel->position->setParentAnchor(0);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
}


void CurveFont::on_actionHistoryFile5_triggered()
{
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    QString fileName = ui->actionHistoryFile5->text();
    if(fileName == "")
    {
        label->setText("");
        return;
    }
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        //        tracerLabel->position->setParentAnchor(0);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
}


void CurveFont::on_actionHistoryFile6_triggered()
{
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    ui->statusbar->addWidget(label);
    QString fileName = ui->actionHistoryFile6->text();
    if(fileName == "")
    {
        label->setText("");
        return;
    }
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        //        tracerLabel->position->setParentAnchor(0);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
}


void CurveFont::on_actionHistoryFile7_triggered()
{
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    ui->statusbar->addWidget(label);
    QString fileName = ui->actionHistoryFile7->text();
    if(fileName == "")
    {
        label->setText("");
        return;
    }
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        //        tracerLabel->position->setParentAnchor(0);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
}


void CurveFont::on_actionHistoryFile8_triggered()
{
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    ui->statusbar->addWidget(label);
    QString fileName = ui->actionHistoryFile8->text();
    if(fileName == "")
    {
        label->setText("");
        return;
    }
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        //        tracerLabel->position->setParentAnchor(0);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
}


void CurveFont::on_actionHistoryFile9_triggered()
{
    QLabel* label = new QLabel(this);
    label->setText(tr("正在处理，请稍后..."));
    ui->statusbar->addWidget(label);
    QString fileName = ui->actionHistoryFile9->text();
    if(fileName == "")
    {
        label->setText("");
        return;
    }
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
        tracerLabel = new QCPItemText(currentPage[0]);
        tracerLabel->setLayer("overlay");                                  //设置图层为overlay，因为需要频繁刷新
        tracerLabel->setPen(QPen(Qt::black));                              //设置游标说明颜色
        tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        tracerLabel->setFont(QFont(font().family(),6));                   //字体大小
        tracerLabel->setPadding(QMargins(1,1,1,1));                        //文字距离边框几个像素
        //        tracerLabel->position->setParentAnchor(0);
        connect(currentPage[0],SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mousemove(QMouseEvent*)));
        currentPage[0]->replot();
        showTrack(0);
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
    label->setText("");
    ui->statusbar->showMessage(tr("完成"),1000);
}


void CurveFont::on_hms_radiobutton_clicked()
{
    if(isFirst)
    {
        QMessageBox::StandardButton result = QMessageBox::critical(this,tr("错误"),tr("当前页面没有数据!"));
        ui->yMdhms_radiobutton->setChecked(true);
        return;
    }
    isHMS=true;
    int cnt = ui->tabWidget->currentIndex();
    for(int i=0 ; i<data_reader[cnt].numOfDataGroup ; i++)
    {
        currentPage[cnt]->graph(i)->data().data()->clear();
        currentPage[cnt]->graph(i)->addData(TimeHMS[cnt],Data[cnt][i]);
    }
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    currentPage[cnt]->xAxis->setTicker(timeTicker);
    currentPage[cnt]->xAxis->setRange(TimeHMS[cnt][0],TimeHMS[cnt][(TimeHMS[cnt].size()-1)]);
    currentPage[cnt]->yAxis->setRange(min[cnt],max[cnt]);
    currentPage[cnt]->yAxis2->setRange(0,100);
    currentPage[cnt]->replot();
}


void CurveFont::on_yMdhms_radiobutton_clicked()
{
    if(isFirst)
    {
        QMessageBox::StandardButton result = QMessageBox::critical(this,tr("错误"),tr("当前页面没有数据!"));
        ui->yMdhms_radiobutton->setChecked(true);
        return;
    }
    isHMS=false;
    int cnt = ui->tabWidget->currentIndex();
    for(int i=0 ; i<data_reader[cnt].numOfDataGroup ; i++)
    {
        currentPage[cnt]->graph(i)->data().data()->clear();
        currentPage[cnt]->graph(i)->addData(Time[cnt],Data[cnt][i]);
    }
    QSharedPointer<QCPAxisTickerDateTime>  dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("yyyy-MM-dd\nhh:mm:ss");
    currentPage[cnt]->xAxis->setTicker(dateTicker);
    currentPage[cnt]->xAxis->setRange(Time[cnt][0],Time[cnt][(Time[cnt].size()-1)]);
    currentPage[cnt]->yAxis->setRange(min[cnt],max[cnt]);
    currentPage[cnt]->yAxis2->setRange(0,100);
    currentPage[cnt]->replot();
}


void CurveFont::on_actionTemak_S_triggered()
{
    QString strs = "本软件归泰美科环境仪器（昆山）有限公司所有<br>公司网站：<a href='http://www.temak.com.cn/'>http://www.temak.com.cn/</a>";
    QMessageBox::about(nullptr,"说明",strs);
}

