#include "LineDataUpdater.h"

#include <Arduino.h>
#include <SPIFFS.h>

#include "predef.h"

extern char g_Lines[][25];
const char* FN_LINE1 = "/line1.txt";
const char* FN_LINE2 = "/line2.txt";
const char* FN_LINE3 = "/line3.txt";
const char* FN_LINE4 = "/line4.txt";
const char* FN_LINE5 = "/line5.txt";

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
  _line2 = "";
  _line3 = "";
  _line4 = "";
  _line5 = "";
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
  if (nn == "line2") {
    _line2 = String(value);
    return;
  }
  if (nn == "line3") {
    _line3 = String(value);
    return;
  }
  if (nn == "line4") {
    _line4 = String(value);
    return;
  }
  if (nn == "line5") {
    _line5 = String(value);
    return;
  }
}

bool LineDataUpdater::IsChanged() {
  bool is_changed = (String(g_Lines[0]) != _line1) && (_line1 != "");
  is_changed = is_changed || ((String(g_Lines[1]) != _line2) && (_line2 != ""));
  is_changed = is_changed || ((String(g_Lines[2]) != _line3) && (_line3 != ""));
  is_changed = is_changed || ((String(g_Lines[3]) != _line4) && (_line4 != ""));
  is_changed = is_changed || ((String(g_Lines[4]) != _line5) && (_line5 != ""));
  logInternalState();
  return is_changed;
}

void LineDataUpdater::Commit() {
  writeInMemory();
  writeFile(SPIFFS, FN_LINE1, g_Lines[0]);
  writeFile(SPIFFS, FN_LINE2, g_Lines[1]);
  writeFile(SPIFFS, FN_LINE3, g_Lines[2]);
  writeFile(SPIFFS, FN_LINE4, g_Lines[3]);
  writeFile(SPIFFS, FN_LINE5, g_Lines[4]);

  CONSOLEPF("Lines commited");
}

void LineDataUpdater::FetchLines() {
  bool isfile_error = false;
  _line1 = readFile(SPIFFS, FN_LINE1, isfile_error);
  if (isfile_error) {
    return;
  }
  _line2 = readFile(SPIFFS, FN_LINE2, isfile_error);
  if (isfile_error) {
    return;
  }
  _line3 = readFile(SPIFFS, FN_LINE3, isfile_error);
  if (isfile_error) {
    return;
  }
  _line4 = readFile(SPIFFS, FN_LINE4, isfile_error);
  if (isfile_error) {
    return;
  }
  _line5 = readFile(SPIFFS, FN_LINE5, isfile_error);
  if (isfile_error) {
    return;
  }

  writeInMemory();
  CONSOLEPF("Lines loaded from FS");
  logInternalState();
}

void LineDataUpdater::writeInMemory() {
  strncpy(g_Lines[0], _line1.c_str(), 13);
  strncpy(g_Lines[1], _line2.c_str(), 13);
  strncpy(g_Lines[2], _line3.c_str(), 13);
  strncpy(g_Lines[3], _line4.c_str(), 13);
  strncpy(g_Lines[4], _line5.c_str(), 13);
}

void LineDataUpdater::logInternalState() {
  CONSOLEPF("Log all lines");
  CONSOLEPF("Line 1: %s", _line1.c_str());
  CONSOLEPF("Line 2: %s", _line2.c_str());
  CONSOLEPF("Line 3: %s", _line3.c_str());
  CONSOLEPF("Line 4: %s", _line4.c_str());
  CONSOLEPF("Line 5: %s", _line5.c_str());
}