//===--- RegistryException.h ---------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//


#ifndef REGISTRY_EXCEPTION_INCLUDED
#define REGISTRY_EXCEPTION_INCLUDED

#include "Registry\RegistryApi.h"

#include "Commons\Exceptions\ErrorCodeException.h"

namespace abscodes {
namespace registry {
    namespace Exceptions {


        ///
        class REGISTRY_API RegistryException : public abscodes::commons::exceptions::ErrorCodeException
        {

        public:
            /// Construct a RegistryException.
            explicit RegistryException(const std::string& keyName, const std::string& message);

            /// Construct a RegistryException.
            explicit RegistryException(const std::string& keyName, const std::string& message, LONG errorCode);

            /// Construct a RegistryException.
            explicit RegistryException(const std::string& message, LONG errorCode);

            /// Construct a RegistryException.
            explicit RegistryException(const std::string& message);

            /// Construct a RegistryException.
            explicit RegistryException(const char* keyName, const char* message);

            /// Construct a RegistryException.
            explicit RegistryException(const char* keyName, const char* message, LONG errorCode);

            /// Construct a RegistryException.
            explicit RegistryException(const char* message, LONG errorCode);

            /// Construct a RegistryException.
            explicit RegistryException(const char* message);

        public:
            /// Get the error key name
            char const* ErrorKeyName() const noexcept;

        private:
            /// Error code, as returned by Windows registry APIs
            char const* _keyName;
        };


    } // namespace Exceptions
} // namespace registry
} // namespace abscodes


#endif // REGISTRY_EXCEPTION_INCLUDED
