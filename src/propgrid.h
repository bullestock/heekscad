/////////////////////////////////////////////////////////////////////////////
// Name:        propgrid.h
// Purpose:     wxPropertyGrid
// Author:      Jaakko Salli
// Modified by:
// Created:     Sep-25-2004
// RCS-ID:      $Id:
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_PROPGRID_PROPGRID_H__
#define __WX_PROPGRID_PROPGRID_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "propgrid.cpp"
#endif

#include <wx/window.h>
#include <wx/bitmap.h>
#include <wx/dcclient.h>
#include <wx/scrolwin.h>
#include <wx/dynarray.h>
#include <wx/hashmap.h>
#include <wx/variant.h>
#include <wx/longlong.h>
#include <wx/tooltip.h>
#include <wx/datetime.h>

// NB: Do *NOT * remove this.
#if defined(SWIG) || defined(SWIG_TYPE_TABLE)
    #ifndef __WXPYTHON__
        #define __WXPYTHON__
    #endif
#endif

//
// In case DOXYGEN was not specified...
#if !defined(DOXYGEN) && !defined(_WX_WINDOW_H_BASE_) && !defined(SWIG)
    // I've tried to get this auto-defined in Doxygen config, but have not
    // succeeded thus far... so using a hack here.
    #define DOXYGEN
#endif


// Doxygen special includes
#ifdef DOXYGEN
    #include "pg_dox_mainpage.h"
    #include "propdev.h"
#endif


//
// Need to define some things for DOXYGEN
#ifdef DOXYGEN
    #define wxUSE_VALIDATORS        1
    #define wxUSE_DATETIME          1
    #define wxUSE_TOOLTIPS          1
    #define wxUSE_SPINBTN           1
    #define wxUSE_DATEPICKCTRL      1
#endif


//
// Check some wxUSE_FOOs
#ifndef wxUSE_VALIDATORS
    #error "wxUSE_VALIDATORS not defined"
#endif

#ifndef wxUSE_DATETIME
    #error "wxUSE_DATETIME not defined"
#endif

#ifndef wxUSE_DATEPICKCTRL
    #error "wxUSE_DATEPICKCTRL not defined"
#endif


// Uncomment next line to allow wxAdv linkage (required for DatePickerCtrl editor)
//    #define wxPG_ALLOW_WXADV

#if defined(__WXPYTHON__)
    #include <Python.h>

    #undef wxPG_ALLOW_WXADV
    #define wxPG_ALLOW_WXADV
#endif

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxPROPGRID_MAJOR          1
#define wxPROPGRID_MINOR          2
#define wxPROPGRID_RELEASE        10

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_PROPGRID_VERSION(major,minor,release) \
    (wxPROPGRID_MAJOR > (major) || \
    (wxPROPGRID_MAJOR == (major) && wxPROPGRID_MINOR > (minor)) || \
    (wxPROPGRID_MAJOR == (major) && wxPROPGRID_MINOR == (minor) && wxPROPGRID_RELEASE >= (release)))


// Legacy version number
#define wxPG_VERSION        ((wxPROPGRID_MAJOR*1000)+(wxPROPGRID_MINOR*100)+(wxPROPGRID_RELEASE*10))


// -----------------------------------------------------------------------


//
// Here are some platform dependent defines
// NOTE: More in propertygrid.cpp
//

#ifndef SWIG

#if defined(__WXMSW__)
    // tested

    #define wxPG_XBEFORETEXT            5 // space between vertical line and value text
    #define wxPG_XBEFOREWIDGET          1 // space between vertical line and value editor control

    #define wxPG_TEXTCTRLXADJUST        3 // x position adjustment for wxTextCtrl (and like)

    #define wxPG_ICON_WIDTH             9 // comment to use bitmap buttons
    #define wxPG_USE_RENDERER_NATIVE    0 // 1 if wxRendererNative should be employed

    #define wxPG_DOUBLE_BUFFER          1 // 1 to use double-buffer that guarantees flicker-free painting

    #define wxPG_HEAVY_GFX              1 // 1 for prettier appearance

    #define wxPG_SUPPORT_TOOLTIPS       1  // Enable tooltips

#elif defined(__WXGTK__)
    // tested

    #define wxPG_XBEFORETEXT            6 // space between vertical line and value text
    #define wxPG_XBEFOREWIDGET          1 // space between vertical line and value editor control

    #define wxPG_TEXTCTRLXADJUST        3 // x position adjustment for wxTextCtrl (and like)

    #define wxPG_ICON_WIDTH             9 // comment to use bitmap buttons
    #define wxPG_USE_RENDERER_NATIVE    0 // 1 if wxRendererNative should be employed

    #define wxPG_DOUBLE_BUFFER          1 // 1 to use double-buffer that guarantees flicker-free painting

    #define wxPG_HEAVY_GFX              1 // 1 for prettier appearance

    #define wxPG_SUPPORT_TOOLTIPS       1  // Enable tooltips

#elif defined(__WXMAC__)
    // *not* tested

    #define wxPG_XBEFORETEXT            5 // space between vertical line and value text
    #define wxPG_XBEFOREWIDGET          1 // space between vertical line and value editor widget

    #define wxPG_TEXTCTRLXADJUST        3 // x position adjustment for wxTextCtrl (and like)

    #define wxPG_ICON_WIDTH             11  // comment to use bitmap buttons
    #define wxPG_USE_RENDERER_NATIVE    1 // 1 if wxRendererNative should be employed

    #define wxPG_DOUBLE_BUFFER          0 // 1 to use double-buffer that guarantees flicker-free painting

    #define wxPG_HEAVY_GFX              1 // 1 for prettier appearance

    #define wxPG_SUPPORT_TOOLTIPS       1  // Enable tooltips

#else
    // defaults
    // tested on: none.

    #define wxPG_XBEFORETEXT            6 // space between vertical line and value text
    #define wxPG_XBEFOREWIDGET          1 // space between vertical line and value editor widget

    #define wxPG_TEXTCTRLXADJUST        3 // x position adjustment for wxTextCtrl (and like)

    #define wxPG_ICON_WIDTH             9 // comment to use bitmap buttons
    #define wxPG_USE_RENDERER_NATIVE    0 // 1 if wxRendererNative should be employed

    #define wxPG_DOUBLE_BUFFER          0 // 1 to use double-buffer that guarantees flicker-free painting

    #define wxPG_HEAVY_GFX              0 // 1 for prettier appearance

    #define wxPG_SUPPORT_TOOLTIPS       0  // Enable tooltips

#endif // #if defined(__WXMSW__)


#if defined(__WXWINCE__)
    #define wxPG_SMALL_SCREEN       1
    #undef wxPG_DOUBLE_BUFFER
    #define wxPG_DOUBLE_BUFFER      0
#else
    #define wxPG_SMALL_SCREEN       0
#endif

#endif // #ifndef SWIG

// Undefine wxPG_ICON_WIDTH to use supplied xpm bitmaps instead
// (for tree buttons)
//#undef wxPG_ICON_WIDTH

// Need to force disable tooltips?
#if !wxUSE_TOOLTIPS
    #undef wxPG_SUPPORT_TOOLTIPS
    #define wxPG_SUPPORT_TOOLTIPS       0
#endif


// Set 1 to include basic properties ( ~48k in 0.9.9.2 )
#define wxPG_INCLUDE_BASICPROPS         1

// Set 1 to include advanced properties (wxFontProperty, wxColourProperty, etc.) ( ~32k in 0.9.9.2 )
#define wxPG_INCLUDE_ADVPROPS           1

// Set 1 include wxPropertyGridManager ( ~36k in 0.9.9.1 )
//#define wxPG_INCLUDE_MANAGER            1

// Set 1 to include checkbox editor class ( ~4k in 0.9.9.1 )
#define wxPG_INCLUDE_CHECKBOX           1

// 1 to allow user data for each property
#define wxPG_USE_CLIENT_DATA            1

// NOTE: This is not supported as 1.
#define wxPG_INCLUDE_WXOBJECT           0

// -----------------------------------------------------------------------

#ifdef wxPG_COMPATIBILITY_1_0_0

    #define wxRECURSE               wxPG_RECURSE
    #define wxKEEP_STRUCTURE        wxPG_KEEP_STRUCTURE
    #define wxPGConstants           wxPGChoices
    #define wxPG_EX_CLASSIC_SPACING 0

    #define wxPGCtrlClass           wxWindow
    #define wxCCustomTextCtrl       wxTextCtrl
    #define wxCCustomComboBox       wxPGOwnerDrawnComboBox
    #define wxCCustomButton         wxButton

#endif // #ifdef wxPG_COMPATIBILITY_1_0_0

#ifdef __WXPYTHON__
    #define wxPG_PGVARIANT_IS_VARIANT   1  // 1
    #define wxPG_VALUETYPE_IS_STRING    0  // 1
#else
    #define wxPG_PGVARIANT_IS_VARIANT   0
    #define wxPG_VALUETYPE_IS_STRING    0
#endif


#ifndef SWIG
    #if !wxCHECK_VERSION(2,9,0)
        #if !defined(wxUniChar)
            #define wxUniChar   wxChar
        #endif
        //#define wxPGGetIterChar(str, i)  str[i]
        #define wxPGGetIterChar(str, i)  *i
    #else
        #define wxPGGetIterChar(str, i)  *i
    #endif
#endif


// -----------------------------------------------------------------------

//
// wxPython special considerations
//
// TODO: Using wxPG_GETVALUE_CONST yields some ugly function
//   names, so might as well make those GetPropertyValueAsXXX non-static
//   for regular C++ build as well.s
//

#if !wxPG_VALUETYPE_IS_STRING
    #define wxPG_VALUETYPE_MSGVAL       const wxPGValueType*
#else
    #define wxPG_VALUETYPE_MSGVAL       wxString
#endif


#ifndef __WXPYTHON__

// Some Strings are returned as const wxChar* in C++, and as wxString in wxPython
// (using just wxString for everything would've been better, but the current scheme
// is necessary for better backwards compatibility).
#define wxPG_CONST_WXCHAR_PTR       const wxChar*
#define wxPG_CONST_WXCHAR_DEFVAL    ((const wxChar*)NULL)
#define wxPG_TO_WXCHAR_PTR(A)       A

#define wxPG_PYTHON_STATIC    static
#define wxPG_GETVALUE_CONST

// In C++ we can stick with overloaded methods
#define SetPropertyValueLong        SetPropertyValue
#define SetPropertyValueDouble      SetPropertyValue
#define SetPropertyValueBool        SetPropertyValue
#define SetPropertyValueString      SetPropertyValue
#define SetPropertyValueWxObjectPtr SetPropertyValue
#define SetPropertyValuePoint       SetPropertyValue
#define SetPropertyValueSize        SetPropertyValue
#define SetPropertyValueArrint2     SetPropertyValue
#define SetPropertyValueArrstr2     SetPropertyValue
#define SetPropertyValueDatetime    SetPropertyValue
#define SetPropertyValueLongLong    SetPropertyValue
#define SetPropertyValueULongLong   SetPropertyValue

#else

// Some Strings are returned as const wxChar* in C++, and as wxString in wxPython
// (using just wxString for everything would've been better, but the current scheme
// is necessary for better backwards compatibility).
#define wxPG_CONST_WXCHAR_PTR       wxString
#define wxPG_CONST_WXCHAR_DEFVAL    wxEmptyString
#define wxPG_TO_WXCHAR_PTR(A)       (A.c_str())

// Declaring GetValues as static will yield problems
#define wxPG_PYTHON_STATIC
#define wxPG_GETVALUE_CONST   const

// Because SWIG has problems combining overloaded functions and
// Python object-to-wxXXX conversion, we need to use Python proxy
// functions for these value setters.
#define SetPropertyValueArrstr2     _SetPropertyValueArrstr
#define SetPropertyValueArrint2     _SetPropertyValueArrint

#endif


// wxPG_CHECK_FOO_DBG - on Release and wxPython builds, show wxLogWarning instead
// (so that the program flow is not interrupted, but the message can still be seen).
#if !defined(__WXDEBUG__) || defined(__WXPYTHON__)
    #define wxPG_CHECK_RET_DBG(A,B) \
        if ( !(A) ) { wxLogWarning(B); return; }
    #define wxPG_CHECK_MSG_DBG(A,B,C) \
        if ( !(A) ) { wxLogWarning(C); return B; }
#else
    #define wxPG_CHECK_RET_DBG(A,B)     wxCHECK_RET(A,B)
    #define wxPG_CHECK_MSG_DBG(A,B,C)   wxCHECK_MSG(A,B,C)
#endif


// -----------------------------------------------------------------------

// Our very custom dynamic object macros. Should only be used
// directly in an abstract (typeless etc.) base property classes.
#if wxPG_INCLUDE_WXOBJECT
    #error "wxPG_INCLUDE_WXOBJECT is not currently supported (as of 1.0.0b)."
    #define WX_PG_DECLARE_GETCLASSNAME
    #define WX_PG_DECLARE_GETCLASSINFO
#else
    #define WX_PG_DECLARE_GETCLASSNAME   virtual wxPG_CONST_WXCHAR_PTR GetClassName() const;
    #define WX_PG_DECLARE_GETCLASSINFO   virtual const wxPGPropertyClassInfo* GetClassInfo() const;
#endif

// -----------------------------------------------------------------------

#ifdef WXMAKINGLIB_PROPGRID
    #define WXDLLIMPEXP_PG
#elif defined(WXMAKINGDLL_PROPGRID)
    #define WXDLLIMPEXP_PG WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_PG //WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_PG
#endif

// -----------------------------------------------------------------------

#if wxPG_PGVARIANT_IS_VARIANT
    #define wxPGVariant wxVariant
#else
    class WXDLLIMPEXP_PG wxPGVariant;
#endif

#ifndef SWIG
class WXDLLIMPEXP_PG wxPGValueType;
class WXDLLIMPEXP_PG wxPGEditor;
class WXDLLIMPEXP_PG wxPGProperty;
class WXDLLIMPEXP_PG wxPGPropertyWithChildren;
class WXDLLIMPEXP_PG wxPropertyCategoryClass;
class WXDLLIMPEXP_PG wxPGChoices;
class WXDLLIMPEXP_PG wxPropertyGridState;
class WXDLLIMPEXP_PG wxPropertyContainerMethods;
class WXDLLIMPEXP_PG wxPropertyGrid;
class WXDLLIMPEXP_PG wxPropertyGridEvent;
class WXDLLIMPEXP_PG wxPropertyGridManager;
class WXDLLIMPEXP_PG wxPGOwnerDrawnComboBox;
class WXDLLIMPEXP_PG wxPGCustomComboControl;

struct wxPGPaintData;

extern WXDLLIMPEXP_PG const wxChar *wxPropertyGridNameStr;

#endif // #ifndef SWIG


#ifdef __WXPYTHON__
    class wxPGPyEditor;
#endif // #ifndef __WXPYTHON__


/** @defgroup miscellaneous wxPropertyGrid Miscellanous
    This section describes some miscellanous values, types and macros.
    @{
*/

#if !defined(SWIG)
    #define wxPG_LABEL              wxString()  // Used to tell wxPGProperty to use label as name as well.
    #define wxPG_NULL_BITMAP        wxNullBitmap
    #define wxPG_COLOUR_BLACK       (*wxBLACK)
#else
    #define wxPG_LABEL              wxString_wxPG_LABEL
    #define wxPG_NULL_BITMAP        wxBitmap_NULL
    #define wxPG_COLOUR_BLACK       wxColour_BLACK
#endif // #ifndef SWIG


// Used to indicate wxPGChoices::Add etc that the value is actually not given
// by the caller.
#define wxPG_INVALID_VALUE      INT_MAX


/** Convert Red, red and Blue to a single 32-bit value.
*/
#define wxPG_COLOUR(R,G,B) ((wxUint32)(R+(G<<8)+(B<<16)))

/** Return this in GetImageSize() to indicate that the custom painted
    property image is flexible. That is, it will paint (dropdown)
    list item images with PREFWID,PREFHEI size.
*/
#define wxPG_FLEXIBLE_SIZE(PREFWID,PREFHEI) wxSize(-(PREFWID),-(PREFHEI))

#define wxPG_FULL_CUSTOM_PAINT_WIDTH        -99999

/** Return this in GetImageSize() to indicate that the property is
    custom painted completely (ie. the text as well).
*/
#define wxPG_FULL_CUSTOM_PAINT_SIZE(HEI)                \
    wxSize(wxPG_FULL_CUSTOM_PAINT_WIDTH,HEI)

/** Return this in GetImageSize() to indicate that the property is
    custom painted completely (ie. the text as well), and with flexible
    height.
*/
#define wxPG_FULL_CUSTOM_PAINT_FLEXIBLE_SIZE(PREFHEI)    \
    wxSize(wxPG_FULL_CUSTOM_PAINT_WIDTH,-(PREFHEI))


/** This callback function is used by atleast wxCustomProperty
    to facilitiate easy custom action on button press.
    \param propGrid
    related wxPropertyGrid
    \param property
    related wxPGProperty
    \param ctrl
    If not NULL (for example, not selected), a wxWindow* or equivalent
    \param data
    Value depends on the context.
    \retval
    True if changed value of the property.
*/
typedef bool (*wxPropertyGridCallback)(wxPropertyGrid* propGrid,
                                       wxPGProperty* property,
                                       wxWindow* ctrl,
                                       int data);

/** This callback function is used by atleast wxCustomProperty
    to facilitiate drawing items in drop down list.

    Works very much like wxPGProperty::OnCustomPaint.
*/
typedef void (*wxPGPaintCallback)(wxPGProperty* property,
                                  wxDC& dc,
                                  const wxRect& rect,
                                  wxPGPaintData& paintdata);

/** Use this with wxPropertyGrid::IsPropertyKindOf. For example, as in
    \code
        pg->IsPropertyKindOf(WX_PG_CLASSINFO(wxStringProperty))
    \endcode
*/
#define WX_PG_CLASSINFO(NAME) NAME##ClassInfo

/** @}
*/

// -----------------------------------------------------------------------

/** @defgroup wndflags wxPropertyGrid Window Styles
    SetWindowStyleFlag method can be used to modify some of these at run-time.
    @{
*/
/** This will cause Sort() automatically after an item is added.
    When inserting a lot of items in this mode, it may make sense to
    use Freeze() before operations and Thaw() afterwards to increase
    performance.
*/
#define wxPG_AUTO_SORT              0x00000010

/** Categories are not initially shown (even if added).
    IMPORTANT NOTE: If you do not plan to use categories, then this
    style will waste resources.
    This flag can also be changed using wxPropertyGrid::EnableCategories method.
*/
#define wxPG_HIDE_CATEGORIES        0x00000020

/* This style combines non-categoric mode and automatic sorting.
*/
#define wxPG_ALPHABETIC_MODE        (wxPG_HIDE_CATEGORIES|wxPG_AUTO_SORT)

/** Modified values are shown in bold font. Changing this requires Refresh()
    to show changes.
*/
#define wxPG_BOLD_MODIFIED          0x00000040

/** When wxPropertyGrid is resized, splitter moves to the center. This
    behaviour stops once the user manually moves the splitter.
*/
#define wxPG_SPLITTER_AUTO_CENTER   0x00000080

/** Display tooltips for cell text that cannot be shown completely. If
    wxUSE_TOOLTIPS is 0, then this doesn't have any effect.
*/
#define wxPG_TOOLTIPS               0x00000100

/** Disables margin and hides all expand/collapse buttons that would appear
    outside the margin (for sub-properties). Toggling this style automatically
    expands all collapsed items.
*/
#define wxPG_HIDE_MARGIN            0x00000200

/** This style prevents user from moving the splitter.
*/
#define wxPG_STATIC_SPLITTER        0x00000400

/** Combination of other styles that make it impossible for user to modify
    the layout.
*/
#define wxPG_STATIC_LAYOUT          (wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER)

/** Disables wxTextCtrl based editors for properties which
    can be edited in another way. Equals calling wxPropertyGrid::LimitPropertyEditing
    for all added properties.
*/
#define wxPG_LIMITED_EDITING        0x00000800

#ifdef DOXYGEN

/** wxTAB_TRAVERSAL allows using Tab/Shift-Tab to travel between properties
    in grid. Travelling forwards from last property will navigate to the
    next control, and backwards from first will navigate to the previous one.
*/
    #define wxTAB_TRAVERSAL         0x00080000

#endif

/** wxPropertyGridManager only: Show toolbar for mode and page selection. */
#define wxPG_TOOLBAR                0x00001000

/** wxPropertyGridManager only: Show adjustable text box showing description
    or help text, if available, for currently selected property.
*/
#define wxPG_DESCRIPTION            0x00002000

/** wxPropertyGridManager only: Show compactor button that toggles hidden
    state of low-priority properties.
*/
#define wxPG_COMPACTOR              0x00004000

/**
    NOTE: wxPG_EX_xxx are extra window styles and must be set using SetExtraStyle()
    member function.

    Speeds up switching to wxPG_HIDE_CATEGORIES mode. Initially, if wxPG_HIDE_CATEGORIES
    is not defined, the non-categorized data storage is not activated, and switching
    the mode first time becomes somewhat slower. wxPG_EX_INIT_NOCAT activates the
    non-categorized data storage right away. IMPORTANT NOTE: If you do plan not
    switching to non-categoric mode, or if you don't plan to use categories at
    all, then using this style will result in waste of resources.

*/
#define wxPG_EX_INIT_NOCAT          0x00001000

/** Extended window style that sets wxPropertyGridManager toolbar to not
    use flat style.
*/
#define wxPG_EX_NO_FLAT_TOOLBAR     0x00002000

/** This extra style allows editing more similar to some Microsoft/Mono
    provided property sheet controls. Currently this includes (but more may be
    added later, incase I missed something):
    * Pressing ENTER in control, in addition to confirming changes, will
      unfocus it.
    * Pressing ESC doesn't cancel edit (but still unfocuses the editor).

    Note that ESC and ENTER events in editor controls are relayed to the
    wxPropertyGrid itself, so that they can be detected by the application.
*/
//#define wxPG_EX_ALTERNATE_KEYS      0x00004000

/** Shows alphabetic/categoric mode buttons from toolbar.
*/
#define wxPG_EX_MODE_BUTTONS        0x00008000

/** Show property help strings as tool tips instead as text on the status bar.
    You can set the help strings using SetPropertyHelpString member function.
*/
#define wxPG_EX_HELP_AS_TOOLTIPS    0x00010000

/** Prevent TAB from focusing to wxButtons. This behaviour was default
    in version 1.2.0 and earlier.
    NOTE! Tabbing to button doesn't work yet. Problem seems to be that on wxMSW
      atleast the button doesn't properly propagate key events (yes, I'm using
      wxWANTS_CHARS).
*/
//#define wxPG_EX_NO_TAB_TO_BUTTON    0x00020000

/** Set this style to have labels of disabled properties become greyed
    along with the values.
*/
#define wxPG_EX_GREY_LABEL_WHEN_DISABLED    0x00040000


/** Allows relying on native double-buffering.
*/
#define wxPG_EX_NATIVE_DOUBLE_BUFFERING     0x00080000


/** Process all events immediately, if possible. That is, ProcessEvent is
    called instead of AddPendingEvent.
*/
#define wxPG_EX_PROCESS_EVENTS_IMMEDIATELY  0x00100000


/** Set this style to let user have ability to set values of properties to
    unspecified state. Currently, this applies to following properties:
    - wxIntProperty, wxUIntProperty, and wxFloatProperty: Clear the
      text field.
*/
#define wxPG_EX_AUTO_UNSPECIFIED_VALUES     0x00200000


/** Combines various styles.
*/
#define wxPG_DEFAULT_STYLE	        (0)

/** Combines various styles.
*/
#define wxPGMAN_DEFAULT_STYLE	    (0)

/** @}
*/

/** Flags for wxPropertyGrid::GetPropertyValues and wxPropertyGridManager::GetPropertyValues. */
#define wxPG_KEEP_STRUCTURE               0x00000010

/** Flags for wxPropertyGrid::SetPropertyAttribute etc */
#define wxPG_RECURSE                      0x00000020
#define wxPG_RECURSE_STARTS               0x00000040
#define wxPG_FORCE                        0x00000080

// -----------------------------------------------------------------------

// Property priorities
#define wxPG_LOW                    1
#define wxPG_HIGH                   2

// -----------------------------------------------------------------------

// Misc argument flags.
#define wxPG_FULL_VALUE             0x00000001 // Get/Store full value instead of displayed value.
#define wxPG_REPORT_ERROR           0x00000002
#define wxPG_PROPERTY_SPECIFIC      0x00000004
#define wxPG_EDITABLE_VALUE         0x00000008 // Get/Store value that must be editable in wxTextCtrl

// -----------------------------------------------------------------------


#if defined(__WXPYTHON__)
    #define wxPG_ID_IS_PTR      1
#else
    #define wxPG_ID_IS_PTR      0
#endif


#if wxPG_ID_IS_PTR

#define wxNullProperty  ((wxPGProperty*)NULL)
#define wxPGId          wxPGProperty*
#define wxPGIdGen(PTR)  PTR
#define wxPGIdToPtr(ID) ((wxPGProperty*)ID)
#define wxPGIdIsOk(ID)  ( ID != ((wxPGProperty*)NULL) )

#else

#define wxNullProperty wxPGId(NULL)


/** \class wxPGId
    \ingroup classes
    \brief
    Simple wrapper for the wxPGProperty pointer.

    NB: This class exists because:
        - Abstract wxPGId would allow both flexibility and speed
          (for possible native'ish implementations, altough this doesn't make
          sense anymore).
        - wxPG methods should be mostly used for property manipulation
          (or such vision I had at first), and since wxPGId id = pg->Append(...)
          is faster tow write, it seemed useful.

    *However* in future I may just start using wxPG_ID_IS_PTR by the default.
    It might even result into slightly smaller code (altough I have checked out
    some MSVC generated assembly, and it seems to optimize out the wrapper in
    usual scenarios).
*/
class WXDLLIMPEXP_PG wxPGId
{
public:
    inline wxPGId() { m_ptr = (wxPGProperty*) NULL; }
    ~wxPGId() {}

    bool IsOk() const { return ( m_ptr != NULL ); }

    bool operator == (const wxPGId& other)
    {
        return m_ptr == other.m_ptr;
    }

    inline const wxString& GetName() const;

#ifndef SWIG
    inline wxPGId( wxPGProperty* ptr ) { m_ptr = ptr; }
    inline wxPGId( wxPGProperty& ref ) { m_ptr = &ref; }

    operator wxPGProperty* ()
    {
        return m_ptr;
    }
    wxPGProperty* GetPropertyPtr() const { return m_ptr; }
#endif // #ifndef SWIG

    wxPGProperty& GetProperty() const { return *m_ptr; }
private:
    wxPGProperty* m_ptr;
};

#define wxPGIdGen(PTR) wxPGId(PTR)
#define wxPGIdToPtr(ID) ID.GetPropertyPtr()
#define wxPGIdIsOk(ID) ID.IsOk()

#endif // wxPG_ID_IS_PTR


// -----------------------------------------------------------------------

WXDLLIMPEXP_PG void wxPGTypeOperationFailed ( const wxPGProperty* p, const wxChar* typestr, const wxChar* op );
WXDLLIMPEXP_PG void wxPGGetFailed ( const wxPGProperty* p, const wxChar* typestr );

// -----------------------------------------------------------------------

/** @defgroup propflags wxPGProperty Flags
    @{
*/

// NOTE: Do not change order of these, and if you add
//   any, remember also to update gs_property_flag_to_string
//   in propgrid.cpp.

/** Indicates bold font.
*/
#define wxPG_PROP_MODIFIED          0x0001

/** Disables ('greyed' text and editor does not activate) property.
*/
#define wxPG_PROP_DISABLED          0x0002

/** Hider button will hide this property.
*/
#define wxPG_PROP_HIDEABLE          0x0004

/** This property has custom paint image just in front of its value.
    If property only draws custom images into a popup list, then this
    flag should not be set.
*/
#define wxPG_PROP_CUSTOMIMAGE       0x0008

/** Do not create text based editor for this property (but button-triggered
    dialog and choice are ok).
*/
#define wxPG_PROP_NOEDITOR          0x0010

/** Value is unspecified.
*/
#define wxPG_PROP_UNSPECIFIED       0x0020

/** Indicates the bit useable by derived properties.
*/
#define wxPG_PROP_CLASS_SPECIFIC_1  0x0040

/** Indicates the bit useable by derived properties.
*/
#define wxPG_PROP_CLASS_SPECIFIC_2  0x0080

/** Property value cannot be modified. However, editor may be created
    so that the value can be easily copied.
*/
#define wxPG_PROP_READONLY          0x0100

/** @}
*/

// Amalgam of flags that should be inherited by sub-properties
#define wxPG_INHERITED_PROPFLAGS        (wxPG_PROP_HIDEABLE|wxPG_PROP_NOEDITOR)


// -----------------------------------------------------------------------

/** @defgroup attrids wxPropertyGrid Property Attribute Identifiers
    wxPropertyGrid::SetPropertyAttribute accepts one of these as
    attrid argument when used with one of the built-in property classes.
    @{
*/

/** wxBoolProperty specific, int, default 0. When 1 sets bool property to
    use checkbox instead of choice.
*/
#define wxPG_BOOL_USE_CHECKBOX              64

/** wxBoolProperty specific, int, default 0. When 1 sets bool property value
    to cycle on double click (instead of showing the popup listbox).
*/
#define wxPG_BOOL_USE_DOUBLE_CLICK_CYCLING  65

/** wxFloatProperty (and similar) specific, int, default -1. Sets the (max) precision
    used when floating point value is rendered as text. The default -1 means infinite
    precision.
*/
#define wxPG_FLOAT_PRECISION                66

/** The text will be echoed as asterisks (wxTE_PASSWORD will be passed to textctrl etc).
*/
#define wxPG_STRING_PASSWORD                67

/** Define base used by a wxUIntProperty. Valid constants are
    wxPG_BASE_OCT, wxPG_BASE_DEC, wxPG_BASE_HEX and wxPG_BASE_HEXL
    (lowercase characters).
*/
#define wxPG_UINT_BASE                      68

/** Define prefix rendered to wxUIntProperty. Accepted constants
    wxPG_PREFIX_NONE, wxPG_PREFIX_0x, and wxPG_PREFIX_DOLLAR_SIGN.
    <b>Note:</b> Only wxPG_PREFIX_NONE works with Decimal and Octal
    numbers.
*/
#define wxPG_UINT_PREFIX                    69

/** wxFileProperty/wxImageFileProperty specific, wxChar*, default is detected/varies.
    Sets the wildcard used in the triggered wxFileDialog. Format is the
    same.
*/
#define wxPG_FILE_WILDCARD                  70

/** wxFileProperty/wxImageFileProperty specific, int, default 1.
    When 0, only the file name is shown (i.e. drive and directory are hidden).
*/
#define wxPG_FILE_SHOW_FULL_PATH            71

/** Specific to wxFileProperty and derived properties, wxString, default empty.
    If set, then the filename is shown relative to the given path string.
*/
#define wxPG_FILE_SHOW_RELATIVE_PATH        72

/** Specific to wxFileProperty and derived properties, wxString, default is empty.
    Sets the initial path of where to look for files.
*/
#define wxPG_FILE_INITIAL_PATH              73

/** Specific to wxFileProperty and derivatives, wxString, default is empty.
    Sets a specific title for the dir dialog.
*/
#define wxPG_FILE_DIALOG_TITLE              74

/** Specific to wxDirProperty, wxString, default is empty.
    Sets a specific message for the dir dialog.
*/
#define wxPG_DIR_DIALOG_MESSAGE             75

/** Sets displayed date format for wxDateProperty.
*/
#define wxPG_DATE_FORMAT                    76

/** Sets wxDatePickerCtrl window style used with wxDateProperty. Default
    is wxDP_DEFAULT | wxDP_SHOWCENTURY.
*/
#define wxPG_DATE_PICKER_STYLE              77


#ifdef wxPG_COMPATIBILITY_1_0_0

/** wxCustomProperty specific, wxPGEditor*. Set editor control. Editor pointer is stored
    in variable named wxPGEditor_EDITORNAME. So the basic built-in editors are at
    wxPGEditor_TextCtrl, wxPGEditor_Choice, wxPGEditor_ComboBox, wxPGEditor_CheckBox,
    wxPGEditor_TextCtrlAndButton, and wxPGEditor_ChoiceAndButton.
*/
#define wxPG_CUSTOM_EDITOR                  128

/** wxCustomProperty specific, wxBitmap*. Sets a small bitmap. Value must be given as
    pointer and it is then copied. If you give it wxNullBitmap, then the current
    image (if any) is deleted.
*/
#define wxPG_CUSTOM_IMAGE                   129

/** wxCustomProperty specific, void*. Sets callback function (of type wxPropertyGridCallback)
    that is called whenever button is pressed.
*/
#define wxPG_CUSTOM_CALLBACK                130

#endif // wxPG_COMPATIBILITY_1_0_0

/** wxCustomProperty specific, void*. Sets callback function (of type wxPGPaintCallback)
    that is called whenever image in front of property needs to be repainted. This attribute
    takes precedence over bitmap set with wxPG_CUSTOM_IMAGE, and it is only proper way
    to draw images to wxCustomProperty's drop down choices list.
    \remarks
    Callback must handle measure calls (i.e. when rect.x < 0, set paintdata.m_drawnHeight to
    height of item in question).
*/
#define wxPG_CUSTOM_PAINT_CALLBACK          131


/** wxCustomProperty specific, int, default 0. Setting to 1 makes children private, similar to other
    properties with children.
    \remarks
    - Children must be added <b>when this attribute has value 0</b>. Otherwise
      there will be an assertion failure.
    - Changed event occurs on the parent only.
*/
#define wxPG_CUSTOM_PRIVATE_CHILDREN        132


/** wxColourProperty and its kind, int, default 1. Setting this attribute to 0 hides custom
    colour from property's list of choices.
*/
#define wxPG_COLOUR_ALLOW_CUSTOM            151


/** First attribute id that is guaranteed not to be used built-in
    properties.
*/
#define wxPG_USER_ATTRIBUTE                 192

/** @}
*/

//
// Valid constants for wxPG_UINT_BASE attribute
// (long because of wxVariant constructor)
#define wxPG_BASE_OCT                       (long)8
#define wxPG_BASE_DEC                       (long)10
#define wxPG_BASE_HEX                       (long)16
#define wxPG_BASE_HEXL                      (long)32

//
// Valid constants for wxPG_UINT_PREFIX attribute
#define wxPG_PREFIX_NONE                    (long)0
#define wxPG_PREFIX_0x                      (long)1
#define wxPG_PREFIX_DOLLAR_SIGN             (long)2


// -----------------------------------------------------------------------
// Value type.

// Value type declarer, with optional declaration part (with creator function).
#define WX_PG_DECLARE_VALUE_TYPE_WITH_DECL(VALUETYPE,DECL) \
    extern DECL const wxPGValueType *wxPGValueType_##VALUETYPE; \
    extern DECL wxPGValueType* wxPGNewVT##VALUETYPE();

// Value type declarer (with creator function).
#define WX_PG_DECLARE_VALUE_TYPE(VALUETYPE) \
    extern const wxPGValueType *wxPGValueType_##VALUETYPE; \
    wxPGValueType* wxPGNewVT##VALUETYPE();

// Value type declarer, with optional declaration part.
#define WX_PG_DECLARE_VALUE_TYPE_BUILTIN_WITH_DECL(VALUETYPE,DECL) \
    extern DECL const wxPGValueType *wxPGValueType_##VALUETYPE;

// Value type accessor.
#if !wxPG_VALUETYPE_IS_STRING
    #define wxPG_VALUETYPE(T)       wxPGValueType_##T
    #define wxPG_VALUETYPE_PTR(T)   wxPGValueType_##T
#else
    #define wxPG_VALUETYPE(T)       wxT(#T)
    #define wxPG_VALUETYPE_PTR(T)   wxPropertyContainerMethods::GetValueType(wxT(#T))
#endif

// Like wxPG_VALUETYPE, but casts pointer to exact class.
#define wxPG_VALUETYPE_EXACT(T) ((wxPGValueType##VALUETYPE##Class)wxPGValueType##T)

// Declare builtin value types.
WX_PG_DECLARE_VALUE_TYPE_BUILTIN_WITH_DECL(none,WXDLLIMPEXP_PG)
WX_PG_DECLARE_VALUE_TYPE_BUILTIN_WITH_DECL(wxString,WXDLLIMPEXP_PG)
WX_PG_DECLARE_VALUE_TYPE_BUILTIN_WITH_DECL(long,WXDLLIMPEXP_PG)
WX_PG_DECLARE_VALUE_TYPE_BUILTIN_WITH_DECL(bool,WXDLLIMPEXP_PG)
WX_PG_DECLARE_VALUE_TYPE_BUILTIN_WITH_DECL(double,WXDLLIMPEXP_PG)
WX_PG_DECLARE_VALUE_TYPE_BUILTIN_WITH_DECL(void,WXDLLIMPEXP_PG)
WX_PG_DECLARE_VALUE_TYPE_BUILTIN_WITH_DECL(wxArrayString,WXDLLIMPEXP_PG)
#ifdef __WXPYTHON__
    WX_PG_DECLARE_VALUE_TYPE_BUILTIN_WITH_DECL(PyObject,WXDLLIMPEXP_PG)
#endif

//
// With wxWidgets 2.9 and later we demand native C++ RTTI support
#if wxCHECK_VERSION(2,9,0)
    #ifdef wxNO_RTTI
        #error "You need to enable compiler RTTI support when using wxWidgets 2.9.0 or later"
    #endif
    #define WX_PG_DECLARE_DYNAMIC_CLASS_VARIANTDATA(A)
    #define WX_PG_IMPLEMENT_DYNAMIC_CLASS_VARIANTDATA(A, B)
#else
    #define WX_PG_DECLARE_DYNAMIC_CLASS_VARIANTDATA DECLARE_DYNAMIC_CLASS
    #define WX_PG_IMPLEMENT_DYNAMIC_CLASS_VARIANTDATA IMPLEMENT_DYNAMIC_CLASS
#endif

// VDC = wxVariantData Class
#define WX_PG_DECLARE_VALUE_TYPE_VDC(VALUETYPE) \
wxVariantData_##VALUETYPE : public wxPGVariantDataWxObj \
{ \
    WX_PG_DECLARE_DYNAMIC_CLASS_VARIANTDATA(wxVariantData_##VALUETYPE) \
protected: \
    VALUETYPE   m_value; \
public: \
    wxVariantData_##VALUETYPE(); \
    wxVariantData_##VALUETYPE(const VALUETYPE& value); \
    virtual void Copy(wxVariantData& data); \
    virtual bool Eq(wxVariantData& data) const; \
    virtual wxString GetType() const; \
    virtual void* GetValuePtr(); \
    inline const VALUETYPE& GetValue () const { return m_value; }

// Value type declarer for void* that need auto-generated .
#define WX_PG_DECLARE_VALUE_TYPE_VOIDP_WITH_DECL(VALUETYPE,DECL) \
class DECL WX_PG_DECLARE_VALUE_TYPE_VDC(VALUETYPE) \
}; \
extern DECL const wxPGValueType *wxPGValueType_##VALUETYPE;

#define WX_PG_DECLARE_VALUE_TYPE_VOIDP(VALUETYPE) \
class WX_PG_DECLARE_VALUE_TYPE_VDC(VALUETYPE) \
}; \
WX_PG_DECLARE_VALUE_TYPE(VALUETYPE)

#ifndef SWIG

/** \class wxPGVariantDataWxObj
    \ingroup classes
    \brief Identical to wxVariantDataWxObjectPtr except that it deletes the
       ptr on destruction.
*/
class WXDLLIMPEXP_PG wxPGVariantDataWxObj : public wxVariantData
{
public:
    wxPGVariantDataWxObj();
    virtual ~wxPGVariantDataWxObj();

#if wxUSE_STD_IOSTREAM
    virtual bool Write(wxSTD ostream& str) const;
#endif
    virtual bool Write(wxString& str) const;
#if wxUSE_STD_IOSTREAM
    virtual bool Read(wxSTD istream& str);
#endif
    virtual bool Read(wxString& str);

    virtual void* GetValuePtr() = 0;
};

#endif // #ifndef SWIG

#if !wxCHECK_VERSION(2,9,0)
typedef wxList wxVariantList;
#endif

// -----------------------------------------------------------------------
// Editor class.

// Editor accessor.
#define wxPG_EDITOR(T)          wxPGEditor_##T

// Declare editor class, with optional part.
#define WX_PG_DECLARE_EDITOR_WITH_DECL(EDITOR,DECL) \
extern DECL wxPGEditor* wxPGEditor_##EDITOR; \
extern DECL wxPGEditor* wxPGConstruct##EDITOR##EditorClass();

// Declare editor class.
#define WX_PG_DECLARE_EDITOR(EDITOR) \
extern wxPGEditor* wxPGEditor_##EDITOR; \
extern wxPGEditor* wxPGConstruct##EDITOR##EditorClass();

// Declare builtin editor classes.
WX_PG_DECLARE_EDITOR_WITH_DECL(TextCtrl,WXDLLIMPEXP_PG)
WX_PG_DECLARE_EDITOR_WITH_DECL(Choice,WXDLLIMPEXP_PG)
WX_PG_DECLARE_EDITOR_WITH_DECL(ComboBox,WXDLLIMPEXP_PG)
WX_PG_DECLARE_EDITOR_WITH_DECL(TextCtrlAndButton,WXDLLIMPEXP_PG)
#if wxPG_INCLUDE_CHECKBOX
WX_PG_DECLARE_EDITOR_WITH_DECL(CheckBox,WXDLLIMPEXP_PG)
#endif
WX_PG_DECLARE_EDITOR_WITH_DECL(ChoiceAndButton,WXDLLIMPEXP_PG)

// -----------------------------------------------------------------------

/** \class wxPGValueType
	\ingroup classes
    \brief wxPGValueType is base class for property value types.
*/

class WXDLLIMPEXP_PG wxPGValueType
{
public:

    virtual ~wxPGValueType() = 0;

    /** Returns type name. If there is wxVariantData for this type, then name should
    be the same that the class uses (otherwise wxT("void*")). */
    virtual wxPG_CONST_WXCHAR_PTR GetTypeName() const = 0;

    /** Returns custom type name. If this is base for a type, should not be overridden,
        as the default implementation already does good thing and calls GetTypeName.
        Otherwise, should be an unique string, such as the class name etc.
    */
    virtual wxPG_CONST_WXCHAR_PTR GetCustomTypeName() const;

    /** Returns default value.
    */
    virtual wxPGVariant GetDefaultValue() const = 0;

    /** Creates wxVariant with supplied value and name.
    */
    virtual wxVariant GenerateVariant( wxPGVariant value, const wxString& name ) const = 0;

    /** Creates new property instance with "proper" class. Initial value is set
        to default.
    */
    virtual wxPGProperty* GenerateProperty( const wxString& label, const wxString& name ) const = 0;

    /** Sets property value from wxVariant.
    */
    virtual void SetValueFromVariant( wxPGProperty* property, wxVariant& value ) const = 0;

    /** Returns type that can be passed to CreatePropertyByType.
    */
    inline wxPG_CONST_WXCHAR_PTR GetType() const
    {
        return GetCustomTypeName();
    }

protected:
};


// -----------------------------------------------------------------------

// wxVariant definition macro (sans functional eq-operator)
#define WX_PG_DECLARE_VARIANT_DATA(CLASSNAME, DATATYPE, DECL) \
class DECL CLASSNAME : public wxVariantData \
{ \
    DECLARE_DYNAMIC_CLASS(CLASSNAME) \
public: \
    CLASSNAME() { } \
    CLASSNAME(const DATATYPE& value) { m_value = value; } \
    inline DATATYPE GetValue() const { return m_value; } \
    inline const DATATYPE& GetValueRef() const { return m_value; } \
    inline void SetValue(const DATATYPE& value) { m_value = value; } \
    virtual bool Eq(wxVariantData&) const { return false; } \
    virtual wxString GetType() const { return wxT(#DATATYPE); } \
    virtual wxVariantData* Clone() { return new CLASSNAME; } \
    virtual void Copy(wxVariantData &data) { ((CLASSNAME&)data).SetValue(m_value); } \
    virtual bool Read(wxString &) { return false; } \
    virtual bool Write(wxString &) const { return true; } \
protected: \
    DATATYPE m_value; \
};


#define WX_PG_DECLARE_VARIANT_DATA_PTR(CLASSNAME, DATATYPE, DECL) \
class DECL CLASSNAME : public wxVariantData \
{ \
    DECLARE_DYNAMIC_CLASS(CLASSNAME) \
public: \
    CLASSNAME() { } \
    CLASSNAME(DATATYPE* value) { m_value = value; } \
    inline void SetValue(DATATYPE* value) { m_value = value; } \
    inline DATATYPE* GetValue() const { return m_value; } \
    virtual bool Eq(wxVariantData&) const { return false; } \
    virtual wxString GetType() const { return wxT(#DATATYPE); } \
    virtual wxVariantData* Clone() { return new CLASSNAME; } \
    virtual void Copy(wxVariantData &data) { ((CLASSNAME&)data).SetValue(m_value); } \
    virtual bool Read(wxString &) { return false; } \
    virtual bool Write(wxString &) const { return true; } \
protected: \
    DATATYPE* m_value; \
};


#if wxPG_PGVARIANT_IS_VARIANT

    #define wxPGVariantToWxObjectPtr(A,B)   wxDynamicCast(A.GetWxObjectPtr(),B);
    #define wxPGVariantGetWxObjectPtr(A)    A.GetWxObjectPtr()
    #define wxPGVariantToWxObject(A)        (*A.GetWxObjectPtr())
    #define wxPGVariantToDateTime(A)        A.GetDateTime()
    #define wxPGVariantToWxPoint(A)         ((wxPGVariantDataPoint*)(A.GetData()))->GetValueRef()
    #define wxPGVariantToWxSize(A)          ((wxPGVariantDataSize*)(A.GetData()))->GetValueRef()
    #define wxPGVariantToWxLongLong(A)      ((wxPGVariantDataLongLong*)(A.GetData()))->GetValueRef()
    #define wxPGVariantToWxULongLong(A)     ((wxPGVariantDataULongLong*)(A.GetData()))->GetValueRef()
    #define wxPGVariantToArrayInt(A)        ((wxPGVariantDataArrayInt*)(A.GetData()))->GetValueRef()
    #define wxPGVariantToPyObject(A)        ((wxPGVariantDataPyObject*)(A.GetData()))->GetValue()
    #define wxPGVariantFromWxObject(A)      wxVariant((wxObject*)A)
    #define wxPGVariantFromLong(A)          wxVariant(((long)A))

  #if !defined(SWIG)

    WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataPoint, wxPoint, WXDLLIMPEXP_PG)
    WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataSize, wxSize, WXDLLIMPEXP_PG)
    WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataArrayInt, wxArrayInt, WXDLLIMPEXP_PG)
    WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataLongLong, wxLongLong, WXDLLIMPEXP_PG)
    WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataULongLong, wxULongLong, WXDLLIMPEXP_PG)
   #ifdef __WXPYTHON__
    WX_PG_DECLARE_VARIANT_DATA_PTR(wxPGVariantDataPyObject, PyObject, WXDLLIMPEXP_PG)
   #endif

    // We need this because wxVariant lacks all necessary constructors
    inline wxVariant wxPGVariantCreator(long a) { return wxVariant((long)a); }
    inline wxVariant wxPGVariantCreator(int a) { return wxVariant((long)a); }
    inline wxVariant wxPGVariantCreator(bool a) { return wxVariant(a); }
    inline wxVariant wxPGVariantCreator(const double& a) { return wxVariant(a); }
    inline wxVariant wxPGVariantCreator(const wxString& a) { return wxVariant(a); }

    // NB: This may look dangerous. However, the wxVariant lives only a very short
    //     time, so it is very unlikely they value will be modified by some
    //     "third party".
    inline wxVariant wxPGVariantCreator(const wxObject* a) { return wxVariant((wxObject*)a); }
    inline wxVariant wxPGVariantCreator(const wxObject& a) { return wxVariant((wxObject*)(&a)); }

    inline wxVariant wxPGVariantCreator(wxObject* a) { return wxVariant(a); }
    inline wxVariant wxPGVariantCreator(wxObject& a) { return wxVariant(&a); }
    inline wxVariant wxPGVariantCreator(void* a) { return wxVariant(a); }
    inline wxVariant wxPGVariantCreator(const wxArrayString& a) { return wxVariant((wxArrayString&)a); }
    inline wxVariant wxPGVariantCreator(const wxArrayInt& a) { return wxVariant(new wxPGVariantDataArrayInt(a)); }
    inline wxVariant wxPGVariantCreator(const wxPoint& a) { return wxVariant(new wxPGVariantDataPoint(a)); }
    inline wxVariant wxPGVariantCreator(const wxSize& a) { return wxVariant(new wxPGVariantDataSize(a)); }
    inline wxVariant wxPGVariantCreator(const wxLongLong& a) { return wxVariant(new wxPGVariantDataLongLong(a)); }
    inline wxVariant wxPGVariantCreator(const wxULongLong& a) { return wxVariant(new wxPGVariantDataULongLong(a)); }
   #ifdef __WXPYTHON__
    inline wxVariant wxPGVariantCreator(PyObject* a) { return wxVariant(new wxPGVariantDataPyObject(a)); }
   #endif
   #if wxUSE_DATETIME
    inline wxVariant wxPGVariantCreator(const wxDateTime& a) { return wxVariant(a); }
   #endif
  #endif // !defined(SWIG)

#else // !wxPG_PGVARIANT_IS_VARIANT

union wxPGVariantUnion
{
    long        m_long;
    void*       m_ptr;
    bool        m_bool;
};

//
// Very simple value wrapper class
//
// NB: It only holds the pointers for a short period, so don't
//     worry about it not making copies.
class WXDLLIMPEXP_PG wxPGVariant
{
public:

    /** Constructor for none. */
    wxPGVariant()
    {
        m_v.m_ptr = (void*)NULL;
    }
#ifndef SWIG
    /** Constructor for long integer. */
    wxPGVariant( long v_long )
    {
        m_v.m_long = v_long;
    }
    /** Constructor for integer. */
    wxPGVariant( int v_long )
    {
        m_v.m_long = v_long;
    }
    /** Constructor for bool. */
    wxPGVariant( bool value )
    {
        m_v.m_bool = value;
    }
    /** Constructor for float. */
    wxPGVariant( const double& v_ptr )
    {
        m_v.m_ptr = (void*)&v_ptr;
    }
    /** Constructor for wxString*. */
    wxPGVariant( const wxString& v_ptr )
    {
        m_v.m_ptr = (void*)&v_ptr;
    }
    /** Constructor for wxArrayString*. */
    wxPGVariant( const wxArrayString& v_ptr )
    {
        m_v.m_ptr = (void*)&v_ptr;
    }
    /** Constructor for wxArrayInt. */
    wxPGVariant( const wxArrayInt& v_ptr )
    {
        m_v.m_ptr = (void*)&v_ptr;
    }
    /** Constructor for wxPoint. */
    wxPGVariant( const wxPoint& v_ptr )
    {
        m_v.m_ptr = (void*)&v_ptr;
    }
    /** Constructor for wxSize. */
    wxPGVariant( const wxSize& v_ptr )
    {
        m_v.m_ptr = (void*)&v_ptr;
    }
    /** Constructor for wxLongLong. */
    wxPGVariant( const wxLongLong& v_ptr )
    {
        m_v.m_ptr = (void*)&v_ptr;
    }
    /** Constructor for wxULongLong. */
    wxPGVariant( const wxULongLong& v_ptr )
    {
        m_v.m_ptr = (void*)&v_ptr;
    }
    /** Constructor for wxObject&. */
    wxPGVariant( const wxObject& v_ptr )
    {
        m_v.m_ptr = (void*)&v_ptr;
    }
    /** Constructor for wxObject*. */
    wxPGVariant( const wxObject* v_ptr )
    {
        m_v.m_ptr = (void*)v_ptr;
    }
    /** Constructor for void*. */
    wxPGVariant( void* v_ptr )
    {
        m_v.m_ptr = v_ptr;
    }
#if wxUSE_DATETIME
    /** Constructor for wxDateTime ptr. */
    wxPGVariant( const wxDateTime& dt )
    {
        m_v.m_ptr = (void*) &dt;
    }
#endif

    /** Returns value as long integer. */
    inline long GetLong() const
    {
        return m_v.m_long;
    }
    /** Returns value as boolean integer. */
    inline bool GetBool() const
    {
        return m_v.m_bool;
    }
    /** Returns value as floating point number. */
    inline double GetDouble() const
    {
        return *((double*)m_v.m_ptr);
    }
    /** Returns value as floating point number ptr. */
    inline double* GetDoublePtr() const
    {
        return (double*) m_v.m_ptr;
    }
    /** Returns value as a wxString. */
    inline const wxString& GetString() const
    {
        return *((const wxString*)m_v.m_ptr);
    }
    /** Returns value as a reference to a wxArrayString. */
    inline wxArrayString& GetArrayString() const
    {
        wxArrayString* temp = (wxArrayString*)m_v.m_ptr;
        return *temp;
    }

    inline const wxObject& GetWxObject() const
    {
        return *((const wxObject*)m_v.m_ptr);
    }

    inline wxObject* GetWxObjectPtr() const
    {
        return (wxObject*)m_v.m_ptr;
    }

    /** Returns value as void*. */
    inline void* GetVoidPtr() const
    {
        return m_v.m_ptr;
    }
#if wxUSE_DATETIME
    /** Returns value as const wxDateTime&. */
    inline const wxDateTime& GetDateTime() const
    {
        return *((const wxDateTime*)m_v.m_ptr);
    }
#endif

#endif // #ifndef SWIG

    /** Returns value as long integer without type checking. */
    inline long GetRawLong() const { return m_v.m_long; }

    /** Returns value as void* without type checking. */
    inline void* GetRawPtr() const { return m_v.m_ptr; }

#undef wxPG_ASSERT_VARIANT_GET

    /** Value in portable format. */
    wxPGVariantUnion          m_v;
};

    #define wxPGVariantGetWxObjectPtr(A)    ((wxObject*)A.GetRawPtr())
    #define wxPGVariantToWxObjectPtr(A,B)   wxDynamicCast((wxObject*)A.GetRawPtr(),B);
    #define wxPGVariantToWxObject(A)        A.GetWxObject()
    #define wxPGVariantToDateTime(A)        *((wxDateTime*)A.GetVoidPtr())
    #define wxPGVariantToWxPoint(A)         *((wxPoint*)wxPGVariantToVoidPtr(A))
    #define wxPGVariantToWxSize(A)          *((wxSize*)wxPGVariantToVoidPtr(A))
    #define wxPGVariantToWxLongLong(A)      *((wxLongLong*)wxPGVariantToVoidPtr(A))
    #define wxPGVariantToWxULongLong(A)     *((wxULongLong*)wxPGVariantToVoidPtr(A))
    #define wxPGVariantToArrayInt(A)        *((wxArrayInt*)A.GetVoidPtr())
    #define wxPGVariantFromWxObject(A)      *((const wxObject*)A)
    #define wxPGVariantFromLong(A)          wxPGVariant(A)
    #define wxPGVariantCreator              wxPGVariant

#endif // !wxPG_PGVARIANT_IS_VARIANT

// Helper macros
#define wxPGVariantToString(A)      A.GetString()
#define wxPGVariantToLong(A)        A.GetLong()
#define wxPGVariantToBool(A)        A.GetBool()
#define wxPGVariantToDouble(A)      A.GetDouble()
#define wxPGVariantToArrayString(A) A.GetArrayString()
#define wxPGVariantToVoidPtr(A)     A.GetVoidPtr()

#define wxPGVariantFromString(A)        A
#define wxPGVariantFromDouble(A)        A
#define wxPGVariantFromArrayString(A)   A
#define wxPGVariantFromBool(A)          A


// -----------------------------------------------------------------------

//
// Property class declaration helper macros
// (wxPGRootPropertyClass and wxPropertyCategory require this).
//

#define WX_PG_DECLARE_CLASSINFO() \
    WX_PG_DECLARE_GETCLASSNAME \
    WX_PG_DECLARE_GETCLASSINFO

// We don't want to create SWIG interface for DoGetEditorClass (we'll use GetEditor instead)
#ifndef SWIG
    #define WX_PG_DECLARE_DOGETEDITORCLASS  virtual const wxPGEditor* DoGetEditorClass() const;
#else
    #define WX_PG_DECLARE_DOGETEDITORCLASS
#endif

#define WX_PG_DECLARE_PROPERTY_CLASS() \
public: \
    virtual wxPG_VALUETYPE_MSGVAL GetValueType() const; \
    WX_PG_DECLARE_DOGETEDITORCLASS \
    WX_PG_DECLARE_CLASSINFO() \
private:


// Added for SWIG (which apparently doesn't support 0-argument macros)
// NB: Othwise, this is *identical* to WX_PG_DECLARE_PROPERTY_CLASS()
#define WX_PG_DECLARE_PROPERTY_CLASS_NOPARENS \
public: \
    virtual wxPG_VALUETYPE_MSGVAL GetValueType() const; \
    WX_PG_DECLARE_DOGETEDITORCLASS \
    WX_PG_DECLARE_CLASSINFO() \
private:



#ifndef SWIG

// -----------------------------------------------------------------------
// wxPGPropertyClassInfo


typedef wxPGProperty* (*wxPGPropertyConstructor) (const wxString&,const wxString&);

/** \class wxPGPropertyClassInfo
	\ingroup classes
    \brief Class info structure for wxPGProperty derivatives (may be deprecated
    in a future release).
*/
struct wxPGPropertyClassInfo
{
    /** One returned by GetPropertyClassName */
    const wxChar*                   m_name;

    /** Classinfo of the base property class. */
    const wxPGPropertyClassInfo*    m_baseInfo;

    /** Simple property constructor function. */
    wxPGPropertyConstructor         m_constructor;
};


// Use this macro to register your custom property classes.
#define wxPGRegisterPropertyClass(NAME) \
    wxPropertyGrid::RegisterPropertyClass(wxT(#NAME),&NAME##ClassInfo)


// -----------------------------------------------------------------------


// Structure for relaying choice/list info.
struct wxPGChoiceInfo
{
    const wxChar**  m_arrWxChars;
    wxString*       m_arrWxString;
    wxPGChoices*    m_choices;
    int             m_itemCount;
};


// -----------------------------------------------------------------------


/** \class wxPGPropertyDataExt
	\ingroup classes
    \brief wxPGPropertyDataExt is data extension class for wxPGProperty.
*/
class WXDLLIMPEXP_PG wxPGPropertyDataExt
{
public:

    wxPGPropertyDataExt()
    {
#if wxUSE_VALIDATORS
        m_validator = (wxValidator*) NULL;
#endif
        m_customEditor = (wxPGEditor*) NULL;
        m_valueBitmap = (wxBitmap*) NULL;
    }

    ~wxPGPropertyDataExt()
    {
        // Do not delete m_customEditor
#if wxUSE_VALIDATORS
        delete m_validator;
#endif
        delete m_valueBitmap;
    }

    // For conviency, declare all members public.

    wxString                    m_helpString; // Help shown in statusbar or help box.
    const wxPGEditor*           m_customEditor; // Overrides editor returned by property class

#if wxUSE_VALIDATORS
    // NOTE: This is candidate for hash mapping.
    wxValidator*                m_validator; // Editor is going to get this validator
#endif

    wxBitmap*                   m_valueBitmap; // Show this in front of the value
};

#endif

// -----------------------------------------------------------------------

/** \class wxPGProperty
	\ingroup classes
    \brief wxPGProperty, alias wxBasePropertyClass, is base class for properties.
    Information here is provided primarily for anyone who creates new properties,
    since <b>all operations on properties should be done via wxPropertyGrid's or
    wxPropertyGridManager's methods</b>.

    \remarks
    - When changing name of a property, it is essential to use wxPropertyGrid::SetPropertyName
      (that's why there is no SetName method).
*/
class WXDLLIMPEXP_PG wxPGProperty
{
#ifndef SWIG
    friend class wxPGPropertyWithChildren;
    friend class wxPropertyGrid;
    friend class wxPropertyContainerMethods;
    friend class wxPropertyGridState;
#endif
public:

// PYSWIG is a special symbol used by my custom scripts. Code to remove it
// automatically should be added in future.
#ifndef PYSWIG
    /** Basic constructor. Should not be necessary to override.
    */
    wxPGProperty();
#endif

    /** Constructor.
        Real used property classes should have constructor of this style:

        \code

        // If MyValueType is a class, then it should be a constant reference
        // (e.g. const MyValueType& ) instead.
        wxMyProperty( const wxString& label, const wxString& name,
            MyValueType value ) : wxPGProperty
        {
            // Only required if MyValueType is not built-in default
            // (wxString, long, double, bool, and wxArrayString are;
            // wxFont, wxColour, etc. are not).
            wxPG_INIT_REQUIRED_TYPE(MyValueType)
            DoSetValue(value); // Generally recommended way to set the initial value.

            // If has child properties (i.e. wxPGPropertyWithChildren is used
            // as the parent class), then create children here. For example:
            //     AddChild( new wxStringProperty( wxT("Subprop 1"), wxPG_LABEL, value.GetSubProp1() ) );
        }

        \endcode

        Of course, in this example, wxPGProperty could also be wxPGPropertyWithChildren
        (if it has sub-properties) or actually any other property class.
    */
    wxPGProperty( const wxString& label, const wxString& name );

    /** Virtual destructor. It is customary for derived properties to override this. */
    virtual ~wxPGProperty();

    /** Sets property's internal value.
        \param value
        Simple container with GetString(), GetLong() etc. methods. Currently recommended
        means to extract value is to use wxPGVariantToXXX(value) macro.
        \remarks
        Example pseudo-implementation with comments:
        \code

        void wxMyProperty::DoSetValue ( wxPGVariant value )
        {
            // A) Get value. For example
            const wxMyValueType* pvalue = wxPGVariantToWxObjectPtr(value,wxMyValueType);
            //    or:
            const wxString& str = wxPGVariantToString(value);
            //    or:
            long val = wxPGVariantToLong(value);

            // B) If value is wxObject or void based with NULL default, then handle that:
            if ( pvalue )
                m_value = *pvalue;
            else
                pmyvalue->SetToDefault();

            // Otherwise
            // m_value = *pvalue;
            // is sufficient.

            // C) If has children, this should be here (before displaying in control).
            RefreshChildren();

        }

        \endcode
    */
    virtual void DoSetValue( wxPGVariant value );

    /** Returns properly constructed wxPGVariant.
    */
    virtual wxPGVariant DoGetValue() const;

    /** Returns text representation of property's value.
        \param argFlags
        If wxPG_FULL_VALUE is set, returns complete, storable value instead of displayable
        one (they may be different).
    */
    virtual wxString GetValueAsString( int argFlags = 0 ) const;

    /** Converts string to a value, and if succesfull, calls DoSetValue() on it.
        Default behaviour is to do nothing.
        \param text
        String to get the value from.
        \param report_error
        If true, invalid string will be reported (prefer to use wxLogError).
        \retval
        true if value was changed.
    */
    virtual bool SetValueFromString( const wxString& text, int flags = 0 );

    /** Converts integer to a value, and if succesfull, calls DoSetValue() on it.
        Default behaviour is to do nothing.
        \param value
        Int to get the value from.
        \param flags
        If has wxPG_FULL_VALUE, then the value given is a actual value and not an index.
        \retval
        true if value was changed.
    */
    virtual bool SetValueFromInt( long value, int flags = 0 );

    /** Returns size of the custom painted image in front of property. This method
        must be overridden to return non-default value if OnCustomPaint is to be
        called.
        \remarks
        - If flexible image size is desired, return wxPG_FLEXIBLE_SIZE(wid,hei).
          OnCustomPaint is then called to measure items as well (see for
          wxPGProperty::OnCustomPaint for measure call specs).
        - If entire property, including text, is to be custom painted, then
          wxPG_FULL_CUSTOM_PAINT_SIZE(hei) or wxPG_FULL_CUSTOM_PAINT_FLEXIBLE_SIZE(hei)
          is to be returned.
        - Default behaviour is to return wxSize(0,0), which means no image.
        - Default image width or height is indicated with dimension -1.
    */
    virtual wxSize GetImageSize() const;

    /** Events received by editor widgets are processed here. Note that editor class
        usually processes most events. Some, such as button press events of
        TextCtrlAndButton class, should be handled here. Also, if custom handling
        for regular events is desired, then that can also be done (for example,
        wxSystemColourProperty custom handles wxEVT_COMMAND_CHOICE_SELECTED).
        \param event
        Associated wxEvent.
        \retval
        Should return true if any changes in value should be reported. This is case,
        for example, when enter is pressed in wxTextCtrl.
    */
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event );

#if wxPG_INCLUDE_WXOBJECT
    inline wxPG_CONST_WXCHAR_PTR GetClassName() const
    {
        return GetClassInfo()->GetClassName();
    }
#else
    /** Returns classname (for example, "wxStringProperty" for wxStringProperty)
        of a property class.
    */
    virtual wxPG_CONST_WXCHAR_PTR GetClassName() const = 0;
#endif

    /** Returns pointer to the object that has methods related to
        the value type of this property. Keep atleast this method
        abstract so the class is kept abstract.
    */
#ifndef __WXPYTHON__
    virtual const wxPGValueType* GetValueType() const = 0;
#else
  #ifndef SWIG
    virtual const wxPGValueType* GetValueType() const;
  #endif

    // Implement this in Python
    virtual wxString GetType() const;
#endif

#if !wxPG_VALUETYPE_IS_STRING
    const wxPGValueType* GetValueTypePtr() const { return GetValueType(); }
#else
    const wxPGValueType* GetValueTypePtr() const;
#endif

#ifndef SWIG
    /** Returns pointer to an instance of editor class.
    */
    virtual const wxPGEditor* DoGetEditorClass() const;
#endif

#ifdef __WXPYTHON__
    /** Returns name of editor used. Takes precendence in the wxPython bindings.
    */
    virtual wxString GetEditor() const;
#endif

#if wxUSE_VALIDATORS
    /** Returns pointer to the wxValidator that should be used
        with the editor of this property (NULL for no validator).
        Setting validator explicitly via SetPropertyValidator
        will override this.

        In most situations, code like this should work well
        (macros are used to maintain one actual validator instance,
        so on the second call the function exits within the first
        macro):

        \code

        wxValidator* wxMyPropertyClass::DoGetValidator () const
        {
            WX_PG_DOGETVALIDATOR_ENTRY()

            wxMyValidator* validator = new wxMyValidator(...);

            ... prepare validator...

            WX_PG_DOGETVALIDATOR_EXIT(validator)
        }

        \endcode

        \remarks
        You can get common filename validator by returning
        wxFilePropertyClass::GetClassValidator(). wxDirProperty,
        for example, uses it.
    */
    virtual wxValidator* DoGetValidator () const;
#endif // #if wxUSE_VALIDATORS

    /** Returns 0 for normal items. 1 for categories, -1 for other properties with children,
        -2 for wxCustomProperty (mostly like -1 ones but with few expections).
        \remarks
        Should not be overridden by new custom properties. Usually only used internally.
    */
    inline signed char GetParentingType() const { return m_parentingType; }

    /** Returns current value's index to the choice control. May also return,
        through pointer arguments, strings that should be inserted to that control.
        Irrelevant to classes which do not employ wxPG_EDITOR(Choice) or similar.
        \remarks
        - If returns -1 in choiceinfo->m_itemCount, then in that case this
          class be derived from wxBaseEnumPropertyClass (see propdev.h) and
          GetEntry is used to fill an array (slower, but handier if you don't
          store your labels as arrays of strings).
        - Must not crash even if property's set of choices is uninitialized
          (i.e. it points to wxPGGlobalVars->m_emptyConstants).
    */
    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );

    /** Override to paint an image in front of the property value text or drop-down
        list item (but only if wxPGProperty::GetImageSize is overridden as well).

        If property's GetImageSize() returns size that has height != 0 but less than
        row height ( < 0 has special meanings), wxPropertyGrid calls this method to
        draw a custom image in a limited area in front of the editor control or
        value text/graphics, and if control has drop-down list, then the image is
        drawn there as well (even in the case GetImageSize() returned higher height
        than row height).

        NOTE: Following applies when GetImageSize() returns a "flexible" height (
        using wxPG_FLEXIBLE_SIZE(W,H) macro), which implies variable height items:
        If rect.x is < 0, then this is a measure item call, which means that
        dc is invalid and only thing that should be done is to set paintdata.m_drawnHeight
        to the height of the image of item at index paintdata.m_choiceItem. This call
        may be done even as often as once every drop-down popup show.

        \param dc
        wxDC to paint on.
        \param rect
        Box reserved for custom graphics. Includes surrounding rectangle, if any.
        If x is < 0, then this is a measure item call (see above).
        \param paintdata
        wxPGPaintData structure with much useful data.

        \remarks
            - You can actually exceed rect width, but if you do so then paintdata.m_drawnWidth
              must be set to the full width drawn in pixels.
            - Due to technical reasons, rect's height will be default even if custom height
              was reported during measure call.
            - Changing font and text colour affects the text drawn next to the painted image
              (which is done immediately after the OnCustomPaint call finishes).
            - Brush is guaranteed to be default background colour. It has been already used to
              clear the background of area being painted. It can be modified.
            - Pen is guaranteed to be 1-wide 'black' (or whatever is the proper colour) pen for
              drawing framing rectangle. It can be changed as well.
        \sa @link GetValueAsString @endlink
    */
    virtual void OnCustomPaint( wxDC& dc,
        const wxRect& rect, wxPGPaintData& paintdata );

    /** Sets an attribute of this property. This is quite property class specific,
        and there are no common attributes. Note that attribute may be specific
        to a property instance, or it may affect all properties of its class.
        \param id
        Identifier of attribute
        \param value
        Value for that attribute.
    */
    virtual void SetAttribute( int id, wxVariant& value );


    /** Adds entry to property's wxPGChoices and editor control (if it is active).
        Returns index of item added.
    */
    inline int AppendChoice( const wxString& label, int value = wxPG_INVALID_VALUE )
    {
        return InsertChoice(label,-1,value);
    }

    /** Removes entry from property's wxPGChoices and editor control (if it is active).

        If selected item is deleted, then the value is set to unspecified.
    */
    void DeleteChoice( int index );

    /** Returns comma-delimited string of property attributes.
    */
    wxString GetAttributes( unsigned int flagmask = 0xFFFF );

#if !wxPG_INCLUDE_WXOBJECT
    /** Returns classinfo of the property class.
    */
    virtual const wxPGPropertyClassInfo* GetClassInfo() const = 0;
#endif

    /** Returns property's label. */
    inline const wxString& GetLabel() const { return m_label; }

#ifndef SWIG
    /** Returns wxPropertyGridState to which this property belongs. */
    wxPropertyGridState* GetParentState() const;
#endif

    /** Returns property's name (alternate way to access property). */
    inline const wxString& GetName() const { return m_name; }
    inline void DoSetName(const wxString& str) { m_name = str; }

    /** If property did not have data extension, one is created now
        (returns true in that case).
    */
    bool EnsureDataExt();

    /** Gets pre-calculated top y coordinate of property graphics.
        This cannot be relied on all times (wxPropertyGrid knows when :) ),
        and value is -1 if property is not visible.
    */
    inline int GetY() const { return m_y; }

    void UpdateControl( wxWindow* primary );

    inline wxString GetDisplayedString() const
    {
        return GetValueAsString(0);
    }

    /** Returns property id. */
    inline wxPGId GetId() { return wxPGIdGen(this); }

    /** Returns property grid where property lies. */
    wxPropertyGrid* GetGrid() const;

    /** Returns highest level non-category, non-root parent. Useful when you
        have nested wxCustomProperties/wxParentProperties.
        \remarks
        Thus, if immediate parent is root or category, this will return the
        property itself.
    */
    wxPGProperty* GetMainParent() const;

    /** Return parent of property */
    inline wxPGPropertyWithChildren* GetParent() const { return m_parent; }

    /** Returns true if property is valid and wxPropertyGrid methods
        can operate on it safely.
    */
    inline bool IsOk() const
    {
        return (( m_y >= -1 )?true:false);
    }

	typedef short FlagType;
#ifndef __WXPYTHON__
    typedef void* ClientDataType;
#else
    typedef PyObject* ClientDataType;
#endif

    inline bool IsFlagSet( FlagType flag ) const
    {
        return ( m_flags & flag ) ? true : false;
    }

    inline bool IsValueUnspecified() const
    {
        return ( m_flags & wxPG_PROP_UNSPECIFIED ) ? true : false;
    }

    bool HasFlag( FlagType flag ) const
    {
        return ( m_flags & flag ) ? true : false;
    }

    /** Initializes the property. Usually only called in the constructor.
    */
    void Init( const wxString& label, const wxString& name );

    /** Returns true if extra children can be added for this property
        (i.e. it is wxPropertyCategory or wxCustomProperty)
    */
    inline bool CanHaveExtraChildren() const
    {
        return ( m_parentingType == 1 || m_parentingType == -2 );
    }

    /** Returns property's data extension (NULL if none). */
    inline wxPGPropertyDataExt* GetDataExt() { return m_dataExt; }

    unsigned int GetFlags() const
    {
        return (unsigned int)m_flags;
    }

    const wxPGEditor* GetEditorClass() const;

#ifndef __WXPYTHON__
    /** Returns type name of property that is compatible with CreatePropertyByType.
        and wxVariant::GetType.
    */
    inline const wxChar* GetType() const
    {
        return GetValueTypePtr()->GetType();
    }
#endif

    /** Adds entry to property's wxPGChoices and editor control (if it is active).
        Returns index of item added.
    */
    int InsertChoice( const wxString& label, int index, int value = wxPG_INVALID_VALUE );

    bool IsKindOf( wxPGPropertyClassInfo& info );

    /** Returns true if this is a sub-property. */
    inline bool IsSubProperty() const
    {
        wxPGProperty* parent = (wxPGProperty*)m_parent;
        if ( parent && parent->GetParentingType() < 0 && parent->m_y > -2 )
            return true;
        return false;
    }

    /** Returns last visible sub-property, recursively.
    */
    const wxPGProperty* GetLastVisibleSubItem() const;

    inline int GetMaxLength() const
    {
        return (int) m_maxLen;
    }

#ifdef SWIG
    %pythoncode {
        def GetValue(self):
            return self.GetGrid().GetPropertyValue(self)
    }
#else
    /** Returns value as wxVariant.
    */
    wxVariant GetValueAsVariant() const;
#endif

    /** Returns true if containing grid uses wxPG_EX_AUTO_UNSPECIFIED_VALUES.
    */
    bool UsesAutoUnspecified() const;

    inline wxBitmap* GetValueImage() const
    {
        if ( m_dataExt )
            return m_dataExt->m_valueBitmap;
        return (wxBitmap*) NULL;
    }

    /** Returns number of children (always 0 for normal properties). */
    size_t GetChildCount() const;

    inline unsigned int GetArrIndex() const { return m_arrIndex; }

    inline unsigned int GetDepth() const { return (unsigned int)m_depth; }

    /** Returns position in parent's array. */
    inline unsigned int GetIndexInParent() const
    {
        return (unsigned int)m_arrIndex;
    }

    /** Hides or reveals the property.
        \param hide
        true for hide, false for reveal.
    */
    inline bool Hide( bool hide );

    inline bool IsEnabled() const
    {
        return ( m_flags & wxPG_PROP_DISABLED ) ? false : true;
    }

    /** If property's editor is created this forces its recreation. Useful
        in SetAttribute etc. Returns true if actually did anything.
    */
    bool RecreateEditor();

    inline void SetAttrib( int id, wxVariant value )
    {
        SetAttribute(id,value);
    }

    /** Sets attributes from a comma-delimited string.
    */
    void SetAttributes( const wxString& attributes );

    /** Sets editor for a property. */
#ifndef SWIG
    inline void SetEditor( const wxPGEditor* editor )
    {
        EnsureDataExt();
        m_dataExt->m_customEditor = editor;
    }
#endif

    /** Sets editor for a property. */
    inline void SetEditor( const wxString& editorName );

    /** Changes value of a property with choices, but only
        works if the value type is long or string. */
    void SetChoiceSelection( int newValue, const wxPGChoiceInfo& choiceInfo );

    /** Set wxBitmap in front of the value. This bitmap will be ignored if
        property class has implemented OnCustomPaint.
    */
    void SetValueImage( wxBitmap& bmp );

    /** If property has choices and they are not yet exclusive, new such copy
        of them will be created.
    */
    void SetChoicesExclusive();

    void SetFlag( FlagType flag ) { m_flags |= flag; }

    inline void SetHelpString( const wxString& helpString )
    {
        EnsureDataExt();
        m_dataExt->m_helpString = helpString;
    }

    inline void SetLabel( const wxString& label ) { m_label = label; }

    inline void SetValueToUnspecified()
    {
        m_flags |= wxPG_PROP_UNSPECIFIED;
    }

#if wxUSE_VALIDATORS
    /** Sets wxValidator for a property*/
    inline void SetValidator( const wxValidator& validator )
    {
        EnsureDataExt();
        m_dataExt->m_validator = wxDynamicCast(validator.Clone(),wxValidator);
    }

    /** Gets assignable version of property's validator. */
    inline wxValidator* GetValidator() const
    {
        if ( m_dataExt )
            return m_dataExt->m_validator;
        return DoGetValidator();
    }
#endif // #if wxUSE_VALIDATORS

    inline bool StdValidationProcedure( wxPGVariant value )
    {
        DoSetValue( value );
        return true;
    }

    /** Updates property value in case there were last minute
        changes. If value was unspecified, it will be set to default.
        Use only for properties that have TextCtrl-based editor.
        \remarks
        If you have code similar to
        \code
            // Update the value in case of last minute changes
            if ( primary && propgrid->IsEditorsValueModified() )
                 GetEditorClass()->CopyValueFromControl( this, primary );
        \endcode
        in wxPGProperty::OnEvent wxEVT_COMMAND_BUTTON_CLICKED handler,
        then replace it with call to this method.
        \retval
        True if value changed.
    */
    bool PrepareValueForDialogEditing( wxPropertyGrid* propgrid );

#if wxPG_USE_CLIENT_DATA
    inline ClientDataType GetClientData() const { return m_clientData; }

    /** Sets client data (void*) of a property.
        \remarks
        This untyped client data has to be deleted manually.
    */
    inline void SetClientData( ClientDataType clientData )
    {
#ifdef __WXPYTHON__
        if ( m_clientData )
            Py_DECREF( m_clientData );
        Py_INCREF( clientData );
#endif
        m_clientData = clientData;
    }
#endif

    /** Sets new set of choices for property.
    */
    bool SetChoices( wxPGChoices& choices );

    /** Sets new set of choices for property.
    */
    inline bool SetChoices( const wxArrayString& labels,
                            const wxArrayInt& values = wxArrayInt() );

    /** Set max length of text editor.
    */
    inline bool SetMaxLength( int maxLen );

    inline wxString GetHelpString() const
    {
        if (m_dataExt)
            return m_dataExt->m_helpString;
        return wxEmptyString;
    }

    void ClearFlag( FlagType flag ) { m_flags &= ~(flag); }

    // Use, for example, to detect if item is inside collapsed section.
    bool IsSomeParent( wxPGProperty* candidate_parent ) const;

    // Shows error as a tooltip or something similar (depends on platform).
    void ShowError( const wxString& msg );

#if defined(__WXPYTHON__) && !defined(SWIG)
    // This is the python object that contains and owns the C++ representation.
    PyObject*                   m_scriptObject;
#endif

#ifndef SWIG
protected:

    // Called in constructors.
    void Init();

    wxString                    m_label;
    wxString                    m_name;
    wxPGPropertyWithChildren*   m_parent;
#if wxPG_USE_CLIENT_DATA
    ClientDataType              m_clientData;
#endif
    wxPGPropertyDataExt*        m_dataExt; // Optional data extension.
    unsigned int                m_arrIndex; // Index in parent's property array.
    int                         m_y; // This could be short int.

    short                       m_maxLen; // Maximum length (mainly for string properties).
                                          // Could be in some sort of wxBaseStringProperty, but currently,
                                          // for maximum flexibility and compatibility, we'll stick it here.
                                          // Anyway, we had 3 excess bytes to use so short int will fit in
                                          // just fine.

    FlagType                    m_flags;

    // 1 = category
    // 0 = no children
    // -1 = has fixed-set of sub-properties
    // -2 = this is wxCustomProperty (sub-properties can be added)
    signed char                 m_parentingType;

    unsigned char               m_depth; // Root has 0, categories etc. at that level 1, etc.

    // m_depthBgCol indicates width of background colour between margin and item
    // (essentially this is category's depth, if none then equals m_depth).
    unsigned char               m_depthBgCol;

    unsigned char               m_bgColIndex; // Background brush index.
    unsigned char               m_fgColIndex; // Foreground colour index.

#endif // #ifndef SWIG
};

extern WXDLLIMPEXP_PG wxPGPropertyClassInfo wxBasePropertyClassInfo;


//
// wxPGId comparison operators.
// TODO: Are these really used?
//

#if !defined(__WXPYTHON__)

inline bool operator==(const wxPGId& id, const wxString& b)
{
    wxASSERT(wxPGIdIsOk(id));
    const wxString& a = wxPGIdToPtr(id)->GetName();
    return (a.Len() == b.Len()) && (a.Cmp(b) == 0);
}

inline bool operator==(const wxPGId& id, const wxChar* b)
{
    wxASSERT(wxPGIdIsOk(id));
    return wxPGIdToPtr(id)->GetName().Cmp(b) == 0;
}

#endif // !defined(__WXPYTHON__)


// For dual-pointer-usage reasons, we need to use this trickery
// instead of wxObjArray. wxPGValueType hash map is declared
// in propdev.h.
typedef wxArrayPtrVoid wxPGArrayProperty;


// Always use wxString based hashmap with unicode, stl, swig and GCC 4.0+
#if !defined(SWIG)
WX_DECLARE_STRING_HASH_MAP_WITH_DECL(void*,
                                     wxPGHashMapS2P,
                                     class WXDLLIMPEXP_PG);
#else
class WXDLLIMPEXP_PG wxPGHashMapS2P;
#endif

#define wxPGPropNameStr            const wxString&
#define wxPGNameConv(STR)      STR


// -----------------------------------------------------------------------


#ifndef SWIG

WX_DECLARE_VOIDPTR_HASH_MAP_WITH_DECL(void*,
                                      wxPGHashMapP2P,
                                      class WXDLLIMPEXP_PG);

#else
class WXDLLIMPEXP_PG wxPGHashMapP2P;
#endif // #ifndef SWIG

// -----------------------------------------------------------------------

/** \class wxPGPropertyWithChildren
    \ingroup classes
    \brief wxPGPropertyWithChildren, alias wxParentPropertyClass, is a base
    class for new properties that have sub-properties. For example,
    wxFontProperty and wxFlagsProperty descend from this class.
*/
class WXDLLIMPEXP_PG wxPGPropertyWithChildren : public wxPGProperty
{
#ifndef SWIG
    friend class wxPGProperty;
    friend class wxPropertyGridState;
    friend class wxPropertyGrid;
#endif
    //_WX_PG_DECLARE_PROPERTY_CLASS(wxPGPropertyWithChildren)
public:

    /** Special constructor only used in special cases. */
    wxPGPropertyWithChildren();

    /** When new class is derived, call this constructor.
        \param label
        Label for the property.
    */
    wxPGPropertyWithChildren( const wxString& label, const wxString& name );

    /** Destructor. */
    virtual ~wxPGPropertyWithChildren();

    //virtual int GetParentingType() const;

    /** Advanced variant of GetValueAsString() that forms a string that
        contains sequence of text representations of sub-properties.
    */
    // Advanced version that gives property list and index to this item
    virtual wxString GetValueAsString( int argFlags = 0 ) const;

    /** This overridden version converts comma or semicolon separated
        tokens into child values.
    */
    virtual bool SetValueFromString( const wxString& text, int flags );

    /** Refresh values of child properties.
    */
    virtual void RefreshChildren();

    /** Called after child property p has been altered.
        The value of this parent property should now be updated accordingly.
    */
    virtual void ChildChanged( wxPGProperty* p );

    /** This is used by properties that have fixed sub-properties. */
    void AddChild( wxPGProperty* prop );

    /** This is used by Insert etc. */
    void AddChild2( wxPGProperty* prop, int index = -1, bool correct_mode = true );

    /** Returns number of sub-properties. */
    inline size_t GetCount() const { return m_children.GetCount(); }

    /** Returns sub-property at index i. */
    inline wxPGProperty* Item( size_t i ) const { return (wxPGProperty*)m_children.Item(i); }

    /** Returns last sub-property.
    */
    wxPGProperty* Last() const { return (wxPGProperty*)m_children.Last(); }

    /** Returns index of given sub-property. */
    inline int Index( const wxPGProperty* p ) const { return m_children.Index((void*)p); }

    /** Deletes all sub-properties. */
    void Empty();

    inline bool IsExpanded() const
    {
        return ( m_expanded > 0 ) ? true : false;
    }

    // Puts correct indexes to children
    void FixIndexesOfChildren( size_t starthere = 0 );

#ifndef SWIG
    // Returns wxPropertyGridState in which this property resides.
    wxPropertyGridState* GetParentState() const { return m_parentState; }
#endif

    wxPGProperty* GetItemAtY( unsigned int y, unsigned int lh );

    /** Returns (direct) child property with given name (or NULL if not found).
    */
    wxPGProperty* GetPropertyByName( const wxString& name ) const;

#ifndef SWIG
    // Call for after sub-properties added with AddChild
    void PrepareSubProperties();

    inline void SetParentState( wxPropertyGridState* pstate ) { m_parentState = pstate; }

    // Call after fixed sub-properties added/removed after creation.
    // if oldSelInd >= 0 and < new max items, then selection is
    // moved to it.
    void SubPropsChanged( int oldSelInd = -1 );

protected:

    wxPropertyGridState*    m_parentState;

    wxPGArrayProperty       m_children;
    unsigned char           m_expanded;
#endif // SWIG
};

extern WXDLLIMPEXP_PG wxPGPropertyClassInfo wxBaseParentPropertyClassInfo;

// -----------------------------------------------------------------------

/** \class wxPGRootPropertyClass
    \ingroup classes
    \brief Root parent property.
*/
class WXDLLIMPEXP_PG wxPGRootPropertyClass : public wxPGPropertyWithChildren
{
public:
    WX_PG_DECLARE_PROPERTY_CLASS_NOPARENS
public:

    /** Constructor. */
    wxPGRootPropertyClass();
    virtual ~wxPGRootPropertyClass();

protected:
};

// -----------------------------------------------------------------------

/** \class wxPropertyCategoryClass
    \ingroup classes
    \brief Category (caption) property.
*/
class WXDLLIMPEXP_PG wxPropertyCategoryClass : public wxPGPropertyWithChildren
{
    WX_PG_DECLARE_PROPERTY_CLASS_NOPARENS
public:

    /** Special constructor only used in special cases. */
    wxPropertyCategoryClass();

    /** Construct.
        \param label
        Label for the category.
        \remarks
        All non-category properties appended will have most recently
        added category.
    */
    wxPropertyCategoryClass( const wxString& label, const wxString& name = wxPG_LABEL );
    ~wxPropertyCategoryClass();

    /** Must be overridden with function that doesn't do anything. */
    virtual wxString GetValueAsString( int argFlags ) const;

    inline int GetTextExtent() const { return m_textExtent; }

    void CalculateTextExtent( wxWindow* wnd, wxFont& font );

    void SetTextColIndex( unsigned int colInd ) { m_capFgColIndex = (wxByte) colInd; }
    unsigned int GetTextColIndex() const { return (unsigned int) m_capFgColIndex; }

protected:
    int     m_textExtent;  // pre-calculated length of text
    wxByte  m_capFgColIndex;  // caption text colour index
};

// -----------------------------------------------------------------------


#ifndef SWIG

typedef void* wxPGChoicesId;

class WXDLLIMPEXP_PG wxPGChoicesData
{
public:

    // Constructor sets m_refCount to 1.
    wxPGChoicesData();

    ~wxPGChoicesData();

    wxArrayString   m_arrLabels;
    wxArrayInt      m_arrValues;

    // So that multiple properties can use the same set
    int             m_refCount;

};

#define wxPGChoicesEmptyData    ((wxPGChoicesData*)NULL)

#endif // SWIG


/** \class wxPGChoices
    \ingroup classes
    \brief Helper class for managing constant (key=value) sequences.
*/
class WXDLLIMPEXP_PG wxPGChoices
{
public:

    /** Basic constructor. */
    wxPGChoices()
    {
        Init();
    }

    /** Copy constructor. */
    wxPGChoices( wxPGChoices& a )
    {
        wxASSERT(a.m_data);
        m_data = a.m_data;
        a.m_data->m_refCount++;
    }

    /** Constructor. */
    wxPGChoices( const wxChar** labels, const long* values = NULL )
    {
        Init();
        Set(labels,values);
    }

    /** Constructor. */
    wxPGChoices( const wxArrayString& labels, const wxArrayInt& values = wxArrayInt() )
    {
        Init();
        Set(labels, values);
    }

    /** Simple interface constructor. */
    inline wxPGChoices( wxPGChoicesData* data )
    {
        wxASSERT(data);
        m_data = data;
        data->m_refCount++;
    }

    /** Destructor. */
    ~wxPGChoices()
    {
        Free ();
    }

    void AssignData( wxPGChoicesData* data );

    inline void Assign( const wxPGChoices& a )
    {
        AssignData(a.m_data);
    }

    /** Adds to current. If did not have own copies, creates them now. If was empty,
        identical to set except that creates copies.
    */
    void Add( const wxChar** labels, const long* values = NULL );

    /** Version that works with wxArrayString. */
    void Add( const wxArrayString& arr, const long* values = NULL );

    /** Version that works with wxArrayString and wxArrayInt. */
    void Add( const wxArrayString& arr, const wxArrayInt& arrint );

    /** Adds single item. */
    void Add( const wxChar* label, int value = wxPG_INVALID_VALUE );

    /** Adds single item. */
    void AddAsSorted( const wxString& label, int value = wxPG_INVALID_VALUE );

    inline void EnsureData()
    {
        if ( m_data == wxPGChoicesEmptyData )
            m_data = new wxPGChoicesData();
    }

    /** Returns reference to wxArrayString of labels for you to modify.
    */
    inline wxArrayString& GetLabels()
    {
        wxASSERT( m_data->m_refCount != 0xFFFFFFF );
        return m_data->m_arrLabels;
    }

    /** Returns reference to wxArrayInt of values for you to modify.
    */
    inline wxArrayInt& GetValues()
    {
        wxASSERT( m_data->m_refCount != 0xFFFFFFF );
        return m_data->m_arrValues;
    }

    /** Returns false if this is a constant empty set of choices,
        which should not be modified.
    */
    bool IsOk () const
    {
        return ( m_data != wxPGChoicesEmptyData );
    }

    /** Gets a unsigned number identifying this list. */
    wxPGChoicesId GetId() const { return (wxPGChoicesId) m_data; };

    /** Removes count items starting at position nIndex. */
    inline void RemoveAt(size_t nIndex, size_t count = 1)
    {
        wxASSERT( m_data->m_refCount != 0xFFFFFFF );
        wxPGChoicesData* data = m_data;
        data->m_arrLabels.RemoveAt(nIndex,count);
        if ( data->m_arrValues.GetCount() )
            data->m_arrValues.RemoveAt(nIndex,count);
    }

#ifndef SWIG
    /** Does not create copies for itself. */
    void Set( const wxChar** labels, const long* values = NULL )
    {
        Free();
        Add(labels,values);
    }

    /** Version that works with wxArrayString.
        TODO: Deprecate this.
    */
    void Set( wxArrayString& arr, const long* values = (const long*) NULL )
    {
        Free();
        Add(arr,values);
    }
#endif // SWIG

    /** Version that works with wxArrayString and wxArrayInt. */
    void Set( const wxArrayString& labels, const wxArrayInt& values = wxArrayInt() )
    {
        Free();
        Add(labels, values);
    }

    // Creates exclusive copy of current choices
    inline void SetExclusive()
    {
        if ( m_data->m_refCount != 1 )
        {
            wxPGChoicesData* data = new wxPGChoicesData;
            data->m_arrLabels = m_data->m_arrLabels;
            data->m_arrValues = m_data->m_arrValues;
            Free();
            m_data = data;
        }
    }

    inline const wxString& GetLabel( size_t ind ) const
    {
        return m_data->m_arrLabels[ind];
    }

    inline const wxArrayString& GetLabels() const { return m_data->m_arrLabels; }

    inline size_t GetCount () const
    {
        wxASSERT_MSG( m_data,
            wxT("When checking if wxPGChoices is valid, use IsOk() instead of GetCount()") );
        return m_data->m_arrLabels.GetCount();
    }

    inline int GetValue( size_t ind ) const { return m_data->m_arrValues[ind]; }
    inline const wxArrayInt& GetValues() const { return m_data->m_arrValues; }

    inline int Index( const wxString& str ) const { return m_data->m_arrLabels.Index(str); }

    /** Inserts single item. */
#if wxCHECK_VERSION(2,9,0)
    void Insert( const wxString& label, int index, int value = wxPG_INVALID_VALUE );
#else
    void Insert( const wxChar* label, int index, int value = wxPG_INVALID_VALUE );
#endif

    // Returns data, increases refcount.
    inline wxPGChoicesData* GetData()
    {
        wxASSERT( m_data->m_refCount != 0xFFFFFFF );
        m_data->m_refCount++;
        return m_data;
    }

    // Returns plain data ptr - no refcounting stuff is done.
    inline wxPGChoicesData* GetDataPtr() const { return m_data; }

    // Changes ownership of data to you.
    inline wxPGChoicesData* ExtractData()
    {
        wxPGChoicesData* data = m_data;
        m_data = wxPGChoicesEmptyData;
        return data;
    }

    inline void AddString( const wxString& str ) { m_data->m_arrLabels.Add(str); }
    inline void AddInt( int val ) { m_data->m_arrValues.Add(val); }

    inline void SetLabels( wxArrayString& arr ) { m_data->m_arrLabels = arr; }
    inline void SetValues( wxArrayInt& arr ) { m_data->m_arrValues = arr; }
#ifndef SWIG
    inline void SetLabels( const wxArrayString& arr ) { m_data->m_arrLabels = arr; }
    inline void SetValues( const wxArrayInt& arr ) { m_data->m_arrValues = arr; }

protected:

    wxPGChoicesData*    m_data;

    void Init();
    void Free();
#endif
};


// -----------------------------------------------------------------------
// Property declaration.

// Doxygen will only generate warnings here
#ifndef DOXYGEN


#define wxPG_CONSTFUNC(PROP) PROP
#define wxPG_PROPCLASS(PROP) PROP##Class

// Macro based constructor.
#define wxPG_NEWPROPERTY(PROP,LABEL,NAME,VALUE) wx##PROP##Property(LABEL,NAME,VALUE)

#define wxPG_DECLARE_PROPERTY_CLASSINFO(NAME) \
    extern wxPGPropertyClassInfo NAME##ClassInfo;

#define wxPG_DECLARE_PROPERTY_CLASSINFO_WITH_DECL(NAME,DECL) \
    extern DECL wxPGPropertyClassInfo NAME##ClassInfo;

#define WX_PG_DECLARE_PROPERTY_WITH_DECL(NAME,VALARG,DEFVAL,DECL) \
    extern DECL wxPGProperty* wxPG_CONSTFUNC(NAME)( const wxString& label, const wxString& name = wxPG_LABEL, VALARG value = DEFVAL ); \
    extern DECL wxPGPropertyClassInfo NAME##ClassInfo;

#define WX_PG_DECLARE_PROPERTY(NAME,VALARG,DEFVAL) \
    extern wxPGProperty* wxPG_CONSTFUNC(NAME)( const wxString& label, const wxString& name = wxPG_LABEL, VALARG value = DEFVAL ); \
    wxPG_DECLARE_PROPERTY_CLASSINFO(NAME)

//
// Specific macro-based declarations.
//

#define WX_PG_DECLARE_STRING_PROPERTY_WITH_DECL(NAME,DECL) \
extern DECL wxPGProperty* wxPG_CONSTFUNC(NAME)( const wxString& label, const wxString& name= wxPG_LABEL, const wxString& value = wxEmptyString ); \
extern DECL wxPGPropertyClassInfo NAME##ClassInfo;

#define WX_PG_DECLARE_STRING_PROPERTY(NAME) \
extern wxPGProperty* wxPG_CONSTFUNC(NAME)( const wxString& label, const wxString& name= wxPG_LABEL, const wxString& value = wxEmptyString ); \
wxPG_DECLARE_PROPERTY_CLASSINFO(NAME)

#define WX_PG_DECLARE_CUSTOM_FLAGS_PROPERTY_WITH_DECL(NAME,DECL) \
WX_PG_DECLARE_PROPERTY_WITH_DECL(NAME,long,-1,DECL)

#define WX_PG_DECLARE_CUSTOM_FLAGS_PROPERTY(NAME) \
WX_PG_DECLARE_PROPERTY(NAME,long,-1)

#define WX_PG_DECLARE_CUSTOM_ENUM_PROPERTY_WITH_DECL(NAME,DECL) \
WX_PG_DECLARE_PROPERTY_WITH_DECL(NAME,int,-1,DECL)

#define WX_PG_DECLARE_CUSTOM_ENUM_PROPERTY(NAME) \
WX_PG_DECLARE_PROPERTY(NAME,int,-1)

#define WX_PG_DECLARE_ARRAYSTRING_PROPERTY_WITH_DECL(NAME,DECL) \
extern DECL wxPGProperty* wxPG_CONSTFUNC(NAME)( const wxString& label, const wxString& name = wxPG_LABEL, const wxArrayString& value = wxArrayString() ); \
extern DECL wxPGPropertyClassInfo NAME##ClassInfo;

#define WX_PG_DECLARE_ARRAYSTRING_PROPERTY(NAME) \
extern wxPGProperty* wxPG_CONSTFUNC(NAME)( const wxString& label, const wxString& name = wxPG_LABEL, const wxArrayString& value = wxArrayString() ); \
wxPG_DECLARE_PROPERTY_CLASSINFO(NAME)

// Declare basic property classes.
WX_PG_DECLARE_PROPERTY_WITH_DECL(wxStringProperty,const wxString&,wxEmptyString,WXDLLIMPEXP_PG)
WX_PG_DECLARE_PROPERTY_WITH_DECL(wxIntProperty,long,0,WXDLLIMPEXP_PG)
WX_PG_DECLARE_PROPERTY_WITH_DECL(wxUIntProperty,unsigned long,0,WXDLLIMPEXP_PG)
WX_PG_DECLARE_PROPERTY_WITH_DECL(wxFloatProperty,double,0.0,WXDLLIMPEXP_PG)
WX_PG_DECLARE_PROPERTY_WITH_DECL(wxBoolProperty,bool,false,WXDLLIMPEXP_PG)
WX_PG_DECLARE_PROPERTY_WITH_DECL(wxLongStringProperty,const wxString&,wxEmptyString,WXDLLIMPEXP_PG)
WX_PG_DECLARE_PROPERTY_WITH_DECL(wxFileProperty,const wxString&,wxEmptyString,WXDLLIMPEXP_PG)
WX_PG_DECLARE_PROPERTY_WITH_DECL(wxArrayStringProperty,const wxArrayString&,wxArrayString(),WXDLLIMPEXP_PG)

WX_PG_DECLARE_STRING_PROPERTY_WITH_DECL(wxDirProperty,WXDLLIMPEXP_PG)

// Enum and Flags Properties require special attention.
#ifndef SWIG

extern WXDLLIMPEXP_PG wxPGProperty* wxEnumProperty( const wxString& label, const wxString& name,
    const wxChar** labels = (const wxChar**) NULL,
    const long* values = NULL, int value = 0 );

extern WXDLLIMPEXP_PG wxPGProperty* wxEnumProperty( const wxString& label, const wxString& name,
    const wxArrayString& labels, int value = 0 );

extern WXDLLIMPEXP_PG wxPGProperty* wxEnumProperty( const wxString& label, const wxString& name,
    wxPGChoices& constants, int value = 0 );

extern WXDLLIMPEXP_PG wxPGProperty* wxEnumProperty( const wxString& label, const wxString& name,
    const wxArrayString& choices, const wxArrayInt& values, int value = 0 );

#else

// Separate for SWIG inorder to have more default arguments
extern WXDLLIMPEXP_PG wxPGProperty* wxEnumProperty( const wxString& label, const wxString& name = wxPG_LABEL,
    const wxArrayString& choices = wxArrayString(), const wxArrayInt& values = wxArrayInt(), int value = 0 );

#endif // SWIG

extern WXDLLIMPEXP_PG wxPGPropertyClassInfo wxEnumPropertyClassInfo;


#ifndef SWIG

extern WXDLLIMPEXP_PG wxPGProperty* wxEditEnumProperty( const wxString& label, const wxString& name,
    const wxChar** labels = (const wxChar**) NULL,
    const long* values = NULL, const wxString& value = wxEmptyString );

extern WXDLLIMPEXP_PG wxPGProperty* wxEditEnumProperty( const wxString& label, const wxString& name,
    const wxArrayString& labels, const wxString& value = wxEmptyString );

extern WXDLLIMPEXP_PG wxPGProperty* wxEditEnumProperty( const wxString& label, const wxString& name,
    wxPGChoices& constants, const wxString& value = wxEmptyString );

extern WXDLLIMPEXP_PG wxPGProperty* wxEditEnumProperty( const wxString& label, const wxString& name,
    const wxArrayString& choices, const wxArrayInt& values, const wxString& value = wxEmptyString );

#else

// Separate for SWIG inorder to have more default arguments
extern WXDLLIMPEXP_PG wxPGProperty* wxEditEnumProperty( const wxString& label, const wxString& name = wxPG_LABEL,
    const wxArrayString& choices = wxArrayString(), const wxArrayInt& values = wxArrayInt(), const wxString& value = wxEmptyString );

#endif // SWIG

extern WXDLLIMPEXP_PG wxPGPropertyClassInfo wxEditEnumPropertyClassInfo;


#ifndef SWIG

extern WXDLLIMPEXP_PG wxPGProperty* wxFlagsProperty( const wxString& label, const wxString& name, const wxChar** labels = (const wxChar**) NULL,
    const long* values = NULL, int value = 0 );

extern WXDLLIMPEXP_PG wxPGProperty* wxFlagsProperty( const wxString& label, const wxString& name,
    const wxArrayString& labels, int value = 0 );

extern WXDLLIMPEXP_PG wxPGProperty* wxFlagsProperty( const wxString& label, const wxString& name,
    wxPGChoices& constants, int value = 0 );

extern WXDLLIMPEXP_PG wxPGProperty* wxFlagsProperty( const wxString& label, const wxString& name,
    const wxArrayString& flag_labels, const wxArrayInt& values, int value = 0 );

#else

// Separate for SWIG inorder to have more default arguments
extern WXDLLIMPEXP_PG wxPGProperty* wxFlagsProperty( const wxString& label, const wxString& name = wxPG_LABEL,
    const wxArrayString& flag_labels = wxArrayString(), const wxArrayInt& values = wxArrayInt(), int value = 0 );

#endif // SWIG


extern WXDLLIMPEXP_PG wxPGPropertyClassInfo wxFlagsPropertyClassInfo;


// wxCustomProperty doesn't have value argument.
extern WXDLLIMPEXP_PG wxPGProperty* wxCustomProperty( const wxString& label, const wxString& name = wxPG_LABEL );
extern WXDLLIMPEXP_PG wxPGPropertyClassInfo wxCustomPropertyClassInfo;

// wxParentProperty doesn't have value argument.
extern WXDLLIMPEXP_PG wxPGProperty* wxParentProperty( const wxString& label, const wxString& name );
extern WXDLLIMPEXP_PG wxPGPropertyClassInfo wxParentPropertyClassInfo;

// wxPropertyCategory doesn't have value argument.
extern WXDLLIMPEXP_PG wxPGProperty* wxPropertyCategory ( const wxString& label, const wxString& name = wxPG_LABEL );
extern WXDLLIMPEXP_PG wxPGPropertyClassInfo wxPropertyCategoryClassInfo;

#endif // DOXYGEN


#ifndef wxDynamicCastVariantData
    #define wxDynamicCastVariantData wxDynamicCast
#endif

// FIXME: Should this be out-of-inline?
inline wxObject* wxPG_VariantToWxObject( wxVariant& variant, wxClassInfo* classInfo )
{
    if ( !variant.IsValueKindOf(classInfo) )
        return (wxObject*) NULL;
    wxVariantData* vdata = variant.GetData();

    wxPGVariantDataWxObj* vdataWxObj = wxDynamicCastVariantData(vdata, wxPGVariantDataWxObj);
    if ( vdataWxObj )
        return (wxObject*) vdataWxObj->GetValuePtr();

    return variant.GetWxObjectPtr();
}

//
// Redefine wxGetVariantCast to also take propertygrid variantdata
// classes into account.
// TODO: Remove after persistent wxObject classes added (i.e.
//   GetWxObjectPtr works for all).
//
#undef wxGetVariantCast
#define wxGetVariantCast(var,classname) (classname*)wxPG_VariantToWxObject(var,&classname::ms_classInfo)

// TODO: After a while, remove this.
#define WX_PG_VARIANT_TO_WXOBJECT(VARIANT,CLASSNAME) (CLASSNAME*)wxPG_VariantToWxObject(VARIANT,&CLASSNAME::ms_classInfo)
//#define WX_PG_VARIANT_TO_WXOBJECT(VARIANT,CLASSNAME) wxGetVariantCast(VARIANT,CLASSNAME)

// -----------------------------------------------------------------------

#ifndef SWIG
// We won't need this class from wxPython

/** \class wxPropertyGridState
	\ingroup classes
    \brief
    Contains information of a single wxPropertyGrid page.
*/
// BM_STATE
class WXDLLIMPEXP_PG wxPropertyGridState
{
    friend class wxPGProperty;
    friend class wxPropertyGrid;
    friend class wxPropertyGridManager;
public:

    /** Constructor. */
    wxPropertyGridState();

    /** Destructor. */
    virtual ~wxPropertyGridState();

    /** Base append. */
    wxPGId Append( wxPGProperty* property );

    wxPGId AppendIn( wxPGPropertyWithChildren* pwc, const wxString& label, const wxString& propname, wxVariant& value );

    /** Returns property by its name. */
    wxPGId BaseGetPropertyByName( wxPGPropNameStr name ) const;

    /** Called in, for example, wxPropertyGrid::Clear. */
    void Clear();

    void ClearModifiedStatus( wxPGProperty* p );

    static void ClearPropertyAndChildrenFlags( wxPGProperty* p, long flags );
    static void SetPropertyAndChildrenFlags( wxPGProperty* p, long flags );

    bool ClearPropertyValue( wxPGProperty* p );

    inline bool ClearSelection()
    {
        return DoSelectProperty(wxPGIdGen((wxPGProperty*)NULL));
    }

    bool Collapse( wxPGProperty* p );

    /** Override this member function to add custom behaviour on property deletion.
    */
    virtual void DoDelete( wxPGProperty* item );

    /** Override this member function to add custom behaviour on property insertion.
    */
    virtual wxPGId DoInsert( wxPGPropertyWithChildren* parent, int index, wxPGProperty* property );

    bool EnableCategories( bool enable );

    /** Enables or disables given property and its subproperties. */
    bool EnableProperty( wxPGProperty* p, bool enable );

    bool Expand( wxPGProperty* p );

    bool ExpandAll( unsigned char do_expand );

    /** Returns id of first item, whether it is a category or property. */
    inline wxPGId GetFirst() const
    {
        wxPGProperty* p = (wxPGProperty*) NULL;
        if ( m_properties->GetCount() )
            p = m_properties->Item(0);
        return wxPGIdGen(p);
    }

    wxPGId GetFirstCategory() const;

    wxPGId GetFirstProperty() const;

    wxPropertyGrid* GetGrid() const { return m_pPropGrid; }

    wxPGId GetNextCategory( wxPGId id ) const;

    wxPGId GetNextProperty( wxPGId id ) const;

    static wxPGId GetNextSibling( wxPGId id );

    static wxPGId GetPrevSibling( wxPGId id );

    wxPGId GetPrevProperty( wxPGId id ) const;

    wxPGId GetPropertyByLabel( const wxString& name, wxPGPropertyWithChildren* parent  = (wxPGPropertyWithChildren*) NULL ) const;

    wxVariant GetPropertyValues( const wxString& listname, wxPGId baseparent, long flags ) const;

    inline wxPGProperty* GetSelection() const { return m_selected; }

    /** Used by SetSplitterLeft. */
    int GetLeftSplitterPos( wxClientDC& dc, wxPGPropertyWithChildren* pwc, bool subProps );

    inline bool IsDisplayed() const;

    inline bool IsInNonCatMode() const { return (bool)(m_properties == m_abcArray); }

    /** Only inits arrays, doesn't migrate things or such. */
    void InitNonCatMode ();

    void LimitPropertyEditing ( wxPGProperty* p, bool limit = true );

    bool DoSelectProperty( wxPGProperty* p, unsigned int flags = 0 );

    void SetPropertyLabel( wxPGProperty* p, const wxString& newlabel );

    bool SetPropertyPriority( wxPGProperty* p, int priority );

    void SetPropVal( wxPGProperty* p, const wxPGVariant& value );

    bool SetPropertyValue( wxPGProperty* p, const wxPGValueType* typeclass, const wxPGVariant& value );

    bool SetPropertyValue( wxPGProperty* p, const wxChar* typestring, const wxPGVariant& value );

    bool SetPropertyValueString( wxPGProperty* p, const wxString& value );

    bool SetPropertyValue( wxPGProperty* p, wxVariant& value );

    bool SetPropertyValueWxObjectPtr( wxPGProperty* p, wxObject* value );

    /** Sets value (long integer) of a property. */
    inline void SetPropertyValueLong( wxPGProperty* p, long value )
    {
        SetPropertyValue( p, wxPG_VALUETYPE(long), wxPGVariantFromLong(value) );
    }
    /** Sets value (integer) of a property. */
    inline void SetPropertyValue( wxPGProperty* p, int value )
    {
        SetPropertyValue( p, wxPG_VALUETYPE(long), wxPGVariantFromLong((long)value) );
    }
    /** Sets value (floating point) of a property. */
    inline void SetPropertyValueDouble( wxPGProperty* p, double value )
    {
        SetPropertyValue( p, wxPG_VALUETYPE(double), wxPGVariantFromDouble(value) );
    }
    /** Sets value (bool) of a property. */
    inline void SetPropertyValueBool( wxPGProperty* p, bool value )
    {
        SetPropertyValue( p, wxPG_VALUETYPE(bool), wxPGVariantFromLong(value?(long)1:(long)0) );
    }
    /** Sets value (wxArrayString) of a property. */
    inline void SetPropertyValueArrstr2( wxPGProperty* p, const wxArrayString& value )
    {
        SetPropertyValue( p, wxPG_VALUETYPE(wxArrayString), wxPGVariantFromArrayString(value) );
    }
    /** Sets value (void*) of a property. */
    inline void SetPropertyValue( wxPGProperty* p, void* value )
    {
        SetPropertyValue( p, wxPG_VALUETYPE(void), value );
    }
    /** Sets value (wxPoint&) of a property. */
    inline void SetPropertyValuePoint( wxPGProperty* p, const wxPoint& value )
    {
        wxASSERT(p);
        SetPropertyValue( p, wxT("wxPoint"), wxPGVariantCreator(value) );
    }
    /** Sets value (wxSize&) of a property. */
    inline void SetPropertyValueSize( wxPGProperty* p, const wxSize& value )
    {
        wxASSERT(p);
        SetPropertyValue( p, wxT("wxSize"), wxPGVariantCreator(value) );
    }
    /** Sets value (wxArrayInt&) of a property. */
    inline void SetPropertyValueArrint2( wxPGProperty* p, const wxArrayInt& value )
    {
        wxASSERT(p);
        SetPropertyValue( p, wxT("wxArrayInt"), wxPGVariantCreator(value));
    }
#if wxUSE_DATETIME
    /** Sets value (wxDateTime&) of a property. */
    inline void SetPropertyValueDatetime( wxPGProperty* p, const wxDateTime& value )
    {
        wxASSERT(p);
        SetPropertyValue( p, wxT("datetime"), wxPGVariantCreator(value) );
    }
#endif
#ifdef __WXPYTHON__
    inline void SetPropertyValuePyObject( wxPGProperty* p, PyObject* value )
    {
        SetPropertyValue( p, wxPG_VALUETYPE(PyObject), wxPGVariantCreator(value) );
    }
#endif
    /** Sets value (wxLongLong&) of a property. */
    inline void SetPropertyValueLongLong( wxPGProperty* p, const wxLongLong& value )
    {
        wxASSERT(p);
        SetPropertyValue( p, wxT("wxLongLong"), wxPGVariantCreator(value) );
    }
    /** Sets value (wxULongLong&) of a property. */
    inline void SetPropertyValueULongLong( wxPGProperty* p, const wxULongLong& value )
    {
        wxASSERT(p);
        SetPropertyValue( p, wxT("wxULongLong"), wxPGVariantCreator(value) );
    }

    void SetPropertyValues( const wxVariantList& list, wxPGId default_category );

    void SetPropertyUnspecified( wxPGProperty* p );

#ifdef wxPG_COMPATIBILITY_1_0_0
    #define SetPropertyValueUnspecified SetPropertyUnspecified
#endif

    void Sort( wxPGProperty* p );
    void Sort();

protected:

#ifndef DOXYGEN
    int PrepareToAddItem ( wxPGProperty* property, wxPGPropertyWithChildren* scheduledParent );

    /** If visible, then this is pointer to wxPropertyGrid.
        This shall *never* be NULL to indicate that this state is not visible.
    */
    wxPropertyGrid*             m_pPropGrid;

    /** Pointer to currently used array. */
    wxPGPropertyWithChildren*   m_properties;

    /** Array for categoric mode. */
    wxPGRootPropertyClass       m_regularArray;

    /** Array for root of non-categoric mode. */
    wxPGRootPropertyClass*      m_abcArray;

    /** Dictionary for name-based access. */
    wxPGHashMapS2P              m_dictName;

    /** Most recently added category. */
    wxPropertyCategoryClass*    m_currentCategory;

    /** Pointer to selected property. */
    wxPGProperty*               m_selected;

    /** 1 if m_lastCaption is also the bottommost caption. */
    unsigned char               m_lastCaptionBottomnest;
    /** 1 items appended/inserted, so stuff needs to be done before drawing;
        If m_bottomy == 0, then calcylatey's must be done.
        Otherwise just sort.
    */
    unsigned char               m_itemsAdded;

    /** 1 if any value is modified. */
    unsigned char               m_anyModified;

#endif // #ifndef DOXYGEN

};

#endif // #ifndef SWIG

inline bool wxPGProperty::SetChoices( const wxArrayString& labels,
                                      const wxArrayInt& values )
{
    wxPGChoices chs(labels,values);
    return SetChoices(chs);
}

// -----------------------------------------------------------------------

/*

    wxASSERT_MSG( wxPGIdIsOk(id), \
                  wxT("invalid property id") ); \

*/


// Helper macro that does necessary preparations when calling
// some wxPGProperty's member function.
#define wxPG_PROP_ID_CALL_PROLOG() \
    wxPGProperty *p = wxPGIdToPtr(id); \
    wxCHECK_RET( p, wxT("invalid property id") );

#define wxPG_PROP_NAME_CALL_PROLOG() \
    wxPGProperty *p = wxPGIdToPtr(GetPropertyByNameI(name)); \
    if ( !p ) return;

#define wxPG_PROP_ID_CALL_PROLOG_RETVAL(RVAL) \
    wxPGProperty *p = wxPGIdToPtr(id); \
    wxCHECK_MSG( p, RVAL, wxT("invalid property id") );

#define wxPG_PROP_NAME_CALL_PROLOG_RETVAL(RVAL) \
    wxPGProperty *p = wxPGIdToPtr(GetPropertyByNameI(name)); \
    if ( !p ) return RVAL;

// GetPropertyName version used internally. Use GetPropertyName for slight speed advantage,
// or GetPropertyNameA for nice assertion (essential for wxPython bindings).
#define GetPropertyByNameI          GetPropertyByNameA

// FOR BACKWARDS COMPATIBILITY
#define GetPropertyByNameWithAssert GetPropertyByNameA

#include "propertycontainermethods.h"

// -----------------------------------------------------------------------


// wxPropertyGrid::DoSelectProperty flags
#define wxPG_SEL_FOCUS      0x01 // Focuses to created editor
#define wxPG_SEL_FORCE      0x02 // Forces deletion and recreation of editor
#define wxPG_SEL_NONVISIBLE 0x04 // For example, doesn't cause EnsureVisible
#define wxPG_SEL_NOVALIDATE 0x08 // Do not validate editor's value before selecting
#define wxPG_SEL_DELETING   0x10 // Property being deselected is about to be deleted
#define wxPG_SEL_SETUNSPEC  0x20 // Property's values was set to unspecified by the user


// -----------------------------------------------------------------------

#ifndef SWIG

// Internal flags
#define wxPG_FL_INITIALIZED                 0x0001
#define wxPG_FL_ACTIVATION_BY_CLICK         0x0002 // Set when creating editor controls if it was clicked on.
#define wxPG_FL_DONT_CENTER_SPLITTER        0x0004
#define wxPG_FL_FOCUSED                     0x0008
#define wxPG_FL_MOUSE_CAPTURED              0x0010
#define wxPG_FL_MOUSE_INSIDE                0x0020
#define wxPG_FL_VALUE_MODIFIED              0x0040
#define wxPG_FL_PRIMARY_FILLS_ENTIRE        0x0080 // don't clear background of m_wndPrimary
#define wxPG_FL_CUR_USES_CUSTOM_IMAGE       0x0100 // currently active editor uses custom image
#define wxPG_FL_HIDE_STATE                  0x0200 // set when hideable properties should be hidden
#define wxPG_FL_SCROLLED                    0x0400
#define wxPG_FL_ADDING_HIDEABLES            0x0800 // set when all added/inserted properties get hideable flag
#define wxPG_FL_NOSTATUSBARHELP             0x1000 // Disables showing help strings on statusbar.
#define wxPG_FL_CREATEDSTATE                0x2000 // Marks that we created the state, so we have to destroy it too.
#define wxPG_FL_SCROLLBAR_DETECTED          0x4000 // Set if scrollbar's existence was detected in last onresize.
#define wxPG_FL_DESC_REFRESH_REQUIRED       0x8000 // Set if wxPGMan requires redrawing of description text box.
#define wxPG_FL_SELECTED_IS_PAINT_FLEXIBLE  0x00010000 // Set if selected has flexible imagesize
#define wxPG_FL_IN_MANAGER                  0x00020000 // Set if contained in wxPropertyGridManager
#define wxPG_FL_GOOD_SIZE_SET               0x00040000 // Set after wxPropertyGrid is shown in its initial good size
#define wxPG_FL_IGNORE_NEXT_NAVKEY          0x00080000 // Next navigation key event will get ignored
#define wxPG_FL_IN_SELECT_PROPERTY          0x00100000 // Set when in SelectProperty.
#define wxPG_FL_STRING_IN_STATUSBAR         0x00200000 // Set when help string is shown in status bar
#define wxPG_FL_SPLITTER_PRE_SET            0x00400000 // Splitter position has been custom-set by the user
#define wxPG_FL_VALIDATION_FAILED           0x00800000 // Validation failed. Clear on modify event.
#define wxPG_FL_SELECTED_IS_FULL_PAINT      0x01000000 // Set if selected is fully painted (ie. both image and text)
#define wxPG_MAN_FL_PAGE_INSERTED           0x02000000 // Set after page has been inserted to manager
#define wxPG_FL_ABNORMAL_EDITOR             0x04000000 // Active editor control is abnormally large

#endif // #ifndef SWIG


// -----------------------------------------------------------------------

#include "propertygrid.h"

// -----------------------------------------------------------------------
//
// Bunch of inlines that need to resolved after all classes have been defined.
//

#ifndef SWIG
inline bool wxPropertyGridState::IsDisplayed() const
{
    return ( this == m_pPropGrid->GetState() );
}
#endif

inline void wxPGProperty::SetEditor( const wxString& editorName )
{
    EnsureDataExt();
    m_dataExt->m_customEditor = wxPropertyContainerMethods::GetEditorByName(editorName);
}

inline bool wxPGProperty::Hide( bool hide )
{
    return GetGrid()->HideProperty(wxPGIdGen(this),hide);
}

inline bool wxPGProperty::SetMaxLength( int maxLen )
{
    return GetGrid()->SetPropertyMaxLength(wxPGIdGen(this),maxLen);
}

#if !wxPG_ID_IS_PTR
inline const wxString& wxPGId::GetName() const
{
    return m_ptr->GetName();
}
#endif

// -----------------------------------------------------------------------

/** \class wxPropertyGridEvent
	\ingroup classes
    \brief A propertygrid event holds information about events associated with
    wxPropertyGrid objects.

    <h4>Derived from</h4>

    wxNotifyEvent\n
    wxCommandEvent\n
    wxEvent\n
    wxObject\n

    <h4>Include files</h4>

    <wx/propertygrid/propertygrid.h>
*/
class WXDLLIMPEXP_PG wxPropertyGridEvent : public wxCommandEvent
{
public:

    /** Constructor. */
    wxPropertyGridEvent(wxEventType commandType=0, int id=0);
#ifndef SWIG
    /** Copy constructor. */
    wxPropertyGridEvent(const wxPropertyGridEvent& event);
#endif
    /** Destructor. */
    ~wxPropertyGridEvent();

    /** Copyer. */
    virtual wxEvent* Clone() const;

    /** Enables property. */
    inline void EnableProperty( bool enable = true )
    {
        m_pg->EnableProperty(wxPGIdGen(m_property),enable);
    }

    /** Disables property. */
    inline void DisableProperty()
    {
        m_pg->EnableProperty(wxPGIdGen(m_property),false);
    }

    inline wxPGId GetMainParent() const
    {
        wxASSERT(m_property);
        return wxPGIdGen(m_property->GetMainParent());
    }

    /** Returns id of associated property. */
    wxPGId GetProperty() const
    {
        return wxPGIdGen(m_property);
    }

#ifndef SWIG
    /** Returns pointer to associated property. */
    wxPGProperty* GetPropertyPtr() const
    {
        return m_property;
    }
#endif

    /** Returns label of associated property. */
    const wxString& GetPropertyLabel() const
    {
        wxASSERT( m_property );
        return m_property->GetLabel();
    }

    /** Returns name of associated property. */
    const wxString& GetPropertyName() const
    {
        wxASSERT( m_property );
        return m_property->GetName();
    }

#if wxPG_USE_CLIENT_DATA
    /** Returns client data of relevant property. */
    wxPGProperty::ClientDataType GetPropertyClientData() const
    {
        wxASSERT( m_property );
        return m_property->GetClientData();
    }
#endif

#ifndef SWIG
    /** Returns value of relevant property. */
    wxVariant GetPropertyValue() const
    {
        wxASSERT( m_property );
        return m_property->GetValueAsVariant();
    }

    inline wxString GetPropertyValueAsString() const
    {
        return m_pg->GetPropertyValueAsString( wxPGIdGen(m_property) );
    }
    inline long GetPropertyValueAsLong() const
    {
        return m_pg->GetPropertyValueAsLong( wxPGIdGen(m_property) );
    }
    inline int GetPropertyValueAsInt() const
    {
        return (int)GetPropertyValueAsLong();
    }
    inline long GetPropertyValueAsBool() const
    {
        return m_pg->GetPropertyValueAsBool( wxPGIdGen(m_property) );
    }
    inline double GetPropertyValueAsDouble() const
    {
        return m_pg->GetPropertyValueAsDouble( wxPGIdGen(m_property) );
    }
    inline const wxObject* GetPropertyValueAsWxObjectPtr() const
    {
        return m_pg->GetPropertyValueAsWxObjectPtr( wxPGIdGen(m_property) );
    }
    inline void* GetPropertyValueAsVoidPtr() const
    {
        return m_pg->GetPropertyValueAsVoidPtr( wxPGIdGen(m_property) );
    }
#if !wxPG_PGVARIANT_IS_VARIANT
    inline const wxArrayString& GetPropertyValueAsArrayString() const
    {
        return m_pg->GetPropertyValueAsArrayString( wxPGIdGen(m_property) );
    }
    inline const wxPoint& GetPropertyValueAsPoint() const
    {
        return m_pg->GetPropertyValueAsPoint( wxPGIdGen(m_property) );
    }
    inline const wxSize& GetPropertyValueAsSize() const
    {
        return m_pg->GetPropertyValueAsSize( wxPGIdGen(m_property) );
    }
    inline const wxArrayInt& GetPropertyValueAsArrayInt() const
    {
        return m_pg->GetPropertyValueAsArrayInt( wxPGIdGen(m_property) );
    }
#else
    inline wxArrayString GetPropertyValueAsArrayString() const
    {
        return m_pg->GetPropertyValueAsArrayString( wxPGIdGen(m_property) );
    }
    inline wxPoint GetPropertyValueAsPoint() const
    {
        return m_pg->GetPropertyValueAsPoint( wxPGIdGen(m_property) );
    }
    inline wxSize GetPropertyValueAsSize() const
    {
        return m_pg->GetPropertyValueAsSize( wxPGIdGen(m_property) );
    }
    inline wxArrayInt GetPropertyValueAsArrayInt() const
    {
        return m_pg->GetPropertyValueAsArrayInt( wxPGIdGen(m_property) );
    }
#endif

    /** Returns value type of relevant property. */
    const wxPGValueType* GetPropertyValueType() const
    {
        return m_property->GetValueTypePtr();
    }

#else
    %pythoncode {
        def GetPropertyValue(self):
            return self.GetProperty().GetValue()
    }
#endif

    /** Returns true if event has associated property. */
    inline bool HasProperty() const { return ( m_property != (wxPGProperty*) NULL ); }

    inline bool IsPropertyEnabled() const
    {
        return m_pg->IsPropertyEnabled(wxPGIdGen(m_property));
    }

#ifndef SWIG

    // IsPending() == true, if sent using AddPendingEvent
    inline void SetPending( bool pending ) { m_pending = pending; }
    inline bool IsPending() const { return m_pending; }

#if !wxPG_ID_IS_PTR
    /** Changes the associated property. */
    void SetProperty( wxPGId id ) { m_property = wxPGIdToPtr(id); }
#endif

    /** Changes the associated property. */
    void SetProperty( wxPGProperty* p ) { m_property = p; }

    void SetPropertyGrid( wxPropertyGrid* pg ) { m_pg = pg; }

private:
    DECLARE_DYNAMIC_CLASS(wxPropertyGridEvent)

    wxPGProperty*       m_property;
    wxPropertyGrid*     m_pg;
    bool                m_pending;
#endif
};


#define wxPG_BASE_EVT_PRE_ID     1775

#ifndef SWIG
BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_SELECTED,           wxPG_BASE_EVT_PRE_ID)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_CHANGED,            wxPG_BASE_EVT_PRE_ID+1)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_HIGHLIGHTED,        wxPG_BASE_EVT_PRE_ID+2)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_RIGHT_CLICK,        wxPG_BASE_EVT_PRE_ID+3)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_PAGE_CHANGED,       wxPG_BASE_EVT_PRE_ID+4)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_ITEM_COLLAPSED,     wxPG_BASE_EVT_PRE_ID+5)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_ITEM_EXPANDED,      wxPG_BASE_EVT_PRE_ID+6)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_PG, wxEVT_PG_DOUBLE_CLICK,       wxPG_BASE_EVT_PRE_ID+7)
END_DECLARE_EVENT_TYPES()
#else
    enum {
        wxEVT_PG_SELECTED = wxPG_BASE_EVT_PRE_ID,
        wxEVT_PG_CHANGED,
        wxEVT_PG_HIGHLIGHTED,
        wxEVT_PG_RIGHT_CLICK,
        wxEVT_PG_PAGE_CHANGED,
        wxEVT_PG_ITEM_COLLAPSED,
        wxEVT_PG_ITEM_EXPANDED,
        wxEVT_PG_DOUBLE_CLICK
    };
#endif


#define wxPG_BASE_EVT_TYPE       wxEVT_PG_SELECTED
#define wxPG_MAX_EVT_TYPE        (wxPG_BASE_EVT_TYPE+30)


#ifndef SWIG
typedef void (wxEvtHandler::*wxPropertyGridEventFunction)(wxPropertyGridEvent&);

#define EVT_PG_SELECTED(id, fn)              DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_SELECTED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_CHANGED(id, fn)               DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_CHANGED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_HIGHLIGHTED(id, fn)           DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_HIGHLIGHTED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_RIGHT_CLICK(id, fn)           DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_RIGHT_CLICK, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_DOUBLE_CLICK(id, fn)          DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_DOUBLE_CLICK, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_PAGE_CHANGED(id, fn)          DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_PAGE_CHANGED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_ITEM_COLLAPSED(id, fn)        DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_ITEM_COLLAPSED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_PG_ITEM_EXPANDED(id, fn)         DECLARE_EVENT_TABLE_ENTRY( wxEVT_PG_ITEM_EXPANDED, id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxPropertyGridEventFunction, & fn ), (wxObject *) NULL ),

#define wxPropertyGridEventHandler(A) ((wxObjectEventFunction)(wxEventFunction)(wxPropertyGridEventFunction)&A)

#endif


// -----------------------------------------------------------------------


/** \class wxPropertyGridPopulator
    \ingroup classes
    \brief Allows populating wxPropertyGrid from arbitrary text source.
*/
class WXDLLIMPEXP_PG wxPropertyGridPopulator
{
public:
    /** Constructor.
        \param pg
        Property grid to populate.
        \param popRoot
        Base parent property. Default is root.
    */
    inline wxPropertyGridPopulator(wxPropertyGrid* pg = (wxPropertyGrid*) NULL,
                                   wxPGId popRoot = wxNullProperty)
    {
        Init(pg, popRoot);
    }

    /** Destructor. */
    ~wxPropertyGridPopulator();

    /** Adds a new set of choices with given id, labels and optional values.
        \remarks
        choicesId can be any id unique in source (so it does not conflict
        with sets of choices created before population process).
    */
    void AddChoices(wxPGChoicesId choicesId,
                    const wxArrayString& choiceLabels,
                    const wxArrayInt& choiceValues = wxArrayInt());

    /** Appends a property under current parent.
        \param classname
        Class name of a property. Understands both wxXXXProperty
        and XXX style names. Thus, for example, wxStringProperty
        could be created with class names "wxStringProperty", and
        "String". Short class name of wxPropertyCategory is
        "Category".
        \param label
        Label for property. Use as in constructor functions.
        \param name
        Name for property. Use as in constructor functions.
        \param value
        Value for property is interpreted from this string.
        \param attributes
        Attributes of a property (both pseudo-attributes like
        "Disabled" and "Modified" in addition to real ones
        like "Precision") are read from this string. Is intended
        for string like one generated by GetPropertyAttributes.
        \param choicesId
        If non-zero: Id for set of choices unique in source. Pass
        either id previously given to AddChoices or a new one.
        If new is given, then choiceLabels and choiceValues are
        loaded as the contents for the newly created set of choices.
        \param choiceLabels
        List of choice labels.
        \param choiceValues
        List of choice values.
    */
    wxPGId AppendByClass(const wxString& classname,
                         const wxString& label,
                         const wxString& name = wxPG_LABEL,
                         const wxString& value = wxEmptyString,
                         const wxString& attributes = wxEmptyString,
                         wxPGChoicesId choicesId = (wxPGChoicesId)0,
                         const wxArrayString& choiceLabels = wxArrayString(),
                         const wxArrayInt& choiceValues = wxArrayInt());

    /** Appends a property under current parent. Works just as
        AppendByClass, except accepts value type name instead of
        class name (value type name of a property can be queried using
        wxPropertyGrid::GetPropertyValueType(property)->GetType()).

        \remarks
        <b>Cannot</b> generate property category.
    */
    wxPGId AppendByType(const wxString& valuetype,
                        const wxString& label,
                        const wxString& name = wxPG_LABEL,
                        const wxString& value = wxEmptyString,
                        const wxString& attributes = wxEmptyString,
                        wxPGChoicesId choicesId = (wxPGChoicesId)0,
                        const wxArrayString& choiceLabels = wxArrayString(),
                        const wxArrayInt& choiceValues = wxArrayInt());

    /** Returns id of parent property for which children can currently be added. */
    inline wxPGId GetCurrentParent() const
    {
        return m_curParent;
    }

    /** Returns true if set of choices with given id has already been added. */
    bool HasChoices( wxPGChoicesId id ) const;

    /** Sets the property grid to be populated. */
    inline void SetGrid( wxPropertyGrid* pg )
    {
        m_propGrid = pg;
    }

    /** If possible, sets the property last added as current parent. */
    bool BeginChildren();

    /** Terminates current parent - sets its parent as the new current parent. */
    inline void EndChildren()
    {
        wxASSERT( wxPGIdIsOk(m_curParent) );
        m_curParent = wxPGIdGen(wxPGIdToPtr(m_curParent)->GetParent());
        m_lastProperty = wxPGIdGen((wxPGProperty*)NULL);
    }

protected:

    wxPGId DoAppend(wxPGProperty* p,
                    const wxString& value,
                    const wxString& attributes,
                    wxPGChoicesId choicesId,
                    const wxArrayString& choiceLabels,
                    const wxArrayInt& choiceValues);

    void Init( wxPropertyGrid* pg, wxPGId popRoot );

    /** Used property grid. */
    wxPropertyGrid* m_propGrid;

    /** Population root. */
    wxPGId          m_popRoot;

    /** Parent of currently added properties. */
    wxPGId          m_curParent;

    /** Id of property last added. */
    wxPGId          m_lastProperty;

    /** Hashmap for source-choices-id to wxPGChoicesData mapping. */
    wxPGHashMapP2P  m_dictIdChoices;
};

// -----------------------------------------------------------------------

//
// Undefine macros that are not needed outside propertygrid sources
//
#ifndef __wxPG_SOURCE_FILE__
    #undef wxPG_FL_DESC_REFRESH_REQUIRED
    #undef wxPG_FL_SCROLLBAR_DETECTED
    #undef wxPG_FL_CREATEDSTATE
    #undef wxPG_FL_NOSTATUSBARHELP
    #undef wxPG_FL_SCROLLED
    #undef wxPG_FL_HIDE_STATE
    #undef wxPG_FL_FOCUS_INSIDE_CHILD
    #undef wxPG_FL_FOCUS_INSIDE
    #undef wxPG_FL_MOUSE_INSIDE_CHILD
    #undef wxPG_FL_CUR_USES_CUSTOM_IMAGE
    #undef wxPG_FL_PRIMARY_FILLS_ENTIRE
    #undef wxPG_FL_VALUE_MODIFIED
    #undef wxPG_FL_MOUSE_INSIDE
    #undef wxPG_FL_FOCUSED
    #undef wxPG_FL_MOUSE_CAPTURED
    #undef wxPG_FL_INITIALIZED
    #undef wxPG_FL_ACTIVATION_BY_CLICK
    #undef wxPG_FL_DONT_CENTER_SPLITTER
    #undef wxPG_SUPPORT_TOOLTIPS
    #undef wxPG_DOUBLE_BUFFER
    #undef wxPG_HEAVY_GFX
    #undef wxPG_ICON_WIDTH
    #undef wxPG_USE_RENDERER_NATIVE
// Following are needed by the manager headers
//    #undef wxPGIdGen
//    #undef wxPGPropNameStr
//    #undef wxPGIdToPtr
#endif

// Doxygen special
#ifdef DOXYGEN
    #include "manager.h"
#endif

// -----------------------------------------------------------------------

#endif // __WX_PROPGRID_PROPGRID_H__

