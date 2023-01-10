#define wxPG_USE_STATE  m_pState

/** \class wxPropertyGrid
	\ingroup classes
    \brief
    wxPropertyGrid is a specialized two-column grid for editing properties
    such as strings, numbers, flagsets, fonts, and colours. wxPropertySheet
    used to do the very same thing, but it hasn't been updated for a while
    and it is currently deprecated.

    wxPropertyGrid is modeled after .NET propertygrid (hence the name),
    and thus features are similar. However, inorder to keep the widget lightweight,
    it does not (and will not) have toolbar for mode and page selection, nor the help
    text box. wxAdvancedPropertyGrid (or something similarly named) is planned to have
    these features in some distant future.

    <h4>Derived from</h4>

    wxPropertyContainerMethods\n
    wxScrolledWindow\n
    wxPanel\n
    wxWindow\n
    wxEvtHandler\n
    wxObject\n

    <h4>Include files</h4>

    <wx/propertygrid/propertygrid.h>

    <h4>Window styles</h4>

    @link wndflags Additional Window Styles@endlink

    <h4>Event handling</h4>

    To process input from a propertygrid control, use these event handler macros to
    direct input to member functions that take a wxPropertyGridEvent argument.

    <table>
    <tr><td>EVT_PG_SELECTED (id, func)</td><td>Property is selected.</td></tr>
    <tr><td>EVT_PG_CHANGED (id, func)</td><td>Property value is modified.</td></tr>
    <tr><td>EVT_PG_HIGHLIGHTED (id, func)</td><td>Mouse moves over property. Event's property is NULL if hovered on area that is not a property.</td></tr>
    <tr><td>EVT_PG_RIGHT_CLICK (id, func)</td><td>Mouse right-clicked on a property.</td></tr>
    <tr><td>EVT_PG_DOUBLE_CLICK (id, func)</td><td>Mouse double-clicked on a property.</td></tr>
    <tr><td>EVT_PG_ITEM_COLLAPSED (id, func)</td><td>User collapses a property or category.</td></tr>
    <tr><td>EVT_PG_ITEM_EXPANDED (id, func)</td><td>User expands a property or category.</td></tr>
    <tr><td>EVT_BUTTON (id, func)</td><td>Button in a property editor was clicked. Only occurs if the property doesn't handle button clicks itself.</td></tr>
    <tr><td>EVT_TEXT (id, func)</td><td>wxTextCtrl based editor was updated (but property value was not yet modified)</td></tr>
    </table>

    \sa @link wxPropertyGridEvent wxPropertyGridEvent@endlink

    \remarks

    - Following functions do not automatically update the screen: Append. You
      probably need to explicitly call Refresh() <b>if</b> you called one of these
      functions outside parent window constructor.

    - Use Freeze() and Thaw() respectively to disable and enable drawing. This
      will also delay sorting etc. miscellaneous calculations to the last possible
      moment.

    - Most methods have two versions - one which accepts property id (faster) and
      another that accepts property name (which is a bit slower since it does a hashmap
      lookup).

    For code examples, see the main page.

*/
// BM_GRID
class WXDLLIMPEXP_PG wxPropertyGrid : public wxScrolledWindow, public wxPropertyContainerMethods
{
#ifndef SWIG
    friend class wxPropertyGridState;
    friend class wxPropertyContainerMethods;
    friend class wxPropertyGridManager;

    DECLARE_CLASS(wxPropertyGrid)
#endif

public:
	/** Two step constructor. Call Create when this constructor is called to build up the
	    wxPropertyGrid
	*/

#ifdef SWIG
    %pythonAppend wxPropertyGrid {
        self._setOORInfo(self)
        self.DoDefaultTypeMappings()
        self.edited_objects = {}
        self.DoDefaultValueTypeMappings()
        if not hasattr(self.__class__,'_vt2setter'):
            self.__class__._vt2setter = {}
    }
    %pythonAppend wxPropertyGrid() ""

    wxPropertyGrid( wxWindow *parent, wxWindowID id = wxID_ANY,
               	    const wxPoint& pos = wxDefaultPosition,
               	    const wxSize& size = wxDefaultSize,
               	    long style = wxPG_DEFAULT_STYLE,
               	    const wxChar* name = wxPyPropertyGridNameStr );
    %RenameCtor(PrePropertyGrid,  wxPropertyGrid());

#else

    wxPropertyGrid();

    /** The default constructor. The styles to be used are styles valid for
        the wxWindow and wxScrolledWindow.
        \sa @link wndflags Additional Window Styles@endlink
    */
    wxPropertyGrid( wxWindow *parent, wxWindowID id = wxID_ANY,
               	    const wxPoint& pos = wxDefaultPosition,
               	    const wxSize& size = wxDefaultSize,
               	    long style = wxPG_DEFAULT_STYLE,
               	    const wxChar* name = wxPropertyGridNameStr );

    /** Destructor */
    virtual ~wxPropertyGrid();
#endif

    /** Appends property to the list. wxPropertyGrid assumes ownership of the object.
        Becomes child of most recently added category.
        \remarks
        - wxPropertyGrid takes the ownership of the property pointer.
        - If appending a category with name identical to a category already in the
          wxPropertyGrid, then newly created category is deleted, and most recently
          added category (under which properties are appended) is set to the one with
          same name. This allows easier adding of items to same categories in multiple
          passes.
        - Does not automatically redraw the control, so you may need to call Refresh
          when calling this function after control has been shown for the first time.
    */
    wxPGId Append( wxPGProperty* property );

    inline wxPGId AppendCategory( const wxString& label, const wxString& name = wxPG_LABEL )
    {
        return Append( new wxPropertyCategoryClass(label,name) );
    }

#ifndef SWIG
#if wxPG_INCLUDE_BASICPROPS
    inline wxPGId Append( const wxString& label, const wxString& name = wxPG_LABEL, const wxString& value = wxEmptyString )
    {
        return Append( wxStringProperty(label,name,value) );
    }

    inline wxPGId Append( const wxString& label, const wxString& name = wxPG_LABEL, int value = 0 )
    {
        return Append( wxIntProperty(label,name,value) );
    }

    inline wxPGId Append( const wxString& label, const wxString& name = wxPG_LABEL, double value = 0.0 )
    {
        return Append( wxFloatProperty(label,name,value) );
    }

    inline wxPGId Append( const wxString& label, const wxString& name = wxPG_LABEL, bool value = false )
    {
        return Append( wxBoolProperty(label,name,value) );
    }
#endif
#endif

    inline wxPGId AppendIn( wxPGId id, wxPGProperty* property )
    {
        return Insert(id,-1,property);
    }

    inline wxPGId AppendIn( wxPGPropNameStr name, wxPGProperty* property )
    {
        return Insert(GetPropertyByNameI(name),-1,property);
    }

    inline wxPGId AppendIn( wxPGId id, const wxString& label, const wxString& propname, wxVariant& value )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(wxNullProperty)
        return m_pState->AppendIn( (wxPGPropertyWithChildren*)p, label, propname, value );
    }

    inline wxPGId AppendIn( wxPGPropNameStr name, const wxString& label, const wxString& propname, wxVariant& value )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxNullProperty)
        return m_pState->AppendIn( (wxPGPropertyWithChildren*)p, label, propname, value );
    }

    /** This static function enables or disables automatic use of wxGetTranslation for
        following strings: wxEnumProperty list labels, wxFlagsProperty sub-property
        labels.
        Default is false.
    */
    static void AutoGetTranslation( bool enable );

    /** Returns true if all property grid data changes have been committed. Usually
        only returns false if value in active editor has been invalidated by a
        wxValidator.
    */
    inline bool CanClose()
    {
        return DoEditorValidate();
    }

    /** Returns true if all property grid data changes have been committed. Usually
        only returns false if value in active editor has been invalidated by a
        wxValidator.
    */
    inline bool EditorValidate()
    {
        return DoEditorValidate();
    }

    /** Centers the splitter. If argument is true, automatic splitter centering is
        enabled (only applicapple if style wxPG_SPLITTER_AUTO_CENTER was defined).
    */
    void CenterSplitter( bool enable_auto_centering = false );

    /** Two step creation. Whenever the control is created without any parameters, use Create to actually
        create it. Don't access the control's public methods before this is called
        \sa @link wndflags Additional Window Styles@endlink
    */
    bool Create( wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxPG_DEFAULT_STYLE,
                 const wxChar* name = wxPropertyGridNameStr );

    /** Deletes all properties. Does not free memory allocated for arrays etc.
        This should *not* be called in wxPropertyGridManager.
    */
    void Clear();

    /** Resets modified status of a property and all sub-properties.
    */
    inline void ClearModifiedStatus( wxPGId id )
    {
        m_pState->ClearModifiedStatus(wxPGIdToPtr(id));
    }

    /** Resets modified status of all properties.
    */
    inline void ClearModifiedStatus()
    {
        m_pState->ClearModifiedStatus(m_pState->m_properties);
        m_pState->m_anyModified = false;
    }

    /** Resets value of a property to its default. */
    bool ClearPropertyValue( wxPGId id );

    /** Resets value of a property to its default. */
    inline bool ClearPropertyValue( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return ClearPropertyValue( wxPGIdGen(p) );
    }

    /** Deselect current selection, if any. Returns true if success
        (ie. validator did not intercept). */
    bool ClearSelection();

    /** Synonymous to Clear.
    */
    inline void ClearTargetPage()
    {
        Clear();
    }

    /** Collapses given category or property with children.
        Returns true if actually collapses.
    */
    inline bool Collapse( wxPGId id )
    {
        return _Collapse(wxPGIdToPtr(id));
    }

    /** Collapses given category or property with children.
        Returns true if actually collapses.
    */
    inline bool Collapse( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return _Collapse(p);
    }

    /** Collapses all items that can be collapsed.
        \retval
        Return false if failed (may fail if editor value cannot be validated).
    */
    inline bool CollapseAll() { return m_pState->ExpandAll(0); }

    /** Shows(arg = false) or hides(arg = true) all hideable properties. */
    bool Compact( bool compact );

    /** Disables property. */
    inline bool Disable( wxPGId id ) { return EnableProperty(id,false); }

    /** Disables property. */
    inline bool Disable( wxPGPropNameStr name ) { return EnableProperty(name,false); }

    /** Disables property. */
    inline bool DisableProperty( wxPGId id ) { return EnableProperty(id,false); }

    /** Disables property. */
    inline bool DisableProperty( wxPGPropNameStr name ) { return EnableProperty(name,false); }

    /** Enables or disables (shows/hides) categories according to parameter enable. */
    bool EnableCategories( bool enable );

    /** Enables or disables property, depending on whether enable is true or false. */
    bool EnableProperty( wxPGId id, bool enable = true );

    /** Enables or disables property, depending on whether enable is true or false. */
    inline bool EnableProperty( wxPGPropNameStr name, bool enable = true )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return EnableProperty( wxPGIdGen(p), enable );
    }

    /** Scrolls and/or expands items to ensure that the given item is visible.
        Returns true if something was actually done.
    */
    bool EnsureVisible( wxPGId id );

    /** Scrolls and/or expands items to ensure that the given item is visible.
        Returns true if something was actually done.
    */
    inline bool EnsureVisible( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return EnsureVisible( wxPGIdGen(p) );
    }

    /** Expands given category or property with children.
        Returns true if actually expands.
    */
    inline bool Expand( wxPGId id )
    {
        return _Expand(wxPGIdToPtr(id));
    }

    /** Expands given category or property with children.
        Returns true if actually expands.
    */
    inline bool Expand( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return _Expand(p);
    }

    /** Expands all items that can be expanded.
    */
    inline void ExpandAll() { m_pState->ExpandAll(1); }

#ifndef SWIG
    /** Returns a wxVariant list containing wxVariant versions of all
        property values. Order is not guaranteed, but generally it should
        match the visible order in the grid.
        \param flags
        Use wxPG_KEEP_STRUCTURE to retain category structure; each sub
        category will be its own wxList of wxVariant.
        \remarks
    */
    wxVariant GetPropertyValues( const wxString& listname = wxEmptyString,
        wxPGId baseparent = wxPGIdGen((wxPGProperty*)NULL), long flags = 0 ) const
    {
        return m_pState->GetPropertyValues(listname,baseparent,flags);
    }
#endif

    inline wxFont& GetCaptionFont() { return m_captionFont; }

    /** Returns current category caption background colour. */
    inline wxColour GetCaptionBackgroundColour() const { return m_colCapBack; }

    /** Returns current category caption text colour. */
    inline wxColour GetCaptionForegroundColour() const { return m_colCapFore; }

    /** Returns current cell background colour. */
    inline wxColour GetCellBackgroundColour() const { return m_colPropBack; }

    /** Returns current cell text colour when disabled. */
    inline wxColour GetCellDisabledTextColour() const { return m_colDisPropFore; }

    /** Returns current cell text colour. */
    inline wxColour GetCellTextColour() const { return m_colPropFore; }

    /** Returns number of children of the root property.
    */
    inline size_t GetChildrenCount()
    {
        return GetChildrenCount( wxPGIdGen(m_pState->m_properties) );
    }

    /** Returns number of children for the property.

        NB: Cannot be in container methods class due to name hiding.
    */
    inline size_t GetChildrenCount( wxPGId id ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(0)
        return p->GetChildCount();
    }

    /** Returns number of children for the property. */
    inline size_t GetChildrenCount( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(0)
        return p->GetChildCount();
    }

    /** Returns id of first item, whether it is a category or property. */
    inline wxPGId GetFirst() const
    {
        return m_pState->GetFirst();
    }

    /** Returns id of first visible item, whether it is a category or property.
        Note that visible item means category, property, or sub-property which
        user can see when control is scrolled properly. It does not only mean
        items that are actually painted on the screen.
    */
    inline wxPGId GetFirstVisible() const
    {
        wxPGProperty* p = NULL;
        if ( m_pState->m_properties->GetCount() )
        {
            p = m_pState->m_properties->Item(0);
            if ( (m_iFlags & wxPG_FL_HIDE_STATE) && p->m_flags & wxPG_PROP_HIDEABLE )
                p = GetNeighbourItem ( p, true, 1 );
        }
        return wxPGIdGen(p);
    }

    /** Returns height of highest characters of used font. */
    int GetFontHeight() const { return m_fontHeight; }

    /** Returns pointer to itself. Dummy function that enables same kind
        of code to use wxPropertyGrid and wxPropertyGridManager.
    */
    wxPropertyGrid* GetGrid() { return this; }

    /** Returns id of first category (from target page). */
    inline wxPGId GetFirstCategory() const
    {
        return m_pState->GetFirstCategory();
    }

    /** Returns id of first property that is not a category. */
    inline wxPGId GetFirstProperty()
    {
        return m_pState->GetFirstProperty();
    }

    /** Returns size of the custom paint image in front of property.
        If no argument is given, returns preferred size.
    */
    wxSize GetImageSize( wxPGId id = wxPGIdGen((wxPGProperty*)NULL) ) const;

    /** Returns property (or category) at given y coordinate (relative to control's
        top left).
    */
    wxPGId GetItemAtY( int y ) { return wxPGIdGen(DoGetItemAtY(y)); }

    /** Returns id of last item. Ignores categories and sub-properties.
    */
    inline wxPGId GetLastProperty()
    {
        if ( !m_pState->m_properties->GetCount() ) return wxPGIdGen((wxPGProperty*)NULL);
        wxPGProperty* p = GetLastItem(false, false);
        if ( p->GetParentingType() > 0 )
            return GetPrevProperty ( wxPGIdGen(p) );
        return wxPGIdGen(p);
    }

    /** Returns id of last child of given property.
        \remarks
        Returns even sub-properties.
    */
    inline wxPGId GetLastChild( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(wxNullProperty)

        wxPGPropertyWithChildren* pwc = (wxPGPropertyWithChildren*) p;
        if ( !pwc->GetParentingType() || !pwc->GetCount() ) return wxNullProperty;
        return wxPGIdGen(pwc->Last());
    }
    inline wxPGId GetLastChild( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxNullProperty)
        return GetLastChild( wxPGIdGen(p) );
    }

    /** Returns id of last visible item. Does <b>not</b> ignore categories sub-properties.
    */
    inline wxPGId GetLastVisible()
    {
        return wxPGIdGen( GetLastItem (true, true) );
    }

    /** Returns colour of lines between cells. */
    inline wxColour GetLineColour() const { return m_colLine; }

    /** Returns background colour of margin. */
    inline wxColour GetMarginColour() const { return m_colMargin; }

    /** Returns id of next property. This does <b>not</b> iterate to sub-properties
        or categories, unlike GetNextVisible.
    */
    inline wxPGId GetNextProperty( wxPGId id )
    {
        return m_pState->GetNextProperty(id);
    }

    /** Returns id of next category after a given property (which does not have to be category). */
    inline wxPGId GetNextCategory( wxPGId id ) const
    {
        return m_pState->GetNextCategory(id);
    }

    /** Returns id of next visible item.
        Note that visible item means category, property, or sub-property which
        user can see when control is scrolled properly. It does not only mean
        items that are actually painted on the screen.
    */
    inline wxPGId GetNextVisible( wxPGId property ) const
    {
        return wxPGIdGen ( GetNeighbourItem( wxPGIdToPtr(property),
            true, 1 ) );
    }

    /** Returns id of previous property. Unlike GetPrevVisible, this skips categories
        and sub-properties.
    */
    inline wxPGId GetPrevProperty( wxPGId id )
    {
        return m_pState->GetPrevProperty(id);
    }

    /** Returns id of previous item under the same parent. */
    inline wxPGId GetPrevSibling( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(wxNullProperty)
        return wxPropertyGridState::GetPrevSibling(id);
    }
    inline wxPGId GetPrevSibling( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxNullProperty)
        return wxPropertyGridState::GetPrevSibling(wxPGIdGen(p));
    }

    /** Returns id of previous visible property.
    */
    inline wxPGId GetPrevVisible( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(wxNullProperty)
        return wxPGIdGen( GetNeighbourItem( wxPGIdToPtr(id), true, -1 ) );
    }

    /** Returns id of property's nearest parent category. If no category
        found, returns invalid wxPGId.
    */
    inline wxPGId GetPropertyCategory( wxPGId id ) const
    {
        return wxPGIdGen( _GetPropertyCategory ( wxPGIdToPtr(id) ) );
    }
    inline wxPGId GetPropertyCategory( wxPGPropNameStr name ) const
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxNullProperty)
        return _GetPropertyCategory(p);
    }

    /** Returns cell background colour of a property. */
    wxColour GetPropertyBackgroundColour( wxPGId id ) const;
    inline wxColour GetPropertyBackgroundColour( wxPGPropNameStr name ) const
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxColour())
        return GetPropertyBackgroundColour(wxPGIdGen(p));
    }

    /** Returns cell background colour of a property. */
    inline wxColour GetPropertyColor( wxPGId id ) const
    {
        return GetPropertyBackgroundColour( id );
    }
    inline wxColour GetPropertyColor( wxPGPropNameStr name ) const
    {
        return GetPropertyBackgroundColour( name );
    }

    /** Returns cell background colour of a property. */
    wxColour GetPropertyTextColour( wxPGId id ) const;
    inline wxColour GetPropertyTextColour( wxPGPropNameStr name ) const
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxColour())
        return GetPropertyTextColour(wxPGIdGen(p));
    }

    /** Returns id of property with given label (case-sensitive). If there is no
        property with such label, returned property id is invalid ( i.e. it will return
        false with IsOk method). If there are multiple properties with identical name,
        most recent added is returned.
    */
    inline wxPGId GetPropertyByLabel( const wxString& name ) const
    {
        return m_pState->GetPropertyByLabel(name);
    }

    /** Returns "root property". It does not have name, etc. and it is not
        visible. It is only useful for accessing its children.
    */
    wxPGId GetRoot() const { return wxPGIdGen(m_pState->m_properties); }

    /** Returns height of a single grid row (in pixels). */
    int GetRowHeight() const { return m_lineHeight; }

    inline wxPGId GetSelectedProperty () const { return GetSelection(); }

    /** Returns currently selected property. */
    inline wxPGId GetSelection() const
    {
        return wxPGIdGen(m_selected);
    }

    /** Returns current selection background colour. */
    inline wxColour GetSelectionBackgroundColour() const { return m_colSelBack; }

    /** Returns current selection text colour. */
    inline wxColour GetSelectionForegroundColour() const { return m_colSelFore; }

    /** Returns current splitter x position. */
    inline int GetSplitterPosition() const { return m_splitterx; }

    /** Returns a binary copy of the current property state.
        NOTE: Too much work to implement, and uses would be few indeed.
    */
    //wxPropertyGridState* GetCopyOfState() const;

    /** Returns current vertical spacing. */
    inline int GetVerticalSpacing() const { return (int)m_vspacing; }

    /** Returns true if a property is selected. */
    inline bool HasSelection() const { return ((m_selected!=(wxPGProperty*)NULL)?true:false); }

    /** Hides all low priority properties. */
    inline void HideLowPriority() { Compact ( true ); }

    /** Inserts property to the list.

        \param priorthis
        New property is inserted just prior to this. Available only
        in the first variant. There are two versions of this function
        to allow this parameter to be either an id or name to
        a property.

        \param parent
        New property is inserted under this category. Available only
        in the second variant. There are two versions of this function
        to allow this parameter to be either an id or name to
        a property.

        \param index
        Index under category. Available only in the second variant.
        If index is < 0, property is appended in category.

        \param newproperty
        Pointer to the inserted property. wxPropertyGrid will take
        ownership of this object.

        \return
        Returns id for the property,

        \remarks

        - wxPropertyGrid takes the ownership of the property pointer.

        While Append may be faster way to add items, make note that when
        both data storages (categoric and
        non-categoric) are active, Insert becomes even more slow. This is
        especially true if current mode is non-categoric.

        Example of use:

        \code

            // append category
            wxPGId my_cat_id = propertygrid->Append( new wxPropertyCategoryClass (wxT("My Category")) );

            ...

            // insert into category - using second variant
            wxPGId my_item_id_1 = propertygrid->Insert( my_cat_id, 0, new wxStringProperty(wxT("My String 1")) );

            // insert before to first item - using first variant
            wxPGId my_item_id_2 = propertygrid->Insert ( my_item_id, new wxStringProperty(wxT("My String 2")) );

        \endcode

      */
    inline wxPGId Insert( wxPGId priorthis, wxPGProperty* newproperty )
    {
        wxPGId res = _Insert( wxPGIdToPtr( priorthis ), newproperty );
        DrawItems( newproperty, (wxPGProperty*) NULL );
        return res;
    }
    /** @link wxPropertyGrid::Insert Insert @endlink */
    inline wxPGId Insert( wxPGPropNameStr name, wxPGProperty* newproperty )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxNullProperty)
        wxPGId res = _Insert( (wxPGPropertyWithChildren*)p, newproperty );
        DrawItems( newproperty, (wxPGProperty*) NULL );
        return res;
    }

    /** @link wxPropertyGrid::Insert Insert @endlink */
    inline wxPGId Insert( wxPGId id, int index, wxPGProperty* newproperty )
    {
        wxPGId res = _Insert( (wxPGPropertyWithChildren*)wxPGIdToPtr (id), index, newproperty );
        DrawItems( newproperty, (wxPGProperty*) NULL );
        return res;
    }

    /** @link wxPropertyGrid::Insert Insert @endlink */
    inline wxPGId Insert( wxPGPropNameStr name, int index, wxPGProperty* newproperty )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(wxNullProperty)
        wxPGId res = _Insert( (wxPGPropertyWithChildren*)p, index, newproperty );
        DrawItems( newproperty, (wxPGProperty*) NULL );
        return res;
    }

    inline wxPGId InsertCategory( wxPGId id, int index, const wxString& label, const wxString& name = wxPG_LABEL )
    {
        return Insert( id, index, new wxPropertyCategoryClass(label,name) );
    }

#if wxPG_INCLUDE_BASICPROPS
    /** @link wxPropertyGrid::Insert Insert @endlink */
    inline wxPGId Insert( wxPGId id, int index, const wxString& label, const wxString& name, const wxString& value = wxEmptyString )
    {
        return Insert( id, index, wxStringProperty(label,name,value) );
    }

    /** @link wxPropertyGrid::Insert Insert @endlink */
    inline wxPGId Insert( wxPGId id, int index, const wxString& label, const wxString& name, int value )
    {
        return Insert( id, index, wxIntProperty(label,name,value) );
    }

    /** @link wxPropertyGrid::Insert Insert @endlink */
    inline wxPGId Insert( wxPGId id, int index, const wxString& label, const wxString& name, double value )
    {
        return Insert( id, index, wxFloatProperty(label,name,value) );
    }

    /** @link wxPropertyGrid::Insert Insert @endlink */
    inline wxPGId Insert( wxPGId id, int index, const wxString& label, const wxString& name, bool value )
    {
        return Insert( id, index, wxBoolProperty(label,name,value) );
    }
#endif

    /** Returns true if any property has been modified by the user. */
    inline bool IsAnyModified() const { return (m_pState->m_anyModified>0); }

    /** Returns true if updating is frozen (ie. Freeze() called but not yet Thaw() ). */
    inline bool IsFrozen() const { return (m_frozen>0)?true:false; }

    /** Returns true if given property is selected. */
    inline bool IsPropertySelected( wxPGId id ) const
    {
        wxPG_PROP_ID_CALL_PROLOG_RETVAL(false)
        return ( m_selected == p ) ? true : false;
    }

    /** Returns true if given property is selected. */
    inline bool IsPropertySelected( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return ( m_selected == p ) ? true : false;
    }

    /** Disables (limit = true) or enables (limit = false) wxTextCtrl editor of a property,
        if it is not the sole mean to edit the value.
    */
    void LimitPropertyEditing( wxPGId id, bool limit = true );

    /** Disables (limit = true) or enables (limit = false) wxTextCtrl editor of a property,
        if it is not the sole mean to edit the value.
    */
    inline void LimitPropertyEditing( wxPGPropNameStr name, bool limit = true )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        LimitPropertyEditing(wxPGIdGen(p),limit);
    }

    /** Moves splitter as left as possible, while still allowing all
        labels to be shown in full.
        \param subProps
        If false, will still allow sub-properties (ie. properties which
        parent is not root or category) to be cropped.
    */
    void SetSplitterLeft( bool subProps = false );

    /** Registers a new value type. Takes ownership of the object.
        \retval
        Pointer to the value type that should be used. If on with
        the same name already existed, then the first one will be used,
        and its pointer is returned instead.
    */
    static wxPGValueType* RegisterValueType( wxPGValueType* valueclass, bool noDefCheck = false,
                                             const wxString& className = wxEmptyString );

#ifndef SWIG
    /** Registers a new editor class.
        \retval
        Pointer to the editor class instance that should be used.
    */
    static wxPGEditor* RegisterEditorClass( wxPGEditor* editor, const wxString& name,
                                            bool noDefCheck = false );
#endif

    /** Resets all colours to the original system values.
    */
    void ResetColours();

    /** Changes keyboard shortcut to push the editor button.
        \remarks
        You can set default with keycode 0. Good value for the platform is guessed,
        but don't expect it to be very accurate.
    */
    void SetButtonShortcut( int keycode, bool ctrlDown = false, bool altDown = false );

    /** Sets text colour of a category caption (but not it's children).
    */
    void SetCaptionTextColour( wxPGId id, const wxColour& col );
    inline void SetCaptionTextColour( wxPGPropNameStr name, const wxColour& col )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetCaptionTextColour( wxPGIdGen(p), col );
    }

    /** Sets the current category - Append will add non-categories under this one.
    */
    inline void SetCurrentCategory( wxPGId id )
    {
        wxPG_PROP_ID_CALL_PROLOG()
        wxPropertyCategoryClass* pc = (wxPropertyCategoryClass*)p;
#ifdef __WXDEBUG__
        if ( pc ) wxASSERT( pc->GetParentingType() > 0 );
#endif
        m_pState->m_currentCategory = pc;
    }

    /** Sets the current category - Append will add non-categories under this one.
    */
    inline void SetCurrentCategory( wxPGPropNameStr name = wxEmptyString )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetCurrentCategory(wxPGIdGen(p));
    }

    /** Sets property attribute for all applicapple properties.
        Be sure to use this method after all properties have been
        added to the grid.
    */
    void SetPropertyAttributeAll( int attrid, wxVariant value );

    /** Sets background colour of property and all its children, recursively. Colours of
        captions are not affected. Background brush cache is optimized for often
        set colours to be set last.
        \remarks
        * Children which already have custom background colour are not affected.
    */
    void SetPropertyBackgroundColour( wxPGId id, const wxColour& col );
    inline void SetPropertyBackgroundColour( wxPGPropNameStr name, const wxColour& col )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyBackgroundColour( wxPGIdGen(p), col );
    }

    /** Sets background colour of property and all its children. Colours of
        captions are not affected. Background brush cache is optimized for often
        set colours to be set last.

        NOTE: This function is deprecated. Use SetPropertyBackgroundColour.
    */
    inline void SetPropertyColor( wxPGId id, const wxColour& col )
    {
        SetPropertyBackgroundColour( id, col );
    }
    inline void SetPropertyColor( wxPGPropNameStr name, const wxColour& col )
    {
        SetPropertyBackgroundColour( name, col );
    }

    /** Sets text colour of property and all its children.
        \remarks
        * Children which already have custom text colour are not affected.
    */
    void SetPropertyTextColour( wxPGId id, const wxColour& col );
    inline void SetPropertyTextColour( wxPGPropNameStr name, const wxColour& col )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyTextColour( wxPGIdGen(p), col );
    }

    /** Sets background and text colour of property and all its children to the default. */
    void SetPropertyColorToDefault( wxPGId id );
    inline void SetPropertyColorToDefault( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyColorToDefault( wxPGIdGen(p) );
    }

    /** Sets category caption background colour. */
    void SetCaptionBackgroundColour(const wxColour& col);

    /** Sets category caption text colour. */
    void SetCaptionForegroundColour(const wxColour& col);

    /** Sets default cell background colour - applies to property cells.
        Note that appearance of editor widgets may not be affected.
    */
    void SetCellBackgroundColour(const wxColour& col);

    /** Sets cell text colour for disabled properties.
    */
    void SetCellDisabledTextColour(const wxColour& col);

    /** Sets default cell text colour - applies to property name and value text.
        Note that appearance of editor widgets may not be affected.
    */
    void SetCellTextColour(const wxColour& col);

    /** Sets colour of lines between cells. */
    void SetLineColour(const wxColour& col);

    /** Sets background colour of margin. */
    void SetMarginColour(const wxColour& col);

    /** Sets selection background colour - applies to selected property name background. */
    void SetSelectionBackground(const wxColour& col);

    /** Sets selection foreground colour - applies to selected property name text. */
    void SetSelectionForeground(const wxColour& col);

    /** Sets x coordinate of the splitter. */
    inline void SetSplitterPosition( int newxpos, bool refresh = true )
    {
        DoSetSplitterPosition(newxpos,refresh);
        m_iFlags |= wxPG_FL_SPLITTER_PRE_SET;
    }

    /** Selects a property. Editor widget is automatically created, but
        not focused unless focus is true. This will generate wxEVT_PG_SELECT event.
        \param id
        Id to property to select.
        \retval
        True if selection finished succesfully. Usually only fails if current
        value in editor is not valid.
        \sa wxPropertyGrid::Unselect
    */
    inline bool SelectProperty( wxPGId id, bool focus = false )
    {
        return DoSelectProperty(wxPGIdToPtr(id),focus?wxPG_SEL_FOCUS:0);
    }
    inline bool SelectProperty( wxPGPropNameStr name, bool focus = false )
    {
        wxPG_PROP_NAME_CALL_PROLOG_RETVAL(false)
        return DoSelectProperty(p,focus?wxPG_SEL_FOCUS:0);
    }

    /** Mostly useful for page switching.
    */
#ifndef SWIG
    void SwitchState( wxPropertyGridState* pNewState );
#endif

    /** Sets label of a property.
        \remarks
        This is the only way to set property's name. There is not
        wxPGProperty::SetLabel() method.
    */
    inline void SetPropertyLabel( wxPGId id, const wxString& newproplabel )
    {
        wxPG_PROP_ID_CALL_PROLOG()
        _SetPropertyLabel( p, newproplabel );
    }
    /** Sets label of a property.
        \remarks
        This is the only way to set property's label. There is no
        wxPGProperty::SetLabel() method.
    */
    inline void SetPropertyLabel( wxPGPropNameStr name, const wxString& newproplabel )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        _SetPropertyLabel( p, newproplabel );
    }

    /** Sets name of a property.
        \param id
        Id of a property.
        \param newname
        New name.
        \remarks
        This is the only way to set property's name. There is not
        wxPGProperty::SetName() method.
    */
    inline void SetPropertyName( wxPGId id, const wxString& newname )
    {
        DoSetPropertyName( wxPGIdToPtr(id), newname );
    }
    /** Sets name of a property.
        \param name
        Label of a property.
        \param newname
        New name.
        \remarks
        This is the only way to set property's name. There is not
        wxPGProperty::SetName() method.
    */
    inline void SetPropertyName( wxPGPropNameStr name, const wxString& newname )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        DoSetPropertyName( p, newname );
    }

    /** Sets value (long integer) of a property.
    */
    inline void SetPropertyValueLong( wxPGId id, long value )
    {
        SetPropertyValue( id, wxPG_VALUETYPE(long), wxPGVariantFromLong(value) );
    }

#ifndef __WXPYTHON__
    /** Sets value (integer) of a property.
    */
    inline void SetPropertyValue( wxPGId id, int value )
    {
        SetPropertyValue( id, wxPG_VALUETYPE(long), wxPGVariantFromLong((long)value) );
    }
#endif
    /** Sets value (floating point) of a property.
    */
    inline void SetPropertyValueDouble( wxPGId id, double value )
    {
        SetPropertyValue( id, wxPG_VALUETYPE(double), wxPGVariantFromDouble(value) );
    }
    /** Sets value (bool) of a property.
    */
    inline void SetPropertyValueBool( wxPGId id, bool value )
    {
        SetPropertyValue( id, wxPG_VALUETYPE(bool), wxPGVariantFromLong(value?(long)1:(long)0) );
    }

    /** Sets value (wxString) of a property.
        \remarks
        This method uses wxPGProperty::SetValueFromString, which all properties
        should implement. This means that there should not be a type error,
        and instead the string is converted to property's actual value type.
    */
    void SetPropertyValueString( wxPGId id, const wxString& value );

#ifndef __WXPYTHON__
    inline void SetPropertyValue( wxPGId id, const wxChar* value )
    {
        SetPropertyValue(id,wxString(value));
    }
#endif

    /** Sets value (wxArrayString) of a property.
    */
    inline void SetPropertyValueArrstr2( wxPGId id, const wxArrayString& value )
    {
        SetPropertyValue( id, wxPG_VALUETYPE(wxArrayString), wxPGVariantFromArrayString(value) );
    }
    /** Sets value (wxObject*) of a property.
    */
    void SetPropertyValueWxObjectPtr( wxPGId id, wxObject* value );
#ifndef __WXPYTHON__
    /** Sets value (void*) of a property. */
    inline void SetPropertyValue( wxPGId id, void* value )
    {
        SetPropertyValue( id, wxPG_VALUETYPE(void), value );
    }
    inline void SetPropertyValue ( wxPGId id, wxObject& value )
    {
        SetPropertyValue(id,&value);
    }

    /** Sets value (wxVariant&) of a property. */
    void SetPropertyValue( wxPGId id, wxVariant& value );
#endif

    /** Sets value (wxPoint&) of a property.
    */
    inline void SetPropertyValuePoint( wxPGId id, const wxPoint& value )
    {
        SetPropertyValue( id, wxT("wxPoint"), wxPGVariantCreator(value) );
    }
    /** Sets value (wxSize&) of a property.
    */
    inline void SetPropertyValueSize( wxPGId id, const wxSize& value )
    {
        SetPropertyValue( id, wxT("wxSize"), wxPGVariantCreator(value) );
    }
    /** Sets value (wxLongLong&) of a property.
    */
    inline void SetPropertyValueLongLong( wxPGId id, const wxLongLong& value )
    {
        SetPropertyValue( id, wxT("wxLongLong"), wxPGVariantCreator(value) );
    }
    /** Sets value (wxULongLong&) of a property.
    */
    inline void SetPropertyValueULongLong( wxPGId id, const wxULongLong& value )
    {
        SetPropertyValue( id, wxT("wxULongLong"), wxPGVariantCreator(value) );
    }
    /** Sets value (wxArrayInt&) of a property.
    */
    inline void SetPropertyValueArrint2( wxPGId id, const wxArrayInt& value )
    {
        SetPropertyValue( id, wxT("wxArrayInt"), wxPGVariantCreator(value) );
    }
    /** Sets value (wxDateTime&) of a property.
    */
#if wxUSE_DATETIME
    inline void SetPropertyValueDatetime( wxPGId id, const wxDateTime& value )
    {
        SetPropertyValue( id, wxT("datetime"), value );
    }
#endif
#ifdef __WXPYTHON__
    inline void SetPropertyValuePyObject( wxPGId id, PyObject* value )
    {
        SetPropertyValue( id, wxT("PyObject"), wxPGVariantCreator(value) );
    }
#endif

    /** Sets value (long integer) of a property.
    */
    inline void SetPropertyValueLong( wxPGPropNameStr name, long value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValue( wxPGIdGen(p), wxPG_VALUETYPE(long), wxPGVariantFromLong(value) );
    }
#ifndef __WXPYTHON__
    /** Sets value (integer) of a property. */
    inline void SetPropertyValue( wxPGPropNameStr name, int value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValue( wxPGIdGen(p), wxPG_VALUETYPE(long), wxPGVariantFromLong(value) );
    }
#endif
    /** Sets value (floating point) of a property.
    */
    inline void SetPropertyValueDouble( wxPGPropNameStr name, double value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValue( wxPGIdGen(p), wxPG_VALUETYPE(double), wxPGVariantFromDouble(value) );
    }
    /** Sets value (bool) of a property.
    */
    inline void SetPropertyValueBool( wxPGPropNameStr name, bool value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValue( wxPGIdGen(p), wxPG_VALUETYPE(bool), wxPGVariantFromLong(value?(long)1:(long)0) );
    }
    /** Sets value (wxString) of a property. For properties which value type is
        not string, calls wxPGProperty::SetValueFromString to translate the value.
    */
    inline void SetPropertyValueString( wxPGPropNameStr name, const wxString& value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValueString( wxPGIdGen(p), value );
    }

#ifndef __WXPYTHON__
    /** Sets value (wxString) of a property. For properties which value type is
        not string, calls wxPGProperty::SetValueFromString to translate the value.
    */
    inline void SetPropertyValue( wxPGPropNameStr name, const wxChar* value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValue( wxPGIdGen(p), wxString(value) );
    }

    /** Sets value (void*) of a property. */
    inline void SetPropertyValue( wxPGPropNameStr name, void* value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValue( wxPGIdGen(p), wxPG_VALUETYPE(void), value );
    }
#endif

    /** Sets value (wxObject*) of a property.
    */
    inline void SetPropertyValueWxObjectPtr( wxPGPropNameStr name, wxObject* value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValueWxObjectPtr( wxPGIdGen(p), value );
    }

#ifndef __WXPYTHON__
    inline void SetPropertyValue( wxPGPropNameStr name, wxObject& value )
    {
        SetPropertyValue(name,&value);
    }

    /** Sets value (wxVariant&) of a property. */
    void SetPropertyValue( wxPGPropNameStr name, wxVariant& value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValue( wxPGIdGen(p), value );
    }

    /** Sets value (wxArrayString) of a property. */
    inline void SetPropertyValueArrstr2( wxPGPropNameStr name, const wxArrayString& value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValue( wxPGIdGen(p), wxPG_VALUETYPE(wxArrayString), wxPGVariantFromArrayString(value) );
    }
    /** Sets value (wxArrayInt&) of a property. */
    inline void SetPropertyValueArrint2( wxPGPropNameStr name, const wxArrayInt& value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValueArrint2( wxPGIdGen(p), value );
    }
#endif
    /** Sets value (wxDateTime&) of a property.
    */
#if wxUSE_DATETIME
    inline void SetPropertyValueDatetime( wxPGPropNameStr name, const wxDateTime& value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValueDatetime( wxPGIdGen(p), value );
    }
#endif
    /** Sets value (wxPoint&) of a property.
    */
    inline void SetPropertyValuePoint( wxPGPropNameStr name, const wxPoint& value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValuePoint( wxPGIdGen(p), value );
    }
    /** Sets value (wxSize&) of a property.
    */
    inline void SetPropertyValueSize( wxPGPropNameStr name, const wxSize& value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValueSize( wxPGIdGen(p), value );
    }
    /** Sets value (wxLongLong&) of a property.
    */
    inline void SetPropertyValueLongLong( wxPGPropNameStr name, const wxLongLong& value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValueLongLong( wxPGIdGen(p), value );
    }
    /** Sets value (wxULongLong&) of a property.
    */
    inline void SetPropertyValueULongLong( wxPGPropNameStr name, const wxULongLong& value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValueULongLong( wxPGIdGen(p), value );
    }
#ifdef __WXPYTHON__
    inline void SetPropertyValuePyObject( wxPGPropNameStr name, PyObject* value )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyValuePyObject( wxPGIdGen(p), value );
    }
#endif

    /** Sets property's value to unspecified. If it has children (it may be category),
        then the same thing is done to them.
    */
    void SetPropertyUnspecified( wxPGId id );
    inline void SetPropertyUnspecified ( wxPGPropNameStr name )
    {
        wxPG_PROP_NAME_CALL_PROLOG()
        SetPropertyUnspecified( wxPGIdGen(p) );
    }

#ifndef SWIG
    /** Sets various property values from a list of wxVariants. If property with
        name is missing from the grid, new property is created under given default
        category (or root if omitted).
    */
    void SetPropertyValues( const wxVariantList& list, wxPGId default_category )
    {
        m_pState->SetPropertyValues(list,default_category);
    }

    inline void SetPropertyValues( const wxVariant& list, wxPGId default_category )
    {
        SetPropertyValues(list.GetList(),default_category);
    }
    inline void SetPropertyValues( const wxVariantList& list, const wxString& default_category = wxEmptyString )
    {
        SetPropertyValues(list,GetPropertyByNameI(default_category));
    }
    inline void SetPropertyValues( const wxVariant& list, const wxString& default_category = wxEmptyString )
    {
        SetPropertyValues(list.GetList(),GetPropertyByNameI(default_category));
    }
#endif

    /** Sets vertical spacing. Can be 1, 2, or 3 - a value relative to font
        height. Value of 2 should be default on most platforms.
        \remarks
        On wxMSW, wxComboBox, when used as property editor widget, will spill
        out with anything less than 3.
    */
    inline void SetVerticalSpacing( int vspacing )
    {
        m_vspacing = (unsigned char)vspacing;
        CalculateFontAndBitmapStuff( vspacing );
        if ( !m_pState->m_itemsAdded ) Refresh();
    }

    /** Shows all low priority properties. */
    inline void ShowLowPriority() { Compact ( false ); }

    /** Shows an brief error message that is related to a property. */
    inline void ShowPropertyError( wxPGId id, const wxString& msg )
    {
        wxPG_PROP_ID_CALL_PROLOG()
        p->ShowError(msg);
    }
    inline void ShowPropertyError( wxPGPropNameStr name, const wxString& msg )
    {
        ShowPropertyError (GetPropertyByNameI(name), msg);
    }

    /** Sorts all items at all levels (except sub-properties). */
    void Sort();

    /** Sorts children of a category.
    */
    void Sort( wxPGId id );

    /** Sorts children of a category.
    */
    inline void Sort( wxPGPropNameStr name )
    {
        Sort( GetPropertyByNameI(name) );
    }

    /** Overridden function.
        \sa @link wndflags Additional Window Styles@endlink
    */
    virtual void SetWindowStyleFlag( long style );

    /** All properties added/inserted will have given priority by default.
        \param
        priority can be wxPG_HIGH (default) or wxPG_LOW.
    */
    inline void SetDefaultPriority( int priority )
    {
        if ( priority == wxPG_LOW )
            m_iFlags |= wxPG_FL_ADDING_HIDEABLES;
        else
            m_iFlags &= ~(wxPG_FL_ADDING_HIDEABLES);
    }

    /** Same as SetDefaultPriority(wxPG_HIGH). */
    inline void ResetDefaultPriority()
    {
        SetDefaultPriority(wxPG_HIGH);
    }

    /** Property editor widget helper methods. */
    //@{
    /** Call when editor widget's contents is modified. For example, this is called
        when changes text in wxTextCtrl (used in wxStringProperty and wxIntProperty).
        \remarks
        This should only be called by properties.
        \sa @link wxPGProperty::OnEvent @endlink
    */
    inline void EditorsValueWasModified() { m_iFlags |= wxPG_FL_VALUE_MODIFIED; }
    /** Reverse of EditorsValueWasModified(). */
    inline void EditorsValueWasNotModified()
    {
        m_iFlags &= ~(wxPG_FL_VALUE_MODIFIED);
    }

    /** Returns true if editor's value was marked modified. */
    inline bool IsEditorsValueModified() const { return  ( m_iFlags & wxPG_FL_VALUE_MODIFIED ) ? true : false; }

    /** Shortcut for creating dialog-caller button. Used, for example, by wxFontProperty.
        \remarks
        This should only be called by properties.
    */
    wxWindow* GenerateEditorButton( const wxPoint& pos, const wxSize& sz );

    /** Fixes position of wxTextCtrl-like control (wxSpinCtrl usually
        fits as one). Call after control has been created (but before
        shown).
    */
    void FixPosForTextCtrl( wxWindow* ctrl );

    /** Shortcut for creating text editor widget.
        \param pos
        Same as pos given for CreateEditor.
        \param sz
        Same as sz given for CreateEditor.
        \param value
        Initial text for wxTextCtrl.
        \param secondary
        If right-side control, such as button, also created, then create it first
        and pass it as this parameter.
        \param extraStyle
        Extra style flags to pass for wxTextCtrl.
        \remarks
        Note that this should generally be called only by new classes derived
        from wxPGProperty.
    */
    wxWindow* GenerateEditorTextCtrl( const wxPoint& pos,
                                      const wxSize& sz,
                                      const wxString& value,
                                      wxWindow* secondary,
                                      int extraStyle = 0,
                                      int maxLen = 0 );

    /* Generates both textctrl and button.
    */
    wxWindow* GenerateEditorTextCtrlAndButton( const wxPoint& pos,
        const wxSize& sz, wxWindow** psecondary, int limited_editing,
        wxPGProperty* property );

    /** Generates position for a widget editor dialog box.
        \param p
        Property for which dialog is positioned.
        \param sz
        Known or over-approximated size of the dialog.
        \retval
        Position for dialog.
    */
    wxPoint GetGoodEditorDialogPosition( wxPGProperty* p,
                                         const wxSize& sz );

    // Converts escape sequences in src_str to newlines,
    // tabs, etc. and copies result to dst_str.
    static wxString& ExpandEscapeSequences( wxString& dst_str, wxString& src_str );

    // Converts newlines, tabs, etc. in src_str to escape
    // sequences, and copies result to dst_str.
    static wxString& CreateEscapeSequences( wxString& dst_str, wxString& src_str );

    /** Returns rectangle that fully contains properties between and including p1 and p2.
    */
    wxRect GetPropertyRect( const wxPGProperty* p1, const wxPGProperty* p2 ) const;

    /** Returns pointer to current active primary editor control (NULL if none).

        If editor uses clipper window, pointer is returned to the actual editor, not the clipper.
    */
    wxWindow* GetEditorControl() const;

    inline wxWindow* GetPrimaryEditor() const
    {
        return m_wndPrimary;
    }

    /** Returns pointer to current active secondary editor control (NULL if none).
    */
    inline wxWindow* GetEditorControlSecondary() const
    {
        return m_wndSecondary;
    }

    inline int IsNextEventIgnored() const
    {
        return m_ignoredEvents;
    }

    inline void IgnoreNextEvent()
    {
        m_ignoredEvents++;
    }

    inline void IgnoredEventPasses()
    {
        wxASSERT( m_ignoredEvents > 0 );
        m_ignoredEvents--;
    }

#ifdef __WXPYTHON__
    // Dummy method to put wxRect type info into the wrapper
    wxRect DummywxRectTypeInit() const { return wxRect(1,2,3,4); }
#endif

#ifndef SWIG

    /** Generates contents for string dst based on the convetents of wxArrayString
        src. Format will be <preDelim>str1<postDelim> <preDelim>str2<postDelim>
        and so on. Set flags to 1 inorder to convert backslashes to double-back-
        slashes and "<preDelims>"'s to "\<preDelims>".
    */
    static void ArrayStringToString( wxString& dst, const wxArrayString& src,
                                     wxChar preDelim, wxChar postDelim,
                                     int flags );

    /** Pass this function to Connect calls in propertyclass::CreateEditor.
    */
    void OnCustomEditorEvent( wxCommandEvent &event );
    /** Puts items into sl. Automatic wxGetTranslation is used if enabled. */
    void SLAlloc ( unsigned int itemcount, const wxChar** items );
    /** Returns sl. */
    inline wxArrayString& SLGet() { return m_sl; }
    //@}

    inline long GetInternalFlags() const { return m_iFlags; }
    inline void ClearInternalFlag( long flag ) { m_iFlags &= ~(flag); }
    inline unsigned int GetBottomY() const { return m_bottomy; }
    inline void SetBottomY( unsigned int y ) { m_bottomy = y; }
    inline void IncFrozen() { m_frozen++; }
    inline void DecFrozen() { m_frozen--; }

    /** Call after a property modified internally.
        selFlags are the same as with DoSelectProperty.
        NB: Avoid using this method, if possible.
    */
    void PropertyWasModified( wxPGProperty* p, int selFlags = 0 );

    void OnComboItemPaint( wxPGCustomComboControl* pCb,int item,wxDC& dc,
                           wxRect& rect,int flags );

    // Used by simple check box for keyboard navigation
    void SendNavigationKeyEvent( int dir );

#if 0
    /* Creates choices for labels and values, or returns existing choices which
       point to the same memory.
    */
    static wxPGChoices* CreateChoicesArray(const wxChar** labels,
                                           const long* values,
                                           int itemcount);

    /* Creates choices for labels and values, or returns existing choices which
       point to the same memory (*only* if acceptLabelsAsId=true).
    */
    static wxPGChoices* CreateChoicesArray(const wxArrayString& labels,
                                           const wxArrayInt& values = wxArrayInt(),
                                           bool acceptLabelsAsId = false);

    /* Creates choices for labels and values in wxPGChoices, or returns existing
       choices which is identical.
    */
    static wxPGChoices* CreateChoicesArray(wxPGChoices& choices);

#ifdef __WXDEBUG__
    // Displays what dynamic arrays are allocated
    static void DumpAllocatedChoiceSets();
#endif

#endif // #if 0

    /** Standardized double-to-string conversion.
    */
    static void DoubleToString( wxString& target,
                                double value,
                                int precision,
                                bool removeZeroes,
                                wxString* precTemplate );


protected:

    /** wxPropertyGridState used by the grid is created here. If grid is used
        in wxPropertyGridManager, there is no point overriding this - instead,
        set custom wxPropertyGridPage classes.
    */
    virtual wxPropertyGridState* CreateState() const;

#ifndef DOXYGEN
public:

    // Control font changer helper.
    void SetCurControlBoldFont();

    //
    // Public methods for semi-public use
    // (not protected for optimization)
    //
    bool DoSelectProperty( wxPGProperty* p, unsigned int flags = 0 );

    // Usually called internally after items added/deleted.
    void CalculateYs( wxPGPropertyWithChildren* startparent, int startindex );

    // Overridden functions.
    virtual bool Destroy();
    virtual wxSize DoGetBestSize() const;
    virtual void Refresh( bool eraseBackground = true,
                          const wxRect *rect = (const wxRect *) NULL );
    virtual bool SetFont( const wxFont& font );
#if wxPG_SUPPORT_TOOLTIPS
    void SetToolTip( const wxString& tipString );
#endif
    virtual void Freeze();
    virtual void SetExtraStyle( long exStyle );
    virtual void Thaw();


protected:

    /** 1 if calling property event handler. */
    unsigned char       m_processingEvent;

#ifndef wxPG_ICON_WIDTH
	wxBitmap            *m_expandbmp, *m_collbmp;
#endif

    wxCursor            *m_cursorSizeWE;

    /** wxWindow pointers to editor control(s). */
    wxWindow            *m_wndPrimary;
    wxWindow            *m_wndSecondary;

#if wxPG_DOUBLE_BUFFER
    wxBitmap            *m_doubleBuffer;
#endif

    wxArrayPtrVoid      *m_windowsToDelete;

    /** Local time ms when control was created. */
    wxLongLong          m_timeCreated;

    /** Indicates bottom of drawn and clickable area on the control. Updated
        by CalculateYs. */
    unsigned int        m_bottomy;

	/** Extra Y spacing between the items. */
	int                 m_spacingy;

    /** Control client area width; updated on resize. */
    int                 m_width;

    /** Control client area height; updated on resize. */
    int                 m_height;

    /** Non-client width (auto-centering helper). */
    int                 m_fWidth;

    /** List of currently visible properties. */
    wxPGArrayProperty   m_arrVisible;

    /** Previously recorded scroll start position. */
    int                 m_prevVY;

    /** Necessary so we know when to re-calculate visibles on resize. */
    int                 m_calcVisHeight;

	/** The gutter spacing in front and back of the image. This determines the amount of spacing in front
	    of each item */
	int                 m_gutterWidth;

    /** Includes separator line. */
    int                 m_lineHeight;

    /** Gutter*2 + image width. */
    int                 m_marginWidth;

    int                 m_buttonSpacingY; // y spacing for expand/collapse button.

    /** Extra margin for expanded sub-group items. */
    int                 m_subgroup_extramargin;

	/** The image width of the [+] icon. This is also calculated in the gutter */
	int                 m_iconWidth;

#ifndef wxPG_ICON_WIDTH

	/** The image height of the [+] icon. This is calculated as minimal size and to align */
	int                 m_iconHeight;
#endif

    /** Current cursor id. */
    int                 m_curcursor;

	/** This captionFont is made equal to the font of the wxScrolledWindow. As extra the bold face
	    is set on it when this is wanted by the user (see flags) */
	wxFont              m_captionFont;

#if !wxPG_HEAVY_GFX
    int                 m_splitterprevdrawnx;

    /** Pen used to draw splitter column when it is being dragged. */
    wxPen               m_splitterpen;

#endif

	int                 m_fontHeight;  // Height of the font.

    int                 m_pushButKeyCode;  // Base keycode for triggering push button.

    //
    // Temporary values
    //

    /** m_splitterx when drag began. */
    int                 m_startingSplitterX;

    /** Bits are used to indicate which colours are customized. */
    unsigned short      m_coloursCustomized;

    /** 0 = not dragging, 1 = drag just started, 2 = drag in progress */
    unsigned char       m_dragStatus;

    /** x - m_splitterx. */
    signed char         m_dragOffset;

    /** 0 = margin, 1 = label, 2 = value. */
    unsigned char       m_mouseSide;

    /** True when editor control is focused. */
    unsigned char       m_editorFocused;

    /** 1 if m_latsCaption is also the bottommost caption. */
    //unsigned char       m_lastCaptionBottomnest;

    /** Set to 1 when graphics frozen. */
    unsigned char       m_frozen;

    unsigned char       m_vspacing;

    unsigned char       m_pushButKeyCodeNeedsAlt;  // Does triggering push button need Alt down?

    unsigned char       m_pushButKeyCodeNeedsCtrl;  // Does triggering push button need Ctrl down?

    unsigned char       m_keyComboConsumed;  // Used to track when Alt/Ctrl+Key was consumed.

    unsigned char       m_ignoredEvents;  // Number of EVT_TEXT-style events to ignore.

    /** Internal flags - see wxPG_FL_XXX constants. */
    wxUint32            m_iFlags;

    /** When drawing next time, clear this many item slots at the end. */
    int                 m_clearThisMany;

    /** Pointer to selected property. Note that this is duplicated in
        m_state for better transiency between pages so that the selected
        item can be retained.
    */
    wxPGProperty*       m_selected;

    wxPGProperty*       m_propHover;  // pointer to property that has mouse on itself

    wxWindow*           m_eventObject;  // EventObject for wxPropertyGridEvents

    wxWindow*           m_curFocused;  // What (global) window is currently focused
                                       // (needed to resolve event handling mess).

    wxEvtHandler*       m_tlwHandler;  // wxPGTLWHandler

    wxWindow*           m_tlp;  // Top level parent

    int                 m_splitterx; // x position for the vertical line dividing name and value

    float               m_fSplitterX; // accurate splitter position

    int                 m_ctrlXAdjust; // x relative to splitter (needed for resize).

    wxColour            m_colLine;     // lines between cells
    wxColour            m_colPropFore; // property labels and values are written in this colour
    wxColour            m_colDisPropFore;  // or with this colour when disabled
    wxColour            m_colPropBack; // background for m_colPropFore
    wxColour            m_colCapFore;  // text color for captions
    wxColour            m_colCapBack;  // background color for captions
    wxColour            m_colSelFore;  // foreground for selected property
    wxColour            m_colSelBack;  // background for selected property (actually use background color when control out-of-focus)
    wxColour            m_colMargin;   // background colour for margin

    // NB: These *cannot* be moved to globals.
    wxArrayPtrVoid      m_arrBgBrushes; // Array of background colour brushes.
    wxArrayPtrVoid      m_arrFgCols; // Array of foreground colours.

    wxArrayString       m_sl;           // string control helper

protected:

    // Sets some members to defaults (called constructors).
	void Init1();

    // Initializes some members (called by Create and complex constructor).
	void Init2();

	void OnPaint(wxPaintEvent &event );

    // main event receivers
    void OnMouseMove( wxMouseEvent &event );
    void OnMouseClick( wxMouseEvent &event );
    void OnMouseRightClick( wxMouseEvent &event );
    void OnMouseDoubleClick( wxMouseEvent &event );
    void OnMouseUp( wxMouseEvent &event );
    void OnKey( wxKeyEvent &event );
    void OnKeyUp( wxKeyEvent &event );
    void OnNavigationKey( wxNavigationKeyEvent& event );
    void OnResize( wxSizeEvent &event );

    // event handlers
    bool HandleMouseMove( int x, unsigned int y, wxMouseEvent &event );
    bool HandleMouseClick( int x, unsigned int y, wxMouseEvent &event );
    bool HandleMouseRightClick( int x, unsigned int y, wxMouseEvent &event );
    bool HandleMouseDoubleClick( int x, unsigned int y, wxMouseEvent &event );
    bool HandleMouseUp( int x, unsigned int y, wxMouseEvent &event );
    void HandleKeyEvent( wxKeyEvent &event );
    bool HandleChildKey( wxKeyEvent& event, bool canDestroy ); // Handle TAB and ESCAPE in control

    void OnMouseEntry( wxMouseEvent &event );

    void OnIdle( wxIdleEvent &event );
    void OnFocusEvent( wxFocusEvent &event );
    void OnChildFocusEvent( wxChildFocusEvent& event );

    bool OnMouseCommon( wxMouseEvent &event, int* px, int *py );
    bool OnMouseChildCommon( wxMouseEvent &event, int* px, int *py );

    // sub-control event handlers
    void OnMouseClickChild( wxMouseEvent &event );
    void OnMouseRightClickChild( wxMouseEvent &event );
    void OnMouseMoveChild( wxMouseEvent &event );
    void OnMouseUpChild( wxMouseEvent &event );
    void OnChildKeyDown( wxKeyEvent &event );
    void OnChildKeyUp( wxKeyEvent &event );
    //void OnFocusChild( wxFocusEvent &event );

    void OnCaptureChange( wxMouseCaptureChangedEvent &event );

    void OnScrollEvent( wxScrollWinEvent &event );

    void OnSysColourChanged( wxSysColourChangedEvent &event );

protected:

	/** Adjust the centering of the bitmap icons (collapse / expand) when the caption font changes. They need to
	    be centered in the middle of the font, so a bit of deltaY adjustment is needed.
        On entry, m_captionFont must be set to window font. It will be modified properly.
    */
	void CalculateFontAndBitmapStuff( int vspacing );

    inline wxRect GetEditorWidgetRect( wxPGProperty* p );

    void CorrectEditorWidgetSizeX( int newSplitterx, int newWidth );

#ifdef __WXDEBUG__
    void _log_items();
    void OnScreenNote( const wxChar* format, ... );
#endif

    void DoDrawItems( wxDC& dc,
                      const wxPGProperty* first_item,
                      const wxPGProperty* last_item,
                      const wxRect* clip_rect );

    void DoDrawItems2( wxDC& dc,
                       const wxPGProperty* first_item,
                       const wxPGProperty* last_item,
                       const wxRect* clip_rect ) const;

    virtual void RefreshProperty( wxPGProperty* p );

    /** Draws items from topitemy to bottomitemy */
    void DrawItems( wxDC& dc, unsigned int topitemy, unsigned int bottomitemy,
                    const wxRect* clip_rect = (const wxRect*) NULL );

    void DrawItems( const wxPGProperty* p1, const wxPGProperty* p2 );

    // In addition to calling DoDrawItems directly, this is the
    // only alternative for using wxClientDC - others just call
    // RefreshRect.
    void DrawItem( wxDC& dc, wxPGProperty* p );

    inline void DrawItem( wxPGProperty* p )
    {
        DrawItems(p,p);
    }

    virtual void DrawItemAndChildren( wxPGProperty* p );

    /** Draws item, children, and consequtive parents as long as category is not met. */
    void DrawItemAndValueRelated( wxPGProperty* p );

    /** Returns property reference for given property id. */
    inline wxPGProperty& GetPropertyById( wxPGId id )
    {
        return *wxPGIdToPtr(id);
    }

    static wxPropertyCategoryClass* _GetPropertyCategory( wxPGProperty* p );

    void ImprovedClientToScreen( int* px, int* py );

    wxPGId _Insert( wxPGProperty* priorthis, wxPGProperty* newproperty );

    inline wxPGId _Insert( wxPGPropertyWithChildren* parent, int index, wxPGProperty* newproperty )
    {
        return m_pState->DoInsert(parent,index,newproperty);
    }

    // Called by focus event handlers. newFocused is the window that becomes focused.
    void HandleFocusChange( wxWindow* newFocused );

    /** Reloads all non-customized colours from system settings. */
    void RegainColours();

    bool DoEditorValidate();

    wxPGProperty* DoGetItemAtY( int y );

    inline wxPGProperty* DoGetItemAtY_Full( int y )
    {
        wxASSERT( y >= 0 );

        if ( (unsigned int)y >= m_bottomy )
            return NULL;

        return m_pState->m_properties->GetItemAtY ( y, m_lineHeight );
    }

    void DoPropertyChanged( wxPGProperty* p, unsigned int selFlags = 0 );

    void DoSetSplitterPosition( int newxpos, bool refresh = true );

    void FreeEditors();

    wxPGProperty* GetLastItem( bool need_visible, bool allow_subprops = true );

    void CalculateVisibles( int vy, bool full_recalc );

    bool _Expand( wxPGProperty* p, bool sendEvent = false );

    bool _Collapse( wxPGProperty* p, bool sendEvent = false );

    /** Forces updating the value of property from the editor control.
        Returns true if DoPropertyChanged was actually called.
    */
    bool CommitChangesFromEditor( wxUint32 flags = 0 );

    // Returns nearest paint visible property (such that will be painted unless
    // window is scrolled or resized). If given property is paint visible, then
    // it itself will be returned.
    wxPGProperty* GetNearestPaintVisible( wxPGProperty* p );

/*#ifdef __WXMSW__
    virtual WXDWORD MSWGetStyle(long flags, WXDWORD *exstyle) const;
#endif*/

    static void RegisterDefaultEditors();

    static void RegisterDefaultValues();

    // Sets m_bgColIndex to this property and all its children.
    void SetBackgroundColourIndex( wxPGProperty* p, int index, int flags );

    // Sets m_fgColIndex to this property and all its children.
    void SetTextColourIndex( wxPGProperty* p, int index, int flags );

    int CacheColour( const wxColour& colour );

    void _SetPropertyLabel( wxPGProperty* p, const wxString& newproplabel );

    void DoSetPropertyName( wxPGProperty* p, const wxString& newname );

    void SetPropertyValue( wxPGId id, const wxPGValueType* typeclass, const wxPGVariant& value );

    void SetPropertyValue( wxPGId id, const wxChar* typestring, const wxPGVariant& value );

    // Setups event handling for child control
    void SetupEventHandling( wxWindow* wnd, int id );

    void CustomSetCursor( int type, bool override = false );

    void RecalculateVirtualSize();

    void PGAdjustScrollbars( int y );

    inline bool UsesAutoUnspecified() const
    {
        return ( GetExtraStyle() & wxPG_EX_AUTO_UNSPECIFIED_VALUES ) ? true : false;
    }

    /** When splitter is dragged to a new position, this is drawn. */
    void DrawSplitterDragColumn( wxDC& dc, int x );

    /** If given index is -1, scans for item to either up (dir=0) or down (dir!=0) */
    //int GetNearestValidItem ( int index, int dir );
    wxPGProperty* GetNeighbourItem( wxPGProperty* item, bool need_visible,
                                    int dir ) const;

    void PrepareAfterItemsAdded();

    void SendEvent( int eventType, wxPGProperty* p, unsigned int selFlags = 0 );

    bool SetPropertyPriority( wxPGProperty* p, int priority );

private:
    bool ButtonTriggerKeyTest( wxKeyEvent &event );

#endif // DOXYGEN_SHOULD_SKIP_THIS

private:
    DECLARE_EVENT_TABLE()
#endif // #ifndef SWIG
};

#undef wxPG_USE_STATE
