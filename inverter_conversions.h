#ifndef _INVERTER_CONVERSIONS_H_
#define _INVERTER_CONVERSIONS_H_

float convert_n_actual_filt(float val);
float convert_t_motor(float val);
float convert_t_igbt(float val);
float convert_iq_act(float val);
float convert_m_set_dig_iq(float val);

#endif  // _INVERTER_CONVERSIONS_H_