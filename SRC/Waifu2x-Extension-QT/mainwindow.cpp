﻿/*
    Copyright (C) 2020  Aaron Feng

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    My Github homepage: https://github.com/AaronFeng753
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaTypeStreamOperators<QList_QMap_QStrQStr >("QList_QMap_QStrQStr");
    //==============
    this->setWindowTitle("Waifu2x-Extension-GUI "+VERSION+" by Aaron Feng");
    //==============
    translator = new QTranslator(this);
    //==============
    ui->tabWidget->setCurrentIndex(1);//显示home tab
    on_tabWidget_currentChanged(1);
    ui->tabWidget_Engines->setCurrentIndex(0);
    this->setAcceptDrops(true);//mainwindow接收drop
    Init_Table();//初始化table
    ui->groupBox_CurrentFile->setVisible(0);//隐藏当前文件进度
    ui->pushButton_Stop->setVisible(0);
    ui->pushButton_ForceRetry->setVisible(0);
    ui->progressBar_CompatibilityTest->setVisible(0);
    //=================== 初始隐藏所有table和按钮 ======================
    ui->tableView_image->setVisible(0);
    ui->tableView_gif->setVisible(0);
    ui->tableView_video->setVisible(0);
    ui->pushButton_ClearList->setVisible(0);
    ui->pushButton_RemoveItem->setVisible(0);
    Table_FileCount_reload();//重载文件列表下的文件数量统计
    //==============
    Init_ActionsMenu_checkBox_ReplaceOriginalFile();//第一次初始化[替换原文件]的右键菜单(需要在载入设定前设置为checkable
    //===========================================
    connect(this, SIGNAL(Send_Add_progressBar_CompatibilityTest()), this, SLOT(Add_progressBar_CompatibilityTest()));
    connect(this, SIGNAL(Send_ListGPUs_Anime4k_Finished(QString)), this, SLOT(ListGPUs_Anime4k_Finished(QString)));
    connect(this, SIGNAL(Send_Unable2Connect_RawGithubusercontentCom()), this, SLOT(Unable2Connect_RawGithubusercontentCom()));
    connect(this, SIGNAL(Send_SetEnable_pushButton_ForceRetry_self()), this, SLOT(SetEnable_pushButton_ForceRetry_self()));
    connect(this, SIGNAL(Send_SystemTray_NewMessage(QString)), this, SLOT(SystemTray_NewMessage(QString)));
    connect(this, SIGNAL(Send_PrograssBar_Range_min_max(int, int)), this, SLOT(progressbar_setRange_min_max(int, int)));
    connect(this, SIGNAL(Send_progressbar_Add()), this, SLOT(progressbar_Add()));
    connect(this, SIGNAL(Send_Table_image_ChangeStatus_rowNumInt_statusQString(int, QString)), this, SLOT(Table_image_ChangeStatus_rowNumInt_statusQString(int, QString)));
    connect(this, SIGNAL(Send_Table_gif_ChangeStatus_rowNumInt_statusQString(int, QString)), this, SLOT(Table_gif_ChangeStatus_rowNumInt_statusQString(int, QString)));
    connect(this, SIGNAL(Send_Table_video_ChangeStatus_rowNumInt_statusQString(int, QString)), this, SLOT(Table_video_ChangeStatus_rowNumInt_statusQString(int, QString)));
    connect(this, SIGNAL(Send_Table_FileCount_reload()), this, SLOT(Table_FileCount_reload()));
    connect(this, SIGNAL(Send_Table_image_insert_fileName_fullPath(QString,QString)), this, SLOT(Table_image_insert_fileName_fullPath(QString,QString)));
    connect(this, SIGNAL(Send_Table_gif_insert_fileName_fullPath(QString,QString)), this, SLOT(Table_gif_insert_fileName_fullPath(QString,QString)));
    connect(this, SIGNAL(Send_Table_video_insert_fileName_fullPath(QString,QString)), this, SLOT(Table_video_insert_fileName_fullPath(QString,QString)));
    connect(this, SIGNAL(Send_Table_image_CustRes_rowNumInt_HeightQString_WidthQString(int,QString,QString)), this, SLOT(Table_image_CustRes_rowNumInt_HeightQString_WidthQString(int,QString,QString)));
    connect(this, SIGNAL(Send_Table_gif_CustRes_rowNumInt_HeightQString_WidthQString(int,QString,QString)), this, SLOT(Table_gif_CustRes_rowNumInt_HeightQString_WidthQString(int,QString,QString)));
    connect(this, SIGNAL(Send_Table_video_CustRes_rowNumInt_HeightQString_WidthQString(int,QString,QString)), this, SLOT(Table_video_CustRes_rowNumInt_HeightQString_WidthQString(int,QString,QString)));
    connect(this, SIGNAL(Send_Table_Read_Saved_Table_Filelist_Finished()), this, SLOT(Table_Read_Saved_Table_Filelist_Finished()));
    connect(this, SIGNAL(Send_Table_Save_Current_Table_Filelist_Finished()), this, SLOT(Table_Save_Current_Table_Filelist_Finished()));
    connect(this, SIGNAL(Send_Waifu2x_Finished()), this, SLOT(Waifu2x_Finished()));
    connect(this, SIGNAL(Send_Waifu2x_Finished_manual()), this, SLOT(Waifu2x_Finished_manual()));
    connect(this, SIGNAL(Send_TextBrowser_NewMessage(QString)), this, SLOT(TextBrowser_NewMessage(QString)));
    connect(this, SIGNAL(Send_Waifu2x_Compatibility_Test_finished()), this, SLOT(Waifu2x_Compatibility_Test_finished()));
    connect(this, SIGNAL(Send_Waifu2x_DetectGPU_finished()), this, SLOT(Waifu2x_DetectGPU_finished()));
    connect(this, SIGNAL(Send_Realsr_ncnn_vulkan_DetectGPU_finished()), this, SLOT(Realsr_ncnn_vulkan_DetectGPU_finished()));
    connect(this, SIGNAL(Send_CheckUpadte_NewUpdate(QString,QString)), this, SLOT(CheckUpadte_NewUpdate(QString,QString)));
    connect(this, SIGNAL(Send_SystemShutDown()), this, SLOT(SystemShutDown()));
    connect(this, SIGNAL(Send_Waifu2x_DumpProcessorList_converter_finished()), this, SLOT(Waifu2x_DumpProcessorList_converter_finished()));
    connect(this, SIGNAL(Send_Read_urls_finfished()), this, SLOT(Read_urls_finfished()));
    connect(this, SIGNAL(Send_SRMD_DetectGPU_finished()), this, SLOT(SRMD_DetectGPU_finished()));
    connect(this, SIGNAL(Send_video_write_VideoConfiguration(QString,int,int,bool,int,int,QString,bool,QString,QString)), this, SLOT(video_write_VideoConfiguration(QString,int,int,bool,int,int,QString,bool,QString,QString)));
    connect(this, SIGNAL(Send_Settings_Save()), this, SLOT(Settings_Save()));
    connect(this, SIGNAL(Send_video_write_Progress_ProcessBySegment(QString,int,bool,bool,int)), this, SLOT(video_write_Progress_ProcessBySegment(QString,int,bool,bool,int)));
    connect(this, SIGNAL(Send_Donate_ReplaceQRCode(QString)), this, SLOT(Donate_ReplaceQRCode(QString)));
    connect(this, SIGNAL(Send_CurrentFileProgress_Start(QString,int)), this, SLOT(CurrentFileProgress_Start(QString,int)));
    connect(this, SIGNAL(Send_CurrentFileProgress_Stop()), this, SLOT(CurrentFileProgress_Stop()));
    connect(this, SIGNAL(Send_CurrentFileProgress_progressbar_Add()), this, SLOT(CurrentFileProgress_progressbar_Add()));
    connect(this, SIGNAL(Send_CurrentFileProgress_progressbar_Add_SegmentDuration(int)), this, SLOT(CurrentFileProgress_progressbar_Add_SegmentDuration(int)));
    //======
    TimeCostTimer = new QTimer();
    connect(TimeCostTimer, SIGNAL(timeout()), this, SLOT(TimeSlot()));
    //==================================================
    Settings_Read_Apply();//读取与应用设置
    //=====================================
    Set_Font_fixed();//固定字体
    //=====================================
    QtConcurrent::run(this, &MainWindow::DeleteErrorLog_Waifu2xCaffe);//删除Waifu2xCaffe生成的错误日志
    QtConcurrent::run(this, &MainWindow::Del_TempBatFile);//删除bat文件缓存
    AutoUpdate = QtConcurrent::run(this, &MainWindow::CheckUpadte_Auto);//自动检查更新线程
    DownloadOnlineQRCode = QtConcurrent::run(this, &MainWindow::Donate_DownloadOnlineQRCode);//在线更新捐赠二维码
    SystemShutDown_isAutoShutDown();//上次是否自动关机
    //====================================
    TextBrowser_StartMes();//显示启动msg
    //===================================
    Tip_FirstTimeStart();
    //==============
    /*
    校验软件是否对所在目录有写权限
    */
    if(file_isDirWritable(Current_Path)==false)
    {
        QMessageBox Msg(QMessageBox::Question, QString(tr("Error")), QString(tr("It is detected that this software lacks the necessary permissions to run.\n\nPlease close this software and start this software again after giving this software administrator permission.\n\nOtherwise, this software may not work properly.")));
        Msg.setIcon(QMessageBox::Warning);
        Msg.addButton(QString("OK"), QMessageBox::NoRole);
        Msg.exec();
    }
    //==============
    Init_SystemTrayIcon();//初始化托盘图标
    Init_ActionsMenu_lineEdit_outputPath();//初始化 输出路径 lineEDIT的右键菜单
    Init_ActionsMenu_FilesList();
    Init_ActionsMenu_pushButton_RemoveItem();
    Init_ActionsMenu_checkBox_ReplaceOriginalFile();//第二次初始化[替换原文件]的右键菜单(载入语言设置
    //==============
    this->showNormal();
    this->activateWindow();
    this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    this->adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isAlreadyClosed)//解决QT退出时重复调用closeEvent的问题
    {
        event->accept();
        return;
    }
    //=============== 询问是否退出 =======================
    if(ui->checkBox_PromptWhenExit->isChecked())
    {
        QMessageBox Msg(QMessageBox::Question, QString(tr("Notification")), QString(tr("Do you really wanna exit Waifu2x-Extension-GUI ?")));
        Msg.setIcon(QMessageBox::Question);
        QAbstractButton *pYesBtn = Msg.addButton(QString(tr("YES")), QMessageBox::YesRole);
        QAbstractButton *pNoBtn = Msg.addButton(QString(tr("NO")), QMessageBox::NoRole);
        Msg.exec();
        if (Msg.clickedButton() == pNoBtn)
        {
            event->ignore();
            return;
        }
        if (Msg.clickedButton() == pYesBtn)isAlreadyClosed=true;
    }
    //=============================
    systemTray->hide();
    this->hide();
    QApplication::setQuitOnLastWindowClosed(true);//無窗口時不再保持運行
    QApplication::closeAllWindows();
    //====
    bool AutoSaveSettings = ui->checkBox_AutoSaveSettings->isChecked();
    if(AutoSaveSettings&&(!Settings_isReseted))
    {
        Settings_Save();
        QtConcurrent::run(this, &MainWindow::Auto_Save_Settings_Watchdog);
    }
    else
    {
        QProcess_stop=true;
        AutoUpdate.cancel();
        DownloadOnlineQRCode.cancel();
        Waifu2xMain.cancel();
        Force_close();
    }
}

int MainWindow::Auto_Save_Settings_Watchdog()
{
    QString settings_ini = Current_Path+"/settings.ini";
    while(!QFile::exists(settings_ini))
    {
        Delay_msec_sleep(250);
    }
    Delay_msec_sleep(500);
    //=====
    QProcess_stop=true;
    AutoUpdate.cancel();
    DownloadOnlineQRCode.cancel();
    Waifu2xMain.cancel();
    Force_close();
    //====
    return 0;
}

int MainWindow::Force_close()
{
    QProcess Close;
    //==============
    Close.start("taskkill /f /t /fi \"imagename eq Waifu2x-Extension-GUI.exe\"");
    Close.waitForStarted(10000);
    Close.waitForFinished(10000);
    //==============
    return 0;
}
/*
最小化
*/
void MainWindow::changeEvent(QEvent *e)
{
    if((e->type()==QEvent::WindowStateChange)&&this->isMinimized())
    {
        if(ui->checkBox_MinimizeToTaskbar->isChecked())
        {
            this->hide();
        }
    }
}

void MainWindow::TimeSlot()
{
    TimeCost++;
    //====================总进度==================
    QString TimeCostStr = tr("Time cost:[")+Seconds2hms(TimeCost)+"]";
    ui->label_TimeCost->setText(TimeCostStr);
    if(ui->label_TimeRemain_CurrentFile->isVisible())
    {
        int TaskNumFinished_tmp = TaskNumFinished;
        long unsigned int TimeCost_tmp = TimeCost;
        int TaskNumTotal_tmp = TaskNumTotal;
        if(TaskNumFinished_tmp>0&&TimeCost_tmp>0&&TaskNumTotal_tmp>0)
        {
            if(NewTaskFinished)
            {
                NewTaskFinished=false;
                long unsigned int avgTimeCost = TimeCost_tmp/TaskNumFinished_tmp;
                ETA = int(avgTimeCost*(TaskNumTotal_tmp-TaskNumFinished_tmp));
            }
            else
            {
                if(ETA>1)
                {
                    ETA--;
                }
            }
            QString TimeRemainingStr = tr("Time remaining:[")+Seconds2hms(ETA)+"]";
            ui->label_TimeRemain->setText(TimeRemainingStr);
            QDateTime time = QDateTime::currentDateTime();
            long unsigned int Time_t = time.toTime_t();
            Time_t+=ETA;
            time = QDateTime::fromTime_t(Time_t);
            QString Current_Time = time.toString("hh:mm:ss");
            QString ETA_str = "ETA:["+Current_Time+"]";
            ui->label_ETA->setText(ETA_str);
        }
    }
    //====================当前文件=================
    if(isStart_CurrentFile)
    {
        TimeCost_CurrentFile++;
        QString TimeCostStr_CurrentFile = tr("Time cost:[")+Seconds2hms(TimeCost_CurrentFile)+"]";
        ui->label_TimeCost_CurrentFile->setText(TimeCostStr_CurrentFile);
        int TaskNumFinished_tmp_CurrentFile = TaskNumFinished_CurrentFile;
        long unsigned int TimeCost_tmp_CurrentFile = TimeCost_CurrentFile;
        int TaskNumTotal_tmp_CurrentFile = TaskNumTotal_CurrentFile;
        if(TaskNumFinished_tmp_CurrentFile>0&&TimeCost_tmp_CurrentFile>0&&TaskNumTotal_tmp_CurrentFile>0)
        {
            if(NewTaskFinished_CurrentFile)
            {
                NewTaskFinished_CurrentFile=false;
                long unsigned int avgTimeCost_CurrentFile = TimeCost_tmp_CurrentFile/TaskNumFinished_tmp_CurrentFile;
                ETA_CurrentFile = int(avgTimeCost_CurrentFile*(TaskNumTotal_tmp_CurrentFile-TaskNumFinished_tmp_CurrentFile));
            }
            else
            {
                if(ETA_CurrentFile>1)
                {
                    ETA_CurrentFile--;
                }
            }
            QString TimeRemainingStr_CurrentFile = tr("Time remaining:[")+Seconds2hms(ETA_CurrentFile)+"]";
            ui->label_TimeRemain_CurrentFile->setText(TimeRemainingStr_CurrentFile);
            QDateTime time_CurrentFile = QDateTime::currentDateTime();
            long unsigned int Time_t_CurrentFile = time_CurrentFile.toTime_t();
            Time_t_CurrentFile+=ETA_CurrentFile;
            time_CurrentFile = QDateTime::fromTime_t(Time_t_CurrentFile);
            QString Current_Time_CurrentFile = time_CurrentFile.toString("hh:mm:ss");
            QString ETA_str_CurrentFile = "ETA:["+Current_Time_CurrentFile+"]";
            ui->label_ETA_CurrentFile->setText(ETA_str_CurrentFile);
        }
    }
}
QString MainWindow::Seconds2hms(long unsigned int seconds)
{
    if(seconds<=0)return "0:0:0";
    long unsigned int mm = 60;
    long unsigned int hh = mm * 60;
    long unsigned int hour = seconds / hh;
    long unsigned int min = (seconds-hour*hh)/mm;
    long unsigned int sec = seconds - hour*hh - min*mm;
    return QString::number(hour,10)+":"+QString::number(min,10)+":"+QString::number(sec,10);
}

void MainWindow::Set_Font_fixed()
{
    QFont font;
    if(ui->checkBox_isCustFontEnable->isChecked())
    {
        font = ui->fontComboBox_CustFont->currentFont();
        font.setPixelSize(ui->spinBox_GlobalFontSize->value());
    }
    else
    {
        font = qApp->font();
        font.setPixelSize(15);
    }
    qApp->setFont(font);
}

void MainWindow::on_pushButton_ClearList_clicked()
{
    curRow_image = -1;
    curRow_gif = -1;
    curRow_video = -1;
    Table_Clear();
    Custom_resolution_list.clear();
    ui->label_DropFile->setVisible(1);
    ui->tableView_gif->setVisible(0);
    ui->tableView_image->setVisible(0);
    ui->tableView_video->setVisible(0);
    ui->pushButton_ClearList->setVisible(0);
    ui->pushButton_RemoveItem->setVisible(0);
    Table_FileCount_reload();
    progressbar_clear();
}

void MainWindow::on_pushButton_Start_clicked()
{
    /*
     * 判断是否启用自定义输出路径
     * 判断输出路径是否合法
    */
    if(ui->checkBox_OutPath_isEnabled->isChecked())
    {
        QString tmp = ui->lineEdit_outputPath->text();
        tmp = tmp.trimmed();
        if(tmp=="")
        {
            //=====
            QMessageBox *MSG = new QMessageBox();
            MSG->setWindowTitle(tr("Error"));
            MSG->setText(tr("Output path is empty."));
            MSG->setIcon(QMessageBox::Warning);
            MSG->setModal(true);
            MSG->show();
            //=====
            emit Send_TextBrowser_NewMessage(tr("Output path is empty."));
            return;
        }
        tmp = tmp.replace("\\","/");
        tmp = tmp.replace("\\\\","/");
        tmp = tmp.replace("//","/");
        if(tmp.right(1)=="/")
        {
            tmp = tmp.left(tmp.length() - 1);
        }
        QFileInfo fileinfo_tmp(tmp);
        if(file_isDirExist(tmp)&&fileinfo_tmp.isDir()&&file_isDirWritable(tmp))
        {
            OutPutFolder_main = tmp;
        }
        else
        {
            //=====
            QMessageBox *MSG = new QMessageBox();
            MSG->setWindowTitle(tr("Error"));
            MSG->setText(tr("Invalid output path.\n\nOr do not have sufficient permissions to write files to the output path, you might need to grant administrator permissions to this software."));
            MSG->setIcon(QMessageBox::Warning);
            MSG->setModal(true);
            MSG->show();
            //=====
            emit Send_TextBrowser_NewMessage(tr("Invalid output path."));
            return;
        }
    }
    //=============================
    //      判断文件列表是否为空
    //=============================
    if(Table_model_image->rowCount()==0&&Table_model_gif->rowCount()==0&&Table_model_video->rowCount()==0)
    {
        //=====
        QMessageBox *MSG = new QMessageBox();
        MSG->setWindowTitle(tr("Error"));
        MSG->setText(tr("Unable to start processing files: The file list is empty."));
        MSG->setIcon(QMessageBox::Warning);
        MSG->setModal(true);
        MSG->show();
        //=====
    }
    else
    {
        Table_ChangeAllStatusToWaiting();//将table内没开始处理或者被打断的条目状态变为等待中
        //============== 判断是否需要隐藏ETA和剩余时间 ================
        if(Table_model_gif->rowCount()!=0||Table_model_video->rowCount()!=0)
        {
            ui->label_TimeRemain->setVisible(0);
            ui->label_ETA->setVisible(0);
        }
        else
        {
            ui->label_TimeRemain->setVisible(1);
            ui->label_ETA->setVisible(1);
        }
        //============== 数值初始化 ==================
        waifu2x_STOP = false;
        waifu2x_STOP_confirm = false;
        ThreadNumMax = 0;
        ThreadNumRunning = 0;
        Progressbar_MaxVal = 0;
        Progressbar_CurrentVal = 0;
        TaskNumTotal=0;
        TaskNumFinished=0;
        NewTaskFinished=false;
        ETA=0;
        isForceRetryClicked=false;
        //============== 界面初始化 ======================
        RemoveFile_FilesList->setEnabled(0);
        this->setAcceptDrops(0);//禁止drop file
        ui->pushButton_Stop->setVisible(1);//启用stop button
        ui->pushButton_Start->setVisible(0);//禁用start button
        ui->groupBox_OutPut->setEnabled(0);
        ui->pushButton_ClearList->setEnabled(0);
        ui->pushButton_RemoveItem->setEnabled(0);
        ui->groupBox_Engine->setEnabled(0);
        ui->groupBox_ScaleRaton_DenoiseLevel->setEnabled(0);
        ui->checkBox_OptGIF->setEnabled(0);
        ui->checkBox_SaveAsJPG->setEnabled(0);
        ui->checkBox_CompressJPG->setEnabled(0);
        ui->checkBox_DelOriginal->setEnabled(0);
        ui->checkBox_ReProcFinFiles->setEnabled(0);
        ui->pushButton_compatibilityTest->setEnabled(0);
        ui->pushButton_CustRes_cancel->setEnabled(0);
        ui->pushButton_CustRes_apply->setEnabled(0);
        ui->pushButton_ReadFileList->setEnabled(0);
        ui->comboBox_AspectRatio_custRes->setEnabled(0);
        ui->spinBox_JPGCompressedQuality->setEnabled(0);
        progressbar_clear();
        ui->label_TimeCost->setText(tr("Time cost:NULL"));
        ui->label_ETA->setText(tr("ETA:NULL"));
        ui->label_TimeRemain->setText(tr("Time remaining:NULL"));
        ui->groupBox_video_settings->setEnabled(0);
        ui->checkBox_Move2RecycleBin->setEnabled(0);
        ui->pushButton_ForceRetry->setVisible(1);
        ui->checkBox_AutoDetectAlphaChannel->setEnabled(0);
        ui->groupBox_AudioDenoise->setEnabled(0);
        ui->checkBox_ProcessVideoBySegment->setEnabled(0);
        ui->spinBox_SegmentDuration->setEnabled(0);
        ui->comboBox_ImageStyle->setEnabled(0);
        ui->comboBox_ImageStyle_Waifu2xCaffe->setEnabled(0);
        ui->checkBox_PreProcessImage->setEnabled(0);
        on_lineEdit_GPUs_Anime4k_editingFinished();
        on_lineEdit_MultiGPUInfo_Waifu2xCaffe_editingFinished();
        ui->checkBox_ReplaceOriginalFile->setEnabled(0);
        //==========
        TimeCost=0;
        TimeCostTimer->start(1000);
        emit Send_TextBrowser_NewMessage(tr("Start processing files."));
        Waifu2xMain = QtConcurrent::run(this, &MainWindow::Waifu2xMainThread);//启动waifu2x 主线程
    }
}
/*
停止处理键
*/
void MainWindow::on_pushButton_Stop_clicked()
{
    TimeCostTimer->stop();
    ui->pushButton_Stop->setVisible(0);//隐藏stop button
    waifu2x_STOP = true;
    emit TextBrowser_NewMessage(tr("Trying to stop, please wait..."));
    QFuture<void> f1 = QtConcurrent::run(this, &MainWindow::Wait_waifu2x_stop);
}
/*
等待处理线程完全停止
*/
void MainWindow::Wait_waifu2x_stop()
{
    while(true)
    {
        if(waifu2x_STOP_confirm||ThreadNumRunning==0)
        {
            waifu2x_STOP_confirm = false;
            emit TextBrowser_NewMessage(tr("Processing of files has stopped."));
            QtConcurrent::run(this, &MainWindow::Play_NFSound);//成功暂停,播放提示音
            break;
        }
        Delay_msec_sleep(300);
    }
    Waifu2xMain.cancel();
    Waifu2xMain.waitForFinished();
    emit Send_Waifu2x_Finished_manual();
}
/*
从tableview移除item
*/
int MainWindow::on_pushButton_RemoveItem_clicked()
{
    ui->pushButton_RemoveItem->setEnabled(0);
    //===========
    if(curRow_image==-1&&curRow_video==-1&&curRow_gif==-1)
    {
        ui->tableView_image->clearSelection();
        ui->tableView_gif->clearSelection();
        ui->tableView_video->clearSelection();
        //=====
        QMessageBox *MSG = new QMessageBox();
        MSG->setWindowTitle(tr("Warning"));
        MSG->setText(tr("No items are currently selected."));
        MSG->setIcon(QMessageBox::Warning);
        MSG->setModal(true);
        MSG->show();
        //=====
        ui->pushButton_RemoveItem->setEnabled(1);
        return 0;
    }
    //==========================
    if(curRow_image >= 0)
    {
        CustRes_remove(Table_model_image->item(curRow_image,2)->text());
        ui->tableView_image->setUpdatesEnabled(false);
        Table_model_image->removeRow(curRow_image);
        ui->tableView_image->setUpdatesEnabled(true);
        curRow_image = -1;
        ui->tableView_image->clearSelection();
    }
    //============================================================
    if(curRow_video >= 0)
    {
        CustRes_remove(Table_model_video->item(curRow_video,2)->text());
        ui->tableView_video->setUpdatesEnabled(false);
        Table_model_video->removeRow(curRow_video);
        ui->tableView_video->setUpdatesEnabled(true);
        curRow_video = -1;
        ui->tableView_video->clearSelection();
    }
    //============================================================
    if(curRow_gif >= 0)
    {
        CustRes_remove(Table_model_gif->item(curRow_gif,2)->text());
        ui->tableView_gif->setUpdatesEnabled(false);
        Table_model_gif->removeRow(curRow_gif);
        ui->tableView_gif->setUpdatesEnabled(true);
        curRow_gif = -1;
        ui->tableView_gif->clearSelection();
    }
    //==================================================
    if(Table_model_gif->rowCount()==0)
    {
        ui->tableView_gif->setVisible(0);
    }
    if(Table_model_image->rowCount()==0)
    {
        ui->tableView_image->setVisible(0);
    }
    if(Table_model_video->rowCount()==0)
    {
        ui->tableView_video->setVisible(0);
    }
    if(Table_model_gif->rowCount()==0&&Table_model_image->rowCount()==0&&Table_model_video->rowCount()==0)
    {
        on_pushButton_ClearList_clicked();
    }
    Table_FileCount_reload();
    //============
    ui->pushButton_RemoveItem->setEnabled(1);
    return 0;
}

//==========================================================
/*
============= 安全的阻塞延时 =====================
*/
void MainWindow::Delay_sec_sleep(int time)
{
    QThread::sleep(time);
}

void MainWindow::Delay_msec_sleep(int time)
{
    QThread::msleep(time);
}
//==========================================================

/*
播放提示音
*/
void MainWindow::Play_NFSound()
{
    if(ui->checkBox_NfSound->isChecked()==false)return;
    //====
    QString NFSound = Current_Path+"/NFSound_Waifu2xEX.mp3";
    if(QFile::exists(NFSound)==false)
    {
        emit Send_TextBrowser_NewMessage(tr("Error! Notification sound file is missing!"));
        return;
    }
    //====
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(NFSound));
    player->play();
}


void MainWindow::on_checkBox_SaveAsJPG_stateChanged(int arg1)
{
    if(ui->checkBox_SaveAsJPG->isChecked())
    {
        ui->checkBox_CompressJPG->setEnabled(1);
        ui->checkBox_AutoDetectAlphaChannel->setEnabled(1);
    }
    else
    {
        ui->checkBox_CompressJPG->setChecked(0);
        ui->checkBox_CompressJPG->setEnabled(0);
        ui->checkBox_AutoDetectAlphaChannel->setEnabled(0);
        ui->checkBox_AutoDetectAlphaChannel->setChecked(1);
        ui->spinBox_JPGCompressedQuality->setEnabled(0);
    }
}

void MainWindow::on_pushButton_Report_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/AaronFeng753/Waifu2x-Extension-GUI/issues/new"));
}

void MainWindow::on_pushButton_ReadMe_clicked()
{
    if(ui->comboBox_language->currentIndex()==1)
    {
        QDesktopServices::openUrl(QUrl("https://github.com/AaronFeng753/Waifu2x-Extension-GUI/"));
        QDesktopServices::openUrl(QUrl("https://gitee.com/aaronfeng0711/Waifu2x-Extension-GUI/"));
    }
    else
    {
        QDesktopServices::openUrl(QUrl("https://github.com/AaronFeng753/Waifu2x-Extension-GUI/"));
    }
}



void MainWindow::on_comboBox_Engine_Image_currentIndexChanged(int index)
{
    switch(ui->comboBox_Engine_Image->currentIndex())
    {
        case 0:
            {
                ui->spinBox_DenoiseLevel_image->setRange(-1,3);
                ui->spinBox_DenoiseLevel_image->setValue(2);
                ui->spinBox_DenoiseLevel_image->setEnabled(1);
                ui->spinBox_DenoiseLevel_image->setToolTip(tr("Range:-1(No noise reduction)~3"));
                ui->label_ImageDenoiseLevel->setToolTip(tr("Range:-1(No noise reduction)~3"));
                break;
            }
        case 1:
            {
                ui->spinBox_DenoiseLevel_image->setRange(0,3);
                ui->spinBox_DenoiseLevel_image->setValue(2);
                ui->spinBox_DenoiseLevel_image->setEnabled(1);
                ui->spinBox_DenoiseLevel_image->setToolTip(tr("Range:0(No noise reduction)~3"));
                ui->label_ImageDenoiseLevel->setToolTip(tr("Range:0(No noise reduction)~3"));
                break;
            }
        case 2:
            {
                ui->spinBox_DenoiseLevel_image->setRange(-1,10);
                ui->spinBox_DenoiseLevel_image->setValue(4);
                ui->spinBox_DenoiseLevel_image->setEnabled(1);
                ui->spinBox_DenoiseLevel_image->setToolTip(tr("Range:-1(No noise reduction)~10"));
                ui->label_ImageDenoiseLevel->setToolTip(tr("Range:-1(No noise reduction)~10"));
                break;
            }
        case 3:
            {
                DenoiseLevelSpinboxSetting_Anime4k();
                break;
            }
        case 4:
            {
                ui->spinBox_DenoiseLevel_image->setRange(-1,3);
                ui->spinBox_DenoiseLevel_image->setValue(2);
                ui->spinBox_DenoiseLevel_image->setEnabled(1);
                ui->spinBox_DenoiseLevel_image->setToolTip(tr("Range:-1(No noise reduction)~3"));
                ui->label_ImageDenoiseLevel->setToolTip(tr("Range:-1(No noise reduction)~3"));
                break;
            }
        case 5:
            {
                ui->spinBox_DenoiseLevel_image->setRange(0,1);
                ui->spinBox_DenoiseLevel_image->setValue(1);
                ui->spinBox_DenoiseLevel_image->setEnabled(0);
                ui->spinBox_DenoiseLevel_image->setToolTip(tr("Realsr-ncnn-vulkan engine will denoise automatically."));
                ui->label_ImageDenoiseLevel->setToolTip(tr("Realsr-ncnn-vulkan engine will denoise automatically."));
                break;
            }
    }
    isShowAnime4kWarning=true;
    on_comboBox_model_vulkan_currentIndexChanged(0);
    if(isWaifu2xCaffeEnabled())
    {
        ui->comboBox_ImageStyle_Waifu2xCaffe->setEnabled(1);
    }
    else
    {
        ui->comboBox_ImageStyle_Waifu2xCaffe->setEnabled(0);
    }
}

void MainWindow::on_comboBox_Engine_GIF_currentIndexChanged(int index)
{
    switch(ui->comboBox_Engine_GIF->currentIndex())
    {
        case 0:
            {
                ui->spinBox_DenoiseLevel_gif->setRange(-1,3);
                ui->spinBox_DenoiseLevel_gif->setValue(2);
                ui->spinBox_DenoiseLevel_gif->setEnabled(1);
                ui->spinBox_DenoiseLevel_gif->setToolTip(tr("Range:-1(No noise reduction)~3"));
                ui->label_GIFDenoiseLevel->setToolTip(tr("Range:-1(No noise reduction)~3"));
                break;
            }
        case 1:
            {
                ui->spinBox_DenoiseLevel_gif->setRange(0,3);
                ui->spinBox_DenoiseLevel_gif->setValue(2);
                ui->spinBox_DenoiseLevel_gif->setEnabled(1);
                ui->spinBox_DenoiseLevel_gif->setToolTip(tr("Range:0(No noise reduction)~3"));
                ui->label_GIFDenoiseLevel->setToolTip(tr("Range:0(No noise reduction)~3"));
                break;
            }
        case 2:
            {
                ui->spinBox_DenoiseLevel_gif->setRange(-1,10);
                ui->spinBox_DenoiseLevel_gif->setValue(4);
                ui->spinBox_DenoiseLevel_gif->setEnabled(1);
                ui->spinBox_DenoiseLevel_gif->setToolTip(tr("Range:-1(No noise reduction)~10"));
                ui->label_GIFDenoiseLevel->setToolTip(tr("Range:-1(No noise reduction)~10"));
                break;
            }
        case 3:
            {
                DenoiseLevelSpinboxSetting_Anime4k();
                break;
            }
        case 4:
            {
                ui->spinBox_DenoiseLevel_gif->setRange(-1,3);
                ui->spinBox_DenoiseLevel_gif->setValue(2);
                ui->spinBox_DenoiseLevel_gif->setEnabled(1);
                ui->spinBox_DenoiseLevel_gif->setToolTip(tr("Range:-1(No noise reduction)~3"));
                ui->label_GIFDenoiseLevel->setToolTip(tr("Range:-1(No noise reduction)~3"));
                break;
            }
        case 5:
            {
                ui->spinBox_DenoiseLevel_gif->setRange(0,1);
                ui->spinBox_DenoiseLevel_gif->setValue(1);
                ui->spinBox_DenoiseLevel_gif->setEnabled(0);
                ui->spinBox_DenoiseLevel_gif->setToolTip(tr("Realsr-ncnn-vulkan engine will denoise automatically."));
                ui->label_GIFDenoiseLevel->setToolTip(tr("Realsr-ncnn-vulkan engine will denoise automatically."));
                break;
            }
    }
    on_comboBox_model_vulkan_currentIndexChanged(0);
    if(isWaifu2xCaffeEnabled())
    {
        ui->comboBox_ImageStyle_Waifu2xCaffe->setEnabled(1);
    }
    else
    {
        ui->comboBox_ImageStyle_Waifu2xCaffe->setEnabled(0);
    }
}

void MainWindow::on_comboBox_Engine_Video_currentIndexChanged(int index)
{
    switch(ui->comboBox_Engine_Video->currentIndex())
    {
        case 0:
            {
                ui->spinBox_DenoiseLevel_video->setRange(-1,3);
                ui->spinBox_DenoiseLevel_video->setValue(2);
                ui->spinBox_DenoiseLevel_video->setEnabled(1);
                ui->spinBox_DenoiseLevel_video->setToolTip(tr("Range:-1(No noise reduction)~3"));
                ui->label_VideoDenoiseLevel->setToolTip(tr("Range:-1(No noise reduction)~3"));
                break;
            }
        case 1:
            {
                ui->spinBox_DenoiseLevel_video->setRange(0,3);
                ui->spinBox_DenoiseLevel_video->setValue(2);
                ui->spinBox_DenoiseLevel_video->setEnabled(1);
                ui->spinBox_DenoiseLevel_video->setToolTip(tr("Range:0(No noise reduction)~3"));
                ui->label_VideoDenoiseLevel->setToolTip(tr("Range:0(No noise reduction)~3"));
                break;
            }
        case 2:
            {
                DenoiseLevelSpinboxSetting_Anime4k();
                break;
            }
        case 3:
            {
                ui->spinBox_DenoiseLevel_video->setRange(-1,10);
                ui->spinBox_DenoiseLevel_video->setValue(4);
                ui->spinBox_DenoiseLevel_video->setEnabled(1);
                ui->spinBox_DenoiseLevel_video->setToolTip(tr("Range:-1(No noise reduction)~10"));
                ui->label_VideoDenoiseLevel->setToolTip(tr("Range:-1(No noise reduction)~10"));
                break;
            }
        case 4:
            {
                ui->spinBox_DenoiseLevel_video->setRange(-1,3);
                ui->spinBox_DenoiseLevel_video->setValue(2);
                ui->spinBox_DenoiseLevel_video->setEnabled(1);
                ui->spinBox_DenoiseLevel_video->setToolTip(tr("Range:-1(No noise reduction)~3"));
                ui->label_VideoDenoiseLevel->setToolTip(tr("Range:-1(No noise reduction)~3"));
                break;
            }
        case 5:
            {
                ui->spinBox_DenoiseLevel_video->setRange(0,1);
                ui->spinBox_DenoiseLevel_video->setValue(1);
                ui->spinBox_DenoiseLevel_video->setEnabled(0);
                ui->spinBox_DenoiseLevel_video->setToolTip(tr("Realsr-ncnn-vulkan engine will denoise automatically."));
                ui->label_VideoDenoiseLevel->setToolTip(tr("Realsr-ncnn-vulkan engine will denoise automatically."));
                break;
            }
    }
    on_comboBox_model_vulkan_currentIndexChanged(0);
    if(isWaifu2xCaffeEnabled())
    {
        ui->comboBox_ImageStyle_Waifu2xCaffe->setEnabled(1);
    }
    else
    {
        ui->comboBox_ImageStyle_Waifu2xCaffe->setEnabled(0);
    }
}

void MainWindow::on_pushButton_clear_textbrowser_clicked()
{
    ui->textBrowser->clear();
    TextBrowser_StartMes();
}

void MainWindow::on_spinBox_textbrowser_fontsize_valueChanged(int arg1)
{
    int size = ui->spinBox_textbrowser_fontsize->value();
    ui->textBrowser->setStyleSheet("font: "+QString::number(size,10)+"pt \"Arial\";");
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::on_pushButton_CustRes_apply_clicked()
{
    ui->pushButton_CustRes_apply->setEnabled(0);
    CustRes_SetCustRes();
    ui->pushButton_CustRes_apply->setEnabled(1);
}

void MainWindow::on_pushButton_CustRes_cancel_clicked()
{
    ui->pushButton_CustRes_cancel->setEnabled(0);
    CustRes_CancelCustRes();
    ui->pushButton_CustRes_cancel->setEnabled(1);
}

void MainWindow::on_pushButton_HideSettings_clicked()
{
    if(ui->groupBox_Setting->isVisible())
    {
        ui->groupBox_Setting->setVisible(0);
        isSettingsHide=true;
        ui->pushButton_HideSettings->setText(tr("Show settings"));
    }
    else
    {
        ui->groupBox_Setting->setVisible(1);
        isSettingsHide=false;
        ui->pushButton_HideSettings->setText(tr("Hide settings"));
    }
}

/*
改变语言设置
*/
void MainWindow::on_comboBox_language_currentIndexChanged(int index)
{
    QString qmFilename;
    switch(ui->comboBox_language->currentIndex())
    {
        case 0:
            {
                qmFilename = Current_Path + "/language_English.qm";
                break;
            }
        case 1:
            {
                qmFilename = Current_Path + "/language_Chinese.qm";
                break;
            }
        case 2:
            {
                qmFilename = Current_Path + "/language_Japanese.qm";
                break;
            }
        case 3:
            {
                qmFilename = Current_Path + "/language_TraditionalChinese.qm";
                break;
            }
    }
    //判断文件是否存在
    if(QFile::exists(qmFilename)==false)
    {
        QMessageBox *MSG_languageFile404 = new QMessageBox();
        MSG_languageFile404->setWindowTitle(tr("Error"));
        MSG_languageFile404->setText(tr("Language file is missing, please reinstall this program."));
        MSG_languageFile404->setIcon(QMessageBox::Warning);
        MSG_languageFile404->setModal(true);
        MSG_languageFile404->show();
        return;
    }
    //加载语言文件
    if (translator->load(qmFilename))
    {
        qApp->installTranslator(translator);
        ui->retranslateUi(this);
        Table_FileCount_reload();
        Init_Table();
        Init_SystemTrayIcon();
        Set_Font_fixed();
        //=========
        if(ui->checkBox_AlwaysHideSettings->isChecked())
        {
            ui->groupBox_Setting->setVisible(0);
            isSettingsHide=true;
            ui->pushButton_HideSettings->setText(tr("Show settings"));
        }
        else
        {
            ui->groupBox_Setting->setVisible(1);
            isSettingsHide=false;
            ui->pushButton_HideSettings->setText(tr("Hide settings"));
        }
        //=========
        if(ui->checkBox_AlwaysHideTextBrowser->isChecked())
        {
            ui->textBrowser->setVisible(0);
            ui->pushButton_HideTextBro->setText(tr("Show Text Browser"));
        }
        else
        {
            ui->textBrowser->setVisible(1);
            ui->pushButton_HideTextBro->setText(tr("Hide Text Browser"));
        }
        //=========
        if(this->windowState()!=Qt::WindowMaximized)
        {
            this->adjustSize();
        }
    }
    else
    {
        QMessageBox *MSG_Unable2LoadLanguageFiles = new QMessageBox();
        MSG_Unable2LoadLanguageFiles->setWindowTitle(tr("Error"));
        MSG_Unable2LoadLanguageFiles->setText(tr("Language file cannot be loaded properly."));
        MSG_Unable2LoadLanguageFiles->setIcon(QMessageBox::Warning);
        MSG_Unable2LoadLanguageFiles->setModal(true);
        MSG_Unable2LoadLanguageFiles->show();
    }
}
void MainWindow::on_pushButton_SaveFileList_clicked()
{
    if(Table_model_video->rowCount()<=0&&Table_model_image->rowCount()<=0&&Table_model_gif->rowCount()<=0)
    {
        QMessageBox *MSG = new QMessageBox();
        MSG->setWindowTitle(tr("Error"));
        MSG->setText(tr("File list is empty!"));
        MSG->setIcon(QMessageBox::Warning);
        MSG->setModal(false);
        MSG->show();
        return;
    }
    this->setAcceptDrops(0);//禁止drop file
    ui->pushButton_Start->setEnabled(0);//禁用start button
    ui->pushButton_ClearList->setEnabled(0);
    ui->pushButton_RemoveItem->setEnabled(0);
    ui->checkBox_ReProcFinFiles->setEnabled(0);
    ui->pushButton_CustRes_cancel->setEnabled(0);
    ui->pushButton_CustRes_apply->setEnabled(0);
    ui->pushButton_ReadFileList->setEnabled(0);
    ui->pushButton_SaveFileList->setEnabled(0);
    emit Send_TextBrowser_NewMessage(tr("Write to the file, please wait."));
    Table_Save_Current_Table_Filelist();
    QtConcurrent::run(this, &MainWindow::Table_Save_Current_Table_Filelist_Watchdog);
}

void MainWindow::on_pushButton_ReadFileList_clicked()
{
    QString Table_FileList_ini = Current_Path+"/Table_FileList.ini";
    if(QFile::exists(Table_FileList_ini))
    {
        //===
        ui->pushButton_ReadFileList->setText(tr("Loading..."));
        //===
        this->setAcceptDrops(0);//禁止drop file
        ui->pushButton_Start->setEnabled(0);//禁用start button
        ui->pushButton_ClearList->setEnabled(0);
        ui->pushButton_RemoveItem->setEnabled(0);
        ui->checkBox_ReProcFinFiles->setEnabled(0);
        ui->pushButton_CustRes_cancel->setEnabled(0);
        ui->pushButton_CustRes_apply->setEnabled(0);
        ui->pushButton_ReadFileList->setEnabled(0);
        ui->pushButton_SaveFileList->setEnabled(0);
        curRow_image = -1;
        curRow_gif = -1;
        curRow_video = -1;
        Table_Clear();
        Custom_resolution_list.clear();
        ui->label_DropFile->setVisible(1);
        ui->tableView_gif->setVisible(0);
        ui->tableView_image->setVisible(0);
        ui->tableView_video->setVisible(0);
        ui->pushButton_ClearList->setVisible(0);
        ui->pushButton_RemoveItem->setVisible(0);
        Table_FileCount_reload();
        Send_TextBrowser_NewMessage(tr("Please wait while reading the file."));
        QtConcurrent::run(this, &MainWindow::Table_Read_Saved_Table_Filelist);
    }
    else
    {
        QMessageBox *MSG_FileList404 = new QMessageBox();
        MSG_FileList404->setWindowTitle(tr("Error"));
        MSG_FileList404->setText(tr("Cannot find the saved Files List!"));
        MSG_FileList404->setIcon(QMessageBox::Warning);
        MSG_FileList404->setModal(true);
        MSG_FileList404->show();
    }
}

void MainWindow::on_Ext_image_editingFinished()
{
    QString ext_image_str = ui->Ext_image->text();
    ext_image_str = ext_image_str.trimmed().replace("：",":").remove(" ").remove("　");
    ui->Ext_image->setText(ext_image_str);
}

void MainWindow::on_Ext_video_editingFinished()
{
    QString ext_video_str = ui->Ext_video->text();
    ext_video_str = ext_video_str.trimmed().replace("：",":").remove(" ").remove("　");
    ui->Ext_video->setText(ext_video_str);
}

void MainWindow::on_checkBox_AutoSaveSettings_clicked()
{
    QString settings_ini = Current_Path+"/settings.ini";
    if(QFile::exists(settings_ini))
    {
        QSettings *configIniWrite = new QSettings(settings_ini, QSettings::IniFormat);
        configIniWrite->setValue("/settings/AutoSaveSettings", ui->checkBox_AutoSaveSettings->isChecked());
    }
}

void MainWindow::on_pushButton_about_clicked()
{
    QMessageBox *MSG = new QMessageBox();
    MSG->setWindowTitle(tr("About"));
    QString line1 = "Waifu2x-Extension-GUI\n\n";
    QString line2 = VERSION+"\n\n";
    QString line3 = "Github: https://github.com/AaronFeng753/Waifu2x-Extension-GUI\n\n";
    QString line4 = "Waifu2x-Extension-GUI is licensed under the\n";
    QString line5 = "GNU Affero General Public License v3.0\n\n";
    QString line6 = "Copyright (C) 2020 Aaron Feng. All rights reserved.\n\n";
    QString line7 = "The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n\n";
    QString line8 = "Icons made by : Freepik (https://www.flaticon.com/authors/freepik) From Flaticon : https://www.flaticon.com/";
    MSG->setText(line1+line2+line3+line4+line5+line6+line7+line8);
    QImage img(":/new/prefix1/icon/icon_main.png");
    QImage img_scaled = img.scaled(50,50,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    QPixmap pix(QPixmap::fromImage(img_scaled));
    MSG->setIconPixmap(pix);
    MSG->setModal(false);
    MSG->show();
}

void MainWindow::on_comboBox_AspectRatio_custRes_currentIndexChanged(int index)
{
    int CurrentIndex = ui->comboBox_AspectRatio_custRes->currentIndex();
    switch(CurrentIndex)
    {
        case 0:
            {
                CustRes_AspectRatioMode = Qt::IgnoreAspectRatio;
                break;
            }
        case 1:
            {
                CustRes_AspectRatioMode = Qt::KeepAspectRatio;
                break;
            }
        case 2:
            {
                CustRes_AspectRatioMode = Qt::KeepAspectRatioByExpanding;
                break;
            }
    }
}

void MainWindow::on_checkBox_AlwaysHideSettings_stateChanged(int arg1)
{
    if(ui->checkBox_AlwaysHideSettings->isChecked())
    {
        ui->groupBox_Setting->setVisible(0);
        ui->pushButton_HideSettings->setText(tr("Show settings"));
        isSettingsHide=true;
    }
}

void MainWindow::on_pushButton_Save_GlobalFontSize_clicked()
{
    QString settings_ini = Current_Path+"/settings.ini";
    QSettings *configIniWrite = new QSettings(settings_ini, QSettings::IniFormat);
    configIniWrite->setValue("/settings/GlobalFontSize", ui->spinBox_GlobalFontSize->value());
    //==========
    if(ui->checkBox_isCustFontEnable->isChecked())
    {
        QMessageBox *MSG = new QMessageBox();
        MSG->setWindowTitle(tr("Notification"));
        MSG->setText(tr("Custom Font Settings saved successfully.\n\nRestart the software to take effect."));
        MSG->setIcon(QMessageBox::Information);
        MSG->setModal(true);
        MSG->show();
    }
    //==========
    return;
}
/*
==================================================================================================
                                      浏览与添加文本
==================================================================================================
*/
void MainWindow::on_pushButton_BrowserFile_clicked()
{
    QString Last_browsed_path = Current_Path+"/LastBrowsedPath_W2xEX.ini";
    //======== 生成 扩展名过滤 字符串 =========
    QStringList nameFilters;
    nameFilters.append("*.gif");
    QString Ext_image_str = ui->Ext_image->text();
    QStringList nameFilters_image = Ext_image_str.split(":");
    for(int i = 0; i < nameFilters_image.size(); ++i)
    {
        QString tmp = nameFilters_image.at(i).trimmed();
        if(tmp=="")continue;
        tmp = "*." + tmp;
        nameFilters.append(tmp);
    }
    QString Ext_video_str = ui->Ext_video->text();
    QStringList nameFilters_video = Ext_video_str.split(":");
    for(int i = 0; i < nameFilters_video.size(); ++i)
    {
        QString tmp = nameFilters_video.at(i).trimmed();
        if(tmp=="")continue;
        tmp = "*." + tmp;
        nameFilters.append(tmp);
    }
    QString nameFilters_QString = "";
    for(int i = 0; i < nameFilters.size(); ++i)
    {
        QString tmp = nameFilters.at(i).trimmed();
        nameFilters_QString = nameFilters_QString +" "+ tmp;
    }
    //=====================================================
    QString BrowserStartPath = "";//浏览文件时的起始文件夹
    //=========== 读取上一次浏览的文件夹 ===========================
    if(QFile::exists(Last_browsed_path))
    {
        QSettings *configIniRead = new QSettings(Last_browsed_path, QSettings::IniFormat);
        configIniRead->setIniCodec(QTextCodec::codecForName("UTF-8"));
        BrowserStartPath = configIniRead->value("/Path").toString();
        if(!QFile::exists(BrowserStartPath))BrowserStartPath = "";
    }
    //===========================================================
    QStringList Input_path_List = QFileDialog::getOpenFileNames(this, tr("Select files"), BrowserStartPath,  tr("All file(")+nameFilters_QString+")");
    if(Input_path_List.isEmpty())
    {
        return;
    }
    //================== 记住上一次浏览的文件夹 =======================
    QFile::remove(Last_browsed_path);
    QSettings *configIniWrite = new QSettings(Last_browsed_path, QSettings::IniFormat);
    configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));
    configIniWrite->setValue("/Warning/EN", "Do not modify this file! It may cause the program to crash! If problems occur after the modification, delete this file and restart the program.");
    QFileInfo lastPath(Input_path_List.at(0));
    QString folder_lastPath = file_getFolderPath(lastPath);
    configIniWrite->setValue("/Path", folder_lastPath);
    //===============================================================
    AddNew_gif=false;
    AddNew_image=false;
    AddNew_video=false;
    //================== 界面管制 ========================
    ui->groupBox_Setting->setEnabled(0);
    ui->groupBox_FileList->setEnabled(0);
    ui->groupBox_InputExt->setEnabled(0);
    ui->pushButton_Start->setEnabled(0);
    ui->checkBox_ScanSubFolders->setEnabled(0);
    this->setAcceptDrops(0);
    ui->label_DropFile->setText(tr("Adding files, please wait."));
    emit Send_TextBrowser_NewMessage(tr("Adding files, please wait."));
    //===================================================
    QtConcurrent::run(this, &MainWindow::Read_Input_paths_BrowserFile, Input_path_List);
}
/*
读取 路径与添加文件
*/
void MainWindow::Read_Input_paths_BrowserFile(QStringList Input_path_List)
{
    Progressbar_MaxVal = Input_path_List.size();
    Progressbar_CurrentVal = 0;
    emit Send_PrograssBar_Range_min_max(0, Progressbar_MaxVal);
    foreach(QString Input_path, Input_path_List)
    {
        Input_path=Input_path.trimmed();
        if(QFile::exists(Input_path)==false)continue;
        Add_File_Folder(Input_path);
        emit Send_progressbar_Add();
    }
    emit Send_Read_urls_finfished();
}
/*
打开wiki
*/
void MainWindow::on_pushButton_wiki_clicked()
{
    if(ui->comboBox_language->currentIndex()==1)
    {
        QDesktopServices::openUrl(QUrl("https://gitee.com/aaronfeng0711/Waifu2x-Extension-GUI/wikis"));
    }
    QDesktopServices::openUrl(QUrl("https://github.com/AaronFeng753/Waifu2x-Extension-GUI/wiki"));
}

void MainWindow::on_pushButton_HideTextBro_clicked()
{
    if(ui->textBrowser->isVisible())
    {
        ui->textBrowser->setVisible(0);
        ui->pushButton_HideTextBro->setText(tr("Show Text Browser"));
    }
    else
    {
        ui->textBrowser->setVisible(1);
        ui->pushButton_HideTextBro->setText(tr("Hide Text Browser"));
    }
}

void MainWindow::on_checkBox_AlwaysHideTextBrowser_stateChanged(int arg1)
{
    if(ui->checkBox_AlwaysHideTextBrowser->isChecked())
    {
        ui->textBrowser->setVisible(0);
        ui->pushButton_HideTextBro->setText(tr("Show Text Browser"));
    }
}



void MainWindow::on_Ext_image_textChanged(const QString &arg1)
{
    QString lower = ui->Ext_image->text().toLower();
    ui->Ext_image->setText(lower);
}

void MainWindow::on_Ext_video_textChanged(const QString &arg1)
{
    QString lower = ui->Ext_video->text().toLower();
    ui->Ext_video->setText(lower);
}

void MainWindow::on_comboBox_model_vulkan_currentIndexChanged(int index)
{
    if(ui->comboBox_model_vulkan->currentIndex()==0)
    {
        ui->comboBox_ImageStyle->setEnabled(1);
        if(ui->comboBox_Engine_Image->currentIndex()!=0&&ui->comboBox_Engine_GIF->currentIndex()!=0&&ui->comboBox_Engine_Video->currentIndex()!=0)
        {
            ui->comboBox_ImageStyle->setEnabled(0);
        }
    }
    if(ui->comboBox_model_vulkan->currentIndex()==1)
    {
        ui->comboBox_ImageStyle->setEnabled(0);
    }
}

void MainWindow::on_comboBox_ImageStyle_currentIndexChanged(int index)
{
    if(ui->comboBox_ImageStyle->currentIndex()==0)
    {
        ui->comboBox_model_vulkan->setEnabled(1);
    }
    if(ui->comboBox_ImageStyle->currentIndex()==1)
    {
        ui->comboBox_model_vulkan->setEnabled(0);
    }
}

void MainWindow::on_pushButton_ResetVideoSettings_clicked()
{
    ui->lineEdit_pixformat->setText("yuv420p");
    ui->lineEdit_encoder_vid->setText("libx264");
    ui->lineEdit_encoder_audio->setText("aac");
    ui->spinBox_bitrate_vid->setValue(6000);
    ui->spinBox_bitrate_audio->setValue(320);
    //====
    ui->spinBox_bitrate_vid_2mp4->setValue(2500);
    ui->spinBox_bitrate_audio_2mp4->setValue(320);
    ui->checkBox_acodec_copy_2mp4->setChecked(0);
    ui->checkBox_vcodec_copy_2mp4->setChecked(0);
    ui->spinBox_bitrate_vid_2mp4->setEnabled(1);
    ui->spinBox_bitrate_audio_2mp4->setEnabled(1);
    //====
    ui->lineEdit_ExCommand_2mp4->setText("");
    ui->lineEdit_ExCommand_output->setText("");
}

void MainWindow::on_lineEdit_encoder_vid_textChanged(const QString &arg1)
{
    QString tmp = ui->lineEdit_encoder_vid->text().trimmed();
    ui->lineEdit_encoder_vid->setText(tmp);
}

void MainWindow::on_lineEdit_encoder_audio_textChanged(const QString &arg1)
{
    QString tmp = ui->lineEdit_encoder_audio->text().trimmed();
    ui->lineEdit_encoder_audio->setText(tmp);
}

void MainWindow::on_lineEdit_pixformat_textChanged(const QString &arg1)
{
    QString tmp = ui->lineEdit_pixformat->text().trimmed();
    ui->lineEdit_pixformat->setText(tmp);
}

void MainWindow::on_checkBox_vcodec_copy_2mp4_stateChanged(int arg1)
{
    if(ui->checkBox_vcodec_copy_2mp4->isChecked())
    {
        ui->spinBox_bitrate_vid_2mp4->setEnabled(0);
    }
    else
    {
        ui->spinBox_bitrate_vid_2mp4->setEnabled(1);
    }
}

void MainWindow::on_checkBox_acodec_copy_2mp4_stateChanged(int arg1)
{
    if(ui->checkBox_acodec_copy_2mp4->isChecked())
    {
        ui->spinBox_bitrate_audio_2mp4->setEnabled(0);
    }
    else
    {
        ui->spinBox_bitrate_audio_2mp4->setEnabled(1);
    }
}


void MainWindow::on_pushButton_encodersList_clicked()
{
    file_OpenFile(Current_Path+"/FFmpeg_Encoders_List_waifu2xEX.bat");
}

void MainWindow::Tip_FirstTimeStart()
{
    QString FirstTimeStart = Current_Path+"/FirstTimeStart";
    if(QFile::exists(FirstTimeStart))
    {
        return;
    }
    else
    {
        /*
          弹出语言选择对话框
        */
        QMessageBox Msg(QMessageBox::Question, QString("Choose your language"), QString("Choose your language.\n\n选择您的语言。\n\n言語を選んでください。"));
        Msg.setIcon(QMessageBox::Information);
        QAbstractButton *pYesBtn_English = Msg.addButton(QString("English"), QMessageBox::YesRole);
        QAbstractButton *pYesBtn_Chinese = Msg.addButton(QString("简体中文"), QMessageBox::YesRole);
        QAbstractButton *pYesBtn_Japanese = Msg.addButton(QString("日本語(機械翻訳)"), QMessageBox::YesRole);
        QAbstractButton *pYesBtn_TraditionalChinese = Msg.addButton(QString("繁體中文(由uimee翻譯)"), QMessageBox::YesRole);
        Msg.exec();
        if (Msg.clickedButton() == pYesBtn_English)ui->comboBox_language->setCurrentIndex(0);
        if (Msg.clickedButton() == pYesBtn_Chinese)ui->comboBox_language->setCurrentIndex(1);
        if (Msg.clickedButton() == pYesBtn_Japanese)ui->comboBox_language->setCurrentIndex(2);
        if (Msg.clickedButton() == pYesBtn_TraditionalChinese)ui->comboBox_language->setCurrentIndex(3);
        on_comboBox_language_currentIndexChanged(0);
        /*
          弹出必读Tips对话框
        */
        QString English_1 = tr("- Please read the Wiki before starting to use the software.\n");
        QString English_7 = tr("- If there is a problem with the software font display, you can modify the font in the additional settings.\n");
        QString English_8 = tr("- This software is free software, if you find anyone selling this software, please report the seller.\n");
        QString English_9 = tr("- This software is free and open source, and is is licensed under the GNU Affero General Public License v3.0. All consequences of using this software are borne by the user, and the developer does not bear any responsibility.\n");
        QString English_10 = tr("- If you like this software, please donate to the developer, thank you.\n");
        //========
        QMessageBox *MSG = new QMessageBox();
        MSG->setWindowTitle(tr("!!! Tips !!!"));
        MSG->setText(English_1+English_7+English_8+English_9+English_10);
        MSG->setIcon(QMessageBox::Information);
        MSG->setModal(true);
        MSG->show();
        //======
        QMessageBox *MSG_2 = new QMessageBox();
        MSG_2->setWindowTitle(tr("Notification"));
        MSG_2->setText(tr("It is detected that this is the first time you have started the software, so the compatibility test will be performed automatically. Please wait for a while, then check the test result."));
        MSG_2->setIcon(QMessageBox::Information);
        MSG_2->setModal(true);
        MSG_2->show();
        //=======
        QFile file(FirstTimeStart);
        file.remove();
        if (file.open(QIODevice::ReadWrite | QIODevice::Text)) //QIODevice::ReadWrite支持读写
        {
            QTextStream stream(&file);
            stream << "Don't delete this file!!";
        }
        //=======
        on_pushButton_clear_textbrowser_clicked();
        on_pushButton_compatibilityTest_clicked();
    }
}

void MainWindow::on_checkBox_DelOriginal_stateChanged(int arg1)
{
    if(ui->checkBox_DelOriginal->isChecked())
    {
        ui->checkBox_Move2RecycleBin->setEnabled(1);
        ui->checkBox_ReplaceOriginalFile->setEnabled(0);
        ui->checkBox_ReplaceOriginalFile->setChecked(0);
    }
    else
    {
        ui->checkBox_Move2RecycleBin->setEnabled(0);
        checkBox_ReplaceOriginalFile_setEnabled_True_Self();
    }
}
/*
是否启用自定义视频设置
*/
void MainWindow::on_checkBox_videoSettings_isEnabled_stateChanged(int arg1)
{
    if(ui->checkBox_videoSettings_isEnabled->isChecked())
    {
        ui->groupBox_OutputVideoSettings->setEnabled(1);
        ui->groupBox_ToMp4VideoSettings->setEnabled(1);
    }
    else
    {
        ui->groupBox_OutputVideoSettings->setEnabled(0);
        ui->groupBox_ToMp4VideoSettings->setEnabled(0);
    }
}

void MainWindow::on_checkBox_FileList_Interactive_stateChanged(int arg1)
{
    if(ui->checkBox_FileList_Interactive->isChecked())
    {
        ui->tableView_image->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        ui->tableView_gif->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        ui->tableView_video->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }
    else
    {
        ui->tableView_image->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView_gif->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView_video->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
}

void MainWindow::on_checkBox_OutPath_isEnabled_stateChanged(int arg1)
{
    if(ui->checkBox_OutPath_isEnabled->isChecked())
    {
        ui->lineEdit_outputPath->setEnabled(1);
        ui->checkBox_OutPath_KeepOriginalFileName->setEnabled(1);
        ui->checkBox_ReplaceOriginalFile->setEnabled(0);
        ui->checkBox_ReplaceOriginalFile->setChecked(0);
    }
    else
    {
        ui->lineEdit_outputPath->setEnabled(0);
        ui->checkBox_OutPath_KeepOriginalFileName->setEnabled(0);
        checkBox_ReplaceOriginalFile_setEnabled_True_Self();
    }
}

//强制重试
void MainWindow::on_pushButton_ForceRetry_clicked()
{
    ui->pushButton_ForceRetry->setEnabled(0);
    //========
    QtConcurrent::run(this, &MainWindow::isForceRetryClicked_SetTrue_Block_Anime4k);//block a4k引擎线程 防止无效图片污染缓存
    //========
    int tmp = ui->spinBox_retry->value();
    tmp++;
    ui->spinBox_retry->setValue(tmp);
    //========
    QProcess Close;
    Close.start("taskkill /f /t /fi \"imagename eq Anime4K_waifu2xEX.exe\"");
    Close.waitForStarted(10000);
    Close.waitForFinished(10000);
    Close.start("taskkill /f /t /fi \"imagename eq waifu2x-ncnn-vulkan_waifu2xEX.exe\"");
    Close.waitForStarted(10000);
    Close.waitForFinished(10000);
    Close.start("taskkill /f /t /fi \"imagename eq waifu2x-ncnn-vulkan-fp16p_waifu2xEX.exe\"");
    Close.waitForStarted(10000);
    Close.waitForFinished(10000);
    Close.start("taskkill /f /t /fi \"imagename eq waifu2x-converter-cpp_waifu2xEX.exe\"");
    Close.waitForStarted(10000);
    Close.waitForFinished(10000);
    Close.start("taskkill /f /t /fi \"imagename eq srmd-ncnn-vulkan_waifu2xEX.exe\"");
    Close.waitForStarted(10000);
    Close.waitForFinished(10000);
    Close.start("taskkill /f /t /fi \"imagename eq waifu2x-caffe_waifu2xEX.exe\"");
    Close.waitForStarted(10000);
    Close.waitForFinished(10000);
    Close.start("taskkill /f /t /fi \"imagename eq realsr-ncnn-vulkan_waifu2xEX.exe\"");
    Close.waitForStarted(10000);
    Close.waitForFinished(10000);
    //========
    emit Send_TextBrowser_NewMessage(tr("Force retry."));
    return;
}
//激活强制重试按钮
void MainWindow::SetEnable_pushButton_ForceRetry_self()
{
    ui->pushButton_ForceRetry->setEnabled(1);
    return;
}

void MainWindow::on_pushButton_PayPal_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.paypal.me/aaronfeng753"));
}

void MainWindow::on_checkBox_AudioDenoise_stateChanged(int arg1)
{
    if(ui->checkBox_AudioDenoise->isChecked())
    {
        ui->doubleSpinBox_AudioDenoiseLevel->setEnabled(1);
    }
    else
    {
        ui->doubleSpinBox_AudioDenoiseLevel->setEnabled(0);
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch(ui->tabWidget->currentIndex())
    {
        case 0:
            {
                //tab 0
                ui->label_DonateQRCode->setVisible(1);
                ui->pushButton_PayPal->setVisible(1);
                ui->label_DonateText->setVisible(1);
                //tab 1
                ui->groupBox_FileList->setVisible(0);
                ui->groupBox_Progress->setVisible(0);
                ui->groupBox_Setting->setVisible(0);
                //tab 2
                ui->groupBox_Engine->setVisible(0);
                ui->groupBox_NumOfThreads->setVisible(0);
                //tab 3
                ui->groupBox_AudioDenoise->setVisible(0);
                ui->groupBox_video_settings->setVisible(0);
                //tab 4
                ui->groupBox_AddSettingsButtons->setVisible(0);
                ui->groupBox_AdditionalSettings->setVisible(0);
                //tab 5
                ui->groupBox_CompatibilityTestRes->setVisible(0);
                ui->pushButton_compatibilityTest->setVisible(0);
                break;
            }
        case 1:
            {
                //tab 0
                ui->label_DonateQRCode->setVisible(0);
                ui->pushButton_PayPal->setVisible(0);
                ui->label_DonateText->setVisible(0);
                //tab 1
                ui->groupBox_FileList->setVisible(1);
                ui->groupBox_Progress->setVisible(1);
                if(isSettingsHide==false)
                {
                    ui->groupBox_Setting->setVisible(1);
                }
                //tab 2
                ui->groupBox_Engine->setVisible(0);
                ui->groupBox_NumOfThreads->setVisible(0);
                //tab 3
                ui->groupBox_AudioDenoise->setVisible(0);
                ui->groupBox_video_settings->setVisible(0);
                //tab 4
                ui->groupBox_AddSettingsButtons->setVisible(0);
                ui->groupBox_AdditionalSettings->setVisible(0);
                //tab 5
                ui->groupBox_CompatibilityTestRes->setVisible(0);
                ui->pushButton_compatibilityTest->setVisible(0);
                break;
            }
        case 2:
            {
                //tab 0
                ui->label_DonateQRCode->setVisible(0);
                ui->pushButton_PayPal->setVisible(0);
                ui->label_DonateText->setVisible(0);
                //tab 1
                ui->groupBox_FileList->setVisible(0);
                ui->groupBox_Progress->setVisible(0);
                ui->groupBox_Setting->setVisible(0);
                //tab 2
                ui->groupBox_Engine->setVisible(1);
                ui->groupBox_NumOfThreads->setVisible(1);
                //tab 3
                ui->groupBox_AudioDenoise->setVisible(0);
                ui->groupBox_video_settings->setVisible(0);
                //tab 4
                ui->groupBox_AddSettingsButtons->setVisible(0);
                ui->groupBox_AdditionalSettings->setVisible(0);
                //tab 5
                ui->groupBox_CompatibilityTestRes->setVisible(0);
                ui->pushButton_compatibilityTest->setVisible(0);
                break;
            }
        case 3:
            {
                //tab 0
                ui->label_DonateQRCode->setVisible(0);
                ui->pushButton_PayPal->setVisible(0);
                ui->label_DonateText->setVisible(0);
                //tab 1
                ui->groupBox_FileList->setVisible(0);
                ui->groupBox_Progress->setVisible(0);
                ui->groupBox_Setting->setVisible(0);
                //tab 2
                ui->groupBox_Engine->setVisible(0);
                ui->groupBox_NumOfThreads->setVisible(0);
                //tab 3
                ui->groupBox_AudioDenoise->setVisible(1);
                ui->groupBox_video_settings->setVisible(1);
                //tab 4
                ui->groupBox_AddSettingsButtons->setVisible(0);
                ui->groupBox_AdditionalSettings->setVisible(0);
                //tab 5
                ui->groupBox_CompatibilityTestRes->setVisible(0);
                ui->pushButton_compatibilityTest->setVisible(0);
                break;
            }
        case 4:
            {
                //tab 0
                ui->label_DonateQRCode->setVisible(0);
                ui->pushButton_PayPal->setVisible(0);
                ui->label_DonateText->setVisible(0);
                //tab 1
                ui->groupBox_FileList->setVisible(0);
                ui->groupBox_Progress->setVisible(0);
                ui->groupBox_Setting->setVisible(0);
                //tab 2
                ui->groupBox_Engine->setVisible(0);
                ui->groupBox_NumOfThreads->setVisible(0);
                //tab 3
                ui->groupBox_AudioDenoise->setVisible(0);
                ui->groupBox_video_settings->setVisible(0);
                //tab 4
                ui->groupBox_AddSettingsButtons->setVisible(1);
                ui->groupBox_AdditionalSettings->setVisible(1);
                //tab 5
                ui->groupBox_CompatibilityTestRes->setVisible(0);
                ui->pushButton_compatibilityTest->setVisible(0);
                break;
            }
        case 5:
            {
                //tab 0
                ui->label_DonateQRCode->setVisible(0);
                ui->pushButton_PayPal->setVisible(0);
                ui->label_DonateText->setVisible(0);
                //tab 1
                ui->groupBox_FileList->setVisible(0);
                ui->groupBox_Progress->setVisible(0);
                ui->groupBox_Setting->setVisible(0);
                //tab 2
                ui->groupBox_Engine->setVisible(0);
                ui->groupBox_NumOfThreads->setVisible(0);
                //tab 3
                ui->groupBox_AudioDenoise->setVisible(0);
                ui->groupBox_video_settings->setVisible(0);
                //tab 4
                ui->groupBox_AddSettingsButtons->setVisible(0);
                ui->groupBox_AdditionalSettings->setVisible(0);
                //tab 5
                ui->groupBox_CompatibilityTestRes->setVisible(1);
                ui->pushButton_compatibilityTest->setVisible(1);
                break;
            }
    }
}

void MainWindow::on_checkBox_ProcessVideoBySegment_stateChanged(int arg1)
{
    if(ui->checkBox_ProcessVideoBySegment->isChecked())
    {
        ui->label_SegmentDuration->setEnabled(1);
        ui->spinBox_SegmentDuration->setEnabled(1);
    }
    else
    {
        ui->label_SegmentDuration->setEnabled(0);
        ui->spinBox_SegmentDuration->setEnabled(0);
    }
}

void MainWindow::on_comboBox_version_Waifu2xNCNNVulkan_currentIndexChanged(int index)
{
    switch (ui->comboBox_version_Waifu2xNCNNVulkan->currentIndex())
    {
        case 0:
            {
                Waifu2x_ncnn_vulkan_FolderPath = Current_Path + "/waifu2x-ncnn-vulkan";
                Waifu2x_ncnn_vulkan_ProgramPath = Waifu2x_ncnn_vulkan_FolderPath + "/waifu2x-ncnn-vulkan_waifu2xEX.exe";
                ui->checkBox_TTA_vulkan->setEnabled(1);
                return;
            }
        case 1:
            {
                Waifu2x_ncnn_vulkan_FolderPath = Current_Path + "/waifu2x-ncnn-vulkan";
                Waifu2x_ncnn_vulkan_ProgramPath = Waifu2x_ncnn_vulkan_FolderPath + "/waifu2x-ncnn-vulkan-fp16p_waifu2xEX.exe";
                ui->checkBox_TTA_vulkan->setEnabled(1);
                return;
            }
        case 2:
            {
                Waifu2x_ncnn_vulkan_FolderPath = Current_Path + "/waifu2x-ncnn-vulkan-old";
                Waifu2x_ncnn_vulkan_ProgramPath = Waifu2x_ncnn_vulkan_FolderPath + "/waifu2x-ncnn-vulkan_waifu2xEX.exe";
                ui->checkBox_TTA_vulkan->setEnabled(0);
                ui->checkBox_TTA_vulkan->setChecked(0);
                return;
            }
    }
}

void MainWindow::on_checkBox_EnablePreProcessing_Anime4k_stateChanged(int arg1)
{
    if(ui->checkBox_EnablePreProcessing_Anime4k->isChecked())
    {
        ui->checkBox_MedianBlur_Pre_Anime4k->setEnabled(1);
        ui->checkBox_MeanBlur_Pre_Anime4k->setEnabled(1);
        ui->checkBox_CASSharping_Pre_Anime4k->setEnabled(1);
        ui->checkBox_GaussianBlurWeak_Pre_Anime4k->setEnabled(1);
        ui->checkBox_GaussianBlur_Pre_Anime4k->setEnabled(1);
        ui->checkBox_BilateralFilter_Pre_Anime4k->setEnabled(1);
        ui->checkBox_BilateralFilterFaster_Pre_Anime4k->setEnabled(1);
    }
    else
    {
        ui->checkBox_MedianBlur_Pre_Anime4k->setEnabled(0);
        ui->checkBox_MeanBlur_Pre_Anime4k->setEnabled(0);
        ui->checkBox_CASSharping_Pre_Anime4k->setEnabled(0);
        ui->checkBox_GaussianBlurWeak_Pre_Anime4k->setEnabled(0);
        ui->checkBox_GaussianBlur_Pre_Anime4k->setEnabled(0);
        ui->checkBox_BilateralFilter_Pre_Anime4k->setEnabled(0);
        ui->checkBox_BilateralFilterFaster_Pre_Anime4k->setEnabled(0);
    }
}

void MainWindow::on_checkBox_EnablePostProcessing_Anime4k_stateChanged(int arg1)
{
    if(ui->checkBox_EnablePostProcessing_Anime4k->isChecked())
    {
        ui->checkBox_MedianBlur_Post_Anime4k->setEnabled(1);
        ui->checkBox_MeanBlur_Post_Anime4k->setEnabled(1);
        ui->checkBox_CASSharping_Post_Anime4k->setEnabled(1);
        ui->checkBox_GaussianBlurWeak_Post_Anime4k->setEnabled(1);
        ui->checkBox_GaussianBlur_Post_Anime4k->setEnabled(1);
        ui->checkBox_BilateralFilter_Post_Anime4k->setEnabled(1);
        ui->checkBox_BilateralFilterFaster_Post_Anime4k->setEnabled(1);
    }
    else
    {
        ui->checkBox_MedianBlur_Post_Anime4k->setEnabled(0);
        ui->checkBox_MeanBlur_Post_Anime4k->setEnabled(0);
        ui->checkBox_CASSharping_Post_Anime4k->setEnabled(0);
        ui->checkBox_GaussianBlurWeak_Post_Anime4k->setEnabled(0);
        ui->checkBox_GaussianBlur_Post_Anime4k->setEnabled(0);
        ui->checkBox_BilateralFilter_Post_Anime4k->setEnabled(0);
        ui->checkBox_BilateralFilterFaster_Post_Anime4k->setEnabled(0);
    }
}

void MainWindow::on_checkBox_SpecifyGPU_Anime4k_stateChanged(int arg1)
{
    if(ui->checkBox_SpecifyGPU_Anime4k->isChecked())
    {
        ui->lineEdit_GPUs_Anime4k->setEnabled(1);
        ui->pushButton_ListGPUs_Anime4k->setEnabled(1);
        ui->pushButton_VerifyGPUsConfig_Anime4k->setEnabled(1);
    }
    else
    {
        ui->lineEdit_GPUs_Anime4k->setEnabled(0);
        ui->pushButton_ListGPUs_Anime4k->setEnabled(0);
        ui->pushButton_VerifyGPUsConfig_Anime4k->setEnabled(0);
    }
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW->setChecked(isCompatible_Waifu2x_NCNN_Vulkan_NEW);
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW_FP16P_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW_FP16P->setChecked(isCompatible_Waifu2x_NCNN_Vulkan_NEW_FP16P);
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_NCNN_Vulkan_OLD_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_NCNN_Vulkan_OLD->setChecked(isCompatible_Waifu2x_NCNN_Vulkan_OLD);
}

void MainWindow::on_checkBox_isCompatible_SRMD_NCNN_Vulkan_clicked()
{
    ui->checkBox_isCompatible_SRMD_NCNN_Vulkan->setChecked(isCompatible_SRMD_NCNN_Vulkan);
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_Converter_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_Converter->setChecked(isCompatible_Waifu2x_Converter);
}

void MainWindow::on_checkBox_isCompatible_Anime4k_CPU_clicked()
{
    ui->checkBox_isCompatible_Anime4k_CPU->setChecked(isCompatible_Anime4k_CPU);
}

void MainWindow::on_checkBox_isCompatible_Anime4k_GPU_clicked()
{
    ui->checkBox_isCompatible_Anime4k_GPU->setChecked(isCompatible_Anime4k_GPU);
}

void MainWindow::on_checkBox_isCompatible_PythonExtension_clicked()
{
    ui->checkBox_isCompatible_PythonExtension->setChecked(isCompatible_PythonExtension);
}

void MainWindow::on_checkBox_isCompatible_FFmpeg_clicked()
{
    ui->checkBox_isCompatible_FFmpeg->setChecked(isCompatible_FFmpeg);
}

void MainWindow::on_checkBox_isCompatible_FFprobe_clicked()
{
    ui->checkBox_isCompatible_FFprobe->setChecked(isCompatible_FFprobe);
}

void MainWindow::on_checkBox_isCompatible_ImageMagick_clicked()
{
    ui->checkBox_isCompatible_ImageMagick->setChecked(isCompatible_ImageMagick);
}

void MainWindow::on_checkBox_isCompatible_Gifsicle_clicked()
{
    ui->checkBox_isCompatible_Gifsicle->setChecked(isCompatible_Gifsicle);
}

void MainWindow::on_checkBox_isCompatible_SoX_clicked()
{
    ui->checkBox_isCompatible_SoX->setChecked(isCompatible_SoX);
}

void MainWindow::on_checkBox_GPUMode_Anime4K_stateChanged(int arg1)
{
    if(ui->checkBox_GPUMode_Anime4K->isChecked())
    {
        ui->checkBox_SpecifyGPU_Anime4k->setEnabled(1);
    }
    else
    {
        ui->checkBox_SpecifyGPU_Anime4k->setEnabled(0);
        ui->checkBox_SpecifyGPU_Anime4k->setChecked(0);
    }
}

void MainWindow::on_checkBox_ShowInterPro_stateChanged(int arg1)
{
    if(ui->checkBox_ShowInterPro->isChecked()==false)
    {
        emit Send_CurrentFileProgress_Stop();
    }
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_Caffe_CPU_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_Caffe_CPU->setChecked(isCompatible_Waifu2x_Caffe_CPU);
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_Caffe_GPU_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_Caffe_GPU->setChecked(isCompatible_Waifu2x_Caffe_GPU);
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_Caffe_cuDNN_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_Caffe_cuDNN->setChecked(isCompatible_Waifu2x_Caffe_cuDNN);
}

void MainWindow::on_pushButton_SplitSize_Add_Waifu2xCaffe_clicked()
{
    int VAL = ui->spinBox_SplitSize_Waifu2xCaffe->value()*2;
    if(VAL<=999999999)
    {
        ui->spinBox_SplitSize_Waifu2xCaffe->setValue(VAL);
    }
}

void MainWindow::on_pushButton_SplitSize_Minus_Waifu2xCaffe_clicked()
{
    int VAL = ui->spinBox_SplitSize_Waifu2xCaffe->value()/2;
    if(VAL>=2)
    {
        ui->spinBox_SplitSize_Waifu2xCaffe->setValue(VAL);
    }
}

void MainWindow::on_checkBox_isCompatible_Realsr_NCNN_Vulkan_clicked()
{
    ui->checkBox_isCompatible_Realsr_NCNN_Vulkan->setChecked(isCompatible_Realsr_NCNN_Vulkan);
}

void MainWindow::on_checkBox_ACNet_Anime4K_stateChanged(int arg1)
{
    if(ui->checkBox_ACNet_Anime4K->isChecked())
    {
        ui->checkBox_HDNMode_Anime4k->setEnabled(1);
        ui->groupBox_PostProcessing_Anime4k->setEnabled(0);
        ui->groupBox_PreProcessing_Anime4k->setEnabled(0);
        ui->doubleSpinBox_PushColorStrength_Anime4K->setEnabled(0);
        ui->doubleSpinBox_PushGradientStrength_Anime4K->setEnabled(0);
        ui->spinBox_Passes_Anime4K->setEnabled(0);
        ui->spinBox_PushColorCount_Anime4K->setEnabled(0);
    }
    else
    {
        ui->checkBox_HDNMode_Anime4k->setEnabled(0);
        on_checkBox_GPUMode_Anime4K_stateChanged(0);
        on_checkBox_SpecifyGPU_Anime4k_stateChanged(0);
        ui->groupBox_PostProcessing_Anime4k->setEnabled(1);
        ui->groupBox_PreProcessing_Anime4k->setEnabled(1);
        ui->doubleSpinBox_PushColorStrength_Anime4K->setEnabled(1);
        ui->doubleSpinBox_PushGradientStrength_Anime4K->setEnabled(1);
        ui->spinBox_Passes_Anime4K->setEnabled(1);
        ui->spinBox_PushColorCount_Anime4K->setEnabled(1);
    }
    DenoiseLevelSpinboxSetting_Anime4k();
}

void MainWindow::on_checkBox_CompressJPG_stateChanged(int arg1)
{
    if(ui->checkBox_CompressJPG->isChecked())
    {
        ui->spinBox_JPGCompressedQuality->setEnabled(1);
    }
    else
    {
        ui->spinBox_JPGCompressedQuality->setEnabled(0);
    }
}

void MainWindow::on_checkBox_HDNMode_Anime4k_stateChanged(int arg1)
{
    DenoiseLevelSpinboxSetting_Anime4k();
}

void MainWindow::ExecuteCMD_batFile(QString cmd_str)
{
    ExecuteCMD_batFile_QMutex.lock();
    QString cmd_commands = "@echo off\n "+cmd_str+"\n exit";
    Delay_msec_sleep(100);//延时防止文件名称碰撞
    file_mkDir(Current_Path+"/batFiles_tmp");
    QString Bat_path = Current_Path+"/batFiles_tmp/W2xEX_"+QDateTime::currentDateTime().toString("dhhmmsszzz")+".bat";
    //========
    QFile OpenFile_cmdFile(Bat_path);
    OpenFile_cmdFile.remove();
    if (OpenFile_cmdFile.open(QIODevice::ReadWrite | QIODevice::Text)) //QIODevice::ReadWrite支持读写
    {
        QTextStream stream(&OpenFile_cmdFile);
        stream << cmd_commands;
    }
    OpenFile_cmdFile.close();
    QDesktopServices::openUrl(QUrl("file:"+Bat_path));
    //========
    ExecuteCMD_batFile_QMutex.unlock();
}

void MainWindow::Del_TempBatFile()
{
    file_DelDir(Current_Path+"/batFiles_tmp");
}

void MainWindow::comboBox_UpdateChannel_setCurrentIndex_self(int index)
{
    comboBox_UpdateChannel_setCurrentIndex_self_QMutex.lock();
    isClicked_comboBox_UpdateChannel=false;
    ui->comboBox_UpdateChannel->setCurrentIndex(index);
    isClicked_comboBox_UpdateChannel=true;
    comboBox_UpdateChannel_setCurrentIndex_self_QMutex.unlock();
}

void MainWindow::on_comboBox_UpdateChannel_currentIndexChanged(int index)
{
    if(isClicked_comboBox_UpdateChannel)
    {
        AutoUpdate = QtConcurrent::run(this, &MainWindow::CheckUpadte_Auto);//自动检查更新线程
    }
}

void MainWindow::on_checkBox_ReplaceOriginalFile_stateChanged(int arg1)
{
    if(ui->checkBox_ReplaceOriginalFile->isChecked())
    {
        ui->groupBox_OutPut->setEnabled(0);
        ui->checkBox_OutPath_isEnabled->setChecked(0);
        ui->checkBox_DelOriginal->setEnabled(0);
    }
    else
    {
        ui->groupBox_OutPut->setEnabled(1);
        ui->checkBox_DelOriginal->setEnabled(1);
    }
}

void MainWindow::checkBox_ReplaceOriginalFile_setEnabled_True_Self()
{
    if(ui->checkBox_DelOriginal->isChecked()==false && ui->checkBox_OutPath_isEnabled->isChecked()==false)
    {
        ui->checkBox_ReplaceOriginalFile->setEnabled(1);
    }
}

bool MainWindow::ReplaceOriginalFile(QString original_fullpath,QString output_fullpath)
{
    //检查是否启用替换源文件,以及输出文件是否存在
    if(ui->checkBox_ReplaceOriginalFile->isChecked()==false || QFile::exists(output_fullpath)==false)return false;
    //=================
    QFileInfo fileinfo_original_fullpath(original_fullpath);
    QFileInfo fileinfo_output_fullpath(output_fullpath);
    QString file_name = file_getBaseName(fileinfo_original_fullpath.filePath());//获取源文件的文件名
    QString file_ext = fileinfo_output_fullpath.suffix();//获取输出文件的后辍
    QString file_path = file_getFolderPath(fileinfo_original_fullpath);//获取源文件的文件路径
    //=================
    QString Target_fullpath="";
    if(checkQAction_AddSuffix->isChecked())
    {
        Target_fullpath=file_path+"/"+file_name+"_W2xEX."+file_ext;
    }
    else
    {
        Target_fullpath=file_path+"/"+file_name+"."+file_ext;
    }
    //=================
    if(checkQAction_MoveToRecycleBin->isChecked())
    {
        file_MoveToTrash(original_fullpath);
    }
    else
    {
        QFile::remove(original_fullpath);
    }
    //=================
    if(QFile::rename(output_fullpath,Target_fullpath)==false)
    {
        emit Send_TextBrowser_NewMessage(tr("Error! Failed to move [")+output_fullpath+tr("] to [")+Target_fullpath+"]");
    }
    return true;
}
