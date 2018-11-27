//===--- Registry.h ------------------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//


#ifndef Registry_INCLUDED
#define Registry_INCLUDED


#include "Registry/RegistryApi.h"

#include "Registry/RegistryKey.h"


namespace AbsCoDes {
namespace Registry {


  /// This field reads the Windows registry base key HKEY_CLASSES_ROOT.
  inline RegistryKey ClassesRoot() {
    return RegistryKey(RegistryHive::ClassesRoot);
  }

  /// This field reads the Windows registry base key HKEY_CURRENT_USER.
  inline RegistryKey CurrentUser() {
    return RegistryKey(RegistryHive::CurrentUser);
  }

  /// This field reads the Windows registry base key HKEY_LOCAL_MACHINE.
  inline RegistryKey LocalMachine() {
    return RegistryKey(RegistryHive::LocalMachine);
  }

  /// This field reads the Windows registry base key HKEY_USERS.
  inline RegistryKey Users() {
    return RegistryKey(RegistryHive::Users);
  }

  /// This field reads the Windows registry base key HKEY_PERFORMANCE_DATA.
  inline RegistryKey PerformanceData() {
    return RegistryKey(RegistryHive::PerformanceData);
  }

  /// This field reads the Windows registry base key HKEY_CURRENT_CONFIG.
  inline RegistryKey CurrentConfig() {
    return RegistryKey(RegistryHive::CurrentConfig);
  }

  /// This field reads the Windows registry base key HKEY_DYN_DATA.
  inline RegistryKey DynData() {
    return RegistryKey(RegistryHive::DynData);
  }


  /// Retrieves the key associated with the specified name, in the specified registry hive.
  /// If the key is not found in the specified hive, a RegistryException is thrown.
  Registry_API RegistryKey GetKey(const RegistryHive hive, const std::string& keyName);

  /// Retrieves the key associated with the specified name, in the specified registry key.
  /// If the key is not found in the specified key, a RegistryException is thrown.
  Registry_API RegistryKey GetKey(RegistryKey& key, const std::string& keyName);


  /// Retrieves the value associated with the specified name, in the specified registry hive.
  /// If the value is not found in the specified hive, a RegistryException is throw.
  Registry_API RegistryValue GetValue(const RegistryHive hive, const std::string& keyName, const std::string& valueName);

  /// Retrieves the value associated with the specified name, in the specified registry key.
  /// If the value is not found in the specified key, a RegistryException is throw.
  Registry_API RegistryValue GetValue(RegistryKey& key, const std::string& keyName, const std::string& valueName);

  /// Retrieves the value associated with the specified name, in the specified registry key.
  /// If the value is not found in the specified key, a RegistryException is throw.
  Registry_API RegistryValue GetValue(RegistryKey& key, const std::string& valueName);


  Registry_API DWORD GetDWord(RegistryKey& key, const std::string& valueName, DWORD defaultValue);
  Registry_API ULONGLONG GetQWord(RegistryKey& key, const std::string& valueName, ULONGLONG defaultValue);
  Registry_API const std::string GetString(RegistryKey& key, const std::string& valueName, const std::string& defaultValue);
  Registry_API const std::string GetExpandString(RegistryKey& key, const std::string& valueName, const std::string& defaultValue);
  Registry_API const std::vector<std::string> GetMultiString(RegistryKey& key, const std::string& valueName, const std::vector<std::string>& defaultValue);
  Registry_API const std::vector<BYTE> GetBinary(RegistryKey& key, const std::string& valueName, const std::vector<BYTE>& defaultValue);


  Registry_API int           GetInt(RegistryKey& key, const std::string& valueName, int defaultValue);
  Registry_API unsigned int  GetUInt(RegistryKey& key, const std::string& valueName, unsigned int defaultValue);
  Registry_API long          GetLong(RegistryKey& key, const std::string& valueName, long defaultValue);
  Registry_API unsigned long GetULong(RegistryKey& key, const std::string& valueName, unsigned long defaultValue);
  Registry_API double        GetDouble(RegistryKey& key, const std::string& valueName, double defaultValue);


  /// Sets the specified name/value pair on the specified registry key.
  /// If the specified key does not exist, it is created.
  Registry_API void SetValue(RegistryHive hive, const std::string& keyName, const std::string& valueName, const RegistryValue& value);

  /// Sets the specified name/value pair on the specified registry key.
  /// If the specified key does not exist, it is created.
  Registry_API void SetValue(RegistryKey& key, const std::string& keyName, const std::string& valueName, const RegistryValue& value);

  /// Sets the specified name/value pair on the specified registry key.
  /// If the specified key does not exist, it is created.
  Registry_API void SetValue(RegistryKey& key, const std::string& valueName, const RegistryValue& value);
  
  Registry_API void SetDWord(RegistryKey& key, const std::string& valueName, DWORD value);
  Registry_API void SetQWord(RegistryKey& key, const std::string& valueName, ULONGLONG value);
  Registry_API void SetString(RegistryKey& key, const std::string& valueName, const std::string& value);
  Registry_API void SetExpandString(RegistryKey& key, const std::string& valueName, const std::string& value);
  Registry_API void SetMultiString(RegistryKey& key, const std::string& valueName, const std::vector<std::string>& value);
  Registry_API void SetBinary(RegistryKey& key, const std::string& valueName, const std::vector<BYTE>& value);

  Registry_API void SetInt(RegistryKey& key, const std::string& valueName, int value);
  Registry_API void SetUInt(RegistryKey& key, const std::string& valueName, unsigned int value);
  Registry_API void SetLong(RegistryKey& key, const std::string& valueName, long value);
  Registry_API void SetULong(RegistryKey& key, const std::string& valueName, unsigned long value);
  Registry_API void SetDouble(RegistryKey& key, const std::string& valueName, double value);


} // namespace Registry
} // namespace AbsCoDes


#endif // Registry_INCLUDED
