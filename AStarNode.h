#ifndef A_STAR_NODE_H
#define A_STAR_NODE_H

namespace AStarSearch
{

class AStarNode
{
private:
	AStarNode* parent;

	float G; //cost of this node + precedessors
	float H; //heuristic estimate of distance to goal
	float F; //Sum of heuristic and cost of this node + precedessors

	int x, y; //Position in the grid of nodes
public:
	AStarNode();
	AStarNode(int x, int y);
	void setG(float g);
	void setF(float f);
	void setH(float h);
	void setX(int x);
	void setY(int y);
	void setParent(AStarNode* newParent);

	float getG();
	float getF();
	float getH();
	int getX();
	int getY();
	AStarNode* getParent();
};

AStarNode::AStarNode() : parent(nullptr),
	G(0.0), H(0.0), F(0.0), x(0), y(0)
{

}

AStarNode::AStarNode(int x, int y) : parent(nullptr), G(0.0), H(0.0), F(0.0), x(x), y(y)
{

}

void AStarNode::setG(float g) { this->G = g; }
void AStarNode::setF(float f) { this->F = f; }
void AStarNode::setH(float h) { this->H = h; }
void AStarNode::setX(int x) { this->x = x; }
void AStarNode::setY(int y) { this->y = y; }
void AStarNode::setParent(AStarNode* newParent) { this->parent = newParent; }

float AStarNode::getG() { return G; }
float AStarNode::getF() { return F; }
float AStarNode::getH() { return H; }
int AStarNode::getX() { return x; }
int AStarNode::getY() { return y; }
AStarNode* AStarNode::getParent() { return parent; }

}; // namespace AStarSearch
#endif
