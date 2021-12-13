#ifndef _LINE_H_UPDATER
#define _LINE_H_UPDATER

#include "WString.h"

class LineDataUpdater {
 public:
  LineDataUpdater();
  void Setup();
  void AddLine(const char* name, const char* value);
  bool IsChanged();
  void Commit();
  void FetchLines();

 private:
  void logInternalState();
  void writeInMemory();
  String _line1;
  String _line2;
  String _line3;
  String _line4;
  String _line5;
};

#endif
