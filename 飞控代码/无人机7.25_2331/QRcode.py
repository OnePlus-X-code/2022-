# Untitled - By: Accelerator - 周日 7月 17 2022

import sensor, image, time, lcd
from pyb import UART

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.VGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)  # 必须关闭此功能，以防止图像冲洗…

clock = time.clock()

Uart = UART(3, 500000)
Uart.init(500000)

class QRcode(object):#数据太少了用不到这个类
    QRcodemessage = 0


QRcode = QRcode()

def UserQRcodeDataPack(flag, Message, X, Y):
    Temp_Message = int(Message)
    Temp_X = int(X) - image.width()/2
    Temp_Y = image.height()/2 - int(Y)
    QRcode_data = bytearray(
        [0xAA, 0x22, flag, Temp_Message, Temp_X >> 8, Temp_X, Temp_Y >> 8, Temp_Y, 0xFF])
    return QRcode_data

def ScanQRcode(img):
    QRcodes=img.find_qrcodes()
    Len_QRcodes=len(QRcodes)

    if Len_QRcodes == 0:
        Pack_QRcode = UserQRcodeDataPack(0, 0, image.width()/2, image.height()/2)
        Uart.write(Pack_QRcode)
    else:
        for qrcode in QRcodes:
            img.draw_rectangle(qrcode.rect(), color=255)
            print('信息', qrcode.payload())
            Pack_QRcode = UserQRcodeDataPack(1, qrcode.payload(), qrcode.x()+qrcode.w()/2, qrcode.y()+qrcode.h()/2)
            Uart.write(Pack_QRcode)



# =====================Variable===========================
QRcode_Start_Flag = bytearray([0XAA, 0X20])
QRcode_Stop_Flag = bytearray([0XAA, 0X21])
QRcode_State = 0
# =====================Main===========================
while(True):
    clock.tick()
    img = sensor.snapshot()
    if Uart.any():
        cmd = Uart.read()
        if a == QRcode_Start_Flag:
            QRcode_State = 1
        elif a == QRcode_Stop_Flag:
            QRcode_State = 0
    if QRcode_State == 1:
        ScanQRcode(img)
