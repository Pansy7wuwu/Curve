#ifndef DATA_READER_H
#define DATA_READER_H

#include<QMap>


class Data_Reader
{
public:
    Data_Reader();
    Data_Reader(QString);

    QMap<long long, int> map; //读取的数据
    QMap<long long, float> Processed_Data; //处理完的数据
    QMap<long long, int> Processed_Time; //处理完的时间
    QMap<long long, int> sdrNumOfData;
    QString filePath; //文件路径
    qlonglong readNumOfByte; //读取的总字节数
    int fileType; //kdt->0 M310(sdr)->1 dat->2 unknow->3
    int kdtType; //kdt后缀文件的类别
    int startTime[6];//开始时间数组分别存放年、月、日、时、分、秒
    int dataFrequency;//数据频率
    int Y_Max;//Y轴上限
    int Y_Min;//Y轴下限
    int startBit;//数据起始位
    int totalTime;//总用时 秒
    int numOfDataGroup;//数据组数
    int numOfYAxis;//Y轴数量
    QString* Track_Info;//每组数据名称
    QString* YAxis_Info;
    int sdrType;//sdr后缀文件类别
    short maxTemperature;
    short minTemperature;
    short maxHumidity;
    short minHumidity;
    short sdrTemperatureMultiple;
    short sdrHumidityMultiple;
    int datType;
    int TDT_Y_MAX;
    int TDT_Y_MIN;
    int TDT_Multiple;
    bool haveStratTime = false;

    void setFilePath(QString);//设置文件路径
    QString getFilePath();
    void readToMap();//读取到map
    void judgeFile();//判断文件后缀类别（kdt\sdr\dat）
    void dealData();//处理数据
    void dealType0();//处理kdt数据
    void judgeKdtType();//判断kdt类别
    void getType0StartBit();//获取起始位
    void dealType1();//处理sdr数据
    void judgeSdrType();//判断sdr类别
    void dealType2();//处理dat数据
    void judgeDatType();//判断dat类别
    void dealType3();//处理未知文件类别
    void dealType4();

private:
    QString fileType1 = "kdt";
    QString fileType2 = "sdr";
    QString fileType3 = "dat";
    QString fileType4 = "tdt";
    QString kdtType0 = "6E0B";
    QString kdtType1 = "5E0C";
    QString kdtType2 = "FE0A";
    QString kdtType3 = "400A";
    QString kdtType4 = "E009";
    QString kdtType5 = "B811";
    QString kdtType6 = "A801";
    QString kdtType7 = "Unknow";
    QString sdrType0 = "0503";
    QString sdrType1 = "0504";
    QString sdrType2 = "0505";
    QString sdrType3 = "Unknow";
    QString datType0 = "0612";
    QString datType1 = "0C13";
    QString datType2 = "0A10";
    QString datType3 = "Unknow";
    QString YAxis_Info0 = "";//y轴名称
    QString YAxis_Info1 = "";
    QString YAxis_Info2 = "";
    QString YAxis_Info3 = "";
    QString YAxis_Info4 = "";
    QString YAxis_Info5 = "";
    QString YAxis_Info0_unit = "";
    QString YAxis_Info1_unit = "";
    QString YAxis_Info2_unit = "";
    QString YAxis_Info3_unit = "";
    QString YAxis_Info4_unit = "";
    QString YAxis_Info5_unit = "";


};

#endif // DATA_READER_H
