#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <cstdlib>
#include <ncurses.h>
#include <string>

std::queue <int> kolejka1;
std::mutex kolejka1Guard;

std::queue <int> kolejka2;
std::mutex kolejka2Guard;

int wym = 0;

int tab[100][100];
std::mutex tabGuard;

std::mutex cursesGuard;

void producent()
{
	srand(time(NULL));
	while(true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1)); 
		kolejka1Guard.lock();
		kolejka2Guard.lock();
		cursesGuard.lock();

		int liczba1 =(std::rand() % wym) + 0;
		kolejka1.push(liczba1);
		
		int liczba2 =(std::rand() % wym) + 0;
		kolejka2.push(liczba2);

		std::cout << "producent" << std::endl;
		
		cursesGuard.unlock();	
		kolejka2Guard.unlock();
		kolejka1Guard.unlock();
	}	
}

void przetwarzacz()
{	
	while(true)
	{	
		/*if(kolejka2.empty())
		{
			continue;
			producent();
		}*/
		std::this_thread::sleep_for(std::chrono::seconds(1)); 
		kolejka2Guard.lock();
		cursesGuard.lock();
		kolejka1Guard.lock();

		tab[kolejka1.front()][kolejka2.front()] = 1;
		kolejka1.pop();
		kolejka2.pop();

		std::cout << "przetwarzacz" << std::endl;
		
		kolejka1Guard.unlock();
		cursesGuard.unlock();
		kolejka2Guard.unlock();
	}	
}

void curses(){

	while(true)
	{	
		//initscr();
		std::this_thread::sleep_for(std::chrono::seconds(1)); 
		
		for( int x = 0 ; x < wym ; x++ )
		{
			for( int y = 0 ; y < wym ; y++ )
			{	
				//printw("%i", tab[x][y]);
				std::cout << tab[x][y] << "  ";
		    		//std::cout << std::endl;
			}
		}
		//endwin();
		std::cout << std::endl;
	}

	//initscr();
	/*std::this_thread::sleep_for(std::chrono::seconds(1)); 
	cursesGuard.lock();
	kolejka2Guard.lock();
	kolejka1Guard.lock();
		
	for( int x = 0 ; x < wym ; x++ )
	{

		for( int y = 0 ; y < wym ; y++ )
		{
		      	//printw("%i", tab[x][y]);
			std::cout << tab[x][y];
			//refresh();
		    	std::cout << std::endl;
		}
	}
	//endwin();
	
	kolejka1Guard.unlock();
	kolejka2Guard.unlock();
	cursesGuard.unlock();*/
}

int main()
{
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

