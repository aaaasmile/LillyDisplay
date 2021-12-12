#ifndef _LINE_H_UPDATER
#define _LINE_H_UPDATER

#include "WString.h"

class LineDataUpdater {
 public:
  LineDataUpdater();
  void AddLine(const char* name, const char* value);
  bool IsChanged();
  void Commit();

 private:
  void logInternalState();
  String _line1;
};

#endif
