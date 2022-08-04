
#include "devices.h"

char state_string[4][10] = {
  "OFF", "UNPOWERED", "OVERRIDE", "ON"
};

device::device(byte sense_pin, byte ctrl_pin, const char* device_name){
  s = device_settings_t{
    .device_name = NULL,
    .sense_pin = sense_pin,
    .ctrl_pin = ctrl_pin,
    .device_state = OFF_
  };
  
  ctrl = LOW;
  strcpy(this->device_name, device_name);
}

char* device::state_str(){
  return state_string[s.device_state];
}

void device::setup(){
#ifdef DEBUG_
  pinMode(s.sense_pin, INPUT_PULLUP);
#else
  pinMode(s.sense_pin, INPUT);
#endif
  pinMode(s.ctrl_pin, OUTPUT);
  ctrl = s.device_state & CTRL_MASK;
  digitalWrite(s.ctrl_pin, ctrl);

  // Subscribe to the device's command topic
  mqtt_client.subscribe(strcat(CMD_STATION_TOPIC, device_name));
}

/**
 * @brief Loop function for device
 *     Called once every few seconds. First, checks if the control setting
 *     has changed. If so, it will update the control pin. If the control
 *     setting is the same, it will check to see if the last state is invalid.
 *     If so, it will send updates via serial and mqtt
 */
void device::loop(){
  // Check if control output has changed
  if(ctrl != digitalRead(s.ctrl_pin)){
    // control output has changed, update it
    digitalWrite(s.ctrl_pin, ctrl);
    return;
  }
  // Check if state has changed
  state state_ = static_cast<state>(
    digitalRead(s.sense_pin) << 1 | digitalRead(s.ctrl_pin));
  if(state_ != s.device_state){
    // print output to command_internal
    Serial.print(s.device_name);
    Serial.print(" ");
    Serial.print(state_string[state_]);
    Serial.flush();
    // publish output
    mqtt_client.publish(
      strcat(DATA_STATION_TOPIC, device_name), 
      state_string[state_]
    );

    s.device_state = state_;
  }
}

void device::set_ctrl(byte ctrl_value){
  LOG("set ctrl ");
  LOG(device_name);
  LOG(" to ");
  LOG(ctrl_value);
  LOG("\n");
  ctrl = ctrl_value != 0; // Flatten to 1 or 0
}

bool device::check_name(const char* name, unsigned int length){
  return strncmp(name, device_name, length) == 0;
}
