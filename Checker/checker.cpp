#include "checker.h"
#include <algorithm>
#include <limits.h>
#include <unistd.h>
checker::checker(const uint8_t m_board[8][8]){
    for(int i =0;i<8;i++){
        for(int j=0;j<8;j++){
            this->board[i][j]=m_board[i][j];
        }
    }
}
vector<checker::move> checker::Getmove(uint8_t board[8][8],uint8_t player){
    vector<move> movelist;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(board[i][j]!=0 && (2-(board[i][j]%2))==player){//for all none zero
                move first;
                first.x[0]=i;
                first.y[0]=j;
                first.m_len=0;
                first.jump=false;
                copy_board(&(first.next_board),board);
                Getpath(&movelist,&first);
            }
        }
    }
    for(int i =0;i<movelist.size();i++){
        if(movelist[i].jump)
            goto TAG;//attempt to use one less loop to remove non-jump when jump is available
    }
    return movelist;
TAG:
    move temp;
    temp.jump=false;
    movelist.erase(remove(movelist.begin(), movelist.end(), temp), movelist.end());
    return movelist;
}
void checker::Getpath(vector<move> *movelist,checker::move *root){
    int8_t x=root->x[root->m_len];
    int8_t y=root->y[root->m_len];
    bool is_king=(root->next_board[x][y]-1)/2;
    int8_t player=2-((root->next_board[x][y])%2);
    int8_t direct= player*2-3;
    bool split=false;
    if((x+direct>7||x+direct<0)&& !is_king){//cut all normal pieces reaching end, become king is handled by wxFrame
        if(root->m_len) movelist->push_back(*root);
        return;
    }
    
    move copy=*root;
    if(x+direct>-1 && x+direct <8){
        if(y>0){//check for non jump moves
            if(root->m_len==0&&root->next_board[x+direct][y-1]==0){
                (root->m_len)++;
                root->x[root->m_len]=x+direct;
                root->y[root->m_len]=y-1;
                root->next_board[x+direct][y-1]=root->next_board[x][y];
                root->next_board[x][y]=0;
                root->jump=false;
                movelist->push_back(*root);
                split=true;
            }//recursive jump path
            else if(root->next_board[x+direct][y-1]!=0 && (2-((root->next_board[x+direct][y-1])%2))!=player){//check if left diagonal is opponent's
                if(x+2*direct<8 && x+2*direct>-1 && y>1 && root->next_board[x+2*direct][y-2]==0){
                    (root->m_len)++;
                    root->x[root->m_len]=x+2*direct;
                    root->y[root->m_len]=y-2;
                    root->next_board[x+2*direct][y-2]=root->next_board[x][y];
                    root->next_board[x][y]=0;
                    root->next_board[x+direct][y-1]=0;
                    root->jump=true;
                    split=true;
                    Getpath(movelist,root);
                }
            }
        }
        if(y<7){
            move *branch=root;
            if(split){
                branch=new move;
                *branch = copy;
            }
            if(branch->m_len==0&&branch->next_board[x+direct][y+1]==0){
                (branch->m_len)++;
                branch->x[branch->m_len]=x+direct;
                branch->y[branch->m_len]=y+1;
                branch->next_board[x+direct][y+1]=branch->next_board[x][y];
                branch->next_board[x][y]=0;
                branch->jump=false;
                movelist->push_back(*branch);
                split=true;
            }
            else if(branch->next_board[x+direct][y+1]!=0 && (2-((branch->next_board[x+direct][y+1])%2))!=player){//check if left diagonal is opponent's
                if(x+2*direct<8 && x+2*direct>-1 && y<6 && branch->next_board[x+2*direct][y+2]==0){
                    (branch->m_len)++;
                    branch->x[branch->m_len]=x+2*direct;
                    branch->y[branch->m_len]=y+2;
                    branch->next_board[x+2*direct][y+2]=branch->next_board[x][y];
                    branch->next_board[x][y]=0;
                    branch->next_board[x+direct][y+1]=0;
                    branch->jump=true;
                    split=true;
                    Getpath(movelist,branch);
            }
        }
    }
    }

    if(is_king && x-direct>-1 && x-direct <8){//same logic but king has 2 additional directions
        direct=-direct;
        if(x+direct>7 || x+direct<0)    return;
        if(y>0){
            move *branch=root;
            if(split){
                branch=new move;
                *branch = copy;
            }
            if(branch->m_len==0&&branch->next_board[x+direct][y-1]==0){
                (branch->m_len)++;
                branch->x[branch->m_len]=x+direct;
                branch->y[branch->m_len]=y-1;
                branch->next_board[x+direct][y-1]=branch->next_board[x][y];
                branch->next_board[x][y]=0;
                branch->jump=false;
                movelist->push_back(*branch);
                split=true;
            }
            else if(branch->next_board[x+direct][y-1]!=0 && (2-((branch->next_board[x+direct][y-1])%2))!=player){//check if left diagonal is opponent's
                if(x+2*direct<8 && x+2*direct>-1 && y>1 && branch->next_board[x+2*direct][y-2]==0){
                    (branch->m_len)++;
                    branch->x[branch->m_len]=x+2*direct;
                    branch->y[branch->m_len]=y-2;
                    branch->next_board[x+2*direct][y-2]=branch->next_board[x][y];
                    branch->next_board[x][y]=0;
                    branch->next_board[x+direct][y-1]=0;
                    branch->jump=true;
                    split=true;
                    Getpath(movelist,branch);
                }
            }
        }
        if(y<7){
            move *branch=root;
            if(split){
                branch=new move;
                *branch = copy;
            }
            if(branch->m_len==0&&branch->next_board[x+direct][y+1]==0){
                (branch->m_len)++;
                branch->x[branch->m_len]=x+direct;
                branch->y[branch->m_len]=y+1;
                branch->next_board[x+direct][y+1]=branch->next_board[x][y];
                branch->next_board[x][y]=0;
                branch->jump=false;
                movelist->push_back(*branch);
                split=true;
            }
            else if(branch->next_board[x+direct][y+1]!=0 && (2-((branch->next_board[x+direct][y+1])%2))!=player){//check if left diagonal is opponent's
                if(x+2*direct<8 && x+2*direct>-1 && y<6 && branch->next_board[x+2*direct][y+2]==0){
                    (branch->m_len)++;
                    branch->x[branch->m_len]=x+2*direct;
                    branch->y[branch->m_len]=y+2;
                    branch->next_board[x+2*direct][y+2]=branch->next_board[x][y];
                    branch->next_board[x][y]=0;
                    branch->next_board[x+direct][y+1]=0;
                    branch->jump=true;
                    split=true;
                    Getpath(movelist,branch);
                }
            }
        }
    }
    if(!split && root->m_len)   movelist->push_back(*root);
    return;
}
void checker::copy_board(uint8_t (*target)[8][8],const uint8_t source[8][8]){
    for(uint8_t i=0;i<8;i++){
        for(uint8_t j=0;j<8;j++){
            (*target)[i][j]=source[i][j];
        }
    }
}
void checker::move_print(vector<checker::move> *movelist){
    cerr<<"movelist:"<<endl;
    for(int i=0;i<movelist->size();i++){
        for(int j=0;j<=(*movelist)[i].m_len;j++){
            cerr<<"("<<(int)(*movelist)[i].y[j]<<", "<<(int)(*movelist)[i].x[j]<<") -> ";
        }
        cerr<<endl;
    }
    cerr<<endl<<endl;
}

checker::move checker::agent(vector<checker::move> *moves){//alpha-beta pruning
    const double stop_time=15e-6;
    checker::node tree[MAX_DEPTH];
    uint8_t max=5;
    uint8_t chosen=0;
    uint8_t choose_thresh=0;
    uint8_t choose=0; 
    int h_score;
    tree[0].movelist=*moves;
    tree[0].player=this->game.next_player;
    tree[0].list_len=tree[0].movelist.size();
    chrono::steady_clock::time_point start=chrono::steady_clock::now();
    chrono::duration<double> time_span;
    do{
        int8_t depth=0;
        tree[0].alpha=INT_MIN;
        tree[0].beta=INT_MAX;
        tree[0].val=(tree[0].player-1)?INT_MAX:INT_MIN;
        tree[0].index=0;
        if(tree[0].list_len<2)
            return (*moves)[0];
        while(1){
            if(tree[depth].index>=tree[depth].list_len || tree[depth].alpha>=tree[depth].beta){//end of branch condition
                depth--;
                if(depth<0) break;
                if(tree[depth].player-1){//player==2 -> min
                    if(tree[depth].val>tree[depth+1].val){
                        tree[depth].val=tree[depth+1].val;
                        if(!depth)
                            choose=tree[depth].index-1;
                    }
                    if(tree[depth].beta>tree[depth].val)
                        tree[depth].beta=tree[depth].val;
                }
                else{//player==1 -> max
                    if(tree[depth].val<tree[depth+1].val){
                        tree[depth].val=tree[depth+1].val;
                        if(!depth)
                            choose=tree[depth].index-1;
                    }
                    if(tree[depth].alpha<tree[depth].val)
                        tree[depth].alpha=tree[depth].val;
                }
            }
            else{
                if(depth+1<max){
                    depth++;
                    tree[depth].player=3-tree[depth-1].player;
                    tree[depth].movelist=checker::Getmove(tree[depth-1].movelist[tree[depth-1].index].next_board,tree[depth].player);
                    tree[depth-1].index++;
                    tree[depth].alpha=tree[depth-1].alpha;
                    tree[depth].beta=tree[depth-1].beta;
                    tree[depth].val=(tree[depth].player-1)?INT_MAX:INT_MIN;
                    tree[depth].index=0;
                    tree[depth].list_len=tree[depth].movelist.size();
                }
                else{
                    h_score=checker::heuristic(tree[depth].movelist[tree[depth].index].next_board);
                    tree[depth].index++;
                    if(tree[depth].player-1){//min
                        if(tree[depth].val>h_score){
                            tree[depth].val=h_score;
                            if(!depth)
                                choose=tree[depth].index-1;
                        }
                        if(tree[depth].beta>tree[depth].val)
                            tree[depth].beta=tree[depth].val;
                    }
                    else{//max
                        if(tree[depth].val<h_score){
                            tree[depth].val=h_score;
                            if(!depth)
                                choose=tree[depth].index-1;
                        }
                        if(tree[depth].alpha<tree[depth].val)
                            tree[depth].alpha=tree[depth].val;
                    }
                }
            }
            time_span = chrono::steady_clock::now() - start;
            if(this->game.timelim-time_span.count()<stop_time)
                goto time_over;
        }
        if(chosen==choose)
            choose_thresh++;
        else
            choose_thresh=0;
        chosen=choose; 
        cerr<<"Depth: "<<(int)max<<" Decision: "<<(int)chosen<<" Thresh: "<<(int)choose_thresh<<endl;
        if(choose_thresh==3)
            goto time_over;
        max++;
    }while(max<MAX_DEPTH&&time_span.count()<(this->game.timelim-time_span.count()));
time_over: 
    cout << "Agent Search took " << time_span.count() << " seconds."<<endl;
    return (*moves)[chosen];
}
int checker::heuristic(uint8_t board[8][8]){
    uint8_t p_count[2]={0};
    uint8_t player=0;
    int8_t coeff;
    int score=0;
    for (uint8_t i=0; i<8;i++){
        for (uint8_t j=0; j<8;j++){
            if(board[i][j]){
                player=board[i][j]%2;
                p_count[1-player]++;
                coeff=player*2-1; 
                //more pieces -> better,king -> better
                score+=coeff*(board[i][j]+player)*1000;
                //nearby pieces -> cannot be jumped -> better
                if(i>0){
                    if(j>0)
                        if(board[i-1][j-1]&&board[i-1][j-1]%2==player)
                                score+=coeff*200;
                    if(j<7)
                        if(board[i-1][j+1]&&board[i-1][j+1]%2==player)
                            score+=coeff*200;
                }
                if(i<7){
                    if(j>0)
                        if(board[i+1][j-1]&&board[i+1][j-1]%2==player)
                                score+=coeff*200;
                    if(j<7)
                        if(board[i+1][j+1]&&board[i+1][j+1]%2==player)
                            score+=coeff*200;
                }
                //try become king (necessary since change from regular->king is not handled by Getmove)
                if(board[i][j]<3){
                    int8_t goal=8;
                    if(player)
                        goal=-1;
                    score+=coeff*(2000/abs(i-goal));
                }
                //occupy center->better
                score-=coeff*(abs(2*j-7))*100;
                //edge better? not implemented
            }
        }
    }
    if(!p_count[0])
        return INT_MIN;//player 1 has none left
    if(!p_count[1])
        return INT_MAX;//player 2 has none left
    return rand()%50+score;//differentiate possible identical scores
}