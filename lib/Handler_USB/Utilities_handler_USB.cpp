#include "Utilities_handler_USB.h"

//* ------ Definicion de funciones auxliares -AUFD- ------------

bool validate_response_quiery(uint8_t _query);
bool validate_response_config(uint8_t _config, String& _buff);

void fnc_clear_HF() { eeprom_native_set_block(_mxa_horary_enable, _EECHAR_RESET_, space_memory_horary); }
void fnc_clear_TPS() { eeprom_native_set_block(_mxa_seq_default_enable, _EECHAR_RESET_, space_memory_defaults); }
void fnc_clear_CV() { eeprom_native_set_block(_mxa_events_enable, _EECHAR_RESET_, space_memory_events); }
void fnc_clear_BTN() { eeprom_native_set_block(_mxa_btns_enable, _EECHAR_RESET_, space_memory_btsn); }
void fnc_clear_SYG() { eeprom_native_set_block(_mxa_sync, _EECHAR_RESET_, space_memory_sync); }

void fnc_write_TSP(uint8_t _nTsp, Sequence_default& _newSQD);
void fnc_write_CV(uint8_t _nCv, Event& _newEvent);
void fnc_write_BTN(uint8_t _nBtn, Sequence& _newSQ);

//* --------------- END (-AUFD-) ------------------------------

Uart_comunication usb_port;
String str_buffer_usb = "";

#define MAX_CMDs_ERROR 5
const char* CMDs_response_errors[MAX_CMDs_ERROR] = {
  "<ERR0>", //0 Error de comunicacion
  "<ERR1>", //1 Error en la transmisión
  "<ERR2>", //2 Error no se reconoce el comando
  "<ERR3>", //3 Error de sintaxis del comando
  "<ERR4>"  //4 Error de sincronización
};

void response_error(uint8_t error_code) {
  if (!(error_code < MAX_CMDs_ERROR)) return;
  uart_usb_port.print(CMDs_response_errors[error_code]);
  H_DEBUG_PRINTLN(CMDs_response_errors[error_code]);
}

enum mode_options : uint8_t {
  QUIERY_MODE,
  CONFIG_MODE,
  UPDATE_MODE
};

uint8_t hdl_mode = QUIERY_MODE;

const char* init_finit_conf[2] = {
  //CMDs para entrar/salir del modo de configuracion
  "<SG+CFGM=1>", "<SG+CFGM=0>"
};

#define MAX_QUERIES 7
const char* queries[MAX_QUERIES] = {
  //Validaciones
  "<SG>",      //0 Validacion general
  "<SG+FRW>",  //1 Solicitud de firmware
  "<SG+HDW>",  //2 Solicitud de hardware
  "<SG+GPS>",  //3 Solicitud de validacion de GPS
  "<SG+MC>" ,  //4 Solicitud de direccion MAC
  "<SG+TIME>", //5 Solicitud de hora actual del controlador
  "<SG+DSG>"   //6 Solicitud de la descripcion general (screen)
};

const char* update_cmds[2] = {
  //CMDs para entrar/salir del modo de configuracion
  "<SG+UPDATE>", "<SG+RP2040_NEW_F_UPDATE>"
};

#define MAX_CONFIGS 9
const char* cmd_config[MAX_CONFIGS] = {
  //Configuraciones
  "<SG+PS=",            //0 Configuracion de la secuencia principal 
  "<SG+HF=",            //1 Configuracion del horario de funcionamiento
  "<SG+TPS=",           //2 Configuracion para secuencias predeterminadas
  "<SG+CV=",            //3 Configuracion para eventos
  "<SG+DSG=",           //4 Configuracion de la descripcion general (DSG)
  "<SG+TIME=",          //5 Configuracion de la hora actual del controlador
  "<SG+BTN=",           //6 Configuracion para botones peatonales
  "<SG+SYG=",           //7 Configuracion para intervalos de sincronizacion
  "<SG+RST_DFT_ALL>"    //8 Comando para reseteo de fabrica
};

uint8_t decode_MSJ(String& _buff) {
  //? Validar sintaxys inicial 
  //Sintaxys de caracters correctos
  if (!(_buff[0] == '<' && _buff[_buff.length() - 1] == '>')) {
    return decode_options::CMD_no_cmd;
  }

  //? Modo solicitudes
  if (hdl_mode == QUIERY_MODE) {
    //Revisar si se esta inciando una nueva configuracion
    if (_buff.compareTo(init_finit_conf[0]) == 0) {
      hdl_mode = mode_options::CONFIG_MODE;
      return decode_options::CMD_enter_CONFIG;
    }

    uint8_t query_index = 0xFF;
    //Revisar si es un comando > si lo es, responder
    for (uint8_t i = 0; i < MAX_QUERIES; i++) {
      if (_buff.compareTo(queries[i]) == 0) {
        query_index = i;
        break;
      }
    }

    if (query_index != 0xFF) {
      // Validacion de la query 
      bool q_resul = validate_response_quiery(query_index);
      uint8_t decode_query_resul = (q_resul) ? decode_options::CMD_query : decode_options::CMD_syntax_error;
      return decode_query_resul;
    }

    // Revisar si se trata de una nueva actualizacion
    if (_buff.compareTo(update_cmds[0]) == 0) {
      hdl_mode = mode_options::UPDATE_MODE;
      return decode_options::CMD_enter_UPDATE;
    }
  }
  //? Modo configuracion
  else if (hdl_mode == CONFIG_MODE) {
    if (_buff.compareTo(init_finit_conf[1]) == 0) {
      return decode_options::CMD_exit_CONFIG;
    }

    uint8_t config_index = 0xFF;
    for (uint8_t i = 0; i < MAX_CONFIGS; i++) {
      if (_buff.indexOf(cmd_config[i]) != -1) {
        config_index = i;
        break;
      }
    }

    if (config_index != 0xFF) {
      bool validate_config = validate_response_config(config_index, _buff);
      uint8_t config_resul = (validate_config) ? decode_options::CMD_new_CONFIG : decode_options::CMD_syntax_error;
      return config_resul;
    }
  }
  //? Modo de actualizacion
  else if (hdl_mode == UPDATE_MODE) {
    //! En caso de no recibir la confirmacion el controlador se resetea (software) de forma automatica.

    if (_buff.compareTo(update_cmds[1]) == 0) {
      //? Recibir validacion
      return decode_options::CMD_confirm_UPDATE;
    }
  }

  return decode_options::CMD_no_cmd;
}

bool validate_response_quiery(uint8_t _query) {
  bool resul = true;

  if (!(_query < MAX_QUERIES)) return false;

  /* Queries */
  String str_h_resp = "";

  switch (_query) {
  case 0: {
    //  0 - "<SG>"
    //TODO: Revisar que el controlador no este haciedo procesos con mayor preoridad
    uart_usb_port.print("<SG_OK>");
    break;
  }
  case 1: {
    // 1 - "<SG+FRW>"
    str_h_resp = "<" + String(__FIRMWARE_VERSION__) + ">";
    uart_usb_port.print(str_h_resp.c_str());
    break;
  }

  case 2: {
    // 2 - "<SG+HDW>"
    str_h_resp = "<" + String(__HARDWARE_VESION__) + ">";
    uart_usb_port.print(str_h_resp.c_str());
    break;
  }

  case 3: {
    // 3 - "<SG+GPS>"
    str_h_resp = "<GPS_" + String(sb_enable_gps.get()) + ">";
    uart_usb_port.print(str_h_resp);
    break;
  }

  case 4: {
    // 4- "<SG+MC>"
    uart_usb_port.print("<" + String(get_mac_ctrs()) + ">");
    break;
  }

  case 5: {
    // 5 - "<SG+TIME>"
    DateTime now = get_safe_now();
    uart_usb_port.print("<" + now.timestamp(DateTime::TIMESTAMP_FULL) + ">");
    break;
  }

  case 6: {
    // 6 - "<SG+DSG>"
    uart_usb_port.print("<" + str_dsc_gen + ">");
    break;
  }

  default:
    resul = false;
    break;
  }

  return resul;
}

bool validate_response_config(uint8_t _config, String& _buff) {
  bool resul = true;

  if (!(_config < MAX_CONFIGS)) return false;

  switch (_config) {
  case 0: {
    //* 0 - > "<SG+PS="
    _buff = _buff.substring(7, _buff.length() - 1);
    bool valid = validate_str_sequence(_buff);

    if (!valid) return false;

    //* Secuencia valida, save eeprom memory
    sdv_sequence.get_buffer_SEQ(u8_buffer_aux_seq);
    eeprom_native_write_block(_mxa_main_seq, u8_buffer_aux_seq, MAX_LENGTH_SEQUENCE);
    uart_usb_port.print("<OK_PS>");
    break;
  }

  case 1: {
    //* 1 - > "<SG+HF="
    if (_buff.compareTo("<SG+HF=0>") == 0) {
      fnc_clear_HF();
    }
    else {
      //HF validacion
      _buff = _buff.substring(7, _buff.length() - 1);
      bool validate_sqf = validate_str_deff_sequence(_buff);

      if (!validate_sqf) return false;

      //* Cambio de horario valido, almacenarlo.
      sdv_sequence_dff.get_buffer_SEQDF(u8_buffer_aux_seq_df);
      eeprom_native_update_byte(_mxa_horary_enable, __ENABLE_CHAR__);
      eeprom_native_write_block(_mxa_horary_seq, u8_buffer_aux_seq_df, MAX_LENGTH_SEQUENCE_DEF);
    }
    uart_usb_port.print("<OK_HF>");
    break;
  }

  case 2: {
    //* 2 -> "<SG+TPS=Nc:"
    if (_buff.compareTo("<SG+TPS=0>") == 0) {
      //Reset parameter
      fnc_clear_TPS();
    }
    else {
      bool index_validation = false;

      int index = _buff.indexOf(':');
      if (index == -1) return false;

      // <SG+TPS=Nc:
      int nC = _buff.substring(8, index).toInt();
      if (!(nC >= 1 && nC <= 5)) return false;

      _buff = _buff.substring(index + 1, _buff.length() - 1);
      bool valid = validate_str_deff_sequence(_buff);

      if (!valid) return false;

      //*Cambio de secuencia valido
      fnc_write_TSP(nC, sdv_sequence_dff);
    }
    uart_usb_port.print("<OK_TPS>");
    break;
  }

  case 3: {
    //* 3 -> "<SG+CV=Nc:"
    if (_buff.compareTo("<SG+CV=0>") == 0) {
      //* Resetear
      fnc_clear_CV();
    }
    else {
      int index = _buff.indexOf(':');
      if (index == -1) return false;

      //<SG+CV=Nc:
      int nC = _buff.substring(7, index).toInt();
      if (!(nC >= 1 && nC <= 20)) return false;

      _buff = _buff.substring(index + 1, _buff.length() - 1);

      bool event_valid = validate_str_event(_buff);
      if (!event_valid)  return false;

      fnc_write_CV(nC, sdv_event);
    }
    uart_usb_port.print("<OK_CV>");
    break;
  }

  case 4: {
    //4 -> "<SG+DSG="
    _buff = _buff.substring(8, _buff.length() - 1);

    if (!(_buff.length() > 0 && _buff.length() < 100)) return false;

    uint8_t _buff_dsg[100];
    for (uint8_t i = 0; i < 100; i++) {
      _buff_dsg[i] = 0;
    }

    for (uint8_t i = 0; i <= _buff.length(); i++) {
      //Agregar final de linea
      if (i == _buff.length()) _buff_dsg[i] = __EXT__;
      else _buff_dsg[i] = _buff[i];
    }

    eeprom_native_write_block(_mxa_dsg, _buff_dsg, 100);

    uart_usb_port.print("<OK_DSG>");
    break;
  }

  case 5: {
    //5 -> "<SG+TIME="
    _buff = _buff.substring(9, _buff.length() - 1);
    // DateTime

    bool dateTimeValid = validDateISO8601(_buff.c_str());

    if (!dateTimeValid) return false;

    DateTime _aux_t(_buff.c_str());

    if (!_aux_t.isValid()) return false;
    //* Actualiza la hora del controlador
    native_adjust_time_rtc(_aux_t);
    uart_usb_port.print("<OK_TIME>");
    break;
  }

  case 6: {
    //* 6-> "<SG+BTN="
    if (_buff.compareTo("<SG+BTN=0>") == 0) {
      //*Resetear 
      fnc_clear_BTN();
    }
    else {
      //<SG+BTN=nb:>"
      int index = _buff.indexOf(':');
      int num_b = _buff.substring(8, index).toInt();
      if (!(num_b >= 1 && num_b <= MAX_INPUTS)) return false;

      _buff = _buff.substring(index + 1, _buff.length() - 1);
      bool btn_valid = validate_str_sequence(_buff);
      if (!btn_valid) return false;

      fnc_write_BTN(num_b, sdv_sequence);
    }
    uart_usb_port.print("OK_BTNR");
    break;
  }

  case 7: {
    //7 -> "<SG+SYG=Hr:Iv>"
    if (_buff.indexOf("<SG+SYG=0>") == 0) {
      fnc_clear_SYG();
    }
    else {
      int index = _buff.indexOf(':');
      if (index == -1) return false;

      //obtener hhmmss
      String aux = _buff.substring(8, index);

      if (aux.length() < 6) return false;

      uint8_t hh = ((aux.charAt(0) - '0') * 10) + (aux.charAt(1) - '0');
      uint8_t mm = ((aux.charAt(2) - '0') * 10) + (aux.charAt(3) - '0');
      uint8_t ss = ((aux.charAt(4) - '0') * 10) + (aux.charAt(5) - '0');

      bool dt_valid = ShortTime().valid_time(hh, mm, ss);
      if (!dt_valid) return false;

      aux = _buff.substring(index + 1, _buff.length() - 1);

      uint32_t u32_time = 0;
      bool val_tim = str_to_u32(aux, u32_time);
      if (!val_tim) return false;

      if (!(u32_time < UINT32_MAX)) {
        response_error(3);
        break;
      }

      //* Sincronizacion valida, actualizarla
      uint32_t _time_sync = (hh << 16) | (mm << 8) | ss;
      uint32_t _total_time = u32_time;

      const uint8_t size_sync_buffer = 9;
      uint8_t _sync_buffer[size_sync_buffer] = {
        __ENABLE_CHAR__,
        //? Horario sincronizacion Byte 0, Byte 1, Byte 2, Byte 3
        static_cast<uint8_t>((_time_sync >> 24) & 0xFF),
        static_cast<uint8_t>((_time_sync >> 16) & 0xFF),
        static_cast<uint8_t>((_time_sync >> 8) & 0xFF),
        static_cast<uint8_t>(_time_sync & 0xFF),
        //? Duracion de secuencia Byte 0, Byte 1, Byte 2, Byte 3
        static_cast<uint8_t>((_total_time >> 24) & 0xFF),
        static_cast<uint8_t>((_total_time >> 16) & 0xFF),
        static_cast<uint8_t>((_total_time >> 8) & 0xFF),
        static_cast<uint8_t>(_total_time & 0xFF)
      };

      //* 7 -> Tamanio maximo para el buffer
      eeprom_native_write_block(_mxa_sync, _sync_buffer, size_sync_buffer);
    }
    uart_usb_port.print("<OK_SYG>");
    break;
  }

  case 8: {
    //8 - > "<SG+RST_DFT_ALL>"
    if (!(_buff.compareTo("<SG+RST_DFT_ALL>") == 0)) return false;
    else {
      //! Comando para resetear toda la informacion
      write_initial_data();
    }
    uart_usb_port.print("<OK_RTSAD>");
    break;
  }
  default:
    break;
  }

  return resul;
}

void fnc_write_TSP(uint8_t _nTsp, Sequence_default& _newSQD) {
  //! Validacion inicial
  if (!(_nTsp >= 1 && _nTsp <= MAX_DEFAULT_SEQ)) return;

  uint8_t _mxa_ = _nTsp - 1;
  _newSQD.get_buffer_SEQDF(u8_buffer_aux_seq_df);

  eeprom_native_update_byte(_mxa_seq_default_enable, __ENABLE_CHAR__);
  eeprom_native_write_block(array_mxa_seq_defaults[_mxa_], u8_buffer_aux_seq_df, MAX_LENGTH_SEQUENCE_DEF);
}

void fnc_write_CV(uint8_t _nCv, Event& _newEvent) {
  if (!(_nCv >= 1 && _nCv <= MAX_EVENTS)) return;

  uint8_t _mxa_ = _nCv - 1;
  _newEvent.get_buffer_EVT(u8_buffer_aux_event);

  eeprom_native_update_byte(_mxa_events_enable, __ENABLE_CHAR__);
  eeprom_native_write_block(array_mxa_events[_mxa_], u8_buffer_aux_event, MAX_LENGTH_EVENT);
}

void fnc_write_BTN(uint8_t _nBtn, Sequence& _newSQ) {
  if (!(_nBtn >= 1 && _nBtn <= MAX_SEQ_BUTTONS))return;

  uint8_t _mxa_ = _nBtn - 1;
  _newSQ.get_buffer_SEQ(u8_buffer_aux_seq);

  eeprom_native_update_byte(_mxa_btns_enable, __ENABLE_CHAR__);
  eeprom_native_write_block(array_mxa_buttons[_mxa_], u8_buffer_aux_seq, MAX_LENGTH_SEQUENCE);
}