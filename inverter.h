#ifndef INVLIB_INVERTER_H
#define INVLIB_INVERTER_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// #include "can/lib/primary/c/ids.h"
// #include "can/lib/primary/c/network.h"

/* Configuration parameters from NDrive */
#define INV_MAX_RPM 6500 /*< The "Nmax" parameter - maximum motor speed */

/* Register IDs for which updates will be activated */
#define INV_CMD_TX_REQ 0x3D
#define INV_REG_TORQUECMD 0x90
#define INV_REG_MODE 0x51
#define INV_REG_STATUS 0x40
#define INV_REG_IOINFO 0xD8
#define INV_REG_ERRORS 0x8F
#define INV_REG_SPEED 0xA8
#define INV_REG_MOT_TEMP 0x49
#define INV_REG_INV_TEMP 0x4A
#define INV_REG_I_CMD 0x26
#define INV_REG_I_ACTUAL 0x27

#define INV_IDX_LEFT 0
#define INV_IDX_RIGHT 1
#define INV_STR_SIZE 255

#ifndef INVLIB_PARKING
/* We know it's PACKING but PARKING sounds a bit better ;) */
#if defined(__MINGW32__)
#define INVLIB_PARKING                                                         \
  __attribute__((__gcc_struct__, __packed__)) // , __aligned__(1)))
#else
#define INVLIB_PARKING __attribute__((__packed__)) // , __aligned__(1)))
#endif                                             // defined(__MINGW32__)
#endif                                             // INVLIB_PARKING

#ifdef CANLIB_TIMESTAMP
#define INVLIB_TIMESTAMP uint64_t _timestamp;
#else
#define INVLIB_TIMESTAMP
#endif // CANLIB_TIMESTAMP

typedef struct INVLIB_PARKING {
  unsigned int drive_enable : 1;
  unsigned int ncr0 : 1;
  unsigned int limp : 1;
  unsigned int limm : 1;
  unsigned int drive_ok : 1;
  unsigned int icns : 1;
  unsigned int t_nlim : 1;
  unsigned int p_n : 1;
  unsigned int n_i : 1;
  unsigned int n0 : 1;
  unsigned int rsw : 1;
  unsigned int cal0 : 1;
  unsigned int cal : 1;
  unsigned int tol : 1;
  unsigned int drive_ready : 1;
  unsigned int brk : 1;
  unsigned int sign_mag : 1;
  unsigned int nclip : 1;
  unsigned int nclipp : 1;
  unsigned int nclipm : 1;
  unsigned int ird_dig : 1;
  unsigned int iuse_rchd : 1;
  unsigned int ird_n : 1;
  unsigned int ird_ti : 1;
  unsigned int ird_tir : 1;
  unsigned int hz10 : 1;
  unsigned int ird_tm : 1;
  unsigned int ird_ana : 1;
  unsigned int iwcns : 1;
  unsigned int rfe_pulse : 1;
  unsigned int md : 1;
  unsigned int hnd_whl : 1;
} inverter_status_expanded_t;

typedef struct INVLIB_PARKING {
  unsigned int bad_param : 1;
  unsigned int hw_fault : 1;
  unsigned int safety_fault : 1;
  unsigned int can_timeout : 1;
  unsigned int encoder_err : 1;
  unsigned int no_power_voltage : 1;
  unsigned int hi_motor_temp : 1;
  unsigned int hi_device_temp : 1;
  unsigned int overvoltage : 1;
  unsigned int overcurrent : 1;
  unsigned int raceaway : 1;
  unsigned int user_err : 1;
  unsigned int unknown_err_12 : 1;
  unsigned int unknown_err_13 : 1;
  unsigned int current_err : 1;
  unsigned int ballast_overload : 1;
  unsigned int device_id_err : 1;
  unsigned int run_sig_fault : 1;
  unsigned int unknown_err_19 : 1;
  unsigned int unknown_err_20 : 1;
  unsigned int powervoltage_warn : 1;
  unsigned int hi_motor_temp_warn : 1;
  unsigned int hi_device_temp_warn : 1;
  unsigned int vout_limit_warn : 1;
  unsigned int overcurrent_warn : 1;
  unsigned int raceaway_warn : 1;
  unsigned int unknown_err_27 : 1;
  unsigned int unknown_err_28 : 1;
  unsigned int unknown_err_29 : 1;
  unsigned int unknown_err_30 : 1;
  unsigned int ballast_overload_warn : 1;
} inverter_errors_expanded_t;

typedef struct INVLIB_PARKING {
  unsigned int lmt1 : 1;
  unsigned int lmt2 : 1;
  unsigned int in2 : 1;
  unsigned int in1 : 1;
  unsigned int frg : 1;
  unsigned int rfe : 1;
  unsigned int unk6 : 1;
  unsigned int unk7 : 1;
  unsigned int out1 : 1;
  unsigned int out2 : 1;
  unsigned int btb : 1;
  unsigned int go : 1;
  unsigned int out3 : 1;
  unsigned int out4 : 1;
  unsigned int g_off : 1;
  unsigned int brk1 : 1;
} inverter_io_info_expanded_t;

typedef struct {
  uint8_t reg_id;
  uint8_t data_0;
  uint8_t data_1;
  uint8_t data_2;
  uint8_t data_3;
  uint8_t data_4;
  uint8_t data_5;
  uint8_t data_6;
  INVLIB_TIMESTAMP
} inverter_message_INV_RESPONSE;

typedef struct {
  uint16_t io_info;
  INVLIB_TIMESTAMP
} inverter_io_info_t;

typedef struct {
  uint32_t status;
  INVLIB_TIMESTAMP
} inverter_status_t;

typedef struct {
  uint32_t errors;
  INVLIB_TIMESTAMP
} inverter_errors_t;

typedef struct {
  float temp;
  INVLIB_TIMESTAMP
} inverter_temp_t;

typedef struct {
  float motor_temp;
  INVLIB_TIMESTAMP
} inverter_motor_temp_t;

typedef struct {
  float speed;
  INVLIB_TIMESTAMP
} inverter_speed_t;

typedef struct {
  float torque;
  INVLIB_TIMESTAMP
} inverter_das_torque_t;

typedef struct {
  float torque;
  INVLIB_TIMESTAMP
} inverter_inv_torque_t;

typedef struct {
  float torque;
  INVLIB_TIMESTAMP
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
  FILE *inverter_temp;
  FILE *inverter_motor_temp;
  FILE *inverter_speed;
  FILE *inverter_status;
  FILE *inverter_errors;
  FILE *inverter_io_info;
  FILE *inverter_das_torque;
  FILE *inverter_inv_torque;
  FILE *inverter_inv_torque_actual;
} inverter_files_t;

// Configurations for the inverter
void set_max_rpm(int16_t value);
void set_inverter_response_id(uint16_t id_left, uint16_t id_right);
void set_inverter_request_id(uint16_t id_left, uint16_t id_right);
uint16_t get_inverter_request_id_left();
uint16_t get_inverter_response_id_left();
uint16_t get_inverter_request_id_right();
uint16_t get_inverter_response_id_right();

// Bitsets to string
int invlib_to_string_InvStatus(uint64_t value, char *buffer);
int invlib_to_string_InvErrors(uint64_t value, char *buffer);
int invlib_to_string_InvIOInfo(uint64_t value, char *buffer);

int32_t inverter_get_errors(inverter_message_INV_RESPONSE *data);
int32_t inverter_get_status(inverter_message_INV_RESPONSE *data);
int16_t inverter_get_io_info(inverter_message_INV_RESPONSE *data);
float inverter_get_motor_temp(inverter_message_INV_RESPONSE *data);
float inverter_get_inverter_temp(inverter_message_INV_RESPONSE *data);
int16_t inverter_get_speed(inverter_message_INV_RESPONSE *data);
float inverter_get_das_torque(inverter_message_INV_RESPONSE *data);
float inverter_get_inv_torque(inverter_message_INV_RESPONSE *data);
float inverter_get_inv_torque_actual(inverter_message_INV_RESPONSE *data);

// expanded enums
void inverter_get_status_expanded(const inverter_status_t *status,
                                  inverter_status_expanded_t *status_expanded);
void inverter_get_errors_expanded(const inverter_errors_t *errors,
                                  inverter_errors_expanded_t *errors_expanded);
void inverter_get_io_info_expanded(
    const inverter_io_info_t *io_info,
    inverter_io_info_expanded_t *io_info_expanded);

bool parse_inverter(inverter_message_INV_RESPONSE *data, int id,
                    inverter_data_t inverters_data[2]);

const char *inverter_filename(int reg_id);
int inverter_fields(FILE *buffer, int reg_id);
void inverter_close_files(inverter_files_t *inverters_files);
void inverter_open_files(const char *folder, inverter_files_t *inverters_files);
bool inverter_to_file(inverter_message_INV_RESPONSE *data, int id,
                      inverter_files_t files[2],
                      inverter_data_t inverters_data[2]);

#endif