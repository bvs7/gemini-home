#ifndef COMMANDS_H_
#define COMMANDS_H_

#define DEBUG_BAUD_RATE 9600

#include "utility.h"

#include "PipedStream.h" // Buffered Streams library for virtual stream for commands

#define DEBUG_BAUD_RATE 9600

void commands_setup();
void commands_loop();

extern PipedStream& command_handle;

#endif
