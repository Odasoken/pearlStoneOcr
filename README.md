
#  -  tesseract ocr识别application(python 和Qt c++)
This demo contains a python project to recognize text from picture with tesseract-OCR, and an implementation of Qt C++ GUI application.<br>
이 데모에는 tesseract-OCR을 사용하여 그림에서 텍스트를 인식하는 Python 프로젝트와 Qt C++ GUI 응용 프로그램 구현이 포함되어 있습니다.<br>
このデモには、tesseract-OCR を使用して画像からテキストを認識する Python プロジェクトと、Qt C++ GUI アプリケーションの実装が含まれています。<br>
 <img src="https://github.com/Odasoken/pyChineseOcrTest/blob/main/demo.jpg" width="50%" height="50%">
  <img src="https://github.com/Odasoken/pyChineseOcrTest/blob/main/demo2.jpg" width="50%" height="50%">

# 
```python
import pytesseract
from PIL import Image
import numpy as np
import os

#需要提前安装 pytesseract， pip install pytesseract
# Mac安装Tesseract库使用brew install  tesseract
# 下载中文tesseract ocr中文数据集 https://gitcode.net/mirrors/tesseract-ocr/tessdata/-/blob/master/chi_sim.traineddata）

print("******************************\n"
      "输入图片路径开始识别，输入0退出程序\n"
      "******************************\n")
#ocr识别方法
def pyChineseocr(url):

    try:

        # 读入图像
        img = Image.open(url)
    except Exception as e:
        return "null"
    finally:
        # 转换成灰度图像
        img_grey = img.convert('L')

        # 转换成numpy数组
        img_arr = np.array(img_grey)

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



while True:

    url = input("请输入图片路径:\n")
    # 去除图片路径中的空格
    url = url.replace(" ", "")

    if url == str(0):
        print("退出程序")
        break
    if os.path.exists(url):
        print("识别中....")
        text = pyChineseocr(url)
        print("✅识别结果：\n",text)
    else:
        print("图片不存在",url)
        continue
```
