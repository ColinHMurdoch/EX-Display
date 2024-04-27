#include "EXDisplayClass.h"

/// @brief Define the first EXDisplay object as a nullptr
EXDisplay *EXDisplay::_first = nullptr;

/*
 * EXDisplayRow class implementation
 */
EXDisplayRow::EXDisplayRow(uint8_t rowNumber, char *rowText, bool ticker, bool changed)
    : _rowNumber(rowNumber), _rowText(rowText), _ticker(ticker), _changed(changed) {}

uint8_t EXDisplayRow::getRowNumber() { return _rowNumber; }

void EXDisplayRow::setRowText(char *rowText) {
  _rowText = rowText;
  _changed = true;
}

char *EXDisplayRow::getRowText() {
  _changed = false;
  return _rowText;
}

void EXDisplayRow::setTicker(bool ticker) { _ticker = ticker; }

bool EXDisplayRow::isTicker() { return _ticker; }

bool EXDisplayRow::isChanged() { return _changed; }

void EXDisplayRow::setNext(EXDisplayRow *next) { _next = next; }

EXDisplayRow *EXDisplayRow::getNext() { return _next; }

/*
 * EXDisplay class implementation
 */
EXDisplay::EXDisplay(uint8_t displayNumber, uint8_t maxRows, uint8_t maxColumns, uint16_t maxRowWidth, bool autoTicker,
                     EXDisplay *copyOf) {
  EXDisplay *existingDisplay = getDisplayByNumber(displayNumber);
  if (existingDisplay) {
    existingDisplay->_maxRows = maxRows;
    existingDisplay->_maxColumns = maxColumns;
    existingDisplay->_maxRowWidth = maxRowWidth;
    existingDisplay->_autoTicker = autoTicker;
    existingDisplay->_copyOf = copyOf;
    _next = existingDisplay->_next;
    _firstRow = existingDisplay->_firstRow;
    return;
  }
  _displayNumber = displayNumber;
  _maxRows = maxRows;
  _maxColumns = maxColumns;
  _maxRowWidth = maxRowWidth;
  _autoTicker = autoTicker;
  _copyOf = copyOf;
  _firstRow = nullptr;
  _next = _first;
  _first = this;

  if (_next == nullptr) {
    _first = this;
  } else {
    EXDisplay *current = _first;
    while (current->_next != nullptr) {
      current = current->_next;
    }
    current->_next = this;
  }
}

uint8_t EXDisplay::getDisplayNumber() {
  return _displayNumber;
}

uint8_t EXDisplay::getMaxRows() {
  return _maxRows;
}

uint8_t EXDisplay::getMaxColumns() {
  return _maxColumns;
}

uint16_t EXDisplay::getMaxRowWidth() {
  return _maxRowWidth;
}

bool EXDisplay::isAutoTicker() {
  return _autoTicker;
}

EXDisplay *EXDisplay::getCopyOf() {
  return _copyOf;
}

EXDisplayRow *EXDisplay::getRowByNumber(uint8_t rowNumber) {
  for (EXDisplayRow *row = _firstRow; row; row = row->getNext()) {
    if (row->getRowNumber() == rowNumber) {
      return row;
    }
  }
  return nullptr;
}

void EXDisplay::updateRow(uint8_t rowNumber, char *rowText, bool ticker) {
  EXDisplayRow *existingRow = getRowByNumber(rowNumber);
  if (existingRow != nullptr) {
    existingRow->setRowText(rowText);
    existingRow->setTicker(ticker);
    return;
  }

  EXDisplayRow *newRow = new EXDisplayRow(rowNumber, rowText, ticker);
  if (_firstRow == nullptr) {
    _firstRow = newRow;
  } else {
    EXDisplayRow *currentRow = _firstRow;
    while (currentRow->getNext() != nullptr) {
      currentRow = currentRow->getNext();
    }
    currentRow->setNext(newRow);
  }
}

void EXDisplay::deleteRowNumber(uint8_t rowNumber) {
  EXDisplayRow *currentRow = _firstRow;
  while (currentRow != nullptr && currentRow->getNext() != nullptr) {
    if (currentRow->getNext()->getRowNumber() == rowNumber) {
      EXDisplayRow *temp = currentRow->getNext();
      currentRow->setNext(temp->getNext());
      delete temp;
      return;
    }
    currentRow = currentRow->getNext();
  }
}

bool EXDisplay::displayNumberExists(uint8_t displayNumber) {
  for (EXDisplay *display = _first; display; display = display->_next) {
    if (display->_displayNumber == displayNumber) {
      return true;
    }
  }
  return false;
}

EXDisplay *EXDisplay::getDisplayByNumber(uint8_t displayNumber) {
  for (EXDisplay *display = _first; display; display = display->_next) {
    if (display->_displayNumber == displayNumber) {
      return display;
    }
  }
  return nullptr;
}
