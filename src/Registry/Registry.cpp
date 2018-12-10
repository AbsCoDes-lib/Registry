//===--- Registry.cpp ----------------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//

#include "Registry/Registry.h"

#include "Commons/NumberUtils.h"

namespace abscodes {
namespace registry {


  /// Retrieves the key associated with the specified name, in the specified registry hive.
  /// If the key is not found in the specified hive, a RegistryException is throw.
  RegistryKey GetKey(const RegistryHive hive, const std::string& keyName) {
    return RegistryKey(hive).OpenSubKey(keyName);
  }

  RegistryKey GetKey(RegistryKey& key, const std::string& keyName) {
    return key.OpenSubKey(keyName);
  }

  /// Retrieves the value associated with the specified name, in the specified registry key.
  /// If the value is not found in the specified key, a RegistryException is throw.
  RegistryValue GetValue(const RegistryHive hive, const std::string& keyName, const std::string& valueName) {
    return RegistryKey(hive).OpenSubKey(keyName).GetValue(valueName);
  }

  RegistryValue GetValue(RegistryKey& key, const std::string& keyName, const std::string& valueName) {
    return key.OpenSubKey(keyName).GetValue(valueName);
  }

  RegistryValue GetValue(RegistryKey& key, const std::string& valueName) {
    return key.GetValue(valueName);
  }

  DWORD GetDWord(RegistryKey& key, const std::string& valueName, DWORD defaultValue) {
    try {
      return key.GetDwordValue(valueName);
    }
    catch(...) {
    }
    return defaultValue;
  }

  ULONGLONG GetQWord(RegistryKey& key, const std::string& valueName, ULONGLONG defaultValue) {
    try {
      return key.GetQwordValue(valueName);
    }
    catch(...) {
    }
    return defaultValue;
  }

  const std::string GetString(RegistryKey& key, const std::string& valueName, const std::string& defaultValue) {
    try {
      return key.GetStringValue(valueName);
    }
    catch(...) {
    }
    return defaultValue;
  }

  const std::string GetExpandString(RegistryKey& key, const std::string& valueName, const std::string& defaultValue) {
    try {
      return key.GetExpandStringValue(valueName);
    }
    catch(...) {
    }
    return defaultValue;
  }

  const std::vector<std::string> GetMultiString(RegistryKey& key, const std::string& valueName, const std::vector<std::string>& defaultValue) {
    try {
      return key.GetMultiStringValue(valueName);
    }
    catch(...) {
    }
    return defaultValue;
  }

  const std::vector<BYTE> GetBinary(RegistryKey& key, const std::string& valueName, const std::vector<BYTE>& defaultValue) {
    try {
      return key.GetBinaryValue(valueName);
    }
    catch(...) {
    }
    return defaultValue;
  }

  int GetInt(RegistryKey& key, const std::string& valueName, int defaultValue) {
    try {
      return commons::number::toNumber<int>(key.GetStringValue(valueName));
    }
    catch(...) {
    }
    return defaultValue;
  }

  unsigned int GetUInt(RegistryKey& key, const std::string& valueName, unsigned int defaultValue) {
    try {
      return commons::number::toNumber<unsigned int>(key.GetStringValue(valueName));
    }
    catch(...) {
    }
    return defaultValue;
  }

  long GetLong(RegistryKey& key, const std::string& valueName, long defaultValue) {
    try {
      return commons::number::toNumber<long>(key.GetStringValue(valueName));
    }
    catch(...) {
    }
    return defaultValue;
  }

  unsigned long GetULong(RegistryKey& key, const std::string& valueName, unsigned long defaultValue) {
    try {
      return commons::number::toNumber<unsigned long>(key.GetStringValue(valueName));
    }
    catch(...) {
    }
    return defaultValue;
  }

  double GetDouble(RegistryKey& key, const std::string& valueName, double defaultValue) {
    try {
      return commons::number::toNumber<double>(key.GetStringValue(valueName));
    }
    catch(...) {
    }
    return defaultValue;
  }

  /// Sets the specified name/value pair on the specified registry key.
  /// If the specified key does not exist, it is created.
  void SetValue(RegistryHive hive, const std::string& keyName, const std::string& valueName, const RegistryValue& value) {
    return RegistryKey(hive).OpenSubKey(keyName).SetValue(valueName, value);
  }

  void SetValue(RegistryKey& key, const std::string& keyName, const std::string& valueName, const RegistryValue& value) {
    return key.OpenSubKey(keyName).SetValue(valueName, value);
  }

  void SetValue(RegistryKey& key, const std::string& valueName, const RegistryValue& value) {
    return key.SetValue(valueName, value);
  }

  void SetDWord(RegistryKey& key, const std::string& valueName, DWORD value) {
    key.SetDwordValue(valueName, value);
  }

  void SetQWord(RegistryKey& key, const std::string& valueName, ULONGLONG value) {
    key.SetQwordValue(valueName, value);
  }

  void SetString(RegistryKey& key, const std::string& valueName, const std::string& value) {
    key.SetStringValue(valueName, value);
  }

  void SetExpandString(RegistryKey& key, const std::string& valueName, const std::string& value) {
    key.SetExpandStringValue(valueName, value);
  }

  void SetMultiString(RegistryKey& key, const std::string& valueName, const std::vector<std::string>& value) {
    key.SetMultiStringValue(valueName, value);
  }

  void SetBinary(RegistryKey& key, const std::string& valueName, const std::vector<BYTE>& value) {
    key.SetBinaryValue(valueName, value);
  }

  void SetInt(RegistryKey& key, const std::string& valueName, int value) {
    try {
      key.SetStringValue(valueName, commons::number::toString(value));
    }
    catch(...) {
    }
  }

  void SetUInt(RegistryKey& key, const std::string& valueName, unsigned int value) {
    try {
      key.SetStringValue(valueName, commons::number::toString(value));
    }
    catch(...) {
    }
  }

  void SetLong(RegistryKey& key, const std::string& valueName, long value) {
    try {
      key.SetStringValue(valueName, commons::number::toString(value));
    }
    catch(...) {
    }
  }

  void SetULong(RegistryKey& key, const std::string& valueName, unsigned long value) {
    try {
      key.SetStringValue(valueName, commons::number::toString(value));
    }
    catch(...) {
    }
  }

  void SetDouble(RegistryKey& key, const std::string& valueName, double value) {
    try {
      key.SetStringValue(valueName, commons::number::toString(value));
    }
    catch(...) {
    }
  }


} // namespace registry
} // namespace abscodes
