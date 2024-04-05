#include "inverter_conversions.h"

#include <inttypes.h>
#include <math.h>

#include "defines.h"

float IGBT_TEMP_COEFFICIENT[6] = {
    (float)3.58282057e-18,  (float)-4.14165530e-13, (float)1.90916947e-08,
    (float)-4.38539758e-04, (float)5.02717412e+00,  (float)-2.30219791e+04};

float convert_n_actual_filt(float val) {
  return val * (INV_MAX_SPEED / UINT16_MAX) * 4.5f / 10.0f;
}
float convert_t_motor(float val) { return (val - 9393.9f) / 55.1f; }
float convert_t_igbt(float val) {
  return (powf((val), 5) * IGBT_TEMP_COEFFICIENT[0]) +
         (powf((val), 4) * IGBT_TEMP_COEFFICIENT[1]) +
         (powf((val), 3) * IGBT_TEMP_COEFFICIENT[2]) +
         (powf((val), 2) * IGBT_TEMP_COEFFICIENT[3]) +
         (val * IGBT_TEMP_COEFFICIENT[4]) + IGBT_TEMP_COEFFICIENT[5];
}
float convert_iq_act(float val) { return val * M_PI / 2.0; }
float convert_m_set_dig_iq(float val) { return val / INT16_MAX * 400.0 * 0.45; }
