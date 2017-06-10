#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <cstdlib>

std::queue <int> kolejka1;
std::mutex kolejka1Guard;

std::queue <int> kolejka2;
std::mutex kolejka2Guard;

int wym = 0;

void producent()
{
	srand(time(NULL));
	for (int i = 0; i < wym; i++)
	{
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
	int tab[wym][wym];

  	/*for( int x = 0 ; x < wym ; x++ )
	{
    		for( int y = 0 ; y < wym ; y++ )
		{
      			tab[kolejka1.front()][kolejka2.front()] = 1;
		}
	}*/

	for (int i = 0; i < wym; i++)
	{
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

int main()
{
	std::cout << "Podaj wymiar macierzy kwadratowej: " << std::endl;
  	std::cin >> wym;
	
	auto producentThread = std::thread(producent); 
	auto przetwarzaczThread = std::thread(przetwarzacz); 

	producentThread.join(); 
	przetwarzaczThread.join();

	std::cout << "main done" << std::endl;

	return 0;
}