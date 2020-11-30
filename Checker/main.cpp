#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <iostream>
#include <string>
#include <vector>
#include "MyFrame.h"
 
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{ 
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

