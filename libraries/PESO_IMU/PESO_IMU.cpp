#include "PESO_IMU.h"

IMU::IMU()
{
}

int IMU::initialize()
{
  if (mpu_init())
    return -1;
  if (mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
    return -2;
  if (mpu_set_sample_rate(100))
    return -3;
  if (mpu_set_accel_fsr(16))
    return -4;
  if (mpu_get_gyro_sens(&gyro_sens))
    return -5;
  if (mpu_get_accel_sens(&aa_sens))
    return -6;
  if (dmp_load_motion_driver_firmware())
    return -7;
  else
  {
    //dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
    
    if (dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL))
      return -8;
    if (dmp_set_fifo_rate(50))
      return -9;
    if (mpu_set_dmp_state(1))
      return -10;
    if (mpu_get_int_status(&mpuIntStatus))
      return -11;
    Serial.println("DMP On");
  }

  return 0;
}

void IMU::update()
{
  mpu_get_int_status(&mpuIntStatus);
  
  if ((mpuIntStatus & 0x10))
  {
    Serial.println("fifo reset");
    mpu_reset_fifo();
    return;
  }
  else if ((mpuIntStatus & 0x01))
  {
    dmp_read_fifo(gyro, aa, q, &timestamp, &sensors, &more);
    mpu_get_temperature(&temperature, &timestamp);
    timestamp = millis();
  }
}
