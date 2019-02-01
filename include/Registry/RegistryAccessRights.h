//===--- RegistryAccessRights.h ------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//


#ifndef REGISTRY_ACCESS_RIGHTS_INCLUDED
#define REGISTRY_ACCESS_RIGHTS_INCLUDED


#include "Registry/RegistryApi.h"

namespace abscodes {
namespace registry {

    ///
    /// The RegistryAccessRights enumeration defines the set of supported registry data types.
    ///
    /// Use the RegistryKey.GetValueKind method to determine the data type of a registry key value before retrieving
    /// the value. When you set a registry key value, use the SetValue method to specify the registry data type explicitly.
    ///
    enum class RegistryAccessRights {
        Read = static_cast<int>(KEY_READ),
        Write = static_cast<int>(KEY_WRITE),
        AllAccess = static_cast<int>(KEY_ALL_ACCESS),
        Query = static_cast<int>(KEY_QUERY_VALUE),
        EnumerateSubKeys = static_cast<int>(KEY_ENUMERATE_SUB_KEYS),
        SetValue = static_cast<int>(KEY_SET_VALUE),
        CreateSubKey = static_cast<int>(KEY_CREATE_SUB_KEY),
        CreateLink = static_cast<int>(KEY_CREATE_LINK),
        Notify = static_cast<int>(KEY_NOTIFY),
    };


    namespace AccessRights {

        static REGSAM Handle(RegistryAccessRights rar) {
            {
                switch(rar) {
                    case RegistryAccessRights::Read: return KEY_READ;
                    case RegistryAccessRights::Write: return KEY_WRITE;
                    case RegistryAccessRights::AllAccess: return KEY_ALL_ACCESS;
                    case RegistryAccessRights::Query: return KEY_QUERY_VALUE;
                    case RegistryAccessRights::EnumerateSubKeys: return KEY_ENUMERATE_SUB_KEYS;
                    case RegistryAccessRights::SetValue: return KEY_SET_VALUE;
                    case RegistryAccessRights::CreateSubKey: return KEY_CREATE_SUB_KEY;
                    case RegistryAccessRights::CreateLink: return KEY_CREATE_LINK;
                    case RegistryAccessRights::Notify: return KEY_NOTIFY;
                    default: throw std::invalid_argument("Unsupported registry acces right.");
                }
            }
        }

    } // namespace AccessRights

} // namespace registry
} // namespace abscodes


#endif // REGISTRY_ACCESS_RIGHTS_INCLUDED
