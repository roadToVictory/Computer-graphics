#include <wx/wx.h>
#include "GUIMyFrame1.h"

float max(float d[100][3], int n) {
    float tmp = d[0][2];

    for (int i = 1; i < n; i++)
        if (tmp < d[i][2])
            tmp = d[i][2];
    return tmp;
}

float min(float d[100][3], int n) {
    float tmp = d[0][2];

    for (int i = 1; i < n; i++)
        if (tmp > d[i][2])
            tmp = d[i][2];
    return tmp;
}

float Shepard(float d[100][3], int n, float x, float y) {
    float l = .0;
    float m = .0;

    for (int i = 0; i < n; i++) {
        float wk = 1.0 / ((x - d[i][0]) * (x - d[i][0]) + (y - d[i][1]) * (y - d[i][1]));      //waga
        l += wk * d[i][2];
        m += wk;
    }
    return l / m;
}

//funkcja zwalniajaca zaalokowana pamiec
void cleaner(float** d) {
    for (int i = 0; i < 500; i++)
        delete[] d[i];
    delete[] d;
    d = nullptr;
}

void GUIMyFrame1::DrawMap(int N, float d[100][3], bool Contour, int MappingType, int NoLevels, bool ShowPoints)
{
 wxMemoryDC memDC;
 memDC.SelectObject(MemoryBitmap);
 memDC.SetBackground(*wxWHITE_BRUSH);
 memDC.Clear();
    
 memDC.SetPen(*wxBLACK_PEN);
 memDC.SetBrush(*wxTRANSPARENT_BRUSH);

 unsigned char* pix = image.GetData();

 float** Vl = new float* [500];

 for (int i = 0; i < 500; i++)
     Vl[i] = new float[500]; //500x500 zgodnie z poleceniem
 
 for (int i = 0; i < 500; i++) {
     for (int j = 0; j < 500; j++) {
         float v = Shepard(d, N, j / 100.0 - 2.5, i / 100.0 * (-1) + 2.5);
         Vl[i][j] = v;
     }
 }

 float mini = min(d, N);
 float maxi = max(d, N);

 if (MappingType) {
     for (int i = 0; i < 500; i++) {
         for (int j = 0; j < 500; j++) {
             float wxy = (Vl[i][j] - mini) / (maxi - mini);

             switch (MappingType) {
             case 1:
                 pix[500 * 3 * i + 3 * j]= (1 - wxy) * 255;
                 pix[500 * 3 * i + 3 * j + 1] = 0;
                 pix[500 * 3 * i + 3 * j + 2] = wxy * 255;
                 break;
                 
             case 2:
                 if (wxy < 0.5) {
                     pix[500 * 3 * i + 3 * j] = .0;
                     pix[500 * 3 * i + 3 * j + 1] = 255 *  (2 * wxy);
                     pix[500 * 3 * i + 3 * j + 2] = (255 * (1-2*wxy));
                 }
                 else {
                     pix[500 * 3 * i + 3 * j] = 0.5 * 255 * wxy;
                     pix[500 * 3 * i + 3 * j + 1] = 255 * 2 * (1 - wxy);
                     pix[500 * 3 * i + 3 * j + 2] = 0;
                 }
                 break;

             case 3:
                 pix[500 * 3 * i + 3 * j] = pix[500 * 3 * i + 3 * j + 1] = pix[500 * 3 * i + 3 * j + 2] = wxy * 255;
                 break;
             default:
                 break;
             }
         }
     }
   
     memDC.DrawBitmap(wxBitmap(image),0,0,false);
 }

 if (Contour) {
     int c;
     for (int i = 0; i < NoLevels; i++) {
         float tresh = (i + 1.0) * (maxi - mini) / (NoLevels + 1.0) + mini;

         for (int y = 0; y < 499; y++) {
             for (int x = 0; x < 499; x++) {
                 c = 0;
                 if (Vl[y + 1][x] < tresh) 
                     c += 1;

                 if (Vl[y + 1][x + 1] < tresh) 
                     c += 2;

                 if (Vl[y][x + 1] < tresh) 
                     c += 4;

                 if (Vl[y][x] < tresh) 
                     c += 8;

                 switch (c) {
                 case 1:
                     memDC.DrawPoint(x, y);
                     memDC.DrawPoint(x + 1, y + 1);
                     memDC.DrawPoint(x + 1, y);
                     break;
                 case 2:
                     memDC.DrawPoint(x, y);
                     memDC.DrawPoint(x + 1, y);
                     memDC.DrawPoint(x, y + 1);
                     break;
                 case 3:
                     memDC.DrawPoint(x, y);
                     memDC.DrawPoint(x + 1, y);
                     break;
                 case 4:
                     memDC.DrawPoint(x, y);
                     memDC.DrawPoint(x + 1, y + 1);
                     memDC.DrawPoint(x, y + 1);
                     break;
                 case 5:
                     memDC.DrawPoint(x, y);
                     memDC.DrawPoint(x + 1, y + 1);
                     break;
                 case 6:
                     memDC.DrawPoint(x, y);
                     memDC.DrawPoint(x, y + 1);
                     break;
                 case 7:
                     memDC.DrawPoint(x, y);
                     break;
                 case 8:
                     memDC.DrawPoint(x + 1, y);
                     memDC.DrawPoint(x + 1, y + 1);
                     memDC.DrawPoint(x, y + 1);
                     break;
                 case 9:
                     memDC.DrawPoint(x + 1, y);
                     memDC.DrawPoint(x + 1, y + 1);
                     break;
                 case 10:
                     memDC.DrawPoint(x + 1, y);
                     memDC.DrawPoint(x, y + 1);
                     break;
                 case 11:
                     memDC.DrawPoint(x + 1, y);
                     break;
                 case 12:
                     memDC.DrawPoint(x, y + 1);
                     memDC.DrawPoint(x + 1, y + 1);
                     break;
                 case 13:
                     memDC.DrawPoint(x + 1, y + 1);
                     break;
                 case 14:
                     memDC.DrawPoint(x, y + 1);
                     break;
                 default:
                     break;
                 }
             }
         }
     }
 }

 cleaner(Vl);   //zwalnianie pamięci
    
 if (MappingType == 1) memDC.DrawText("Mapa niebiesko-czerwona...", wxPoint(10, 30));
 if (MappingType == 2) memDC.DrawText("Mapa niebiesko-zielono-czerwona...", wxPoint(10, 30));
 if (MappingType == 3) memDC.DrawText("Mapa szara...", wxPoint(10, 30));
  
 if (Contour) memDC.DrawText("Kontury...", wxPoint(10, 10));
 if (ShowPoints) memDC.DrawText("Punkty...", wxPoint(10, 50));
      
}
