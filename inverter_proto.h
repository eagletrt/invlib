#ifndef INVLIB_INVERTER_PROTO_H
#define INVLIB_INVERTER_PROTO_H

#include <stdio.h>
#include "inverter.h"
#include "inverter.pb.h"

#include "can/proto/primary/cpp/mapping.h"

#ifndef CANLIB_CIRCULAR_BUFFER_SIZE
#define CANLIB_CIRCULAR_BUFFER_SIZE 2000
#endif//CANLIB_CIRCULAR_BUFFER_SIZE

typedef struct {
    canlib_circular_buffer<inverter_motor_temp_t, CANLIB_CIRCULAR_BUFFER_SIZE> motor_temp;
    canlib_circular_buffer<inverter_temp_t, CANLIB_CIRCULAR_BUFFER_SIZE> temp;
    canlib_circular_buffer<inverter_speed_t, CANLIB_CIRCULAR_BUFFER_SIZE> speed;
    canlib_circular_buffer<inverter_status_t, CANLIB_CIRCULAR_BUFFER_SIZE> status;
    canlib_circular_buffer<inverter_errors_t, CANLIB_CIRCULAR_BUFFER_SIZE> errors;
    canlib_circular_buffer<inverter_das_torque_t, CANLIB_CIRCULAR_BUFFER_SIZE> das_torque;
    canlib_circular_buffer<inverter_inv_torque_t, CANLIB_CIRCULAR_BUFFER_SIZE> inv_torque;
    canlib_circular_buffer<inverter_inv_torque_actual_t, CANLIB_CIRCULAR_BUFFER_SIZE> inv_torque_actual;
} inverter_proto_t;

void inverter_serialize(void* data, int id, inverter::InverterVec* inverter, inverter_data_t inverters_data[2]);
void inverter_deserialize(inverter_proto_t* data, const inverter::InverterVec* inverter);
void inverter_mapping_adaptor_construct(const inverter_proto_t& pack, mapping_adaptor& mapping_map, std::string suffix);


// --- PROTO IMPLEMENTATION ---
#ifdef INVLIB_PROTO_IMPLEMENTATION

void inverter_serialize(void* data, int id, inverter::InverterVec* inverter, inverter_data_t inverters_data[2]){
  primary_message_INV_L_RESPONSE* inv_data = (primary_message_INV_L_RESPONSE*)data;
  inverter_data_t* inv = id == primary_ID_INV_L_RESPONSE ? &inverters_data[INV_IDX_LEFT] : &inverters_data[INV_IDX_RIGHT];
  switch (inv_data->reg_id) {
    case INV_REG_MOT_TEMP:{
      auto new_el = inverter->add_inverter_motor_temp();
#ifdef CANLIB_TIMESTAMP
      new_el->set__inner_timestamp(inv->inverter_motor_temp._timestamp);
#endif // CANLIB_TIMESTAMP
      new_el->set_motor_temp(inv->inverter_motor_temp.motor_temp);
      break;
    }
    case INV_REG_INV_TEMP:{
      auto new_el = inverter->add_inverter_temp();
#ifdef CANLIB_TIMESTAMP
      new_el->set__inner_timestamp(inv->inverter_temp._timestamp);
#endif // CANLIB_TIMESTAMP
      new_el->set_temperature(inv->inverter_temp.temp);
      break;
    }
    case INV_REG_SPEED:{
      auto new_el = inverter->add_inverter_speed();
#ifdef CANLIB_TIMESTAMP
      new_el->set__inner_timestamp(inv->inverter_speed._timestamp);
#endif // CANLIB_TIMESTAMP
      new_el->set_speed(inv->inverter_speed.speed);
      break;
    }
    case INV_REG_STATUS:{
      auto new_el = inverter->add_inverter_status();
#ifdef CANLIB_TIMESTAMP
      new_el->set__inner_timestamp(inv->inverter_status._timestamp);
#endif // CANLIB_TIMESTAMP
      new_el->set_status((uint32_t)inv->inverter_status.status);
      break;
    }
    case INV_REG_ERRORS:{
      auto new_el = inverter->add_inverter_errors();
#ifdef CANLIB_TIMESTAMP
      new_el->set__inner_timestamp(inv->inverter_errors._timestamp);
#endif // CANLIB_TIMESTAMP
      new_el->set_errors((uint32_t)inv->inverter_errors.errors);
      break;
    }
    case INV_REG_TORQUECMD:{
      auto new_el = inverter->add_inverter_das_torque();
#ifdef CANLIB_TIMESTAMP
      new_el->set__inner_timestamp(inv->inverter_das_torque._timestamp);
#endif // CANLIB_TIMESTAMP
      new_el->set_torque((double)inv->inverter_das_torque.torque);
      break;
    }
    case INV_REG_I_CMD:{
      auto new_el = inverter->add_inverter_inv_torque();
#ifdef CANLIB_TIMESTAMP
      new_el->set__inner_timestamp(inv->inverter_inv_torque._timestamp);
#endif // CANLIB_TIMESTAMP
      new_el->set_torque((double)inv->inverter_inv_torque.torque);
      break;
    }
    case INV_REG_I_ACTUAL:{
      auto new_el = inverter->add_inverter_inv_torque_actual();
#ifdef CANLIB_TIMESTAMP
      new_el->set__inner_timestamp(inv->inverter_inv_torque_actual._timestamp);
#endif // CANLIB_TIMESTAMP
      new_el->set_torque((double)inv->inverter_inv_torque_actual.torque);
      break;
    }
  }
}

void inverter_deserialize(inverter_proto_t* data, const inverter::InverterVec* inverter){
  for(int i = 0; i < inverter->inverter_motor_temp_size(); i++){
    static inverter_motor_temp_t instance;
#ifdef CANLIB_TIMESTAMP
    instance._timestamp = inverter->inverter_motor_temp(i)._inner_timestamp();
#endif // CANLIB_TIMESTAMP
    instance.motor_temp = inverter->inverter_motor_temp(i).motor_temp();
    data->motor_temp.push(instance);
  }
  for(int i = 0; i < inverter->inverter_temp_size(); i++){
    static inverter_temp_t instance;
#ifdef CANLIB_TIMESTAMP
    instance._timestamp = inverter->inverter_temp(i)._inner_timestamp();
#endif // CANLIB_TIMESTAMP
    instance.temp = inverter->inverter_temp(i).temperature();
    data->temp.push(instance);
  }
  for(int i = 0; i < inverter->inverter_speed_size(); i++){
    static inverter_speed_t instance;
#ifdef CANLIB_TIMESTAMP
    instance._timestamp = inverter->inverter_speed(i)._inner_timestamp();
#endif // CANLIB_TIMESTAMP
    instance.speed = inverter->inverter_speed(i).speed();
    data->speed.push(instance);
  }
  for(int i = 0; i < inverter->inverter_status_size(); i++){
    static inverter_status_t instance;
#ifdef CANLIB_TIMESTAMP
    instance._timestamp = inverter->inverter_status(i)._inner_timestamp();
#endif // CANLIB_TIMESTAMP
    instance.status = (uint32_t)inverter->inverter_status(i).status();
    data->status.push(instance);
  }
  for(int i = 0; i < inverter->inverter_errors_size(); i++){
    static inverter_errors_t instance;
#ifdef CANLIB_TIMESTAMP
    instance._timestamp = inverter->inverter_errors(i)._inner_timestamp();
#endif // CANLIB_TIMESTAMP
    instance.errors = (uint32_t)inverter->inverter_errors(i).errors();
    data->errors.push(instance);
  }
  for(int i = 0; i < inverter->inverter_das_torque_size(); i++){
    static inverter_das_torque_t instance;
#ifdef CANLIB_TIMESTAMP
    instance._timestamp = inverter->inverter_das_torque(i)._inner_timestamp();
#endif // CANLIB_TIMESTAMP
    instance.torque = (float)inverter->inverter_das_torque(i).torque();
    data->das_torque.push(instance);
  }
  for(int i = 0; i < inverter->inverter_inv_torque_size(); i++){
    static inverter_inv_torque_t instance;
#ifdef CANLIB_TIMESTAMP
    instance._timestamp = inverter->inverter_inv_torque(i)._inner_timestamp();
#endif // CANLIB_TIMESTAMP
    instance.torque = (float)inverter->inverter_inv_torque(i).torque();
    data->inv_torque.push(instance);
  }
  for(int i = 0; i < inverter->inverter_inv_torque_actual_size(); i++){
    static inverter_inv_torque_actual_t instance;
#ifdef CANLIB_TIMESTAMP
    instance._timestamp = inverter->inverter_inv_torque_actual(i)._inner_timestamp();
#endif // CANLIB_TIMESTAMP
    instance.torque = (float)inverter->inverter_inv_torque_actual(i).torque();
    data->inv_torque_actual.push(instance);
  }
}


void inverter_mapping_adaptor_construct(const inverter_proto_t& pack, mapping_adaptor& mapping_map, std::string suffix){
  
  mapping_map["INVERTER_MOTOR_TEMP" + suffix].size = std::bind(&canlib_circular_buffer<inverter_motor_temp_t, CANLIB_CIRCULAR_BUFFER_SIZE>::size, &pack.motor_temp);
  mapping_map["INVERTER_MOTOR_TEMP" + suffix].offset = std::bind(&canlib_circular_buffer<inverter_motor_temp_t, CANLIB_CIRCULAR_BUFFER_SIZE>::offset, &pack.motor_temp);
  mapping_map["INVERTER_MOTOR_TEMP" + suffix].stride = sizeof(inverter_motor_temp_t);
#ifdef CANLIB_TIMESTAMP
  mapping_map["INVERTER_MOTOR_TEMP" + suffix].field["_timestamp"].value._uint64 = &pack.motor_temp.start()._timestamp;
  mapping_map["INVERTER_MOTOR_TEMP" + suffix].field["_timestamp"].type = mapping_type_uint64;
#endif // CANLIB_TIMESTAMP
  mapping_map["INVERTER_MOTOR_TEMP" + suffix].field["motor_temp"].value._float32 = &pack.motor_temp.start().motor_temp;
  mapping_map["INVERTER_MOTOR_TEMP" + suffix].field["motor_temp"].type = mapping_type_float32;


  mapping_map["INVERTER_TEMP" + suffix].size = std::bind(&canlib_circular_buffer<inverter_temp_t, CANLIB_CIRCULAR_BUFFER_SIZE>::size, &pack.temp);
  mapping_map["INVERTER_TEMP" + suffix].offset = std::bind(&canlib_circular_buffer<inverter_temp_t, CANLIB_CIRCULAR_BUFFER_SIZE>::offset, &pack.temp);
  mapping_map["INVERTER_TEMP" + suffix].stride = sizeof(inverter_temp_t);
#ifdef CANLIB_TIMESTAMP
  mapping_map["INVERTER_TEMP" + suffix].field["_timestamp"].value._uint64 = &pack.temp.start()._timestamp;
  mapping_map["INVERTER_TEMP" + suffix].field["_timestamp"].type = mapping_type_uint64;
#endif // CANLIB_TIMESTAMP
  mapping_map["INVERTER_TEMP" + suffix].field["temp"].value._float32 = &pack.temp.start().temp;
  mapping_map["INVERTER_TEMP" + suffix].field["temp"].type = mapping_type_float32;


  mapping_map["INVERTER_SPEED" + suffix].size = std::bind(&canlib_circular_buffer<inverter_speed_t, CANLIB_CIRCULAR_BUFFER_SIZE>::size, &pack.speed);
  mapping_map["INVERTER_SPEED" + suffix].offset = std::bind(&canlib_circular_buffer<inverter_speed_t, CANLIB_CIRCULAR_BUFFER_SIZE>::offset, &pack.speed);
  mapping_map["INVERTER_SPEED" + suffix].stride = sizeof(inverter_speed_t);
#ifdef CANLIB_TIMESTAMP
  mapping_map["INVERTER_SPEED" + suffix].field["_timestamp"].value._uint64 = &pack.speed.start()._timestamp;
  mapping_map["INVERTER_SPEED" + suffix].field["_timestamp"].type = mapping_type_uint64;
#endif // CANLIB_TIMESTAMP
  mapping_map["INVERTER_SPEED" + suffix].field["speed"].value._float32 = &pack.speed.start().speed;
  mapping_map["INVERTER_SPEED" + suffix].field["speed"].type = mapping_type_float32;


  mapping_map["INVERTER_STATUS" + suffix].size = std::bind(&canlib_circular_buffer<inverter_status_t, CANLIB_CIRCULAR_BUFFER_SIZE>::size, &pack.status);
  mapping_map["INVERTER_STATUS" + suffix].offset = std::bind(&canlib_circular_buffer<inverter_status_t, CANLIB_CIRCULAR_BUFFER_SIZE>::offset, &pack.status);
  mapping_map["INVERTER_STATUS" + suffix].stride = sizeof(inverter_status_t);
#ifdef CANLIB_TIMESTAMP
  mapping_map["INVERTER_STATUS" + suffix].field["_timestamp"].value._uint64 = &pack.status.start()._timestamp;
  mapping_map["INVERTER_STATUS" + suffix].field["_timestamp"].type = mapping_type_uint64;
#endif // CANLIB_TIMESTAMP
  mapping_map["INVERTER_STATUS" + suffix].field["status"].value._uint32 = &pack.status.start().status;
  mapping_map["INVERTER_STATUS" + suffix].field["status"].type = mapping_type_uint32;


  mapping_map["INVERTER_ERRORS" + suffix].size = std::bind(&canlib_circular_buffer<inverter_errors_t, CANLIB_CIRCULAR_BUFFER_SIZE>::size, &pack.errors);
  mapping_map["INVERTER_ERRORS" + suffix].offset = std::bind(&canlib_circular_buffer<inverter_errors_t, CANLIB_CIRCULAR_BUFFER_SIZE>::offset, &pack.errors);
  mapping_map["INVERTER_ERRORS" + suffix].stride = sizeof(inverter_errors_t);
#ifdef CANLIB_TIMESTAMP
  mapping_map["INVERTER_ERRORS" + suffix].field["_timestamp"].value._uint64 = &pack.errors.start()._timestamp;
  mapping_map["INVERTER_ERRORS" + suffix].field["_timestamp"].type = mapping_type_uint64;
#endif // CANLIB_TIMESTAMP
  mapping_map["INVERTER_ERRORS" + suffix].field["errors"].value._uint32 = &pack.errors.start().errors;
  mapping_map["INVERTER_ERRORS" + suffix].field["errors"].type = mapping_type_uint32;


  mapping_map["INVERTER_DAS_TORQUE" + suffix].size = std::bind(&canlib_circular_buffer<inverter_das_torque_t, CANLIB_CIRCULAR_BUFFER_SIZE>::size, &pack.das_torque);
  mapping_map["INVERTER_DAS_TORQUE" + suffix].offset = std::bind(&canlib_circular_buffer<inverter_das_torque_t, CANLIB_CIRCULAR_BUFFER_SIZE>::offset, &pack.das_torque);
  mapping_map["INVERTER_DAS_TORQUE" + suffix].stride = sizeof(inverter_das_torque_t);
#ifdef CANLIB_TIMESTAMP
  mapping_map["INVERTER_DAS_TORQUE" + suffix].field["_timestamp"].value._uint64 = &pack.das_torque.start()._timestamp;
  mapping_map["INVERTER_DAS_TORQUE" + suffix].field["_timestamp"].type = mapping_type_uint64;
#endif // CANLIB_TIMESTAMP
  mapping_map["INVERTER_DAS_TORQUE" + suffix].field["torque"].value._float32 = &pack.das_torque.start().torque;
  mapping_map["INVERTER_DAS_TORQUE" + suffix].field["torque"].type = mapping_type_float32;


  mapping_map["INVERTER_INV_TORQUE" + suffix].size = std::bind(&canlib_circular_buffer<inverter_inv_torque_t, CANLIB_CIRCULAR_BUFFER_SIZE>::size, &pack.inv_torque);
  mapping_map["INVERTER_INV_TORQUE" + suffix].offset = std::bind(&canlib_circular_buffer<inverter_inv_torque_t, CANLIB_CIRCULAR_BUFFER_SIZE>::offset, &pack.inv_torque);
  mapping_map["INVERTER_INV_TORQUE" + suffix].stride = sizeof(inverter_inv_torque_t);
#ifdef CANLIB_TIMESTAMP
  mapping_map["INVERTER_INV_TORQUE" + suffix].field["_timestamp"].value._uint64 = &pack.inv_torque.start()._timestamp;
  mapping_map["INVERTER_INV_TORQUE" + suffix].field["_timestamp"].type = mapping_type_uint64;
#endif // CANLIB_TIMESTAMP
  mapping_map["INVERTER_INV_TORQUE" + suffix].field["torque"].value._float32 = &pack.inv_torque.start().torque;
  mapping_map["INVERTER_INV_TORQUE" + suffix].field["torque"].type = mapping_type_float32;


  mapping_map["INVERTER_INV_TORQUE_ACTUAL" + suffix].size = std::bind(&canlib_circular_buffer<inverter_inv_torque_actual_t, CANLIB_CIRCULAR_BUFFER_SIZE>::size, &pack.inv_torque_actual);
  mapping_map["INVERTER_INV_TORQUE_ACTUAL" + suffix].offset = std::bind(&canlib_circular_buffer<inverter_inv_torque_actual_t, CANLIB_CIRCULAR_BUFFER_SIZE>::offset, &pack.inv_torque_actual);
  mapping_map["INVERTER_INV_TORQUE_ACTUAL" + suffix].stride = sizeof(inverter_inv_torque_actual_t);
#ifdef CANLIB_TIMESTAMP
  mapping_map["INVERTER_INV_TORQUE_ACTUAL" + suffix].field["_timestamp"].value._uint64 = &pack.inv_torque_actual.start()._timestamp;
  mapping_map["INVERTER_INV_TORQUE_ACTUAL" + suffix].field["_timestamp"].type = mapping_type_uint64;
#endif // CANLIB_TIMESTAMP
  mapping_map["INVERTER_INV_TORQUE_ACTUAL" + suffix].field["torque"].value._float32 = &pack.inv_torque_actual.start().torque;
  mapping_map["INVERTER_INV_TORQUE_ACTUAL" + suffix].field["torque"].type = mapping_type_float32;
}

#endif // INVLIB_PROTO_IMPLEMENTATION

#endif // INVLIB_INVERTER_PROTO_H