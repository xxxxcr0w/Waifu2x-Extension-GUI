﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Waifu2xMainThread()
{
    QtConcurrent::run(this, &MainWindow::Table_ChangeAllStatusToWaiting);
    Progressbar_MaxVal = FileList_image.count() + FileList_gif.count() + FileList_video.count();
    Progressbar_CurrentVal = 0;
    TaskNumFinished=0;
    emit Send_PrograssBar_Range_min_max(0, Progressbar_MaxVal);
    if(!FileList_image.isEmpty())
    {
        int ImageEngine = ui->comboBox_Engine_Image->currentIndex();
        QList<QMap<QString, QString>> FileList_image_tmp = FileList_image;
        for ( int i = 0; i != FileList_image_tmp.size(); ++i )
        {
            if(waifu2x_STOP)
            {
                while (ThreadNumRunning > 0)
                {
                    Delay_msec_sleep(500);
                }
                waifu2x_STOP_confirm = true;
                return 0;//如果启用stop位,则直接return
            }
            QMap<QString, QString> File_map = FileList_image_tmp.at(i);
            ThreadNumMax = ui->spinBox_ThreadNum_image->value();//获取image线程数量最大值
            switch(ImageEngine)
            {
                case 0:
                    {
                        ThreadNumRunning++;//线程数量统计+1s
                        QtConcurrent::run(this, &MainWindow::Waifu2x_NCNN_Vulkan_Image, File_map);
                        while (ThreadNumRunning >= ThreadNumMax)
                        {
                            Delay_msec_sleep(500);
                        }
                        break;
                    }
                case 1:
                    {
                        ThreadNumRunning++;//线程数量统计+1s
                        QtConcurrent::run(this, &MainWindow::Waifu2x_Converter_Image, File_map);
                        while (ThreadNumRunning >= ThreadNumMax)
                        {
                            Delay_msec_sleep(500);
                        }
                        break;
                    }
            }
        }
    }
    while (ThreadNumRunning!=0)
    {
        Delay_msec_sleep(500);
    }
    if(!FileList_gif.isEmpty())
    {
        int GIFEngine = ui->comboBox_Engine_GIF->currentIndex();
        QList<QMap<QString, QString>> FileList_gif_tmp = FileList_gif;
        for ( int i = 0; i != FileList_gif_tmp.size(); ++i )
        {
            if(waifu2x_STOP)
            {
                while (ThreadNumRunning > 0)
                {
                    Delay_msec_sleep(500);
                }
                waifu2x_STOP_confirm = true;
                return 0;//如果启用stop位,则直接return
            }
            QMap<QString, QString> File_map = FileList_gif_tmp.at(i);
            ThreadNumMax = ui->spinBox_ThreadNum_gif->value();//获取gif线程数量最大值
            switch(GIFEngine)
            {
                case 0:
                    {
                        ThreadNumRunning++;//线程数量统计+1s
                        QtConcurrent::run(this, &MainWindow::Waifu2x_NCNN_Vulkan_GIF, File_map);
                        while (ThreadNumRunning >= ThreadNumMax)
                        {
                            Delay_msec_sleep(500);
                        }
                        break;
                    }
                case 1:
                    {
                        ThreadNumRunning++;//线程数量统计+1s
                        QtConcurrent::run(this, &MainWindow::Waifu2x_Converter_GIF, File_map);
                        while (ThreadNumRunning >= ThreadNumMax)
                        {
                            Delay_msec_sleep(500);
                        }
                        break;
                    }
            }
        }
    }
    while (ThreadNumRunning!=0)
    {
        Delay_msec_sleep(500);
    }
    if(!FileList_video.isEmpty())
    {
        int VideoEngine = ui->comboBox_Engine_Video->currentIndex();
        QList<QMap<QString, QString>> FileList_video_tmp = FileList_video;
        for ( int i = 0; i != FileList_video_tmp.size(); ++i )
        {
            if(waifu2x_STOP)
            {
                while (ThreadNumRunning > 0)
                {
                    Delay_msec_sleep(500);
                }
                waifu2x_STOP_confirm = true;
                return 0;//如果启用stop位,则直接return
            }
            QMap<QString, QString> File_map = FileList_video_tmp.at(i);
            ThreadNumMax = ui->spinBox_ThreadNum_video->value();//获取video线程数量最大值
            switch(VideoEngine)
            {
                case 0:
                    {
                        ThreadNumRunning++;//线程数量统计+1s
                        QtConcurrent::run(this, &MainWindow::Waifu2x_NCNN_Vulkan_Video, File_map);
                        while (ThreadNumRunning >= ThreadNumMax)
                        {
                            Delay_msec_sleep(500);
                        }
                        break;
                    }
                case 1:
                    {
                        ThreadNumRunning++;//线程数量统计+1s
                        QtConcurrent::run(this, &MainWindow::Waifu2x_Converter_Video, File_map);
                        while (ThreadNumRunning >= ThreadNumMax)
                        {
                            Delay_msec_sleep(500);
                        }
                        break;
                    }
                case 2:
                    {
                        ThreadNumRunning++;//线程数量统计+1s
                        QtConcurrent::run(this, &MainWindow::Anime4k_Video, File_map);
                        while (ThreadNumRunning >= ThreadNumMax)
                        {
                            Delay_msec_sleep(500);
                        }
                        break;
                    }
            }
        }
    }
    while (ThreadNumRunning!=0)
    {
        Delay_msec_sleep(500);
    }
    emit Send_Waifu2x_Finished();
    return 0;
}

void MainWindow::Waifu2x_Finished()
{
    //===================== 关机 ==============================
    if(ui->checkBox_AutoTurnOFF->checkState())
    {
        emit Send_TextBrowser_NewMessage("The computer will automatically shut down in 60 seconds!");
        Delay_sec(60);
        SystemShutDown();
    }
    //=================== 提示音 =================================
    if(ui->checkBox_NfSound->checkState())
    {
        QtConcurrent::run(this, &MainWindow::Play_NFSound);
    }
    Waifu2x_Finished_manual();
}

void MainWindow::Waifu2x_Finished_manual()
{
    TimeCostTimer->stop();
    //================== 界面恢复 ===============================
    this->setAcceptDrops(1);
    ui->pushButton_Stop->setEnabled(0);
    ui->pushButton_Start->setEnabled(1);
    ui->groupBox_Input->setEnabled(1);
    ui->pushButton_ClearList->setEnabled(1);
    ui->pushButton_RemoveItem->setEnabled(1);
    ui->groupBox_Engine->setEnabled(1);
    ui->groupBox_ScaleRaton_DenoiseLevel->setEnabled(1);
    ui->checkBox_OptGIF->setEnabled(1);
    ui->checkBox_SaveAsJPG->setEnabled(1);
    if(ui->checkBox_SaveAsJPG->checkState())
    {
        ui->checkBox_CompressJPG->setEnabled(1);
    }
    ui->checkBox_DelOriginal->setEnabled(1);
    ui->checkBox_ReProcFinFiles->setEnabled(1);
    //=================== 数值恢复 ================================
    ThreadNumMax = 0;
    ThreadNumRunning = 0;
    Progressbar_MaxVal = 0;
    Progressbar_CurrentVal = 0;
    TaskNumTotal=0;
    TaskNumFinished=0;
    NewTaskFinished=false;
    ETA=0;
    TimeCost=0;
}

int MainWindow::Waifu2x_Compatibility_Test()
{
    emit Send_TextBrowser_NewMessage("Compatibility test is ongoing, please wait.");
    //===============
    QString Current_Path = qApp->applicationDirPath();
    QString InputPath = Current_Path + "/Compatibility_Test/Compatibility_Test.jpg";
    QString OutputPath = Current_Path + "/Compatibility_Test/res.jpg";
    QFile::remove(OutputPath);
    //==============
    QString Waifu2x_folder_path = Current_Path + "/waifu2x-ncnn-vulkan";
    QString program = Waifu2x_folder_path + "/waifu2x-ncnn-vulkan.exe";
    QString model_path = Waifu2x_folder_path+"/models-upconv_7_anime_style_art_rgb";
    QProcess *Waifu2x = new QProcess();
    QString cmd = "\"" + program + "\"" + " -i " + "\"" + InputPath + "\"" + " -o " + "\"" + OutputPath + "\"" + " -s 2 -n 0 -t 50 -m " + "\"" + model_path + "\"" + " -j 1:1:1";
    Waifu2x->start(cmd);
    Waifu2x->waitForStarted();
    Waifu2x->waitForFinished();
    if(file_isFileExist(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-ncnn-vulkan: Yes");
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-ncnn-vulkan: No.");
    }
    QFile::remove(OutputPath);
    //================
    Waifu2x_folder_path = Current_Path + "/waifu2x-converter";
    program = Waifu2x_folder_path + "/waifu2x-converter_x64.exe";
    model_path= Waifu2x_folder_path + "/models_rgb";
    QString Denoise_cmd = " --noise_level 1 ";
    cmd = "\"" + program + "\"" + " -i " + "\"" + InputPath + "\"" + " -o " + "\"" + OutputPath + "\"" + " --scale_ratio 2" + Denoise_cmd + " --model_dir " + "\"" + model_path + "\"";
    Waifu2x->start(cmd);
    Waifu2x->waitForStarted();
    Waifu2x->waitForFinished();
    if(file_isFileExist(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-converter: Yes.");
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-converter: No.");
    }
    QFile::remove(OutputPath);
    //===============
    Current_Path = qApp->applicationDirPath();
    QString Anime4k_folder_path = Current_Path + "/Anime4K";
    program = Anime4k_folder_path + "/Anime4K.jar";
    cmd = "java -jar \"" + program + "\" \"" + InputPath + "\" \"" + OutputPath + "\" 2";
    Waifu2x->start(cmd);
    Waifu2x->waitForStarted();
    Waifu2x->waitForFinished();
    if(file_isFileExist(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with Anime4k: Yes.");
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with Anime4k: No.");
    }
    QFile::remove(OutputPath);
    //===============
    emit Send_TextBrowser_NewMessage("Compatibility test is complete!");
    emit Send_Waifu2x_Compatibility_Test_finished();
    return 0;
}

int MainWindow::Waifu2x_Compatibility_Test_finished()
{
    ui->pushButton_Start->setEnabled(1);
}

