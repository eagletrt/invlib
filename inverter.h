#ifndef _INVERTERS_H_
#define _INVERTERS_H_

#include <inttypes.h>
#include <stdio.h>

typedef enum inverter_side_t {
  INVERTER_SIDE_LEFT = 0,
  INVERTER_SIDE_RIGHT = 1
} inverter_side_t;

typedef enum inverter_rcv_type {
  INV_RCV_N_ACT_FILT = 0,
  INV_RCV_IQ_ACT_FILT,
  INV_RCV_MODE,
  INV_RCV_T_IGBT,
  INV_RCV_T_MOTOR,

  INV_RCV_SIZE
} inverter_rcv_type;

typedef enum inverter_send_type {
  INV_SEND_SET_DIG = 0,  // INVERTERS_INV_L_SEND_SEND_MUX_ID_90_M_SETDIG_CHOICE

  INV_SEND_SIZE
} inverter_send_type;

typedef struct inverter_files_t {
  FILE* l_send[INV_SEND_SIZE];
  FILE* l_rcv[INV_RCV_SIZE];
  FILE* r_send[INV_SEND_SIZE];
  FILE* r_rcv[INV_RCV_SIZE];
} inverter_files_t;

typedef struct inverter_string_t {
  char str[1024];
} inverter_string_t;

void inverter_close_files(inverter_files_t* files);
void inverter_flush_files(inverter_files_t* files);
void inverter_open_files(inverter_files_t* files, const char* path);

void inverter_rcv_to_file(inverter_files_t* files, inverter_side_t side,
                          void* message);
void inverter_send_to_file(inverter_files_t* files, inverter_side_t side,
                           void* message);

void inverter_headers_to_file(inverter_files_t* files);
void inverter_rcv_header_to_file(inverter_files_t* files, inverter_side_t side,
                                 inverter_rcv_type rcv_type);
void inverter_send_header_to_file(inverter_files_t* files, inverter_side_t side,
                                  inverter_send_type send_type);

uint8_t inverter_rcv_type_to_mux_val(inverter_rcv_type msg_type);
inverter_rcv_type inverter_mux_val_to_rcv_type(uint8_t mux_val);
uint8_t inverter_send_type_to_mux_val(inverter_send_type msg_type);
inverter_send_type inverter_mux_val_to_send_type(uint8_t mux_val);
inverter_string_t inverter_get_mux_name(uint16_t can_id, uint8_t mux_val);

void inverter_rcv_apply_conversions(inverter_side_t side, void* message);
void inverter_send_apply_conversions(inverter_side_t side, void* message);

#endif  // _INVERTERS_H_
