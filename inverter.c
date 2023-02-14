#include "inverter.h"
#include "common_types.h"

#include <math.h>
#include <string.h>

static int16_t max_rpm = INV_MAX_RPM;
static int16_t inv_id_request_left = 0;
static int16_t inv_id_request_right = 0;
static int16_t inv_id_response_left = 0;
static int16_t inv_id_response_right = 0;

void set_max_rpm(int16_t value) { max_rpm = value; }

void set_inverter_response_id(uint16_t id_left, uint16_t id_right) {
  inv_id_response_left = id_left;
  inv_id_response_right = id_right;
}
void set_inverter_request_id(uint16_t id_left, uint16_t id_right) {
  inv_id_request_left = id_left;
  inv_id_request_right = id_right;
}
uint16_t get_inverter_request_id_left() { return inv_id_request_left; }
uint16_t get_inverter_response_id_left() { return inv_id_response_left; }
uint16_t get_inverter_request_id_right() { return inv_id_request_right; }
uint16_t get_inverter_response_id_right() { return inv_id_response_right; }

int32_t inline inverter_get_errors(inverter_message_INV_RESPONSE *data) {
  return ((uint32_t)data->data_3 << 24) | (data->data_2 << 16) |
         (data->data_1 << 8) | data->data_0;
}

int32_t inline inverter_get_status(inverter_message_INV_RESPONSE *data) {
  return ((uint32_t)data->data_3 << 24) | (data->data_2 << 16) |
         (data->data_1 << 8) | data->data_0;
}

int16_t inline inverter_get_io_info(inverter_message_INV_RESPONSE *data) {
  return (data->data_1 << 8) | data->data_0;
}

float inline inverter_get_motor_temp(inverter_message_INV_RESPONSE *data) {
  int16_t m_temp = ((data->data_1 << 8) | data->data_0);
  return (m_temp - 9393.9f) / 55.1f;
}

float inline inverter_get_inverter_temp(inverter_message_INV_RESPONSE *data) {
  uint16_t i_temp = ((data->data_1 << 8) | data->data_0);
  return -43.23745 + 0.01073427 * i_temp - 5.523417e-7 * pow(i_temp, 2) +
         1.330787e-11 * pow(i_temp, 3);
}

int16_t inline inverter_get_speed(inverter_message_INV_RESPONSE *data) {
  return max_rpm * (int16_t)((data->data_1 << 8) | data->data_0) / 32767;
}

float rpm_to_rads(int16_t rpm) { return rpm * 2 * M_PI / 60; }

float inline inverter_get_das_torque(inverter_message_INV_RESPONSE *data) {
  uint16_t cmd = ((data->data_1 << 8) | data->data_0);
  return (float)cmd;
}

float inline inverter_get_inv_torque(inverter_message_INV_RESPONSE *data) {
  int16_t cmd = ((data->data_1 << 8) | data->data_0);
  return (float)cmd;
}

float inline inverter_get_inv_torque_actual(
    inverter_message_INV_RESPONSE *data) {
  int16_t cmd_actual = ((data->data_1 << 8) | data->data_0);
  return cmd_actual;
}

void inverter_get_status_expanded(const inverter_status_t *status,
                                  inverter_status_expanded_t *status_expanded) {
  memcpy(status_expanded, &(status->status),
         sizeof(inverter_status_expanded_t));
}

void inverter_get_errors_expanded(const inverter_errors_t *errors,
                                  inverter_errors_expanded_t *errors_expanded) {
  memcpy(errors_expanded, &(errors->errors),
         sizeof(inverter_errors_expanded_t));
}

void inverter_get_io_info_expanded(
    const inverter_io_info_t *io_info,
    inverter_io_info_expanded_t *io_info_expanded) {
  memcpy(io_info_expanded, &(io_info->io_info),
         sizeof(inverter_io_info_expanded_t));
}

FILE *inverter_open_file(const char *path, int reg_id, int inv_index) {
  FILE *out = NULL;
  char buff[400];
  strcpy(buff, path);
  strcat(buff, inverter_filename(reg_id));
  if (inv_index == INV_IDX_LEFT)
    strcat(buff, "_L.csv");
  else if (inv_index == INV_IDX_RIGHT)
    strcat(buff, "_R.csv");

  out = fopen(buff, "w");
  inverter_fields(out, reg_id);
  fflush(out);
  return out;
}

void inverter_open_files(const char *folder,
                         inverter_files_t *inverters_files) {
  inverters_files[INV_IDX_LEFT].inverter_temp =
      inverter_open_file(folder, INV_REG_INV_TEMP, INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_motor_temp =
      inverter_open_file(folder, INV_REG_MOT_TEMP, INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_speed =
      inverter_open_file(folder, INV_REG_SPEED, INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_status =
      inverter_open_file(folder, INV_REG_STATUS, INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_errors =
      inverter_open_file(folder, INV_REG_ERRORS, INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_io_info =
      inverter_open_file(folder, INV_REG_IOINFO, INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_das_torque =
      inverter_open_file(folder, INV_REG_TORQUECMD, INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_inv_torque =
      inverter_open_file(folder, INV_REG_I_CMD, INV_IDX_LEFT);
  inverters_files[INV_IDX_LEFT].inverter_inv_torque_actual =
      inverter_open_file(folder, INV_REG_I_ACTUAL, INV_IDX_LEFT);

  inverters_files[INV_IDX_RIGHT].inverter_temp =
      inverter_open_file(folder, INV_REG_INV_TEMP, INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_motor_temp =
      inverter_open_file(folder, INV_REG_MOT_TEMP, INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_speed =
      inverter_open_file(folder, INV_REG_SPEED, INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_status =
      inverter_open_file(folder, INV_REG_STATUS, INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_errors =
      inverter_open_file(folder, INV_REG_ERRORS, INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_io_info =
      inverter_open_file(folder, INV_REG_IOINFO, INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_das_torque =
      inverter_open_file(folder, INV_REG_TORQUECMD, INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_inv_torque =
      inverter_open_file(folder, INV_REG_I_CMD, INV_IDX_RIGHT);
  inverters_files[INV_IDX_RIGHT].inverter_inv_torque_actual =
      inverter_open_file(folder, INV_REG_I_ACTUAL, INV_IDX_RIGHT);
}

void inverter_close_files(inverter_files_t *inverters_files) {
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

const char *inverter_filename(int reg_id) {
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

bool inverter_to_file(inverter_message_INV_RESPONSE *data, int id,
                      inverter_files_t *inverters_files,
                      inverter_data_t inverters_data[2]) {
  inverter_data_t *inv =
      (id == inv_id_request_left || id == inv_id_response_left)
          ? &inverters_data[INV_IDX_LEFT]
          : &inverters_data[INV_IDX_RIGHT];
  inverter_files_t *file =
      (id == inv_id_request_left || id == inv_id_response_left)
          ? &inverters_files[INV_IDX_LEFT]
          : &inverters_files[INV_IDX_RIGHT];

  static char str_buffer[INV_STR_SIZE];

  switch (data->reg_id) {
  case INV_REG_MOT_TEMP:
    if (file->inverter_motor_temp == NULL)
      return false;
    fprintf(file->inverter_motor_temp,
#ifdef CANLIB_TIMESTAMP
            "%" PRIu64
#endif // CANLIB_TIMESTAMP
            ","
            "%f"
            "\n",
#ifdef CANLIB_TIMESTAMP
            inv->inverter_motor_temp._timestamp,
#endif // CANLIB_TIMESTAMP
            inv->inverter_motor_temp.motor_temp);
    fflush(file->inverter_motor_temp);
    break;
  case INV_REG_INV_TEMP:
    if (file->inverter_temp == NULL)
      return false;
    fprintf(file->inverter_temp,
#ifdef CANLIB_TIMESTAMP
            "%" PRIu64
#endif // CANLIB_TIMESTAMP
            ","
            "%f"
            "\n",
#ifdef CANLIB_TIMESTAMP
            inv->inverter_temp._timestamp,
#endif // CANLIB_TIMESTAMP
            inv->inverter_temp.temp);
    fflush(file->inverter_temp);
    break;
  case INV_REG_SPEED:
    if (file->inverter_speed == NULL)
      return false;
    fprintf(file->inverter_speed,
#ifdef CANLIB_TIMESTAMP
            "%" PRIu64
#endif // CANLIB_TIMESTAMP
            ","
            "%f"
            "\n",
#ifdef CANLIB_TIMESTAMP
            inv->inverter_speed._timestamp,
#endif // CANLIB_TIMESTAMP
            inv->inverter_speed.speed);
    fflush(file->inverter_speed);
    break;
  case INV_REG_STATUS:
    if (file->inverter_status == NULL)
      return false;
    memset(str_buffer, 0, INV_STR_SIZE);
    invlib_to_string_InvStatus(inv->inverter_status.status, str_buffer);
    fprintf(file->inverter_status,
#ifdef CANLIB_TIMESTAMP
            "%" PRIu64
#endif // CANLIB_TIMESTAMP
            ","
            "%" PRIu32 ","
            "\"%s\""
            "\n",
#ifdef CANLIB_TIMESTAMP
            inv->inverter_status._timestamp,
#endif // CANLIB_TIMESTAMP
            inv->inverter_status.status, str_buffer);
    fflush(file->inverter_status);
    break;
  case INV_REG_ERRORS:
    if (file->inverter_errors == NULL)
      return false;
    memset(str_buffer, 0, INV_STR_SIZE);
    invlib_to_string_InvErrors(inv->inverter_errors.errors, str_buffer);
    fprintf(file->inverter_errors,
#ifdef CANLIB_TIMESTAMP
            "%" PRIu64
#endif // CANLIB_TIMESTAMP
            ","
            "%" PRIu32 ","
            "\"%s\""
            "\n",
#ifdef CANLIB_TIMESTAMP
            inv->inverter_errors._timestamp,
#endif // CANLIB_TIMESTAMP
            inv->inverter_errors.errors, str_buffer);
    fflush(file->inverter_errors);
    break;
  case INV_REG_IOINFO:
    if (file->inverter_io_info == NULL)
      return false;
    memset(str_buffer, 0, INV_STR_SIZE);
    invlib_to_string_InvIOInfo(inv->inverter_io_info.io_info, str_buffer);
    fprintf(file->inverter_io_info,
#ifdef CANLIB_TIMESTAMP
            "%" PRIu64
#endif // CANLIB_TIMESTAMP
            ","
            "%" PRIu16 ","
            "\"%s\""
            "\n",
#ifdef CANLIB_TIMESTAMP
            inv->inverter_io_info._timestamp,
#endif // CANLIB_TIMESTAMP
            inv->inverter_io_info.io_info, str_buffer);
    fflush(file->inverter_io_info);
    break;
  case INV_REG_TORQUECMD:
    if (file->inverter_das_torque == NULL)
      return false;
    fprintf(file->inverter_das_torque,
#ifdef CANLIB_TIMESTAMP
            "%" PRIu64
#endif // CANLIB_TIMESTAMP
            ","
            "%f"
            "\n",
#ifdef CANLIB_TIMESTAMP
            inv->inverter_das_torque._timestamp,
#endif // CANLIB_TIMESTAMP
            inv->inverter_das_torque.torque);
    fflush(file->inverter_das_torque);
    break;
  case INV_REG_I_CMD:
    if (file->inverter_inv_torque == NULL)
      return false;
    fprintf(file->inverter_inv_torque,
#ifdef CANLIB_TIMESTAMP
            "%" PRIu64
#endif // CANLIB_TIMESTAMP
            ","
            "%f"
            "\n",
#ifdef CANLIB_TIMESTAMP
            inv->inverter_inv_torque._timestamp,
#endif // CANLIB_TIMESTAMP
            inv->inverter_inv_torque.torque);
    fflush(file->inverter_inv_torque);
    break;
  case INV_REG_I_ACTUAL:
    if (file->inverter_inv_torque_actual == NULL)
      return false;
    fprintf(file->inverter_inv_torque_actual,
#ifdef CANLIB_TIMESTAMP
            "%" PRIu64
#endif // CANLIB_TIMESTAMP
            ","
            "%f"
            "\n",
#ifdef CANLIB_TIMESTAMP
            inv->inverter_inv_torque_actual._timestamp,
#endif // CANLIB_TIMESTAMP
            inv->inverter_inv_torque_actual.torque);
    fflush(file->inverter_inv_torque_actual);
    break;
  default:
    return false;
    break;
  }
  return true;
}

bool parse_inverter(inverter_message_INV_RESPONSE *data, int id,
                    inverter_data_t inverters_data[2]) {
  inverter_data_t *inv =
      (id == inv_id_request_left || id == inv_id_response_left)
          ? &inverters_data[INV_IDX_LEFT]
          : &inverters_data[INV_IDX_RIGHT];
  switch (data->reg_id) {
  case INV_REG_MOT_TEMP:
#ifdef CANLIB_TIMESTAMP
    inv->inverter_motor_temp._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
    inv->inverter_motor_temp.motor_temp = inverter_get_motor_temp(data);
    break;
  case INV_REG_INV_TEMP:
#ifdef CANLIB_TIMESTAMP
    inv->inverter_temp._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
    inv->inverter_temp.temp = inverter_get_inverter_temp(data);
    break;
  case INV_REG_SPEED:
#ifdef CANLIB_TIMESTAMP
    inv->inverter_speed._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
    inv->inverter_speed.speed = rpm_to_rads(inverter_get_speed(data));
    break;
  case INV_REG_STATUS:
#ifdef CANLIB_TIMESTAMP
    inv->inverter_status._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
    inv->inverter_status.status = inverter_get_status(data);
    break;
  case INV_REG_ERRORS:
#ifdef CANLIB_TIMESTAMP
    inv->inverter_errors._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
    inv->inverter_errors.errors = inverter_get_errors(data);
    break;
  case INV_REG_IOINFO:
#ifdef CANLIB_TIMESTAMP
    inv->inverter_io_info._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
    inv->inverter_io_info.io_info = inverter_get_io_info(data);
    break;
  case INV_REG_TORQUECMD:
#ifdef CANLIB_TIMESTAMP
    inv->inverter_das_torque._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
    inv->inverter_das_torque.torque = inverter_get_das_torque(data);
    break;
  case INV_REG_I_CMD:
#ifdef CANLIB_TIMESTAMP
    inv->inverter_inv_torque._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
    inv->inverter_inv_torque.torque = inverter_get_inv_torque(data);
    break;
  case INV_REG_I_ACTUAL:
#ifdef CANLIB_TIMESTAMP
    inv->inverter_inv_torque_actual._timestamp = data->_timestamp;
#endif // CANLIB_TIMESTAMP
    inv->inverter_inv_torque_actual.torque =
        inverter_get_inv_torque_actual(data);
    break;
  default:
    return false;
    break;
  }
  return true;
}

int invlib_to_string_InvStatus(uint64_t value, char *buffer) {
  int offset = 0;
  if (CANLIB_BITTEST_BITMASK(value, 1))
    offset += sprintf(buffer + offset, "DRIVE_ENABLE ");
  if (CANLIB_BITTEST_BITMASK(value, 2))
    offset += sprintf(buffer + offset, "NCR0 ");
  if (CANLIB_BITTEST_BITMASK(value, 4))
    offset += sprintf(buffer + offset, "LIMP ");
  if (CANLIB_BITTEST_BITMASK(value, 8))
    offset += sprintf(buffer + offset, "LIMM ");
  if (CANLIB_BITTEST_BITMASK(value, 16))
    offset += sprintf(buffer + offset, "DRIVE_OK ");
  if (CANLIB_BITTEST_BITMASK(value, 32))
    offset += sprintf(buffer + offset, "ICNS ");
  if (CANLIB_BITTEST_BITMASK(value, 64))
    offset += sprintf(buffer + offset, "T_NLIM ");
  if (CANLIB_BITTEST_BITMASK(value, 128))
    offset += sprintf(buffer + offset, "P_N ");
  if (CANLIB_BITTEST_BITMASK(value, 256))
    offset += sprintf(buffer + offset, "N_I ");
  if (CANLIB_BITTEST_BITMASK(value, 512))
    offset += sprintf(buffer + offset, "N0 ");
  if (CANLIB_BITTEST_BITMASK(value, 1024))
    offset += sprintf(buffer + offset, "RSW ");
  if (CANLIB_BITTEST_BITMASK(value, 2048))
    offset += sprintf(buffer + offset, "CAL0 ");
  if (CANLIB_BITTEST_BITMASK(value, 4096))
    offset += sprintf(buffer + offset, "CAL ");
  if (CANLIB_BITTEST_BITMASK(value, 8192))
    offset += sprintf(buffer + offset, "TOL ");
  if (CANLIB_BITTEST_BITMASK(value, 16384))
    offset += sprintf(buffer + offset, "DRIVE_READY ");
  if (CANLIB_BITTEST_BITMASK(value, 32768))
    offset += sprintf(buffer + offset, "BRK ");
  if (CANLIB_BITTEST_BITMASK(value, 65536))
    offset += sprintf(buffer + offset, "SIGN_MAG ");
  if (CANLIB_BITTEST_BITMASK(value, 131072))
    offset += sprintf(buffer + offset, "NCLIP ");
  if (CANLIB_BITTEST_BITMASK(value, 262144))
    offset += sprintf(buffer + offset, "NCLIPP ");
  if (CANLIB_BITTEST_BITMASK(value, 524288))
    offset += sprintf(buffer + offset, "NCLIPM ");
  if (CANLIB_BITTEST_BITMASK(value, 1048576))
    offset += sprintf(buffer + offset, "IRD_DIG ");
  if (CANLIB_BITTEST_BITMASK(value, 2097152))
    offset += sprintf(buffer + offset, "IUSE_RCHD ");
  if (CANLIB_BITTEST_BITMASK(value, 4194304))
    offset += sprintf(buffer + offset, "IRD_N ");
  if (CANLIB_BITTEST_BITMASK(value, 8388608))
    offset += sprintf(buffer + offset, "IRD_TI ");
  if (CANLIB_BITTEST_BITMASK(value, 16777216))
    offset += sprintf(buffer + offset, "IRD_TIR ");
  if (CANLIB_BITTEST_BITMASK(value, 33554432))
    offset += sprintf(buffer + offset, "HZ10 ");
  if (CANLIB_BITTEST_BITMASK(value, 67108864))
    offset += sprintf(buffer + offset, "IRD_TM ");
  if (CANLIB_BITTEST_BITMASK(value, 134217728))
    offset += sprintf(buffer + offset, "IRD_ANA ");
  if (CANLIB_BITTEST_BITMASK(value, 268435456))
    offset += sprintf(buffer + offset, "IWCNS ");
  if (CANLIB_BITTEST_BITMASK(value, 536870912))
    offset += sprintf(buffer + offset, "RFE_PULSE ");
  if (CANLIB_BITTEST_BITMASK(value, 1073741824))
    offset += sprintf(buffer + offset, "MD ");
  if (CANLIB_BITTEST_BITMASK(value, 2147483648))
    offset += sprintf(buffer + offset, "HND_WHL ");
  buffer[offset] = '\0';
  return offset;
}

int invlib_to_string_InvErrors(uint64_t value, char *buffer) {
  int offset = 0;
  if (CANLIB_BITTEST_BITMASK(value, 1))
    offset += sprintf(buffer + offset, "BAD_PARAM ");
  if (CANLIB_BITTEST_BITMASK(value, 2))
    offset += sprintf(buffer + offset, "HW_FAULT ");
  if (CANLIB_BITTEST_BITMASK(value, 4))
    offset += sprintf(buffer + offset, "SAFETY_FAULT ");
  if (CANLIB_BITTEST_BITMASK(value, 8))
    offset += sprintf(buffer + offset, "CAN_TIMEOUT ");
  if (CANLIB_BITTEST_BITMASK(value, 16))
    offset += sprintf(buffer + offset, "ENCODER_ERR ");
  if (CANLIB_BITTEST_BITMASK(value, 32))
    offset += sprintf(buffer + offset, "NO_POWER_VOLTAGE ");
  if (CANLIB_BITTEST_BITMASK(value, 64))
    offset += sprintf(buffer + offset, "HI_MOTOR_TEMP ");
  if (CANLIB_BITTEST_BITMASK(value, 128))
    offset += sprintf(buffer + offset, "HI_DEVICE_TEMP ");
  if (CANLIB_BITTEST_BITMASK(value, 256))
    offset += sprintf(buffer + offset, "OVERVOLTAGE ");
  if (CANLIB_BITTEST_BITMASK(value, 512))
    offset += sprintf(buffer + offset, "OVERCURRENT ");
  if (CANLIB_BITTEST_BITMASK(value, 1024))
    offset += sprintf(buffer + offset, "RACEAWAY ");
  if (CANLIB_BITTEST_BITMASK(value, 2048))
    offset += sprintf(buffer + offset, "USER_ERR ");
  if (CANLIB_BITTEST_BITMASK(value, 4096))
    offset += sprintf(buffer + offset, "UNKNOWN_ERR_12 ");
  if (CANLIB_BITTEST_BITMASK(value, 8192))
    offset += sprintf(buffer + offset, "UNKNOWN_ERR_13 ");
  if (CANLIB_BITTEST_BITMASK(value, 16384))
    offset += sprintf(buffer + offset, "CURRENT_ERR ");
  if (CANLIB_BITTEST_BITMASK(value, 32768))
    offset += sprintf(buffer + offset, "BALLAST_OVERLOAD ");
  if (CANLIB_BITTEST_BITMASK(value, 65536))
    offset += sprintf(buffer + offset, "DEVICE_ID_ERR ");
  if (CANLIB_BITTEST_BITMASK(value, 131072))
    offset += sprintf(buffer + offset, "RUN_SIG_FAULT ");
  if (CANLIB_BITTEST_BITMASK(value, 262144))
    offset += sprintf(buffer + offset, "UNKNOWN_ERR_19 ");
  if (CANLIB_BITTEST_BITMASK(value, 524288))
    offset += sprintf(buffer + offset, "UNKNOWN_ERR_20 ");
  if (CANLIB_BITTEST_BITMASK(value, 1048576))
    offset += sprintf(buffer + offset, "POWERVOLTAGE_WARN ");
  if (CANLIB_BITTEST_BITMASK(value, 2097152))
    offset += sprintf(buffer + offset, "HI_MOTOR_TEMP_WARN ");
  if (CANLIB_BITTEST_BITMASK(value, 4194304))
    offset += sprintf(buffer + offset, "HI_DEVICE_TEMP_WARN ");
  if (CANLIB_BITTEST_BITMASK(value, 8388608))
    offset += sprintf(buffer + offset, "VOUT_LIMIT_WARN ");
  if (CANLIB_BITTEST_BITMASK(value, 16777216))
    offset += sprintf(buffer + offset, "OVERCURRENT_WARN ");
  if (CANLIB_BITTEST_BITMASK(value, 33554432))
    offset += sprintf(buffer + offset, "RACEAWAY_WARN ");
  if (CANLIB_BITTEST_BITMASK(value, 67108864))
    offset += sprintf(buffer + offset, "UNKNOWN_ERR_27 ");
  if (CANLIB_BITTEST_BITMASK(value, 134217728))
    offset += sprintf(buffer + offset, "UNKNOWN_ERR_28 ");
  if (CANLIB_BITTEST_BITMASK(value, 268435456))
    offset += sprintf(buffer + offset, "UNKNOWN_ERR_29 ");
  if (CANLIB_BITTEST_BITMASK(value, 536870912))
    offset += sprintf(buffer + offset, "UNKNOWN_ERR_30 ");
  if (CANLIB_BITTEST_BITMASK(value, 1073741824))
    offset += sprintf(buffer + offset, "BALLAST_OVERLOAD_WARN ");
  buffer[offset] = '\0';
  return offset;
}

int invlib_to_string_InvIOInfo(uint64_t value, char *buffer) {
  int offset = 0;
  if (CANLIB_BITTEST_BITMASK(value, 1))
    offset += sprintf(buffer + offset, "LMT1 ");
  if (CANLIB_BITTEST_BITMASK(value, 2))
    offset += sprintf(buffer + offset, "LMT2 ");
  if (CANLIB_BITTEST_BITMASK(value, 4))
    offset += sprintf(buffer + offset, "IN2 ");
  if (CANLIB_BITTEST_BITMASK(value, 8))
    offset += sprintf(buffer + offset, "IN1 ");
  if (CANLIB_BITTEST_BITMASK(value, 16))
    offset += sprintf(buffer + offset, "FRG ");
  if (CANLIB_BITTEST_BITMASK(value, 32))
    offset += sprintf(buffer + offset, "RFE ");
  if (CANLIB_BITTEST_BITMASK(value, 64))
    offset += sprintf(buffer + offset, "UNK6 ");
  if (CANLIB_BITTEST_BITMASK(value, 128))
    offset += sprintf(buffer + offset, "UNK7 ");
  if (CANLIB_BITTEST_BITMASK(value, 256))
    offset += sprintf(buffer + offset, "OUT1 ");
  if (CANLIB_BITTEST_BITMASK(value, 512))
    offset += sprintf(buffer + offset, "OUT2 ");
  if (CANLIB_BITTEST_BITMASK(value, 1024))
    offset += sprintf(buffer + offset, "BTB ");
  if (CANLIB_BITTEST_BITMASK(value, 2048))
    offset += sprintf(buffer + offset, "GO ");
  if (CANLIB_BITTEST_BITMASK(value, 4096))
    offset += sprintf(buffer + offset, "OUT3 ");
  if (CANLIB_BITTEST_BITMASK(value, 8192))
    offset += sprintf(buffer + offset, "OUT4 ");
  if (CANLIB_BITTEST_BITMASK(value, 16384))
    offset += sprintf(buffer + offset, "G_OFF ");
  if (CANLIB_BITTEST_BITMASK(value, 32768))
    offset += sprintf(buffer + offset, "BRK1 ");
  buffer[offset] = '\0';
  return offset;
}