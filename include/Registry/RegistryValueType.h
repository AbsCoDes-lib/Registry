//===--- RegistryValueType.h ---------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//


#ifndef REGISTRY_VALUE_KIND_INCLUDED
#define REGISTRY_VALUE_KIND_INCLUDED


namespace abscodes {
namespace registry {

  ///
  /// The RegistryValueType enumeration defines the set of supported registry data types.
  ///
  /// Use the RegistryKey.GetValueKind method to determine the data type of a registry key value before retrieving
  ///	the value. When you set a registry key value, use the SetValue method to specify the registry data type explicitly.
  ///
  /// https://docs.microsoft.com/en-us/windows/desktop/SysInfo/registry-value-types
  ///
  enum class RegistryValueType {
    None           = static_cast<int>(REG_NONE),
    DWord          = static_cast<int>(REG_DWORD),
    DWordBigEndian = static_cast<int>(REG_DWORD_BIG_ENDIAN),
    QWord          = static_cast<int>(REG_QWORD),
    String         = static_cast<int>(REG_SZ),
    ExpandString   = static_cast<int>(REG_EXPAND_SZ),
    MultiString    = static_cast<int>(REG_MULTI_SZ),
    Binary         = static_cast<int>(REG_BINARY),
    Link           = static_cast<int>(REG_LINK),
  };


  namespace ValueType {

    static int Handle(RegistryValueType valueType) {
      {
        switch(valueType) {
          case RegistryValueType::None: return REG_NONE;
          case RegistryValueType::DWord: return REG_DWORD;
          case RegistryValueType::DWordBigEndian: return REG_DWORD_BIG_ENDIAN;
          case RegistryValueType::QWord: return REG_QWORD;
          case RegistryValueType::String: return REG_SZ;
          case RegistryValueType::ExpandString: return REG_EXPAND_SZ;
          case RegistryValueType::MultiString: return REG_MULTI_SZ;
          case RegistryValueType::Binary: return REG_BINARY;
          case RegistryValueType::Link: return REG_LINK;
          default: throw std::invalid_argument("Unsupported registry value type.");
        }
      }
    }

    static RegistryValueType Handle(DWORD valueType) {
      {
        switch(valueType) {
          case REG_NONE: return RegistryValueType::None;
          case REG_DWORD: return RegistryValueType::DWord;
          case REG_DWORD_BIG_ENDIAN: return RegistryValueType::DWordBigEndian;
          case REG_QWORD: return RegistryValueType::QWord;
          case REG_SZ: return RegistryValueType::String;
          case REG_EXPAND_SZ: return RegistryValueType::ExpandString;
          case REG_MULTI_SZ: return RegistryValueType::MultiString;
          case REG_BINARY: return RegistryValueType::Binary;
          case REG_LINK: return RegistryValueType::Link;
          default: throw std::invalid_argument("Unsupported registry value type.");
        }
      }
    }

  } // namespace ValueType


} // namespace registry
} // namespace abscodes


#endif // REGISTRY_VALUE_KIND_INCLUDED
