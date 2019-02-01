//===--- RegistryApi.h ---------------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//


#ifndef REGISTRY_API_INCLUDED
#define REGISTRY_API_INCLUDED

#if _MSC_VER > 1000
#    pragma once
#endif // _MSC_VER > 1000


//
// By default this library is intended to be used as a DLL
// To use it as a static library add REGISTRY_STATIC to your preprocessor list
//


//
// Ensure that Visual Studio is used
//
#if !defined(_MSC_VER)
#    error "This set of tools only works with Visual Studio"
#endif


// Ensure that we build with the multithreaded
// versions of the runtime libraries
#if defined(_MSC_VER) && !defined(_MT)
#    error Must compile with /MD, /MDd, /MT or /MTd
#endif


//
// Ensure that REGISTRY_DLL is default unless REGISTRY_STATIC is defined
//
#if defined(_WIN32) && defined(_DLL)
#    if !defined(REGISTRY_DLL) && !defined(REGISTRY_STATIC)
#        define REGISTRY_DLL
#    endif
#endif


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the REGISTRY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// REGISTRY_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(REGISTRY_DLL)
#    ifdef REGISTRY_EXPORTS
#        define REGISTRY_API __declspec(dllexport)
#    else
#        define REGISTRY_API __declspec(dllimport)
#    endif
#endif


#if !defined(REGISTRY_API)
#    define REGISTRY_API
#endif


//
// Automatically link Registry library.
//
#if defined(_MSC_VER)
#    if defined _WIN64
#        define X64_SUFFIX "_x64"
#    else
#        define X64_SUFFIX ""
#    endif

#    if defined(REGISTRY_DLL)
#        if defined(_DEBUG)
#            define REGISTRY_LIB_SUFFIX X64_SUFFIX "_d"
#        else
#            define REGISTRY_LIB_SUFFIX X64_SUFFIX ""
#        endif
#    elif defined(_DLL)
#        if defined(_DEBUG)
#            define REGISTRY_LIB_SUFFIX X64_SUFFIX "_mdd"
#        else
#            define REGISTRY_LIB_SUFFIX X64_SUFFIX "_md"
#        endif
#    else
#        if defined(_DEBUG)
#            define REGISTRY_LIB_SUFFIX X64_SUFFIX "_mtd"
#        else
#            define REGISTRY_LIB_SUFFIX X64_SUFFIX "_mt"
#        endif
#    endif

#    if !defined(REGISTRY_NO_AUTOLIB) && !defined(REGISTRY_EXPORTS)
#        pragma comment(lib, "Registry" REGISTRY_LIB_SUFFIX ".lib")
#        pragma message("Automatically linking with Registry" REGISTRY_LIB_SUFFIX ".lib")
#    endif
#endif


// Reduce bloat
#if defined(_WIN32) && !defined(WIN32_LEAN_AND_MEAN)
#    define WIN32_LEAN_AND_MEAN
#endif


// Last Windows platform
#include <SDKDDKVer.h>

//
#include <Windows.h> // Windows Platform SDK
#include <crtdbg.h> // _ASSERTE()

//
#include <string>


#endif // REGISTRY_API_INCLUDED
