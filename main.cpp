#include "AStarSearch.h"
#include <iostream>

int main()
{
	AStarSearch::AStarSearch as;
	std::vector<std::vector<bool> > walkable = {
	{ true, false, true, true, true, false, true },
	{ true, false, true, false, true, false, true },
	{ true, true, true, false, true, true, true },
	{ false, true, true, false, true, true, true },
	{ true, false, true, false, true, true, true },
	{ true, true, true, true, true, true, true }
	};
	as.init(7, 6, 0, 0, 6, 0, walkable);
	AStarSearch::AStarPath path;
	try
	{
		path = as.calculatePath();
	}
	catch (const char* e)
	{
		std::cerr << e;
	}

	auto start = path.getFirstNode();
	auto curr = start;

	for (int i = 0; i < path.getLength(); ++i)
	{
		if (curr == nullptr)
			break;
		std::cout << curr->getX() << " " << curr->getY() << "\n";
		curr = path.getNextNode(curr);
	}

	std::cin.get();
	return 0;
}