#include "inverter.h"

#include <math.h>
#include <string.h>

#include "can/lib/inverters/inverters_network.h"

void inverter_close_files(inverter_files_t *files) {
  for (int i = 0; i < INV_SEND_SIZE; ++i) {
    if (files->l_send[i]) fclose(files->l_send[i]);
    if (files->r_send[i]) fclose(files->r_send[i]);
  }
  for (int i = 0; i < INV_RCV_SIZE; ++i) {
    if (files->l_rcv[i]) fclose(files->l_rcv[i]);
    if (files->r_rcv[i]) fclose(files->r_rcv[i]);
  }
}
void inverter_flush_files(inverter_files_t *files) {
  for (int i = 0; i < INV_SEND_SIZE; ++i) {
    if (files->l_send[i]) fflush(files->l_send[i]);
    if (files->r_send[i]) fflush(files->r_send[i]);
  }
  for (int i = 0; i < INV_RCV_SIZE; ++i) {
    if (files->l_rcv[i]) fflush(files->l_rcv[i]);
    if (files->r_rcv[i]) fflush(files->r_rcv[i]);
  }
}
void inverter_open_files(inverter_files_t *files, const char *path) {
  size_t path_len = strlen(path);
  const size_t max_buff_len = 1024;
  if (path_len >= max_buff_len) return;

  uint16_t can_id;
  char buffer[max_buff_len];
  memcpy(buffer, path, strlen(path));

  if (path[path_len - 1] != '/') {
    buffer[path_len] = '/';
    path_len++;
  }
  for (int i = 0; i < INV_SEND_SIZE; ++i) {
    can_id = INVERTERS_INV_L_SEND_FRAME_ID;
    memset(buffer + path_len, 0, max_buff_len - path_len);
    strcat(buffer, "inv_l_");
    strcat(buffer,
           inverter_get_mux_name(can_id, inverter_send_type_to_mux_val(i)).str);
    strcat(buffer, ".csv");

    files->l_send[i] = fopen(buffer, "w");

    can_id = INVERTERS_INV_R_SEND_FRAME_ID;
    memset(buffer + path_len, 0, max_buff_len - path_len);
    strcat(buffer, "inv_r_");
    strcat(buffer,
           inverter_get_mux_name(can_id, inverter_send_type_to_mux_val(i)).str);
    strcat(buffer, ".csv");

    files->r_send[i] = fopen(buffer, "w");
  }

  for (int i = 0; i < INV_RCV_SIZE; ++i) {
    can_id = INVERTERS_INV_L_RCV_FRAME_ID;
    memset(buffer + path_len, 0, max_buff_len - path_len);
    strcat(buffer, "inv_l_");
    strcat(buffer,
           inverter_get_mux_name(can_id, inverter_rcv_type_to_mux_val(i)).str);
    strcat(buffer, ".csv");

    files->l_rcv[i] = fopen(buffer, "w");

    can_id = INVERTERS_INV_R_RCV_FRAME_ID;
    memset(buffer + path_len, 0, max_buff_len - path_len);
    strcat(buffer, "inv_r_");
    strcat(buffer,
           inverter_get_mux_name(can_id, inverter_rcv_type_to_mux_val(i)).str);
    strcat(buffer, ".csv");

    files->r_rcv[i] = fopen(buffer, "w");
  }
}
void inverter_headers_to_file(inverter_files_t *files) {
  for (int i = 0; i < INV_RCV_SIZE; ++i) {
    inverter_rcv_header_to_file(files, INVERTER_SIDE_LEFT, i);
    inverter_rcv_header_to_file(files, INVERTER_SIDE_RIGHT, i);
  }
  for (int i = 0; i < INV_SEND_SIZE; ++i) {
    inverter_send_header_to_file(files, INVERTER_SIDE_LEFT, i);
    inverter_send_header_to_file(files, INVERTER_SIDE_RIGHT, i);
  }
}
void inverter_rcv_header_to_file(inverter_files_t *files, inverter_side_t side,
                                 inverter_rcv_type rcv_type) {
  FILE *out;
  if (side == INVERTER_SIDE_LEFT) {
    out = files->l_rcv[rcv_type];
  } else if (side == INVERTER_SIDE_RIGHT) {
    out = files->r_rcv[rcv_type];
  } else {
    return;
  }
  switch (rcv_type) {
    case INV_RCV_N_ACT_FILT:
      fprintf(out, "_timestamp,n_act_filt\n");
      break;
    case INV_RCV_IQ_ACT_FILT:
      fprintf(out, "_timestamp,iq_act_filt\n");
      break;
    case INV_RCV_MODE:
      fprintf(out, "_timestamp,mode\n");
      break;
    case INV_RCV_T_IGBT:
      fprintf(out, "_timestamp,t_igbt\n");
      break;
    case INV_RCV_T_MOTOR:
      fprintf(out, "_timestamp,t_motor\n");
      break;
    default:
      return;
      break;
  }
}
void inverter_send_header_to_file(inverter_files_t *files, inverter_side_t side,
                                  inverter_send_type send_type) {
  FILE *out;
  if (side == INVERTER_SIDE_LEFT) {
    out = files->l_send[send_type];
  } else if (side == INVERTER_SIDE_RIGHT) {
    out = files->r_send[send_type];
  } else {
    return;
  }
  switch (send_type) {
    case INV_SEND_SET_DIG:
      fprintf(out, "_timestamp,set_dig\n");
      break;
    default:
      return;
      break;
  }
}
void inverter_rcv_to_file(inverter_files_t *files, inverter_side_t side,
                          void *message) {
  FILE *out;
  uint8_t msg_type;
  // TODO: check in a better way if the message is _converter_t or not
  if (side == INVERTER_SIDE_LEFT) {
    inverters_inv_l_rcv_t *msg_raw = (inverters_inv_l_rcv_t *)message;
    inverters_inv_l_rcv_converted_t *msg =
        (inverters_inv_l_rcv_converted_t *)message;
    msg_type = inverter_mux_val_to_rcv_type(msg->rcv_mux);
    if (msg_type == INV_RCV_SIZE) return;

    out = files->l_rcv[msg_type];
    switch (msg_type) {
      case INV_RCV_N_ACT_FILT:
        fprintf(out, "%" PRIu64 ",%f\n", msg->_timestamp, msg->n_actual_filt);
        break;
      case INV_RCV_IQ_ACT_FILT:
        fprintf(out, "%" PRIu64 ",%f\n", msg->_timestamp, msg->iq_actual);
        break;
      case INV_RCV_MODE:
        fprintf(out, "%" PRIu64 ",NOT IMPLEMENTED\n", msg->_timestamp);
        break;
      case INV_RCV_T_IGBT:
        fprintf(out, "%" PRIu64 ",%f\n", msg->_timestamp, msg->t_igbt);
        break;
      case INV_RCV_T_MOTOR:
        fprintf(out, "%" PRIu64 ",%f\n", msg->_timestamp, msg->t_motor);
        break;
      default:
        break;
    }
  } else if (side == INVERTER_SIDE_RIGHT) {
    inverters_inv_r_rcv_t *msg_raw = (inverters_inv_r_rcv_t *)message;
    inverters_inv_r_rcv_converted_t *msg =
        (inverters_inv_r_rcv_converted_t *)message;
    msg_type = inverter_mux_val_to_rcv_type(msg->rcv_mux);
    if (msg_type == INV_RCV_SIZE) return;

    out = files->r_rcv[msg_type];
    switch (msg_type) {
      case INV_RCV_N_ACT_FILT:
        fprintf(out, "%" PRIu64 ",%f\n", msg->_timestamp, msg->n_actual_filt);
        break;
      case INV_RCV_IQ_ACT_FILT:
        fprintf(out, "%" PRIu64 ",%f\n", msg->_timestamp, msg->iq_actual);
        break;
      case INV_RCV_MODE:
        fprintf(out, "%" PRIu64 ",NOT IMPLEMENTED\n", msg->_timestamp);
        break;
      case INV_RCV_T_IGBT:
        fprintf(out, "%" PRIu64 ",%f\n", msg->_timestamp, msg->t_igbt);
        break;
      case INV_RCV_T_MOTOR:
        fprintf(out, "%" PRIu64 ",%f\n", msg->_timestamp, msg->t_motor);
        break;
      default:
        break;
    }
  }
}

void inverter_send_to_file(inverter_files_t *files, inverter_side_t side,
                           void *message) {
  FILE *out;
  inverter_send_type msg_type;
  // TODO: check in a better way if the message is _converter_t or not
  if (side == INVERTER_SIDE_LEFT) {
    inverters_inv_l_send_t *msg_raw = (inverters_inv_l_send_t *)message;
    inverters_inv_l_send_converted_t *msg =
        (inverters_inv_l_send_converted_t *)message;
    msg_type = inverter_mux_val_to_send_type(msg->send_mux);
    if (msg_type == INV_SEND_SIZE) return;

    out = files->l_send[msg_type];
    switch (msg_type) {
      case INV_SEND_SET_DIG:
        fprintf(out, "%" PRIu64 ",%f\n", msg_raw->_timestamp,
                msg_raw->m_setdig__iq);
        break;
      default:
        break;
    }
  } else if (side == INVERTER_SIDE_RIGHT) {
    inverters_inv_r_send_t *msg_raw = (inverters_inv_r_send_t *)message;
    inverters_inv_r_send_converted_t *msg =
        (inverters_inv_r_send_converted_t *)message;
    msg_type = inverter_mux_val_to_send_type(msg->send_mux);
    if (msg_type == INV_SEND_SIZE) return;

    out = files->r_send[msg_type];
    switch (msg_type) {
      case INV_SEND_SET_DIG:
        fprintf(out, "%" PRIu64 ",%f\n", msg_raw->_timestamp,
                msg_raw->m_setdig__iq);
        break;
      default:
        break;
    }
  }
}

// UTILS
uint8_t inverter_rcv_type_to_mux_val(inverter_rcv_type msg_type) {
  switch (msg_type) {
    case INV_RCV_N_ACT_FILT:
      return INVERTERS_INV_L_RCV_RCV_MUX_ID_A8_N_ACTUAL_FILT_CHOICE;
      break;
    case INV_RCV_IQ_ACT_FILT:
      return INVERTERS_INV_R_RCV_RCV_MUX_ID_27_IQ_ACTUAL_CHOICE;
      break;
    case INV_RCV_MODE:
      return INVERTERS_INV_R_RCV_RCV_MUX_ID_51_KERN_MODE_STATE_CHOICE;
      break;
    case INV_RCV_T_IGBT:
      return INVERTERS_INV_R_RCV_RCV_MUX_ID_4A_T_IGBT_CHOICE;
      break;
    case INV_RCV_T_MOTOR:
      return INVERTERS_INV_R_RCV_RCV_MUX_ID_49_T_MOTOR_CHOICE;
      break;

    default:
      return 0;
  }
}
uint8_t inverter_send_type_to_mux_val(inverter_send_type msg_type) {
  switch (msg_type) {
    case INV_SEND_SET_DIG:
      return INVERTERS_INV_L_SEND_SEND_MUX_ID_90_M_SETDIG_CHOICE;
      break;

    default:
      return 0;
  }
}
inverter_rcv_type inverter_mux_val_to_rcv_type(uint8_t mux_val) {
  switch (mux_val) {
    case INVERTERS_INV_L_RCV_RCV_MUX_ID_A8_N_ACTUAL_FILT_CHOICE:
      return INV_RCV_N_ACT_FILT;
      break;
    case INVERTERS_INV_R_RCV_RCV_MUX_ID_27_IQ_ACTUAL_CHOICE:
      return INV_RCV_IQ_ACT_FILT;
      break;
    case INVERTERS_INV_R_RCV_RCV_MUX_ID_51_KERN_MODE_STATE_CHOICE:
      return INV_RCV_MODE;
      break;
    case INVERTERS_INV_R_RCV_RCV_MUX_ID_4A_T_IGBT_CHOICE:
      return INV_RCV_T_IGBT;
      break;
    case INVERTERS_INV_R_RCV_RCV_MUX_ID_49_T_MOTOR_CHOICE:
      return INV_RCV_T_MOTOR;
      break;

    default:
      return INV_RCV_SIZE;
  }
}
inverter_send_type inverter_mux_val_to_send_type(uint8_t mux_val) {
  switch (mux_val) {
    case INVERTERS_INV_L_SEND_SEND_MUX_ID_90_M_SETDIG_CHOICE:
      return INV_SEND_SET_DIG;

    default:
      return INV_SEND_SIZE;
  }
}
inverter_string_t inverter_get_mux_name(uint16_t can_id, uint8_t mux_val) {
  inverter_string_t str = {.str = 0};
  switch (can_id) {
    case INVERTERS_INV_L_SEND_FRAME_ID:
      inverters_inv_l_send_send_mux_enum_to_string(mux_val, str.str);
      break;
    case INVERTERS_INV_L_RCV_FRAME_ID:
      inverters_inv_l_rcv_rcv_mux_enum_to_string(mux_val, str.str);
      break;
    case INVERTERS_INV_R_SEND_FRAME_ID:
      inverters_inv_r_send_send_mux_enum_to_string(mux_val, str.str);
      break;
    case INVERTERS_INV_R_RCV_FRAME_ID:
      inverters_inv_r_rcv_rcv_mux_enum_to_string(mux_val, str.str);
      break;
    default:
      break;
  }
  // lowercase
  for (int i = 0; i < strlen(str.str); ++i) {
    if (str.str[i] > 64 && str.str[i] < 91) {
      str.str[i] += 32;
    }
  }
  return str;
}

// CONVERSIONS
void inverter_rcv_apply_conversions(inverter_side_t side, void *message) {
  uint8_t msg_type;
  if (side == INVERTER_SIDE_LEFT) {
    inverters_inv_l_rcv_converted_t *msg =
        (inverters_inv_l_rcv_converted_t *)message;
    msg->n_actual_filt = -convert_n_actual_filt(msg->n_actual_filt);
    msg->iq_actual = -convert_iq_act(msg->iq_actual);
    msg->t_igbt = convert_t_igbt(msg->t_igbt);
    msg->t_motor = convert_t_motor(msg->t_motor);
  } else if (side == INVERTER_SIDE_RIGHT) {
    inverters_inv_r_rcv_converted_t *msg =
        (inverters_inv_r_rcv_converted_t *)message;
    msg->n_actual_filt = convert_n_actual_filt(msg->n_actual_filt);
    msg->iq_actual = convert_iq_act(msg->iq_actual);
    msg->t_igbt = convert_t_igbt(msg->t_igbt);
    msg->t_motor = convert_t_motor(msg->t_motor);
  }
}
void inverter_send_apply_conversions(inverter_side_t side, void *message) {
  inverter_send_type msg_type;
  if (side == INVERTER_SIDE_LEFT) {
    inverters_inv_l_send_converted_t *msg =
        (inverters_inv_l_send_converted_t *)message;
    msg->m_setdig__iq = convert_m_set_dig_iq(msg->m_setdig__iq);
  } else if (side == INVERTER_SIDE_RIGHT) {
    inverters_inv_r_send_converted_t *msg =
        (inverters_inv_r_send_converted_t *)message;
    msg->m_setdig__iq = convert_m_set_dig_iq(msg->m_setdig__iq);
  }
}

float IGBT_TEMP_COEFFICIENT[6] = { (float) 3.58282057e-18, (float) -4.14165530e-13, (float) 1.90916947e-08, (float) -4.38539758e-04, (float) 5.02717412e+00, (float) -2.30219791e+04 };

float convert_n_actual_filt(float val) {
  return val * (INV_MAX_SPEED / UINT16_MAX) * 4.5f / 10.0f;
}
float convert_t_motor(float val) { return (val - 9393.9f) / 55.1f; }
float convert_t_igbt(float val) {
  return (powf((val), 5) * IGBT_TEMP_COEFFICIENT[0]) + (powf((val), 4) * IGBT_TEMP_COEFFICIENT[1]) + (powf((val), 3) * IGBT_TEMP_COEFFICIENT[2]) + (powf((val), 2) * IGBT_TEMP_COEFFICIENT[3]) + (val * IGBT_TEMP_COEFFICIENT[4]) + IGBT_TEMP_COEFFICIENT[5];
}
float convert_iq_act(float val) { return val * M_PI / 2.0; }
float convert_m_set_dig_iq(float val) { return val / INT16_MAX * 400.0 * 0.45; }
