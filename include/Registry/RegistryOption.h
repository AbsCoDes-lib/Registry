//===--- RegistryOption.h ------------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//


#ifndef REGISTRY_OPTION_INCLUDED
#define REGISTRY_OPTION_INCLUDED

#include "Registry/RegistryApi.h"

namespace abscodes {
namespace registry {


    ///
    /// The RegistryOptions enumeration defines the set of supported registry data types.
    ///
    /// Use the RegistryKey.GetValueKind method to determine the data type of a registry key value before retrieving
    ///	the value. When you set a registry key value, use the SetValue method to specify the registry data type explicitly.
    ///
    enum class RegistryOption {
        None = 0,
        Volatile = 1,
    };


    namespace Option {

        static int Handle(RegistryOption option) {
            {
                switch(option) {
                    case RegistryOption::None: return 0;
                    case RegistryOption::Volatile: return 1;
                    default: throw std::invalid_argument("Unsupported registry option.");
                }
            }
        }

    } // namespace Option


} // namespace registry
} // namespace abscodes

#pragma warning(pop)

#endif // REGISTRY_OPTION_INCLUDED
