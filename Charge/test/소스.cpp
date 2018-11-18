#include <queue>
#include <random>
void main()
{
	std::queue<int> test;
	if (  test.empty() )
	{
		printf("»ç¶÷");
	}


	std::uniform_int_distribution<> uid(0, 4);
	std::default_random_engine dre;





	int i = uid(dre);
}