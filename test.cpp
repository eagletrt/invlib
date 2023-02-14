#define CANLIB_TIMESTAMP
extern "C" {
#include "inverter.h"
}
#include "inverter.pb.h"
#include "inverter_proto.h"

#include <stdio.h>

int main() {
  inverter_message_INV_RESPONSE data;
  inverter::InverterVec inverter;
  inverter_data_t inverters_data[2];
  inverter_serialize(&data, 0, &inverter, inverters_data);
  network_enums net_enums;
  network_signals net_signals;
  network_strings net_strings;
  inverter_deserialize(&net_enums, &net_signals, &net_strings, &inverter, 100,
                       "_L");
  return 0;
}