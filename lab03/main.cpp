#include <wx/wxprec.h>
#include <wx/colordlg.h>
#include <wx/dcbuffer.h>
#include <wx/brush.h>
#include <wx/dc.h>
#include <wx/fontenum.h>

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
	MyFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~MyFrame();
private:
	void OnExit(wxCloseEvent& e);
	void Save(wxCommandEvent& e);
	void Check_Banan(wxCommandEvent& e);
	void Scroll(wxScrollEvent& e);
	void set_Color(wxCommandEvent& e);
	void set_Text(wxCommandEvent& e);
	void Combo_choice(wxCommandEvent& e);
	void Font_Choice(wxCommandEvent& e);
	void Paint_guy();
	void UpdateUI(wxUpdateUIEvent& e);

	enum {
		ID_ButtonSave = 1001,
		ID_CheckBox_Banan = 1002,
		ID_ScrollBar = 1003,
		ID_Gauge = 1004,
		ID_ButtonColor = 1005,
		ID_TextCtrl = 1006,
		ID_ComboBox = 1007,
	};

	wxPanel* m_panel;
	wxButton* m_buttonSave;
	wxCheckBox* m_checkBox;
	wxScrollBar* m_scrollBar;
	wxGauge* m_gauge;
	wxButton* m_buttonColor;
	wxTextCtrl* m_textCtrl;
	wxComboBox* m_comboBox;

	unsigned choice = 0;
	wxString updateText = "tekst";
	wxImage MyImage;
	wxBitmap banan;
	wxBitmap bitmap;
	wxColor color;
	wxFileDialog* file;

	wxComboBox* m_comboBoxFont;
	unsigned choiceFont = 0;
};

bool MyApp::OnInit()
{
	MyFrame* frame = new MyFrame(NULL, wxID_ANY, "lab03 - ludzik z bananem");
	frame->Show(true);
	return true;
}
wxIMPLEMENT_APP(MyApp);

MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(500, 300);

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizerL;
	bSizerL = new wxBoxSizer(wxHORIZONTAL);

	m_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	m_panel->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK));

	bSizerL->Add(m_panel, 1, wxEXPAND | wxALL, 5);

	bSizer->Add(bSizerL, 1, wxEXPAND, 5);

	wxBoxSizer* bSizerR;
	bSizerR = new wxBoxSizer(wxVERTICAL);

	m_buttonSave = new wxButton(this, ID_ButtonSave, wxT("zapis do pliku"), wxDefaultPosition, wxDefaultSize, 0);
	bSizerR->Add(m_buttonSave, 0, wxALL, 5);

	m_checkBox = new wxCheckBox(this, ID_CheckBox_Banan, wxT("banan"), wxDefaultPosition, wxDefaultSize, 0);
	bSizerR->Add(m_checkBox, 0, wxALL, 5);

	m_scrollBar = new wxScrollBar(this, ID_ScrollBar, wxDefaultPosition, wxSize(185, -1), wxSB_HORIZONTAL);
	m_scrollBar->SetScrollbar(0, 1, 100, 1);
	m_scrollBar->Disable();
	bSizerR->Add(m_scrollBar, 0, wxALL, 5);

	m_gauge = new wxGauge(this, ID_Gauge, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	m_gauge->SetValue(0);
	bSizerR->Add(m_gauge, 0, wxALL, 5);

	m_buttonColor = new wxButton(this, ID_ButtonColor, wxT("kolor gwiazdki"), wxDefaultPosition, wxDefaultSize, 0);
	bSizerR->Add(m_buttonColor, 0, wxALL, 5);

	m_textCtrl = new wxTextCtrl(this, ID_TextCtrl, updateText, wxDefaultPosition, wxDefaultSize, 0);
	bSizerR->Add(m_textCtrl, 0, wxALL, 5);

	wxArrayString m_choicesFont;
	m_comboBoxFont = new wxComboBox(this, wxID_ANY, wxT("wybierz czcionke"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	wxArrayString SystemFonts;
	SystemFonts = wxFontEnumerator::GetFacenames();
	for (unsigned int i = 0; i < SystemFonts.Count(); i++) 
		m_comboBoxFont->Append(SystemFonts[i]);
	m_comboBoxFont->SetSelection(0);
	bSizerR->Add(m_comboBoxFont, 0, wxALL, 5);

	m_comboBox = new wxComboBox(this, ID_ComboBox, wxT("gwiazdka"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	m_comboBox->Append(wxT("gwiazdka"));
	m_comboBox->Append(wxT("ksiezyc"));
	m_comboBox->Append(wxT("slonko"));
	m_comboBox->SetSelection(0);
	bSizerR->Add(m_comboBox, 0, wxALL, 5);

	bSizer->Add(bSizerR, 0, wxEXPAND, 5);

	this->SetSizer(bSizer);
	this->Layout();
	this->Centre(wxBOTH);

	Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnExit, this);
	Bind(wxEVT_BUTTON, &MyFrame::Save, this, ID_ButtonSave);
	Bind(wxEVT_CHECKBOX, &MyFrame::Check_Banan, this, ID_CheckBox_Banan);
	Bind(wxEVT_SCROLL_THUMBTRACK, &MyFrame::Scroll, this, ID_ScrollBar);
	Bind(wxEVT_BUTTON, &MyFrame::set_Color, this, ID_ButtonColor);
	Bind(wxEVT_TEXT, &MyFrame::set_Text, this, ID_TextCtrl);
	Bind(wxEVT_COMBOBOX, &MyFrame::Combo_choice, this, ID_ComboBox);
	Bind(wxEVT_UPDATE_UI, &MyFrame::UpdateUI, this);
	m_comboBoxFont->Bind(wxEVT_CHOICE, &MyFrame::Font_Choice, this);

	color = wxColor(*wxBLACK);
	wxImage b = wxImage();
	b.AddHandler(new wxPNGHandler);
	b.LoadFile("banan.png", wxBITMAP_TYPE_PNG);
	banan = wxBitmap(b);
}

MyFrame::~MyFrame(){}

void MyFrame::OnExit(wxCloseEvent& e) {
	if (wxMessageDialog(NULL, "Czy napewno chcesz wyjsc?", "Warning", wxYES | wxNO).ShowModal() == wxID_YES) Destroy();
}

void MyFrame::Save(wxCommandEvent& e) {
	file = new wxFileDialog(this, _("Choose a file:"), _(""), _(""), _("JPG files (*.jpg)|*.jpg"), wxFD_SAVE);
	if (file->ShowModal() == wxID_OK) {
		MyImage = bitmap.ConvertToImage();
		MyImage.AddHandler(new wxJPEGHandler);
		//MyImage.SaveFile("picture.jpg"); //zapisuje bezposrednio w katalogu w ktorym znajduje sie plik zrodlowy
		MyImage.SaveFile(file->GetPath());	//zapisuje w dowolnie wybranej lokalizacji
	}
}

void MyFrame::Check_Banan(wxCommandEvent& e) {
	if (m_checkBox->IsChecked())
		m_scrollBar->Enable();
	else m_scrollBar->Disable();
}

void MyFrame::Scroll(wxScrollEvent& e) {
	m_gauge->SetValue(m_scrollBar->GetThumbPosition());
	Paint_guy(); ////////podnoszenie
}


void MyFrame::set_Color(wxCommandEvent& e) {
	wxColourDialog colour(this, nullptr);
	if (colour.ShowModal() == wxID_OK)
		color = colour.GetColourData().GetColour();
}


void MyFrame::set_Text(wxCommandEvent& e) {
	updateText = m_textCtrl->GetValue();
}


void MyFrame::Combo_choice(wxCommandEvent& e) {
	choice = m_comboBox->GetSelection();
}

void MyFrame::Font_Choice(wxCommandEvent& e) {
	choiceFont = m_comboBoxFont->GetSelection();
	Refresh();
}

void MyFrame::Paint_guy() {
	wxClientDC ClientDC(m_panel);
	bitmap = wxBitmap(m_panel->GetSize().x, m_panel->GetSize().y);
	wxBufferedDC BufferDC(&ClientDC, bitmap);
	
	int x = (m_panel->GetVirtualSize().GetWidth()) / 2;
	int y = (m_panel->GetVirtualSize().GetHeight()) / 2;	//x,y srodek 
	BufferDC.SetDeviceOrigin(0, 0);							//centrowanie rysunku

	BufferDC.SetBackground(*wxWHITE_BRUSH);
	BufferDC.Clear();
	ClientDC.SetPen(wxPen(*wxBLACK, 1));
	BufferDC.SetPen(wxPen(*wxBLACK_PEN));

	BufferDC.DrawCircle(x, y - 45, 25); 					//glowa
	if (m_checkBox->IsChecked()) {
		BufferDC.DrawBitmap(banan, x - 75, y - 0.4 * m_gauge->GetValue());
		BufferDC.DrawEllipse(x - 15, y - 57, 10, 8);	 	//lewe oko
		BufferDC.DrawEllipse(x + 5, y - 57, 8, 8); 			//prawe
		BufferDC.DrawEllipticArc(x - 18, y-45, 35, 15, 180, 360);
	}
	else {
		BufferDC.DrawEllipse(x - 15, y - 57, 10, 8); 		//lewe oko
		BufferDC.DrawEllipse(x + 5, y - 57, 8, 10); 		//prawe
		BufferDC.DrawEllipticArc(x - 18, y-42, 35, 15, 0, 180);
	}
	
	BufferDC.DrawLine(x, y - 20, x, y + 50); 				//tulow
	BufferDC.DrawLine(x, y, x + 50, y + 20); 				//prawa reka
	BufferDC.DrawLine(x,y,x-50,y+20-0.4*m_gauge->GetValue()); //lewa reka
	BufferDC.DrawLine(x, y + 50, x + 30, y + 100); 			//prawa noga
	BufferDC.DrawLine(x, y + 50, x - 30, y + 100); 			//lewa noga

	wxPoint star[5] = { wxPoint(x - 60, y - 90),wxPoint(x - 40, y - 40),wxPoint(x - 85, y - 70),wxPoint(x - 30, y - 70),wxPoint(x - 75, y - 40) };

	switch (choice) {
		case 0: //gwiazdka
			BufferDC.SetPen(wxPen(color));
			BufferDC.SetBrush(wxBrush(color));				//ustawienie kolorow
			BufferDC.DrawPolygon(5, star, 0,0);
			break;
		case 1: //ksiezyc
			BufferDC.SetPen(wxPen(*wxLIGHT_GREY_PEN));
			BufferDC.SetBrush(wxBrush(*wxLIGHT_GREY_BRUSH));	//ustawienie kolorow
			BufferDC.DrawCircle(x-65,y-65,25);
			BufferDC.SetPen(wxPen(*wxWHITE_PEN));
			BufferDC.SetBrush(wxBrush(*wxWHITE_BRUSH));	//ustawienie kolorow
			BufferDC.DrawCircle(x - 80, y - 80, 25);
			BufferDC.SetPen(wxPen(*wxBLACK_PEN));
			BufferDC.SetBrush(wxBrush(*wxBLACK_BRUSH));	//ustawienie kolorow
			BufferDC.DrawCircle(x - 50, y - 65, 3);
			BufferDC.DrawLine(x -65, y -60, x -58, y -55);
			break;
		case 2: //slonce
			BufferDC.SetPen(wxPen(*wxYELLOW_PEN));
			BufferDC.SetBrush(wxBrush(*wxYELLOW_BRUSH));
			BufferDC.DrawCircle(x - 65, y - 65, 25);
			break;
	}

	wxString text_horizontal(updateText);
	BufferDC.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Calibri"));
	BufferDC.DrawText(text_horizontal, x -100, y + 90);
	wxString text_vertical(updateText);
	BufferDC.SetFont(wxFont(20, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, false, "FG Kristine Alt"));
	BufferDC.DrawRotatedText(text_vertical, x + 50, y + 90, 90);

}

void MyFrame::UpdateUI(wxUpdateUIEvent& e) {
	Paint_guy();
}
