//===--- RegistryKey.cpp -------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//

#include "Registry/RegistryKey.h"

#include "Commons/StringUtils.h"
#include "Commons/Utf8Convert.h"
#include "Registry/RegistryException.h"

namespace abscodes {
namespace registry {

  RegistryKey::RegistryKey(RegistryHive hive) noexcept
    : _hive(hive)
    , _hKey(Hive::Handle(hive)) {}

  RegistryKey::RegistryKey(RegistryHive hive, RegistryView view) noexcept
    : _hive(hive)
    , _hKey(Hive::Handle(hive))
    , _view(view) {}

  RegistryKey::RegistryKey(RegistryHive hive, RegistryView view, RegistryAccessRights access) noexcept
    : _hive(hive)
    , _hKey(Hive::Handle(hive))
    , _view(view)
    , _access(access) {}

  RegistryKey::RegistryKey(RegistryKey&& other) noexcept
    : _hive(other._hive)
    , _hKey(Hive::Handle(other._hive))
    , _keyName(other._keyName)
    , _view(other._view)
    , _access(other._access) {
    // Other doesn't own the handle anymore
    other._hKey = nullptr;
  }

  RegistryKey& RegistryKey::operator=(RegistryKey&& other) noexcept {
    // Prevent self-move-assign
    if((this != &other) && (_hKey != other._hKey)) {
      // Close current
      Close();

      // Move from other (i.e. take ownership of other's raw handle)
      _hKey       = other._hKey;
      other._hKey = nullptr;
    }
    return *this;
  }

  RegistryKey::~RegistryKey() {
    // Release the owned handle (if any)
    Close();
  }

  RegistryKey::RegistryKey(const RegistryKey& other, HKEY hKey, std::string subkey)
    : _hive(other._hive)
    , _hKey(hKey)
    , _view(other._view)
    , _access(other._access) {
    ValidateKeyName(subkey);
    _keyName = subkey;
  }

  bool RegistryKey::IsValid() const noexcept {
    return _hKey != nullptr;
  }

  RegistryKey::operator bool() const noexcept {
    return IsValid();
  }

  RegistryHive registry::RegistryKey::GetHive() const {
    EnsureNotDisposed();
    return _hive;
  }

  RegistryView RegistryKey::GetView() const {
    EnsureNotDisposed();
    return _view;
  }

  RegistryAccessRights registry::RegistryKey::GetAccessRights() const {
    EnsureNotDisposed();
    return _access;
  }

  std::string RegistryKey::GetName() const {
    EnsureNotDisposed();
    return _keyName;
  }

  size_t RegistryKey::GetSubKeyCount() {
    try {
      return EnumSubKeys().size();
    }
    catch(...) {
    }
    return 0;
  }

  size_t RegistryKey::GetValueCount() {
    try {
      return EnumValues().size();
    }
    catch(...) {
    }
    return 0;
  }

  HKEY RegistryKey::Get() const noexcept {
    return _hKey;
  }

  void RegistryKey::Close() noexcept {
    Dispose(true);
  }

  void RegistryKey::Flush() {
    if(IsValid() && IsDirty()) {
      const auto retCode = ::RegFlushKey(_hKey);
      if(retCode != ERROR_SUCCESS) {
        throw Exceptions::RegistryException("RegFlushKey failed.", retCode);
      }
    }
  }

  void RegistryKey::Dispose() noexcept {
    Dispose(true);
  }

  HKEY RegistryKey::Detach() noexcept {
    const auto hKey(_hKey);

    // We don't own the HKEY handle anymore
    _hKey = nullptr;

    // Transfer ownership to the caller
    return hKey;
  }

  void RegistryKey::Attach(RegistryHive hive) noexcept {
    if(Hive::Handle(hive) != _hKey) {
      // Close current
      Close();
      // Take ownership
      _hKey = Hive::Handle(hive);
    }
  }

  void RegistryKey::SwapWith(RegistryKey& other) noexcept {
    // Swap the raw handle members
    std::swap(_hKey, other._hKey);
  }

  RegistryKey RegistryKey::CreateSubKey(std::string subkey, RegistryAccessRights desiredAccess) {
    return CreateSubKey(subkey, this->GetView(), desiredAccess, RegistryOption::None);
  }

  RegistryKey RegistryKey::CreateSubKey(std::string subkey, RegistryView view, RegistryAccessRights desiredAccess, RegistryOption option) {

    _ASSERTE(IsValid());

    // Update acces rights
    _access = desiredAccess;
    // Update acces rights
    _view = view;
    // Make sure the key is writable
    EnsureWriteable();
    // validate the subkey
    ValidateKeyName(subkey);

    std::wstring wsubkey = commons::utf8convert::Utf8ToUtf16(subkey);

    HKEY       hKey    = nullptr;
    const auto retCode = ::RegCreateKeyEx(_hKey, //
                                          wsubkey.c_str(), //
                                          0, // reserved
                                          REG_NONE, // user-defined class type parameter not supported
                                          (DWORD)option, //
                                          (DWORD)desiredAccess | (DWORD)view, //
                                          nullptr, // securityAttributes,
                                          &hKey, //
                                          nullptr // disposition
    );
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegCreateKeyEx failed.", retCode);
    }

    //
    return RegistryKey(*this, hKey, _keyName + '\\' + subkey);
  }

  RegistryKey RegistryKey::OpenSubKey(std::string subkey) {
    return OpenSubKey(subkey, this->GetView(), this->GetAccessRights(), RegistryOption::None);
  }

  RegistryKey RegistryKey::OpenSubKey(std::string subkey, RegistryAccessRights desiredAccess) {
    return OpenSubKey(subkey, this->GetView(), desiredAccess, RegistryOption::None);
  }

  RegistryKey RegistryKey::OpenSubKey(std::string subkey, RegistryView view, RegistryAccessRights desiredAccess, RegistryOption option) {

    _ASSERTE(IsValid());

    // Update acces rights
    _access = desiredAccess;
    // Update view
    _view = view;
    // validate the subkey
    ValidateKeyName(subkey);

    std::wstring wsubkey = commons::utf8convert::Utf8ToUtf16(subkey);

    HKEY       hKey    = nullptr;
    const auto retCode = ::RegOpenKeyEx(_hKey, //
                                        wsubkey.c_str(), //
                                        (DWORD)option, //
                                        (DWORD)desiredAccess | (DWORD)view, //
                                        &hKey);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegOpenKeyEx failed.", retCode);
    }

    //
    return RegistryKey(*this, hKey, _keyName + '\\' + subkey);
  }

  void RegistryKey::DeleteSubKey(std::string subkey, RegistryAccessRights desiredAccess) {
    return DeleteSubKey(subkey, this->GetView(), desiredAccess);
  }

  void RegistryKey::DeleteSubKey(std::string subkey, RegistryView view, RegistryAccessRights desiredAccess) {

    _ASSERTE(IsValid());

    // Update acces rights
    _access = desiredAccess;
    // Update view
    _view = view;
    // Make sure the key is writable
    EnsureWriteable();
    // validate the subkey
    ValidateKeyName(subkey);

    std::wstring wsubkey = commons::utf8convert::Utf8ToUtf16(subkey);

    const auto retCode = ::RegDeleteKeyEx(_hKey, //
                                          wsubkey.c_str(), //
                                          (REGSAM)desiredAccess | (DWORD)view, //
                                          0);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegDeleteKeyEx failed.", retCode);
    }
  }

  void RegistryKey::DeleteSubKeyTree(std::string subkey, RegistryAccessRights desiredAccess) {
    DeleteSubKeyTree(subkey, this->GetView(), desiredAccess);
  }

  void RegistryKey::DeleteSubKeyTree(std::string subkey, RegistryView view, RegistryAccessRights desiredAccess) {

    _ASSERTE(IsValid());

    // Update acces rights
    _access = desiredAccess;
    // Update view
    _view = view;
    // Make sure the key is writable
    EnsureWriteable();
    // validate the subkey
    ValidateKeyName(subkey);

    std::wstring wsubkey = commons::utf8convert::Utf8ToUtf16(subkey);

    RegistryKey key = OpenSubKey(subkey, view, desiredAccess, RegistryOption::None);

    if(key.IsValid()) {
      try {
        for(auto subKey : key.EnumSubKeys()) {
          key.DeleteSubKeyTree(subKey, view, desiredAccess);
        }
      }
      catch(...) {
      }

      key.Close();

      DeleteSubKey(subkey, view, desiredAccess);
    }
  }

  void RegistryKey::DeleteValue(std::string valueName) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);

    const auto retCode = ::RegDeleteValue(_hKey, //
                                          sValueName.c_str() //
    );
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegDeleteValue failed.", retCode);
    }
  }

  void RegistryKey::SetValue(const std::string& valueName, const RegistryValue& value) {

    _ASSERTE(IsValid());

    switch(value.GetType()) {
      case RegistryValueType::DWord: return SetDwordValue(valueName, value.DWord());
      case RegistryValueType::DWordBigEndian: return SetDwordValue(valueName, value.DWord());
      case RegistryValueType::QWord: return SetQwordValue(valueName, value.QWord());
      case RegistryValueType::String: return SetStringValue(valueName, value.String());
      case RegistryValueType::ExpandString: return SetExpandStringValue(valueName, value.ExpandString());
      case RegistryValueType::MultiString: return SetMultiStringValue(valueName, value.MultiString());
      case RegistryValueType::Binary: return SetBinaryValue(valueName, value.Binary());
      default: throw std::invalid_argument("Unsupported Windows registry value type.");
    }
  }

  // MSVC emits a warning in 64-bit builds when assigning size_t to DWORD.
  // So, only in 64-bit builds, check proper size limits before conversion
  // and throw std::overflow_error if the size_t value is too big.
  DWORD SafeSizeToDwordCast(size_t size) {
#ifdef _WIN64
    if(size > static_cast<size_t>((std::numeric_limits<DWORD>::max)())) {
      throw std::overflow_error("SafeSizeToDwordCast(): Input size_t too long, it doesn't fit in a DWORD.");
    }

    // This cast is now safe
    return static_cast<DWORD>(size);
#else
    // Just fine in 32-bit builds
    return size;
#endif
  }

  void RegistryKey::SetDwordValue(const std::string& valueName, DWORD value) {

    _ASSERTE(IsValid());

    const DWORD        data       = value;
    const DWORD        dataSize   = sizeof(data);
    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);
    const auto         result     = ::RegSetValueEx(_hKey, //
                                        sValueName.c_str(), //
                                        0, // reserved
                                        REG_DWORD, //
                                        reinterpret_cast<const BYTE*>(&data), //
                                        dataSize);
    if(result != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegSetValueEx() failed in writing REG_DWORD value.", result);
    }
  }

  void RegistryKey::SetQwordValue(const std::string& valueName, const ULONGLONG& value) {

    _ASSERTE(IsValid());

    const ULONGLONG    data       = value;
    const DWORD        dataSize   = sizeof(data);
    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);
    const auto         result     = ::RegSetValueEx(_hKey, //
                                        sValueName.c_str(), //
                                        0, // reserved
                                        REG_QWORD, //
                                        reinterpret_cast<const BYTE*>(&data), //
                                        dataSize);
    if(result != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegSetValueEx() failed in writing REG_QWORD value.", result);
    }
  }

  void RegistryKey::SetStringValue(const std::string& valueName, const std::string& value) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);
    const std::wstring sValue     = commons::utf8convert::Utf8ToUtf16(value);
    // According to MSDN doc, this size must include the terminating NULL
    // Note that size is in *BYTES*, so we must scale by wchar_t.
    const DWORD dataSize = SafeSizeToDwordCast((sValue.size() + 1) * sizeof(wchar_t));

    const auto result = ::RegSetValueEx(_hKey, //
                                        sValueName.c_str(), //
                                        0, // reserved
                                        REG_SZ, //
                                        reinterpret_cast<const BYTE*>(sValue.c_str()), //
                                        dataSize);
    if(result != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegSetValueEx() failed in writing REG_SZ value.", result);
    }
  }

  void RegistryKey::SetExpandStringValue(const std::string& valueName, const std::string& value) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);
    const std::wstring sValue     = commons::utf8convert::Utf8ToUtf16(value);
    // According to MSDN doc, this size must include the terminating NULL
    // Note that size is in *BYTES*, so we must scale by wchar_t.
    const DWORD dataSize = SafeSizeToDwordCast((sValue.size() + 1) * sizeof(wchar_t));

    const auto result = ::RegSetValueEx(_hKey, //
                                        sValueName.c_str(), //
                                        0, // reserved
                                        REG_EXPAND_SZ, //
                                        reinterpret_cast<const BYTE*>(sValue.c_str()), //
                                        dataSize);
    if(result != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegSetValueEx() failed in writing REG_EXPAND_SZ value.", result);
    }
  }

  void RegistryKey::SetMultiStringValue(const std::string& valueName, const std::vector<std::string>& value) {

    _ASSERTE(IsValid());

    const std::wstring               sValueName   = commons::utf8convert::Utf8ToUtf16(valueName);
    const std::vector<std::wstring>& sMultiString = commons::utf8convert::Utf8ToUtf16(value);

    // We need to build a whole array containing the multi-strings, with double-NUL termination
    std::vector<wchar_t> buffer;

    // Get total buffer size, in wchar_ts
    size_t totalLen = 0;
    for(const std::wstring& s : sMultiString) {
      // +1 to include the terminating NUL for current string
      totalLen += (s.size() + 1);
    }
    // Consider another terminating NUL (double-NUL-termination)
    totalLen++;

    // Optimization: reserve room in the multi-string buffer
    buffer.resize(totalLen);

    // Deep copy the single strings in the buffer
    if(!sMultiString.empty()) {
      wchar_t* dest = &buffer[0];
      for(const std::wstring& s : sMultiString) {
        // Copy the whole string to destination buffer, including the terminating NUL
        wmemcpy(dest, s.c_str(), s.size() + 1);

        // Skip to the next string slot
        dest += s.size() + 1;
      }

      // Add another NUL terminator
      *dest = L'\0';
    }
    else {
      // Just write two NULs in the buffer
      buffer.resize(2);
      buffer[0] = L'\0';
      buffer[1] = L'\0';
    }

    // Size is in *BYTES*
    const DWORD dataSize = SafeSizeToDwordCast(buffer.size() * sizeof(wchar_t));

    const auto result = ::RegSetValueEx(_hKey, //
                                        sValueName.c_str(), //
                                        0, // reserved
                                        REG_MULTI_SZ, //
                                        reinterpret_cast<const BYTE*>(buffer.data()), //
                                        dataSize);
    if(result != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegSetValueEx() failed in writing REG_MULTI_SZ value.", result);
    }
  }

  void RegistryKey::SetBinaryValue(const std::string& valueName, const std::vector<BYTE>& value) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);

    const std::vector<BYTE>& data     = value;
    const DWORD              dataSize = SafeSizeToDwordCast(data.size());
    const auto               result   = ::RegSetValueEx(_hKey, //
                                        sValueName.c_str(), //
                                        0, // reserved
                                        REG_BINARY, //
                                        &data[0], //
                                        dataSize);
    if(result != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegSetValueEx() failed in writing REG_BINARY value.", result);
    }
  }

  void RegistryKey::SetBinaryValue(const std::string& valueName, const BYTE lpByte[], DWORD& dataSize) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);
    const auto         result     = ::RegSetValueEx(_hKey, //
                                        sValueName.c_str(), //
                                        0, // reserved
                                        REG_BINARY, //
                                        lpByte, //
                                        dataSize);
    if(result != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegSetValueEx() failed in writing REG_BINARY value.", result);
    }
  }

  RegistryValue RegistryKey::GetValue(const std::string& valueName) {

    DWORD             dValueType = QueryValueType(valueName);
    RegistryValueType valueType  = ValueType::Handle(dValueType);

    RegistryValue value(valueType);

    switch(valueType) {
      case RegistryValueType::DWord: value.DWord() = GetDwordValue(valueName); break;
      case RegistryValueType::DWordBigEndian: value.DWord() = GetDwordValue(valueName); break;
      case RegistryValueType::QWord: value.QWord() = GetQwordValue(valueName); break;
      case RegistryValueType::String: value.String() = GetStringValue(valueName); break;
      case RegistryValueType::ExpandString: value.ExpandString() = GetExpandStringValue(valueName); break;
      case RegistryValueType::MultiString: value.MultiString() = GetMultiStringValue(valueName); break;
      case RegistryValueType::Binary: value.Binary() = GetBinaryValue(valueName); break;
      default: throw std::invalid_argument("Unsupported registry value type.");
    }

    return value;
  }

  DWORD RegistryKey::GetDwordValue(const std::string& valueName) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);

    DWORD data {}; // to be read from the registry
    DWORD dataSize = sizeof(data); // size of data, in bytes

    const DWORD flags   = RRF_RT_REG_DWORD;
    const auto  retCode = ::RegGetValue(_hKey, //
                                       nullptr, // no subkey
                                       sValueName.c_str(), //
                                       flags, //
                                       nullptr, // type not required
                                       &data, //
                                       &dataSize //
    );
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("Cannot get DWORD value: RegGetValue failed.", retCode);
    }

    return data;
  }

  ULONGLONG RegistryKey::GetQwordValue(const std::string& valueName) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);

    ULONGLONG data {}; // to be read from the registry
    DWORD     dataSize = sizeof(data); // size of data, in bytes

    const DWORD flags   = RRF_RT_REG_QWORD;
    const auto  retCode = ::RegGetValue(_hKey, //
                                       nullptr, // no subkey
                                       sValueName.c_str(), //
                                       flags, //
                                       nullptr, // type not required
                                       &data, //
                                       &dataSize //
    );
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("Cannot get QWORD value: RegGetValue failed.", retCode);
    }

    return data;
  }

  std::string RegistryKey::GetStringValue(const std::string& valueName) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);

    // Get the size of the result string
    DWORD       dataSize = 0; // size of data, in bytes
    const DWORD flags    = RRF_RT_REG_SZ;
    auto        retCode  = ::RegGetValue(_hKey, //
                                 nullptr, // no subkey
                                 sValueName.c_str(), //
                                 flags, //
                                 nullptr, // type not required
                                 nullptr, // output buffer not needed now
                                 &dataSize //
    );
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("Cannot get size of string value: RegGetValue failed.", retCode);
    }

    // Allocate a string of proper size.
    // Note that dataSize is in bytes and includes the terminating NUL;
    // we have to convert the size from bytes to wchar_ts for wstring::resize.
    std::wstring result;
    result.resize(dataSize / sizeof(wchar_t));

    // Call RegGetValue for the second time to read the string's content
    retCode = ::RegGetValue(_hKey, //
                            nullptr, // no subkey
                            sValueName.c_str(), //
                            flags, //
                            nullptr, // type not required
                            &result[0], // output buffer
                            &dataSize);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("Cannot get string value: RegGetValue failed.", retCode);
    }

    // Remove the NUL terminator scribbled by RegGetValue from the wstring
    result.resize((dataSize / sizeof(wchar_t)) - 1);

    return commons::utf8convert::Utf16ToUtf8(result);
  }

  std::string RegistryKey::GetExpandStringValue(const std::string& valueName, ExpandStringOption expandOption) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);

    DWORD flags = RRF_RT_REG_EXPAND_SZ;

    // Adjust the flag for RegGetValue considering the expand string option specified by the caller
    if(expandOption == ExpandStringOption::DontExpand) {
      flags |= RRF_NOEXPAND;
    }

    // Get the size of the result string
    DWORD dataSize = 0; // size of data, in bytes
    auto  retCode  = ::RegGetValue(_hKey, //
                                 nullptr, // no subkey
                                 sValueName.c_str(), //
                                 flags, //
                                 nullptr, // type not required
                                 nullptr, // output buffer not needed now
                                 &dataSize);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("Cannot get size of expand string value: RegGetValue failed.", retCode);
    }

    // Allocate a string of proper size.
    // Note that dataSize is in bytes and includes the terminating NUL.
    // We must convert from bytes to wchar_ts for wstring::resize.
    std::wstring result;
    result.resize(dataSize / sizeof(wchar_t));

    // Call RegGetValue for the second time to read the string's content
    retCode = ::RegGetValue(_hKey, //
                            nullptr, // no subkey
                            sValueName.c_str(), //
                            flags, //
                            nullptr, // type not required
                            &result[0], // output buffer
                            &dataSize);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("Cannot get expand string value: RegGetValue failed.", retCode);
    }

    // Remove the NUL terminator scribbled by RegGetValue from the wstring
    result.resize((dataSize / sizeof(wchar_t)) - 1);

    return commons::utf8convert::Utf16ToUtf8(result);
  }

  std::vector<std::string> RegistryKey::GetMultiStringValue(const std::string& valueName) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);

    const DWORD flags = RRF_RT_REG_MULTI_SZ;

    // Request the size of the multi-string, in bytes
    DWORD dataSize = 0;
    auto  retCode  = ::RegGetValue(_hKey, //
                                 nullptr, // no subkey
                                 sValueName.c_str(), //
                                 flags, //
                                 nullptr, // type not required
                                 nullptr, // output buffer not needed now
                                 &dataSize);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("Cannot get size of multi-string value: RegGetValue failed.", retCode);
    }

    // Allocate room for the result multi-string.
    // Note that dataSize is in bytes, but our vector<wchar_t>::resize method requires size
    // to be expressed in wchar_ts.
    std::vector<wchar_t> data;
    data.resize(dataSize / sizeof(wchar_t));

    // Read the multi-string from the registry into the vector object
    retCode = ::RegGetValue(_hKey, //
                            nullptr, // no subkey
                            sValueName.c_str(), //
                            flags, //
                            nullptr, // no type required
                            &data[0], // output buffer
                            &dataSize);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("Cannot get multi-string value: RegGetValue failed.", retCode);
    }

    // Resize vector to the actual size returned by GetRegValue.
    // Note that the vector is a vector of wchar_ts, instead the size returned by GetRegValue
    // is in bytes, so we have to scale from bytes to wchar_t count.
    data.resize(dataSize / sizeof(wchar_t));

    // Parse the double-NUL-terminated string into a vector<wstring>,
    // which will be returned to the caller
    std::vector<std::wstring> result;
    const wchar_t*            currStringPtr = &data[0];
    while(*currStringPtr != L'\0') {
      // Current string is NUL-terminated, so get its length calling wcslen
      const size_t currStringLength = wcslen(currStringPtr);

      // Add current string to the result vector
      result.push_back(std::wstring {currStringPtr, currStringLength});

      // Move to the next string
      currStringPtr += currStringLength + 1;
    }

    return commons::utf8convert::Utf16ToUtf8(result);
  }

  std::vector<BYTE> RegistryKey::GetBinaryValue(const std::string& valueName) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);

    const DWORD flags = RRF_RT_REG_BINARY;

    // Get the size of the binary data
    DWORD dataSize = 0; // size of data, in bytes
    auto  retCode  = ::RegGetValue(_hKey, //
                                 nullptr, // no subkey
                                 sValueName.c_str(), //
                                 flags, //
                                 nullptr, // type not required
                                 nullptr, // output buffer not needed now
                                 &dataSize);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("Cannot get size of binary data: RegGetValue failed.", retCode);
    }

    // Allocate a buffer of proper size to store the binary data
    std::vector<BYTE> data(dataSize);

    // Call RegGetValue for the second time to read the data content
    retCode = ::RegGetValue(_hKey, //
                            nullptr, // no subkey
                            sValueName.c_str(), //
                            flags, //
                            nullptr, // type not required
                            &data[0], // output buffer
                            &dataSize);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("Cannot get binary data: RegGetValue failed.", retCode);
    }

    return data;
  }

  DWORD RegistryKey::QueryValueType(const std::string& valueName) {

    _ASSERTE(IsValid());

    const std::wstring sValueName = commons::utf8convert::Utf8ToUtf16(valueName);

    DWORD typeId {}; // will be returned by RegQueryValueEx

    const auto retCode = ::RegQueryValueEx(_hKey, //
                                           sValueName.c_str(), //
                                           nullptr, // reserved
                                           &typeId,
                                           nullptr, // not interested
                                           nullptr // not interested
    );

    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("Cannot get the value type: RegQueryValueEx failed.", retCode);
    }

    return typeId;
  }

  void RegistryKey::QueryInfoKey(DWORD& subKeys, DWORD& values, FILETIME& lastWriteTime) {

    _ASSERTE(IsValid());

    const auto retCode = ::RegQueryInfoKey(_hKey, //
                                           nullptr, //
                                           nullptr, //
                                           nullptr, //
                                           &subKeys, //
                                           nullptr, //
                                           nullptr, //
                                           &values, //
                                           nullptr, //
                                           nullptr, //
                                           nullptr, //
                                           &lastWriteTime);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegQueryInfoKey failed.", retCode);
    }
  }

  std::vector<std::string> RegistryKey::EnumSubKeys() {

    _ASSERTE(IsValid());

    // Get some useful enumeration info, like the total number of subkeys
    // and the maximum length of the subkey names
    DWORD subKeyCount {};
    DWORD maxSubKeyNameLen {};
    auto  retCode = ::RegQueryInfoKey(_hKey, //
                                     nullptr, // no user-defined class
                                     nullptr, // no user-defined class size
                                     nullptr, // reserved
                                     &subKeyCount, //
                                     &maxSubKeyNameLen, //
                                     nullptr, // no subkey class length
                                     nullptr, // no value count
                                     nullptr, // no value name max length
                                     nullptr, // no max value length
                                     nullptr, // no security descriptor
                                     nullptr // no last write time
    );
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegQueryInfoKey failed while preparing for subkey enumeration.", retCode);
    }

    // NOTE: According to the MSDN documentation, the size returned for subkey name max length
    // does *not* include the terminating NULL, so let's add +1 to take it into account
    // when I allocate the buffer for reading subkey names.
    maxSubKeyNameLen++;

    // Preallocate a buffer for the subkey names
    auto nameBuffer = std::make_unique<wchar_t[]>(maxSubKeyNameLen);

    // The result subkey names will be stored here
    std::vector<std::string> subkeyNames;

    // Enumerate all the subkeys
    for(DWORD index = 0; index < subKeyCount; index++) {
      // Get the name of the current subkey
      DWORD subKeyNameLen = maxSubKeyNameLen;
      retCode             = ::RegEnumKeyEx(_hKey, //
                               index, //
                               nameBuffer.get(), //
                               &subKeyNameLen, //
                               nullptr, // reserved
                               nullptr, // no class
                               nullptr, // no class
                               nullptr // no last write time
      );
      if(retCode != ERROR_SUCCESS) {
        throw Exceptions::RegistryException("Cannot enumerate subkeys: RegEnumKeyEx failed.", retCode);
      }

      // On success, the ::RegEnumKeyEx API writes the length of the
      // subkey name in the subKeyNameLen output parameter
      // (not including the terminating NUL).
      // So I can build a string based on that length.
      std::string subkey = commons::utf8convert::Utf16ToUtf8(std::wstring(nameBuffer.get(), subKeyNameLen));
      subkeyNames.push_back(subkey);
    }

    return subkeyNames;
  }

  std::vector<std::pair<std::string, RegistryValueType>> RegistryKey::EnumValues() {

    _ASSERTE(IsValid());

    // Get useful enumeration info, like the total number of values
    // and the maximum length of the value names
    DWORD valueCount {};
    DWORD maxValueNameLen {};
    auto  retCode = ::RegQueryInfoKey(_hKey, //
                                     nullptr, // no user-defined class
                                     nullptr, // no user-defined class size
                                     nullptr, // reserved
                                     nullptr, // no subkey count
                                     nullptr, // no subkey max length
                                     nullptr, // no subkey class length
                                     &valueCount, //
                                     &maxValueNameLen, //
                                     nullptr, // no max value length
                                     nullptr, // no security descriptor
                                     nullptr // no last write time
    );
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegQueryInfoKey failed while preparing for value enumeration.", retCode);
    }

    // NOTE: According to the MSDN documentation, the size returned for value name max length
    // does *not* include the terminating NUL, so let's add +1 to take it into account
    // when I allocate the buffer for reading value names.
    maxValueNameLen++;

    // Preallocate a buffer for the value names
    auto nameBuffer = std::make_unique<wchar_t[]>(maxValueNameLen);

    // The value names and types will be stored here
    std::vector<std::pair<std::string, RegistryValueType>> valueInfo;

    // Enumerate all the values
    for(DWORD index = 0; index < valueCount; index++) {
      // Get the name and the type of the current value
      DWORD valueNameLen = maxValueNameLen;
      DWORD valueType {};
      retCode = ::RegEnumValue(_hKey, //
                               index, //
                               nameBuffer.get(), //
                               &valueNameLen, //
                               nullptr, // reserved
                               &valueType,
                               nullptr, // no data
                               nullptr // no data size
      );
      if(retCode != ERROR_SUCCESS) {
        throw Exceptions::RegistryException("Cannot enumerate values: RegEnumValue failed.", retCode);
      }

      // On success, the RegEnumValue API writes the length of the
      // value name in the valueNameLen output parameter
      // (not including the terminating NUL).
      // So we can build a wstring based on that.
      std::string       subkey = commons::utf8convert::Utf16ToUtf8(std::wstring(nameBuffer.get(), valueNameLen));
      RegistryValueType type   = ValueType::Handle(valueType);
      valueInfo.push_back(std::make_pair(subkey, type));
    }

    return valueInfo;
  }

  void RegistryKey::EnableReflectionKey() {
    const auto retCode = ::RegEnableReflectionKey(_hKey);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegEnableReflectionKey failed.", retCode);
    }
  }

  void RegistryKey::DisableReflectionKey() {
    const auto retCode = ::RegDisableReflectionKey(_hKey);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegDisableReflectionKey failed.", retCode);
    }
  }

  bool RegistryKey::QueryReflectionKey() {
    BOOL       isReflectionDisabled = FALSE;
    const auto retCode              = ::RegQueryReflectionKey(_hKey, &isReflectionDisabled);
    if(retCode != ERROR_SUCCESS) {
      throw Exceptions::RegistryException("RegQueryReflectionKey failed.", retCode);
    }

    return (isReflectionDisabled ? true : false);
  }

  void RegistryKey::Dispose(bool disposing) {

    if(IsValid()) {
      // System keys should never be closed.  However, we want to call RegCloseKey
      // on HKEY_PERFORMANCE_DATA when called from PerformanceCounter.CloseSharedResources
      // (i.e. when disposing is true) so that we release the PERFLIB cache and cause it
      // to be refreshed (by re-reading the registry) when accessed subsequently.
      // This is the only way we can see the just installed perf counter.
      // NOTE: since HKEY_PERFORMANCE_DATA is process wide, there is inherent ---- in closing
      // the key asynchronously. While Vista is smart enough to rebuild the PERFLIB resources
      // in this situation the down level OSes are not. We have a small window of ---- between
      // the dispose below and usage elsewhere (other threads). This is By Design.
      // This is less of an issue when OS > NT5 (i.e Vista & higher), we can close the perfkey
      // (to release & refresh PERFLIB resources) and the OS will rebuild PERFLIB as necessary.
      if(!IsSystemKey() || (disposing && IsPerfDataKey())) {
        const auto retCode = ::RegCloseKey(_hKey);
        if(retCode != ERROR_SUCCESS)
          throw Exceptions::RegistryException("RegCloseKey failed.", retCode);

        // Avoid dangling references
        _hKey = nullptr;
      }
    }
  }

  bool RegistryKey::IsSystemKey() const {
    if((_hKey == HKEY_CURRENT_USER) || (_hKey == HKEY_LOCAL_MACHINE) || (_hKey == HKEY_CLASSES_ROOT) || (_hKey == HKEY_CURRENT_CONFIG) ||
       (_hKey == HKEY_CURRENT_USER_LOCAL_SETTINGS) || (_hKey == HKEY_PERFORMANCE_DATA) || (_hKey == HKEY_PERFORMANCE_NLSTEXT) ||
       (_hKey == HKEY_PERFORMANCE_TEXT) || (_hKey == HKEY_USERS))
      return true;

    return false;
  }

  bool RegistryKey::IsPerfDataKey() const {
    if(_hKey == HKEY_PERFORMANCE_DATA)
      return true;

    return false;
  }

  bool RegistryKey::IsWritable() const {
    return AccessRights::Handle(_access) == AccessRights::Handle(RegistryAccessRights::Write) || AccessRights::Handle(RegistryAccessRights::AllAccess);
  }

  bool RegistryKey::IsDirty() const {
    return _dirty;
  }

  void RegistryKey::setDirty() {
    _dirty = true;
  }

  void RegistryKey::EnsureNotDisposed() const {
    if(!IsValid()) {
      throw Exceptions::RegistryException(_keyName, "Registry key cannot be null!");
    }
  }

  void RegistryKey::EnsureWriteable() const {
    EnsureNotDisposed();
    if(!IsWritable()) {
      throw Exceptions::RegistryException(_keyName, "Key is not writable!");
    }
  }

  void RegistryKey::ValidateKeyName(std::string& keyName) {
    FixupName(keyName);
    commons::string::InPlace::ltrim(keyName);

    // check keyname
    if(keyName.empty()) {
      throw std::invalid_argument("keyName cannot be empty!");
    }

    // check keyName length
    size_t nextSlash = keyName.find_first_of('\\');
    size_t current   = 0;
    while(nextSlash != -1) {
      if((nextSlash - current) > _maxKeyLength)
        throw std::length_error("keyName is too long!");

      current   = nextSlash + 1;
      nextSlash = keyName.find_first_of('\\', current);
    }

    // check keyName length
    if((keyName.length() - current) > _maxKeyLength)
      throw std::length_error("keyName is too long!");
  }

  std::string& RegistryKey::FixupName(std::string& name) {
    if(name.find_first_of('\\') == std::string::npos)
      return name;

    name = commons::string::replaceAll(name, "\\\\", "\\");
    name = commons::string::ltrim(name, '\\');
    name = commons::string::rtrim(name, '\\');
    return name;
  }

} // namespace registry
} // namespace abscodes
