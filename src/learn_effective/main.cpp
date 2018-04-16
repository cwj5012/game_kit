#include <windows.h>

class GamePlayer {
private:
	static const int num_turns = 5;
	int scores[num_turns];
};

const int GamePlayer::num_turns;

int main(int argc, char* argv[]) {
	Sleep(3000);
	return 0;
}
