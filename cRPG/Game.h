#ifndef GAME_H
#define GAME_H

class Game
{
public:
	Game();
	~Game();

	void Run();

private:

	bool is_running_;

	bool Init();

	void ProcessInputs();
	void Update();
	void Draw();
};

#endif // !GAME_H
