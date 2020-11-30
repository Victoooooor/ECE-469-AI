#include <iostream>
#include <string> 
#include <vector>
#include <chrono>
#define MAX_DEPTH 23//should be enough
using namespace std;
class checker{ 
    public:
    typedef struct move{
        bool jump; 
        uint8_t m_len;
        uint8_t x[13];
        uint8_t y[13];
        uint8_t next_board[8][8];
        friend bool operator==(const checker::move& lhs, const checker::move& rhs)
	    {
		    return lhs.jump==rhs.jump;
	    }
    }move;
    typedef struct node{
        vector<checker::move> movelist;
        uint8_t list_len;
        uint8_t index;
        uint8_t player;
        int alpha,beta,val;
    }node;
    typedef struct{
        bool player[2];
        short timelim;
        uint8_t next_player;
    }setting;
    
    uint8_t board[8][8];
    setting game;
    checker(const uint8_t m_board[8][8]);
    vector<move> Getmove(uint8_t board[8][8],uint8_t player);
    move agent(vector<checker::move> *moves);
    
    void move_print(vector<move> *movelist);
    void copy_board(uint8_t (*target)[8][8],const uint8_t source[8][8]);
    int  heuristic(uint8_t board[8][8]);
    private:
    void Getpath(vector<move> *movelist,move *root);
    
};