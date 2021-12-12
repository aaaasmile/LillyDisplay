#ifndef __PREDEF___H
#define __PREDEF___H

#define DEBUG

#ifndef CONSOLEPF
inline void CONSOLEPF(const char* fmt, ...) {
#ifdef DEBUG
  char myBuff[512];
  va_list args;

  va_start(args, fmt); /* Initialize variable arguments. */

  vsprintf(myBuff, fmt, args);
  Serial.println(myBuff);
#endif
}
#endif

#ifndef CONSOLEWRITE
inline void CONSOLEWRITE(uint8_t d){
#ifdef DEBUG
  Serial.write(d);
#endif
}
#endif

#endif