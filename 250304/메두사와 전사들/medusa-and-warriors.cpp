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
deque<pair<pii, int>> soldierLocs;
deque<pii> medusaRoute;
// 매 턴마다 {{y, x}, 현 좌표에 있는 병사 수} 미리 돌며 넣어주기.
// 병사가 이동하며 좌표가 겹칠 수 있기 때문에 cnt 값 따로 관리 해줘야함
int dy[4] = { -1, 1, 0, 0 }, dx[4] = { 0, 0, -1, 1 };

bool isInRange(int y, int x) {
	if (y < 0 || x < 0 || y >= n || x >= n) return false;
	return true;
}

void hide(int y, int x, int direction) {
	// 현재 전사 위치가

	// 메두사와 같은 가로줄인 경우
	if (medusaY == y) {
		if (direction == RIGHT) for (int i = x + 1; i < n; i++) sight[y][i] = 2;
		else if (direction == LEFT) for (int i = x - 1; i >= 0; i--) sight[y][i] = 2;
		return;
	}

	// 메두사와 같은 세로줄인 경우
	if (medusaX == x) {
		if (direction == UP) for (int i = y - 1; i >= 0; i--) sight[i][x] = 2;
		else if (direction == DOWN) for (int i = y + 1; i < n; i++) sight[i][x] = 2;
		return;
	}

	if (direction == UP && medusaX > x) {
		for (int i = x; i >= 0; i--) {
			int yStart = x - i <= 1 ? y - 1 : y - (x - i);
			for (int j = yStart; j >= 0; j--) sight[j][i] = 2;
		}
		return;
	}
	if (direction == UP && medusaX < x) {
		for (int i = x; i < n; i++) {
			int yStart = i - x <= 1 ? y - 1 : y - (i - x);
			for (int j = yStart; j >= 0; j--) sight[j][i] = 2;
		}
		return;
	}

	if (direction == DOWN && medusaX > x) {
		for (int i = x; i >= 0; i--) {
			int yStart = x - i <= 1 ? y + 1 : y + (x - i);
			for (int j = yStart; j < n; j++) sight[j][i] = 2;
		}
		return;
	}
	if (direction == DOWN && medusaX < x) {
		for (int i = x; i < n; i++) {
			int yStart = i - x <= 1 ? y + 1 : y + (i - x);
			for (int j = yStart; j < n; j++) sight[j][i] = 2;
		}
		return;
	}

	if (direction == LEFT && medusaY < y) {
		for (int i = y; i < n; i++) {
			int xStart = i - y <= 1 ? x - 1 : x - (i - y);
			for (int j = xStart; j >= 0; j--) sight[i][j] = 2;
		}
		return;
	}
	if (direction == LEFT && medusaY > y) {
		for (int i = y; i >= 0; i--) {
			int xStart = y - i <= 1 ? x - 1 : x - (y - i);
			for (int j = xStart; j >= 0; j--) sight[i][j] = 2;
		}
		return;
	}

	if (direction == RIGHT && medusaY < y) {
		for (int i = y; i < n; i++) {
			int xStart = i - y <= 1 ? x + 1 : x + (i - y);
			for (int j = xStart; j < n; j++) sight[i][j] = 2;
		}
		return;
	}
	if (direction == RIGHT && medusaY > y) {
		for (int i = y; i >= 0; i--) {
			int xStart = y - i <= 1 ? x + 1 : x + (y - i);
			for (int j = xStart; j < n; j++) sight[i][j] = 2;
		}
		return;
	}

}

// direction에 따라 돌로 만들 수 있는 병사 수 return
int lookDirection(int direction) {
	fill(&sight[0][0], &sight[n - 1][n], 0);
	int currStoneCnt = 0;

	for (int i = 1; i <= n; i++) {
		for (int j = i * -1; j <= i; j++) {
			int nextY = direction == UP || direction == DOWN ? medusaY + i * dy[direction] : medusaY + j;
			int nextX = direction == LEFT || direction == RIGHT ? medusaX + i * dx[direction] : medusaX + j;

			if (!isInRange(nextY, nextX)) continue;
			if (sight[nextY][nextX] == 2) continue; // 이미 가려진 칸이면 Pass

			sight[nextY][nextX] = 1;
			if (soldiers[nextY][nextX] == 0) continue;


			// 가려지지 않았고, 전사가 있는 경우 -> 돌 됨
			currStoneCnt += soldiers[nextY][nextX];
			hide(nextY, nextX, direction);
		}
	}
	return currStoneCnt;
}

/* // 매 턴마다 메두사의 루트를 찾는 함수
pair<int, int> findMedusaRoute() {
	fill(&visit[0][0], &visit[n - 1][n], false);
	pii prevRoute[52][52];
	queue<pair<pii, pii>> q; // {{nextY, nextX}, {firstY, firstX}}
	q.push({ {medusaY, medusaX}, {-1, -1} });
	visit[medusaY][medusaX] = true;
	//cout << "=======bfs start=========\n시작 좌표 : (" << medusaY << ", " << medusaX << ")\n";

	while (!q.empty()) {
		int currY = q.front().first.first;
		int currX = q.front().first.second;
		int firstY = q.front().second.first;
		int firstX = q.front().second.second;
		q.pop();
		//cout << currY << ", " << currX << "\n";

		// 길 찾은 경우 첫 좌표 반환
		if (currY == parkY && currX == parkX) {
			//cout << "return 첫 좌표 : (" << firstY << ", " << firstX << "\n";
			return { firstY, firstX };
		}

		for (int i = 0; i < 4; i++) {
			int nextY = currY + dy[i];
			int nextX = currX + dx[i];

			if (nextY < 0 || nextX < 0 || nextY >= n || nextX >= n) continue;
			if (!isRoad[nextY][nextX] || visit[nextY][nextX]) continue;

			visit[nextY][nextX] = true;
			if (firstY == -1) {
				q.push({ {nextY, nextX}, {nextY, nextX} });
				continue;
			}

			q.push({ {nextY, nextX}, {firstY, firstX} });
		}
	}
	return { -1, -1 };
}
*/

// 맨 처음에 메두사의 
void findMedusaRoute() {
	fill(&visit[0][0], &visit[n - 1][n], false);
	pii prevRoute[52][52];
	queue<pii> q; // 
	q.push({ medusaY, medusaX });
	visit[medusaY][medusaX] = true;
	//cout << "=======bfs start=========\n시작 좌표 : (" << medusaY << ", " << medusaX << ")\n";

	while (!q.empty()) {
		int currY = q.front().first;
		int currX = q.front().second;
		q.pop();
		//cout << currY << ", " << currX << "\n";

		// 길 찾은 경우 첫 좌표 반환
		if (currY == parkY && currX == parkX) {
			medusaRoute.push_front({ currY, currX });
			
			int prevY = prevRoute[currY][currX].first;
			int prevX = prevRoute[currY][currX].second;
			while (prevY != medusaY || prevX != medusaX) {
				medusaRoute.push_front({ prevY, prevX });
				prevY = prevRoute[medusaRoute.front().first][medusaRoute.front().second].first;
				prevX = prevRoute[medusaRoute.front().first][medusaRoute.front().second].second;
			}
			return;
		}

		for (int i = 0; i < 4; i++) {
			int nextY = currY + dy[i];
			int nextX = currX + dx[i];

			if (nextY < 0 || nextX < 0 || nextY >= n || nextX >= n) continue;
			if (!isRoad[nextY][nextX] || visit[nextY][nextX]) continue;

			visit[nextY][nextX] = true;
			prevRoute[nextY][nextX] = { currY, currX };
			q.push({nextY, nextX});
		}
	}
	return;
	
}

pair<int, int> findSoldierRoute(int fromY, int fromX, bool isFirst) {
	fill(&visit[0][0], &visit[n - 1][n], false);
	queue<pair<pii, pair<pii, int>>> q; // {{fromY, fromX}, {{firstY, firstX}, 거리}}
	q.push({ {fromY, fromX}, {{-1, -1}, 0} });
	visit[fromY][fromX] = true;
	//cout << "=======bfs start=========\n시작 좌표 : (" << fromY << ", " << fromX << ")\n";
	int euclidDist = abs(fromY - medusaY) + abs(fromX - medusaX);

	while (!q.empty()) {
		int currY = q.front().first.first;
		int currX = q.front().first.second;
		int firstY = q.front().second.first.first;
		int firstX = q.front().second.first.second;
		int currDist = q.front().second.second;
		q.pop();
		//cout << currY << ", " << currX << "\n";

		// 길 찾은 경우 첫 좌표 반환
		if (currY == medusaY && currX == medusaX && currDist == euclidDist) {
			//cout << "return 첫 좌표 : (" << firstY << ", " << firstX << "\n";
			return { firstY, firstX };
		}

		int from = isFirst ? 0 : 2; // 첫번재 이동은 상하좌우 순
		int till = isFirst ? 3 : 5; // 두번째 이동은 좌우상하 순
		for (int i = from; i <= till; i++) {
			int nextY = currY + dy[i % 4];
			int nextX = currX + dx[i % 4];

			if (!isInRange(nextY, nextX)) continue;
			if (visit[nextY][nextX]) continue;
			// 이동할 첫번째 칸이 메두사의 시야라면 갈 수 없음, 
			// 그 이후 칸들은 메두사의 시야여도 됨
			if (currY == fromY && currX == fromX && sight[nextY][nextX] == 1) continue;

			visit[nextY][nextX] = true;

			// 첫 칸 설정
			if (firstY == -1) {
				q.push({ {nextY, nextX}, {{nextY, nextX}, currDist + 1} });
				continue;
			}

			q.push({ {nextY, nextX}, {{firstY, firstX}, currDist + 1} });
		}
	}

	// 이동할 수 없는 경우
	return { -1, -1 };
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(nullptr);
	cout.tie(nullptr);
	cin >> n >> m;
	cin >> medusaY >> medusaX >> parkY >> parkX;
	leftSoldierCnt = m;

	for (int i = 0; i < m; i++) {
		int y, x;
		cin >> y >> x;
		soldiers[y][x]++;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int a;
			cin >> a;
			if (a == 0) isRoad[i][j] = true;
			else isRoad[i][j] = false;
		}
	}
	findMedusaRoute();
	// 메두사가 공원에 도착할 때 까지 반복
	while (!(medusaY == parkY && medusaX == parkX)) {
		moveCnt = 0;
		stoneCnt = 0;
		defeatCnt = 0;

		// 1. 메두사 이동
		//pair<int, int> nextYX = findMedusaRoute();
		
		// 메두사가 공원까지 갈 수 없다면 -1 출력 후 종료
		if (medusaRoute.empty()) {
			cout << "-1\n";
			return 0;
		}
		
		pii nextYX = medusaRoute.front();
		medusaY = nextYX.first;
		medusaX = nextYX.second;
		medusaRoute.pop_front();
		// 공원 도착 시 0 출력 후 종료
		if (medusaY == parkY && medusaX == parkX) {
			cout << "0\n";
			return 0;
		}
		

		// 메두사가 이동한 칸에 전사가 있다면 죽음
		if (soldiers[medusaY][medusaX] != 0) {
			leftSoldierCnt -= soldiers[medusaY][medusaX];
			soldiers[medusaY][medusaX] = 0;
		}

		// 전사가 다 죽은 경우 아래 코드 스킵
		if (leftSoldierCnt == 0) {
			cout << "0 0 0\n";
			continue;
		}

		// 2. 메두사의 시선
		int maxDirection = -1, maxStoneCnt = 0;
		for (int i = 0; i < 4; i++) {
			int currStoneCnt = lookDirection(i);
			
			/*
			cout << "바라보는 방향 : " << i << "\n";
			for(int a = 0; a < n;a++){
			    for(int j = 0; j < n; j++){
			        if(a == medusaY && j == medusaX){
			            cout << "X ";
			            continue;
			        }
			        cout << sight[a][j] << " ";
			    }
			    cout << "\n";
			}
			cout << "\n";
			*/
			if (currStoneCnt <= maxStoneCnt) continue; // 상하좌우 순이므로 같아도 continue
			maxDirection = i;
			maxStoneCnt = currStoneCnt;
		}
		// 최대 방향으로 sight 배열 설정
		stoneCnt = lookDirection(maxDirection);
		/*
		        cout << "바라보는 방향 : " << maxDirection << "\n";
		        for(int i = 0; i < n;i++){
		            for(int j = 0; j < n; j++){
		                if(i == medusaY && j == medusaX){
		                    cout << "X ";
		                    continue;
		                }
		                cout << sight[i][j] << " ";
		            }
		            cout << "\n";
		        }
		        cout << "\n";
			*/
		
		
		// 3. 전사 이동
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (soldiers[i][j] != 0 && sight[i][j] != 1) soldierLocs.push_back({ {i, j}, soldiers[i][j] });
			}
		}
		
		int soldierCnt = soldierLocs.size();
		// 첫번째 이동
		for (int i = 0; i < soldierCnt; i++) {
			int currY = soldierLocs.front().first.first;
			int currX = soldierLocs.front().first.second;
			int cnt = soldierLocs.front().second;

			pii result = findSoldierRoute(currY, currX, true);
			int nextY = result.first;
			int nextX = result.second;
			soldierLocs.pop_front();

			// 움직일 수 없거나 최단거리가 메두사의 시야일 경우
			if (nextY == -1 || sight[nextY][nextX] == 1) {
				soldierLocs.push_back({ {currY, currX}, cnt });
				continue;
			}

			moveCnt += cnt;
			soldiers[currY][currX] -= cnt;

			// 가는 도중 메두사를 만난 경우 defeatCnt에 더해주기
			if (nextY == medusaY && nextX == medusaX) {
				defeatCnt += cnt;
				leftSoldierCnt -= cnt;
				continue;
			}

			// 다시 덱에 넣어줌
			soldierLocs.push_back({ {nextY, nextX}, cnt });
			soldiers[nextY][nextX] += cnt;
		}
		/*
		        cout << "첫 번째 전사들 이동 결과\n";
		        for(int i = 0; i < n;i++){
		            for(int j = 0; j < n; j++){
		                cout << soldiers[i][j] << " ";
		            }
		            cout << "\n";
		        }
		        cout << "\n";
				*/

				// 두번째 이동
		while (!soldierLocs.empty()) {
			int currY = soldierLocs.front().first.first;
			int currX = soldierLocs.front().first.second;
			int cnt = soldierLocs.front().second;

			pii result = findSoldierRoute(currY, currX, false);
			int nextY = result.first;
			int nextX = result.second;
			soldierLocs.pop_front();

			// 움직일 수 없거나 최단거리가 메두사의 시야일 경우
			if (nextY == -1 || sight[nextY][nextX] == 1) continue;

			moveCnt += cnt;
			soldiers[nextY][nextX] += cnt;
			soldiers[currY][currX] -= cnt;

			// 두번 이동 후 메두사를 못 만난 경우 넘김
			if (nextY != medusaY || nextX != medusaX) continue;

			// 가는 도중 메두사를 만난 경우 값 저장
			defeatCnt += cnt;
			leftSoldierCnt -= cnt;
			soldiers[nextY][nextX] -= cnt;
		}
		/*
		        cout << "두 번째 전사들 이동 결과\n";
		        for(int i = 0; i < n;i++){
		            for(int j = 0; j < n; j++){
		                cout << soldiers[i][j] << " ";
		            }
		            cout << "\n";
		        }
		        cout << "\n";
				*/
		cout << moveCnt << " " << stoneCnt << " " << defeatCnt << "\n";

	}
}
