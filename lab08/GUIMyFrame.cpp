#include "GUIMyFrame.h"
#include <iostream>


GUIMyFrame::GUIMyFrame(wxWindow* parent) : MyFrame(parent) {
	tmp = new wxImage();
	m_original_image = new wxImage();
	wxInitAllImageHandlers();
}

GUIMyFrame::~GUIMyFrame() {
	if (tmp || m_original_image) {
		delete tmp;
		delete m_original_image;
	}
}

wxString Metadane(FIBITMAP* bitmap) {
	FITAG* tag = nullptr;
	FIMETADATA* info = FreeImage_FindFirstMetadata(FIMD_EXIF_MAIN, bitmap, &tag);
	wxString metadataString = "";

	metadataString << "Rozmiar obrazka: " << FreeImage_GetWidth(bitmap) << "*" << FreeImage_GetHeight(bitmap) << "\n";

	while (FreeImage_FindNextMetadata(info, &tag))
		metadataString << FreeImage_GetTagKey(tag) << ": " << FreeImage_TagToString(FIMD_EXIF_MAIN, tag) << "\n";
		
	FreeImage_FindCloseMetadata(info);

	return metadataString;
}

void GUIMyFrame::onUpdateUI(wxUpdateUIEvent& event) {
	wxClientDC dc(m_panel1);
	wxBufferedDC buffer(&dc);
	tmp->Rescale(dc.GetSize().x, dc.GetSize().y);
	const wxBitmap bitmap(*tmp);
	buffer.DrawBitmap(bitmap, 0, 0);
}

void GUIMyFrame::loadImg(wxCommandEvent& event) {
	wxClientDC dc(m_panel1);
	wxString info = "";
	auto* dialog = new wxFileDialog(this, "Wybierz obrazek ", "./", "", wxT("Obraz JPEG (*.jpg)|*.jpg"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if (dialog->ShowModal() == wxID_CANCEL) 
		return;

	if (tmp->LoadFile(dialog->GetPath(), wxBITMAP_TYPE_JPEG)) {
		*m_original_image = tmp->Copy();

		FIBITMAP* FBI = FreeImage_Load(FIF_JPEG, dialog->GetPath(), JPEG_DEFAULT);
		m_textCtrl1->WriteText(Metadane(FBI));

		wxBufferedDC buffer(&dc);
		tmp->Rescale(dc.GetSize().x, dc.GetSize().y);
		const wxBitmap bitmap(*tmp);
		buffer.DrawBitmap(bitmap, 0, 0);
	}
}

void GUIMyFrame::Cenzure(wxCommandEvent& event) {
	wxClientDC dc(m_panel1);

	*tmp = m_original_image->Copy();

	const int h = 0.13 * tmp->GetHeight();
	const int w = 0.20 * tmp->GetWidth();
	const int Y = 0.02 * tmp->GetHeight();
	const int X = 0.55 * tmp->GetWidth();

	cimg_library::CImg<unsigned char> cImage(w, h, 1, 3);
	unsigned char* imgData = tmp->GetData();

	for (int i = 0; i < h; ++i) {
		for (int x = 0; x < w; ++x) {
			cImage(x, i, 0) = imgData[3 * ((i + Y) * tmp->GetWidth() + x + X)];
			cImage(x, i, 1) = imgData[1+3 * ((i + Y) * tmp->GetWidth() + x + X)];
			cImage(x, i, 2) = imgData[2+3 * ((i + Y) * tmp->GetWidth() + x + X)];
		}
	}

	cImage.blur(2);

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			imgData[3 * ((y + Y) * tmp->GetWidth() + x + X)] = cImage(x, y, 0);
			imgData[1+3 * ((y + Y) * tmp->GetWidth() + x + X)] = cImage(x, y, 1);
			imgData[2+3 * ((y + Y) * tmp->GetWidth() + x + X)] = cImage(x, y, 2);
		}
	}
	wxBufferedDC buffer(&dc);
	tmp->Rescale(dc.GetSize().x, dc.GetSize().y);
	const wxBitmap bitmap(*tmp);
	buffer.DrawBitmap(bitmap, 0, 0);
}

void GUIMyFrame::Erode(wxCommandEvent& event) {
	wxClientDC dc(m_panel1);
	
	*tmp = m_original_image->Copy();

	const int w = tmp->GetWidth();

	cimg_library::CImg<unsigned char> cImage(w, tmp->GetHeight(), 1, 3);
	unsigned char* imgData = tmp->GetData();

	for (int y = 0; y < tmp->GetHeight(); ++y) {
		for (int x = 0; x < w; ++x) {
			cImage(x, y, 0) = imgData[3 * (y * w + x)];
			cImage(x, y, 1) = imgData[1 + 3 * (y * w + x)];
			cImage(x, y, 2) = imgData[2 + 3 * (y * w + x)];
		}
	}

	cImage.erode(5, 5);

	for (int y = 0; y < tmp->GetHeight(); ++y)
		for (int x = 0; x < w; ++x) {
			imgData[3 * (y * w + x)] = cImage(x, y, 0);
			imgData[1 + 3 * (y * w + x)] = cImage(x, y, 1);
			imgData[2 + 3 * (y * w + x)] = cImage(x, y, 2);
		}

	wxBufferedDC buffer(&dc);
	tmp->Rescale(dc.GetSize().x, dc.GetSize().y);
	const wxBitmap bitmap(*tmp);
	buffer.DrawBitmap(bitmap, 0, 0);
}
		