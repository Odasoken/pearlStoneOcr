# This Python file uses the following encoding: utf-8
import sys
from PySide6.QtWidgets import QApplication
from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton,  QPlainTextEdit, QMessageBox, QLabel
from PySide6.QtGui import QIcon, QPixmap, QImage
from PySide6.QtWidgets import QFileDialog
from PySide6.QtGui import QGuiApplication
from PySide6.QtCore import Qt

import pytesseract
from PIL import Image
import numpy as np
import os

#ocr识别方法
def pyChineseocr(url):

        # 读入图像
        img = Image.open(url)
        # 转换成灰度图像
        img_gray = img.convert('L')

        # 转换成numpy数组
        img_arr = np.array(img_gray)

        # 设置阈值将图片二值化
        threshold = np.mean(img_arr) * 0.65
        img_binary = np.where(img_arr > threshold, 255, 0)

        # 转换成PIL图像
        img_result = Image.fromarray(np.uint8(img_binary))

        # 调用Tesseract库进行OCR识别（
        # 中文识别
        text = pytesseract.image_to_string(img_result, lang='chi_sim')
        # 英文
        # text = pytesseract.image_to_string(img_result, lang='eng')
        return text

if __name__ == "__main__":

    def showMessage(msg):
       QMessageBox.about(window,"Tips",msg)
    #刷新图片
    def refreshImg(url):
        image = QImage(url)
        ori_width =  image.size().width()
        ori_height =  image.size().height()

        m_width = 400
        m_height = 400

        pro = 1
        if (ori_width / m_width >= ori_height / m_height):

            pro = ori_width / m_width
        else:

            pro = ori_height / m_height

        scale_width = ori_width / pro
        scale_height = ori_height / pro
        scale_image =  QImage()
        scale_image = image.scaled(scale_width, scale_height, Qt.IgnoreAspectRatio)#Qt.KeepAspectRatio




        label.setPixmap(QPixmap.fromImage(scale_image));
#        label.resize(w,h)
    def handleSelectImg():
        filePath, _  = QFileDialog.getOpenFileName(
                    window,             # 父窗口对象
                    "选择图片", # 标题
                    r"d:\\data",        # 起始目录
                    "图片类型 (*.png *.jpg *.bmp *.jpeg)" # 选择类型过滤项，过滤内容在括号中
                )
        textEdit.setPlainText(filePath)
        if filePath:
            refreshImg(filePath)
    #复制内容
    def handleClickToCopy():
        clipboard = QGuiApplication.clipboard()
        # 获取剪贴板内容
        #originalText = cb.text()
        text = textEdit2.toPlainText()
        # 设置剪贴板内容
        clipboard.setText(text)
    def handleRegnize():
        url = textEdit.toPlainText()
        # 去除图片路径中的空格
        url = url.replace(" ", "")
        if url :



            print("开始处理",url)
            text = pyChineseocr(url)
            print(text)
            textEdit2.setPlainText(text)

        else:
            showMessage("请输入图片路径")


    app = QApplication([])

    window = QMainWindow()
    window.resize(800, 800)
    window.move(300, 310)
    window.setWindowTitle('图片文字识别')

    textEdit = QPlainTextEdit(window)
    textEdit.setPlaceholderText("请输入图片路径")
    textEdit.move(10,25)
    textEdit.resize(300,50)

    textEdit2 = QPlainTextEdit(window)
    textEdit2.setPlaceholderText("识别内容")
    textEdit2.move(10,110)
    textEdit2.resize(300,300)

    button1 = QPushButton('选择图片', window)
    button1.move(330,30)
    button1.clicked.connect(handleSelectImg)

    label = QLabel('', window)
    label.move(330,80)
    label.resize(400,400)



    button = QPushButton('识别', window)
    button.move(30,80)
    button.clicked.connect(handleRegnize)

    button2 = QPushButton('复制内容', window)
    button2.clicked.connect(handleClickToCopy)

    button2.move(230,80)
    button2.resize(80,30)
    window.show()

    app.exec_()
