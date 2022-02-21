///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GUI.h"

///////////////////////////////////////////////////////////////////////////

MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);
	bSizer2->SetMinSize(wxSize(300, 0));
	
	m_button1 = new wxButton(this, wxID_ANY, wxT("Wczytaj obrazek"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_button1, 0, wxALL |wxALIGN_CENTER, 5);

	m_button2 = new wxButton(this, wxID_ANY, wxT("Cenzura"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_button2, 0, wxALL | wxALIGN_CENTER, 5);

	m_button3 = new wxButton(this, wxID_ANY, wxT("Erode"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_button3, 0, wxALL | wxALIGN_CENTER, 5);

	m_checkBox1 = new wxCheckBox(this, wxID_ANY, wxT("Animacja"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_checkBox1, 0, wxALL | wxALIGN_CENTER, 5);

	m_gauge1 = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	m_gauge1->SetValue(0);
	m_gauge1->Hide();

	bSizer2->Add(m_gauge1, 0, wxALL | wxEXPAND, 5);

	m_textCtrl1 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	bSizer2->Add(m_textCtrl1, 1, wxALL | wxEXPAND, 5);


	bSizer1->Add(bSizer2, 0, wxEXPAND | wxFIXED_MINSIZE, 1);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);

	m_panel1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	m_panel1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));

	bSizer4->Add(m_panel1, 1, wxEXPAND, 5);


	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	this->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MyFrame::onUpdateUI));
	m_button1->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::loadImg), NULL, this);
	m_button2->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::Cenzure), NULL, this);
	m_button3->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::Erode), NULL, this);
	m_checkBox1->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MyFrame::Animation), NULL, this);
}

MyFrame::~MyFrame()
{
	// Disconnect Events
	this->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MyFrame::onUpdateUI));
	m_button1->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::loadImg), NULL, this);
	m_button2->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::Cenzure), NULL, this);
	m_button3->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::Erode), NULL, this);
	m_checkBox1->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MyFrame::Animation), NULL, this);
}