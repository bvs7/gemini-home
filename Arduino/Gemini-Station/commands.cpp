
#include "commands.h"

#include "connection.h"

char serial_command_buffer_[64];
SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

void show_conn_sett_ssid(Stream *serial){
  serial->print("SSID:    ");
  serial->println(conn_sett.ssid);
}
void show_conn_sett_pass(Stream *serial){
  serial->print("PASS:    ");
  serial->println(conn_sett.pass);
}
void show_conn_sett_server(Stream *serial){
  serial->print("SERVER:  ");
  serial->println(conn_sett.server);
}
void show_conn_sett_mqtt_id(Stream *serial){
  serial->print("MQTT_ID: ");
  serial->println(conn_sett.mqtt_id);
}

void show_conn_sett_all(Stream *serial){
  show_conn_sett_ssid(serial);
  show_conn_sett_pass(serial);
  show_conn_sett_server(serial);
  show_conn_sett_mqtt_id(serial);
}

//This is the default handler, and gets called when no other command matches. 
void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
  sender->GetSerial()->print("Unrecognized command [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("]");
}

void read_all(SerialCommands *sender, char *buf){
  char *var = sender->Next();
  if(var == NULL){
    return;
  }
  size_t buf_len = SETTINGS_SIZE - 1;
  size_t len = strlen(var);
  if(len>buf_len){
    len = buf_len;
  }
  strncat(buf,var, len);
  buf_len -= len;
  var = sender->Next();
  while(!var == NULL && buf_len > 0){
    strcat(buf, " ");
    buf_len--;
    size_t len = strlen(var);
    if(len>buf_len){
    len = buf_len;
    }
    strncat(buf,var, len);
    buf_len -= len;
    
    var = sender->Next();
  }
}

void cmd_set(SerialCommands *sender){
  char *var = sender->Next();
  if(var == NULL){
    sender->GetSerial()->println("Error: no setting name");
    return;
  }
  
  char cmd[16] = "";
  strncpy(cmd, var, 16-1);
  sender->GetSerial()->print("Set ");
  if(strcmp(cmd, "ssid") == 0){
    strcpy(conn_sett.ssid, "");
    read_all(sender, conn_sett.ssid);
    show_conn_sett_ssid(sender->GetSerial());
    save_conn();
  }
  else if(strcmp(cmd, "pass") == 0){
    strcpy(conn_sett.pass, "");
    read_all(sender, conn_sett.pass);
    show_conn_sett_pass(sender->GetSerial());
    save_conn();
  }
  else if(strcmp(cmd, "server") == 0){
    strcpy(conn_sett.server, "");
    read_all(sender, conn_sett.server);
    mqtt_client.disconnect();
    mqtt_client.setServer(conn_sett.server, MQTT_PORT);
    show_conn_sett_server(sender->GetSerial());
    save_conn();
  }
  else if(strcmp(cmd, "mqtt_id") == 0){
    strcpy(conn_sett.mqtt_id, "");
    read_all(sender, conn_sett.mqtt_id);
    show_conn_sett_mqtt_id(sender->GetSerial());
    save_conn();
  }
  else {
    sender->GetSerial()->print("Error: Invalid setting name: ");
    sender->GetSerial()->println(cmd);
  }
}
SerialCommand cmd_set_("set", cmd_set);

void cmd_get(SerialCommands *sender){
  char *var = sender->Next();
  if(var == NULL){
    sender->GetSerial()->println("Error: no setting name");
    return;
  }
  char cmd[16] = "";
  strncpy(cmd, var, 16-1);
  if(strcmp(cmd, "ssid") == 0){
    show_conn_sett_ssid(sender->GetSerial());
    return;
  }else if(strcmp(cmd, "pass") == 0){
    show_conn_sett_pass(sender->GetSerial());
    return;
  }else if(strcmp(cmd, "server") == 0){
    show_conn_sett_server(sender->GetSerial());
    return;
  }else if(strcmp(cmd, "mqtt_id") == 0){
    show_conn_sett_mqtt_id(sender->GetSerial());
    return;
  }else if(strcmp(cmd, "connection") == 0){
    show_conn_sett_all(sender->GetSerial());
    return;
  }else if(strcmp(cms, "devices") == 0){
    sender->GetSerial()->println("No devices now...");
  }
  
  else {
    sender->GetSerial()->print("Error: Invalid setting name: ");
    sender->GetSerial()->println(cmd);
  }
}
SerialCommand cmd_get_("get", cmd_get);


void commands_setup() {
  
  Serial.begin(DEBUG_BAUD_RATE);

  serial_commands_.SetDefaultHandler(cmd_unrecognized);
  serial_commands_.AddCommand(&cmd_set_);
  serial_commands_.AddCommand(&cmd_get_);

}

void commands_loop() {
  serial_commands_.ReadSerial();
}
