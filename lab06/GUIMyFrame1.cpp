#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
 m_staticText1->SetLabel(_(L"Jasno\u015B\u0107"));
 m_b_threshold->SetLabel(_(L"Pr\u00F3g 128"));
 this->SetBackgroundColour(wxColor(192, 192, 192));
 m_scrolledWindow->SetScrollbars(25, 25, 52, 40);
 m_scrolledWindow->SetBackgroundColour(wxColor(192, 192, 192));
}

void GUIMyFrame1::Repaint()
{
	wxBitmap bitmap(Img_Cpy);          // Tworzymy tymczasowa bitmape na podstawie Img_Cpy
	wxClientDC dc(m_scrolledWindow);   // Pobieramy kontekst okna
	m_scrolledWindow->DoPrepareDC(dc); // Musimy wywolac w przypadku wxScrolledWindow, zeby suwaki prawidlowo dzialaly
	dc.DrawBitmap(bitmap, 0, 0, true); // Rysujemy bitmape na kontekscie urzadzenia
}

void GUIMyFrame1::m_scrolledWindow_update(wxUpdateUIEvent& event)
{
	Repaint();
}

void GUIMyFrame1::m_s_brightness_scroll(wxScrollEvent& event)
{
	Brightness(m_s_brightness->GetValue() - 100);
	Repaint();
}

void GUIMyFrame1::m_s_contrast_scroll(wxScrollEvent& event)
{
	Contrast(m_s_contrast->GetValue() - 100);
	Repaint();
}


void GUIMyFrame1::m_b_grayscale_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy().ConvertToGreyscale();
}

void GUIMyFrame1::m_b_blur_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy().Blur(5);
}

void GUIMyFrame1::m_b_mirror_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy().Mirror(TRUE);	//horizontal = true
}

void GUIMyFrame1::m_b_replace_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.Replace(254, 0, 0, 0, 0, 255);
}

void GUIMyFrame1::m_b_rescale_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	m_scrolledWindow->ClearBackground();
	Img_Cpy.Rescale(320, 240);
}

void GUIMyFrame1::m_b_rotate_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Rotate(30. * M_PI / 180., wxPoint(Img_Org.GetSize().x, Img_Org.GetSize().y));
}

void GUIMyFrame1::m_b_rotate_hue_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.RotateHue(180. / 360.);
}

void GUIMyFrame1::m_b_mask_click( wxCommandEvent& event )
{
	wxClientDC dc(m_scrolledWindow);
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.SetMaskFromImage(Img_Mask, 0, 0, 0);
	dc.Clear();
}

void GUIMyFrame1::m_b_prewitt_click( wxCommandEvent& event ){}

void GUIMyFrame1::m_b_threshold_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	unsigned char* data = Img_Cpy.GetData();
	int tresh = 128;
	int size = Img_Cpy.GetSize().x * Img_Cpy.GetSize().y*3;

	for (int i = 0; i < size; i++){
		if (data[i] < tresh)
			data[i] = 0;
		else data[i] = 255;	
	}
}


void GUIMyFrame1::Contrast(int value)
{
	Img_Cpy = Img_Org.Copy();
	unsigned char* data = Img_Cpy.GetData();
	int size = Img_Cpy.GetSize().x * Img_Cpy.GetSize().y * 3;
	double tmp;
	double x = (100. + value) / (101. - value);

	for (int i = 0; i < size; i++) {
		tmp = x * (data[i] - 127) + 127;
		if (tmp < 0)
			data[i] = 0;
		else if (tmp > 255)
			data[i] = 255;
		else data[i] = tmp;
	}
}



void GUIMyFrame1::Brightness(int value)
{
	Img_Cpy = Img_Org.Copy();
	unsigned char* data = Img_Cpy.GetData();
	int size = Img_Cpy.GetSize().x * Img_Cpy.GetSize().y * 3;
	double tmp = 0.;
	double x = (100. + value) / (101. - value);

	for (int i = 0; i < size; i++) {
		tmp = (double)data[i] + value;
		if (tmp < 0)
			data[i] = 0;
		else if (tmp > 255)
			data[i] = 255;
		else data[i] = tmp;
	}
}