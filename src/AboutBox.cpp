// AboutBox.cpp
// Copyright (c) 2009, Dan Heeks
// This program is released under the BSD license. See the file COPYING for details.

#include "stdafx.h"
#include "AboutBox.h"
#include "HeeksFrame.h"

BEGIN_EVENT_TABLE( CAboutBox, wxDialog )
EVT_BUTTON( wxID_OK, CAboutBox::OnButtonOK )
END_EVENT_TABLE()

CAboutBox::CAboutBox(wxWindow *parent):wxDialog(parent, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{
    wxPanel *panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

    wxButton *ok_button = new wxButton(panel, wxID_OK, _("OK"));

    wxBoxSizer *mainsizer = new wxBoxSizer( wxVERTICAL );

	wxTextCtrl* text_ctrl = new wxTextCtrl(panel, wxID_ANY, _T(""),	wxDefaultPosition, wxSize(800,600), wxTE_READONLY | wxTE_MULTILINE);

	wxString str = wxString(_T("HeeksCAD\n see http://sites.google.com/site/heekscad/, or for source code: http://github.com/danheeks/heekscad\n\nusing Open CASCADE solid modeller - http://www.opencascade.org"))
		+ wxString(_T("\n\nwindows made with wxWidgets 2.8.9 - http://wxwidgets.org"))
		+ wxString(_T("\n\ntext uses glFont Copyright (c) 1998 Brad Fish E-mail: bhf5@email.byu.edu Web: http://students.cs.byu.edu/~bfish/"))
		+ wxString(_T("\n\nWritten by:\n Dan Heeks\n"))
		+ wxString(_T("\n\nWith contributions from:\n"
                      "- David Nicholls\n"
                      "- Perttu Ahola\n"
                      "- Dave (the archivist)\n"
                      "- mpictor\n"
                      "- fenn\n"
                      "- fungunner2\n"
                      "- andrea (openSUSE)\n"
                      "- g_code\n"
                      "- Luigi Barbati (Italian translation)\n"
                      "- Andre Pascual (French translation)\n"
                      "- Juan Carlos Gavari (Italian translation)"))
		+ wxString(_T("\n\nThis is free, open source software."));

	wxString version_str = wxGetApp().m_version_number;
	version_str.Replace(_T(" "), _T("."));

	this->SetTitle(version_str);

	text_ctrl->WriteText(str + wxGetApp().m_frame->m_extra_about_box_str);

    //mainsizer->Add( text_ctrl, wxSizerFlags().Align(wxALIGN_CENTER).Border(wxALL, 10).Expand() );
    //mainsizer->Add( ok_button, wxSizerFlags().Align(wxALIGN_CENTER) );
	mainsizer->Add( text_ctrl );
	mainsizer->Add( ok_button );
	mainsizer->Layout();

    // tell frame to make use of sizer
    panel->SetAutoLayout( true );
    panel->SetSizer( mainsizer );

#ifndef __WXWINCE__
    // don't allow frame to get smaller than what the sizers tell ye
    mainsizer->SetSizeHints( this );
#endif

    Show(true);
}

void CAboutBox::OnButtonOK(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}
