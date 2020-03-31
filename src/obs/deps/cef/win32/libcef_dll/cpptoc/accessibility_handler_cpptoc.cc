// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=236a2f5e705cde035d9d87a8a6045dccd258a2d0$
//

#include "libcef_dll/cpptoc/accessibility_handler_cpptoc.h"
#include "libcef_dll/ctocpp/value_ctocpp.h"
#include "libcef_dll/shutdown_checker.h"

namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

void CEF_CALLBACK accessibility_handler_on_accessibility_tree_change(
    struct _cef_accessibility_handler_t* self,
    struct _cef_value_t* value) {
  shutdown_checker::AssertNotShutdown();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;
  // Verify param: value; type: refptr_diff
  DCHECK(value);
  if (!value)
    return;

  // Execute
  CefAccessibilityHandlerCppToC::Get(self)->OnAccessibilityTreeChange(
      CefValueCToCpp::Wrap(value));
}

void CEF_CALLBACK accessibility_handler_on_accessibility_location_change(
    struct _cef_accessibility_handler_t* self,
    struct _cef_value_t* value) {
  shutdown_checker::AssertNotShutdown();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;
  // Verify param: value; type: refptr_diff
  DCHECK(value);
  if (!value)
    return;

  // Execute
  CefAccessibilityHandlerCppToC::Get(self)->OnAccessibilityLocationChange(
      CefValueCToCpp::Wrap(value));
}

}  // namespace

// CONSTRUCTOR - Do not edit by hand.

CefAccessibilityHandlerCppToC::CefAccessibilityHandlerCppToC() {
  GetStruct()->on_accessibility_tree_change =
      accessibility_handler_on_accessibility_tree_change;
  GetStruct()->on_accessibility_location_change =
      accessibility_handler_on_accessibility_location_change;
}

// DESTRUCTOR - Do not edit by hand.

CefAccessibilityHandlerCppToC::~CefAccessibilityHandlerCppToC() {
  shutdown_checker::AssertNotShutdown();
}

template <>
CefRefPtr<CefAccessibilityHandler> CefCppToCRefCounted<
    CefAccessibilityHandlerCppToC,
    CefAccessibilityHandler,
    cef_accessibility_handler_t>::UnwrapDerived(CefWrapperType type,
                                                cef_accessibility_handler_t*
                                                    s) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

template <>
CefWrapperType CefCppToCRefCounted<CefAccessibilityHandlerCppToC,
                                   CefAccessibilityHandler,
                                   cef_accessibility_handler_t>::kWrapperType =
    WT_ACCESSIBILITY_HANDLER;
