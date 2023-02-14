#include "inverter_proto.h"

void inverter_serialize(inverter_message_INV_RESPONSE *inv_data, uint16_t id,
                        inverter::InverterVec *inverter,
                        inverter_data_t inverters_data[2]) {
  inverter_data_t *inv = id == get_inverter_response_id_left()
                             ? &inverters_data[INV_IDX_LEFT]
                             : &inverters_data[INV_IDX_RIGHT];
  switch (inv_data->reg_id) {
  case INV_REG_MOT_TEMP: {
    auto new_el = inverter->add_inverter_motor_temp();
#ifdef CANLIB_TIMESTAMP
    new_el->set__inner_timestamp(inv->inverter_motor_temp._timestamp);
#endif // CANLIB_TIMESTAMP
    new_el->set_motor_temp(inv->inverter_motor_temp.motor_temp);
    break;
  }
  case INV_REG_INV_TEMP: {
    auto new_el = inverter->add_inverter_temp();
#ifdef CANLIB_TIMESTAMP
    new_el->set__inner_timestamp(inv->inverter_temp._timestamp);
#endif // CANLIB_TIMESTAMP
    new_el->set_temperature(inv->inverter_temp.temp);
    break;
  }
  case INV_REG_SPEED: {
    auto new_el = inverter->add_inverter_speed();
#ifdef CANLIB_TIMESTAMP
    new_el->set__inner_timestamp(inv->inverter_speed._timestamp);
#endif // CANLIB_TIMESTAMP
    new_el->set_speed(inv->inverter_speed.speed);
    break;
  }
  case INV_REG_STATUS: {
    auto new_el = inverter->add_inverter_status();
#ifdef CANLIB_TIMESTAMP
    new_el->set__inner_timestamp(inv->inverter_status._timestamp);
#endif // CANLIB_TIMESTAMP
    new_el->set_status((uint32_t)inv->inverter_status.status);
    break;
  }
  case INV_REG_ERRORS: {
    auto new_el = inverter->add_inverter_errors();
#ifdef CANLIB_TIMESTAMP
    new_el->set__inner_timestamp(inv->inverter_errors._timestamp);
#endif // CANLIB_TIMESTAMP
    new_el->set_errors((uint32_t)inv->inverter_errors.errors);
    break;
  }
  case INV_REG_TORQUECMD: {
    auto new_el = inverter->add_inverter_das_torque();
#ifdef CANLIB_TIMESTAMP
    new_el->set__inner_timestamp(inv->inverter_das_torque._timestamp);
#endif // CANLIB_TIMESTAMP
    new_el->set_torque((double)inv->inverter_das_torque.torque);
    break;
  }
  case INV_REG_I_CMD: {
    auto new_el = inverter->add_inverter_inv_torque();
#ifdef CANLIB_TIMESTAMP
    new_el->set__inner_timestamp(inv->inverter_inv_torque._timestamp);
#endif // CANLIB_TIMESTAMP
    new_el->set_torque((double)inv->inverter_inv_torque.torque);
    break;
  }
  case INV_REG_I_ACTUAL: {
    auto new_el = inverter->add_inverter_inv_torque_actual();
#ifdef CANLIB_TIMESTAMP
    new_el->set__inner_timestamp(inv->inverter_inv_torque_actual._timestamp);
#endif // CANLIB_TIMESTAMP
    new_el->set_torque((double)inv->inverter_inv_torque_actual.torque);
    break;
  }
  }
}

void inverter_deserialize(network_enums *net_enums,
                          network_signals *net_signals,
                          network_strings *net_strings,
                          const inverter::InverterVec *inverter,
                          uint64_t resample_us, const std::string &inv_suffix) {
  for (int i = 0; i < inverter->inverter_motor_temp_size(); i++) {
#ifdef CANLIB_TIMESTAMP
    static uint64_t last_timestamp = 0;
    if (inverter->inverter_motor_temp(i)._inner_timestamp() - last_timestamp <
        resample_us)
      continue;
    else
      last_timestamp = inverter->inverter_motor_temp(i)._inner_timestamp();
    (*net_signals)["INV" + inv_suffix]["_timestamp"].push(
        inverter->inverter_motor_temp(i)._inner_timestamp());
#endif // CANLIB_TIMESTAMP
    (*net_signals)["INV" + inv_suffix]["motor_temp"].push(
        inverter->inverter_motor_temp(i).motor_temp());
  }
  for (int i = 0; i < inverter->inverter_temp_size(); i++) {
#ifdef CANLIB_TIMESTAMP
    static uint64_t last_timestamp = 0;
    if (inverter->inverter_temp(i)._inner_timestamp() - last_timestamp <
        resample_us)
      continue;
    else
      last_timestamp = inverter->inverter_temp(i)._inner_timestamp();
    (*net_signals)["INV" + inv_suffix]["_timestamp"].push(
        inverter->inverter_temp(i)._inner_timestamp());
#endif // CANLIB_TIMESTAMP
    (*net_signals)["INV" + inv_suffix]["temp"].push(
        inverter->inverter_temp(i).temperature());
  }
  for (int i = 0; i < inverter->inverter_speed_size(); i++) {
#ifdef CANLIB_TIMESTAMP
    static uint64_t last_timestamp = 0;
    if (inverter->inverter_speed(i)._inner_timestamp() - last_timestamp <
        resample_us)
      continue;
    else
      last_timestamp = inverter->inverter_speed(i)._inner_timestamp();
    (*net_signals)["INV" + inv_suffix]["_timestamp"].push(
        inverter->inverter_speed(i)._inner_timestamp());
#endif // CANLIB_TIMESTAMP
    (*net_signals)["INV" + inv_suffix]["speed"].push(
        inverter->inverter_speed(i).speed());
  }
  for (int i = 0; i < inverter->inverter_status_size(); i++) {
#ifdef CANLIB_TIMESTAMP
    static uint64_t last_timestamp = 0;
    if (inverter->inverter_status(i)._inner_timestamp() - last_timestamp <
        resample_us)
      continue;
    else
      last_timestamp = inverter->inverter_status(i)._inner_timestamp();
    (*net_signals)["INV" + inv_suffix]["_timestamp"].push(
        inverter->inverter_status(i)._inner_timestamp());
#endif // CANLIB_TIMESTAMP
    (*net_signals)["INV" + inv_suffix]["status"].push(
        (uint32_t)inverter->inverter_status(i).status());
  }
  for (int i = 0; i < inverter->inverter_errors_size(); i++) {
#ifdef CANLIB_TIMESTAMP
    static uint64_t last_timestamp = 0;
    if (inverter->inverter_errors(i)._inner_timestamp() - last_timestamp <
        resample_us)
      continue;
    else
      last_timestamp = inverter->inverter_errors(i)._inner_timestamp();
    (*net_signals)["INV" + inv_suffix]["_timestamp"].push(
        inverter->inverter_errors(i)._inner_timestamp());
#endif // CANLIB_TIMESTAMP
    (*net_signals)["INV" + inv_suffix]["errors"].push(
        (uint32_t)inverter->inverter_errors(i).errors());
  }
  for (int i = 0; i < inverter->inverter_das_torque_size(); i++) {
#ifdef CANLIB_TIMESTAMP
    static uint64_t last_timestamp = 0;
    if (inverter->inverter_das_torque(i)._inner_timestamp() - last_timestamp <
        resample_us)
      continue;
    else
      last_timestamp = inverter->inverter_das_torque(i)._inner_timestamp();
    (*net_signals)["INV" + inv_suffix]["_timestamp"].push(
        inverter->inverter_das_torque(i)._inner_timestamp());
#endif // CANLIB_TIMESTAMP
    (*net_signals)["INV" + inv_suffix]["das_torque"].push(
        inverter->inverter_das_torque(i).torque());
  }
  for (int i = 0; i < inverter->inverter_inv_torque_size(); i++) {
#ifdef CANLIB_TIMESTAMP
    static uint64_t last_timestamp = 0;
    if (inverter->inverter_inv_torque(i)._inner_timestamp() - last_timestamp <
        resample_us)
      continue;
    else
      last_timestamp = inverter->inverter_inv_torque(i)._inner_timestamp();
    (*net_signals)["INV" + inv_suffix]["_timestamp"].push(
        inverter->inverter_inv_torque(i)._inner_timestamp());
#endif // CANLIB_TIMESTAMP
    (*net_signals)["INV" + inv_suffix]["inv_torque"].push(
        inverter->inverter_inv_torque(i).torque());
  }
  for (int i = 0; i < inverter->inverter_inv_torque_actual_size(); i++) {
#ifdef CANLIB_TIMESTAMP
    static uint64_t last_timestamp = 0;
    if (inverter->inverter_inv_torque_actual(i)._inner_timestamp() -
            last_timestamp <
        resample_us)
      continue;
    else
      last_timestamp =
          inverter->inverter_inv_torque_actual(i)._inner_timestamp();
    (*net_signals)["INV" + inv_suffix]["_timestamp"].push(
        inverter->inverter_inv_torque_actual(i)._inner_timestamp());
#endif // CANLIB_TIMESTAMP
    (*net_signals)["INV" + inv_suffix]["inv_torque_actual"].push(
        inverter->inverter_inv_torque_actual(i).torque());
  }
}
