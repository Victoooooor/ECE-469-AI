#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/numdlg.h>
#include <iostream>
#include <string>
#include <vector>
#include "checker.h"
class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    checker *wx_checker;
    vector<checker::move> *moves;
    bool can_jump;
    bool click_toggle;
    uint8_t x_temp;
    uint8_t y_temp;
    wxPanel* m_board;
    wxBitmap m_black; 
    wxBitmap m_blackKing;
    wxBitmap m_white;
    wxBitmap m_whiteKing;
    void OnPaint(wxPaintEvent& event);
    void ErrMsg(const wxString &message);
    void OnStart(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void loadgame(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
    bool OnPlay(); 
    void Display(const uint8_t (*Board)[8][8]);
    void clickhandle(wxMouseEvent& event);
    void drawbox(int x, int y,int r=0,int g=0,int b=0);
    void drawbox2(int x, int y,int r,int g,int b);
};

enum
{
    ID_start = 1,
    ID_pause = 2,
    ID_load =3
};

const uint8_t black_start[8][8]{
    {0,2,0,2,0,2,0,2},
    {2,0,2,0,2,0,2,0},
    {0,2,0,2,0,2,0,2},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {1,0,1,0,1,0,1,0},
    {0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,0},
};

const uint8_t Emp[8][8]{
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
};
