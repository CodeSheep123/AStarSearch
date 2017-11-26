#ifndef A_STAR_PATH
#define A_STAR_PATH

#include "AStarNode.h"
#include <deque>

namespace AStarSearch
{
class AStarPath
{
private:
	std::deque<AStarNode> path;
public:
	void addNodeAtFront(AStarNode node);
	void addNodeAtEnd(AStarNode node);

	int getLength();
	AStarNode* getFirstNode();
	AStarNode* getNextNode(AStarNode* currentNode);

	void reverse();
};

void AStarPath::addNodeAtFront(AStarNode node)
{
	path.push_front(node);
}

void AStarPath::addNodeAtEnd(AStarNode node)
{
	path.push_back(node);
}

int AStarPath::getLength() { return path.size(); }

AStarNode* AStarPath::getFirstNode() { return &path.front(); }

AStarNode* AStarPath::getNextNode(AStarNode* currentNode)
{
	/*Check if we're at the end*/
	if (currentNode == &(*(path.end() - 1)))
	{
		return nullptr;
	}
	for (std::deque<AStarNode>::iterator it = path.begin(); it != path.end(); ++it)
	{
		/*Check if iterator is equal to currentNode*/
		if ((*it).getX() == currentNode->getX() && (*it).getY() == currentNode->getY())
		{
			/*Return a pointer to the next node*/
			return &(*(it + 1));
		}
	}
	return nullptr;
}


}; // namespace AStarSearch

#endif
