//===--- RegistryValue.cpp -----------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//

#include "Registry/RegistryValue.h"

#include "Registry/RegistryException.h"

namespace abscodes {
namespace registry {

    RegistryValue::RegistryValue(RegistryValueType type)
      : _type(type) {}

    RegistryValueType RegistryValue::GetType() const {
        return _type;
    }

    void RegistryValue::Reset(RegistryValueType type) {
        ResetValues();
        _type = type;
    }

    bool RegistryValue::IsEmpty() const {
        return _type == RegistryValueType::None;
    }

    void RegistryValue::ResetValues() {
        _dword = 0;
        _qword = 0;
        _string.clear();
        _expandString.clear();
        _multiString.clear();
        _binary.clear();
    }

    DWORD RegistryValue::DWord() const {

        _ASSERTE(_type == RegistryValueType::DWord);
        if(_type != RegistryValueType::DWord) {
            throw Exceptions::RegistryException("RegistryValue::DWord() called on a non-DWORD registry value.");
        }

        return _dword;
    }

    ULONGLONG RegistryValue::QWord() const {

        _ASSERTE(_type == RegistryValueType::QWord);
        if(_type != RegistryValueType::QWord) {
            throw Exceptions::RegistryException("RegistryValue::QWord() called on a non-QWORD registry value.");
        }

        return _qword;
    }

    const std::string& RegistryValue::String() const {

        _ASSERTE(_type == RegistryValueType::String);
        if(_type != RegistryValueType::String) {
            throw Exceptions::RegistryException("RegistryValue::String() called on a non-REG_SZ registry value.");
        }

        return _string;
    }

    const std::string& RegistryValue::ExpandString() const {

        _ASSERTE(_type == RegistryValueType::ExpandString);
        if(_type != RegistryValueType::ExpandString) {
            throw Exceptions::RegistryException("RegistryValue::ExpandString() called on a non-REG_EXPAND_SZ registry value.");
        }

        return _expandString;
    }

    const std::vector<std::string>& RegistryValue::MultiString() const {

        _ASSERTE(_type == RegistryValueType::MultiString);
        if(_type != RegistryValueType::MultiString) {
            throw Exceptions::RegistryException("RegistryValue::MultiString() called on a non-REG_MULTI_SZ  registry value.");
        }

        return _multiString;
    }

    const std::vector<BYTE>& RegistryValue::Binary() const {

        _ASSERTE(_type == RegistryValueType::Binary);
        if(_type != RegistryValueType::Binary) {
            throw Exceptions::RegistryException("RegistryValue::Binary() called on a non-REG_BINARY   registry value.");
        }

        return _binary;
    }

    DWORD& RegistryValue::DWord() {

        _ASSERTE(_type == RegistryValueType::DWord);
        if(_type != RegistryValueType::DWord) {
            throw Exceptions::RegistryException("RegistryValue::DWord() called on a non-DWORD registry value.");
        }

        return _dword;
    }

    ULONGLONG& RegistryValue::QWord() {

        _ASSERTE(_type == RegistryValueType::QWord);
        if(_type != RegistryValueType::QWord) {
            throw Exceptions::RegistryException("RegistryValue::QWord() called on a non-QWORD registry value.");
        }

        return _qword;
    }

    std::string& RegistryValue::String() {

        _ASSERTE(_type == RegistryValueType::String);
        if(_type != RegistryValueType::String) {
            throw Exceptions::RegistryException("RegistryValue::String() called on a non-REG_SZ registry value.");
        }

        return _string;
    }

    std::string& RegistryValue::ExpandString() {

        _ASSERTE(_type == RegistryValueType::ExpandString);
        if(_type != RegistryValueType::ExpandString) {
            throw Exceptions::RegistryException("RegistryValue::ExpandString() called on a non-REG_EXPAND_SZ registry value.");
        }

        return _expandString;
    }

    std::vector<std::string>& RegistryValue::MultiString() {

        _ASSERTE(_type == RegistryValueType::MultiString);
        if(_type != RegistryValueType::MultiString) {
            throw Exceptions::RegistryException("RegistryValue::MultiString() called on a non-REG_MULTI_SZ  registry value.");
        }

        return _multiString;
    }

    std::vector<BYTE>& RegistryValue::Binary() {

        _ASSERTE(_type == RegistryValueType::Binary);
        if(_type != RegistryValueType::Binary) {
            throw Exceptions::RegistryException("RegistryValue::Binary() called on a non-REG_BINARY   registry value.");
        }

        return _binary;
    }

} // namespace registry
} // namespace abscodes
