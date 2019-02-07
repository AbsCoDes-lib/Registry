//===--- RegistryKey.h ---------------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//


#ifndef REGISTRY_KEY_INCLUDED
#define REGISTRY_KEY_INCLUDED

#include "Registry/RegistryApi.h"

#pragma warning(push)
#pragma warning(disable : 4251)

#include <vector>

#include "Registry/RegistryAccessRights.h"
#include "Registry/RegistryHive.h"
#include "Registry/RegistryOption.h"
#include "Registry/RegistryValue.h"
#include "Registry/RegistryValueType.h"
#include "Registry/RegistryView.h"

#pragma warning(push)
#pragma warning(disable : 4251)


namespace abscodes {
namespace registry {


    ///
    class REGISTRY_API RegistryKey
    {

    public:
        ///
        /// Initialize an empty RegistryKey.
        ///
        RegistryKey() = default;

        ///
        /// Initialize a empty RegistryKey.
        ///
        explicit RegistryKey(RegistryHive hive) noexcept;

        ///
        /// Initialize a empty RegistryKey.
        ///
        explicit RegistryKey(RegistryHive hive, RegistryView view) noexcept;

        ///
        /// Initialize a empty RegistryKey.
        ///
        explicit RegistryKey(RegistryHive hive, RegistryView view, RegistryAccessRights desiredAccess) noexcept;

        /// Take ownership of the input key handle.
        /// The input key handle wrapper is reset to an empty state.
        RegistryKey(RegistryKey&& other) noexcept;

        /// Move-assign from the input key handle.
        /// Properly check against self-move-assign (which is safe and does nothing).
        RegistryKey& operator=(RegistryKey&& other) noexcept;

        /// Non copyable
        RegistryKey(const RegistryKey&) = delete;

        /// Non copyable
        RegistryKey& operator=(const RegistryKey&) = delete;

        // Safely close the wrapped key handle (if any)
        ~RegistryKey() noexcept;

    private:
        /// Internal copy constructor
        RegistryKey(const RegistryKey&, HKEY hKey, std::string subkey);

        //
        // Properties
        //

    public:
        /// Is the wrapped HKEY handle valid?
        bool IsValid() const noexcept;

        /// Same as IsValid(), but allow a short "if (regKey)" syntax
        explicit operator bool() const noexcept;

        //
        // Accessor
        //

    public:
        ///
        RegistryHive GetHive() const;

        ///
        RegistryView GetView() const;

        ///
        RegistryAccessRights GetAccessRights() const;

        ///
        std::string GetName() const;

        ///
        size_t GetSubKeyCount();

        ///
        size_t GetValueCount();

        //
        // Operations
        //

    public:
        /// Access the wrapped handle
        HKEY Get() const noexcept;

        /// Closes this key, flushes it to disk if the contents have been modified.
        void Close() noexcept;

        /// Flush the content to disk
        void Flush();

        ///
        void Dispose() noexcept;

        /// Transfer ownership of current HKEY to the caller.
        /// Note that the caller is responsible for closing the key handle!
        HKEY Detach() noexcept;

        /// Take ownership of the input HKEY handle.
        /// Safely close any previously open handle.
        /// Input key handle can be nullptr.
        void Attach(RegistryHive hive) noexcept;

        /// Non-throwing swap;
        /// Note: There's also a non-member swap overload
        void SwapWith(RegistryKey& other) noexcept;

        ///
        /// Creates a new subkey, or opens an existing one.
        ///
        /// @param subkey Name or path to subkey to create or open.
        ///
        /// @exception RegistryException
        ///
        RegistryKey CreateSubKey(std::string subkey);

        ///
        /// Creates a new subkey, or opens an existing one.
        ///
        /// @param subkey Name or path to subkey to create or open.
        /// @param desiredAccess Access rights, default to all access.
        ///
        /// @exception RegistryException
        ///
        RegistryKey CreateSubKey(std::string subkey, RegistryAccessRights desiredAccess);

        ///
        /// Creates a new subkey, or opens an existing one.
        ///
        /// @param subkey Name or path to subkey to create or open.
        /// @param view A scpecific registry view
        /// @param desiredAccess Access rights, default to all access.
        /// @param option todo
        ///
        /// @exception RegistryException
        ///
        RegistryKey CreateSubKey(std::string subkey, RegistryView view, RegistryAccessRights desiredAccess, RegistryOption option);

        ///
        /// Open a subkey. Will throw an exception if the subkey doesn't exist
        ///
        /// @param subkey Name or path to subkey to create or open.
        ///
        /// @exception RegistryException
        ///
        RegistryKey OpenSubKey(std::string subkey);

        ///
        /// Open a subkey. Will throw an exception if the subkey doesn't exist
        ///
        /// @param subkey Name or path to subkey to create or open.
        /// @param desiredAccess Access rights, default to all access.
        ///
        /// @exception RegistryException
        ///
        RegistryKey OpenSubKey(std::string subkey, RegistryAccessRights desiredAccess);

        ///
        /// Open a subkey. Will throw an exception if the subkey doesn't exist
        ///
        /// @param subkey Name or path to subkey to create or open.
        /// @param view A scpecific registry view
        /// @param desiredAccess Access rights, default to all access.
        /// @param option todo
        ///
        /// @exception RegistryException
        ///
        RegistryKey OpenSubKey(std::string subkey, RegistryView view, RegistryAccessRights desiredAccess, RegistryOption option);

        ///
        /// Deletes the specified subkey. Will throw an exception if the subkey has
        /// subkeys. To delete a tree of subkeys use, DeleteSubKeyTree.
        ///
        /// @param subkey SubKey to delete.
        /// @param desiredAccess Access rights, default to all access.
        ///
        /// @exception RegistryException thrown if the subkey has child subkeys.
        ///
        void DeleteSubKey(std::string subkey, RegistryAccessRights desiredAccess = RegistryAccessRights::AllAccess);

        ///
        /// Deletes the specified subkey. Will throw an exception if the subkey has
        /// subkeys. To delete a tree of subkeys use, DeleteSubKeyTree.
        ///
        /// @param subkey SubKey to delete.
        /// @param view A scpecific registry view
        /// @param desiredAccess Access rights, default to all access.
        ///
        /// @exception RegistryException thrown if the subkey has child subkeys.
        ///
        void DeleteSubKey(std::string subkey, RegistryView view, RegistryAccessRights desiredAccess = RegistryAccessRights::AllAccess);

        ///
        /// Deletes the specified subkey and its tree.
        ///
        /// @param subkey SubKey to delete.
        ///
        /// @exception RegistryException
        ///
        void DeleteSubKeyTree(std::string subkey, RegistryAccessRights desiredAccess = RegistryAccessRights::AllAccess);

        ///
        /// Deletes the specified subkey and its tree.
		///
        /// @param view A scpecific registry view
        /// @param desiredAccess Access rights, default to all access.
        ///
        /// @param subkey SubKey to delete.
        ///
        void DeleteSubKeyTree(std::string subkey, RegistryView view, RegistryAccessRights desiredAccess = RegistryAccessRights::AllAccess);

        ///
        /// Deletes the specified value from this key.
        ///
        /// @param name Name of value to delete.
        ///
        void DeleteValue(std::string valueName);

        //
        // Setters
        //

    public:
        // Writes/updates a value in the registry.
        // Wraps ::RegSetValueEx().
        void SetValue(const std::string& valueName, const RegistryValue& value);
        // Writes/updates a value in the registry.
        // Wraps ::RegSetValueEx().
        void SetDwordValue(const std::string& valueName, DWORD value);
        // Writes/updates a value in the registry.
        // Wraps ::RegSetValueEx().
        void SetQwordValue(const std::string& valueName, const ULONGLONG& value);
        // Writes/updates a value in the registry.
        // Wraps ::RegSetValueEx().
        void SetStringValue(const std::string& valueName, const std::string& value);
        // Writes/updates a value in the registry.
        // Wraps ::RegSetValueEx().
        void SetExpandStringValue(const std::string& valueName, const std::string& value);
        // Writes/updates a value in the registry.
        // Wraps ::RegSetValueEx().
        void SetMultiStringValue(const std::string& valueName, const std::vector<std::string>& value);
        // Writes/updates a value in the registry.
        // Wraps ::RegSetValueEx().
        void SetBinaryValue(const std::string& valueName, const std::vector<BYTE>& value);
        // Writes/updates a value in the registry.
        // Wraps ::RegSetValueEx().
        void SetBinaryValue(const std::string& valueName, const BYTE lpByte[], DWORD& dataSize);


        //
        // Getters
        //

    public:
        enum class ExpandStringOption { DontExpand, Expand };

        RegistryValue GetValue(const std::string& valueName);
        DWORD GetDwordValue(const std::string& valueName);
        ULONGLONG GetQwordValue(const std::string& valueName);
        std::string GetStringValue(const std::string& valueName);
        std::string GetExpandStringValue(const std::string& valueName, ExpandStringOption expandOption = ExpandStringOption::DontExpand);
        std::vector<std::string> GetMultiStringValue(const std::string& valueName);
        std::vector<BYTE> GetBinaryValue(const std::string& valueName);


        //
        // Query Operations
        //

    public:
        /// Return the DWORD type ID for the input registry value
        DWORD QueryValueType(const std::string& valueName);

        ///
        void QueryInfoKey(DWORD& subKeys, DWORD& values, FILETIME& lastWriteTime);

        /// Enumerate the subkeys of the registry key, using RegEnumKeyEx
        std::vector<std::string> EnumSubKeys();

        /// Enumerate the values under the registry key, using RegEnumValue.
        /// Returns a vector of pairs: In each pair, the wstring is the value name,
        /// the DWORD is the value type.
        std::vector<std::pair<std::string, RegistryValueType>> EnumValues();


        //
        // Reflection Operations
        //

        ///
        void EnableReflectionKey();

        ///
        void DisableReflectionKey();

        ///
        bool QueryReflectionKey();

        //
        // Internal Operations
        //

    private:
        /// Close the key
        void Dispose(bool disposing);

        /// Check if the current key is a system key
        bool IsSystemKey() const;

        /// Check if the current key is HKEY_PERFORMANCE_DATA
        bool IsPerfDataKey() const;

        ///
        bool IsWritable() const;

        bool IsDirty() const;
        void setDirty();

        /// Ensure the key is not closed
        void EnsureNotDisposed() const;

        /// Ensure the key is writable
        void EnsureWriteable() const;

        ///
        void ValidateKeyName(std::string& keyName);

        /// Fixup multiple slashes to a single slash
        std::string& FixupName(std::string& name);


    private:
        ///
        RegistryHive _hive;
        ///
        RegistryView _view = RegistryView::Default;
        ///
        RegistryAccessRights _access = RegistryAccessRights::Read;
        /// The wrapped registry key handle
        HKEY _hKey = nullptr;
        /// Key name
        std::string _keyName;
        /// Tells if the needs to be flush
        bool _dirty = false;


        /// MSDN defines the following limits for registry key names & values:
        /// Key Name: 255 characters
        /// Value name:  16,383 Unicode characters
        /// Value: either 1 MB or current available memory, depending on registry format.
        const size_t _maxKeyLength = 255;
        const size_t _maxValueLength = 16383;
    };

    inline void swap(RegistryKey& lhs, RegistryKey& rhs) noexcept {
        lhs.SwapWith(rhs);
    }

    //------------------------------------------------------------------------------
    //          Overloads of relational comparison operators for RegistryKey
    //------------------------------------------------------------------------------
	inline bool operator==(const RegistryKey& a, HKEY& b) noexcept {
		return a.Get() == b;
	}

	inline bool operator!=(const RegistryKey& a, HKEY& b) noexcept {
		return a.Get() != b;
	}

    inline bool operator==(const RegistryKey& a, const RegistryKey& b) noexcept {
        return a.GetHive() == b.GetHive() && a.GetView() == b.GetView() && a.GetAccessRights() == b.GetAccessRights() && a.GetName() == b.GetName();
    }

    inline bool operator!=(const RegistryKey& a, const RegistryKey& b) noexcept {
		return a.GetHive() != b.GetHive() || a.GetView() != b.GetView() || a.GetAccessRights() != b.GetAccessRights() || a.GetName() != b.GetName();
    }

} // namespace registry
} // namespace abscodes

#pragma warning(pop)

#endif // REGISTRY_KEY_INCLUDED
