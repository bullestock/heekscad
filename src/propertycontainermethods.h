/** \class wxPropertyContainerMethods
    \ingroup classes
    \brief In order to have most same base methods, both wxPropertyGrid and
    wxPropertyGridManager must derive from this.
*/
class WXDLLIMPEXP_PG wxPropertyContainerMethods
// BM_METHODS
{
public:

    /** Destructor */
    virtual ~wxPropertyContainerMethods() { };

    /** Adds choice to a property that can accept one.
        \remarks
        - If you need to make sure that you modify only the set of choices of
          a single property (and not also choices of other properties with initially
          identical set), call wxPropertyGrid::SetPropertyChoicesPrivate.
        - This usually only works for wxEnumProperty and derivatives (wxFlagsProperty
          can get accept new items but its items may not get updated).
    */
    void AddPropertyChoice( wxPGId id, const wxString& label, int value = wxPG_INVALID_VALUE );
    inline void AddPropertyChoice( wxPGPropNameStr name, const wxString& label, int value = wxPG_INVALID_VALUE )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        AddPropertyChoice(wxPGIdGen(p),label,value);
    }

    /** Inorder to add new items into a property with fixed children (for instance, wxFlagsProperty),
        you need to call this method. After populating has been finished, you need to call EndAddChildren.
    */
    void BeginAddChildren( wxPGId id );
    inline void BeginAddChildren( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        BeginAddChildren(wxPGIdGen(p));
    }

    /** Called after population of property with fixed children has finished.
    */
    void EndAddChildren( wxPGId id );
    inline void EndAddChildren( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        EndAddChildren(wxPGIdGen(p));
    }

    /** Inserts choice to a property that can accept one.

        See AddPropertyChoice for more details.
    */
    void InsertPropertyChoice( wxPGId id, const wxString& label, int index, int value = wxPG_INVALID_VALUE );
    inline void InsertPropertyChoice( wxPGPropNameStr name, const wxString& label, int index, int value = wxPG_INVALID_VALUE )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        InsertPropertyChoice(wxPGIdGen(p),label,index,value);
    }

    /** Deletes choice from a property.

        If selected item is deleted, then the value is set to unspecified.

        See AddPropertyChoice for more details.
    */
    void DeletePropertyChoice( wxPGId id, int index );
    inline void DeletePropertyChoice( wxPGPropNameStr name, int index )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        DeletePropertyChoice(wxPGIdGen(p),index);
    }

    /** Constructs a property. Class used is given as the first
        string argument. It may be either normal property class
        name, such as "wxIntProperty" or a short one such as
        "Int".
    */
    static wxPGProperty* CreatePropertyByClass(const wxString &classname,
                                               const wxString &label,
                                               const wxString &name);

    /** Constructs a property. Value type name used is given as the first
        string argument. It may be "string", "long", etc. Any value returned
        by wxVariant::GetType fits there.

        Otherwise, this is similar as CreatePropertyByClass.
        \remarks
        <b>Cannot</b> generate property category.
    */
    static wxPGProperty* CreatePropertyByType(const wxString &valuetype,
                                              const wxString &label,
                                              const wxString &name);

    /** Deletes a property by id. If category is deleted, all children are automatically deleted as well. */
    void Delete( wxPGId id );

    /** Deletes a property by name. */
    inline void Delete( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        Delete( wxPGIdGen(p) );
    }

    /** Returns id of first child of given property.
        \remarks
        Does not return sub-properties!
    */
    inline wxPGId GetFirstChild( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(wxNullProperty)
        wxPGPropertyWithChildren* pwc = (wxPGPropertyWithChildren*) p;
        if ( pwc->GetParentingType()==0 || pwc->GetParentingType()==-1 || !pwc->GetCount() )
            return wxNullProperty;
        return wxPGIdGen(pwc->Item(0));
    }
    inline wxPGId GetFirstChild( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxNullProperty)
        return GetFirstChild( wxPGIdGen(p) );
    }

    /** Returns next item under the same parent. */
    inline wxPGId GetNextSibling( wxPGId id )
    {
        return wxPropertyGridState::GetNextSibling(id);
    }

    inline wxPGId GetNextSibling( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxNullProperty)
        return wxPropertyGridState::GetNextSibling(wxPGIdGen(p));
    }

    /** Returns comma-delimited string with property's attributes (both
        pseudo-attributes such as "Disabled" and "Modified" and real
        attributes such as "BoolUseCheckbox" - actual names may vary).
        \param flagmask
        Combination of property flags that should be included (in addition
        to any other attributes). For example, to avoid adding Modified
        attribute use ~(wxPG_PROP_MODIFIED).
        \remarks
        Atleast in 1.2.x and earlier this does not return complete list of attributes
        (for example, no floating point precision) and some attributes have
        generic names (such as "Special1" instead of "UseCheckbox" etc)
    */
    inline wxString GetPropertyAttributes( wxPGId id, unsigned int flagmask = 0xFFFF ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(m_emptyString)
        return p->GetAttributes(flagmask);
    }

    /** Sets attributes from a string generated by GetPropertyAttributes.
        \remarks
        Performance may not be top-notch.
    */
    inline static void SetPropertyAttributes( wxPGId id, const wxString& attributes )
    {
        wxPG_PROP_ID_CALL_PROLOG()
        p->SetAttributes(attributes);
    }

    inline void SetPropertyAttributes( wxPGPropNameStr name, const wxString& attributes ) const
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        p->SetAttributes(attributes);
    }

    /** Returns id of property with given name (case-sensitive). If there is no
        property with such name, returned property id is invalid ( i.e. it will return
        false with IsOk method).
        \remarks
        - Sub-properties (i.e. properties which have parent that is not category or
          root) can not be accessed globally by their name. Instead, use
          "<property>.<subproperty>" in place of "<subproperty>".
    */
    wxPGId GetPropertyByName( wxPGPropNameStr name ) const;

    /** Returns id of a sub-property 'subname' of property 'name'. Same as
        calling GetPropertyByNameI(wxT("name.subname")), albeit slightly faster.
    */
    wxPGId GetPropertyByName( wxPGPropNameStr name, wxPGPropNameStr subname ) const;

    /** Returns writable reference to property's list of choices (and relevant
        values). If property does not have any choices, will return reference
        to an invalid set of choices that will return false on IsOk call.
    */
    wxPGChoices& GetPropertyChoices( wxPGId id );
    wxPGChoices& GetPropertyChoices( wxPGPropNameStr name );

    /** Gets name of property's constructor function. */
    inline wxPG_CONST_WXCHAR_PTR GetPropertyClassName( wxPGId id ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(wxPG_CONST_WXCHAR_DEFVAL)
        return p->GetClassName();
    }

    /** Gets name of property's constructor function. */
    inline wxPG_CONST_WXCHAR_PTR GetPropertyClassName( wxPGPropNameStr name ) const
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxPG_CONST_WXCHAR_DEFVAL)
        return p->GetClassName();
    }

#if wxPG_USE_CLIENT_DATA
    /** Returns client data (void*) of a property. */
    inline wxPGProperty::ClientDataType GetPropertyClientData( wxPGId id ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(NULL)
        return p->GetClientData();
    }
    /** Returns client data (void*) of a property. */
    inline wxPGProperty::ClientDataType GetPropertyClientData( wxPGPropNameStr name ) const
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(NULL)
        return p->GetClientData();
    }
#endif

    /** Returns property's editor. */
    inline const wxPGEditor* GetPropertyEditor( wxPGId id ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(NULL)
        return p->GetEditorClass();
    }

    inline const wxPGEditor* GetPropertyEditor( wxPGPropNameStr name ) const
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(NULL)
        return p->GetEditorClass();
    }

    /** Returns property's custom value image (NULL of none). */
    inline wxBitmap* GetPropertyImage( wxPGId id ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(NULL)
        if ( p->GetDataExt() )
            return p->GetDataExt()->m_valueBitmap;
        return (wxBitmap*) NULL;
    }

    inline wxBitmap* GetPropertyImage( wxPGPropNameStr name ) const
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(NULL)
        return GetPropertyImage(wxPGIdGen(p));
    }

    /** Returns property's position under its parent. */
    inline unsigned int GetPropertyIndex( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(INT_MAX)
        return p->GetIndexInParent();
    }

    /** Returns property's position under its parent. */
    inline unsigned int GetPropertyIndex( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(INT_MAX)
        return p->GetIndexInParent();
    }

    /** Returns label of a property. */
    inline const wxString& GetPropertyLabel( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(m_emptyString)
        return p->GetLabel();
    }
    inline const wxString& GetPropertyLabel( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(m_emptyString)
        return p->GetLabel();
    }

    /** Returns name of a property. Note that obviously there is no name-version
        of this member function. */
    inline const wxString& GetPropertyName( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(m_emptyString)
        return p->GetName();
    }

    /** Returns parent item of a property. */
    inline wxPGId GetPropertyParent( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(wxNullProperty)
        return p->GetParent();
    }

    /** Returns parent item of a property. */
    inline wxPGId GetPropertyParent( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxNullProperty)
        return p->GetParent();
    }

    /** Returns priority of a property (wxPG_HIGH or wxPG_LOW). */
    inline int GetPropertyPriority( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(wxPG_HIGH)
        if ( p && p->IsFlagSet(wxPG_PROP_HIDEABLE) )
            return wxPG_LOW;
        return wxPG_HIGH;
    }

    /** Returns priority of a property (wxPG_HIGH or wxPG_LOW). */
    inline int GetPropertyPriority( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxPG_HIGH)
        return GetPropertyPriority(wxPGIdGen(p));
    }

    /** Returns pointer to a property.
    */
    inline wxPGProperty* GetPropertyPtr( wxPGId id ) const { return wxPGIdToPtr(id); }

    /** Returns pointer to a property.
    */
    inline wxPGProperty* GetPropertyPtr( wxPGPropNameStr name ) const
    {
        return wxPGIdToPtr(GetPropertyByName(name));
    }

    /** Returns help string associated with a property. */
    inline wxString GetPropertyHelpString( wxPGId id ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(m_emptyString)
        return p->GetHelpString();
    }

    /** Returns help string associated with a property. */
    inline wxString GetPropertyHelpString( wxPGPropNameStr name ) const
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(m_emptyString)
        return p->GetHelpString();
    }

    /** Returns short name for property's class. For example,
        "wxPropertyCategory" translates to "Category" and "wxIntProperty"
        to "Int".
    */
    wxPG_PYTHON_STATIC wxString GetPropertyShortClassName( wxPGId id );

#if wxUSE_VALIDATORS
    /** Returns validator of a property as a reference, which you
        can pass to any number of SetPropertyValidator.
    */
    inline wxValidator* GetPropertyValidator( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(NULL)
        return p->GetValidator();
    }
    inline wxValidator* GetPropertyValidator( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(NULL)
        return p->GetValidator();
    }
#endif

#ifndef SWIG
    /** Returns value as wxVariant. To get wxObject pointer from it,
        you will have to use WX_PG_VARIANT_TO_WXOBJECT(VARIANT,CLASSNAME) macro.

        If property value is unspecified, Null variant is returned.
    */
    inline wxVariant GetPropertyValue( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(wxVariant())
        return p->GetValueAsVariant();
    }

    /** Returns value as wxVariant. To get wxObject pointer from it,
        you will have to use WX_PG_VARIANT_TO_WXOBJECT(VARIANT,CLASSNAME) macro.

        If property value is unspecified, Null variant is returned.
    */
    inline wxVariant GetPropertyValue( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxVariant())
        return p->GetValueAsVariant();
    }
#endif

    wxPG_PYTHON_STATIC wxString GetPropertyValueAsString( wxPGId id ) wxPG_GETVALUE_CONST;
    wxPG_PYTHON_STATIC long GetPropertyValueAsLong( wxPGId id ) wxPG_GETVALUE_CONST;
#ifndef SWIG
    wxPG_PYTHON_STATIC inline int GetPropertyValueAsInt( wxPGId id ) wxPG_GETVALUE_CONST { return (int)GetPropertyValueAsLong(id); }
#endif
    wxPG_PYTHON_STATIC bool GetPropertyValueAsBool( wxPGId id ) wxPG_GETVALUE_CONST;
    wxPG_PYTHON_STATIC double GetPropertyValueAsDouble( wxPGId id ) wxPG_GETVALUE_CONST;
    wxPG_PYTHON_STATIC const wxObject* GetPropertyValueAsWxObjectPtr( wxPGId id ) wxPG_GETVALUE_CONST;
    wxPG_PYTHON_STATIC void* GetPropertyValueAsVoidPtr( wxPGId id ) wxPG_GETVALUE_CONST;

#define wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(TYPENAME, DEFVAL) \
    wxPG_PROP_ID_CALL_PROLOG_RETVAL(DEFVAL) \
    if ( wxStrcmp(p->GetValueTypePtr()->GetCustomTypeName(),TYPENAME) != 0 ) \
    { \
        wxPGGetFailed(p,TYPENAME); \
        return DEFVAL; \
    }

#if !wxPG_PGVARIANT_IS_VARIANT
    wxPG_PYTHON_STATIC const wxArrayString& GetPropertyValueAsArrayString( wxPGId id ) wxPG_GETVALUE_CONST;
#else
    wxPG_PYTHON_STATIC inline wxArrayString GetPropertyValueAsArrayString( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("arrstring"), wxArrayString())
        return wxPGVariantToArrayString(p->DoGetValue());
    }
#endif

#if !wxPG_PGVARIANT_IS_VARIANT
    wxPG_PYTHON_STATIC inline const wxPoint& GetPropertyValueAsPoint( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxPoint"), *((const wxPoint*)NULL))
        return wxPGVariantToWxPoint(p->DoGetValue());
    }
#else
    wxPG_PYTHON_STATIC inline wxPoint GetPropertyValueAsPoint( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxPoint"), wxPoint())
        return wxPGVariantToWxPoint(p->DoGetValue());
    }
#endif

#if !wxPG_PGVARIANT_IS_VARIANT
    wxPG_PYTHON_STATIC inline const wxSize& GetPropertyValueAsSize( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxSize"), *((const wxSize*)NULL))
        return wxPGVariantToWxSize(p->DoGetValue());
    }
#else
    wxPG_PYTHON_STATIC inline wxSize GetPropertyValueAsSize( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxSize"), wxSize())
        return wxPGVariantToWxSize(p->DoGetValue());
    }
#endif

#if !wxPG_PGVARIANT_IS_VARIANT
    wxPG_PYTHON_STATIC inline const wxLongLong& GetPropertyValueAsLongLong( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxLongLong"), *((const wxLongLong*)NULL))
        return wxPGVariantToWxLongLong(p->DoGetValue());
    }
#else
    wxPG_PYTHON_STATIC inline wxLongLong GetPropertyValueAsLongLong( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxLongLong"), wxLongLong())
        return wxPGVariantToWxLongLong(p->DoGetValue());
    }
#endif

#if !wxPG_PGVARIANT_IS_VARIANT
    wxPG_PYTHON_STATIC inline const wxULongLong& GetPropertyValueAsULongLong( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxULongLong"), *((const wxULongLong*)NULL))
        return wxPGVariantToWxULongLong(p->DoGetValue());
    }
#else
    wxPG_PYTHON_STATIC inline wxULongLong GetPropertyValueAsULongLong( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxULongLong"), wxULongLong())
        return wxPGVariantToWxULongLong(p->DoGetValue());
    }
#endif

#if !wxPG_PGVARIANT_IS_VARIANT
    wxPG_PYTHON_STATIC inline const wxArrayInt& GetPropertyValueAsArrayInt( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxArrayInt"), m_empty_int_array)
        return wxPGVariantToArrayInt(p->DoGetValue());
    }
#else
    wxPG_PYTHON_STATIC inline wxArrayInt GetPropertyValueAsArrayInt( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_GETPROPVAL_CALL_PROLOG_RETVAL(wxT("wxArrayInt"), m_empty_int_array)
        wxArrayInt arr = wxPGVariantToArrayInt(p->DoGetValue());
        return arr;
    }
#endif

#if wxUSE_DATETIME
    wxPG_PYTHON_STATIC inline wxDateTime GetPropertyValueAsDateTime( wxPGId id ) wxPG_GETVALUE_CONST
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(wxDateTime())

        if ( wxStrcmp(p->GetValueTypePtr()->GetCustomTypeName(),wxT("datetime")) != 0 )
        {
            wxPGGetFailed(p,wxT("datetime"));
            return wxDateTime();
        }
        return p->DoGetValue().GetDateTime();
    }
#endif

#ifdef __WXPYTHON__
    wxPG_PYTHON_STATIC PyObject* GetPropertyValueAsPyObject( wxPGId id ) wxPG_GETVALUE_CONST;
#endif

    inline wxString GetPropertyValueAsString( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsString( GetPropertyByNameI(name) );
    }
    inline long GetPropertyValueAsLong( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsLong( GetPropertyByNameI(name) );
    }
#ifndef SWIG
    inline int GetPropertyValueAsInt( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsInt( GetPropertyByNameI(name) );
    }
#endif
    inline bool GetPropertyValueAsBool( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsBool( GetPropertyByNameI(name) );
    }
    inline double GetPropertyValueAsDouble( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsDouble( GetPropertyByNameI(name) );
    }
    inline const wxObject* GetPropertyValueAsWxObjectPtr ( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsWxObjectPtr( GetPropertyByNameI(name) );
    }
#if !wxPG_PGVARIANT_IS_VARIANT
    inline const wxArrayString& GetPropertyValueAsArrayString ( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsArrayString( GetPropertyByNameI(name) );
    }
    inline const wxPoint& GetPropertyValueAsPoint( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsPoint( GetPropertyByNameI(name) );
    }
    inline const wxSize& GetPropertyValueAsSize( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsSize( GetPropertyByNameI(name) );
    }
    inline const wxArrayInt& GetPropertyValueAsArrayInt( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsArrayInt( GetPropertyByNameI(name) );
    }
#else
    inline wxArrayString GetPropertyValueAsArrayString ( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsArrayString( GetPropertyByNameI(name) );
    }
    inline wxPoint GetPropertyValueAsPoint( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsPoint( GetPropertyByNameI(name) );
    }
    inline wxSize GetPropertyValueAsSize( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsSize( GetPropertyByNameI(name) );
    }
    inline wxArrayInt GetPropertyValueAsArrayInt( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsArrayInt( GetPropertyByNameI(name) );
    }
#endif
#if wxUSE_DATETIME
    inline wxDateTime GetPropertyValueAsDateTime( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsDateTime( GetPropertyByNameI(name) );
    }
#endif
#ifdef __WXPYTHON__
    inline PyObject* GetPropertyValueAsPyObject( wxPGPropNameStr name ) const
    {
        return GetPropertyValueAsPyObject( GetPropertyByNameI(name) );
    }
#endif

    /** Returns a wxPGValueType class instance that describes
        the property's data type.
    */
    wxPG_VALUETYPE_MSGVAL GetPropertyValueType( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(wxPG_VALUETYPE(none))
        return p->GetValueType();
    }
    wxPG_VALUETYPE_MSGVAL GetPropertyValueType( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxPG_VALUETYPE(none))
        return p->GetValueType();
    }

    /** Returns property value type name.
    */
    inline wxString GetPVTN( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(m_emptyString)
        const wxPGValueType* vt = p->GetValueTypePtr();
        return vt->GetCustomTypeName();
    }

    inline wxString GetPVTN( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(m_emptyString)
        const wxPGValueType* vt = p->GetValueTypePtr();
        return vt->GetCustomTypeName();
    }

    /** Returns property value type identifier.
    */
    inline size_t GetPVTI( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(0)
        const wxPGValueType* vt = p->GetValueTypePtr();
        return size_t(vt);
    }

    inline size_t GetPVTI( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(0)
        const wxPGValueType* vt = p->GetValueTypePtr();
        return size_t(vt);
    }

#ifndef SWIG
    inline wxPropertyGridState* GetState() const { return m_pState; }
#endif

    /** Returns value type class instance for given type name.
    */
    static wxPGValueType* GetValueType( const wxString &type );

#if wxPG_VALUETYPE_IS_STRING
    /** Return value type class instance for given value type class name.
    */
    static wxPGValueType* GetValueTypeByName( const wxString &className );
#endif

    /** Hides or reveals a property.
        \param hide
        If true, hides property, otherwise reveals it.
        \remarks
        Hiding properties is not compatible with priority system. Using both
        at the same time will yield unpredictable results.
    */
    bool HideProperty( wxPGId id, bool hide = true );

    inline bool HideProperty( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return HideProperty(wxPGIdGen(p));
    }

#if wxPG_INCLUDE_ADVPROPS
    /** Initializes additional property editors (SpinCtrl etc.). Causes references
        to most object files in the library, so calling this may cause significant increase
        in executable size when linking with static library.
    */
    static void RegisterAdditionalEditors();
#else
    static inline void RegisterAdditionalEditors() { }
#endif

#if wxPG_INCLUDE_ADVPROPS
    /** Initializes *all* property types. Causes references to most object
        files in the library, so calling this may cause significant increase
        in executable size when linking with static library.
    */
    static void InitAllTypeHandlers();
#else
    static inline void InitAllTypeHandlers() { }
#endif

    /** Returns true if property is enabled. */
    inline bool IsPropertyEnabled( wxPGId id ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(false)
        return (!(p->GetFlags() & wxPG_PROP_DISABLED))?true:false;
    }

    /** Returns true if property is enabled. */
    inline bool IsPropertyEnabled( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return (!(p->GetFlags() & wxPG_PROP_DISABLED))?true:false;
    }

    /** Returns true if property is shown (ie. hideproperty with true not called for it). */
    inline bool IsPropertyShown( wxPGId id ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(false)
        return (!(p->GetFlags() & wxPG_PROP_HIDEABLE))?true:false;
    }

    /** Returns true if property is shown (ie. hideproperty with true not called for it). */
    inline bool IsPropertyShown( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return (!(p->GetFlags() & wxPG_PROP_HIDEABLE))?true:false;
    }

    /** Returns true if property's value type has name typestr. */
    inline bool IsPropertyValueType( wxPGId id, const wxChar* typestr )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(false)
        return (wxStrcmp(p->GetValueTypePtr()->GetTypeName(),typestr) == 0);
    }

#if !wxPG_VALUETYPE_IS_STRING
    /** Returns true if property's value type is valuetype */
    inline bool IsPropertyValueType( wxPGId id, const wxPGValueType* valuetype )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(false)
        return ( p->GetValueTypePtr() == valuetype );
    }
#endif

    /** Returns true if property's value type has same name as a class. */
    inline bool IsPropertyValueType( wxPGId id, const wxClassInfo* classinfo )
    {
        return IsPropertyValueType(id,classinfo->GetClassName());
    }

    /** Returns true if property's value type has name typestr. */
    inline bool IsPropertyValueType( wxPGPropNameStr name, const wxChar* typestr )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return (wxStrcmp(p->GetValueTypePtr()->GetTypeName(),typestr) == 0);
    }

#if !wxPG_VALUETYPE_IS_STRING
    /** Returns true if property's value type is valuetype */
    inline bool IsPropertyValueType( wxPGPropNameStr name, const wxPGValueType* valuetype )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return ( p->GetValueType() == valuetype );
    }
#endif

    /** Returns true if property's value type has same name as a class. */
    inline bool IsPropertyValueType( wxPGPropNameStr name, const wxClassInfo* classinfo )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return IsPropertyValueType(wxPGIdGen(p),classinfo->GetClassName());
    }

    /** Returns true if given property is expanded. Naturally, always returns false
        for properties that cannot be expanded.
    */
    static bool IsPropertyExpanded( wxPGId id );
    inline bool IsPropertyExpanded( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return IsPropertyExpanded(wxPGIdGen(p));
    }

    /** Returns true if property is of certain type.
        \param info
        Preferably use WX_PG_CLASSINFO(PROPERTYNAME). Alternative is
        PROPERTYNAMEClassInfo.
    */
    static inline bool IsPropertyKindOf( wxPGId id, wxPGPropertyClassInfo& info )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(false)
        return p->IsKindOf(info);
    }
    inline bool IsPropertyKindOf( wxPGPropNameStr name, wxPGPropertyClassInfo& info )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return p->IsKindOf(info);
    }

    /** Returns true if property has been modified after value set or modify flag
        clear by software.

        NOTE: Try to use IsPropertyModified instead.
    */
    inline bool IsModified( wxPGId id ) const
    {
        return IsPropertyModified(id);
    }

    inline bool IsModified( wxPGPropNameStr name )
    {
        return IsPropertyModified(name);
    }

    /** Returns true if property is a category. */
    inline bool IsPropertyCategory( wxPGId id ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(false)
        return (p->GetParentingType()>0)?true:false;
    }

    inline bool IsPropertyCategory( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return (p->GetParentingType()>0)?true:false;
    }

    /** Returns true if property has been modified after value set or modify flag
        clear by software.
    */
    inline bool IsPropertyModified( wxPGId id ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(false)
        return ( (p->GetFlags() & wxPG_PROP_MODIFIED) ? true : false );
    }
    inline bool IsPropertyModified( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return ( (p->GetFlags() & wxPG_PROP_MODIFIED) ? true : false );
    }

    /** Returns true if property value is set to unspecified.
    */
#ifdef wxPG_COMPATIBILITY_1_0_0
    inline bool IsPropertyValueUnspecified( wxPGId id ) const
#else
    inline bool IsPropertyUnspecified( wxPGId id ) const
#endif
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(false)
        return ( (p->GetFlags() & wxPG_PROP_UNSPECIFIED) ? true : false );
    }
#ifdef wxPG_COMPATIBILITY_1_0_0
    inline bool IsPropertyValueUnspecified( wxPGPropNameStr name )
    {
        return IsPropertyValueUnspecified(GetPropertyByNameI(name));
    }
#else
    inline bool IsPropertyUnspecified( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return IsPropertyUnspecified(wxPGIdGen(p));
    }
#endif

    /** Basic property classes are registered by the default, but this
        registers advanced ones as well.
    */
    static void RegisterAdvancedPropertyClasses();

    /** Registers property class info with specific name. Preferably use
        wxPGRegisterPropertyClass(PROPERTYNAME) macro.
    */
    static bool RegisterPropertyClass( const wxChar* name, wxPGPropertyClassInfo* classinfo );

    /** Replaces property with id with newly created property. For example,
        this code replaces existing property named "Flags" with one that
        will have different set of items:
        \code
            pg->ReplaceProperty(wxT("Flags"),
                wxFlagsProperty(wxT("Flags"),wxPG_LABEL,newItems))
        \endcode
        For more info, see wxPropertyGrid::Insert.
    */
    wxPGId ReplaceProperty( wxPGId id, wxPGProperty* property );

    inline wxPGId ReplaceProperty( wxPGPropNameStr name, wxPGProperty* property )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxNullProperty)
        return ReplaceProperty(wxPGIdGen(p),property);
    }

    /** Lets user to set the strings listed in the choice dropdown of a wxBoolProperty.
        Defaults are "True" and "False", so changing them to, say, "Yes" and "No" may
        be useful in some less technical applications.
    */
    static void SetBoolChoices( const wxChar* true_choice, const wxChar* false_choice );

    /** Set choices of a property to specified set of labels and values.
    */
    static inline void SetPropertyChoices(wxPGId id, wxPGChoices& choices)
    {
        wxPG_PROP_ID_CALL_PROLOG()
        p->SetChoices(choices);
    }


    /** Set choices of a property to specified set of labels and values.
    */
    inline void SetPropertyChoices(wxPGPropNameStr name, wxPGChoices& choices)
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        p->SetChoices(choices);
    }

    /** If property's set of choices is shared, then calling this method converts
        it to private.
    */
    inline void SetPropertyChoicesExclusive( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG()
        p->SetChoicesExclusive();
    }
    inline void SetPropertyChoicesExclusive( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        p->SetChoicesExclusive();
    }

    /** Sets an attribute of a property. Ids and relevants values are totally
        specific to property classes and may affect either the given instance
        or all instances of that class. See \ref attrids for list of built-in
        attributes.
        \param argFlags
        Optional. Use wxPG_RECURSE to set the attribute to child properties
        as well.
        \remarks
        wxVariant doesn't have int constructor (as of 2.5.4), so <b>you will
        need to cast int values (including most numeral constants) to long</b>.
    */
    inline void SetPropertyAttribute( wxPGId id, int attrid, wxVariant value, long argFlags = 0 )
    {
        DoSetPropertyAttribute(id,attrid,value,argFlags);
    }
    inline void SetPropertyAttribute( wxPGPropNameStr name, int attrid, wxVariant value, long argFlags = 0  )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        DoSetPropertyAttribute(wxPGIdGen(p),attrid,value,argFlags);
    }

#ifndef SWIG
    /** Sets editor control of a property. As editor argument, use
        wxPG_EDITOR(EditorName), where basic built-in editor names are TextCtrl, Choice,
        ComboBox, CheckBox, TextCtrlAndButton, and ChoiceAndButton. Additional editors
        include SpinCtrl and DatePickerCtrl, which also require wxPropertyGrid::RegisterAdditionalEditors()
        call prior using.
    */
    inline void SetPropertyEditor( wxPGId id, const wxPGEditor* editor )
    {
        wxPG_PROP_ID_CALL_PROLOG()
        wxCHECK_RET( editor, wxT("unknown/NULL editor") );
        p->SetEditor(editor);
        RefreshProperty(p);
    }
    inline void SetPropertyEditor( wxPGPropNameStr name, const wxPGEditor* editor )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        wxCHECK_RET( editor, wxT("unknown/NULL editor") );
        p->SetEditor(editor);
        RefreshProperty(p);
    }
#endif // #ifndef SWIG

    /** Sets editor control of a property. As editor argument, use
        editor name string, such as wxT("TextCtrl") or wxT("Choice").
    */
    inline void SetPropertyEditor( wxPGId id, const wxString& editorName )
    {
        SetPropertyEditor(id,GetEditorByName(editorName));
    }
    inline void SetPropertyEditor( wxPGPropNameStr name, const wxString& editorName )
    {
        SetPropertyEditor(name,GetEditorByName(editorName));
    }

#if wxPG_USE_CLIENT_DATA
    /** Sets client data (void*) of a property.
        \remarks
        This untyped client data has to be deleted manually.
    */
    inline void SetPropertyClientData( wxPGId id, wxPGProperty::ClientDataType clientData )
    {
        wxPG_PROP_ID_CALL_PROLOG()
        p->SetClientData(clientData);
    }
    /** Sets client data (void*) of a property.
        \remarks
        This untyped client data has to be deleted manually.
    */
    inline void SetPropertyClientData( wxPGPropNameStr name, wxPGProperty::ClientDataType clientData )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        p->SetClientData(clientData);
    }
#endif

    /** Associates the help string with property.
        \remarks
        By default, text is shown either in the manager's "description"
        text box or in the status bar. If extra window style wxPG_EX_HELP_AS_TOOLTIPS
        is used, then the text will appear as a tooltip.
    */
    inline void SetPropertyHelpString( wxPGId id, const wxString& helpString )
    {
        wxPG_PROP_ID_CALL_PROLOG()
        p->SetHelpString(helpString);
    }

    inline void SetPropertyHelpString( wxPGPropNameStr name, const wxString& helpString )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        p->SetHelpString(helpString);
    }

    /** Set wxBitmap in front of the value.
        \remarks
        - Bitmap will be ignored if property class has implemented OnCustomPaint.
        - Bitmap will be scaled to a size returned by wxPropertyGrid::GetImageSize();
    */
    inline void SetPropertyImage( wxPGId id, wxBitmap& bmp )
    {
        wxPG_PROP_ID_CALL_PROLOG()
        p->SetValueImage(bmp);
        RefreshProperty(p);
    }

    inline void SetPropertyImage( wxPGPropNameStr name, wxBitmap& bmp )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        p->SetValueImage(bmp);
        RefreshProperty(p);
    }

    /** Sets max length of property's text.
    */
    bool SetPropertyMaxLength( wxPGId id, int maxLen );

    /** Sets max length of property's text.
    */
    inline bool SetPropertyMaxLength( wxPGPropNameStr name, int maxLen )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return SetPropertyMaxLength(wxPGIdGen(p),maxLen);
    }

    /** Property is to be hidden/shown when hider button is toggled or
        when wxPropertyGrid::Compact is called.
    */
    bool SetPropertyPriority( wxPGId id, int priority );

    /** Property is to be hidden/shown when hider button is toggled or
        when wxPropertyGrid::Compact is called.
    */
    inline bool SetPropertyPriority( wxPGPropNameStr name, int priority )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return SetPropertyPriority(wxPGIdGen(p),priority);
    }

#if wxUSE_VALIDATORS
    /** Sets validator of a property. For example
        \code
          // Allow property's value range from -100 to 100
          wxIntPropertyValidator validator(-100,100);
          wxPGId id = pg->Append( wxIntProperty(wxT("Value 1",wxPG_LABEL,0)) );
          pg->SetPropertyValidator( id, validator );
        \endcode
    */
    inline void SetPropertyValidator( wxPGId id, const wxValidator& validator )
    {
        wxPG_PROP_ID_CALL_PROLOG()
        p->SetValidator(validator);
    }
    inline void SetPropertyValidator( wxPGPropNameStr name, const wxValidator& validator )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        p->SetValidator(validator);
    }
#endif

    /** Toggles priority of a property between wxPG_HIGH and wxPG_LOW.
    */
    inline void TogglePropertyPriority( wxPGId id )
    {
        int priority = wxPG_LOW;
        if ( GetPropertyPriority(id) == wxPG_LOW )
            priority = wxPG_HIGH;
        SetPropertyPriority(id,priority);
    }

    /** Toggles priority of a property between wxPG_HIGH and wxPG_LOW.
    */
    inline void TogglePropertyPriority( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        TogglePropertyPriority(wxPGIdGen(p));
    }

#ifdef SWIG
    %pythoncode {
        def MapType(class_,factory):
            "Registers Python type/class to property mapping.\n\nfactory: Property builder function/class."
            global _type2property
            try:
                mappings = _type2property
            except NameError:
                raise AssertionError("call only after a propertygrid or manager instance constructed")

            mappings[class_] = factory


        def DoDefaultTypeMappings(self):
            "Map built-in properties."
            global _type2property
            try:
                mappings = _type2property

                return
            except NameError:
                mappings = {}
                _type2property = mappings

            mappings[str] = StringProperty
            mappings[unicode] = StringProperty
            mappings[int] = IntProperty
            mappings[float] = FloatProperty
            mappings[bool] = BoolProperty
            mappings[list] = ArrayStringProperty
            mappings[tuple] = ArrayStringProperty
            mappings[wx.Font] = FontProperty
            mappings[wx.Colour] = ColourProperty
            mappings[wx.Size] = SizeProperty
            mappings[wx.Point] = PointProperty
            mappings[wx.FontData] = FontDataProperty


        def GetPropertyValue(self,p):
            "Returns Python object value for property.\n\nCaches getters on value type id basis for performance purposes."
            global _vt2getter
            vtid = self.GetPVTI(p)
            if not vtid:
                raise TypeError("Property '%s' doesn't have valid value type"%(p.GetName()))
            try:
                getter = _vt2getter[vtid]
            except KeyError:

                cls = PropertyContainerMethods
                vtn = self.GetPVTN(p)

                if vtn == 'long':
                    getter = cls.GetPropertyValueAsLong
                elif vtn == 'string':
                    getter = cls.GetPropertyValueAsString
                elif vtn == 'double':
                    getter = cls.GetPropertyValueAsDouble
                elif vtn == 'bool':
                    getter = cls.GetPropertyValueAsBool
                elif vtn == 'arrstring':
                    getter = cls.GetPropertyValueAsArrayString
                elif vtn == 'wxArrayInt':
                    getter = cls.GetPropertyValueAsArrayInt
                elif vtn == 'PyObject':
                    getter = cls.GetPropertyValueAsPyObject
                elif vtn == 'datetime':
                    getter = cls.GetPropertyValueAsDateTime
                elif vtn == 'wxPoint':
                    getter = cls.GetPropertyValueAsPoint
                elif vtn == 'wxSize':
                    getter = cls.GetPropertyValueAsSize
                elif vtn.startswith('wx'):
                    getter = cls.GetPropertyValueAsWxObjectPtr
                elif not vtn:
                    if p:
                        raise ValueError("no property with name '%s'"%p)
                    else:
                        raise ValueError("NULL property")
                else:
                    raise AssertionError("Unregistered property grid value type '%s'"%vtn)
                _vt2getter[vtid] = getter
            return getter(self,p)


        def SetPropertyValueArrstr(self,p,v):
            "NB: We must implement this in Python because SWIG has problems combining"
            "    conversion of list to wxArrayXXX and overloaded arguments."
            if not isinstance(p,basestring):
                self._SetPropertyValueArrstr(p,v)
            else:
                self._SetPropertyValueArrstr(self.GetPropertyByNameI(p),v)


        def SetPropertyValueArrint(self,p,v):
            "NB: We must implement this in Python because SWIG has problems combining"
            "    conversion of list to wxArrayXXX and overloaded arguments."
            if not isinstance(p,basestring):
                self._SetPropertyValueArrint(p,v)
            else:
                self._SetPropertyValueArrint(self.GetPropertyByNameI(p),v)


        def SetPropertyValue(self,p,v):
            "Set property value from Python object.\n\nCaches setters on value type id basis for performance purposes."
            cls = self.__class__
            if not isinstance(v,basestring):
                _vt2setter = cls._vt2setter
                vtid = self.GetPVTI(p)
                try:
                    setter = _vt2setter[vtid]
                except KeyError:

                    vtn = self.GetPVTN(p)

                    if vtn == 'long':
                        setter = cls.SetPropertyValueLong
                    elif vtn == 'string':
                        setter = cls.SetPropertyValueString
                    elif vtn == 'double':
                        setter = cls.SetPropertyValueDouble
                    elif vtn == 'bool':
                        setter = cls.SetPropertyValueBool
                    elif vtn == 'arrstring':
                        setter = cls.SetPropertyValueArrstr
                    elif vtn == 'wxArrayInt':
                        setter = cls.SetPropertyValueArrint
                    elif vtn == 'PyObject':
                        setter = cls.SetPropertyValuePyObject
                    elif vtn == 'datetime':
                        setter = cls.SetPropertyValueDatetime
                    elif vtn == 'wxPoint':
                        setter = cls.SetPropertyValuePoint
                    elif vtn == 'wxSize':
                        setter = cls.SetPropertyValueSize
                    elif vtn == 'wxLongLong':
                        setter = cls.SetPropertyValueLongLong
                    elif vtn == 'wxULongLong':
                        setter = cls.SetPropertyValueULongLong
                    elif vtn.startswith('wx'):
                        setter = cls.SetPropertyValueWxObjectPtr
                    elif not vtn:
                        if p:
                            raise ValueError("no property with name '%s'"%p)
                        else:
                            raise ValueError("NULL property")
                    else:
                        raise AssertionError("Unregistered property grid value type '%s'"%vtn)
                    _vt2setter[vtid] = setter
            else:
                setter = cls.SetPropertyValueString

            return setter(self,p,v)


        def DoDefaultValueTypeMappings(self):
            "Map pg value type ids to getter methods."
            global _vt2getter
            try:
                vt2getter = _vt2getter

                return
            except NameError:
                vt2getter = {}
                _vt2getter = vt2getter


        def _GetValues(self,parent,fc,dict_,getter):
            p = fc

            while p:
                pfc = self.GetFirstChild(p)
                if pfc:
                    self._GetValues(p,pfc,dict_,getter)
                else:
                    dict_[p.GetName()] = getter(p)

                p = self.GetNextSibling(p)


        def GetPropertyValues(self,dict_=None,as_strings=False):
            "Returns values in the grid."
            ""
            "dict_: if not given, then a new one is created. dict_ can be"
            "  object as well, in which case it's __dict__ is used."
            "as_strings: if True, then string representations of values"
            "  are fetched instead of native types. Useful for config and such."
            ""
            "Return value: dictionary with values. It is always a dictionary,"
            "so if dict_ was object with __dict__ attribute, then that attribute"
            "is returned."

            if dict_ is None:
                dict_ = {}
            elif hasattr(dict_,'__dict__'):
                dict_ = dict_.__dict__

            if not as_strings:
                getter = self.GetPropertyValue
            else:
                getter = self.GetPropertyValueAsString

            root = self.GetRoot()
            self._GetValues(root,self.GetFirstChild(root),dict_,getter)

            return dict_

        GetValues = GetPropertyValues


        def SetPropertyValues(self,dict_):
            "Sets property values from dict_, which can be either\ndictionary or an object with __dict__ attribute."
            ""
            "autofill: If true, keys with not relevant properties"
            "  are auto-created. For more info, see AutoFill."
            ""
            "Notes:"
            "  * Keys starting with underscore are ignored."

            autofill = False

            if dict_ is None:
                dict_ = {}
            elif hasattr(dict_,'__dict__'):
                dict_ = dict_.__dict__

            def set_sub_obj(k0,dict_):
                for k,v in dict_.iteritems():
                    if k[0] != '_':
                        try:
                            self.SetPropertyValue(k,v)
                        except:
                            try:
                                if autofill:
                                    self._AutoFillOne(k0,k,v)
                                    continue
                            except:
                                if isinstance(v,dict):
                                    set_sub_obj(k,v)
                                elif hasattr(v,'__dict__'):
                                    set_sub_obj(k,v.__dict__)


            cur_page = False
            is_manager = isinstance(self,PropertyGridManager)

            try:
                set_sub_obj(self.GetRoot(),dict_)
            except:
                import traceback
                traceback.print_exc()

            self.Refresh()


        SetValues = SetPropertyValues


        def _AutoFillMany(self,cat,dict_):
            for k,v in dict_.iteritems():
                self._AutoFillOne(cat,k,v)


        def _AutoFillOne(self,cat,k,v):
            global _type2property

            factory = _type2property.get(v.__class__,None)

            if factory:
                self.AppendIn( cat, factory(k,k,v) )
            elif hasattr(v,'__dict__'):
                cat2 = self.AppendIn( cat, PropertyCategory(k) )
                self._AutoFillMany(cat2,v.__dict__)
            elif isinstance(v,dict):
                cat2 = self.AppendIn( cat, PropertyCategory(k) )
                self._AutoFillMany(cat2,v)
            elif not k.startswith('_'):
                raise AssertionError("member '%s' is of unregisted type/class '%s'"%(k,v.__class__))


        def AutoFill(self,obj,parent=None):
            "Clears properties and re-fills to match members and\nvalues of given object or dictionary obj."

            self.edited_objects[parent] = obj

            cur_page = False
            is_manager = isinstance(self,PropertyGridManager)

            if not parent:
                if is_manager:
                    page = self.GetTargetPage()
                    self.ClearPage(page)
                    parent = self.GetPageRoot(page)
                else:
                    self.Clear()
                    parent = self.GetRoot()
            else:
                p = self.GetFirstChild(parent)
                while p:
                    self.Delete(p)
                    p = self.GetNextSibling(p)

            if not is_manager or page == self.GetSelectedPage():
                self.Freeze()
                cur_page = True

            try:
                self._AutoFillMany(parent,obj.__dict__)
            except:
                import traceback
                traceback.print_exc()

            if cur_page:
                self.Thaw()


        def RegisterEditor(self, editor, editorName=None):
            "Transform class into instance, if necessary."
            if not isinstance(editor, PGEditor):
                editor = editor()
            if not editorName:
                editorName = editor.__class__.__name__
            try:
                self._editor_instances.append(editor)
            except:
                self._editor_instances = [editor]
            RegisterEditor(editor, editorName)

    }
#endif

	/** Sets property as read-only. It's value cannot be changed by the user, but the
	    editor may still be created for copying purposes.
	*/
	void SetPropertyReadOnly( wxPGId id, bool readOnly = true )
	{
        wxPG_PROP_ID_CALL_PROLOG()
		if ( readOnly )
			p->SetFlag(wxPG_PROP_READONLY);
		else
			p->ClearFlag(wxPG_PROP_READONLY);
	}

	/** Sets property as read-only. It's value cannot be changed by the user, but the
	    editor may still be created for copying purposes.
	*/
	void SetPropertyReadOnly( wxPGPropNameStr name, bool readOnly = true )
	{
        wxPG_PROP_NAME_CALL_PROLOG()
		if ( readOnly )
			p->SetFlag(wxPG_PROP_READONLY);
		else
			p->ClearFlag(wxPG_PROP_READONLY);
	}

    // GetPropertyByNameI With nice assertion error message.
    wxPGId GetPropertyByNameA( wxPGPropNameStr name ) const;

#ifndef SWIG

    static wxPGEditor* GetEditorByName( const wxString& editorName );
    
protected:

    // Deriving classes must set this (it must be only or current page).
    wxPropertyGridState*         m_pState;

    // Default call's m_pState's BaseGetPropertyByName
    virtual wxPGId DoGetPropertyByName( wxPGPropNameStr name ) const;

    virtual void RefreshProperty( wxPGProperty* p ) = 0;

    // Intermediate version needed due to wxVariant copying inefficiency
    static void DoSetPropertyAttribute( wxPGId id, int attrid, wxVariant& value, long argFlags );

    // Empty string object to return from member functions returning const wxString&.
    wxString                    m_emptyString;

    static wxArrayInt m_empty_int_array;
    static wxArrayString m_empty_string_array;
    
#endif // #ifndef SWIG
};
