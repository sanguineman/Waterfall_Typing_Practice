#include "ofApp.h"
_Water drop;

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(15); // 초마다 15 프레임
    idx = 0; //물이 떨어질 점을 선택할 때 관리해주는 인덱스 변수
    ofBackground(255, 255, 255); //배경 흰색으로 설정
    ofSetBackgroundAuto(false); //화면에 그린 그림이 사라지지 않도록 해줌.
    ofSetLineWidth(4); //라인 굵기 4
    draw_flag = 0; //드로우 플래그 0
    load_flag = 0; //파일 로드 플래그 0
    tmp_flag = 0; //물 떨어질 때 직선들을 여러번 계산하는 것을 방지하기 위한 변수 플래그 0
    s_key_flag = 0; //물을 그렸는지 확인을 위한 플래그 0
    dot_diameter = 20.0f; //물이 떨어지는 속도 20.0f 씩
    tmp_diameter = dot_diameter; //물이 직선을 따라움직일 때도 같은 속도로 20.0f
    writing_flag = 0; //문장을 작성 중인지 확인하는 플래그 0
    quotes_flag = 0; //문장을 불러왔는지 확인하는 플래그 0
    done_writing_flag = 0; //문장 작성을 다 끝마쳤는지 확인하는 플래그 0
    reach_flag = 0; //물이 땅에 닿았는지 확인하는 플래그 0
    quotes_index = 0; //문장들을 바꿔주는 플래그 0
    WRONG_LIMIT = 5; //문자 지우기 개수 제한 5번
    thefont.load("BMDOHYEON_otf.otf",25,true,true); //폰트 불러오기 25 크기로
    thefont.setLineHeight(15.0f); //라인 높이를 15.0f로 제한
    thefont.setLetterSpacing(1.035); //문자간 간격 1.035
    thenum.load("BMDOHYEON_otf.otf",15,true,true); //폰트 불러오기 15 크기로
    thenum.setLineHeight(15.0f); //라인 높이를 15.0f로 제한
    thenum.setLetterSpacing(1); //문자간 간격 1
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(127, 23, 31);  // 갈색으로 색깔 지정
    ofDrawRectangle(0, 0, 1024, 40); // 상단 직사각형을 그려준다. 글이 보여지는 배경
    ofDrawRectangle(0, 728, 1024, 40); // 하단 직사각형을 그려준다. 글이 작성되는 배경
    ofSetLineWidth(5); // 굵기 5
    if (draw_flag==1) { //드로우 플래그가 1이라면
        thenum.drawString("# of command uses left: " + ofToString(WRONG_LIMIT),50,100); //문자 지우기 남은 개수 출력
        for (int i = 0; i < num_of_line; i++) {
            ofDrawLine(lines[i].first.first, lines[i].first.second, lines[i].second.first, lines[i].second.second);
        } //선반들을 다 그려준다.
        ofSetColor(ofColor::black); // 검정색으로 색깔 설정
        for (int i = 0; i < num_of_dot; i++) { //점 그리기
            if (dots[i].first == dots[idx].first && dots[i].second == dots[idx].second) {
                ofSetColor(ofColor::red);
            } //선택한 지점은 빨간색으로 그려줌.
            else
                ofSetColor(ofColor::black); // 선택하지 않은 점은 검정색으로 그려줌
            ofDrawCircle(dots[i].first, dots[i].second, 10);
        }
        
    }
    if (water_flag == 1) { //워터 플래그가 1이면, 물이 떨어지기 시작한다.
        ofSetColor(ofColor::blue); // 파란색으로 설정
        ofSetLineWidth(2); //굵기 2
        initializeWaterLines(); //물 그려줌
    }
    if(quotes_flag == 1){ //쿼츠 플래그가 1이면, 문장을 상단에 그려준다.
        ofSetColor(ofColor::lightGrey); //색깔 밝은 회색으로 설정
        if(quotes_index < quotes.size()) //문장들의 개수보다 작을 때
            thefont.drawString(quotes[quotes_index],50,25); //문장 그리기
        else if(quotes_index == quotes.size()){ //더 이상 할 스테이지가 없으면 프로그램 종료
            resetVar();
            freeMemory();
            _Exit(1);
        }
    }
    thefont.drawString(typed,50,757); //계속해서 플레이어가 입력한 문자를 화면에 출력
    if(back_flag == 1){ //커맨드 키를 눌러서, 글자를 지우려 한다면,
        ofSetBackgroundAuto(false); //화면에 그리는 그림이 사라지지 않도록 ofSetBackgroundAuto false로 설정
        saveWaterLines(); //글자를 지울 때, 그 때까지의 물을 그려줌. 계속해서 이어지도록 하기 위해서.
    }
    if(reach_flag == 1){ //물이 땅에 닿았다면
        ofClear(ofColor::darkGrey); //배경을 깔끔하게 지우고, 짙은 회색으로 바꿈.
        draw_gameover(); //게임 오버 출력
        freeMemory(); //메모리 해제
        resetVar(); //변수 초기화
    }
    if(WRONG_LIMIT == 0){ //문자 지우기 개수를 모두 소진했다면
        ofClear(ofColor::darkGrey); //배경을 깔끔하게 지우고, 짙은 회색으로 바꿈.
        draw_gameover(); //게임 오버 출력
        freeMemory(); //메모리 해제
        resetVar(); //변수 초기화
    }
}

void ofApp::draw_gameover(){
    ofPushMatrix(); //현재의 좌표 시스템 저장
    string gameover = "GAMEOVER"; //게임 오버 문자열
    ofRectangle border = thefont.getStringBoundingBox(gameover, 0, 0); //게임 오버 문구의 커지고 작아짐의 경계선 역할을 하는 직사각형을 설정.
    ofTranslate(ofGetWidth() / 2 - 100 + border.width/2, ofGetHeight()/2 - 100 + border.height/2,0); //좌표를 옮겨줌
    ofScale(3.5 + cos(ofGetElapsedTimef()), 3.5 + cos(ofGetElapsedTimef()), 1.0); //x,y,z 좌표 비율 조정.
    ofSetColor(ofColor::lightPink); //밝은 핑크로 색깔 변경
    thefont.drawString(gameover,-border.width/2,border.height/2); //게임 오버 문구 출력
    ofPopMatrix(); //좌표 시스템 저장 해제
}

void ofApp::freeMemory(){ //메모리 해제
    dots.clear();
    vector <pair<int, int>>().swap(dots);
    lines.clear();
    vector <pair<pair<int, int>, pair<int, int>>>().swap(lines);
    candidates.clear();
    vector <pair<pair<int, int>, pair<int, int>>>().swap(candidates);
    quotes.clear();
    vector <string>().swap(quotes);
    savePoints.clear();
    vector<pair<int,int>>().swap(savePoints);
    cout << "Dynamically allocated memory has been freed." << endl;
}

void ofApp::resetVar(){ //변수 초기화
    draw_flag = 0;
    water_flag = 0;
    quotes_flag = 0;
    back_flag = 0;
    typed = "";
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if(!writing_flag){ //문장을 쓰고 있는 도중이 아닐 때
        if (key == 'q' || key == 'Q') { //큐 키가 눌렸다면
            // Reset flags
            if(done_writing_flag){ //문장을 다 썼을 경우에
                resetVar(); //변수 초기화
                freeMemory(); //메모리 해제

                _Exit(0); //프로그램 종료
            }
            
        }
        if (key == 'd' || key == 'D') { //디 키가 눌렸을 때
            if (!load_flag) return; //먼저 파일을 로드한 후 그려주기 위해서
            if(draw_flag) //두 번 그려주는 것을 방지
                return;
            draw_flag = 1; //처음 불렸을 때 드로우 플래그를 1로 바꿔준다.
        }
        if (key == 's' || key == 'S') { //에스 키가 눌렸을 떄
            if(draw_flag && !s_key_flag){ //드로우 플래그가 1이고, s키가 한번도 눌린적이 없을 때
                ofSetBackgroundAuto(false); //지금부터 그리는 그림 계속 화면에 남기기
                drop._x = dots[idx].first; //물이 떨어지는 좌표 저장
                drop._y = dots[idx].second; //물이 떨어지는 좌표 저장
                water_flag = 1; //물이 떨어지는 중이라는 water_flag 1로 바꿔준다.
                dot_diameter = 20.0f;
                tmp_diameter = 20.0f;
                s_key_flag = 1; //s_key_flag를 1로 바꿔준다.
                writing_flag = 1; //문장을 작성하는 중이라는 writing_flag 1로 바꿔준다.
                done_writing_flag = 0;
                back_flag = 0;
                savePoints.clear();
                savePoints.push_back({drop._x,drop._y}); //물의 좌표를 저장할 수 있도록 벡터에 추가
            }
        }
        if (key == 'e' || key == 'E') { //이 키가 눌렸다면
            if(draw_flag && done_writing_flag){ //드로우 플래그가 1이고, 문장 작성이 끝났다면
                ofSetBackgroundAuto(true); //화면을 한번 지워준다.
                draw_flag = 1; //드로우 플래그 1로 다시 설정하면, 화면에 점과 선 다시 그려진다.
                dot_diameter = 20.0f;
                tmp_diameter = 20.0f;
                s_key_flag = 0; //s_key_flag 0으로 바꿔주어서 게임 시작 가능토록 한다.
            }
        }
        
        if (key == 'r' || key == 'R'){ //알 키가 눌렸다면
            if(!reach_flag){ //물이 땅에 닿지 않았다면
                readFile(); // 문장 파일 불러오기
                quotes_flag = 1; //쿼츠 플래그 1로 변경
            }
            
        }
    }
    else{
        if((key >= 'a' && key <= 'z') || key == ' '){ //a부터 z 사이의 문자 혹은 공백문자가 입력되었을 때
            typed.append(1,(char)key); //문자열에 그 문자 추가
        }
        if(key == OF_KEY_RETURN){ //엔터키가 눌렸을 때
            ofSetBackgroundAuto(true); //화면을 한번 지워줌.
            done_writing_flag = 1; //문장이 다 작성되었다는 done_writing_flag 1로 변경
            quotes_index++; //다음 문장 출력을 위해 인덱스 1 증가
            writing_flag = 0;
            water_flag = 0;
            back_flag = 0;
            WRONG_LIMIT = 5;
            typed = "";
            // 모두 변수 초기화
        }
        if(key == OF_KEY_COMMAND && typed.size() > 0){ //커맨드 버튼을 눌렀을 때, 문자열 길이가 0 이상일 떄
            ofSetBackgroundAuto(true);//화면을 한번 지워줌.
            string st = typed.substr(0,typed.size()-1); //문자열을 지운 문자 그 이전까지 자르기
            typed[0] = '\0';
            typed = st;
            //문자 초기화 후 다시 할당
            WRONG_LIMIT--; //문자 지우기 횟수 1 감소
            back_flag = 1; // 문자를 지웠다는 플래그 1로 변경
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    if(!writing_flag){
        if (key == 'l' || key == 'L') {
            // 엘 키가 눌렸을 때, 워터폴 정보를 담은 파일을 선택,
            ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a only txt for Waterfall");

            // 사용자가 파일을 선택했는지 확인
            if (openFileResult.bSuccess) {
                ofLogVerbose("User selected a file");

                //파일 정보를 읽어들임 및 load_flag를 1로 바꾸어주어 파일을 열었다라는 것을 표시.
                processOpenFileSelection(openFileResult);
                load_flag = 1;
            }
        }

        //워터폴 실습 때 구현한 사항, 오른쪽 키를 누르면, 물이 내려올 점을 현재 점에서 오른쪽 점으로 옮기고, 왼쪽을 누르면 그 반대로 옮김.
        if (key == OF_KEY_RIGHT) {
            if (draw_flag != 1)
                return;
            // 맨 오른쪽 점에서 오른쪽을 누르면, 다시 맨 왼쪽 점으로 이동해야 함.
            if (idx == num_of_dot - 1) {
                idx = -1;
            }
            idx++;
            //떨어질 점의 좌표를 초기화 시켜주는 것.
            drop._x = dots[idx].first;
            drop._y = dots[idx].second;
            originY = (float)dots[idx].second;
            cout << "Selcted Dot Coordinate is (" << dots[idx].first << ", " << dots[idx].second << ")" << endl;
        }
        if (key == OF_KEY_LEFT) {
            if (draw_flag != 1)
                return;
            // 맨 왼쪽 점에서 왼쪽을 누르면, 다시 맨 오른쪽 점으로 이동해야함.
            if (idx == 0) {
                idx = num_of_dot;
            }
            idx--;
            //떨어질 점의 좌표를 초기화 시켜주는 것.
            drop._x = dots[idx].first;
            drop._y = dots[idx].second;
            originY = (float)dots[idx].second;
            cout << "Selcted Dot Coordinate is (" << dots[idx].first << ", " << dots[idx].second << ")" << endl;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//타자 연습을 위한 문장들이 저장되어 있는 파일을 불러오고, 읽어들이는 함수.
bool ofApp::readFile(){
    //문장이 담긴 파일을 선택,
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select only quotes file");
        
        //사용자가 파일을 선택했는지,
        if(openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            //파일을 읽고, 자료구조에 맞게 저장하는 processSaveQuotes를 호출.
            processSaveQuotes(openFileResult);
            return true;
        }
}

//파일에 있는 문장들을 벡터 자료구조에 저장하는 함수
void ofApp::processSaveQuotes(ofFileDialogResult openFileResult){
    string file_loc = openFileResult.getPath();
    ifstream inputf;
    //파일을 열어서
    inputf.open(file_loc);
    string line;
    //열기에 실패했다면 에러 메세지
    if(!inputf.is_open()){
        cout << "fail to open the file" << '\n';
        return 0;
    }
    //문장 한 줄씩 string을 담는 벡터 자료구조에 저장한다.
    while(getline(inputf,line)){
        quotes.push_back(line);
    }
}

//워터폴 실습 파일에 이미 구현되어 있었던 함수, 파일을 열고, 자료구조에 알맞게 저장하는 함수.
void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {

    string fileName = openFileResult.getName();
    ofFile file(fileName);
    //파일이 없으면 에러 메세지 출력 후 리턴.
    if (!file.exists()) {
        cout << "Target file does not exists." << endl;
        return;
    }

    else cout << "We found the target file." << endl;

    ofBuffer buffer(file);

    int input_type = 0;

    int cnt = 0;
    // 라인 별로 파일을 읽어들임.
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;

        // 공백을 기준으로 라인을 단어 단위로 파싱
        vector<string> words = ofSplitString(line, " ");

        if (words.size() == 1) { //파싱된 단어가 1개면
            if (input_type == 0) { // 라인들이 몇 개인지를 나타내는 것
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
            }
            else { // 점들의 개수가 몇 개인지를 나타내는 것
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;
            }
        }

        else if (words.size() >= 2) { //파싱된 단어가 둘 이상이면
            int x1, y1, x2, y2;
            if (input_type == 0) { //라인에 대한 정보를 저장
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                x2 = atoi(words[2].c_str());
                y2 = atoi(words[3].c_str());
                // 입력받은 좌표가 범위 밖을 벗어나면, 메모리 초기화 후 프로그램 종료.
                if (((x1 < 0 || x1 > 1024) || (y1 < 40 || y1 > 728)) || (x2 < 0 || x2 > 1024) || (y2 < 40 || y2 > 728)) {
                    dots.clear();
                    vector <pair<int, int>>().swap(dots);
                    lines.clear();
                    vector <pair<pair<int, int>, pair<int, int>>>().swap(lines);
                    candidates.clear();
                    vector <pair<pair<int, int>, pair<int, int>>>().swap(candidates);
                    cout << "the coordinates out of bound!" << endl;
                    _Exit(0);
                }
                // 좌표 정보를 라인 정보를 담는 pair<pair<int,int>,pair<int,int>> 벡터 자료구조에 저장
                lines.push_back({ {x1,y1},{x2,y2} });
                cnt++;
                //라인에 대한 정보를 다 받았다면, 이제 점들에 대한 정보를 받을 수 있도록 input_type 을 1로 변경.
                if (cnt == num_of_line)
                    input_type = 1;
            }
            else { // 점들에 대한 정보를 저장
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                // 점 역시 범위를 벗어났을 경우 메모리 초기화 후 프로그램 종료
                if ((x1 < 0 || x1 > 1024) || (y1 < 40 || y1 > 728)) {
                    dots.clear();
                    vector <pair<int, int>>().swap(dots);
                    lines.clear();
                    vector <pair<pair<int, int>, pair<int, int>>>().swap(lines);
                    candidates.clear();
                    vector <pair<pair<int, int>, pair<int, int>>>().swap(candidates);
                    cout << "the coordinates out of bound!" << endl;
                    _Exit(0);
                }
                //점들에 대한 정보를 저장할 수 있는 pair<int,int> 형 벡터 자료구조에 저장
                dots.push_back({ x1,y1 });
            }
        }
    }
    //점들을 x 좌표가 작은 순서대로 정렬 및 시작점 초기화
    sort(dots.begin(), dots.end());
    drop._x = dots[idx].first;
    drop._y = dots[idx].second;
    originY = (float)dots[idx].second;
}

//커맨드가 눌렸을 때까지(즉, 문자를 하나 지울 때)에 대한 물을 다시 그려주는 함수
void ofApp::saveWaterLines(){
    pair <int, int> p;
    ofSetColor(ofColor::blue);
    //x좌표, y좌표를 각각 p.first 와 p.second에 저장.
    p.first = savePoints[0].first;
    p.second = savePoints[0].second;
    for(int i = 1 ; i < savePoints.size() - 1; i++){
        //현재 p.first와 p.second에 저장되어 있는 점 좌표와 savePoints[i]의 점 좌표를 이어서 그려준다.
        ofDrawLine(p.first,p.second,savePoints[i].first,savePoints[i].second);
        //그려준 후에는 savePoints[i]의 점 좌표가 p의 점 좌표가 되도록 할당해준다. 이 과정을 반복하면 모든 점들을 선으로 이어 그릴 수 있음.
        p.first = savePoints[i].first;
        p.second = savePoints[i].second;
    }
}

//워터폴 실습 때 구현한 물 떨어지는 것을 그려주는 함수
void ofApp::initializeWaterLines() {
    float dist;
    //탈출 조건, 물이 바닥에 닿았다면, 물이 닿았다는 정보를 담는 reach_flag를 1로 바꾸고 리턴.
    if (originY + dot_diameter > 728) {
        ofDrawLine(drop._x, drop._y - 3, drop._x, 728);
        reach_flag = 1;
        return;
    }
    //함수가 불릴 때마다 매번 좌표를 갱신할 필요는 없고, 물이 직선에 닿았을 때나 물이 직선 끝에서 다시 아래로 떨어질 때 한번만 계산해주면 되기 때문에
    //tmp_flag를 통해 제어함
    if (!tmp_flag) {
        for (int i = 0; i < num_of_line; i++) {
            //똑같은 직선을 선택하면 안되기 때문에, 똑같은 직선을 만난다면 continue
            if (lines[i].first.first == drop._x || lines[i].second.first == drop._x)
                continue;
            //현재의 x좌표가 다음 떨어질 직선의 범위 안에 들어가있어야 후보군으로 선정될 수 있다.
            if (lines[i].first.first <= drop._x && drop._x <= lines[i].second.first) {
                candidates.push_back(lines[i]);
            }
        }
        drop.mxY = 728.0;
        //뽑힌 후보 직선들을 또 순회하면서, 이 직선들 중 제일 먼저 물이 만날 직선을 선택해야함.
        for (vector <pair<pair<int, int>, pair<int, int>>>::iterator iter = candidates.begin(); iter != candidates.end(); iter++) {
            float result;
            float x1 = (float)iter->first.first;
            float x2 = (float)iter->second.first;
            float y1 = (float)iter->first.second;
            float y2 = (float)iter->second.second;
            //직선의 방정식을 만들어, y좌표를 구한다.
            result = (y2 - y1) / (x2 - x1) * (drop._x - x1) + y1;
            //y좌표의 값이 낮을 수록 오픈프레임웍스의 좌표 시스템 상으로는 가장 위이므로, 계속해서 갱신시켜준다.
            if (result < drop.mxY && result >= drop._y) {
                drop.mxY = result;
                drop._line.first.first = x1;
                drop._line.first.second = y1;
                drop._line.second.first = x2;
                drop._line.second.second = y2;
            }
        }
        //후보를 고르고 나서는 후보군 벡터 자료구조를 비워준다.
        candidates.clear();
    }
    //물이 닿을 지점의 y좌표와 현재 물이 떨어지고 있는 위치의 차이를 dist에 저장.
    dist = originY + dot_diameter - drop.mxY;
    //이 차이가 0보다 크게 된다면, 직선에 물이 닿았다는 뜻
    if (dist > 0) {
        tmp_flag = 1;
        //문자를 삭제하겠다는 커맨드 키가 입력되었다면, 즉 back_flag가 1이라면
        if(back_flag == 1){
            //이 때의 좌표를 savePoints 벡터 자료구조에 저장한다.
            savePoints.push_back({drop._x,originY+ dot_diameter - 3});
        }
        //savePoints 벡터 자료구조에 현재 직선과 물이 맞닿은 점의 좌표를 저장한다.
        savePoints.push_back({drop._x,originY+ dot_diameter - 3});
        //물을 그려주고
        ofDrawLine(drop._x, drop._y - 3, drop._x, originY + dot_diameter - 3);
        //직선에 맞닿았을 때 물이 흐르는 것을 따로 그려주는 followline 함수를 호출한다.
        followline(&tmp_diameter);
    }
    else {
        //문자를 삭제하겠다는 커맨드 키가 입력되었다면, 즉 back_flag가 1이라면
        if(back_flag == 1){
            //이 때의 좌표를 savePoints 벡터 자료구조에 저장한다.
            savePoints.push_back({drop._x,originY+ dot_diameter - 3});
        }
        //물이 떨어지는 것을 그려준다.
        ofDrawLine(drop._x, drop._y-3, drop._x, originY + dot_diameter-3);
        //물이 떨어지는 것을 구현하기 위해서 y 좌표의 값을 일정하게 증가시켜주는 dot_diameter를 3.0만큼 증가시킨다.
        dot_diameter += 3.0;
        tmp_diameter += 3.0;
    }

}

//물이 직선을 따라 내려갈 때, 물을 그려주는 함수
void ofApp::followline(float* diameter) {
    //물이 직선을 따라 흐를 때 dot_diameter를 증가시키게 되면, 계속해서 증가해서 initializeWaterLines 함수 if(dist>0) 문 조건에서 빠져나오게 된다.
    //그래서 tmp_diameter를 따로 만들어 똑같이 증가시키면서 followline이 불렸을 때는 tmp_diameter만 넘겨주어 증가시킨다. dot_diameter 값에 영향을 미치지 않도록 말이다.

    //직선이 우상향하는 직선인지, 우하향하는 직선인지 파악하기 위해 양쪽 끝의 y좌표를 비교해서 물이 향할 좌표를 saveX와 saveY에 저장.
    if (drop._line.first.second < drop._line.second.second) {
        saveX = drop._line.second.first;
        saveY = drop._line.second.second;
    }
    else {
        saveX = drop._line.first.first;
        saveY = drop._line.first.second;
    }
    // 물이 향하는 선분 위 끝지점의 좌표 saveX, saveY
    // 물이 선분 위로 떨어졌을 때의 좌표 drop._x
    float dist;
    //계속해서 변하는 y좌표에 따라 x 좌표를 구하기 위해 직선의 방정식을 x에 대한 식으로 정리하였다.
    float folx = (originY + *diameter - (float)drop._line.first.second) * (((float)drop._line.second.first - (float)drop._line.first.first) / ((float)drop._line.second.second - (float)drop._line.first.second)) + (float)drop._line.first.first;
    //이 직선 위에서 현재 x 좌표와 물이 향하는 선분 위 끝지점의 x 좌표의 차이를 저장하는 dist 변수
    dist = folx - (float)saveX;
    //물이 선분 끝에 도달했을 때, 즉 현재 y 좌표와 선분 끝의 y 좌표의 차이가 0 보다 클 때
    if (originY + *diameter - saveY > 0) {
        tmp_flag = 0;
        //선분 끝까지 물을 그려주고, 미세하게 짤리는 부분을 메꾸기 위해서
        ofDrawLine(drop._x, drop.mxY-3, saveX, saveY-3);
        //dot_diameter도 현재의 tmp_diameter 값으로 업데이트해주어 initializeWaterLines 함수로 돌아갔을 때 update 한 값으로 진행될 수 있도록 함.
        dot_diameter = *diameter;
        //문자를 삭제하겠다는 커맨드 키가 입력되었다면, 즉 back_flag가 1이라면
        if(back_flag == 1){
            //이 때의 좌표를 savePoints 벡터 자료구조에 저장한다.
            savePoints.push_back({saveX,saveY-3});
        }
        //savePoints 벡터 자료구조에 현재 직선과 물이 맞닿은 점의 좌표를 저장한다.
        savePoints.push_back({saveX,saveY-3});
        //initializeWaterLines로 돌아가서 다시 직선 후보군들을 골라야 하므로 좌표 초기화
        drop._x = (float)saveX;
        drop._y = (float)saveY;
        drop.mxY = 728.0;
    }
    else {
        //문자를 삭제하겠다는 커맨드 키가 입력되었다면, 즉 back_flag가 1이라면
        if(back_flag == 1){
            //이 때의 좌표를 savePoints 벡터 자료구조에 저장한다.
            savePoints.push_back({folx,originY+ *diameter - 3});
        }
        //물이 직선을 따라 흘러 내려가는 것을 그려준다.
        ofDrawLine(drop._x, drop.mxY-3, folx, originY + *diameter-3);
        //tmp_diameter 3.0만큼 증가시킴
        *diameter = *diameter + 3.0;
    }
}
