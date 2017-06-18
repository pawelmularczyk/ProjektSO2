#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <cstdlib>
#include <ncurses.h>

std::queue <int> kolejka1;
std::mutex kolejka1Guard;

std::queue <int> kolejka2;
std::mutex kolejka2Guard;

int wym = 0;

int tab[100][100];
std::mutex tabGuard;

void producent()
{
	srand(time(NULL));
	for (int i = 0; i < wym; i++)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1)); 
		kolejka1Guard.lock();
		kolejka2Guard.lock();

		int liczba1 =(std::rand() % wym) + 0;
		kolejka1.push(liczba1);
		
		int liczba2 =(std::rand() % wym) + 0;
		kolejka2.push(liczba2);

		std::cout << "producent" << std::endl;
		
		kolejka2Guard.unlock();
		kolejka1Guard.unlock();
	}	
}

void przetwarzacz()
{	
	for (int i = 0; i < wym; i++)
	{	
		if(kolejka2.empty())
		{
			continue;
			producent();
		}
		std::this_thread::sleep_for(std::chrono::seconds(1)); 
		kolejka2Guard.lock();
		kolejka1Guard.lock();

		tab[kolejka1.front()][kolejka2.front()] = 1;
		kolejka1.pop();
		kolejka2.pop();

		/*for( int x = 0 ; x < wym ; x++ )
		{
			for( int y = 0 ; y < wym ; y++ )
		      		std::cout << tab[x][y] << "  ";
		    		std::cout << std::endl;
		}*/

		std::cout << "przetwarzacz" << std::endl;
		
		kolejka1Guard.unlock();
		kolejka2Guard.unlock();
	}	
}

void curses(){
	for (int i = 0; i < wym; i++)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep(2);
		std::cout << "curses" << std::endl;
	}
}

void testCurses(){
	initscr();			
	printw("Hello World !!!");	
	refresh();		
	getch();			
	endwin();			
}

int main()
{
	testCurses();
	std::cout << "Podaj wymiar macierzy kwadratowej: " << std::endl;
  	std::cin >> wym;
	
	auto producentThread = std::thread(producent); 
	auto przetwarzaczThread = std::thread(przetwarzacz);
	auto cursesThread = std::thread(curses); 

	producentThread.join(); 
	przetwarzaczThread.join();
	cursesThread.join();

	std::cout << "main done" << std::endl;

	return 0;
}

