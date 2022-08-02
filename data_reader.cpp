#include "data_reader.h"
#include<QDebug>
#include<QFile>
#include<fstream>
#include<iostream>
#include<QMessageBox>
using namespace std;

Data_Reader::Data_Reader()
{
    filePath = "";
}

Data_Reader::Data_Reader(QString filePath_)
{
    filePath = filePath_;
}

void Data_Reader::setFilePath(QString filePath_)
{
    if(filePath_=="")
    {
        qDebug() << " 弹窗警告文件路径为空！ " << Qt::endl;
        return;
    }
    filePath = filePath_;
}

void Data_Reader::readToMap()
{
    if(filePath=="")
    {
        qDebug() << " 弹窗警告文件路径为空！ " << Qt::endl;
        return;
    }
    char* filePath_;
    QByteArray ba = filePath.toLocal8Bit();
    filePath_ = ba.data();
    ifstream toMap;
    toMap = ifstream(filePath_,ios::in|ios::binary);
    if(!toMap.is_open())
    {
        qDebug() << " 弹窗警告文件打开失败！ " << Qt::endl;
        return;
    }
    readNumOfByte = 0;
    unsigned char temp;
    while(toMap.read((char*)&temp,1))
    {
        map.insert(readNumOfByte,(unsigned)temp);
        readNumOfByte++;
    }
    // 测试map读取是否正确
    //    QMap<long long,int>::iterator iter = map.begin();
    //    for(int i=0 ; i<10 ; i++)
    //    {
    //        qDebug() << iter.key() << iter.value() << Qt::endl;
    //        iter++;
    //    }
    toMap.close();
    return;
}

QString Data_Reader::getFileName()
{
    QString path = filePath;
    QString name;
    for(int i=path.size()-1; i>0 ; i--)
    {
        if(path[i]!='\\')
        {
            name+=path[i];
        }
        else
            break;
    }
    QString name_;
    for(int i=name.size()-1 ; i>=0 ; i--)
    {
        name_+=name[i];
    }
    return name_;
}

void Data_Reader::judgeFile()
{
    QString Path = filePath.toLower();
    int length = Path.length();
    length -= 3;
    QString strs= Path.mid(length);
    //    qDebug() << strs;
    if(strs == fileType1)
        fileType = 0;
    else if(strs == fileType2)
        fileType = 1;
    else if(strs == fileType3)
        fileType = 2;
    else if(strs == fileType4)
        fileType = 4;
    else
        fileType = 3;
    return;
}

//处理文件
void Data_Reader::dealData()
{
    judgeFile();
    switch(fileType)
    {
    case 0:
        dealType0();
        break;
    case 1:
        dealType1();
        break;
    case 2:
        dealType2();
        break;
    case 3:
        dealType3();
        break;
    case 4:
        dealType4();
        break;
    }
}

//判断kdt文件的类型
void Data_Reader::judgeKdtType()
{
    if(map.isEmpty())
        readToMap();
    int bit13,bit14;
    bit13 = map.value(12);
    bit14 = map.value(13);
    if(bit13 == 110 && bit14 == 11)
        kdtType = 0;
    else if(bit13 == 94 && bit14 == 12)
        kdtType = 1;
    else if(bit13 == 254 && bit14 == 10)
        kdtType = 2;
    else if(bit13 == 64 && bit14 == 10)
        kdtType = 3;
    else if(bit13 == 224 && bit14 == 9)
        kdtType = 4;
    else if(bit13 == 184 && bit14 == 17)
        kdtType = 5;
    else if(bit13 == 168 && bit14 == 1)
        kdtType = 6;
    else
        kdtType = 7;
}

//获取kdt文件的起始位
void Data_Reader::getType0StartBit()
{
    if(map.isEmpty())
        readToMap();
    QMap<long long , int>::Iterator iter;
    iter = map.begin();
    for(;iter!=map.end();)
    {
        if(iter.value() == 68)
        {
            iter++;
            if(iter.value() == 65)
            {
                iter++;
                if(iter.value() == 84)
                {
                    iter++;
                    if(iter.value() == 65)
                    {
                        startBit = iter.key()+1;
                        break;
                    }
                    else
                    {
                        iter--;
                        iter--;
                        iter--;
                    }
                }
                else
                {
                    iter--;
                    iter--;
                }
            }
            else
                iter--;
        }
        iter++;
    }
    return;
}

//kdt后缀数据解析
void Data_Reader::dealType0()
{
    judgeKdtType();
    getType0StartBit();
    switch(kdtType)
    {
    case 0:
    {
        //Y轴内容
        numOfYAxis = 1;
        YAxis_Info0 = "Temperature";
        YAxis_Info0_unit = "°C";
        YAxis_Info = new QString[numOfYAxis];
        YAxis_Info[0] = "Temperature(°C)";
        //获取数据组数
        numOfDataGroup = 12;
        Track_Info = new QString[numOfDataGroup];
        for(int i=0; i<numOfDataGroup ; i++)
        {
            Track_Info[i] = "Temperature(°C)";
        }
        //开始时间
        haveStratTime = true;
        short year[2];
        year[0] = map.value(24);
        year[1] = map.value(25);
        short temp;
        temp = (unsigned short)(year[1] << 8)+ year[0];
        startTime[0] = temp;
        startTime[1] = map.value(26);
        startTime[2] = map.value(28);
        startTime[3] = map.value(30);
        startTime[4] = map.value(32);
        startTime[5] = map.value(34);
        //数据频率
        dataFrequency = map.value(36);
        //纵轴上限
        unsigned char YMax_temp[8];
        YMax_temp[0] = map.value(262);
        YMax_temp[1] = map.value(263);
        YMax_temp[2] = map.value(264);
        YMax_temp[3] = map.value(265);
        YMax_temp[4] = map.value(266);
        YMax_temp[5] = map.value(267);
        YMax_temp[6] = map.value(268);
        YMax_temp[7] = map.value(269);
        double YMax_d;
        unsigned char* YMax_uc = (unsigned char*)&YMax_d;
        for(int i=0 ; i<8 ; i++)
        {
            YMax_uc[i] = YMax_temp[i];
        }
        Y_Max = YMax_d;
        //纵轴下限
        unsigned char YMin_temp[8];
        YMin_temp[0] = map.value(270);
        YMin_temp[1] = map.value(271);
        YMin_temp[2] = map.value(272);
        YMin_temp[3] = map.value(273);
        YMin_temp[4] = map.value(274);
        YMin_temp[5] = map.value(275);
        YMin_temp[6] = map.value(276);
        YMin_temp[7] = map.value(277);
        double YMin_d;
        unsigned char* YMin_uc = (unsigned char*)&YMin_d;
        for(int i=0 ; i<8 ; i++)
        {
            YMin_uc[i] = YMin_temp[i];
        }
        Y_Min = YMin_d;
        //解析数据
        QMap<long long,int>::iterator iter = map.end();
        iter--;
        long long bit = (long long)startBit;
        int count = 0;
        int indexTime = 0;
        int indexData = 0;
        int tempTime[4];
        char tempData[4];
        while(bit <= iter.key())
        {
            if(count%(numOfDataGroup*4+4) == 0)
            {
                tempTime[0] = map.value(bit++);
                tempTime[1] = map.value(bit++);
                tempTime[2] = map.value(bit++);
                tempTime[3] = map.value(bit++);
                count+=4;
                int temp_a = (unsigned int)(tempTime[3]<<24);
                int temp_b = (unsigned int)(tempTime[2]<<16);
                int temp_c = (unsigned int)(tempTime[1]<<8);
                int real_time = tempTime[0]+temp_a+temp_b+temp_c;
                Processed_Time.insert(indexTime,real_time);
                indexTime++;
            }
            else
            {
                tempData[0] = map.value(bit++);
                tempData[1] = map.value(bit++);
                tempData[2] = map.value(bit++);
                tempData[3] = map.value(bit++);
                count+=4;
                float temp_f = (*(float*)tempData);
                Processed_Data.insert(indexData,temp_f);
                indexData++;
            }
        }
        QMap<long long,int>::iterator totalTime_iter = Processed_Time.end();
        totalTime_iter--;
        totalTime = totalTime_iter.value();
        qDebug()<<"case 0 OK";
        break;
    }
    case 1:
    {
        //Y轴内容
        numOfYAxis = 1;
        YAxis_Info0 = "Temperature";
        YAxis_Info0_unit = "°C";
        YAxis_Info = new QString[1];
        YAxis_Info[0] = "temperature(°C)";
        //获取组数
        numOfDataGroup = 17;
        Track_Info = new QString[numOfDataGroup];
        for(int i=0; i<numOfDataGroup ; i++)
        {
            Track_Info[i] = "Temperature(°C)";
        }
        //开始时间
        haveStratTime = true;
        short year[2];
        year[0] = map.value(24);
        year[1] = map.value(25);
        short temp;
        temp = (unsigned short)(year[1] << 8)+ year[0];
        startTime[0] = temp;
        startTime[1] = map.value(26);
        startTime[2] = map.value(28);
        startTime[3] = map.value(30);
        startTime[4] = map.value(32);
        startTime[5] = map.value(34);
        //数据频率
        dataFrequency = map.value(36);
        //纵轴上限
        unsigned char YMax_temp[8];
        YMax_temp[0] = map.value(262);
        YMax_temp[1] = map.value(263);
        YMax_temp[2] = map.value(264);
        YMax_temp[3] = map.value(265);
        YMax_temp[4] = map.value(266);
        YMax_temp[5] = map.value(267);
        YMax_temp[6] = map.value(268);
        YMax_temp[7] = map.value(269);
        double YMax_d;
        unsigned char* YMax_uc = (unsigned char*)&YMax_d;
        for(int i=0 ; i<8 ; i++)
        {
            YMax_uc[i] = YMax_temp[i];
        }
        Y_Max = YMax_d;
        //纵轴下限
        unsigned char YMin_temp[8];
        YMin_temp[0] = map.value(270);
        YMin_temp[1] = map.value(271);
        YMin_temp[2] = map.value(272);
        YMin_temp[3] = map.value(273);
        YMin_temp[4] = map.value(274);
        YMin_temp[5] = map.value(275);
        YMin_temp[6] = map.value(276);
        YMin_temp[7] = map.value(277);
        double YMin_d;
        unsigned char* YMin_uc = (unsigned char*)&YMin_d;
        for(int i=0 ; i<8 ; i++)
        {
            YMin_uc[i] = YMin_temp[i];
        }
        Y_Min = YMin_d;
        //解析数据
        long long bit = (long long)startBit;
        QMap<long long,int>::iterator iter = map.end();
        iter--;
        int count = 0;
        int indexTime = 0;
        int indexData = 0;
        int tempTime[4];
        char tempData[4];
        while(bit <= iter.key())
        {
            if(count%(numOfDataGroup*4+4) == 0)
            {
                tempTime[0] = map.value(bit++);
                tempTime[1] = map.value(bit++);
                tempTime[2] = map.value(bit++);
                tempTime[3] = map.value(bit++);
                count+=4;
                int temp_a = (unsigned int)(tempTime[3]<<24);
                int temp_b = (unsigned int)(tempTime[2]<<16);
                int temp_c = (unsigned int)(tempTime[1]<<8);
                int real_time = tempTime[0]+temp_a+temp_b+temp_c;
                Processed_Time.insert(indexTime,real_time);
                indexTime++;
            }
            else
            {
                tempData[0] = map.value(bit++);
                tempData[1] = map.value(bit++);
                tempData[2] = map.value(bit++);
                tempData[3] = map.value(bit++);
                count+=4;
                float temp_f = (*(float*)tempData);
                Processed_Data.insert(indexData,temp_f);
                indexData++;
            }
        }
        QMap<long long,int>::iterator totalTime_iter = Processed_Time.end();
        totalTime_iter--;
        totalTime = totalTime_iter.value();
        qDebug()<<"case 1 OK";
        break;
    }
    case 2:
    {
        //Y轴内容
        numOfYAxis = 3;
        YAxis_Info0 = "Temperature";
        YAxis_Info0_unit = "°C";
        YAxis_Info1 = "Humidity";
        YAxis_Info1_unit = "%RH";
        YAxis_Info2 = "PID";
        YAxis_Info2_unit = "%";
        YAxis_Info = new QString[3];
        YAxis_Info[0] = "Temperature(°C)";
        YAxis_Info[1] = "Humidity(RH%)";
        YAxis_Info[2] = "PID(%)";
        //获取组数
        numOfDataGroup = 6;
        Track_Info = new QString[numOfDataGroup];
        Track_Info[0] = "Temperature(°C)";
        Track_Info[1] = "Humidity(%RH)";
        Track_Info[2] = "Wet Bulb Temperature(°C)";
        Track_Info[3] = "Temp PID(%)";
        Track_Info[4] = "Humi PID(%)";
        Track_Info[5] = "Svr PID(%)";
        //获取时间
        haveStratTime = true;
        short year[2];
        year[0] = map.value(24);
        year[1] = map.value(25);
        short temp;
        temp = (unsigned short)(year[1] << 8)+ year[0];
        startTime[0] = temp;
        startTime[1] = map.value(26);
        startTime[2] = map.value(28);
        startTime[3] = map.value(30);
        startTime[4] = map.value(32);
        startTime[5] = map.value(34);
        //数据频率
        dataFrequency = map.value(36);
        //纵轴上限
        unsigned char YMax_temp[4];
        YMax_temp[0] = map.value(262);
        YMax_temp[1] = map.value(263);
        YMax_temp[2] = map.value(264);
        YMax_temp[3] = map.value(265);
        float YMax_d;
        unsigned char* YMax_uc = (unsigned char*)&YMax_d;
        for(int i=0 ; i<4 ; i++)
        {
            YMax_uc[i] = YMax_temp[i];
        }
        Y_Max = YMax_d;
        //纵轴下限
        unsigned char YMin_temp[4];
        YMin_temp[0] = map.value(266);
        YMin_temp[1] = map.value(267);
        YMin_temp[2] = map.value(268);
        YMin_temp[3] = map.value(269);
        float YMin_d;
        unsigned char* YMin_uc = (unsigned char*)&YMin_d;
        for(int i=0 ; i<4 ; i++)
        {
            YMin_uc[i] = YMin_temp[i];
        }
        Y_Min = YMin_d;
        //获取倍数
        int multiple;
        unsigned char multiple_temp[4];
        multiple_temp[0] = map.value(356);
        multiple_temp[1] = map.value(357);
        multiple_temp[2] = map.value(358);
        multiple_temp[3] = map.value(359);
        float multiple_d;
        unsigned char* multiple_uc = (unsigned char*)&multiple_d;
        for(int i=0 ; i<4 ; i++)
        {
            multiple_uc[i] =multiple_temp[i];
        }
        multiple = multiple_d;
        //解析数据
        long long bit = (long long)startBit;
        QMap<long long,int>::iterator iter = map.end();
        iter--;
        int count = 0;
        int indexTime = 0;
        int time = 0;
        int indexData = 0;
        int tempData[2];
        float realData;;
        while(bit <= iter.key())
        {
            if(count%(numOfDataGroup*2+2) == 0)
            {
                count+=2;
                Processed_Time.insert(indexTime,time);
                indexTime++;
                time+=dataFrequency;
            }
            else
            {
                tempData[0] = map.value(bit++);
                tempData[1] = map.value(bit++);
                count+=2;
                realData = ((tempData[1]<<8)+tempData[0])*1.0/multiple*1.0;
                Processed_Data.insert(indexData,realData);
                indexData++;
            }
        }
        QMap<long long,int>::iterator totalTime_iter = Processed_Time.end();
        totalTime_iter--;
        totalTime = totalTime_iter.value();
        qDebug()<<"case 2 OK";
        break;
    }
    case 3:
    {
        //Y轴内容
        numOfYAxis = 2;
        YAxis_Info0 = "Temperature";
        YAxis_Info0_unit = "°C";
        YAxis_Info1 = "PID";
        YAxis_Info1_unit = "%";
        YAxis_Info = new QString[2];
        YAxis_Info[0] = "Temperature(°C)";
        YAxis_Info[1] = "PID(%)";
        //获取组数
        numOfDataGroup = 4;
        Track_Info = new QString[numOfDataGroup];
        Track_Info[0] = "Temperature(°C)";
        Track_Info[1] = "Wet Bulb Temperature(°C)";
        Track_Info[2] = "Temp PID(%)";
        Track_Info[3] = "Svr PID(%)";
        //获取时间
        haveStratTime = true;
        short year[2];
        year[0] = map.value(24);
        year[1] = map.value(25);
        short temp;
        temp = (unsigned short)(year[1] << 8)+ year[0];
        startTime[0] = temp;
        startTime[1] = map.value(26);
        startTime[2] = map.value(28);
        startTime[3] = map.value(30);
        startTime[4] = map.value(32);
        startTime[5] = map.value(34);
        //数据频率
        dataFrequency = map.value(36);
        //纵轴上限
        unsigned char YMax_temp[4];
        YMax_temp[0] = map.value(262);
        YMax_temp[1] = map.value(263);
        YMax_temp[2] = map.value(264);
        YMax_temp[3] = map.value(265);
        float YMax_d;
        unsigned char* YMax_uc = (unsigned char*)&YMax_d;
        for(int i=0 ; i<4 ; i++)
        {
            YMax_uc[i] = YMax_temp[i];
        }
        Y_Max = YMax_d;
        //纵轴下限
        unsigned char YMin_temp[4];
        YMin_temp[0] = map.value(266);
        YMin_temp[1] = map.value(267);
        YMin_temp[2] = map.value(268);
        YMin_temp[3] = map.value(269);
        float YMin_d;
        unsigned char* YMin_uc = (unsigned char*)&YMin_d;
        for(int i=0 ; i<4 ; i++)
        {
            YMin_uc[i] = YMin_temp[i];
        }
        Y_Min = YMin_d;
        //获取倍数
        int multiple;
        unsigned char multiple_temp[4];
        multiple_temp[0] = map.value(356);
        multiple_temp[1] = map.value(357);
        multiple_temp[2] = map.value(358);
        multiple_temp[3] = map.value(359);
        float multiple_d;
        unsigned char* multiple_uc = (unsigned char*)&multiple_d;
        for(int i=0 ; i<4 ; i++)
        {
            multiple_uc[i] =multiple_temp[i];
        }
        multiple = multiple_d;
        //解析数据
        long long bit = (long long)startBit;
        QMap<long long,int>::iterator iter = map.end();
        iter--;
        int count = 0;
        int indexTime = 0;
        int time = 0;
        int indexData = 0;
        int tempData[2];
        float realData;;
        while(bit <= iter.key())
        {
            if(count%(numOfDataGroup*2+2) == 0)
            {
                count+=2;
                Processed_Time.insert(indexTime,time);
                indexTime++;
                time+=dataFrequency;
            }
            else
            {
                tempData[0] = map.value(bit++);
                tempData[1] = map.value(bit++);
                count+=2;
                realData = ((tempData[1]<<8)+tempData[0])*1.0/multiple*1.0;
                Processed_Data.insert(indexData,realData);
                indexData++;
            }
        }
        QMap<long long,int>::iterator totalTime_iter = Processed_Time.end();
        totalTime_iter--;
        totalTime = totalTime_iter.value();
        qDebug()<<"case 3 OK";
        break;
    }
    case 4:
    {
        //Y轴内容
        numOfYAxis = 2;
        YAxis_Info0 = "Temperature";
        YAxis_Info0_unit = "°C";
        YAxis_Info1 = "Humidity";
        YAxis_Info1_unit = "%RH";
        YAxis_Info = new QString[2];
        YAxis_Info[0] = "Temperature(°C)";
        YAxis_Info[1] = "Humidity(RH%)";
        //获取组数
        numOfDataGroup = 2;
        Track_Info = new QString[numOfDataGroup];
        Track_Info[0] = "Temperature(°C)";
        Track_Info[1] = "Humidity(%RH)";
        //获取时间
        haveStratTime = true;
        short year[2];
        year[0] = map.value(24);
        year[1] = map.value(25);
        short temp;
        temp = (unsigned short)(year[1] << 8)+ year[0];
        startTime[0] = temp;
        startTime[1] = map.value(26);
        startTime[2] = map.value(28);
        startTime[3] = map.value(30);
        startTime[4] = map.value(32);
        startTime[5] = map.value(34);
        //数据频率
        dataFrequency = map.value(36);
        //纵轴上限
        unsigned char YMax_temp[4];
        YMax_temp[0] = map.value(262);
        YMax_temp[1] = map.value(263);
        YMax_temp[2] = map.value(264);
        YMax_temp[3] = map.value(265);
        float YMax_d;
        unsigned char* YMax_uc = (unsigned char*)&YMax_d;
        for(int i=0 ; i<4 ; i++)
        {
            YMax_uc[i] = YMax_temp[i];
        }
        Y_Max = YMax_d;
        //纵轴下限
        unsigned char YMin_temp[4];
        YMin_temp[0] = map.value(266);
        YMin_temp[1] = map.value(267);
        YMin_temp[2] = map.value(268);
        YMin_temp[3] = map.value(269);
        float YMin_d;
        unsigned char* YMin_uc = (unsigned char*)&YMin_d;
        for(int i=0 ; i<4 ; i++)
        {
            YMin_uc[i] = YMin_temp[i];
        }
        Y_Min = YMin_d;
        //获取倍数
        int multiple;
        unsigned char multiple_temp[4];
        multiple_temp[0] = map.value(356);
        multiple_temp[1] = map.value(357);
        multiple_temp[2] = map.value(358);
        multiple_temp[3] = map.value(359);
        float multiple_d;
        unsigned char* multiple_uc = (unsigned char*)&multiple_d;
        for(int i=0 ; i<4 ; i++)
        {
            multiple_uc[i] =multiple_temp[i];
        }
        multiple = multiple_d;
        //解析数据
        long long bit = (long long)startBit;
        QMap<long long,int>::iterator iter = map.end();
        iter--;
        int count = 0;
        int indexTime = 0;
        int time = 0;
        int indexData = 0;
        int tempData[2];
        float realData;
        while(bit <= iter.key())
        {
            if(count%(numOfDataGroup*2+2) == 0)
            {
                count+=2;
                Processed_Time.insert(indexTime,time);
                indexTime++;
                time+=dataFrequency;
            }
            else
            {
                tempData[0] = map.value(bit++);
                tempData[1] = map.value(bit++);
                count+=2;
                realData = ((tempData[1]<<8)+tempData[0])*1.0/multiple*1.0;
                Processed_Data.insert(indexData,realData);
                indexData++;
            }
        }
        QMap<long long,int>::iterator totalTime_iter = Processed_Time.end();
        totalTime_iter--;
        totalTime = totalTime_iter.value();
        qDebug()<<"case 4 OK";
        break;
    }
    case 5:
    {
        //Y轴内容
        numOfYAxis = 2;
        YAxis_Info0 = "Temperature";
        YAxis_Info0_unit = "°C";
        YAxis_Info1 = "Humidity";
        YAxis_Info1_unit = "%RH";
        YAxis_Info2 = "Voltage";
        YAxis_Info2_unit = "V";
        YAxis_Info3 = "PID";
        YAxis_Info3_unit = "%";
        YAxis_Info4 = "Opens";
        YAxis_Info4_unit = "%";
        YAxis_Info5 = "Output";
        YAxis_Info5_unit = "ON/OFF";
        YAxis_Info = new QString[6];
        YAxis_Info[0] = "Temperature(°C)";
        YAxis_Info[1] = "Humidity(RH%)";
        YAxis_Info[2] = "Voltage(V)";
        YAxis_Info[3] = "PID(%)";
        YAxis_Info[4] = "Opens(%)";
        YAxis_Info[5] = "Output(ON/OFF)";
        //获取组数
        numOfDataGroup = 36;
        Track_Info = new QString[numOfDataGroup];
        Track_Info[0] = "Temp Pv(°C)";
        Track_Info[1] = "Humi Pv(%RH)";
        Track_Info[2] = "Wet Pv(°C)";
        Track_Info[3] = "Temp Sv(°C)";
        Track_Info[4] = "Humi Sv(%RH)";
        Track_Info[5] = "Wet Sv(°C)";
        Track_Info[6] = "C1 Dryer Temp(°C)";
        Track_Info[7] = "Ambient or Cooling Water Temp(°C)";
        Track_Info[8] = "AC Volt(V)";
        Track_Info[9] = "C1 Out Temp(°C)";
        Track_Info[10] = "C2 Out Temp(°C)";
        Track_Info[11] = "C3 Out Temp(°C)";
        Track_Info[12] = "C4 Out Temp(°C)";
        Track_Info[13] = "Tester Temp(°C)";
        Track_Info[14] = "Chamber Actual Temp(°C)";
        Track_Info[15] = "TPid(%)";
        Track_Info[16] = "HPid(%)";
        Track_Info[17] = "WPid(%)";
        Track_Info[18] = "SvrPid(%)";
        Track_Info[19] = "Humi Max Opens(%)";
        Track_Info[20] = "Humi Min Opens(%)";
        Track_Info[21] = "Svr Max Opens(%)";
        Track_Info[22] = "Svr Min Opens(%)";
        Track_Info[23] = "C2SvrPid(%)";
        Track_Info[24] = "Fan Opens(%)";
        Track_Info[25] = "P1(ON/OFF)";
        Track_Info[26] = "P2(ON/OFF)";
        Track_Info[27] = "P3(ON/OFF)";
        Track_Info[28] = "V1(ON/OFF)";
        Track_Info[29] = "V2(ON/OFF)";
        Track_Info[30] = "V3(ON/OFF)";
        Track_Info[31] = "V4(ON/OFF)";
        Track_Info[32] = "LNV(ON/OFF)";
        Track_Info[33] = "C1(ON/OFF)";
        Track_Info[34] = "C2(ON/OFF)";
        Track_Info[35] = "C12(ON/OFF)";
        //获取时间
        haveStratTime = true;
        short year[2];
        year[0] = map.value(24);
        year[1] = map.value(25);
        short temp;
        temp = (unsigned short)(year[1] << 8)+ year[0];
        startTime[0] = temp;
        startTime[1] = map.value(26);
        startTime[2] = map.value(28);
        startTime[3] = map.value(30);
        startTime[4] = map.value(32);
        startTime[5] = map.value(34);
        //数据频率
        dataFrequency = map.value(36);
        //纵轴上限
        unsigned char YMax_temp[4];
        YMax_temp[0] = map.value(262);
        YMax_temp[1] = map.value(263);
        YMax_temp[2] = map.value(264);
        YMax_temp[3] = map.value(265);
        float YMax_d;
        unsigned char* YMax_uc = (unsigned char*)&YMax_d;
        for(int i=0 ; i<4 ; i++)
        {
            YMax_uc[i] = YMax_temp[i];
        }
        Y_Max = YMax_d;
        //纵轴下限
        unsigned char YMin_temp[4];
        YMin_temp[0] = map.value(266);
        YMin_temp[1] = map.value(267);
        YMin_temp[2] = map.value(268);
        YMin_temp[3] = map.value(269);
        float YMin_d;
        unsigned char* YMin_uc = (unsigned char*)&YMin_d;
        for(int i=0 ; i<4 ; i++)
        {
            YMin_uc[i] = YMin_temp[i];
        }
        Y_Min = YMin_d;
        //获取倍数
        int multiple;
        unsigned char multiple_temp[4];
        multiple_temp[0] = map.value(356);
        multiple_temp[1] = map.value(357);
        multiple_temp[2] = map.value(358);
        multiple_temp[3] = map.value(359);
        float multiple_d;
        unsigned char* multiple_uc = (unsigned char*)&multiple_d;
        for(int i=0 ; i<4 ; i++)
        {
            multiple_uc[i] =multiple_temp[i];
        }
        multiple = multiple_d;
        //解析数据
        long long bit = (long long)startBit;
        QMap<long long,int>::iterator iter = map.end();
        iter--;
        int count = 0;
        int indexTime = 0;
        int time = 0;
        int indexData = 0;
        int tempData[2];
        float realData;
        while(bit <= iter.key())
        {
            if(count%(numOfDataGroup*2) == 0)
            {
                count+=2;
                Processed_Time.insert(indexTime,time);
                indexTime++;
                time+=dataFrequency;
            }
            else
            {
                tempData[0] = map.value(bit++);
                tempData[1] = map.value(bit++);
                count+=2;
                realData = ((tempData[1]<<8)+tempData[0])*1.0/multiple*1.0;
                Processed_Data.insert(indexData,realData);
                indexData++;
            }
        }
        QMap<long long,int>::iterator totalTime_iter = Processed_Time.end();
        totalTime_iter--;
        totalTime = totalTime_iter.value();
        qDebug()<<"case 5 OK";
        break;
    }
    case 6:
    {
        //Y轴内容
        numOfYAxis = 2;
        YAxis_Info0 = "Temperature";
        YAxis_Info0_unit = "°C";
        YAxis_Info1 = "Average Temperature";
        YAxis_Info1_unit = "°C";
        YAxis_Info = new QString[2];
        YAxis_Info[0] = "Temperature(°C)";
        YAxis_Info[1] = "Average Temperature(°C)";
        //获取组数
        numOfDataGroup = 7;
        Track_Info = new QString[numOfDataGroup];
        for(int i=0 ; i<6 ; i++)
        {
            Track_Info[i] = "Temperature(°C)";
        }
        Track_Info[6] = "Average Temperature(°C)";
        //获取时间
        haveStratTime = true;
        short year[2];
        year[0] = map.value(24);
        year[1] = map.value(25);
        short temp;
        temp = (unsigned short)(year[1] << 8)+ year[0];
        startTime[0] = temp;
        startTime[1] = map.value(26);
        startTime[2] = map.value(28);
        startTime[3] = map.value(30);
        startTime[4] = map.value(32);
        startTime[5] = map.value(34);
        //数据频率
        dataFrequency = map.value(36);
        //纵轴上限
        unsigned char YMax_temp[8];
        YMax_temp[0] = map.value(258);
        YMax_temp[1] = map.value(259);
        YMax_temp[2] = map.value(260);
        YMax_temp[3] = map.value(261);
        YMax_temp[4] = map.value(262);
        YMax_temp[5] = map.value(263);
        YMax_temp[6] = map.value(264);
        YMax_temp[7] = map.value(265);
        double YMax_d;
        unsigned char* YMax_uc = (unsigned char*)&YMax_d;
        for(int i=0 ; i<8 ; i++)
        {
            YMax_uc[i] = YMax_temp[i];
        }
        Y_Max = YMax_d;
        //纵轴下限
        unsigned char YMin_temp[8];
        YMin_temp[0] = map.value(266);
        YMin_temp[1] = map.value(267);
        YMin_temp[2] = map.value(268);
        YMin_temp[3] = map.value(269);
        YMin_temp[4] = map.value(270);
        YMin_temp[5] = map.value(271);
        YMin_temp[6] = map.value(272);
        YMin_temp[7] = map.value(273);
        double YMin_d;
        unsigned char* YMin_uc = (unsigned char*)&YMin_d;
        for(int i=0 ; i<8 ; i++)
        {
            YMin_uc[i] = YMin_temp[i];
        }
        Y_Min = YMin_d;
        //获取倍数
        int multiple;
        unsigned char multiple_temp[8];
        multiple_temp[0] = map.value(274);
        multiple_temp[1] = map.value(275);
        multiple_temp[2] = map.value(276);
        multiple_temp[3] = map.value(277);
        multiple_temp[4] = map.value(278);
        multiple_temp[5] = map.value(279);
        multiple_temp[6] = map.value(280);
        multiple_temp[7] = map.value(281);
        double multiple_d;
        unsigned char* multiple_uc = (unsigned char*)&multiple_d;
        for(int i=0 ; i<8 ; i++)
        {
            multiple_uc[i] =multiple_temp[i];
        }
        multiple = multiple_d;
        //解析数据
        QMap<long long,int>::iterator iter = map.end();
        iter--;
        long long bit = (long long)startBit;
        int count = 0;
        int time = 0;
        int indexTime = 0;
        int indexData = 0;
        char tempData[4];
        while(bit <= iter.key())
        {
            if(count%(numOfDataGroup*4+4) == 0)
            {
                count+=4;
                Processed_Time.insert(indexTime,time);
                indexTime++;
                time+=dataFrequency;
            }
            else
            {
                tempData[0] = map.value(bit++);
                tempData[1] = map.value(bit++);
                tempData[2] = map.value(bit++);
                tempData[3] = map.value(bit++);
                count+=4;
                float temp_f = (*(float*)tempData);
                Processed_Data.insert(indexData,(temp_f*1.0/multiple*1.0));
                indexData++;
            }
        }
        QMap<long long,int>::iterator totalTime_iter = Processed_Time.end();
        totalTime_iter--;
        totalTime = totalTime_iter.value();
        qDebug()<<"case 6 OK";
        break;
    }
    case 7:
    {
        qDebug()<<" 警告未识别的KDT类别！ ";
    }
    }
    return;
}

void Data_Reader::judgeSdrType()
{
    if(map.isEmpty())
        readToMap();
    int bit7,bit8;
    bit7 = map.value(6);
    bit8 = map.value(7);
    if(bit7 == 5 && bit8 == 3)
        sdrType = 0;
    else if(bit7 == 5 && bit8 == 4)
        sdrType = 1;
    else if(bit7 == 5 && bit8 ==5)
        sdrType = 2;
    else
        sdrType = 3;
}

void Data_Reader::dealType1()
{
    judgeSdrType();
    switch(sdrType)
    {
    case 0:
    {
        //Y轴内容
        numOfYAxis = 2;
        YAxis_Info0 = "Temperture";
        YAxis_Info0_unit = "°C";
        YAxis_Info1 = "Humidity";
        YAxis_Info1_unit = "RH%";
        YAxis_Info = new QString[2];
        YAxis_Info[0] = "Temperture(°C)";
        YAxis_Info[1] = "Humidity(RH%)";
        //数据组数
        numOfDataGroup = 4;
        Track_Info = new QString[numOfDataGroup];
        Track_Info[0] = "Temperature(°C)";
        Track_Info[1] = "PV Temperature(°C)";
        Track_Info[2] = "Humidity(RH%)";
        Track_Info[3] = "PV Humidity(RH%)";
        //获取温度实际值上限 获取温度倍数
        short maxTemp[2];
        maxTemp[0] = map.value(132);
        maxTemp[1] = map.value(133);
        maxTemperature = (maxTemp[1]<<8)+maxTemp[0];
        if(maxTemperature >= 4000)
            sdrTemperatureMultiple = 100;
        else
            sdrTemperatureMultiple = 10;
        //获取湿度实际值上限 获取湿度倍数
        short maxHumi[2];
        maxHumi[0] = map.value(136);
        maxHumi[1] = map.value(137);
        maxHumidity = (maxHumi[1]<<8)+maxHumi[0];
        if(maxHumidity >= 4000)
            sdrHumidityMultiple = 100;
        else
            sdrHumidityMultiple = 10;
        //获取温度实际值下限
        short minTemp[2];
        minTemp[0] = map.value(164);
        minTemp[1] = map.value(165);
        minTemperature = (minTemp[1]<<8)+minTemp[0];
        //获取湿度实际值下限
        short minHumi[2];
        minHumi[0] = map.value(168);
        minHumi[1] = map.value(169);
        minHumidity = (minHumi[1]<<8)+minHumi[1];
        //获取起始位
        startBit = 512;
        //获取结束标志
        QMap<long long,int>::Iterator iter = map.end();
        iter--;
        //解析数据
        long long bit = startBit;
        int timeIndex = 0;
        int dataIndex = 0;
        int sdrNumOfDataIndex = 0;
        int numTemp[2];
        int num = 0;
        int actualNumTemp[2];
        int actualNum;
        float realityNum;
        float A;
        while(bit <= iter.key())
        {
            int judge[2];
            judge[0] = map.value(bit++);
            judge[1] = map.value(bit++);
            if(judge[0] == 68 && judge[1] == 0)
            {
                Processed_Time.insert(timeIndex++,(map.value(bit++)+2000));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                dataFrequency = map.value(bit++)/2;
                bit+=5;
                numTemp[0] = map.value(bit++);
                numTemp[1] = map.value(bit++);
                num = (numTemp[1]<<8)+numTemp[0];
                sdrNumOfData.insert(sdrNumOfDataIndex++,num);
                for(int i=0 ; i<(num/4) ; i++)
                {
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxTemperature-minTemperature)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minTemperature)*1.0/sdrTemperatureMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxTemperature-minTemperature)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minTemperature)*1.0/sdrTemperatureMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxHumidity-minHumidity)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minHumidity)*1.0/sdrHumidityMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxHumidity-minHumidity)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minHumidity)*1.0/sdrHumidityMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                }
            }
        }
        break;
    }
    case 1:
    {
        //Y轴内容
        numOfYAxis = 2;
        YAxis_Info0 = "Temperture";
        YAxis_Info0_unit = "°C";
        YAxis_Info1 = "Humidity";
        YAxis_Info1_unit = "RH%";
        YAxis_Info = new QString[2];
        YAxis_Info[0] = "Temperture(°C)";
        YAxis_Info[1] = "Humidity(RH%)";
        //数据组数
        numOfDataGroup = 4;
        Track_Info = new QString[numOfDataGroup];
        Track_Info[0] = "Temperature(°C)";
        Track_Info[1] = "PV Temperature(°C)";
        Track_Info[2] = "Humidity(RH%)";
        Track_Info[3] = "PV Humidity(RH%)";
        //获取温度实际值上限 获取温度倍数
        short maxTemp[2];
        maxTemp[0] = map.value(132);
        maxTemp[1] = map.value(133);
        maxTemperature = (maxTemp[1]<<8)+maxTemp[0];
        if(maxTemperature >= 4000)
            sdrTemperatureMultiple = 100;
        else
            sdrTemperatureMultiple = 10;
        //获取湿度实际值上限 获取湿度倍数
        short maxHumi[2];
        maxHumi[0] = map.value(136);
        maxHumi[1] = map.value(137);
        maxHumidity = (maxHumi[1]<<8)+maxHumi[0];
        if(maxHumidity >= 4000)
            sdrHumidityMultiple = 100;
        else
            sdrHumidityMultiple = 10;
        //获取温度实际值下限
        short minTemp[2];
        minTemp[0] = map.value(164);
        minTemp[1] = map.value(165);
        minTemperature = (minTemp[1]<<8)+minTemp[0];
        //获取湿度实际值下限
        short minHumi[2];
        minHumi[0] = map.value(168);
        minHumi[1] = map.value(169);
        minHumidity = (minHumi[1]<<8)+minHumi[1];
        //获取起始位
        startBit = 512;
        //获取结束标志
        QMap<long long,int>::Iterator iter = map.end();
        iter--;
        //解析数据
        long long bit = startBit;
        int timeIndex = 0;
        int dataIndex = 0;
        int sdrNumOfDataIndex = 0;
        int numTemp[2];
        int num = 0;
        int actualNumTemp[2];
        int actualNum;
        float realityNum;
        float A;
        while(bit <= iter.key())
        {
            int judge[2];
            judge[0] = map.value(bit++);
            judge[1] = map.value(bit++);
            if(judge[0] == 68 && judge[1] == 0)
            {
                Processed_Time.insert(timeIndex++,(map.value(bit++)+2000));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                dataFrequency = map.value(bit++)/2;
                bit+=5;
                numTemp[0] = map.value(bit++);
                numTemp[1] = map.value(bit++);
                num = (numTemp[1]<<8)+numTemp[0];
                sdrNumOfData.insert(sdrNumOfDataIndex++,num);
                for(int i=0 ; i<(num/4) ; i++)
                {
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxTemperature-minTemperature)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minTemperature)*1.0/sdrTemperatureMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxTemperature-minTemperature)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minTemperature)*1.0/sdrTemperatureMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxHumidity-minHumidity)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minHumidity)*1.0/sdrHumidityMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxHumidity-minHumidity)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minHumidity)*1.0/sdrHumidityMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                }
            }
        }
        break;
    }
    case 2:
    {
        //Y轴内容
        numOfYAxis = 2;
        YAxis_Info0 = "Temperture";
        YAxis_Info0_unit = "°C";
        YAxis_Info1 = "Humidity";
        YAxis_Info1_unit = "RH%";
        YAxis_Info = new QString[2];
        YAxis_Info[0] = "Temperture(°C)";
        YAxis_Info[1] = "Humidity(RH%)";
        //数据组数
        numOfDataGroup = 4;
        Track_Info = new QString[numOfDataGroup];
        Track_Info[0] = "Temperature(°C)";
        Track_Info[1] = "PV Temperature(°C)";
        Track_Info[2] = "Humidity(RH%)";
        Track_Info[3] = "PV Humidity(RH%)";
        //获取温度实际值上限 获取温度倍数
        short maxTemp[2];
        maxTemp[0] = map.value(132);
        maxTemp[1] = map.value(133);
        maxTemperature = (maxTemp[1]<<8)+maxTemp[0];
        if(maxTemperature >= 4000)
            sdrTemperatureMultiple = 100;
        else
            sdrTemperatureMultiple = 10;
        //获取湿度实际值上限 获取湿度倍数
        short maxHumi[2];
        maxHumi[0] = map.value(136);
        maxHumi[1] = map.value(137);
        maxHumidity = (maxHumi[1]<<8)+maxHumi[0];
        if(maxHumidity >= 4000)
            sdrHumidityMultiple = 100;
        else
            sdrHumidityMultiple = 10;
        //获取温度实际值下限
        short minTemp[2];
        minTemp[0] = map.value(164);
        minTemp[1] = map.value(165);
        minTemperature = (minTemp[1]<<8)+minTemp[0];
        //获取湿度实际值下限
        short minHumi[2];
        minHumi[0] = map.value(168);
        minHumi[1] = map.value(169);
        minHumidity = (minHumi[1]<<8)+minHumi[1];
        //获取起始位
        startBit = 512;
        //获取结束标志
        QMap<long long,int>::Iterator iter = map.end();
        iter--;
        //解析数据
        long long bit = startBit;
        int timeIndex = 0;
        int dataIndex = 0;
        int sdrNumOfDataIndex = 0;
        int numTemp[2];
        int num = 0;
        int actualNumTemp[2];
        int actualNum;
        float realityNum;
        float A;
        while(bit <= iter.key())
        {
            int judge[2];
            judge[0] = map.value(bit++);
            judge[1] = map.value(bit++);
            if(judge[0] == 68 && judge[1] == 0)
            {
                Processed_Time.insert(timeIndex++,(map.value(bit++)+2000));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                Processed_Time.insert(timeIndex++,map.value(bit++));
                dataFrequency = map.value(bit++)/2;
                bit+=5;
                numTemp[0] = map.value(bit++);
                numTemp[1] = map.value(bit++);
                num = (numTemp[1]<<8)+numTemp[0];
                sdrNumOfData.insert(sdrNumOfDataIndex++,num);
                for(int i=0 ; i<(num/4) ; i++)
                {
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxTemperature-minTemperature)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minTemperature)*1.0/sdrTemperatureMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxTemperature-minTemperature)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minTemperature)*1.0/sdrTemperatureMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxHumidity-minHumidity)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minHumidity)*1.0/sdrHumidityMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                    actualNumTemp[0] = map.value(bit++);
                    actualNumTemp[1] = map.value(bit++);
                    actualNum = (actualNumTemp[1]<<8)+actualNumTemp[0];
                    A = (actualNum*1.0*(maxHumidity-minHumidity)*1.0)*1.0/30000*1.0;
                    realityNum =(A+minHumidity)*1.0/sdrHumidityMultiple*1.0;
                    Processed_Data.insert(dataIndex++,realityNum);
                }
            }
        }
        break;
    }
    case 3:
    {
        qDebug()<<" 警告未知sdr类型！ ";
    }
    }
    return;
}

void Data_Reader::judgeDatType()
{
    if(map.isEmpty())
        readToMap();
    int bit27,bit28;
    bit27 = map.value(26);
    bit28 = map.value(27);
    if(bit27 == 6 && bit28 ==18)
        datType = 0;
    else if(bit27 == 12 && bit28 == 19)
        datType = 1;
    else if(bit27 == 10 && bit28 == 16)
        datType = 2;
    else
        datType = 3;
}

void Data_Reader::dealType2()
{
    judgeDatType();
    switch(datType)
    {
    case 0:
    {
        //Y轴内容
        numOfYAxis = 2;
        YAxis_Info0 = "Temperature";
        YAxis_Info0_unit = "°C";
        YAxis_Info1 = "Humidity";
        YAxis_Info1_unit = "RH%";
        YAxis_Info = new QString[2];
        YAxis_Info[0] = "Temperture(°C)";
        YAxis_Info[1] = "Humidity(RH%)";
        //数据组数
        numOfDataGroup = 2;
        Track_Info = new QString[numOfDataGroup];
        Track_Info[0] = "Temperature(°C)";
        Track_Info[1] = "Humidity(RH%)";
        //获取时间
        haveStratTime = true;
        int year[2];
        year[0] = map.value(12);
        year[1] = map.value(13);
        startTime[0] = (year[1]<<8)+year[0];
        startTime[1] = map.value(14);
        startTime[2] = map.value(16);
        startTime[3] = map.value(18);
        startTime[4] = map.value(20);
        startTime[5] = map.value(22);
        //获取数据频率
        int dataFrequencyTemp[2];
        dataFrequencyTemp[0] = map.value(24);
        dataFrequencyTemp[1] = map.value(25);
        dataFrequency = (dataFrequencyTemp[1]<<8)+dataFrequencyTemp[0];
        //获取起始位
        startBit = 28;
        //解析数据
        int bit = startBit;
        short dataTemp[2];
        QMap<long long,int>::Iterator iter = map.end();
        iter--;
        int index = 0;
        short actualdata;
        float relitydata;
        while(bit<=iter.key())
        {
            dataTemp[0] = map.value(bit++);
            dataTemp[1] = map.value(bit++);
            actualdata = (dataTemp[1]<<8)+dataTemp[0];
            relitydata = actualdata*1.0/100*1.0;
            Processed_Data.insert(index++,relitydata);
        }
        break;
    }
    case 1:
    {
        //Y轴内容
        numOfYAxis = 2;
        YAxis_Info0 = "Temperature";
        YAxis_Info0_unit = "°C";
        YAxis_Info1 = "Pressure";
        YAxis_Info1_unit = "kg/cm^2";
        YAxis_Info = new QString[2];
        YAxis_Info[0] = "Temperture(°C)";
        YAxis_Info[1] = "Pressure(kg/cm^2)";
        //数据组数
        numOfDataGroup = 2;
        Track_Info = new QString[numOfDataGroup];
        Track_Info[0] = "Temperature(°C)";
        Track_Info[1] = "Pressure(kg/cm^2)";
        //获取时间
        haveStratTime = true;
        int year[2];
        year[0] = map.value(12);
        year[1] = map.value(13);
        startTime[0] = (year[1]<<8)+year[0];
        startTime[1] = map.value(14);
        startTime[2] = map.value(16);
        startTime[3] = map.value(18);
        startTime[4] = map.value(20);
        startTime[5] = map.value(22);
        //获取数据频率
        int dataFrequencyTemp[2];
        dataFrequencyTemp[0] = map.value(24);
        dataFrequencyTemp[1] = map.value(25);
        dataFrequency = (dataFrequencyTemp[1]<<8)+dataFrequencyTemp[0];
        //获取起始位
        startBit = 28;
        //解析数据
        int bit = startBit;
        short dataTemp[2];
        QMap<long long,int>::Iterator iter = map.end();
        iter--;
        int index = 0;
        short actualdata;
        float relitydata;
        while(bit<=iter.key())
        {
            dataTemp[0] = map.value(bit++);
            dataTemp[1] = map.value(bit++);
            actualdata = (dataTemp[1]<<8)+dataTemp[0];
            relitydata = actualdata*1.0/100*1.0;
            Processed_Data.insert(index++,relitydata);
        }
        break;
    }
    case 2:
    {
        //Y轴内容
        numOfYAxis = 1;
        YAxis_Info0 = "Temperature";
        YAxis_Info0_unit = "°C";
        YAxis_Info = new QString[1];
        YAxis_Info[0] = "Temperture(°C)";
        //数据组数
        numOfDataGroup = 1;
        Track_Info = new QString[numOfDataGroup];
        Track_Info[0] = "Temperature(°C)";
        //获取时间
        haveStratTime = true;
        int year[2];
        year[0] = map.value(12);
        year[1] = map.value(13);
        startTime[0] = (year[1]<<8)+year[0];
        startTime[1] = map.value(14);
        startTime[2] = map.value(16);
        startTime[3] = map.value(18);
        startTime[4] = map.value(20);
        startTime[5] = map.value(22);
        //获取数据频率
        int dataFrequencyTemp[2];
        dataFrequencyTemp[0] = map.value(24);
        dataFrequencyTemp[1] = map.value(25);
        dataFrequency = (dataFrequencyTemp[1]<<8)+dataFrequencyTemp[0];
        //获取起始位
        startBit = 28;
        //解析数据
        int bit = startBit;
        short dataTemp[4];
        QMap<long long,int>::Iterator iter = map.end();
        iter--;
        int index = 0;
        short actualdata;
        float relitydata;
        while(bit<=iter.key())
        {
            dataTemp[0] = map.value(bit++);
            dataTemp[1] = map.value(bit++);
            dataTemp[2] = map.value(bit++);
            dataTemp[3] = map.value(bit++);
            actualdata = (dataTemp[3]<<24)+(dataTemp[2]<<16)+(dataTemp[1]<<8)+dataTemp[0];
            relitydata = actualdata*1.0/100*1.0;
            Processed_Data.insert(index++,relitydata);
        }
        break;
    }
    case 3:
    {
        qDebug()<<" 警告未知dat类型！ ";
    }
    }
    return;
}

void Data_Reader::dealType3()
{
    qDebug()<<"警告未知文件后缀!";
}

void Data_Reader::dealType4()
{
    if(map.isEmpty())
        readToMap();
    getType0StartBit();
    //Y轴内容
    numOfYAxis = 2;
    YAxis_Info = new QString[2];
    YAxis_Info[0] = "Temperture(°C)";
    YAxis_Info[0] = "Humidity(RH%)";
    //数据组数
    numOfDataGroup = 7;
    Track_Info = new QString[numOfDataGroup];
    Track_Info[0] = "Temperature SV(°C)";
    Track_Info[1] = "Temperature PV(°C)";
    Track_Info[2] = "Humidity SV(RH%)";
    Track_Info[3] = "Humidity PV(RH%)";
    Track_Info[4] = "Temp PID(%)";
    Track_Info[5] = "Humi PID(%)";
    Track_Info[6] = "Svr PID(%)";
    //获取时间
    haveStratTime = true;
    short year[2];
    year[0] = map.value(16);
    year[1] = map.value(17);
    startTime[0] = (year[1]<<8)+year[0];
    startTime[1] = map.value(18);
    startTime[2] = map.value(19);
    startTime[3] = map.value(20);
    startTime[4] = map.value(21);
    startTime[5] = map.value(22);
    //Y轴最大值
    int temp[4];
    temp[0] = map.value(208);
    temp[1] = map.value(209);
    temp[2] = map.value(210);
    temp[3] = map.value(211);
    TDT_Y_MAX = (temp[3]<<24)+(temp[2]<<16)+(temp[1]<<8)+temp[0];
    //Y轴最小值
    int temp_[4];
    temp_[0] = map.value(212);
    temp_[1] = map.value(213);
    temp_[2] = map.value(214);
    temp_[3] = map.value(215);
    TDT_Y_MIN = (temp_[3]<<24)+(temp_[2]<<16)+(temp_[1]<<8)+temp_[0];
    //数据倍数
    int _temp[4];
    _temp[0] = map.value(216);
    _temp[1] = map.value(217);
    _temp[2] = map.value(218);
    _temp[3] = map.value(219);
    TDT_Multiple = (_temp[3]<<24)+(_temp[2]<<16)+(_temp[1]<<8)+_temp[0];
    TDT_Y_MAX /=TDT_Multiple;
    TDT_Y_MIN /=TDT_Multiple;
    //获取数据频率
    dataFrequency = map.value(23);
    //解析数据
    int bit = startBit;
    short dataTemp[2];
    QMap<long long,int>::Iterator iter = map.end();
    iter--;
    short actualdata;
    float relitydata;
    int index = 0;
    int index_ = 0;
    while(bit<=iter.key())
    {
        if((iter.key()-bit)>=512)
        {
            for(int i=0 ; i<36 ; i++)
            {
                Processed_Time.insert(index_,dataFrequency*index_);
                index_++;
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0/100*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0/100*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0/10*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0/10*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0;
                Processed_Data.insert(index++,relitydata);
            }
            bit+=8;
        }
        else
        {
            while((iter.key()-bit)>14)
            {
                Processed_Time.insert(index_,dataFrequency*index_);
                index_++;
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0/100*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0/100*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0/10*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0/10*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0;
                Processed_Data.insert(index++,relitydata);
                dataTemp[0] = map.value(bit++);
                dataTemp[1] = map.value(bit++);
                actualdata = (dataTemp[1]<<8)+dataTemp[0];
                relitydata = actualdata*1.0;
                Processed_Data.insert(index++,relitydata);
            }
            break;
        }
    }
}

QString Data_Reader::getFilePath()
{
    return filePath;
}
