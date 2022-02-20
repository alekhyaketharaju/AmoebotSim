
#include "alg/demo/caterpillar1.h"
//#include "stdafx.h"
#include <iostream>
#include <cstdlib>

using namespace std;

std::vector<int> _nextOne; //contains {number i in next_i, x, y, label pointed to}
std::vector<int> _nextTwo; //contains {number i in next_i, x, y, label pointed to}
std::vector<std::vector<int>> _firstNode; //contains {number i in firstnode_i, x, y, label pointed to}
int layer = 1;

Caterpillar1Particle::Caterpillar1Particle(const Node head,
	const int globalTailDir,
	const int orientation,
	AmoebotSystem &system,
	State state)
	: AmoebotParticle(head, globalTailDir, orientation, system),
	_state(state),
	_followingBondedNbr(-1),
	_precedingBondedNbr(-1) {
	if (_state == State::Leader)
		_color = getLeaderColor();
	else
		_color = getRandColor();
}

void Caterpillar1Particle::activate() {
	//int dir = randDir();
	//bool flag = true;
	if (_state == State::Leader) {
		//Leader state
		if (isExpanded())
			contractTail();
		_color = getLeaderColor();
		_firstNode.push_back({ layer, head.x, head.y, 3 });
		if (hasNbrAtLabel(_precedingBondedNbr)) {
			nbrAtLabel(_precedingBondedNbr)._receive_head = 0; //root activation
			nbrAtLabel(_precedingBondedNbr)._state = State::Root;
		}
	}
	else if (_state == State::Root) {
		//Root state
		if (isExpanded())
			contractTail();
		_color = getRootColor();
		if (isPointedByFirstNode(_firstNode, head.x, head.y) == 1) {
			_nextOne = { 1, head.x, head.y, 1 };
			_nextTwo = { 1, head.x, head.y, 5 };

			_receive_head = 1;
			int r_stat = rotate();

		}
		else if (isPointedByFirstNode(_firstNode, head.x, head.y) != 1 || isPointedByFirstNode(_firstNode, head.x, head.y) != -1) {
			_nextOne = { isPointedByFirstNode(_firstNode, head.x, head.y), head.x, head.y, 2 };
			_nextTwo = { isPointedByFirstNode(_firstNode, head.x, head.y), head.x, head.y, 5 };

			_receive_head = 1;
			int r_stat = rotate();

		}

		else if (((isPointedByNextNode(_nextOne, head.x, head.y) != -1) ^ (isPointedByNextNode(_nextOne, head.x, head.y) != -1))) { //pointed only by one nexti
			int pointed = -1;
			if (isPointedByNextNode(_nextOne, head.x, head.y) != -1)
				pointed = 1;
			else
				pointed = 2;
			for (int label : tailLabels()) {
				if (!hasNbrAtLabel(label)) {
					int a = label + 1;
					int b = label - 1;
					if ((hasNbrAtLabel(a) && nbrAtLabel(a)._state == State::Retired && a != _precedingBondedNbr && a != _followingBondedNbr) || (hasNbrAtLabel(b) && nbrAtLabel(b)._state == State::Retired && b != _precedingBondedNbr && b != _followingBondedNbr)) {
						if (pointed == 1) {
							_nextOne = { isPointedByNextNode(_nextOne, head.x, head.y), head.x, head.y, label };
						}
						else
							_nextTwo = { isPointedByNextNode(_nextTwo, head.x, head.y), head.x, head.y, label };
					}
				}
			}

			if (hasNbrAtLabel(_precedingBondedNbr)) {
				if (isPointedByNextNode(_nextOne, nbrAtLabel(_precedingBondedNbr).head.x, nbrAtLabel(_precedingBondedNbr).head.y) == -1 || isPointedByNextNode(_nextTwo, nbrAtLabel(_precedingBondedNbr).head.x, nbrAtLabel(_precedingBondedNbr).head.y) == -1) {
					_receive_head = 1;
					int r_stat = rotate();
				}
			}
		}

		else {
			int pointed = isPointedByNextNode(_nextOne, head.x, head.y);
			_firstNode.push_back({ (pointed + 1) % 4, head.x, head.y, 3 });
		}

		if (hasNbrAtLabel(_precedingBondedNbr)) {
			nbrAtLabel(_precedingBondedNbr)._receive_head = 0;
			nbrAtLabel(_precedingBondedNbr)._state = State::Root;
		}
		_state = State::Retired;


	}

	else if (_state == State::Follower) {
		if (_receive_head == 0) {
			_state = State::Root;
		}


		else if (_state == State::Retired) {
			if (_precedingBondedNbr == -1 && hasNbrAtLabel(_followingBondedNbr)) {
				nbrAtLabel(_followingBondedNbr)._receive_tail = 0; //terminate
				_state = State::Terminated;
			}

			if (_receive_tail == 0) {
				if (hasNbrAtLabel(_followingBondedNbr)) {
					nbrAtLabel(_followingBondedNbr)._receive_tail = 0; //terminate
				}
				_state = State::Terminated;
			}
		}


	}
}



int Caterpillar1Particle::headMarkColor() const {
	switch (_color) {
	case Color::Red:    return 0xff0000;
	case Color::Yellow:	return 0xffff00;
	case Color::Green:	return 0x00ff00;
	case Color::Blue:   return 0x0000ff;
	case Color::Indigo:	return 0x4b0082;
	}

	return -1;
}

int Caterpillar1Particle::headMarkDir() const {
	return _precedingBondedNbr;
}

int Caterpillar1Particle::tailMarkColor() const {
	return headMarkColor();
}

std::vector<int> Caterpillar1Particle::getNodeFromLabel(int x, int y, int label) const {
	std::vector<int> res;
	if (label == 0) {
		res.push_back(x+1);
		res.push_back(y);
	}
	else if (label == 1) {
		res.push_back(x);
		res.push_back(y + 1);
	}
	else if (label == 2) {
		res.push_back(x - 1);
		res.push_back(y + 1);
	}
	else if (label == 3) {
		res.push_back(x - 1);
		res.push_back(y);
	}
	else if (label == 4) {
		res.push_back(x);
		res.push_back(y - 1);
	}
	else {//if (label == 5) 
		res.push_back(x + 1);
		res.push_back(y - 1);
	}
	return res;
}


bool Caterpillar1Particle::checkIfNodeIsNbr(int x1, int y1, int x2, int y2) const {
	if (abs(x1 - x2) > 1 || abs(y1 - y1) > 1) {
		return false;
	}
	return true;
}

int Caterpillar1Particle::isPointedByFirstNode(std::vector<std::vector<int>> firstNode, int x, int y) const {

	for (int i = 0; i < firstNode.size(); i++) {

		if (firstNode[i][3] == 0 && y == 0 && x > firstNode[i][1]) {
			return (firstNode[i][0]);
		}
		else if (firstNode[i][3] == 1 && x == 0 && y > firstNode[i][2]) {
			return firstNode[i][0];
		}
		else if (firstNode[i][3] == 2 && x == -y && y > firstNode[i][2]) {
			return firstNode[i][0];
		}
		else if (firstNode[i][3] == 3 && y == 0 && x < firstNode[i][1]) {
			return firstNode[i][0];
		}
		else if (firstNode[i][3] == 4 && x == 0 && y < firstNode[i][2]) {
			return firstNode[i][0];
		}
		else if (firstNode[i][3] == 5 && x == -y && x > firstNode[i][1]) {
			return firstNode[i][0];
		}

	}


	return -1;
}


int Caterpillar1Particle::isPointedByNextNode(std::vector<int> next, int x, int y) const {


	if (next[3] == 0 && y == 0 && x > next[1]) {
		return next[0];
	}
	else if (next[3] == 1 && x == 0 && y > next[2]) {
		return next[0];
	}
	else if (next[3] == 2 && x == -y && y > next[2]) {
		return next[0];
	}
	else if (next[3] == 3 && y == 0 && x < next[1]) {
		return next[0];
	}
	else if (next[3] == 4 && x == 0 && y < next[2]) {
		return next[0];
	}
	else if (next[3] == 5 && x == -y && x > next[1]) {
		return next[0];
	}


	return -1;
}

//Rotate function
//return values: (-1) - ack (not pointed), 1 - completed for next1, 2 - completed for next2
//
int Caterpillar1Particle::rotate() {

	if (_state == State::Root) {
		//send rotate to next follower particloe
		Caterpillar1Particle* temp = &nbrAtLabel(_precedingBondedNbr);
		while (temp->_state != State::Follower) {
			if (temp->hasNbrAtLabel(temp->_precedingBondedNbr))
				temp = &(temp->nbrAtLabel(temp->_precedingBondedNbr));
		}
		temp->_rotate_status_head = 1; //rotate status is rotate
		int r_stat = temp->rotate();

		//Ack
		if (_rotate_status_tail == 2 && hasNbrAtLabel(_precedingBondedNbr)) {
			nbrAtLabel(_precedingBondedNbr)._rotate_status_head = 1; //send rotate
			int r_stat = nbrAtLabel(_precedingBondedNbr).rotate();

		}

		//Completed
		if (_rotate_status_tail == 3) {
			_rotate_status_tail = -1;
			_rotate_status_head = -1;

			return 3;
		}

	}

	else if (_state == State::Follower) {
		if (_rotate_status_head == 1) {
			if (_precedingBondedNbr != -1 && hasNbrAtLabel(_precedingBondedNbr)) {
				nbrAtLabel(_precedingBondedNbr)._rotate_status_head = 1;
				int r_stat = nbrAtLabel(_precedingBondedNbr).rotate();
			}
			else {
				//Rotate once clockwise
				if (isExpanded()) {
					if (_precedingBondedNbr == -1) {
						contractTail();

					}
					else {
						for (int label : tailLabels()) {
							if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr != -1
								&& pointsAtMe(nbrAtLabel(label), nbrAtLabel(label)._followingBondedNbr)) {
								if (canPull(label)) {
									nbrAtLabel(label)._followingBondedNbr =
										dirToNbrDir(nbrAtLabel(label), (tailDir() + 3) % 6);
									pull(label);
								}
								break;
							}
						}
					}
					cout << _precedingBondedNbr << endl;

				}

				int expandDir = (_followingBondedNbr + 1) % 6;
				if (canExpand(expandDir))
					expand(expandDir);
				if (isExpanded()) {
					if (_precedingBondedNbr == -1)
						contractTail();
					else {
						for (int label : tailLabels()) {
							if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr != -1
								&& pointsAtMe(nbrAtLabel(label), nbrAtLabel(label)._followingBondedNbr)) {
								if (canPull(label)) {
									nbrAtLabel(label)._followingBondedNbr =
										dirToNbrDir(nbrAtLabel(label), (tailDir() + 3) % 6);
									pull(label);
								}
								break;
							}
						}
					}

				}
				if (isPointedByNextNode(_nextOne, head.x, head.y) != -1 || isPointedByNextNode(_nextTwo, head.x, head.y) != -1) {
					if (hasNbrAtLabel(_followingBondedNbr)) {
						nbrAtLabel(_followingBondedNbr)._rotate_status_tail = 3;//Complete

					}
					return 3;
				}
				else {
					if (hasNbrAtLabel(_followingBondedNbr)) {
						nbrAtLabel(_followingBondedNbr)._rotate_status_tail = 2;//Ack

					}
					return 2;
				}
			}
		}
		if (isExpanded())
			contractTail();
		//if receives Ack
		if (_rotate_status_tail == 2) {

			//Rotate once
			if (isExpanded()) {
				if (_precedingBondedNbr == -1)
					contractTail();
				else {
					for (int label : tailLabels()) {
						if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr != -1
							&& pointsAtMe(nbrAtLabel(label), nbrAtLabel(label)._followingBondedNbr)) {
							if (canPull(label)) {
								nbrAtLabel(label)._followingBondedNbr =
									dirToNbrDir(nbrAtLabel(label), (tailDir() + 3) % 6);
								pull(label);
							}
							break;
						}
					}
				}

			}
			int expandDir = (_followingBondedNbr + 1) % 6;
			if (canExpand(expandDir))
				expand(expandDir);
			//pull if any connected particle is present
			if (isExpanded()) {
				if (_precedingBondedNbr == -1)
					contractTail();
				else {
					for (int label : tailLabels()) {
						if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr != -1
							&& pointsAtMe(nbrAtLabel(label), nbrAtLabel(label)._followingBondedNbr)) {
							if (canPull(label)) {
								nbrAtLabel(label)._followingBondedNbr =
									dirToNbrDir(nbrAtLabel(label), (tailDir() + 3) % 6);
								pull(label);
							}
							break;
						}
					}
				}

			}

			//Next or direction
			if (isPointedByNextNode(_nextOne, head.x, head.y) != -1 || isPointedByNextNode(_nextTwo, head.x, head.y) != -1) {
				_rotate_status_tail = 3;
				//Write next or direction somehow
				if (hasNbrAtLabel(_precedingBondedNbr)) {
					nbrAtLabel(_precedingBondedNbr)._rotate_status_head = 1;
					int r_stat = nbrAtLabel(_precedingBondedNbr).rotate();
				}
				return 3;
			}
			else {
				if (hasNbrAtLabel(_followingBondedNbr)) {
					nbrAtLabel(_followingBondedNbr)._rotate_status_tail = 2; //Ack

				}
				return 2;
			}

		}
		if (isExpanded())
			contractTail();
		//if receives Completed
		if (_rotate_status_tail == 3) { //completed
			if (hasNbrAtLabel(_followingBondedNbr)) {
				nbrAtLabel(_followingBondedNbr)._rotate_status_tail = 3;

			}

		}
		if (isExpanded())
			contractTail();
	}

	return 3;
}


QString Caterpillar1Particle::inspectionText() const {
	QString text;
	text += "Global Info:\n";
	text += "  head: (" + QString::number(head.x) + ", "
		+ QString::number(head.y) + ")\n";
	text += "  orientation: " + QString::number(orientation) + "\n";
	text += "  globalTailDir: " + QString::number(globalTailDir) + "\n\n";
	text += "Local Info:\n";
	text += "  state: ";
	text += [this]() {
		switch (_state) {
		case State::Leader:   return "leader\n";
		case State::Follower: return "follower\n";
		}
		return "no state\n";
	}();
	text += [this]() {
		switch (_color) {
		case Color::Red:    return "red\n";
		case Color::Green:	return "green\n";
		case Color::Blue:   return "blue\n";

		}
		return "no color\n";
	}();
	text += "  partnerTail: " + QString::number(_precedingBondedNbr);

	return text;
}

Caterpillar1Particle& Caterpillar1Particle::nbrAtLabel(int label) const {
	return AmoebotParticle::nbrAtLabel<Caterpillar1Particle>(label);
}

Caterpillar1Particle::Color Caterpillar1Particle::getRandColor() const {
	// Randomly select an integer and return the corresponding color via casting.
	return static_cast<Color>(2);
}

Caterpillar1Particle::Color Caterpillar1Particle::getRootColor() const {
	// Randomly select an integer and return the corresponding color via casting.
	return static_cast<Color>(1);
}

Caterpillar1Particle::Color Caterpillar1Particle::getLeaderColor() const {
	// Randomly select an integer and return the corresponding color via casting.
	return static_cast<Color>(0);
}



Caterpillar1System::Caterpillar1System(unsigned int numParticles) {


	std::set<Node> occupied;
	unsigned int numParticlesAdded = 0;
	int x = 2;
	int y = 2;
	Node leaderNode(x, y);
	numParticlesAdded++;
	Caterpillar1Particle* leader =
		new Caterpillar1Particle(leaderNode, -1, 0, *this,
			Caterpillar1Particle::State::Leader);
	insert(leader);
	leader->_followingBondedNbr = -1;
	leader->_precedingBondedNbr = 3;
	//leader->_color = BallroomDemoParticle::Color BallroomDemoParticle::getLeaderColor();
	Node prev = leaderNode;
	occupied.insert(leaderNode);
	while (numParticlesAdded < numParticles) {
		// Choose an (x,y) position within the rhombus for the Leader and a random
		// adjacent node for its Follower partner.

		int followerDir = 3;
		//occupied.back()->_partnerTail = followerDir;
		Node followerNode = prev.nodeInDir(followerDir);
		Caterpillar1Particle* follower =
			new Caterpillar1Particle(followerNode, -1, 0, *this,
				Caterpillar1Particle::State::Follower);
		follower->_followingBondedNbr = follower->globalToLocalDir((followerDir + 3) % 6);
		if (numParticles - numParticlesAdded == 1) {
			follower->_precedingBondedNbr = -1;
			insert(follower);
			occupied.insert(followerNode);
			break;
		}
		else {
			follower->_precedingBondedNbr = 3;
		}
		insert(follower);
		occupied.insert(followerNode);
		prev = followerNode;
		numParticlesAdded++;


	}
}
