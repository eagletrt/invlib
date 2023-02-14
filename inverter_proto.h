#ifndef INVLIB_INVERTER_PROTO_H
#define INVLIB_INVERTER_PROTO_H

extern "C" {
#include "inverter.h"
}
#include "common_types.h"
#include "inverter.pb.h"

#include <stdio.h>

void inverter_serialize(inverter_message_INV_RESPONSE *data, uint16_t id,
                        inverter::InverterVec *inverter,
                        inverter_data_t inverters_data[2]);
void inverter_deserialize(network_enums *net_enums,
                          network_signals *net_signals,
                          network_strings *net_strings,
                          const inverter::InverterVec *inverter,
                          uint64_t resample_us,
                          const std::string &inverter_suffix);

#endif // INVLIB_INVERTER_PROTO_H