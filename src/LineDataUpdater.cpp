#include "LineDataUpdater.h"
#include <Arduino.h>
#include "predef.h"

extern char g_Lines[][25];

LineDataUpdater::LineDataUpdater(){
  _line1 = "";
}

void LineDataUpdater::AddLine(const char* name, const char* value) {
  String nn = String(name);
  if (nn == "line1") {
    _line1 = String(value);
    return;
  }
}

bool LineDataUpdater::IsChanged() {
  bool is_changed = (String(g_Lines[0]) != _line1) && (_line1 != "");
  logInternalState();
  return is_changed;
}

void LineDataUpdater::Commit(){
  strncpy(g_Lines[0], _line1.c_str(), 13);
  CONSOLEPF("Lines changed");
}

void LineDataUpdater::logInternalState(){
  CONSOLEPF("Log all lines");
  CONSOLEPF("Line 1: %s",_line1.c_str());
}