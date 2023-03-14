#include "Kalman.h"
#include "inv_mpu.h"

extern float pitch,roll,yaw;
 
float Kalman_Filter_U(float val)  //������ѹ�Ŀ������˲�
{
    static float Xk1 = 0.1;
    static float eESTK1 = 0.5;           //�������
    static float eMEAK1 = 0.2;           //�������
    static float Kk1;
    
    Kk1 = eESTK1 / (eESTK1 + eMEAK1);
    Xk1 = Xk1 + Kk1 * (val - Xk1);
    eESTK1 = (1 - Kk1) * eESTK1;
    
    return Xk1;
}

float Kalman_Filter_Dis(float val)  //��������������Ŀ������˲�
{
    static float Xk = 1;
    static float eESTK = 0.5;           //�������
    static float eMEAK = 2;           //�������
    static float Kk;
    
    Kk = eESTK / (eESTK + eMEAK);
    Xk = Xk + Kk * (val - Xk);
    eESTK = (1 - Kk) * eESTK;
    
    return Xk;
}

