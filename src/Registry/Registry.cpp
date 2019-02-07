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

    void DeleteSubKey(RegistryKey& key, std::string subkey, RegistryAccessRights desiredAccess) noexcept {
        try {
            key.DeleteSubKey(subkey, desiredAccess);
        }
        catch(...) {
        }
    }

    void DeleteSubKey(RegistryKey& key, std::string subkey, RegistryView view, RegistryAccessRights desiredAccess) noexcept {
        try {
            key.DeleteSubKey(subkey, view, desiredAccess);
        }
        catch(...) {
        }
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

    DWORD GetDWord(RegistryKey& key, const std::string& valueName, DWORD defaultValue) noexcept {
        try {
            return key.GetDwordValue(valueName);
        }
        catch(...) {
        }
        return defaultValue;
    }

    ULONGLONG GetQWord(RegistryKey& key, const std::string& valueName, ULONGLONG defaultValue) noexcept {
        try {
            return key.GetQwordValue(valueName);
        }
        catch(...) {
        }
        return defaultValue;
    }

    const std::string GetString(RegistryKey& key, const std::string& valueName, const std::string& defaultValue) noexcept {
        try {
            return key.GetStringValue(valueName);
        }
        catch(...) {
        }
        return defaultValue;
    }

    const std::string GetExpandString(RegistryKey& key, const std::string& valueName, const std::string& defaultValue) noexcept {
        try {
            return key.GetExpandStringValue(valueName);
        }
        catch(...) {
        }
        return defaultValue;
    }

    const std::vector<std::string> GetMultiString(RegistryKey& key, const std::string& valueName, const std::vector<std::string>& defaultValue) noexcept {
        try {
            return key.GetMultiStringValue(valueName);
        }
        catch(...) {
        }
        return defaultValue;
    }

    const std::vector<BYTE> GetBinary(RegistryKey& key, const std::string& valueName, const std::vector<BYTE>& defaultValue) noexcept {
        try {
            return key.GetBinaryValue(valueName);
        }
        catch(...) {
        }
        return defaultValue;
    }

    int GetInt(RegistryKey& key, const std::string& valueName, int defaultValue) noexcept {
        try {
            return commons::number::toNumber<int>(key.GetStringValue(valueName));
        }
        catch(...) {
        }
        return defaultValue;
    }

    unsigned int GetUInt(RegistryKey& key, const std::string& valueName, unsigned int defaultValue) noexcept {
        try {
            return commons::number::toNumber<unsigned int>(key.GetStringValue(valueName));
        }
        catch(...) {
        }
        return defaultValue;
    }

    long GetLong(RegistryKey& key, const std::string& valueName, long defaultValue) noexcept {
        try {
            return commons::number::toNumber<long>(key.GetStringValue(valueName));
        }
        catch(...) {
        }
        return defaultValue;
    }

    unsigned long GetULong(RegistryKey& key, const std::string& valueName, unsigned long defaultValue) noexcept {
        try {
            return commons::number::toNumber<unsigned long>(key.GetStringValue(valueName));
        }
        catch(...) {
        }
        return defaultValue;
    }

    double GetDouble(RegistryKey& key, const std::string& valueName, double defaultValue) noexcept {
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

    void SetValue(RegistryKey& key, const std::string& valueName, const RegistryValue& value) noexcept {
        try {
            return key.SetValue(valueName, value);
        }
        catch(...) {
        }
    }

    void SetDWord(RegistryKey& key, const std::string& valueName, DWORD value) noexcept {
        try {
            key.SetDwordValue(valueName, value);
        }
        catch(...) {
        }
    }

    void SetQWord(RegistryKey& key, const std::string& valueName, ULONGLONG value) noexcept {
        try {
            key.SetQwordValue(valueName, value);
        }
        catch(...) {
        }
    }

    void SetString(RegistryKey& key, const std::string& valueName, const std::string& value) noexcept {
        try {
            key.SetStringValue(valueName, value);
        }
        catch(...) {
        }
    }

    void SetExpandString(RegistryKey& key, const std::string& valueName, const std::string& value) noexcept {
        try {
            key.SetExpandStringValue(valueName, value);
        }
        catch(...) {
        }
    }

    void SetMultiString(RegistryKey& key, const std::string& valueName, const std::vector<std::string>& value) noexcept {
        try {
            key.SetMultiStringValue(valueName, value);
        }
        catch(...) {
        }
    }

    void SetBinary(RegistryKey& key, const std::string& valueName, const std::vector<BYTE>& value) noexcept {
        try {
            key.SetBinaryValue(valueName, value);
        }
        catch(...) {
        }
    }

    void SetInt(RegistryKey& key, const std::string& valueName, int value) noexcept {
        try {
            key.SetStringValue(valueName, commons::number::toString(value));
        }
        catch(...) {
        }
    }

    void SetUInt(RegistryKey& key, const std::string& valueName, unsigned int value) noexcept {
        try {
            key.SetStringValue(valueName, commons::number::toString(value));
        }
        catch(...) {
        }
    }

    void SetLong(RegistryKey& key, const std::string& valueName, long value) noexcept {
        try {
            key.SetStringValue(valueName, commons::number::toString(value));
        }
        catch(...) {
        }
    }

    void SetULong(RegistryKey& key, const std::string& valueName, unsigned long value) noexcept {
        try {
            key.SetStringValue(valueName, commons::number::toString(value));
        }
        catch(...) {
        }
    }

    void SetDouble(RegistryKey& key, const std::string& valueName, double value) noexcept {
        try {
            key.SetStringValue(valueName, commons::number::toString(value));
        }
        catch(...) {
        }
    }

    REGISTRY_API void DeleteValue(RegistryKey& key, std::string valueName) noexcept {
        try {
            key.DeleteValue(valueName);
        }
        catch(...) {
        }
    }


} // namespace registry
} // namespace abscodes
