#include "GUIMyFrame1.h"
#include <vector>
#include <fstream>
#include "vecmat.h"

struct Point {
 float x, y, z;
 Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Color {
 int R, G, B;
 Color(int _R, int _G, int _B) : R(_R), G(_G), B(_B) {}
};

struct Segment {
 Point begin, end;
 Color color;
 Segment(Point _begin, Point _end, Color _color) : begin(_begin), end(_end), color(_color) {}
};

std::vector<Segment> data;

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
 m_button_load_geometry->SetLabel(_("Wczytaj Geometri\u0119"));
 m_staticText25->SetLabel(_("Obr\u00F3t X:"));
 m_staticText27->SetLabel(_("Obr\u00F3t Y:"));
 m_staticText29->SetLabel(_("Obr\u00F3t Z:"));

 WxSB_TranslationX->SetRange(0, 200); WxSB_TranslationX->SetValue(100);
 WxSB_TranslationY->SetRange(0, 200); WxSB_TranslationY->SetValue(100);
 WxSB_TranslationZ->SetRange(0, 200); WxSB_TranslationZ->SetValue(100);

 WxSB_RotateX->SetRange(0, 360); WxSB_RotateX->SetValue(0);
 WxSB_RotateY->SetRange(0, 360); WxSB_RotateY->SetValue(0);
 WxSB_RotateZ->SetRange(0, 360); WxSB_RotateZ->SetValue(0);

 WxSB_ScaleX->SetRange(1, 200); WxSB_ScaleX->SetValue(100);
 WxSB_ScaleY->SetRange(1, 200); WxSB_ScaleY->SetValue(100);
 WxSB_ScaleZ->SetRange(1, 200); WxSB_ScaleZ->SetValue(100);
}

void GUIMyFrame1::WxPanel_Repaint( wxUpdateUIEvent& event )
{
 Repaint();
}

void GUIMyFrame1::m_button_load_geometry_click( wxCommandEvent& event )
{
 wxFileDialog WxOpenFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("Geometry file (*.geo)|*.geo"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

 if (WxOpenFileDialog.ShowModal() == wxID_OK)
 {
  double x1, y1, z1, x2, y2, z2;
  int r, g, b;

  std::ifstream in(WxOpenFileDialog.GetPath().ToAscii());
  if (in.is_open())
  {
   data.clear();
   while (!in.eof())
   {
    in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> r >> g >> b;
    data.push_back(Segment(Point(x1, y1, z1), Point(x2, y2, z2), Color(r, g, b)));
   }
   in.close();
  }
 }
}

void GUIMyFrame1::Scrolls_Updated( wxScrollEvent& event )
{
WxST_TranslationX->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationX->GetValue() - 100) / 50.0));
WxST_TranslationY->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationY->GetValue() - 100) / 50.0));
WxST_TranslationZ->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationZ->GetValue() - 100) / 50.0));

WxST_RotateX->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateX->GetValue()));
WxST_RotateY->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateY->GetValue()));
WxST_RotateZ->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateZ->GetValue()));

WxST_ScaleX->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleX->GetValue() / 100.0));
WxST_ScaleY->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleY->GetValue() / 100.0));
WxST_ScaleZ->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleZ->GetValue() / 100.0));


Repaint();
}

void GUIMyFrame1::Repaint()
{
    wxClientDC dc(WxPanel);
    wxBufferedDC buffer(&dc);

    buffer.Clear();

    int h, w;
    Matrix4 Trans, RotacjaX, RotacjaY, RotacjaZ, Scale, Middle;
    WxPanel->GetSize(&w, &h);  //Returns the size of the entire window in pixels, including title bar, border, scrollbars, etc.
    dc.SetClippingRegion(wxRect(0, 0, w, h));   //obcinanie obrazu poza widzianym zakresem, optymalizacja

    Middle.data[0][0] = Middle.data[1][1] = Middle.data[2][2] = 1.;
    Middle.data[0][3] = Middle.data[1][3] = 0.5;

//translacja o wektor
    Trans.data[0][0] = Trans.data[1][1] = Trans.data[2][2] = Trans.data[3][3]=1;
    Trans.data[0][3] = (WxSB_TranslationX->GetValue() - 100.) / 50.;
    Trans.data[1][3] = (-1)*(WxSB_TranslationY->GetValue() - 100.) / 50.;
    Trans.data[2][3] = 2.0+(WxSB_TranslationZ->GetValue() - 100.) / 50.;

// rotacja wokol osi X
    RotacjaX.data[0][0] = RotacjaX.data[3][3] = 1.;
    RotacjaX.data[1][1] = RotacjaX.data[2][2] = cos(WxSB_RotateX->GetValue() * M_PI / 180.);
    RotacjaX.data[2][1] = sin(WxSB_RotateX->GetValue() * M_PI / 180.);
    RotacjaX.data[1][2] = sin(WxSB_RotateX->GetValue() * M_PI / 180.) * (-1.);
// rotacja wokol osi Y
    RotacjaY.data[1][1] = RotacjaY.data[3][3] = 1.;
    RotacjaY.data[0][0] = RotacjaY.data[2][2] = cos(WxSB_RotateY->GetValue() * M_PI / 180.);
    RotacjaY.data[0][2] = sin(WxSB_RotateY->GetValue() * M_PI / 180.);
    RotacjaY.data[2][0] = sin(WxSB_RotateY->GetValue() * M_PI / 180.) * (-1.);
 // rotacja wokol osi Z
    RotacjaZ.data[2][2] = RotacjaZ.data[3][3] = 1.;
    RotacjaZ.data[0][0] = RotacjaZ.data[1][1] = cos(WxSB_RotateZ->GetValue() * M_PI / 180.);
    RotacjaZ.data[1][0] = sin(WxSB_RotateZ->GetValue() * M_PI / 180.);
    RotacjaZ.data[0][1] = sin(WxSB_RotateZ->GetValue() * M_PI / 180.) * (-1.);

    Matrix4 Rotacja = RotacjaX * RotacjaY * RotacjaZ;

    Scale.data[0][0] = WxSB_ScaleX->GetValue() / 100.;
    Scale.data[1][1] = -WxSB_ScaleY->GetValue() / 100.;
    Scale.data[2][2] = WxSB_ScaleZ->GetValue() / 100.;
    Scale.data[3][3] = 1.;

    Matrix4 matrix = Trans * Rotacja * Scale;

    for (Segment& var : data) {
        Vector4 start, end;
        start.Set(var.begin.x, var.begin.y, var.begin.z);
        end.Set(var.end.x, var.end.y, var.end.z);
        start = matrix * start;
        end = matrix * end;

        if (start.GetZ() < 10e-6) 
            start.data[2] = 1.;
        
        if (end.GetZ() < 10e-6) 
            end.data[2] = 1.;
        
        if (start.GetZ() > 10e-6 && end.GetZ() > 10e-6) {
            start.Set(start.GetX() / start.GetZ(), start.GetY() / start.GetZ(), start.GetZ());
            end.Set(end.GetX() / end.GetZ(), end.GetY() / end.GetZ(), end.GetZ());
            start = Middle * start;
            end = Middle * end;

            buffer.DrawLine(start.GetX() * w, start.GetY() * h, end.GetX() * w, end.GetY() * h);
        }
    }
}