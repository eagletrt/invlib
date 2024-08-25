#include "inverter_conversions.h"

#include <inttypes.h>
#include <math.h>

#include "defines.h"

float convert_n_actual_filt(float val) {
  return val * (INV_MAX_SPEED / INT16_MAX) * 10 / 4.5 * M_PI / 30.0f;
}
float convert_t_motor(float val) { return (val - 9393.9f) / 55.1f; }
float convert_t_igbt(float val) {
  return -43.23745 + 0.01073427 * val - 5.523417e-7 * pow(val, 2) + 1.330787e-11 * pow(val, 3);
}
float convert_iq_act(float val) { return val * 0.54; }
float convert_m_set_dig_iq(float val) { return val / INT16_MAX * 169.7 * 0.54; }
