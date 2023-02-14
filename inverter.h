#ifndef INVLIB_INVERTER_H
#define INVLIB_INVERTER_H

#include <stdio.h>

#include "can/lib/primary/c/ids.h"
#include "can/lib/primary/c/network.h"

/* Configuration parameters from NDrive */
#define INV_MAX_RPM    6500                      /*< The "Nmax" parameter - maximum motor speed */

/* Register IDs for which updates will be activated */
#define INV_CMD_TX_REQ    0x3D
#define INV_REG_TORQUECMD 0x90
#define INV_REG_MODE      0x51
#define INV_REG_STATUS    0x40
#define INV_REG_IOINFO    0xD8
#define INV_REG_ERRORS    0x8F
#define INV_REG_SPEED     0xA8
#define INV_REG_MOT_TEMP  0x49
#define INV_REG_INV_TEMP  0x4A
#define INV_REG_I_CMD     0x26
#define INV_REG_I_ACTUAL  0x27

#define INV_IDX_LEFT      0
#define INV_IDX_RIGHT     1
#define INV_STR_SIZE      255

#ifndef INVLIB_PARKING
/* We know it's PACKING but PARKING sounds a bit better ;) */
#if defined(__MINGW32__)
#define INVLIB_PARKING __attribute__((__gcc_struct__, __packed__)) // , __aligned__(1)))
#else
#define INVLIB_PARKING __attribute__((__packed__)) // , __aligned__(1)))
#endif // defined(__MINGW32__)
#endif // INVLIB_PARKING


typedef struct INVLIB_PARKING{
  unsigned int drive_enable:1;
  unsigned int ncr0:1;
  unsigned int limp:1;
  unsigned int limm:1;
  unsigned int drive_ok:1;
  unsigned int icns:1;
  unsigned int t_nlim:1;
  unsigned int p_n:1;
  unsigned int n_i:1;
  unsigned int n0:1;
  unsigned int rsw:1;
  unsigned int cal0:1;
  unsigned int cal:1;
  unsigned int tol:1;
  unsigned int drive_ready:1;
  unsigned int brk:1;
  unsigned int sign_mag:1;
  unsigned int nclip:1;
  unsigned int nclipp:1;
  unsigned int nclipm:1;
  unsigned int ird_dig:1;
  unsigned int iuse_rchd:1;
  unsigned int ird_n:1;
  unsigned int ird_ti:1;
  unsigned int ird_tir:1;
  unsigned int hz10:1;
  unsigned int ird_tm:1;
  unsigned int ird_ana:1;
  unsigned int iwcns:1;
  unsigned int rfe_pulse:1;
  unsigned int md:1;
  unsigned int hnd_whl:1;
}inverter_status_expanded_t;

typedef struct INVLIB_PARKING{
  unsigned int bad_param:1;
  unsigned int hw_fault:1;
  unsigned int safety_fault:1;
  unsigned int can_timeout:1;
  unsigned int encoder_err:1;
  unsigned int no_power_voltage:1;
  unsigned int hi_motor_temp:1;
  unsigned int hi_device_temp:1;
  unsigned int overvoltage:1;
  unsigned int overcurrent:1;
  unsigned int raceaway:1;
  unsigned int user_err:1;
  unsigned int unknown_err_12:1;
  unsigned int unknown_err_13:1;
  unsigned int current_err:1;
  unsigned int ballast_overload:1;
  unsigned int device_id_err:1;
  unsigned int run_sig_fault:1;
  unsigned int unknown_err_19:1;
  unsigned int unknown_err_20:1;
  unsigned int powervoltage_warn:1;
  unsigned int hi_motor_temp_warn:1;
  unsigned int hi_device_temp_warn:1;
  unsigned int vout_limit_warn:1;
  unsigned int overcurrent_warn:1;
  unsigned int raceaway_warn:1;
  unsigned int unknown_err_27:1;
  unsigned int unknown_err_28:1;
  unsigned int unknown_err_29:1;
  unsigned int unknown_err_30:1;
  unsigned int ballast_overload_warn:1;
} inverter_errors_expanded_t;

typedef struct INVLIB_PARKING{
  unsigned int lmt1:1;
  unsigned int lmt2:1;
  unsigned int in2:1;
  unsigned int in1:1;
  unsigned int frg:1;
  unsigned int rfe:1;
  unsigned int unk6:1;
  unsigned int unk7:1;
  unsigned int out1:1;
  unsigned int out2:1;
  unsigned int btb:1;
  unsigned int go:1;
  unsigned int out3:1;
  unsigned int out4:1;
  unsigned int g_off:1;
  unsigned int brk1:1;
}inverter_io_info_expanded_t;

typedef struct CANLIB_PARKING{
    primary_uint8 reg_id;
    primary_uint8 data_0;
    primary_uint8 data_1;
    primary_uint8 data_2;
    primary_uint8 data_3;
    primary_uint8 data_4;
    primary_uint8 data_5;
    primary_uint8 data_6;
#ifdef CANLIB_TIMESTAMP
    primary_uint64 _timestamp;
#endif // CANLIB_TIMESTAMP
}inverter_message_INV_RESPONSE;
static_assert(sizeof(inverter_message_INV_RESPONSE) == sizeof(primary_message_INV_L_RESPONSE), "invlib l response message size != primary l response message size");
static_assert(sizeof(inverter_message_INV_RESPONSE) == sizeof(primary_message_INV_R_RESPONSE), "invlib r response message size != primary r response message size");

typedef struct {
    uint16_t io_info;
#ifdef CANLIB_TIMESTAMP
    uint64_t _timestamp;
#endif // CANLIB_TIMESTAMP
} inverter_io_info_t;

typedef struct {
    uint32_t status;
#ifdef CANLIB_TIMESTAMP
    uint64_t _timestamp;
#endif // CANLIB_TIMESTAMP
} inverter_status_t;

typedef struct {
    uint32_t errors;
#ifdef CANLIB_TIMESTAMP
    uint64_t _timestamp;
#endif // CANLIB_TIMESTAMP
} inverter_errors_t;

typedef struct {
    float temp;
#ifdef CANLIB_TIMESTAMP
    uint64_t _timestamp;
#endif // CANLIB_TIMESTAMP
} inverter_temp_t;

typedef struct {
    float motor_temp;
#ifdef CANLIB_TIMESTAMP
    uint64_t _timestamp;
#endif // CANLIB_TIMESTAMP
} inverter_motor_temp_t;

typedef struct {
    float speed;
#ifdef CANLIB_TIMESTAMP
    uint64_t _timestamp;
#endif // CANLIB_TIMESTAMP
} inverter_speed_t;

typedef struct {
    float torque;
#ifdef CANLIB_TIMESTAMP
    uint64_t _timestamp;
#endif // CANLIB_TIMESTAMP
} inverter_das_torque_t;

typedef struct {
    float torque;
#ifdef CANLIB_TIMESTAMP
    uint64_t _timestamp;
#endif // CANLIB_TIMESTAMP
} inverter_inv_torque_t;

typedef struct {
    float torque;
#ifdef CANLIB_TIMESTAMP
    uint64_t _timestamp;
#endif // CANLIB_TIMESTAMP
} inverter_inv_torque_actual_t;

typedef struct {
    inverter_temp_t inverter_temp;
    inverter_speed_t inverter_speed;
    inverter_motor_temp_t inverter_motor_temp;
    inverter_status_t inverter_status;
    inverter_errors_t inverter_errors;
    inverter_io_info_t inverter_io_info;
    inverter_das_torque_t inverter_das_torque;
    inverter_inv_torque_t inverter_inv_torque;
    inverter_inv_torque_actual_t inverter_inv_torque_actual;
} inverter_data_t;

typedef struct {
    FILE* inverter_temp;
    FILE* inverter_motor_temp;
    FILE* inverter_speed;
    FILE* inverter_status;
    FILE* inverter_errors;
    FILE* inverter_io_info;
    FILE* inverter_das_torque;
    FILE* inverter_inv_torque;
    FILE* inverter_inv_torque_actual;
} inverter_files_t;

void set_max_rpm(int16_t value);
int32_t inline inverter_get_errors(inverter_message_INV_RESPONSE *data);
int32_t inline inverter_get_status(inverter_message_INV_RESPONSE *data);
int16_t inline inverter_get_io_info(inverter_message_INV_RESPONSE *data);
float inline inverter_get_motor_temp(inverter_message_INV_RESPONSE *data);
float inline inverter_get_inverter_temp(inverter_message_INV_RESPONSE *data);
int16_t inline inverter_get_speed(inverter_message_INV_RESPONSE *data);
float inline inverter_get_das_torque(inverter_message_INV_RESPONSE *data);
float inline inverter_get_inv_torque(inverter_message_INV_RESPONSE *data);
float inline inverter_get_inv_torque_actual(inverter_message_INV_RESPONSE *data);

// expanded enums
void inverter_get_status_expanded(const inverter_status_t* status, inverter_status_expanded_t* status_expanded);
void inverter_get_errors_expanded(const inverter_errors_t* errors, inverter_errors_expanded_t* errors_expanded);
void inverter_get_io_info_expanded(const inverter_io_info_t* io_info, inverter_io_info_expanded_t* io_info_expanded);

bool parse_inverter(inverter_message_INV_RESPONSE *data, int id, inverter_data_t inverters_data[2]);

const char* inverter_filename(int reg_id);
int inverter_fields(FILE *buffer, int reg_id);
void inverter_close_files(inverter_files_t* inverters_files);
void inverter_open_files(const char* folder, inverter_files_t* inverters_files);
bool inverter_to_file(inverter_message_INV_RESPONSE *data, int id, inverter_files_t files[2], inverter_data_t inverters_data[2]);

static int16_t max_rpm = INV_MAX_RPM;

#ifdef INVLIB_IMPLEMENTATION
#include <math.h>

void set_max_rpm(float value){
  max_rpm = value;
}

int32_t inline inverter_get_errors(inverter_message_INV_RESPONSE *data){
  return ((uint32_t)data->data_3 << 24) | (data->data_2 << 16) | (data->data_1 << 8) | data->data_0;
}

int32_t inline inverter_get_status(inverter_message_INV_RESPONSE *data){
  return ((uint32_t)data->data_3 << 24) | (data->data_2 << 16) | (data->data_1 << 8) | data->data_0;
}

int16_t inline inverter_get_io_info(inverter_message_INV_RESPONSE *data){
  return (data->data_1 << 8) | data->data_0;
}

float inline inverter_get_motor_temp(inverter_message_INV_RESPONSE *data) {
  int16_t m_temp = ((data->data_1 << 8) | data->data_0);
  return (m_temp - 9393.9f) / 55.1f;
}

float inline inverter_get_inverter_temp(inverter_message_INV_RESPONSE *data) {
  uint16_t i_temp = ((data->data_1 << 8) | data->data_0);
  return -43.23745 + 0.01073427*i_temp - 5.523417e-7*pow(i_temp, 2) + 1.330787e-11*pow(i_temp, 3);
}

int16_t inline inverter_get_speed(inverter_message_INV_RESPONSE *data) {
  return max_rpm * (int16_t)((data->data_1 << 8) | data->data_0) / 32767;
}

float inline rpm_to_rads(int16_t rpm) {
  return rpm * 2 * M_PI / 60;
}

float inline inverter_get_das_torque(inverter_message_INV_RESPONSE *data){
  uint16_t cmd = ((data->data_1 << 8) | data->data_0);
  return (float)cmd;
}

float inline inverter_get_inv_torque(inverter_message_INV_RESPONSE *data){
  int16_t cmd = ((data->data_1 << 8) | data->data_0);
  return (float)cmd;
}

float inline inverter_get_inv_torque_actual(inverter_message_INV_RESPONSE *data){
  int16_t cmd_actual = ((data->data_1 << 8) | data->data_0);
  return cmd_actual;
}

void inverter_get_status_expanded(const inverter_status_t* status, inverter_status_expanded_t* status_expanded){
  memcpy(status_expanded, &(status->status), sizeof(inverter_status_expanded_t));
}

void inverter_get_errors_expanded(const inverter_errors_t* errors, inverter_errors_expanded_t* errors_expanded){
  memcpy(errors_expanded, &(errors->errors), sizeof(inverter_errors_expanded_t));
}

void inverter_get_io_info_expanded(const inverter_io_info_t* io_info, inverter_io_info_expanded_t* io_info_expanded){
  memcpy(io_info_expanded, &(io_info->io_info), sizeof(inverter_io_info_expanded_t));
}

FILE* inverter_open_file(const char* path, int reg_id, int inv_index){
  FILE* out = NULL;
  char buff[400];
  strcpy(buff, path);
  strcat(buff, inverter_filename(reg_id));
  if(inv_index == INV_IDX_LEFT)
    strcat(buff, "_L.csv");
  else if(inv_index == INV_IDX_RIGHT)
    strcat(buff, "_R.csv");

  out = fopen(buff, "w");
  inverter_fields(out, reg_id);
  fflush(out);
  return out;
}

void inverter_open_files(const char* folder, inverter_files_t* inverters_files){
  inverters_files[INV_IDX_LEFT].inverter_temp               = inverter_open_file(folder, INV_REG_INV_TEMP,  INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_motor_temp         = inverter_open_file(folder, INV_REG_MOT_TEMP,  INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_speed              = inverter_open_file(folder, INV_REG_SPEED,     INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_status             = inverter_open_file(folder, INV_REG_STATUS,    INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_errors             = inverter_open_file(folder, INV_REG_ERRORS,    INV_IDX_LEFT); 
  inverters_files[INV_IDX_LEFT].inverter_io_info            = inverter_open_file(folder, INV_REG_IOINFO,    INV_IDX_LEFT); 
  inverters_files[INV_IDX_LEFT].inverter_das_torque         = inverter_open_file(folder, INV_REG_TORQUECMD, INV_IDX_LEFT); 
  inverters_files[INV_IDX_LEFT].inverter_inv_torque         = inverter_open_file(folder, INV_REG_I_CMD,     INV_IDX_LEFT); 
  inverters_files[INV_IDX_LEFT].inverter_inv_torque_actual  = inverter_open_file(folder, INV_REG_I_ACTUAL,  INV_IDX_LEFT); 

  inverters_files[INV_IDX_RIGHT].inverter_temp              = inverter_open_file(folder, INV_REG_INV_TEMP,  INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_motor_temp        = inverter_open_file(folder, INV_REG_MOT_TEMP,  INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_speed             = inverter_open_file(folder, INV_REG_SPEED,     INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_status            = inverter_open_file(folder, INV_REG_STATUS,    INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_errors            = inverter_open_file(folder, INV_REG_ERRORS,    INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_io_info           = inverter_open_file(folder, INV_REG_IOINFO,    INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_das_torque        = inverter_open_file(folder, INV_REG_TORQUECMD, INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_inv_torque        = inverter_open_file(folder, INV_REG_I_CMD,     INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_inv_torque_actual = inverter_open_file(folder, INV_REG_I_ACTUAL,  INV_IDX_RIGHT);
}

void inverter_close_files(inverter_files_t* inverters_files){
  // LEFT
  // Flush
  fflush(inverters_files[INV_IDX_LEFT].inverter_temp);
  fflush(inverters_files[INV_IDX_LEFT].inverter_motor_temp);
  fflush(inverters_files[INV_IDX_LEFT].inverter_speed);
  fflush(inverters_files[INV_IDX_LEFT].inverter_status);
  fflush(inverters_files[INV_IDX_LEFT].inverter_errors);
  fflush(inverters_files[INV_IDX_LEFT].inverter_io_info);
  fflush(inverters_files[INV_IDX_LEFT].inverter_das_torque);
  fflush(inverters_files[INV_IDX_LEFT].inverter_inv_torque);
  fflush(inverters_files[INV_IDX_LEFT].inverter_inv_torque_actual);
  // Close
  fclose(inverters_files[INV_IDX_LEFT].inverter_temp);
  fclose(inverters_files[INV_IDX_LEFT].inverter_motor_temp);
  fclose(inverters_files[INV_IDX_LEFT].inverter_speed);
  fclose(inverters_files[INV_IDX_LEFT].inverter_status);
  fclose(inverters_files[INV_IDX_LEFT].inverter_errors);
  fclose(inverters_files[INV_IDX_LEFT].inverter_io_info);
  fclose(inverters_files[INV_IDX_LEFT].inverter_das_torque);
  fclose(inverters_files[INV_IDX_LEFT].inverter_inv_torque);
  fclose(inverters_files[INV_IDX_LEFT].inverter_inv_torque_actual);

  // RIGHT
  // Flush
  fflush(inverters_files[INV_IDX_RIGHT].inverter_temp);
  fflush(inverters_files[INV_IDX_RIGHT].inverter_motor_temp);
  fflush(inverters_files[INV_IDX_RIGHT].inverter_speed);
  fflush(inverters_files[INV_IDX_RIGHT].inverter_status);
  fflush(inverters_files[INV_IDX_RIGHT].inverter_errors);
  fflush(inverters_files[INV_IDX_RIGHT].inverter_io_info);
  fflush(inverters_files[INV_IDX_RIGHT].inverter_das_torque);
  fflush(inverters_files[INV_IDX_RIGHT].inverter_inv_torque);
  fflush(inverters_files[INV_IDX_RIGHT].inverter_inv_torque_actual);
  // Close
  fclose(inverters_files[INV_IDX_RIGHT].inverter_temp);
  fclose(inverters_files[INV_IDX_RIGHT].inverter_motor_temp);
  fclose(inverters_files[INV_IDX_RIGHT].inverter_speed);
  fclose(inverters_files[INV_IDX_RIGHT].inverter_status);
  fclose(inverters_files[INV_IDX_RIGHT].inverter_errors);
  fclose(inverters_files[INV_IDX_RIGHT].inverter_io_info);
  fclose(inverters_files[INV_IDX_RIGHT].inverter_das_torque);
  fclose(inverters_files[INV_IDX_RIGHT].inverter_inv_torque);
  fclose(inverters_files[INV_IDX_RIGHT].inverter_inv_torque_actual);
}

const char* inverter_filename(int reg_id){
  switch (reg_id) {
    case INV_REG_INV_TEMP:
      return "INVERTER_TEMP";
    case INV_REG_MOT_TEMP:
      return "INVERTER_MOTOR_TEMP";
    case INV_REG_SPEED:
      return "INVERTER_SPEED";
    case INV_REG_STATUS:
      return "INVERTER_STATUS";
    case INV_REG_ERRORS:
      return "INVERTER_ERRORS";
    case INV_REG_IOINFO:
      return "INVERTER_IOINFO";
    case INV_REG_TORQUECMD:
      return "INVERTER_DAS_TORQUE";
    case INV_REG_I_CMD:
      return "INVERTER_INV_TORQUE";
    case INV_REG_I_ACTUAL:
      return "INVERTER_INV_TORQUE_ACTUAL";
  }
  return "";
}

int inverter_fields(FILE *buffer, int reg_id) {
  switch (reg_id) {
    case INV_REG_INV_TEMP:
      return fprintf(buffer, "_timestamp,inverter_temp\n");
    break;
    case INV_REG_MOT_TEMP:
      return fprintf(buffer, "_timestamp,motor_temp\n");
    break;
    case INV_REG_SPEED:
      return fprintf(buffer, "_timestamp,speed\n");
    break;
    case INV_REG_STATUS:
      return fprintf(buffer, "_timestamp,status,status_str\n");
    break;
    case INV_REG_ERRORS:
      return fprintf(buffer, "_timestamp,errors,errors_str\n");
    break;
    case INV_REG_IOINFO:
      return fprintf(buffer, "_timestamp,io_info,io_info_str\n");
    break;
    case INV_REG_TORQUECMD:
      return fprintf(buffer, "_timestamp,torque\n");
    break;
    case INV_REG_I_CMD:
      return fprintf(buffer, "_timestamp,torque\n");
    break;
    case INV_REG_I_ACTUAL:
      return fprintf(buffer, "_timestamp,torque\n");
    break;
  }
  return 0;
}

bool inverter_to_file(inverter_message_INV_RESPONSE *data, int id, inverter_files_t* inverters_files, inverter_data_t inverters_data[2]) {
  inverter_data_t& inv = (id == primary_ID_INV_L_RESPONSE ||  id == primary_ID_INV_L_REQUEST) ? inverters_data[INV_IDX_LEFT] : inverters_data[INV_IDX_RIGHT];
  inverter_files_t& file = (id == primary_ID_INV_L_RESPONSE ||  id == primary_ID_INV_L_REQUEST) ? inverters_files[INV_IDX_LEFT] : inverters_files[INV_IDX_RIGHT];

  const unsigned int str_buffer_size = 255;
  static char str_buffer[str_buffer_size];

  switch (data->reg_id) {
    case INV_REG_MOT_TEMP:
      if(file.inverter_motor_temp == NULL) return false;
      fprintf(file.inverter_motor_temp,
#ifdef CANLIB_TIMESTAMP
        "%" PRIu64
#endif // CANLIB_TIMESTAMP
        ","
        "%f"
        "\n",
#ifdef CANLIB_TIMESTAMP
        inv.inverter_motor_temp._timestamp,
#endif // CANLIB_TIMESTAMP
        inv.inverter_motor_temp.motor_temp
      );
      fflush(file.inverter_motor_temp);
      break;
    case INV_REG_INV_TEMP:
      if(file.inverter_temp == NULL) return false;
      fprintf(file.inverter_temp,
#ifdef CANLIB_TIMESTAMP
        "%" PRIu64
#endif // CANLIB_TIMESTAMP
        ","
        "%f"
        "\n",
#ifdef CANLIB_TIMESTAMP
        inv.inverter_temp._timestamp,
#endif // CANLIB_TIMESTAMP
        inv.inverter_temp.temp
      );
      fflush(file.inverter_temp);
      break;
    case INV_REG_SPEED:
      if(file.inverter_speed == NULL) return false;
      fprintf(file.inverter_speed,
#ifdef CANLIB_TIMESTAMP
        "%" PRIu64
#endif // CANLIB_TIMESTAMP
        ","
        "%f"
        "\n",
#ifdef CANLIB_TIMESTAMP
        inv.inverter_speed._timestamp,
#endif // CANLIB_TIMESTAMP
        inv.inverter_speed.speed
      );
      fflush(file.inverter_speed);
      break;
    case INV_REG_STATUS:
      if(file.inverter_status == NULL) return false;
      memset(str_buffer, 0, str_buffer_size);
      primary_to_string_InvStatus((primary_InvStatus)inv.inverter_status.status, str_buffer);
      fprintf(file.inverter_status,
#ifdef CANLIB_TIMESTAMP
        "%" PRIu64
#endif // CANLIB_TIMESTAMP
        ","
        "%" PRIu32 ","
        "\"%s\""
        "\n",
#ifdef CANLIB_TIMESTAMP
        inv.inverter_status._timestamp,
#endif // CANLIB_TIMESTAMP
        inv.inverter_status.status,
        str_buffer
      );
      fflush(file.inverter_status);
      break;
    case INV_REG_ERRORS:
      if(file.inverter_errors == NULL) return false;
      memset(str_buffer, 0, str_buffer_size);
      primary_to_string_InvErrors((primary_InvErrors)inv.inverter_errors.errors, str_buffer);
      fprintf(file.inverter_errors,
#ifdef CANLIB_TIMESTAMP
        "%" PRIu64
#endif // CANLIB_TIMESTAMP
        ","
        "%" PRIu32 ","
        "\"%s\""
        "\n",
#ifdef CANLIB_TIMESTAMP
        inv.inverter_errors._timestamp,
#endif // CANLIB_TIMESTAMP
        inv.inverter_errors.errors,
        str_buffer
      );
      fflush(file.inverter_errors);
      break;
    case INV_REG_IOINFO:
      if(file.inverter_io_info == NULL) return false;
      memset(str_buffer, 0, str_buffer_size);
      primary_to_string_Inv_IOInfo((primary_Inv_IOInfo)inv.inverter_io_info.io_info, str_buffer);
      fprintf(file.inverter_io_info,
#ifdef CANLIB_TIMESTAMP
        "%" PRIu64
#endif // CANLIB_TIMESTAMP
        ","
        "%" PRIu16 ","
        "\"%s\""
        "\n",
#ifdef CANLIB_TIMESTAMP
        inv.inverter_io_info._timestamp,
#endif // CANLIB_TIMESTAMP
        inv.inverter_io_info.io_info,
        str_buffer
      );
      fflush(file.inverter_io_info);
      break;
    case INV_REG_TORQUECMD:
      if(file.inverter_das_torque == NULL) return false;
      fprintf(file.inverter_das_torque,
#ifdef CANLIB_TIMESTAMP
        "%" PRIu64
#endif // CANLIB_TIMESTAMP
        ","
        "%f"
        "\n",
#ifdef CANLIB_TIMESTAMP
        inv.inverter_das_torque._timestamp,
#endif // CANLIB_TIMESTAMP
        inv.inverter_das_torque.torque
      );
      fflush(file.inverter_das_torque);
    break;
    case INV_REG_I_CMD:
      if(file.inverter_inv_torque == NULL) return false;
      fprintf(file.inverter_inv_torque,
#ifdef CANLIB_TIMESTAMP
        "%" PRIu64
#endif // CANLIB_TIMESTAMP
        ","
        "%f"
        "\n",
#ifdef CANLIB_TIMESTAMP
        inv.inverter_inv_torque._timestamp,
#endif // CANLIB_TIMESTAMP
        inv.inverter_inv_torque.torque
      );
      fflush(file.inverter_inv_torque);
    break;
    case INV_REG_I_ACTUAL:
    if(file.inverter_inv_torque_actual == NULL) return false;
      fprintf(file.inverter_inv_torque_actual,
#ifdef CANLIB_TIMESTAMP
        "%" PRIu64
#endif // CANLIB_TIMESTAMP
        ","
        "%f"
        "\n",
#ifdef CANLIB_TIMESTAMP
        inv.inverter_inv_torque_actual._timestamp,
#endif // CANLIB_TIMESTAMP
        inv.inverter_inv_torque_actual.torque
      );
      fflush(file.inverter_inv_torque_actual);
    break;
    default:
      return false;
      break;
  }
  return true;
}

bool parse_inverter(inverter_message_INV_RESPONSE *data, int id, inverter_data_t inverters_data[2]){
  inverter_data_t& inv = (id == primary_ID_INV_L_RESPONSE || id == primary_ID_INV_L_REQUEST) ? inverters_data[INV_IDX_LEFT] : inverters_data[INV_IDX_RIGHT];
  switch (data->reg_id) {
    case INV_REG_MOT_TEMP:
#ifdef CANLIB_TIMESTAMP
      inv.inverter_motor_temp._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
      inv.inverter_motor_temp.motor_temp = inverter_get_motor_temp(data);
      break;
    case INV_REG_INV_TEMP:
#ifdef CANLIB_TIMESTAMP
      inv.inverter_temp._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
      inv.inverter_temp.temp = inverter_get_inverter_temp(data);
      break;
    case INV_REG_SPEED:
#ifdef CANLIB_TIMESTAMP
      inv.inverter_speed._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
      inv.inverter_speed.speed = rpm_to_rads(inverter_get_speed(data));
      break;
    case INV_REG_STATUS:
#ifdef CANLIB_TIMESTAMP
      inv.inverter_status._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
      inv.inverter_status.status = inverter_get_status(data);
      break;
    case INV_REG_ERRORS:
#ifdef CANLIB_TIMESTAMP
      inv.inverter_errors._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
      inv.inverter_errors.errors = inverter_get_errors(data);
      break;
    case INV_REG_IOINFO:
#ifdef CANLIB_TIMESTAMP
      inv.inverter_io_info._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
      inv.inverter_io_info.io_info = inverter_get_io_info(data);
      break;
    case INV_REG_TORQUECMD:
#ifdef CANLIB_TIMESTAMP
      inv.inverter_das_torque._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
      inv.inverter_das_torque.torque = inverter_get_das_torque(data);
      break;
    case INV_REG_I_CMD:
#ifdef CANLIB_TIMESTAMP
      inv.inverter_inv_torque._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
      inv.inverter_inv_torque.torque = inverter_get_inv_torque(data);
      break;
    case INV_REG_I_ACTUAL:
#ifdef CANLIB_TIMESTAMP
      inv.inverter_inv_torque_actual._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
      inv.inverter_inv_torque_actual.torque = inverter_get_inv_torque_actual(data);
      break;
    default:
      return false;
      break;
  }
  return true;
}

#endif // INVLIB_IMPLEMENTATION
#endif