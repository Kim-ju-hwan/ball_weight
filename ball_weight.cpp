#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>
#include <cstdlib>
#include <ctime>

using namespace bangtal;

int x[6] = { 100,250,400,550,700,850 };
int weight[6] = { 1,2,3,4,5,6 };

ObjectPtr obj[6];

class ball {
	int weight;
	ObjectPtr ball1;
public:
	ball() :weight(0), ball1(NULL) {};
    void show() {ball1->show();}
    void hide() {ball1->hide();}
    void locate(ScenePtr scene, int x, int y) {ball1->locate(scene, x, y);}
	void set(int a, ObjectPtr b) {
		weight = a;
		ball1 = b;
	}
	int get_weight(ball b) { return b.weight; }
	bool Is_big(ball a) {
		if (a.weight < weight)
			return true;
		else
			return false;
	}
};

int game_index(ObjectPtr object) {
	for (int i = 0;i < 6;i++) {
		if (obj[i] == object)	return i;
	};
}

void set_game() {
	auto scene = Scene::create("", "Images/배경.png");
	int ballnum[9] = { 1,2,3,4,5,6,7,8,9 };
	bool moved[6] = { false, false, false, false, false, false };
	int choose =0;
	int round = 1, can_try=5, pick_num = 0;
	int correct = 0, wrong = 0;
	ball balls[6],pick[6];
	ball moved_ball;
	auto shown = Object::create("Images/비교창.png", scene, 320, 475);
	shown->setScale(0.5f);
	auto big= Object::create("Images/크다.png", scene, 455, 500);
	auto small= Object::create("Images/작다.png", scene, 455, 500);
	big->hide(); small->hide();
	char path[50];
	srand(time(NULL));
	int temp;
	int r;
	//무게 랜덤 배정
	for (int i = 0; i < 6; i++) {
		r = rand() % 6;
		temp = weight[i];
		weight[i] = weight[r];
		weight[r] = temp;
	}
	//공 랜덤 배정
	for (int i = 0; i < 9; i++) {
		r = rand() % 9;
		temp = ballnum[i];
		ballnum[i] = ballnum[r];
		ballnum[r] = temp;
	}

	//공 6개 생성 및 마우스 콜백함수 설정
	int move_num = 0;
	for (int i = 0;i < 6;i++) {
		sprintf(path, "Images/공들_00%d.png", ballnum[i]);
		obj[i] = Object::create(path, scene, x[i], 900);
		obj[i]->setScale(0.5f);
		balls[i].set(weight[i], obj[i]);
		obj[i]->setOnMouseCallback([&](ObjectPtr object, int x1, int y1, MouseAction action)->bool {
			int num = game_index(object);
			if (move_num < can_try) {
				if (moved[num] == false) {
					if (choose==1) {
						obj[num]->locate(scene, 550, 500);
						moved[num] = true;
						move_num++;
						choose++;
						if (balls[num].Is_big(moved_ball)) {
							small->show();
							big->hide();
						}
						else {
							big->show();
							small->hide();
						}
					}
					else if(choose==0) {
						obj[num]->locate(scene, 350, 500);
						moved[num] = true;						
						moved_ball = balls[num];
						choose++;
					}
				}
				else {
					for (i = 0;i < 6;i++) {
						obj[i]->locate(scene, x[i], 900);
						moved[i] = false;
					}
					choose =0;
					big->hide();
					small->hide();
				}
			}
			else {
				if (moved[num]) {
					for (i = 0;i < 6;i++) {
						obj[i]->locate(scene, x[i], 900);
						moved[i] = false;
					}
					shown->hide();
					big->hide();
					small->hide();
					pick_num=0;
					choose = 0;
				}
				else if(choose==0){
					pick[pick_num] = balls[num];
					moved[num] = true;
					if(round<4) balls[num].locate(scene, x[pick_num+1], 500);
					else balls[num].locate(scene, x[pick_num], 500);
					pick_num++;
				}
			}
			return true;
			});
	}
	if (round < 4) {
		can_try = 5;
		balls[0].hide();
		balls[5].hide();
	}
	
	auto next_button= Object::create("Images/다음.png", scene, 750, 700);
	next_button->setOnMouseCallback([&](ObjectPtr object, int x1, int y1, MouseAction action)->bool {
		bool is_correct=true;
		if (round < 4) {
			for (int i = 0;i < 3;i++) {
				if (pick[i].Is_big(pick[i + 1]))
					continue;
				else {
					is_correct = false;
					break;
				}
			}
		}
		else if (round < 7) {
			for (int i = 0;i < 4;i++) {
				if (pick[i].Is_big(pick[i + 1]))
					continue;
				else {
					is_correct = false;
					break;
				}
			}
		}
		else {
			for (int i = 0;i < 5;i++) {
				if (pick[i].Is_big(pick[i + 1]))
					continue;
				else {
					is_correct = false;
					break;
				}
			}
		}
		if (is_correct)	correct++;
		else wrong++;
		if (round < 3) {
			can_try = 5;
			balls[0].hide();
			balls[5].hide();
		}
		else if (round < 6) {
			can_try = 8;
			balls[0].show();
			balls[5].hide();
		}
		else {
			can_try = 10;
			balls[5].show();
		}
		round++;
		printf("%d %d %d\n", correct, wrong, round-1);
		pick_num = 0,move_num=0;
		for (int i = 0;i < 6;i++) moved[i] = false,pick[i]=balls[0];
		choose = 0;
		//무게와 공을 다시 랜덤 배정, 라운드에 따라  다음라운드 시도 횟수도 조정
		if (round < 10) {
			shown->show();
			for (int i = 0; i < 9; i++) {
				r = rand() % 9;
				temp = ballnum[i];
				ballnum[i] = ballnum[r];
				ballnum[r] = temp;
			}
			for (int i = 0; i < 9; i++) {
				r = rand() % 9;
				temp = ballnum[i];
				ballnum[i] = ballnum[r];
				ballnum[r] = temp;
			}
			for (int i = 0;i < 6;i++) {
				sprintf(path, "Images/공들_00%d.png", ballnum[i]);
				obj[i]->setImage(path);
				balls[i].set(weight[i], obj[i]);
				balls[i].locate(scene, x[i], 900);
			}	
		}
		else {
			sprintf(path, "정답 : %d  오답 : %d", correct,wrong);
			showMessage(path);
		}
		return true;
		});
	startGame(scene);
}

int main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	set_game();
	return 0;
}
