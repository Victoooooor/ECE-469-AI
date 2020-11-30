#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/imagpng.h>
#include <iostream>
#include <string>
#include <vector>
#include "MyFrame.h"
#include <fstream>
MyFrame::MyFrame():wxFrame(NULL, wxID_ANY, "Checker",wxPoint(100,100),wxSize(850,870)){//setting up the general layout of the UI
    click_toggle=true;
    SetMinSize(GetSize());//cannot be bothered to handle resizing window, so lock window size
    SetMaxSize(GetSize());
    wx_checker=new checker(Emp);
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_start,"Start Game");
    menuFile->AppendSeparator();
    menuFile->Append(ID_load,"Load Game");
    menuFile->AppendSeparator();
    menuFile->Append(ID_pause,"Pause");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&Option");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText("Welcome to Checker!");
    wxFont *my_Font=new wxFont();
    my_Font->SetPointSize(20);
    m_board = new wxPanel(this, wxID_ANY);
    m_board->Bind(wxEVT_PAINT, &MyFrame::OnPaint, this);//bind is used for handling all UI interface events, similar to signal?
    m_board->Bind(wxEVT_LEFT_DCLICK, &MyFrame::clickhandle, this, wxID_ANY);//all events disabled during calculation
    m_board->SetFont(*my_Font);
    Bind(wxEVT_MENU, &MyFrame::OnStart, this, ID_start);
    Bind(wxEVT_MENU, &MyFrame::loadgame, this, ID_load);
    Bind(wxEVT_MENU, &MyFrame::OnPause, this, ID_pause);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    wxInitAllImageHandlers();
    wxImage im("./chess.png", wxBITMAP_TYPE_PNG);
    wxBitmap temp(im);
    m_black = temp.GetSubBitmap(wxRect(535,113,100,100));
    m_blackKing = temp.GetSubBitmap(wxRect(5,113,100,100));
    m_white = temp.GetSubBitmap(wxRect(535,5,100,100));
    m_whiteKing = temp.GetSubBitmap(wxRect(5,5,100,100));
    moves=NULL;
    srand((unsigned)time(NULL));
}
void MyFrame::OnExit(wxCommandEvent& event)//triggered when 
{
    Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a Checker Game made by Victoooooor",
                 "About Checker", wxOK | wxICON_INFORMATION);
}
void MyFrame::loadgame(wxCommandEvent& event){
    int result;
    wxTextEntryDialog *dial =new wxTextEntryDialog(NULL, 
      wxT("Enter file path & name"), wxT("Question"));
    result=dial->ShowModal();
    if(result!=wxID_OK){
        Display(&(wx_checker->board));
        return;
    }
    std::ifstream file;
    file.open(dial->GetValue());
    if(!file.is_open()){
        ErrMsg(wxT("Open File Failed"));
        this->OnPlay();
        return;
    }
    uint8_t temp[8][8];
    wx_checker->copy_board(&temp,Emp);
    int black=0;
    int white=0;
    int buff=0;
    try{
        for(int i =0;i<8;i++){
            for(int j=0;j<4;j++){
                file>>buff;
                temp[i][2*j+(i+1)%2]=buff;
            }
        }
        file>>buff;
        wx_checker->game.next_player=buff;
        file>>wx_checker->game.timelim;
    }
    catch(...){
        ErrMsg(wxT("Read File Error"));
    }
    wxMessageDialog *dialed;
    dialed = new wxMessageDialog(NULL, 
      wxT("Player Black setting: (Default: Human)"), wxT("Question"), 
      wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    dialed->SetYesNoLabels(_("&Human"), _("&Agent"));
    result=dialed->ShowModal();
    if (result==wxID_NO)    wx_checker->game.player[0]=false;
    else    wx_checker->game.player[0]=true;

    dialed = new wxMessageDialog(NULL, 
      wxT("Player White setting: (Default: Agent)"), wxT("Question"), 
      wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    dialed->SetYesNoLabels(_("&Human"), _("&Agent"));
    result=dialed->ShowModal();
    if(result==wxID_YES)    wx_checker->game.player[1]=true;
    else    wx_checker->game.player[1]=false;


    wx_checker->copy_board(&(wx_checker->board),temp);
    if(!(wx_checker->game.player[0]||wx_checker->game.player[1]))   while(OnPlay()); //for handling agent vs agent
    else    this->OnPlay();
    return;
}
void MyFrame::OnStart(wxCommandEvent& event){
    delete this->wx_checker;
    wx_checker=new checker(black_start);
    int result;
    wxMessageDialog *dial = new wxMessageDialog(NULL, 
      wxT("Choose Color [Black (default) moves first]"), wxT("Question"), 
      wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    dial->SetYesNoLabels(_("&Black"), _("&White"));
    result=dial->ShowModal();
    if (result==wxID_NO){
        wx_checker->game.next_player=2;
    }
    else{
        wx_checker->game.next_player=1;
    }
    dial = new wxMessageDialog(NULL, 
      wxT("Player Black setting: (Default: Human)"), wxT("Question"), 
      wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    dial->SetYesNoLabels(_("&Human"), _("&Agent"));
    result=dial->ShowModal();
    if (result==wxID_NO)    wx_checker->game.player[0]=false;
    else    wx_checker->game.player[0]=true;

    dial = new wxMessageDialog(NULL, 
      wxT("Player White setting: (Default: Agent)"), wxT("Question"), 
      wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    dial->SetYesNoLabels(_("&Human"), _("&Agent"));
    result=dial->ShowModal();
    if(result==wxID_YES)    wx_checker->game.player[1]=true;
    else    wx_checker->game.player[1]=false;

    wxNumberEntryDialog *T_lim = new wxNumberEntryDialog(NULL,wxT("Enter Agent Time Limit:"), wxT("Question"),
    wxT("Caption"),5,1,3600);
    wx_checker->game.timelim=5;
    if(!(wx_checker->game.player[0]&&wx_checker->game.player[1])){
        while(T_lim->ShowModal()!=wxID_OK);
        wx_checker->game.timelim=T_lim->GetValue();
    }
    if(!(wx_checker->game.player[0]||wx_checker->game.player[1]))   while(OnPlay()); //for handling agent vs agent
    else    this->OnPlay();
    return;
}

void MyFrame::OnPause(wxCommandEvent& event)
{ 
    wxLogMessage("Game Paused, Close to resume");
}

void MyFrame::OnPaint(wxPaintEvent&)
{   
    click_toggle=true;
    MyFrame::Display(&(wx_checker->board));
}

void MyFrame::Display(const uint8_t (*Board)[8][8]){
    wxPaintDC dc(m_board);
    dc.Clear();
    dc.SetPen(wxColour(209,139,71));
    dc.SetBrush(wxColour(209,139,71));
    dc.DrawRectangle(0,0,850,870);
    dc.SetPen(wxColour(255,206,158));
    dc.SetBrush(wxColour(255,206,158));
    for ( int i = 0 ; i< 8 ; ++i )
    {
        for ( int j = i%2 ; j< 8 ; j+=2 )
        {
            dc.DrawRectangle(i*100,j*100,100,100);
        }
        dc.DrawText(wxString(std::to_string(8-i)), wxPoint(810, i*100+35));
        dc.DrawText(wxString((char)('A'+i)), wxPoint(i*100+40,810));
    }
    wxBitmap temp;
    for (int i =0;i<8;i++){
        for(int j=0;j<8;j++){
            if((*Board)[i][j]==0)  continue;
            else if((*Board)[i][j]==1)  temp=m_black;
            else if((*Board)[i][j]==2)  temp=m_white;
            else if((*Board)[i][j]==3)  temp=m_blackKing;
            else if((*Board)[i][j]==4)  temp=m_whiteKing;
            dc.DrawBitmap(temp, j*100, i*100, true);
        }
    }
}

void MyFrame::clickhandle(wxMouseEvent& event){
    int x;
    int y;
    event.GetPosition(&y,&x);
    x/=100;
    y/=100;
    if(x>8||y>8||wx_checker->board==Emp)    return;
    if(moves==NULL) OnPlay();
    if(click_toggle){
        if(wx_checker->board[x][y]==0 
        ||(2-wx_checker->board[x][y]%2!=wx_checker->game.next_player)
        ||wx_checker->game.player[1-wx_checker->board[x][y]%2]==false)  return;
        Display(&(this->wx_checker->board));
        MyFrame::drawbox(y,x);
        for(auto i : *moves){
            if(i.x[0]==x && i.y[0]==y && i.jump==can_jump){
                MyFrame::drawbox2(i.y[i.m_len],i.x[i.m_len],255,255,255);
            }
        }
        x_temp=x;
        y_temp=y;
        click_toggle=false;
    }
    else{
        short max_len=-1;
        checker::move choice;
        for(auto i: *moves){
            if(i.x[0]==x_temp && i.y[0]==y_temp && i.jump==can_jump){
                if(i.y[i.m_len]==y&&i.x[i.m_len]==x&&i.m_len>max_len){
                    max_len=i.m_len;
                    choice=i;
                }
            }
        }
        if (max_len!=-1){
            wx_checker->copy_board(&(wx_checker->board),choice.next_board);
            if((x==7 || x==0)&&wx_checker->board[x][y]<3){
                wx_checker->board[x][y]+=2;//regular to king
            }
            wx_checker->game.next_player=3-wx_checker->game.next_player;
            this->OnPlay();
        }
        Display(&(this->wx_checker->board));
        click_toggle=true;
        
    }
    
}   

void MyFrame::drawbox(int x, int y,int r,int g,int b){
    wxPaintDC dc(m_board);
    dc.SetPen(wxColour(r,g,b));
    dc.SetBrush(wxColour(r,g,b));
    dc.DrawRectangle(x*100+5,y*100+5,5,90);
    dc.DrawRectangle(x*100+5,y*100+5,90,5);
    dc.DrawRectangle(x*100+90,y*100+5,5,90);
    dc.DrawRectangle(x*100+5,y*100+90,90,5);
}
void MyFrame::drawbox2(int x, int y,int r,int g,int b){
    wxPaintDC dc(m_board);
    dc.SetPen(wxColour(r,g,b));
    dc.SetBrush(wxColour(r,g,b));
    dc.DrawRectangle(x*100,y*100,5,100);
    dc.DrawRectangle(x*100,y*100,100,5);
    dc.DrawRectangle(x*100+95,y*100,5,100);
    dc.DrawRectangle(x*100,y*100+95,100,5);
}
void MyFrame::ErrMsg(const wxString &message){
    wxMessageDialog *dial = new wxMessageDialog(NULL, 
        message, wxT("Error"), wxICON_ERROR);
    dial->ShowModal();
    MyFrame::Display(&(wx_checker->board));
    return;
}

bool MyFrame::OnPlay(){
    Display(&(this->wx_checker->board));
    m_board->SetEvtHandlerEnabled(false);//disable all events,ensure calculation part behaves as expected
    can_jump=false;
    click_toggle=true;
    delete moves;
    moves=new vector<checker::move> (wx_checker->Getmove(wx_checker->board,wx_checker->game.next_player));//wxwidgets doesnt like having vector<move> in frame class,reason unknown
    SetStatusText("No Jump Available");
    for(auto i : *moves){
        if(i.jump){
            can_jump=true;
            SetStatusText("Jump Available, Must perform Jump");
            break;
        } 
    }
    if(moves->size()){//game continues
        wx_checker->move_print(moves);
        if(wx_checker->game.player[wx_checker->game.next_player-1]){//human player move
            m_board->SetEvtHandlerEnabled(true);
            return true;
        }
        else{//agent
            checker::move agent_move;
            agent_move=wx_checker->agent(moves);
            wx_checker->copy_board(&(wx_checker->board),agent_move.next_board);
            uint8_t x=agent_move.x[agent_move.m_len];
            uint8_t y=agent_move.y[agent_move.m_len];
            if((x==7 || x==0)&&wx_checker->board[x][y]<3)
                wx_checker->board[x][y]+=2;//regular->king
            
            wx_checker->game.next_player=3-wx_checker->game.next_player;
            Display(&(this->wx_checker->board));
            delete moves;
            moves=NULL;
            if(!wx_checker->Getmove(wx_checker->board,wx_checker->game.next_player).size())//when assigning moves at this point, clickhandle does not work as intended, so only using Getmove to check win condition
                goto TAG_WIN;
        }
    }
    else{
        //wins
        TAG_WIN:
        if(wx_checker->game.next_player==2)
            ErrMsg("Black Won!");
        else
            ErrMsg("White Won!");
        wx_checker->copy_board(&(wx_checker->board),Emp);
        Display(&(this->wx_checker->board));
        m_board->SetEvtHandlerEnabled(true);
        return false;
    }
    m_board->SetEvtHandlerEnabled(true);
    return true;
}
