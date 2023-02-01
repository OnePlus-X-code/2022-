#include "Ano_Kalman.h"

// 2. 以高度为例 定义卡尔曼结构体并初始化参数
KFP KFP_Distance = { 0.02, 0, 0, 0, 0.001, 0.1 };
KFP KFP_Height   = { 0.02, 0, 0, 0, 0.0005, 0.2 };
/**
 *卡尔曼滤波器
 * @param KFP *kfp 卡尔曼结构体参数
 * @param float input 需要滤波的参数的测量值（即传感器的采集值）
 *
 * @return 滤波后的参数（最优值）
 */

float kalmanFilter( KFP* kfp, float input )
{
    //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
    kfp->Now_P = kfp->Last_P + kfp->Q;
    //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 +
    //观测噪声协方差）
    kfp->Kg = kfp->Now_P / ( kfp->Now_P + kfp->R );
    //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值
    //-状态变量的预测值）
    kfp->out = kfp->out +
               kfp->Kg * ( input - kfp->out );    //因为这一次的预测值就是上一次的输出值
    //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
    kfp->Last_P = ( 1 - kfp->Kg ) * kfp->Now_P;
    return kfp->out;
}
