#pragma once

#include "GUI.h"

#include <wx/window.h>
#include <wx/filedlg.h>
#include <wx/msw/filedlg.h>
#include <wx/dcbuffer.h>
#include <FreeImage.h>
#include "CImg.h"

/** Implementing MyFrame1 */
class GUIMyFrame : public MyFrame
{
protected:
	void onUpdateUI(wxUpdateUIEvent& event) override;
	void loadImg(wxCommandEvent& event) override;
	void Cenzure(wxCommandEvent& event) override;
	void Erode(wxCommandEvent& event) override;

	wxImage* tmp;
	wxImage* m_original_image;

public:
	GUIMyFrame(wxWindow* parent);
	~GUIMyFrame();
};