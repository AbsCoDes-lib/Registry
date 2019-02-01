//===--- RegistryView.h --------------------------------------------------------------------------------*- C++ -*-===//
//
// This source file is part of the Absolute Codes Design open source projects
//
// Copyright (c) 2016-2019 Absolute Codes Design and the project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://raw.githubusercontent.com/AbsCoDes/AbsCoDes.github.io/master/Licence.txt for license information
//
//===-------------------------------------------------------------------------------------------------------------===//


#ifndef REGISTRY_VIEW_INCLUDED
#define REGISTRY_VIEW_INCLUDED


namespace abscodes {
namespace registry {


    ///
    /// Specifies which registry view to target on a 64-bit operating system.
    ///
    enum class RegistryView {
        Default = 0,
        Registry64 = static_cast<int>(KEY_WOW64_64KEY),
        Registry32 = static_cast<int>(KEY_WOW64_32KEY),
    };


    namespace View {

        static int Handle(RegistryView view) {
            {
                switch(view) {
                    case RegistryView::Default: return 0;
                    case RegistryView::Registry64: return KEY_WOW64_64KEY;
                    case RegistryView::Registry32: return KEY_WOW64_32KEY;
                    default: throw std::invalid_argument("Unsupported registry view.");
                }
            }
        }

    } // namespace View


} // namespace registry
} // namespace abscodes


#endif // REGISTRY_VIEW_INCLUDED
