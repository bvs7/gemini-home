#include "settings.h"

#define CATEGORY_ "gemini-home"
#define STATION_NAME_ "garden-1"

// clang-format off
byte p_sense[] = {UNUSED_PIN, 7, 9, UNUSED_PIN, UNUSED_PIN};
byte p_ctrl[] =  {UNUSED_PIN, 6, 8, UNUSED_PIN, UNUSED_PIN};

byte a_input[] = {UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN};
byte a_trig[] =  {UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN};
// clang-format on

#define POLL_INTERVAL_ 300
