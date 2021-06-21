#pragma once

#include "ofMain.h"
#include <vector>

class _Water {
    public:
        float mxY;
        float _x;
        float _y;
        pair <pair<int,int>,pair<int,int>> _line;
};

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        bool readFile();
        void processSaveQuotes(ofFileDialogResult openFileResult);
        void saveWaterLines();
        void draw_gameover();
        void resetVar();
        void freeMemory();
    /* 워터폴 실습 변수들 */
    // 플래그 변수들
    int draw_flag;
    int load_flag;
    int s_key_flag;
    int tmp_flag;
    int idx;
    // 점, 선 관련 변수들
    int num_of_line, num_of_dot;
    int saveX;
    int saveY;
    float originY;
    float dot_diameter;
    float tmp_diameter;
    vector <pair<int,int>> dots;
    vector <pair<pair<int,int>,pair<int,int>>> lines;
    vector <pair<pair<int, int>, pair<int, int>>> candidates;
    /* 워터폴 실습 변수들 */
    
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void initializeWaterLines();
    void followline(float * diameter);
    /* 컴실 프로젝트 추가 변수 */
    int quotes_flag;
    int writing_flag;
    int water_flag;
    int done_writing_flag;
    int reach_flag;
    int back_flag;
    int WRONG_LIMIT;
    int quotes_index;
    ofTrueTypeFont thefont;
    ofTrueTypeFont thenum;
    vector <string> quotes;
    string typed;
    vector <pair<int,int>> savePoints;
    
};
