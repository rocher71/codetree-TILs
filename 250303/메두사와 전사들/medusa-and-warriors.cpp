#include <iostream>
#include <deque>
#include <queue>
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
using namespace std;

typedef pair<int, int> pii;

int n, m, medusaY, medusaX, parkY, parkX, moveCnt, stoneCnt, defeatCnt, leftSoldierCnt;
int soldiers[52][52], sight[52][52];
bool isRoad[52][52], visit[52][52];
deque<pair<int, int>> soldierLocs;
int dy[4] = {-1, 1, 0, 0}, dx[4] = {0, 0, -1, 1};

bool isInRange(int y, int x){
    if(y < 0 || x < 0 || y >= n || y >= n) return false;
    return true;
}

void hide(int y, int x, int direction){
    // 현재 전사 위치가

    // 메두사와 같은 가로줄인 경우
    if(medusaY == y){
        if(direction == RIGHT) for(int i = x + 1; i < n; i++) sight[y][i] = 2;
        else if(direction == LEFT) for(int i = x-1; i >= 0; i--) sight[y][i] = 2;
        return;
    }

    // 메두사와 같은 세로줄인 경우
    if(medusaX == x){
        if(direction == UP) for(int i = y - 1; i >= 0; i--) sight[i][x] = 2;
        else if(direction == DOWN) for(int i = y + 1; i < n; i++) sight[i][x] = 2;
        return;
    }

    if(direction == UP && medusaX > x) {
        for (int i = x; i >= 0; i--) {
            int yStart = x - i <= 1 ? y - 1 : y - (x - i);
            for (int j = yStart; j >= 0; j--) sight[j][i] = 2;
        }
        return;
    }
    if(direction == UP && medusaX < x){
        for(int i = x; i < n; i++){
            int yStart = i - x <= 1 ? y - 1 : y - (i - x);
            for(int j = yStart; j >= 0; j--) sight[j][i] = 2;
        }
        return;
    }

    if(direction == DOWN && medusaX > x){
        for(int i = x; i >= 0; i--){
            int yStart = x - i <= 1 ? y + 1 : y + (x-i);
            for(int j = yStart; j < n; j++) sight[j][i] = 2;
        }
        return;
    }
    if(direction == DOWN && medusaX < x){
        for(int i = x; i < n; i++){
            int yStart = i - x <= 1 ? y + 1 : y + (i - x);
            for(int j = yStart; j < n ; j++) sight[j][i] = 2;
        }
        return;
    }

    if(direction == LEFT && medusaY > y){
        for(int i = y; i < n; i++){
            int xStart = i - y <= 1 ? x - 1 : x - (i - y);
            for(int j = xStart; j >= 0; j--) sight[i][j] = 2;
        }
        return;
    }
    if(direction == LEFT && medusaY < y){
        for(int i = y; i >= 0; i--){
            int xStart = y - i <= 1 ? x - 1 : x - (y - i);
            for(int j = xStart; j >= 0; j--) sight[i][j] = 2;
        }
        return;
    }

    if(direction == RIGHT && medusaY > y){
        for(int i = y; i < n; i++){
            int xStart = i - y <= 1 ? x + 1 : x - (i - y);
            for(int j = xStart; j < n; j++) sight[i][j] = 2;
        }
        return;
    }
    if(direction == RIGHT && medusaY < y){
        for(int i = y; i >= 0; i--){
            int xStart = y - i <= 1 ? x + 1 : x + (y - i);
            for(int j = xStart; j < n; j++) sight[i][j] = 2;
        }
        return;
    }

}

// direction에 따라 돌로 만들 수 있는 병사 수 return
int lookDirection(int direction){
    fill(&sight[0][0], &sight[n-1][n], 0);
    int currStoneCnt = 0;

    for(int i = 1; i<= n; i++){
        for(int j = i * -1; j <= i; j++){
            int nextY = direction == 0 || direction == 1 ? medusaY + i * dy[direction] : medusaY + j;
            int nextX = direction == 2 || direction == 3 ? medusaX + i * dx[direction] : medusaX + j;

            if(!isInRange(nextY, nextX)) continue;
            if(sight[nextY][nextX] == 2) continue; // 이미 가려진 칸이면 Pass

            sight[nextY][nextX] = 1;
            if(soldiers[nextY][nextX] == 0) continue;


            // 가려지지 않았고, 전사가 있는 경우 -> 돌 됨
            currStoneCnt += soldiers[nextY][nextX];
            hide(nextY, nextX, direction);
        }
    }
    return currStoneCnt;

    // 상하 방향
//    if(direction == 2 || direction == 3){
//        for(int i = 1; i <= n; i++){
//            for(int j = i * -1; j <= i; j++){
//                int nextY = medusaY + i * dy[direction];
//                int nextX = medusaX + j;
//
//                if(!isInRange(nextY, nextX)) continue;
//                if(sight[nextY][nextX] == 2) continue; // 이미 가려진 곳이면 Pass
//
//                sight[nextY][nextX] = 1; // 메두사 시야 표시
//                if(soldiers[nextY][nextX] == 0) continue;
//
//                // 가려지지 않았고, 전사가 있는 경우 -> 돌 됨
//                 currStoneCnt += soldiers[nextY][nextX];
//                hide(nextY, nextX, direction);
//            }
//        }
//        return currStoneCnt;
//    }
//
//    // 좌우 방향
//    for(int j = 1; j <= n; j++){
//        for(int i = j * -1; i <= j; i++){
//            int nextY = medusaY + i;
//            int nextX = medusaX + i * direction;
//
//            if(!isInRange(nextY, nextX)) continue;
//            if(sight[nextY][nextX] == 2) continue;
//
//            sight[nextY][nextX] = 1;
//            if(soldiers[nextY][nextX] == 0) continue;
//
//            // 가려지지 않았고, 전사가 있는 경우 -> 돌 됨
//            currStoneCnt += soldiers[nextY][nextX];
//            hide(nextY, nextX, direction);
//        }
//    }
//    return currStoneCnt;
}

pair<int, int> findMedusaRoute(){
    fill(&visit[0][0], &visit[n-1][n], false);
    queue<pair<pii, pii>> q; // {{nextY, nextX}, {firstY, firstX}}
    q.push({{medusaY, medusaX}, {-1, -1}});
    visit[medusaY][medusaX] = true;
    //cout << "=======bfs start=========\n시작 좌표 : (" << medusaY << ", " << medusaX << ")\n";

    while(!q.empty()){
        int currY = q.front().first.first;
        int currX = q.front().first.second;
        int firstY = q.front().second.first;
        int firstX = q.front().second.second;
        q.pop();
//        cout << currY << ", " << currX << "\n";

        // 길 찾은 경우 첫 좌표 반환
        if(currY == parkY && currX == parkX){
//            cout << "return 첫 좌표 : (" << firstY << ", " << firstX << "\n";
            return {firstY, firstX};
        }

        for(int i = 0; i< 4; i++){
            int nextY = currY + dy[i];
            int nextX = currX + dx[i];

            if(nextY < 0 || nextX < 0 || nextY >= n || nextX >= n) continue;
            if(!isRoad[nextY][nextX] || visit[nextY][nextX]) continue;

            visit[nextY][nextX] = true;
            if(firstY == -1){
                q.push({{nextY, nextX}, {nextY, nextX}});
                continue;
            }

            q.push({{nextY, nextX}, {firstY, firstX}});
        }
    }
    return {-1, -1};
}

pair<int, int> findSoldierRoute(int fromY, int fromX, bool isFirst){
    fill(&visit[0][0], &visit[n-1][n], false);
    queue<pair<pii, pii>> q; // {{fromY, fromX}, {firstY, firstX}}
    q.push({{fromY, fromX}, {-1, -1}});
    visit[fromY][fromX] = true;
    //cout << "=======bfs start=========\n시작 좌표 : (" << fromY << ", " << fromX << ")\n";

    while(!q.empty()){
        int currY = q.front().first.first;
        int currX = q.front().first.second;
        int firstY = q.front().second.first;
        int firstX = q.front().second.second;
        q.pop();
//        cout << currY << ", " << currX << "\n";

        // 길 찾은 경우 첫 좌표 반환
        if(currY == medusaY && currX == medusaX){
//            cout << "return 첫 좌표 : (" << firstY << ", " << firstX << "\n";
            return {firstY, firstX};
        }

        int from = isFirst ? 0 : 2;
        int till = isFirst ? 3 : 5;
        for(int i = from; i<= till; i++){
            int nextY = currY + dy[i % 4];
            int nextX = currX + dx[i % 4];

            if(nextY < 0 || nextX < 0 || nextY >= n || nextX >= n) continue;
            if(visit[nextY][nextX]) continue;
//            if(currY == fromY && currX == fromX && sight[nextY][nextX] == 1) continue;

            visit[nextY][nextX] = true;
            if(firstY == -1){
                q.push({{nextY, nextX}, {nextY, nextX}});
                continue;
            }

            q.push({{nextY, nextX}, {firstY, firstX}});
        }
    }
    return {-1, -1};
}

int main(){
    cin >> n >> m;
    cin >> medusaY >> medusaX >> parkY >> parkX;
    leftSoldierCnt = m;

    for(int i = 0; i< m; i++){
        int y, x;
        cin >> y >> x;
        soldiers[y][x]++;
    }

    for(int i  = 0 ; i< n; i++){
        for(int j =0; j < n; j++){
            int a;
            cin >> a;
            if(a == 0) isRoad[i][j] = true;
            else isRoad[i][j] = false;
        }
    }

    // 메두사가 공원에 도착할 때 까지 반복
    while(!(medusaY == parkY && medusaX == parkX)){
        moveCnt = 0;
        stoneCnt = 0;
        defeatCnt = 0;

        // 1. 메두사 이동
        pair<int, int> nextYX = findMedusaRoute();
        // 메두사가 공원까지 갈 수 없다면 -1 출력 후 종료
        if(nextYX.first == -1 || nextYX.second == -1){
            cout << "-1\n";
            return 0;
        }
        // 공원 도착 시 0 출력 후 종료
        if(nextYX.first == parkY && nextYX.second == parkX){
            cout << "0\n";
            return 0;
        }
        medusaY = nextYX.first;
        medusaX = nextYX.second;

        if(soldiers[medusaY][medusaX] != 0){
            leftSoldierCnt -= soldiers[medusaY][medusaX];
            soldiers[medusaY][medusaX] = 0;
        }

        // 전사가 다 죽은 경우 아래 코드 스킵
        if(leftSoldierCnt == 0){
            cout << "0 0 0\n";
            continue;
        }

        // 2. 메두사의 시선
        int maxDirection = -1, maxStoneCnt = 0;
        for(int i = 0; i< 4; i++){
            int currStoneCnt = lookDirection(i);
            if(currStoneCnt <= maxStoneCnt) continue; // 상하좌우 순이므로 같아도 continue
            maxDirection = i;
            maxStoneCnt = currStoneCnt;
        }

        // 최대 방향으로 sight 배열 설정
        stoneCnt =  lookDirection(maxDirection);
//        cout << "바라보는 방향 : " << maxDirection << "\n";
//        for(int i = 0; i < n;i++){
//            for(int j = 0; j < n; j++){
//                if(i == medusaY && j == medusaX){
//                    cout << "X ";
//                    continue;
//                }
//                cout << sight[i][j] << " ";
//            }
//            cout << "\n";
//        }
//        cout << "\n";

        for(int i = 0; i < n;i++){
            for(int j = 0; j < n; j++) {
                if(soldiers[i][j] != 0 && sight[i][j] != 1) soldierLocs.push_back({i, j});
            }
        }

        // 3. 전사 이동
        int soldierCnt = soldierLocs.size();
        // 첫번째 이동
        for(int i = 0 ; i< soldierCnt; i++){
            int currY = soldierLocs.front().first;
            int currX = soldierLocs.front().second;

            auto [nextY, nextX] = findSoldierRoute(currY, currX, true);
            soldierLocs.pop_front();

            // 최단거리가 메두사의 시야일 경우
            if(sight[nextY][nextX] == 1){
                soldierLocs.push_back({currY, currX});
                continue;
            }

            moveCnt += soldiers[currY][currX];

            // 가는 도중 메두사를 만난 경우 잡아먹힘
            if(nextY == medusaY && nextX == medusaX){
                defeatCnt += soldiers[currY][currX];
                leftSoldierCnt -= soldiers[currY][currX];
                soldiers[currY][currX] = 0;
                continue;
            }

            // 다시 덱에 넣어줌
            soldierLocs.push_back({nextY, nextX});
            soldiers[nextY][nextX] = soldiers[currY][currX];
            soldiers[currY][currX] = 0;

        }

//        cout << "첫 번째 전사들 이동 결과\n";
//        for(int i = 0; i < n;i++){
//            for(int j = 0; j < n; j++){
//                cout << soldiers[i][j] << " ";
//            }
//            cout << "\n";
//        }
//        cout << "\n";

        // 두번째 이동
        while(!soldierLocs.empty()){
            int currY = soldierLocs.front().first;
            int currX = soldierLocs.front().second;

            auto [nextY, nextX] = findSoldierRoute(currY, currX, false);
            soldierLocs.pop_front();

            // 최단거리가 메두사의 시야일 경우
            if(sight[nextY][nextX] == 1) continue;

            moveCnt += soldiers[currY][currX];
            soldiers[nextY][nextX] = soldiers[currY][currX];
            soldiers[currY][currX] = 0;

            // 두번 이동 후 메두사를 못 만난 경우 넘김
            if(nextY != medusaY || nextX != medusaX) continue;

            // 가는 도중 메두사를 만난 경우 값 저장
            defeatCnt += soldiers[nextY][nextX];
            leftSoldierCnt -= soldiers[nextY][nextX];
            soldiers[nextY][nextX] = 0;
        }
//        cout << "두 번째 전사들 이동 결과\n";
//        for(int i = 0; i < n;i++){
//            for(int j = 0; j < n; j++){
//                cout << soldiers[i][j] << " ";
//            }
//            cout << "\n";
//        }
//        cout << "\n";

        cout << moveCnt << " " << stoneCnt << " " << defeatCnt << "\n";

    }
}