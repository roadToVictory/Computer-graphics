#include <wx/dc.h>
#include <memory>

#include "ChartClass.h"
#include "vecmat.h"

ChartClass::ChartClass(std::shared_ptr<ConfigClass> c)
{
    cfg= std::move(c);
    x_step=200;
}

void ChartClass::Set_Range()
{
 double xmin=9999.9,xmax=-9999.9,ymin=9999.9,ymax=-9999.9;
 double x,y,step;
 int i;

 xmin=cfg->Get_x_start();
 xmax=cfg->Get_x_stop();

 step=(cfg->Get_x_stop()-cfg->Get_x_start())/(double)x_step;
 x=cfg->Get_x_start();

 for (i=0;i<=x_step;i++)
  {
   y=GetFunctionValue(x);
   if (y>ymax) ymax=y;
   if (y<ymin) ymin=y;
   x=x+step;
  }

 y_min=ymin;
 y_max=ymax;
 x_min=xmin;
 x_max=xmax;
}

double ChartClass::Get_Y_min()
{
    Set_Range();
    return y_min;
}

double ChartClass::Get_Y_max()
{
    Set_Range();
    return y_max;
}

double ChartClass::GetFunctionValue(double x)
{
 if (cfg->Get_F_type()==1) return x*x;
 if (cfg->Get_F_type()==2) return 0.5*exp(4*x-3*x*x);
 return x+sin(x*4.0);
}

void ChartClass::Draw(wxDC *dc, int w, int h)
{
 dc->SetBackground(wxBrush(RGB(255, 255, 255)));
 dc->Clear();
 dc->SetPen(wxPen(RGB(255, 0, 0)));
 dc->DrawRectangle(10, 10, w - 20, h - 20);
 
 dc->SetClippingRegion(wxRect(10, 10, w - 20, h - 20)); //This is an overloaded member function, provided for convenience. It differs from the above function only in what argument(s) it involve.

 Matrix Mat1 = transMat(w, h);

 dc->SetPen(wxPen(*wxBLUE_PEN));
 AxisXY(w, h, Mat1, dc);

 dc->SetPen(wxPen(*wxRED_PEN));

 double unit;
 
 if (x_step < 0)
     unit = (cfg->Get_x1() - cfg->Get_x0()) / 10e-4;
 else
     unit = (cfg->Get_x1() - cfg->Get_x0()) / (cfg->Get_x1() - cfg->Get_x0()) / x_step;

 for (double i = cfg->Get_x_start(); i < cfg->Get_x_stop(); i += unit)
     line2d(i, i + unit, GetFunctionValue(i), GetFunctionValue(i + unit), h, Mat1, dc);
}

void ChartClass::line2d(double x_start, double x_end, double y_start, double y_end, int h, const Matrix& matrix, wxDC* dc)
{
    Vector start; start.Set(x_start, y_start); start = matrix * start;
    Vector end; end.Set(x_end, y_end); end = matrix*end;
    
    dc->DrawLine(start.GetX(), h-start.GetY(), end.GetX(), h-end.GetY());
}

void ChartClass::AxisXY(int w, int h, const Matrix& matrix, wxDC* dc) {
    line2d(cfg->Get_x_start(), cfg->Get_x_stop(), 0, 0, h, matrix, dc); //X
    line2d(0, 0, Get_Y_min(), Get_Y_max(), h, matrix, dc);              //Y
    double unitX = (cfg->Get_x1() - cfg->Get_x0()) /5;
    double unitY = (cfg->Get_y1() - cfg->Get_y0()) /5;

    for (double i = x_min; i < x_max; i += unitX) {
        line2d(i, i, 0.03, -0.03, h, matrix, dc);
        Vector placeXY; placeXY.Set(i - 0.03, -0.03);
        placeXY = matrix * placeXY;
        dc->DrawRotatedText(wxString::Format("%g", i), placeXY.GetX(), h - placeXY.GetY(), cfg->Get_Alpha());
    }

    for (double i = y_min; i < y_max; i += unitY) {
        line2d(0.03, -0.03, i, i, h, matrix, dc);
        Vector placeXY; placeXY.Set(0.03, i);
        placeXY = matrix * placeXY;
        dc->DrawRotatedText(wxString::Format("%g", i), placeXY.GetX(), h - placeXY.GetY(), cfg->Get_Alpha());
    }
    
}

Matrix ChartClass::transMat(int w, int h) {
    Matrix resize;
    Matrix move;
    Matrix rotation;
    Matrix move_f;
    Matrix move_b;

    double x = cfg->Get_x1() - cfg->Get_x0();
    double y = cfg->Get_y1() - cfg->Get_y0();

    if (abs(x) > 10e-5)
        resize.data[0][0] = w / x;
    else
        resize.data[0][0] = w;

    if (abs(y) > 10e-5)
        resize.data[1][1] = h / y;
    else
        resize.data[1][1] = h;

    resize.data[0][2] = 10. - resize.data[0][0] * cfg->Get_x0();
    resize.data[1][2] = 10. - resize.data[1][1] * cfg->Get_y0();

    move.data[0][0] = 1;
    move.data[1][1] = 1;
    move.data[0][2] = cfg->Get_dX();
    move.data[1][2] = cfg->Get_dY();

    double cosrot = cos(cfg->Get_Alpha() * M_PI / 180.);
    double sinrot = sin(cfg->Get_Alpha() * M_PI / 180.);
    rotation.data[0][0] = cosrot;
    rotation.data[1][1] = rotation.data[0][0];
    rotation.data[0][1] = sinrot * (-1);
    rotation.data[1][0] = sinrot;

    move_f.data[0][0] = 1.;
    move_f.data[1][1] = 1.;
    move_b.data[0][0] = 1.;
    move_b.data[1][1] = 1.;
    
    if (cfg->RotateScreenCenter()) {
        move_f.data[0][2] = w/2;    //œrodek ekranu
        move_f.data[1][2] = h/2;
    }
    else {
        move_f.data[0][2] = resize.data[0][2];
        move_f.data[1][2] = resize.data[1][2];
    }
    move_b.data[0][2] = move_f.data[0][2] * (-1);
    move_b.data[1][2] = move_f.data[1][2] * (-1);
    rotation = move_f * rotation * move_b;

    if (cfg->RotateScreenCenter())
        return rotation * move * resize;
    return move * rotation * resize;
}