//===--- RegistryValue.h -------------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//


#ifndef RegistryValue_INCLUDED
#define RegistryValue_INCLUDED

#include "Registry/RegistryApi.h"

#include <string>
#include <vector>

#include "Registry/RegistryValueType.h"

#pragma warning(push)
#pragma warning(disable : 4251)

namespace AbsCoDes {
namespace Registry {

  ///
  /// "Variant-style" Registry value.
  ///
  class Registry_API RegistryValue {

   public:
    ///
    /// Initialize an empty value (REG_NONE).
    ///
    RegistryValue() = default;

    /// Initialize with the given value kind.
    /// Caller can use accessor corresponding to the given type (e.g. String() for REG_SZ)
    /// to set the desired value.
    explicit RegistryValue(RegistryValueType type);

    /// Registry value type (e.g. REG_SZ) associated to current value.
    RegistryValueType GetType() const;

    /// Reset current "variant-style" value to the specified type.
    /// Note that previous values are cleared.
    /// Caller can use accessor corresponding to the given type (e.g. String() for REG_SZ)
    /// to set the desired value.
    void Reset(RegistryValueType type = RegistryValueType::None);

    /// Is it REG_NONE?
    /// Note: "empty" (i.e. REG_NONE) can be used to indicate error conditions as well
    bool IsEmpty() const;


    //
    // Getters
    //

    DWORD                           DWord() const;
    ULONGLONG                       QWord() const;
    const std::string&              String() const;
    const std::string&              ExpandString() const;
    const std::vector<std::string>& MultiString() const;
    const std::vector<BYTE>&        Binary() const;


    //
    // Setters
    //

    DWORD&                    DWord();
    ULONGLONG&                QWord();
    std::string&              String();
    std::string&              ExpandString();
    std::vector<std::string>& MultiString();
    std::vector<BYTE>&        Binary();

   private:
    /// Registry value type
    RegistryValueType _type = RegistryValueType::None;

    /// Store REG_DWORD value
    DWORD _dword = 0;
    /// Store REG_QWORD value
    ULONGLONG _qword = 0;
    /// Store REG_SZ value
    std::string _string;
    /// Store REG_EXPAND_SZ value
    std::string _expandString;
    /// Store REG_MULTI_SZ value
    std::vector<std::string> _multiString;
    /// Store REG_BINARY value
    std::vector<BYTE> _binary;

    /// Clear all the data members
    void ResetValues();
  };


} // namespace Registry
} // namespace AbsCoDes

#pragma warning(pop)

#endif // RegistryValue_INCLUDED
