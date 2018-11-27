//===--- RegistryException.cpp -------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//


#include "Registry/RegistryException.h"


namespace AbsCoDes {
namespace Registry {
  namespace Exceptions {


    RegistryException::RegistryException(const std::string& keyName, const std::string& message)
      : ErrorCodeException(message, 0L)
      , _keyName(keyName.c_str()) {}

    RegistryException::RegistryException(const std::string& keyName, const std::string& message, LONG errorCode)
      : ErrorCodeException(message, errorCode)
      , _keyName(keyName.c_str()) {}

    RegistryException::RegistryException(const std::string& message, LONG errorCode)
      : ErrorCodeException(message, errorCode)
      , _keyName("") {}

    RegistryException::RegistryException(const std::string& message)
      : ErrorCodeException(message, 0L)
      , _keyName("") {}

    RegistryException::RegistryException(const char* keyName, const char* message)
      : ErrorCodeException(message, 0L)
      , _keyName(keyName) {}

    RegistryException::RegistryException(const char* keyName, const char* message, LONG errorCode)
      : ErrorCodeException(message, errorCode)
      , _keyName(keyName) {}

    RegistryException::RegistryException(const char* message, LONG errorCode)
      : ErrorCodeException(message, errorCode)
      , _keyName("") {}

    RegistryException::RegistryException(const char* message)
      : ErrorCodeException(message, 0L)
      , _keyName("") {}

    char const* RegistryException::ErrorKeyName() const noexcept {
      return _keyName;
    }

  } // namespace Exceptions
} // namespace Registry
} // namespace AbsCoDes
