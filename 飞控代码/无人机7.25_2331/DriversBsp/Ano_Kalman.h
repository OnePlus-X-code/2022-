// 1. 结构体类型定义
typedef struct
{
    float Last_P;    // 上次估算协方差 初始化值为0.02
    float Now_P;     // 当前估算协方差 初始化值为0
    float out;       // 卡尔曼滤波器输出 初始化值为0
    float Kg;        // 卡尔曼增益 初始化值为0
    float Q;         // 过程噪声协方差 初始化值为0.001
    float R;         // 观测噪声协方差 初始化值为0.543
} KFP;               // 卡尔曼滤波参数 Kalman Filter parameter

// #: 卡尔曼滤波参数_超声波距离
extern KFP KFP_Distance;
extern KFP KFP_Height;
// 滤波函数
float kalmanFilter( KFP* kfp, float input );
