#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QImage>
#include <QApplication>
#include <QClipboard>
#include <QThread>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <QSettings>

//Tesseract


QImage Binaryzation(const QImage &img);
Pix* QImage2Pix(const QImage &image);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     //UI settings
     ui->lineEdit->setPlaceholderText("Please Input Image path");
     ui->textEdit->setPlaceholderText("Recognized Text");
     QStringList strList;
     strList<<"中文"<<"한국어"<<"Other";
     ui->comboBox->addItems(strList);

    //Load settings
      QSettings settings("xtres_languages", "otresApp");
      int idtx = settings.value("xt_lang_key").toInt();
      if(idtx > 0)
      {
         langIndx = idtx;
         if(idtx == 2)
         {

             QStringList list = settings.value("xt_other_lang_path").toStringList();
            QString lastpath =  list.first();
            refreshLangugePath(lastpath);

         }
      }
     ui->comboBox->setCurrentIndex(langIndx);


}

MainWindow::~MainWindow()
{
    delete ui;
}
/* update image*/
void MainWindow::refreshImage(QString filePath)
{
    QImage *img =  new QImage(filePath);
    if(img->isNull()) return;
    int ori_width = img->size().width();
    int ori_height =  img->size().height();

    int m_width = 320;
    int m_height = 410;
    int pro = 1;
    if (ori_width / m_width >= ori_height / m_height){

        pro = ori_width / m_width;
    }else{
         pro = ori_height / m_height;
    }



   int scale_width = ori_width / pro;
    int scale_height = ori_height / pro;
   QImage scale_image;
   scale_image = img->scaled(scale_width, scale_height, Qt::IgnoreAspectRatio);//Qt.KeepAspectRatio
   ui->label->setPixmap(QPixmap::fromImage(scale_image));
}
/* recognize image*/
void MainWindow::recognizeImage(QString filePath){
    qDebug() << filePath;
    if(langIndx == 2 && (otherLangPath.isNull() || otherLangPath.length() <= 0))
    {
        selectOtherLanguges();
        return;
    }


     QImage *img =  new QImage(filePath);
     QImage grayImage = img->convertToFormat(QImage::Format_Grayscale8);
     QImage tresholdImage = Binaryzation(grayImage);
//     QImage::threshold();

//
     tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
     // Initialize tesseract-ocr with lang, with specifying tessdata path
     QString dataPath = QApplication::applicationDirPath() + "/../resources/tessdata";
     qDebug()<< dataPath;
     std::string str = dataPath.toStdString();
     const char *dataStr = str.c_str();
     qDebug()<< dataStr;//+ kor
     const char *langStr =  "chi_sim";
     if(langIndx == 1)
     {
         langStr =  "kor";
     }
     else if(langIndx == 2)
     {
        str = langDir.toStdString();
        dataStr = str.c_str();

        std::string str2 = langName.toStdString();
        langStr = str2.c_str();

        qDebug()<<"hello"<<langDir<<langName;

     }
     if (api->Init(dataStr, langStr))
     {
         showMessage("Could not initialize tesseract.");
         qDebug()<< "Could not initialize tesseract.\n";
         return;
     }

     Pix *pix_image  = QImage2Pix(tresholdImage);
     api->SetImage(pix_image);
     char *outText;
     outText = api->GetUTF8Text();
     QString trText =  QString(outText);
     ui->textEdit->setText(trText);
     qDebug()<<outText;
     api->End();


}

/* select Other Languges*/
void MainWindow::selectOtherLanguges(){
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this,"Select trained language data","~/documents","Trained data(*.traineddata)");
     refreshLangugePath(filePath);
    QSettings settings("xtres_languages", "otresApp");
    settings.setValue("xt_other_lang_path",filePath);
    qDebug() << filePath;
}

void MainWindow::refreshLangugePath(QString path){
   otherLangPath = path;
   if(!otherLangPath.isEmpty())
   {
       QStringList list = otherLangPath.split("/");
       if( list.length() > 1)
       {
           QString file = list.last();
           QString *fullpath = new QString(otherLangPath);
           QString langDir  = fullpath->replace(file,"");
           QString langName  = file.replace(".traineddata","",Qt::CaseInsensitive);
           this->langDir = langDir;
           this->langName = langName;
           qDebug()<<"hello"<<langDir<<langName;
       }


   }


}

void MainWindow::showMessage(QString msg){
    ui->tipsLabel->setText(msg);
    QThread::sleep(1);

//    ui->tipsLabel->setText("");

}

void MainWindow::on_selectButton_clicked()
{
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this,"Select Image","~/documents","Image Types(*.png *.jpg *.bmp *.jpeg)");
    qDebug() << filePath;

    ui->lineEdit->setText(filePath);
    this->refreshImage(filePath);

}


void MainWindow::on_regnizeButton_clicked()
{
     ui->tipsLabel->setText("");
     QString fileUrl =  ui->lineEdit->text();
     if(fileUrl.isNull() || fileUrl.isEmpty() || fileUrl.length() == 0)
     {
         return;
     }
     recognizeImage(fileUrl);
}


void MainWindow::on_copyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    QString text = ui->textEdit->toPlainText();
    clipboard->setText(text);

    showMessage("copy success!");


}
///treshold image
QImage Binaryzation(const QImage &img)
{
    QImage imgCopy;

    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);
    } else {
        imgCopy = QImage(img);
    }
    uint8_t *rgb = imgCopy.bits();
    int newGray = 0;
    int gray = 0;
    int size = img.width() * img.height();
    for (int i = 0; i < size ; i++) {
        gray = (rgb[i * 3] + rgb[i * 3 + 1] + rgb[i * 3 + 2]) / 3;
        if (gray > 128)
            newGray = 255;
        else
            newGray = 0;
        rgb[i * 3] = newGray;
        rgb[i * 3 + 1] = newGray;
        rgb[i * 3 + 2] = newGray;
    }
    return imgCopy;
}

Pix* QImage2Pix(const QImage &image)
{
    Pix * pix;
    int width = image.width();
    int height = image.height();
    int depth = image.depth();
    pix = pixCreate(width, height, depth);
    if(image.isNull() )
    {
        qDebug() << "image is null";
        return nullptr;
    }
    if( image.colorCount() )
    {
        QVector<QRgb> table = image.colorTable();

        PIXCMAP * map = pixcmapCreate(8);

        int n = table.size();
        for(int i = 0; i < n; i++)
        {
            pixcmapAddColor(map, qRed(table[i]), qGreen(table[i]), qBlue(table[i]));
        }
        pixSetColormap(pix, map);
    }
    int bytePerLine = image.bytesPerLine();
    l_uint32* start = pixGetData(pix);
    l_int32 wpld = pixGetWpl(pix);
    if(image.format() == QImage::Format_Mono || image.format() == QImage::Format_Indexed8 || image.format() == QImage::Format_RGB888)
    {
        for(int i = 0; i < height; i++)
        {
            const uchar * lines = image.scanLine(i);
            uchar * lined = (uchar *)(start + wpld * i) ;
            memcpy(lined , lines, static_cast<size_t>(bytePerLine));
        }
    }
    else if (image.format() == QImage::Format_RGB32 || image.format() == QImage::Format_ARGB32)
    {
        qDebug() << "QImage::Format_RGB32";
        for(int i = 0; i < image.height(); i++)
        {
            const QRgb * lines = (const QRgb *)image.scanLine(i);
            l_uint32 * lined = start + wpld * i ;
            for(int j = 0; j < width; j ++)
            {
                uchar rval = qRed(lines[j]);
                uchar gval = qGreen(lines[j]);
                uchar bval = qBlue(lines[j]);
                l_uint32 pixel;
                composeRGBPixel(rval, gval, bval, &pixel);
                lined[j] = pixel;
            }
        }
    }
    return pix;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index != langIndx)
    {
        langIndx = index;
        QSettings settings("xtres_languages", "otresApp");
        settings.setValue("xt_lang_key",langIndx);

        if(index == 2){
            selectOtherLanguges();
        }
    }

}

