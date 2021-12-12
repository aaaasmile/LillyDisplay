#include "LineDataUpdater.h"

#include <Arduino.h>
#include <SPIFFS.h>

#include "predef.h"

extern char g_Lines[][25];
const char* FN_LINE1 = "/line1.txt";

void writeFile(fs::FS& fs, const char* path, const char* message) {
  CONSOLEPF("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file) {
    CONSOLEPF("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    CONSOLEPF("- file written");
  } else {
    CONSOLEPF("- write failed");
  }
}

String readFile(fs::FS& fs, const char* path, bool& isfile_error) {
  CONSOLEPF("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    CONSOLEPF("- empty file or failed to open file: %s", path);
    isfile_error = true;
    return String();
  }
  isfile_error = false;
  CONSOLEPF("- read from file:");
  String fileContent;
  while (file.available()) {
    fileContent += String((char)file.read());
  }
  file.close();
  CONSOLEPF(fileContent.c_str());
  return fileContent;
}

LineDataUpdater::LineDataUpdater() {
  _line1 = "";
}

void LineDataUpdater::Setup() {
  if (!SPIFFS.begin(true)) {
    CONSOLEPF("An Error has occurred while mounting SPIFFS");
  }
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

void LineDataUpdater::Commit() {
  writeInMemory();
  writeFile(SPIFFS, FN_LINE1, g_Lines[0]);

  CONSOLEPF("Lines commited");
}

void LineDataUpdater::FetchLines() {
  bool isfile_error = false;
  _line1 = readFile(SPIFFS, FN_LINE1, isfile_error);
  if (isfile_error) {
    return;
  }

  writeInMemory();
  CONSOLEPF("Lines loaded from FS");
  logInternalState();
}

void LineDataUpdater::writeInMemory() {
  strncpy(g_Lines[0], _line1.c_str(), 13);
}

void LineDataUpdater::logInternalState() {
  CONSOLEPF("Log all lines");
  CONSOLEPF("Line 1: %s", _line1.c_str());
}