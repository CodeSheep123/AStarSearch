#ifndef A_STAR_SEARCH_H
#define A_STAR_SEARCH_H


#include <vector> //Vector
#include <iostream> //Debug output
#include <algorithm> //std::remove
#include <cmath> //std::sqrt and std::pow
#include "AStarNode.h"
#include "AStarPath.h"

namespace AStarSearch
{

	class AStarSearch
	{
	private:
		std::vector<std::vector<AStarNode> > nodes;
		std::vector<std::vector<bool> > walkableNodes;
		int width, height;
		AStarNode* startNode;
		AStarNode* endNode;

		void setNodeListSize();
		void resetWalkable();
		void createNodes();

		bool wasInit;

		void calculateScores();
		void calculateGScores();
		void calculateHScores();
		void calculateFScores();
		void calculateGScore(int x, int y, int prevScore, std::vector<std::vector<bool> > &visitedGScore);
		float getHeuristicDistance(AStarNode &start, AStarNode &end);

		std::vector<AStarNode*> getWalkableAdjacentNodes(int x, int y);
		bool listContains(std::vector<AStarNode*> v, AStarNode* to_find);
	public:
		AStarSearch();

		void setStart(int x, int y);
		void setEnd(int x, int y);

		bool init(int width, int height, int startX, int startY, int endX, int endY, std::vector<std::vector<bool> > walkable);
		void reset();

		/*@return an instance of AStarPath, which can be used to get the coordinates for every step of the path
		  @except throws a const char* if A* search is not initialized or if there is no path*/
		AStarPath calculatePath();
	};

	AStarSearch::AStarSearch()
	{
		/*Give all variables their default values*/
		width = 0;
		height = 0;
		startNode = nullptr;
		endNode = nullptr;
		setNodeListSize();
		resetWalkable();

		wasInit = false;
	}

	void AStarSearch::setNodeListSize()
	{
		/*Resize main vector (vector of rows)*/
		nodes.resize(height);
		for (int index = 0; index < height; ++index)
		{
			/*Resize subvectors (rows)*/
			nodes[index].resize(width);
		}
	}

	void AStarSearch::resetWalkable()
	{
		for (int index = 0; index < height; ++index)
		{
			walkableNodes[index].resize(0);
		}
		walkableNodes.resize(0);
	}

	void AStarSearch::createNodes()
	{
		/*Set every node to have it's default values, except x = the X coord in the node list, and y = the Y coord in the node list*/
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				nodes[y][x] = AStarNode(x, y);
			}
		}
	}

	bool AStarSearch::listContains(std::vector<AStarNode*> v, AStarNode* to_find)
	{
		for (unsigned int index = 0; index < v.size(); ++index)
		{
			if (v[index] == to_find)
				return true;
		}
		return false;
	}

	bool AStarSearch::init(int width, int height, int startX, int startY, int endX, int endY, std::vector<std::vector<bool> > walkable)
	{
		/*Check if start or end position are out of range*/
		if (startX < 0 || startY < 0 || startX >= width || startY >= height ||
			endX < 0 || endY < 0 || endX >= width || endY >= height || (startX == endX && startY == endY))
			return false;

		/*Set size*/
		this->width = width;
		this->height = height;
		/*Create nodes*/
		setNodeListSize();
		createNodes();

		/*Set start and end node pointers*/
		startNode = &nodes[startY][startX];
		endNode = &nodes[endY][endX];

		/*Nullptr check*/
		if (startNode == nullptr || endNode == nullptr)
			return false;

		walkableNodes = walkable;


		calculateScores();

		/*Initialization success*/
		wasInit = true;

		return true;
	}

	void AStarSearch::reset()
	{
		/*Reset all values to default, set wasInit to false*/
		startNode = nullptr;
		endNode = nullptr;
		width = 0;
		height = 0;
		setNodeListSize();

		wasInit = false;
	}

	void AStarSearch::setStart(int x, int y)
	{
		/*Check if out of range*/
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;
		startNode = &nodes[y][x];
		/*Start position changed, recalculate scores*/
		calculateScores();
	}

	void AStarSearch::setEnd(int x, int y)
	{
		/*Check if out of range*/
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;
		endNode = &nodes[y][x];

		/*End position changed, recalculate scores*/
		calculateScores();
	}

	void AStarSearch::calculateScores()
	{
		calculateHScores();
		calculateGScores();
		calculateFScores();
	}

	void AStarSearch::calculateGScore(int x, int y, int prevScore, std::vector<std::vector<bool> > &visited)
	{
		/*Get new score*/
		int newScore = prevScore + 1;
		/*If new score is greater than old score, and we have already visited the node, return*/
		if (visited[y][x] == true && nodes[y][x].getG() < newScore)
			return;
		
		/*Set G score for current Node*/
		nodes[y][x].setG((float)newScore);

		/*Set visited*/
		visited[y][x] = true;

		/*Recursively call calculateGScore for surrounding nodes
		  We need to test to make sure we don't go out of bounds. We also have to check if the node is walkable.
		  Note that we can't go out of bounds in the if statement because the second condition won't be tested if the first
		  is false.*/
		if (x > 0 && walkableNodes[y][x - 1])
			calculateGScore(x - 1, y, newScore, visited);
		if (x < width - 1 && walkableNodes[y][x + 1])
			calculateGScore(x + 1, y, newScore, visited);
		if (y > 0 && walkableNodes[y - 1][x])
			calculateGScore(x, y - 1, newScore, visited);
		if (y < height - 1 && walkableNodes[y + 1][x])
			calculateGScore(x, y + 1, newScore, visited);
	}

	void AStarSearch::calculateGScores()
	{
		std::vector<std::vector<bool> > visitedGScore;
		visitedGScore.resize(height);

		for (int y = 0; y < height; ++y)
		{
			visitedGScore[y].resize(width);
			for (int x = 0; x < width; ++x)
			{
				visitedGScore[y][x] = false;
				nodes[y][x].setG(10000);
			}
		}

		int startX = startNode->getX();
		int startY = startNode->getY();

		visitedGScore[startY][startX] = true;


	}

	void AStarSearch::calculateFScores()
	{
		/*Set the F score for every node to be equal to the sum of it's G and H scores.
		  If the node is not walkable, set the score to a hight number (10000)*/
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				if (walkableNodes[y][x])
					nodes[y][x].setF(nodes[y][x].getG() + nodes[y][x].getH());
				else
					nodes[y][x].setF(10000);
			}
		}
	}

	void AStarSearch::calculateHScores()
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				nodes[y][x].setH(getHeuristicDistance(nodes[y][x], *endNode));
			}
		}
	}

	float AStarSearch::getHeuristicDistance(AStarNode &start, AStarNode &end)
	{
		return float(sqrt(pow(start.getX() - end.getX(), 2) + pow(start.getY() - end.getY(), 2)));
	}

	std::vector<AStarNode*> AStarSearch::getWalkableAdjacentNodes(int x, int y)
	{
		/*Create the vector*/
		std::vector<AStarNode*> adjacent;

		/*Check if adjacent nodes are not out of range and if they are walkable*/
		if (x > 0 && walkableNodes[y][x - 1])
			adjacent.push_back(&nodes[y][x - 1]);
		if (x < width - 1 && walkableNodes[y][x + 1])
			adjacent.push_back(&nodes[y][x + 1]);
		if (y > 0 && walkableNodes[y - 1][x])
			adjacent.push_back(&nodes[y - 1][x]);
		if (y < height - 1 && walkableNodes[y + 1][x])
			adjacent.push_back(&nodes[y + 1][x]);

		return adjacent;
	}

	AStarPath AStarSearch::calculatePath()
	{
		if (!wasInit)
		{
			throw "AStarSearch::calculatePath():  A* Search was not initialized!\n";
		}
		/*Create open and closed lists*/
		std::vector<AStarNode*> openList;
		std::vector<AStarNode*> closedList;

		openList.clear();
		closedList.clear();

		/*Add the start node to the open list*/
		openList.push_back(startNode);

		bool foundPath = false;

		do
		{
			/*Get the lastly added node with lowest F score in the open list*/
			AStarNode* currentNode = openList[0];
			for (unsigned int index = 0; index < openList.size(); ++index)
			{
				if (openList[index]->getF() <= currentNode->getF())
					currentNode = openList[index];
			}

			/*Remove the current node from the open list, add it to the closed list*/
			openList.erase(std::find(openList.begin(), openList.end(), currentNode));
			closedList.push_back(currentNode);

			/*Check if the destination is in the closed list*/
			if (listContains(closedList, endNode))
			{
				/*Found a path, break the loop*/
				foundPath = true;
				break;
			}

			/*Find walkable and adjacent nodes*/
			std::vector<AStarNode*> walkableAdjacent = getWalkableAdjacentNodes(currentNode->getX(), currentNode->getY());

			for (std::vector<AStarNode*>::iterator it = walkableAdjacent.begin(); it != walkableAdjacent.end(); ++it)
			{
				/*Skip the node if it is in the closed list*/
				if (std::find(closedList.begin(), closedList.end(), *it) != closedList.end())
				{
					/*Skip to next node*/
					continue;
				}
				/*If the node is not in the open list, set it's parent and add it to the open list*/
				if (!listContains(openList, *it))
				{
					/*Set the parent to the current node*/
					(*it)->setParent(currentNode);
					/*Add the node to the open list*/
					openList.push_back(*it);
				}
				else
				{
					float gScoreFromHere = currentNode->getG() + 1;
					if (gScoreFromHere < (*it)->getG())
					{
						(*it)->setG(gScoreFromHere);
						(*it)->setParent(currentNode);
					}
				}
			}

		} while (!openList.empty());

		/*Throw an exception if we didn't find a path*/
		if (!foundPath)
		{
			throw "AStarSearch::calculatePath(): No path found!\n";
		}

		/*If we now go to the end node, and backtrace all parents, we get the path from the start to the end*/
		AStarPath finalPath;
		AStarNode* currentNode = endNode;
		do
		{
			finalPath.addNodeAtFront(*currentNode);
			/*Nullptr check*/
			if (currentNode->getParent() == nullptr)
				break;
			currentNode = currentNode->getParent();
		} while (currentNode != startNode);
		finalPath.addNodeAtFront(*startNode);

		return finalPath;
	}
	


}; // namespace AStarSearch

#endif
