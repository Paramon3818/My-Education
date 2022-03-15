#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
ifstream fin("input.txt");
ofstream fout("output.txt");


class Animal 
{
protected:
	int x, y;
	int direct;
	int turn;
	int type;
public:
	int age;
	

	Animal(int x_1, int y_1, int direct_1, int turn_1) 
	{
		x = x_1;
		y = y_1;
		direct = direct_1;
		turn = turn_1;
		age = 0;
		int type = -1;
	}
	int getType() { return type; }
	int getX() { return x; }
	int getY() { return y; }
	int getDirect() { return direct; }
	int getTurn() { return turn; }
	void setX(int a) { x = a; }
	void setY(int a) { y = a; }
	void setDirect(int a) { direct = a; }
	void setTurn(int a) { turn = a; }
};

class Rabbit : public Animal 
{
public:
	Rabbit(int x_1, int y_1, int direct_1, int turn_1) : Animal(x_1, y_1, direct_1, turn_1)
	{
		this -> type = 0;
	}
};

class Wolf : public Animal
{
	int hungry;
public:
	Wolf (int x_1, int y_1, int direct_1, int turn_1) : Animal(x_1, y_1, direct_1, turn_1)
	{
		this->type = 1;
		this->hungry = 0;
	}
	int getHungry() { return hungry; }
	void setHungry(int a) { hungry = a; }


	static int Compare(const void* wolf1, const void* wolf2)
	{
		return(((Wolf*)wolf1)->age - ((Wolf*)wolf2)->age);
	}
};

class Simulation {
	int N;
	int M;
	int turns;
public:
	int move;
	Rabbit* rabbit;
	Wolf* wolf;
	int rabbits;
	int wolves;

	Simulation() {
		move = 0;

		fin >> N >> M >> turns >> rabbits >> wolves;
		rabbit = (Rabbit*) malloc(sizeof(Rabbit) * rabbits);
		wolf = (Wolf*)malloc(sizeof(Wolf) * wolves);

		for (int i = 0; i < rabbits; i++) 
		{
			int x, y;
			int direct;
			int age;
			fin >> x >> y >> direct >> age;
			new(&rabbit[i]) Rabbit(x, y ,direct, age);
		}

		for (int i = 0; i < wolves; i++)
		{
			int x, y;
			int direct;
			int age;
			fin >> x >> y >> direct >> age;
			new(&wolf[i]) Wolf(x, y, direct, age);
			wolf[i].setHungry(0);
		}

		qsort(wolf, wolves, sizeof(Wolf), Wolf::Compare);

		for (int i = 0; i < wolves; i++) 
		{
			wolf[i].setHungry(0);
		}
	}



	
	void NewRabbit(int x_1, int y_1, int direct_1, int turn_1) 
	{
		if (rabbit == NULL) 
		{
			rabbits = 1;

			rabbit = (Rabbit*)malloc(sizeof(Rabbit));
			new(&rabbit[0])Rabbit(x_1, y_1, direct_1, turn_1);

			return;
		}

		rabbits++;

		Rabbit* p = (Rabbit*)realloc(rabbit, rabbits * sizeof(Rabbit));
		rabbit = p;

		new(&rabbit[rabbits - 1]) Rabbit(x_1, y_1, direct_1, turn_1);
	}

	void NewWolf(int x_1, int y_1, int direct_1, int turn_1) 
	{
		if (wolf == NULL)
		{
			wolves = 1;

			wolf = (Wolf*)malloc(sizeof(Wolf));
			new(&wolf[0]) Wolf(x_1, y_1, direct_1, turn_1);
			return;
		}

		wolves++;

		Wolf* p = (Wolf*)realloc(wolf, wolves * sizeof(Wolf));
		wolf = p;

		new(&wolf[wolves - 1])  Wolf(x_1, y_1, direct_1, turn_1);
		wolf[wolves - 1].setHungry(0);
	}


	void DeathofWolf(int index) {
		if (index < 0 || wolves <= 0 || index > wolves - 1) 
		{ 
			return;
		}

		if (wolves == 1)
		{
			free(wolf);
			wolf = NULL;
			wolves = 0;

			return;
		}

		if (index == wolves - 1) 
		{
			wolves--;

			Wolf* p = (Wolf*)realloc(wolf, wolves * sizeof(Wolf));
			wolf = p;

			return;
		}

		Wolf copy(wolf[index].getX(), wolf[index].getY(), wolf[index].getDirect(), wolf[index].getTurn());

		wolf[index].setX(wolf[wolves - 1].getX());
		wolf[index].setY(wolf[wolves - 1].getY());
		wolf[index].setDirect(wolf[wolves - 1].getDirect());
		wolf[index].setTurn(wolf[wolves - 1].getTurn());
		wolf[index].age = 0;

		wolf[wolves - 1].setX(copy.getX());
		wolf[wolves - 1].setY(copy.getY());
		wolf[wolves - 1].setDirect(copy.getDirect());
		wolf[wolves - 1].setTurn(copy.getTurn());



		wolves--;

		Wolf* p = (Wolf*)realloc(wolf, wolves * sizeof(Wolf));
		wolf = p;
	}


	void DeathofRabbit(int index) {
		if (index < 0 || rabbits <= 0 || index > rabbits - 1)
		{
			return;
		}

		if (rabbits == 1) 
		{
			free(rabbit);
			rabbit = NULL;
			rabbits = 0;

			return;
		}

		if (index == rabbits - 1) 
		{
			rabbits--;

			Rabbit* p = (Rabbit*)realloc(rabbit, rabbits * sizeof(Rabbit));
			rabbit = p;

			return;
		}

		Rabbit copy(rabbit[index].getX(), rabbit[index].getY(), rabbit[index].getDirect(), rabbit[index].getTurn());

		rabbit[index].setX(rabbit[rabbits - 1].getX());
		rabbit[index].setY(rabbit[rabbits - 1].getY());
		rabbit[index].setDirect(rabbit[rabbits - 1].getDirect());
		rabbit[index].setTurn(rabbit[rabbits - 1].getTurn());
		rabbit[index].age = 0;

		rabbit[rabbits - 1].setX(copy.getX());
		rabbit[rabbits - 1].setY(copy.getY());
		rabbit[rabbits - 1].setDirect(copy.getDirect());
		rabbit[rabbits - 1].setTurn(copy.getTurn());



		rabbits--;

		Rabbit* p = (Rabbit*)realloc(rabbit, rabbits * sizeof(Rabbit));
		rabbit = p;
	}

	int Eatting(int x, int y) 
	{
		int dead = 0;

		for (int i = 0; i < rabbits; i++)
		{
			if (rabbit[i].getX() == x && rabbit[i].getY() == y)
			{
				dead++;
				DeathofRabbit(i);
			}
		}
		return dead;
	}



	void Print() {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				int k = 0;
				for (int w = 0; w < wolves; w++)
				{
					if (wolf[w].getX() == j && wolf[w].getY() == i)
					{
						k--;
					}
				}
				for (int r = 0; r < rabbits; r++) 
				{
					if (rabbit[r].getX() == j && rabbit[r].getY() == i) 
					{
						k++;
					}
				}
				if (k == 0) 
				{
					fout << "#"	 ;
				}
				else 
				{
					fout << k;
				}

			}

			fout << endl;
		}
	}

	void Move(Animal* animal)
	{
		int step = 1;

		if (animal->getType() == 1) 
		{
			step = 2;
		}

		if (animal->getDirect() == 0) animal->setY(animal->getY() - step);
		if (animal->getDirect() == 1) animal->setX(animal->getX() + step);
		if (animal->getDirect() == 2) animal->setY(animal->getY() + step);
		if (animal->getDirect() == 3) animal->setX(animal->getX() - step);

		if (move % animal->getTurn() == 0) 
		{
			animal->setDirect(animal->getDirect() + 1);
			animal->setDirect(animal->getDirect() % 4);
		}

		if (animal->getY() < 0) 
		{
			animal->setY(animal->getY() + N);
		}
		if (animal->getY() >= N) 
		{
			animal->setY(animal->getY() - N);
		}
		if (animal->getX() < 0) 
		{
			animal->setX(animal->getX() + M);
		}
		if (animal->getX() >= M) 
		{
			animal->setX(animal->getX() - M);
		}
	}


	void Move() 
	{
		move++;



		for (int i = 0; i < rabbits; i++) 
		{
			Move((Animal*)&rabbit[i]);
		}
		for (int i = 0; i < wolves; i++) 
		{
			Move((Animal*)&wolf[i]);			
		}




		for (int i = 0; i < wolves; i++)
		{
			wolf[i].setHungry(wolf[i].getHungry() + Eatting(wolf[i].getX(), wolf[i].getY()));
		}




		for (int i = 0; i < rabbits; i++)
		{
			rabbit[i].age++;
		}
		for (int i = 0; i < wolves; i++)
		{
			wolf[i].age++;
		}




		for (int i = 0; i < wolves; i++)
		{
			while (wolf[i].getHungry() >= 2) 
			{
				wolf[i].setHungry(0);
				NewWolf(wolf[i].getX(), wolf[i].getY(), wolf[i].getDirect(), wolf[i].getTurn());
			}
		}
		for (size_t i = 0; i < rabbits; i++)
		{
			if (rabbit[i].age == 5 || rabbit[i].age == 10) 
			{
				NewRabbit(rabbit[i].getX(), rabbit[i].getY(), rabbit[i].getDirect(), rabbit[i].getTurn());
			}
		}



		for (int i = 0; i < wolves; i++)
		{
			if (wolf[i].age == 15) 
			{
				DeathofWolf(i);
				qsort(wolf, wolves, sizeof(Wolf), Wolf::Compare);
			}
		}

		for (int i = 0; i < rabbits; i++)
		{
			if (rabbit[i].age == 10) 
			{
				DeathofRabbit(i);
			}
		}
		}




	void EEEEEAAAATTTT() 
	{
		while (move < turns) 
		{
			Move();
		}
		Print();
	}

};


int main()
{
	Simulation sim;
	sim.EEEEEAAAATTTT();
	fin.close();
	fout.close();
};