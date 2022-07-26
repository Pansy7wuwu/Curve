#ifndef CURVEFONT_H
#define CURVEFONT_H

#include <QMainWindow>
#include <data_reader.h>
#include <QCloseEvent>
#include <QQueue>
#include <QTranslator>
#include <QVector>
#include <qcustomplot.h>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class CurveFont; }
QT_END_NAMESPACE

class CurveFont : public QMainWindow
{
    Q_OBJECT

public:
    CurveFont(QWidget *parent = nullptr);
    ~CurveFont();


    QVector<Data_Reader> data_reader;
    QQueue<QString> historyFile;
    int numOfPage = 0;
    bool isFirst = true;
    QVector<QCustomPlot*> currentPage;
    QVector<QVector<double>> Time;
    QVector<QVector<double>> TimeHMS;
    QVector<QVector<QVector<double>>> Data;
    QVector<double> min,max;
    QColor* color;
    QCPItemTracer* tracer;
    QCPItemText* tracerLabel;
    QButtonGroup* curveName;
    QButtonGroup* btnColor;
    QButtonGroup* rBtnTimeStyle;
    QVector<QLineEdit*> editData,editTime;
    int pageIndex = 1;
    QVector<double> NOW;
    bool isHMS=false;
    QVector<QMap<int , bool>> TDT_TrackUpdate;
    QVector<QMap<int , bool>> SDR_TrackUpdate;

    void closeEvent(QCloseEvent* event);
    void updateHistoryFile();
    void draw();
    void showTrack(int);
    void hideTrack();
    void TDTupdateTrack(int);
    void SDRupdateTrack(int);


public slots:
    void mousemove(QMouseEvent* e);

private slots:
    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_actionChinese_triggered();

    void on_actionEnglish_triggered();

    void on_actionTchinese_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_checkBox_1_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

    void on_checkBox_5_clicked();

    void on_checkBox_6_clicked();

    void on_checkBox_7_clicked();

    void on_checkBox_8_clicked();

    void on_checkBox_9_clicked();

    void on_checkBox_10_clicked();

    void on_checkBox_11_clicked();

    void on_checkBox_12_clicked();

    void on_checkBox_13_clicked();

    void on_checkBox_14_clicked();

    void on_checkBox_15_clicked();

    void on_checkBox_16_clicked();

    void on_checkBox_17_clicked();

    void on_color_1_clicked();

    void on_color_2_clicked();

    void on_color_3_clicked();

    void on_color_4_clicked();

    void on_color_5_clicked();

    void on_color_6_clicked();

    void on_color_7_clicked();

    void on_color_8_clicked();

    void on_color_9_clicked();

    void on_color_10_clicked();

    void on_color_11_clicked();

    void on_color_12_clicked();

    void on_color_13_clicked();

    void on_color_14_clicked();

    void on_color_15_clicked();

    void on_color_16_clicked();

    void on_color_17_clicked();

    void on_actionOpenFile_triggered();

    void on_actionDelete_triggered();

    void on_actionSave_triggered();

    void on_actionReset_triggered();

    void on_actionHistoryFile0_triggered();

    void on_actionHistoryFile1_triggered();

    void on_actionHistoryFile2_triggered();

    void on_actionHistoryFile3_triggered();

    void on_actionHistoryFile4_triggered();

    void on_actionHistoryFile5_triggered();

    void on_actionHistoryFile6_triggered();

    void on_actionHistoryFile7_triggered();

    void on_actionHistoryFile8_triggered();

    void on_actionHistoryFile9_triggered();

    void on_hms_radiobutton_clicked();

    void on_yMdhms_radiobutton_clicked();

    void on_actionTemak_S_triggered();

private:
    Ui::CurveFont *ui;
    int numOfHistoryFile = 0;
};
#endif // CURVEFONT_H
