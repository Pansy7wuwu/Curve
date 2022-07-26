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
    QVector<QVector<QVector<double>>> Data;
    QVector<double> min,max;
    QColor* color;
    QCPItemTracer* tracer;
    QButtonGroup curveName;
    QButtonGroup btnColor;
    QVector<QLineEdit*> editData,editTime;

    void closeEvent(QCloseEvent* event);
    void updateHistoryFile();
    void draw();

public slots:
    void mousemove(QMouseEvent* e);

private slots:
    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_actionChinese_triggered();

    void on_actionEnglish_triggered();

    void on_actionTchinese_triggered();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::CurveFont *ui;
    int numOfHistoryFile = 0;
};
#endif // CURVEFONT_H
