//===--- RegistryHive.h --------------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//


#ifndef REGISTRY_HIVE_INCLUDED
#define REGISTRY_HIVE_INCLUDED


#include "Registry/RegistryApi.h"

namespace abscodes {
namespace registry {


    ///
    /// The RegistryHive enumeration defines the set of supported registry data types.
    ///
    /// Use the RegistryKey.GetValueKind method to determine the data type of a registry key value before retrieving
    /// the value. When you set a registry key value, use the SetValue method to specify the registry data type explicitly.
    ///
    enum class RegistryHive {
        ClassesRoot = static_cast<int>(0x80000000), // HKEY_CLASSES_ROOT
        CurrentUser = static_cast<int>(0x80000001), // HKEY_CURRENT_USER
        LocalMachine = static_cast<int>(0x80000002), // HKEY_LOCAL_MACHINE
        Users = static_cast<int>(0x80000003), // HKEY_USERS
        PerformanceData = static_cast<int>(0x80000004), // HKEY_PERFORMANCE_DATA
        CurrentConfig = static_cast<int>(0x80000005), // HKEY_CURRENT_CONFIG
        DynData = static_cast<int>(0x80000006), // HKEY_DYN_DATA
    };


    namespace Hive {

        static HKEY Handle(RegistryHive hive) {
            {
                switch(hive) {
                    case RegistryHive::ClassesRoot: return HKEY_CLASSES_ROOT;
                    case RegistryHive::CurrentUser: return HKEY_CURRENT_USER;
                    case RegistryHive::LocalMachine: return HKEY_LOCAL_MACHINE;
                    case RegistryHive::Users: return HKEY_USERS;
                    case RegistryHive::PerformanceData: return HKEY_PERFORMANCE_DATA;
                    case RegistryHive::CurrentConfig: return HKEY_CURRENT_CONFIG;
                    case RegistryHive::DynData: return HKEY_DYN_DATA;
                    default: throw std::invalid_argument("Unsupported Windows registry hive.");
                }
            }
        }

        static std::string Name(RegistryHive hive) {
            {
                switch(hive) {
                    case RegistryHive::ClassesRoot: return "HKEY_CLASSES_ROOT";
                    case RegistryHive::CurrentUser: return "HKEY_CURRENT_USER";
                    case RegistryHive::LocalMachine: return "HKEY_LOCAL_MACHINE";
                    case RegistryHive::Users: return "HKEY_USERS";
                    case RegistryHive::PerformanceData: return "HKEY_PERFORMANCE_DATA";
                    case RegistryHive::CurrentConfig: return "HKEY_CURRENT_CONFIG";
                    case RegistryHive::DynData: return "HKEY_DYN_DATA";
                    default: throw std::invalid_argument("Unsupported Windows registry hive.");
                }
            }
        }
    } // namespace Hive


} // namespace registry
} // namespace abscodes


#endif // REGISTRY_HIVE_INCLUDED
