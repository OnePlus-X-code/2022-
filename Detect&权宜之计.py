import sensor, image, time, math, mjpeg
from pyb import UART
from pyb import LED

# ==============================Circle==============================
ROI = (240, 180, 160, 120)


def UserCircleDataPack(flag, X, Y):
    Temp_X = int(X)
    Temp_Y = int(Y)
    Circle_data = bytearray(
        [0xAA, 0x31, flag, Temp_X >> 8, Temp_X, Temp_Y >> 8, Temp_Y, 0xFF])
    return Circle_data

Circle_State_Counter = 0
def CircleCheck(img):
    global Circle_State_Counter

    Circles = img.find_circles(threshold=5000,
                               x_margin=5,
                               y_margin=5,
                               r_margin=30)
    Len_Circles = len(Circles)
    if Len_Circles == 0:
        Circle_State_Counter = 0
        Pack_Circle = UserCircleDataPack(0, 0, 0)
        Uart.write(Pack_Circle)
    else:
        if Circle_State_Counter < 5:
            Circle_State_Counter = Circle_State_Counter + 1
        else:
            print("圆")
            img.draw_circle(Circles[0].x(),
                            Circles[0].y(),
                            Circles[0].r(),
                            color=(255, 0, 0))
            print(4*(Circles[0].x() - 80),4*(60 - Circles[0].y()))
            Pack_Circle = UserCircleDataPack(1, 4*(Circles[0].x() - 80),
                                            4*(60 - Circles[0].y()))
            Uart.write(Pack_Circle)

# ==============================ColorRecognition==============================

# 红色阈值
red_threshold = (5, 89, 19, 127, -60, 96)
#(5, 91, 34, 127, -60, 96)

# 蓝色阈值
blue_threshold = (0, 55, -16, 23, -128, -5)

# 识别色块的目标阈值
thresholds = [red_threshold,
              blue_threshold]



# 颜色1: 红色的颜色代码
red_color_code = 1
# 颜色2: 蓝色的颜色代码
blue_color_code = 2

def FindMax(blobs):
    max_size = 1
    if blobs:
        max_blob = 0
        for blob in blobs:
            blob_size = blob.w() * blob.h()
            if ((blob_size > max_size) & (blob_size > 50)):
                if (math.fabs(blob.w() / blob.h() - 1) < 2.0):
                    max_blob = blob
                    max_size = blob.w() * blob.h()
        return max_blob
    else:
        return None

def ColorRecognition(temp_blob):
    if temp_blob:
        color_code = temp_blob[8]
        if color_code == red_color_code:
            return 1
        elif color_code == blue_color_code:
            return 2
    return 0

# =====================ShapeRecognition===========================
def UserShapeDataPack(flag, color, shape, X, Y):
    Temp_Color=int(color)
    Temp_X = int(X)
    Temp_Y = int(Y)
    Shape_data = bytearray(
        [0xAA, 0x32, flag, Temp_Color, shape, Temp_X >> 8, Temp_X, Temp_Y >> 8, Temp_Y, 0xFF])
    return Shape_data


def ShapeRecognition2(temp_blob):
    if temp_blob:
        x=temp_blob.cx()- 160
        y=120-temp_blob.cy()
        print(x,y)
        Pack_Shape = UserShapeDataPack(1, Target_Color, Target_Shape,x,y)
        Uart.write(Pack_Shape)



def ShapeRecognition(temp_blob):
    if temp_blob:
        if len(img.find_circles(threshold=5000))!=0:
            return 3
        elif len(img.find_rects(threshold=18000))!=0:
            return 1
        else:
            return 2
    else:
        return 0


def UserDetectDataPack(flag, color, shape):
    Detect_data = bytearray(
        [0xAA, 0x33, flag, color, shape, 0xFF])
    return Detect_data


Detect_State_Counter = 0
Detect_Color = -1
Detect_Shape = -1

def Detect(temp_blob):
    global Detect_State_Counter
    global Detect_Color
    global Detect_Shape
    if temp_blob:
        temp_color = ColorRecognition(temp_blob)
        temp_shape = ShapeRecognition(temp_blob)
        if Detect_Color == temp_color and Detect_Shape == temp_shape and temp_shape!=0 and temp_color!=0:
            if Detect_State_Counter >= 5:
                print("color:",Detect_Color)
                print("shape:",Detect_Shape)
                Pack_Detect = UserDetectDataPack(1, Detect_Color, Detect_Shape)
                Uart.write(Pack_Detect)
            else:
                Detect_State_Counter=Detect_State_Counter+1
        else:
            Detect_State_Counter = 0
            Detect_Color = temp_color
            Detect_Shape = temp_shape



# =====================Variable===========================
Circle_State = 0
Detect_State = 0
Shape_State = 0
#frame_start_flag = 1 #接收命令的临时变量，无需注意

Target_Shape = 0 #1为方形，2为三角形，3为圆形
Target_Color = 0 #1为red，4为blue


# =====================Init===========================
sensor.reset()
sensor.set_pixformat(sensor.RGB565) #设置为彩色
sensor.set_framesize(sensor.QVGA) #设置清晰度，ShapeRecognition参数与之有关，需要跟着改变
sensor.skip_frames(time = 2000) #跳过前2000ms的图像
sensor.set_auto_gain(False) # 关闭自动自动增益。默认开启的。
sensor.set_auto_whitebal(False) #关闭白平衡。在颜色识别中，一定要关闭白平衡。

clock = time.clock() # 初始化时钟

Uart = UART(3, 500000)
Uart.init(500000)

led1=LED(1)
led3=LED(3)


# ========================main=============================
while (True):
    clock.tick()
    img = sensor.snapshot()
    if Uart.any():
        cmds = Uart.read()
        for cmd in cmds:
                if (cmd == 0xAA):
                    frame_start_flag = 1
                elif (frame_start_flag == 1 and cmd != 0xAA):
                    if (cmd == 0x30):
                        Circle_State = 1
                        sensor.set_framesize(sensor.QQVGA)
                        #m1 = mjpeg.Mjpeg("example1.mjpeg")#录像文件初始化
                    elif cmd == 0x31:
                        Circle_State = 0
                        #m1.close(clock.fps())
                    elif cmd == 0x32:
                        sensor.set_framesize(sensor.QVGA)
                        Detect_State = 1
                        #m2 = mjpeg.Mjpeg("example1.mjpeg")#录像文件初始化
                    elif cmd == 0x33:
                        sensor.set_framesize(sensor.QVGA)
                        Detect_State = 0
                        #m2.close(clock.fps())
                    elif cmd == 0x34:
                        sensor.set_framesize(sensor.QVGA)
                        Shape_State = 1
                        Target_Shape = 1
                        Target_Color = 1
                    elif cmd == 0x35:
                        sensor.set_framesize(sensor.QVGA)
                        Shape_State = 1
                        Target_Shape = 2
                        Target_Color = 1
                    elif cmd == 0x36:
                        sensor.set_framesize(sensor.QVGA)
                        Shape_State = 1
                        Target_Shape = 3
                        Target_Color = 1
                    elif cmd == 0x37:
                        sensor.set_framesize(sensor.QVGA)
                        Shape_State = 1
                        Target_Shape = 1
                        Target_Color = 2
                    elif cmd == 0x38:
                        sensor.set_framesize(sensor.QVGA)
                        Shape_State = 1
                        Target_Shape = 2
                        Target_Color = 2
                    elif cmd == 0x39:
                        sensor.set_framesize(sensor.QVGA)
                        Shape_State = 1
                        Target_Shape = 3
                        Target_Color = 2
                    frame_start_flag = 0

    if Circle_State == 1:
        CircleCheck(img)


    if Shape_State == 1 or Detect_State == 1:
        blobs = img.find_blobs(thresholds, pixels_threshold=200, area_threshold=200)
        blob=FindMax(blobs)
        if Shape_State == 1:
            ShapeRecognition2(blob)
        if Detect_State == 1:
            Detect(blob)

# ***************** End of File *******************
