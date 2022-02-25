
#include "alg/demo/caterpillar1.h"
//#include "stdafx.h"

#include <QDebug>


using namespace std;

std::vector<int> _nextOne; //contains {number i in next_i, x, y, label pointed to}
std::vector<int> _nextTwo; //contains {number i in next_i, x, y, label pointed to}
std::vector<std::vector<int>> _nextNodes; //each next node contains (x,y,label) where x and y are the node that is being pointed to (not by) and label is direction
std::vector<std::vector<int>> _firstNode; //each next node contains (x,y,label) where x and y are the node that is being pointed to (not by) and label is direction
std::vector<std::vector<int>> _directionNodes; //each next node contains (x,y,label) where x and y are the node that is being pointed to (not by) and label is direction

int layer = 1;
std::vector<int> next_node_to_insert;
std::vector<int> first_node_to_insert;
std::vector<int> dir_node_to_insert;

Caterpillar1Particle::Caterpillar1Particle(const Node head,
	const int globalTailDir,
	const int orientation,
	AmoebotSystem &system,
	State state)
	: AmoebotParticle(head, globalTailDir, orientation, system),
	_state(state),
	_followingBondedNbr(-1),
	_precedingBondedNbr(-1) {
	_color = getColor(_state);
}

void Caterpillar1Particle::activate() {
	//--------------------- LEADER STATE ---------------------
	if (_state == State::Leader) {
		//Leader state

		qDebug() << "----------LEaDER = " << _name << endl;
		_color = getColor(_state);
		first_node_to_insert = getNodeFromLabel(head.x, head.y, 3);
		_firstNode.push_back({ first_node_to_insert[0], first_node_to_insert[1], 3 });
		if (hasNbrAtLabel(_precedingBondedNbr)) {
			//qDebug() << "Root is x,y=" << endl;
			//qDebug() << nbrAtLabel(_precedingBondedNbr).head.x << endl;
			//qDebug() << nbrAtLabel(_precedingBondedNbr).head.y << endl;
			nbrAtLabel(_precedingBondedNbr)._receive_head = 0; //root activation
			//nbrAtLabel(_precedingBondedNbr)._state = State::Root;
		}

		if (_receive_tail == 0) //receives terminate
			_state = State::Terminated;
	}

	//--------------------- ROOT STATE ---------------------
	else if (_state == State::Root) {
		qDebug() << "----------Root = " << _name << endl;

		

		_color = getColor(_state);

		//pointed by FirstNode1
		//qDebug() << "x,y= " << endl;
		//qDebug() << head.x << endl;
		//qDebug() <<head.y << endl;
		//qDebug() << _firstNode << endl;
		qDebug() << "Pointed by first node o/p = " << endl;

		qDebug() << isPointedByFirstNode(_firstNode, head.x, head.y) << endl;
		if (isPointedByFirstNode(_firstNode, head.x, head.y) != -1 && layer == 1) {
			qDebug() << "----------root - pointed by FirstNode1" << endl;
			next_node_to_insert = getNodeFromLabel(head.x, head.y, 2);

			_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], 2});
			//_nextOne = { 1, head.x, head.y, 2 };
			//_nextTwo = { 1, head.x, head.y, 5 };

			_receive_head = 1; //init rotate
			rotate();
			qDebug() << "one rotate complete" << endl;
			qDebug() << "next nodes: " << _nextNodes << endl;
			qDebug() << "dir nodes: " << _directionNodes << endl;

			_nextNodes.clear();
			_directionNodes.clear();
			qDebug() << "after clearing, next nodes: " << _nextNodes << endl;
			qDebug() << "after clearing, dir nodes: " << _directionNodes << endl;
			next_node_to_insert = getNodeFromLabel(head.x, head.y, 1);
			_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], 1});

			next_node_to_insert = getNodeFromLabel(head.x, head.y, 5);
			_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], 5});

			//_nextOne = { 1, head.x, head.y, 1 };
			//_nextTwo = { 1, head.x, head.y, 5 };
			_receive_head = 1; //init rotate
			rotate();
			_directionNodes.clear();

		}
		
		//pointed by FirstNodei, where i > 1
		
		else if (isPointedByFirstNode(_firstNode, head.x, head.y) != -1 && layer>1) {
			qDebug() << "----------root - pointed by FirstNodei where i>1" << endl;
			next_node_to_insert = getNodeFromLabel(head.x, head.y, 2);
			_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], 2 });

			next_node_to_insert = getNodeFromLabel(head.x, head.y, 5);
			_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], 5 });

			//_nextOne = { isPointedByFirstNode(_firstNode, head.x, head.y), head.x, head.y, 2 };
			//_nextTwo = { isPointedByFirstNode(_firstNode, head.x, head.y), head.x, head.y, 5 };

			_receive_head = 1;
			rotate();
			_directionNodes.clear();


		}
		//pointed by only one Nexti
		else if(isPointedByNextNode(_nextNodes, head.x, head.y) == 1) { //pointed only by one nexti
			qDebug() << "----------root - pointed by only one next i" << endl;
			
			for (int label = 0; label <=5; label++) {
				
				qDebug() << "Label = " << label << endl;
				qDebug() << "hasNbrAtLabel(label): " << hasNbrAtLabel(label) << endl;
				if (label == _precedingBondedNbr) {
					int a = (label + 1) % 6;
					int b = label == 0 ? 5 : label - 1;
					qDebug() << "a, b = " << a << ", " << b << endl;
					if ((hasNbrAtLabel(a) && (nbrAtLabel(a)._state == State::Retired || nbrAtLabel(a)._state == State::Leader) && a != _followingBondedNbr) || (hasNbrAtLabel(b) && (nbrAtLabel(b)._state == State::Retired || nbrAtLabel(b)._state == State::Leader) && b != _followingBondedNbr)) {
						next_node_to_insert = getNodeFromLabel(head.x, head.y, label);
						_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], label });
						break;
					}
				}

				if (!hasNbrAtLabel(label) && _precedingBondedNbr!=-1) {
					int a = (label + 1) % 6;
					int b = label == 0 ? 5 : label - 1;
					qDebug() << "a, b = " << a << ", " << b << endl;
					if ((hasNbrAtLabel(a) && (nbrAtLabel(a)._state == State::Retired || nbrAtLabel(a)._state == State::Leader) && a != _followingBondedNbr) || (hasNbrAtLabel(b) && (nbrAtLabel(b)._state == State::Retired || nbrAtLabel(b)._state == State::Leader) && b != _followingBondedNbr)) {
						next_node_to_insert = getNodeFromLabel(head.x, head.y, label);
						_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], label });
						break;
					}
				}
			}

			if (_precedingBondedNbr != -1 && hasNbrAtLabel(_precedingBondedNbr)) {
				if (isPointedByNextNode(_nextNodes, nbrAtLabel(_precedingBondedNbr).head.x, nbrAtLabel(_precedingBondedNbr).head.y) == 0) {
					_receive_head = 1;
					rotate();
					_directionNodes.clear();

				}
			}
		}
		//pointed by two Nexti
		else {
			qDebug() << "----------root - pointed by two next i" << endl;
			//int pointed = isPointedByNextNode(_nextOne, head.x, head.y);
			
			first_node_to_insert = getNodeFromLabel(head.x, head.y, 3);
			_firstNode.push_back({ first_node_to_insert[0], first_node_to_insert[1], 3 });
			//_firstNode.push_back({ (layer + 1) % 4, head.x, head.y, 3 });
			layer = (layer + 1) % 4;
		}

		//Forward root activation to preceding bonded nbr
		if (_precedingBondedNbr!=-1 && hasNbrAtLabel(_precedingBondedNbr)) {
			qDebug() << "----------root - forwarding root activation to preceding nbr" << endl;
			nbrAtLabel(_precedingBondedNbr)._receive_head = 0;
			//nbrAtLabel(_precedingBondedNbr)._state = State::Root;
		}
		_state = State::Retired;


	}

	//--------------------- FOLLOWER STATE ---------------------
	else if (_state == State::Follower) {
		_color = getColor(_state);
		if (_receive_head == 0) {
			qDebug() << "Follower " << _name << " becoming root" << endl;
			_state = State::Root;
		}
	}

	//--------------------- RETIRED STATE ---------------------
	else if (_state == State::Retired) {
		_color = getColor(_state);
		qDebug() << "Retired state = " << _name << endl;
		qDebug() << "_precedingBondedNbr == -1: " << (_precedingBondedNbr == -1) << " (_followingBondedNbr): " << _followingBondedNbr << endl;
		if (_precedingBondedNbr == -1 && hasNbrAtLabel(_followingBondedNbr)) {
			qDebug() << "Tail particle retired, sending terminate" << endl;
			nbrAtLabel(_followingBondedNbr)._receive_tail = 0; //terminate
			_state = State::Terminated;
		}

		if (_receive_tail == 0) {
			qDebug() << "Received termination, forwarding to following bonded nbr" << endl;
			if (hasNbrAtLabel(_followingBondedNbr)) {
				nbrAtLabel(_followingBondedNbr)._receive_tail = 0; //terminate
			}
			_state = State::Terminated;
		}
	}
	//--------------------- TERMINATED STATE ---------------------
	else if (_state == State::Terminated) {
		_color = getColor(_state);
		qDebug() << "Terminated state = " << _name << endl;
	}

}





int Caterpillar1Particle::headMarkColor() const {
	switch (_color) {
	case Color::Red:    return 0xff0000;
	case Color::Yellow:	return 0xffff00;
	case Color::Green:	return 0x00ff00;
	case Color::Blue:   return 0x0000ff;
	case Color::Black:	return 0x000000;
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
		res.push_back(x + 1);
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

int Caterpillar1Particle::getLabelFromNode(int x_current, int y_current, int x_pre, int y_pre) const { //Current -> pre label
	if (y_current == y_pre && x_current < x_pre) {
		return 0;
	}
	else if (x_current == x_pre && y_current < y_pre) {
		return 1;
	}
	else if (y_current < y_pre && x_current > x_pre) {
		return 2;
	}
	else if (y_current == y_pre && x_current > x_pre) {
		return 3;
	}
	else if (x_current == x_pre && y_current > y_pre) {
		return 4;
	}
	else if (y_current > y_pre && x_current < x_pre) {
		return 5;
	}
}

bool Caterpillar1Particle::checkIfNodeIsNbr(int x1, int y1, int x2, int y2) const {
	if (abs(x1 - x2) > 1 || abs(y1 - y1) > 1) {
		return false;
	}
	return true;
}


int Caterpillar1Particle::labelPointing(std::vector<std::vector<int>> nextNode, std::vector<std::vector<int>> directionNode, int x, int y) const {
	for (int i = directionNode.size() - 1; i >= 0; i--) {
		if (directionNode[i][0] == x && directionNode[i][1] == y)
			return directionNode[i][2]; //label pointing
	}

	for (int i = nextNode.size() - 1; i >= 0; i--) {
		if (nextNode[i][0] == x && nextNode[i][1] == y)
			return nextNode[i][2]; //label pointing
	}

	
	return -1;
}


int Caterpillar1Particle::isPointedByFirstNode(std::vector<std::vector<int>> firstNode, int x, int y) const {
	
	for (int i = 0; i < firstNode.size(); i++) {
		if (firstNode[i][0] == x && firstNode[i][1] == y)
			return firstNode[i][2];
	}
		/*if (firstNode[i][3] == 0 && y == firstNode[i][2] && x > firstNode[i][1]) {
			return (firstNode[i][0]);
		}
		else if (firstNode[i][3] == 1 && x == firstNode[i][1] && y > firstNode[i][2]) {
			return firstNode[i][0];
		}
		else if (firstNode[i][3] == 2 && x < firstNode[i][1] && y > firstNode[i][2]) {
			return firstNode[i][0];
		}
		else if (firstNode[i][3] == 3 && y == firstNode[i][2] && x < firstNode[i][1]) {
			return firstNode[i][0];
		}
		else if (firstNode[i][3] == 4 && x == firstNode[i][1] && y < firstNode[i][2]) {
			return firstNode[i][0];
		}
		else if (firstNode[i][3] == 5 && x > firstNode[i][1] && y < firstNode[i][2]) {
			return firstNode[i][0];
		}*/

	


	return -1;
}


int Caterpillar1Particle::isPointedByNextNode(std::vector<std::vector<int>> nextNode, int x, int y) const {
	//next i,x,y,label
	int count = 0;
	for (int i = 0; i < nextNode.size(); i++) {
		if (nextNode[i][0] == x && nextNode[i][1] == y)
			count = count + 1;
	}
	/*if (next[3] == 0 && y == next[2] && x > next[1]) {
		return next[0];
	}
	else if (next[3] == 1 && x == next[1] && y > next[2]) {
		return next[0];
	}
	else if (next[3] == 2 && x < next[1] && y > next[2]) {
		return next[0];
	}
	else if (next[3] == 3 && y == next[2] && x < next[1]) {
		return next[0];
	}
	else if (next[3] == 4 && x == next[1] && y < next[2]) {
		return next[0];
	}
	else if (next[3] == 5 && x > next[1] && y < next[2]) {
		return next[0];
	}

	*/
	return count;
}

//Rotate function
//return values: (-1) - ack (not pointed), 1 - completed for next1, 2 - completed for next2
//
void Caterpillar1Particle::rotate() {

	//----------------- ROOT STATE -----------------
	if (_state == State::Root) {
		//send rotate to next follower particle
		if (_receive_head == 1) {
			_receive_head = -1;
			qDebug() << "Rotate - root state - entered by " << _name << endl;
			Caterpillar1Particle* temp = &nbrAtLabel(_precedingBondedNbr);
			while (temp->_state != State::Follower) {
				if (temp->hasNbrAtLabel(temp->_precedingBondedNbr))
					temp = &(temp->nbrAtLabel(temp->_precedingBondedNbr));
			}
			temp->_rotate_status_head = 1; //rotate status is rotate
			qDebug() << "Rotate - sending by " << _name << endl;
			temp->rotate();
			//temp->_rotate_status_head = -1;
			//_receive_h
		}

		//Ack
		if (_rotate_status_tail == 2 && _precedingBondedNbr!=-1 && hasNbrAtLabel(_precedingBondedNbr)) {
			qDebug() << "Rotate - root - received ack at  " << _name << endl;
			qDebug() << "Rotate - sending rotate to preceding bonded nbr from " << _name << endl;
			nbrAtLabel(_precedingBondedNbr)._rotate_status_head = 1; //send rotate
			nbrAtLabel(_precedingBondedNbr).rotate();
			//nbrAtLabel(_precedingBondedNbr)._rotate_status_head = -1;
			//qDebug() << "Rotate - root - while ack" << endl;
			//qDebug() << "_precedingBondedNbr = " << _precedingBondedNbr << endl;
		}

		//Completed
		if (_rotate_status_tail == 3) {
			qDebug() << "Rotate - root - received completed at " << _name << endl;
			_rotate_status_tail = -1;
			_rotate_status_head = -1;

			return;
		}
		//rotate();
	}

	//---------------- FOLLOWER STATE ----------------
	else if (_state == State::Follower) {
		qDebug() << "Rotate - follower state - entered by " << _name << endl;

		//Follower receives Rotate
		if (_rotate_status_head == 1) {
			_rotate_status_head = -1;
			qDebug() << "Rotate - follower - received rotate " << _name << endl;
			//if not the tail particle
			if (_precedingBondedNbr != -1 && hasNbrAtLabel(_precedingBondedNbr)) {
				qDebug() << "Rotate - follower - sending rotate to preceding bonded nbr " << _name << endl;
				nbrAtLabel(_precedingBondedNbr)._rotate_status_head = 1;
				nbrAtLabel(_precedingBondedNbr).rotate();
			}
			//f tail particle
			else {

				//Rotate once clockwise
				qDebug() << "Rotate - follower - rotate once clockwise started " << _name << endl;
				
				//----------------------------- EXPAND CODE ---------------------------------------------------------------------------------------------------------------------
				//--------------------------------------------------------------------------------------------------------------------------------------------------------------
				int expandDir = (_followingBondedNbr + 1) % 6;
				qDebug() << "Following bonded nbr = " << _followingBondedNbr << endl;
				qDebug() << "before expanding, has Following bonded nbr = " << hasNbrAtLabel(_followingBondedNbr) << endl;
				qDebug() << "tail dir before expanding = " << tailDir() << endl;
				//int _new_dir = nbrAtLabel(_followingBondedNbr)._precedingBondedNbr != 0 ? abs(nbrAtLabel(_followingBondedNbr)._precedingBondedNbr - 1) % 6 : 5;
				//qDebug() << "new dir before: " << _new_dir << endl;
				int _new_dir = getLabelFromNode(nbrAtLabel(_followingBondedNbr).head.x, nbrAtLabel(_followingBondedNbr).head.y, getNodeFromLabel(head.x, head.y, expandDir)[0], getNodeFromLabel(head.x, head.y, expandDir)[1]);
				qDebug() << "new dir after = " << _new_dir << endl;
				//nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
				vector<int> nbrNode = getNodeFromLabel(head.x, head.y, expandDir);
				qDebug() << "new dir = " << _new_dir << endl;
				//int new_dir_1 = getLabelFromNode();
				vector<int> pBN;
				qDebug() << "prec bonded nbr = " << _precedingBondedNbr << endl;
				if (_precedingBondedNbr != -1 && hasNbrAtLabel(_precedingBondedNbr))
					pBN = { nbrAtLabel(_precedingBondedNbr).head.x, nbrAtLabel(_precedingBondedNbr).head.y };
				//qDebug() << "pbn =  " << pBN << endl;
				if (canExpand(expandDir))
					expand(expandDir);

				_followingBondedNbr = (_new_dir + 3) % 6;

				//nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
				qDebug() << "tail dir = " << tailDir() << endl;
				qDebug() << "after expandingFollowing bonded nbr = " << hasNbrAtLabel(_followingBondedNbr) << endl;
				if (isExpanded()) {
					//If it is the last particle
					if (_precedingBondedNbr == -1) {

						contractTail();
						qDebug() << "aftercontracting Following bonded nbr = " << hasNbrAtLabel(_followingBondedNbr) << " at " << _name << endl;
						qDebug() << "Position of " << _name << " is " << head.x << ", " << head.y << endl;
						qDebug() << "Nexts: " << _nextOne << " and " << _nextTwo << endl;
						nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
						_followingBondedNbr = (_new_dir + 3) % 6;


					}
					//If it is not the last particle
					else {
						//If the transition doesnot change the connectivity
						qDebug() << "check if node is nbr = " << checkIfNodeIsNbr(pBN[0], pBN[1], nbrNode[0], nbrNode[1]) << endl;
						if (checkIfNodeIsNbr(pBN[0], pBN[1], nbrNode[0], nbrNode[1])) { //if prec bonded nbr is a neighbor to new expanded position
							qDebug() << "No need to pull at " << _name << endl;
							if (isExpanded())
								contractTail();
							if (hasNbrAtLabel(_followingBondedNbr))
								nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir; //right particle pBN changes
							_precedingBondedNbr = getLabelFromNode(head.x, head.y, pBN[0], pBN[1]);
							nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (_precedingBondedNbr + 3) % 6;
							_followingBondedNbr = (_new_dir + 3) % 6;
						}
						else {

							//------------------------------------------------------------------------------
						//START FROM HERE
							qDebug() << "Pull mechanism starts at " << _name << endl;
							vector<int> tailNode = getNodeFromLabel(head.x, head.y, tailDir());
							vector<int> pBN;
							vector<int> fBN;
							if (_precedingBondedNbr != -1) {

								pBN = getNodeFromLabel(tailNode[0], tailNode[1], _precedingBondedNbr);
								qDebug() << "pbn = " << pBN << endl;
							}
							if (_followingBondedNbr != -1) {
								fBN = getNodeFromLabel(head.x, head.y, _followingBondedNbr);
								qDebug() << "fbn = " << fBN << endl;

							}
							qDebug() << "current pos of " << _name << " = " << head.x << ", " << head.y << endl;
							qDebug() << "tail dir of " << _name << " = " << tailNode[0] << ", " << tailNode[1] << endl;
							qDebug() << "pbn of " << _name << " = " << pBN << endl;
							qDebug() << "fbn of " << _name << " = " << fBN << endl;

							//nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (tailDir() + 3) % 6;
							//_precedingBondedNbr = tailDir();
							//if (canPull(_precedingBondedNbr))
							//int labelToPull = getLabelFromNode(tailNode[0], tailNode[1], pBN[0], pBN[1]);
							//qDebug() << "label to pull " << labelToPull << endl;
							//qDebug() << "can pull?  " << canPull(labelToPull) << endl;


							for (int label : tailLabels()) {
								qDebug() << "label = " << label << endl;
								qDebug() << "_precedingBondedNbr = " << _precedingBondedNbr << endl;
								if (hasNbrAtLabel(label))
									qDebug() << "nbrAtLabel(label)._followingBondedNbr = " << nbrAtLabel(label)._followingBondedNbr << endl;

								if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr == (_precedingBondedNbr + 3) % 6) {
									qDebug() << "can pull this label: " << canPull(label) << endl;
									if (canPull(label)) {
										_precedingBondedNbr = tailDir();
										pull(label);
										break;
									}
								}
							}
							//_precedingBondedNbr = tailDir();
							qDebug() << "prec bonded nbr becomes " << _precedingBondedNbr << endl;




							//if (canPull(labelToPull))
								//pull(labelToPull);
							nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (_precedingBondedNbr + 3) % 6;
							if (hasNbrAtLabel(_followingBondedNbr)) {
								nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
								_followingBondedNbr = (_new_dir + 3) % 6;
							}
							Caterpillar1Particle *temp = &nbrAtLabel(_precedingBondedNbr);

							while ((temp->_precedingBondedNbr) != -1) {
								qDebug() << "Pulling continues at " << temp->_name << endl;
								vector<int> tailNode = temp->getNodeFromLabel(temp->head.x, temp->head.y, temp->tailDir());
								vector<int> pBN = temp->getNodeFromLabel(tailNode[0], tailNode[1], temp->_precedingBondedNbr);
								vector<int> fBN = temp->getNodeFromLabel(temp->head.x, temp->head.y, temp->_followingBondedNbr);
								qDebug() << "current pos of " << temp->_name << " = " << temp->head.x << ", " << temp->head.y << endl;
								qDebug() << "tail dir of " << temp->_name << " = " << tailNode[0] << ", " << tailNode[1] << endl;
								qDebug() << "pbn of " << temp->_name << " = " << pBN << endl;
								qDebug() << "fbn of " << temp->_name << " = " << fBN << endl;



								if (checkIfNodeIsNbr(temp->head.x, temp->head.y, pBN[0], pBN[1])) { //if prec bonded nbr is a neighbor to new expanded position
									temp->contractTail();
									temp->_followingBondedNbr = getLabelFromNode(temp->head.x, temp->head.y, fBN[0], fBN[1]);
									if (temp->hasNbrAtLabel(temp->_followingBondedNbr))
										temp->nbrAtLabel(temp->_followingBondedNbr)._precedingBondedNbr = getLabelFromNode(fBN[0], fBN[1], temp->head.x, temp->head.y); //right particle pBN changes
									temp->_precedingBondedNbr = getLabelFromNode(temp->head.x, temp->head.y, pBN[0], pBN[1]);
									temp->nbrAtLabel(temp->_precedingBondedNbr)._followingBondedNbr = (temp->_precedingBondedNbr + 3) % 6;
								}
								else {
									//temp->nbrAtLabel(temp->_precedingBondedNbr)._followingBondedNbr = (temp->tailDir() + 3) % 6;

									for (int label : temp->tailLabels()) {
										qDebug() << "label = " << label << endl;
										qDebug() << "_precedingBondedNbr = " << temp->_precedingBondedNbr << endl;
										if (temp->hasNbrAtLabel(label))
											qDebug() << "nbrAtLabel(label)._followingBondedNbr = " << temp->nbrAtLabel(label)._followingBondedNbr << " at " << temp->_name << endl;

										if (temp->hasNbrAtLabel(label) && temp->nbrAtLabel(label)._followingBondedNbr == (temp->_precedingBondedNbr + 3) % 6) {
											qDebug() << "can pull this label: " << temp->canPull(label) << ", tail dir is " << temp->tailDir() << endl;
											if (temp->canPull(label)) {
												temp->_precedingBondedNbr = temp->tailDir();
												temp->pull(label);
												qDebug() << "prec bonded nbr becomes (inside for loop, before break stmt): " << temp->_precedingBondedNbr << endl;
											}
											break;
										}
									}

									qDebug() << "prec bonded nbr becomes " << temp->_precedingBondedNbr << endl;
									qDebug() << "fBN = " << fBN << " " << temp->head.x << " " << temp->head.y << endl;
									qDebug() << "current pos of " << temp->_name << " = " << temp->head.x << ", " << temp->head.y << endl;

									//temp->_precedingBondedNbr = temp->tailDir();
									//vector<int> tailNode = getNodeFromLabel(temp->head.x, temp->head.y, temp->tailDir());
									//vector<int> fBN = getNodeFromLabel(tailNode[0], tailNode[1], temp->_followingBondedNbr);
									temp->_followingBondedNbr = getLabelFromNode(temp->head.x, temp->head.y, fBN[0], fBN[1]);
									qDebug() << "fBN label becomes = " << temp->_followingBondedNbr << " at " << temp->_name << endl;

									if (temp->hasNbrAtLabel(temp->_followingBondedNbr)) {

										temp->nbrAtLabel(temp->_followingBondedNbr)._precedingBondedNbr = (temp->_followingBondedNbr + 3) % 6;
									}
									//int labelToPull = getLabelFromNode(tailNode[0], tailNode[1], pBN[0], pBN[1]);
									//if (temp->canPull(labelToPull))
										//temp->pull(labelToPull);
									//if (temp->canPull(temp->_precedingBondedNbr)) {
									//temp->pull(temp->_precedingBondedNbr);
									//}
									temp->nbrAtLabel(temp->_precedingBondedNbr)._followingBondedNbr = (temp->_precedingBondedNbr + 3) % 6;

									temp = &temp->nbrAtLabel(temp->_precedingBondedNbr);


									/*for (int label : tailLabels()) {
										if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr != -1
											&& pointsAtMe(nbrAtLabel(label), nbrAtLabel(label)._followingBondedNbr)) {
											if (canPull(label)) {
												nbrAtLabel(label)._followingBondedNbr =
													dirToNbrDir(nbrAtLabel(label), (tailDir() + 3) % 6);

												pull(label);
											}
											break;
										}
									}*/
								}

							}
							qDebug() << "Pull stops at " << temp->_name << endl;
							if (temp->isExpanded()) {
								temp->contractTail();
							}
						}

					}
				
}
				qDebug() << "Expansion ends at " << _name << endl;
				//----------------------------------- END OF EXPAND ---------------------------------------------------------------------------------------
				//-----------------------------------------------------------------------------------------------------------------------------------------


				if (isPointedByNextNode(_nextNodes, head.x, head.y) != 0 || isPointedByNextNode(_directionNodes, head.x, head.y) != 0) {
					qDebug() << "Rotate - follower - after rotation " << _name << " pointed by next " << endl;
					//qDebug() << isPointedByNextNode(_nextOne, head.x, head.y) << _name << endl;
					//qDebug() << isPointedByNextNode(_nextTwo, head.x, head.y) << _name << endl;
					qDebug() << "pbn and fbn at " << _name << " are " << _precedingBondedNbr << " and " << _followingBondedNbr << endl;

					if (hasNbrAtLabel(_followingBondedNbr)) {
						qDebug() << "sending complete from " << _name << endl;
						nbrAtLabel(_followingBondedNbr)._rotate_status_tail = 3;//Complete
						return;
					}
					return;
				}
				else {
					qDebug() << "Rotate - follower - not pointed by nexts - sending ack to following bonded nbr " << _name << endl;
					qDebug() << "following bonded nbr of " << _name << " is " << _followingBondedNbr << endl;
					qDebug() << "Has following bonded nbr " << hasNbrAtLabel(_followingBondedNbr) << endl;
					qDebug() << "pbn and fbn at " << _name << " are " << _precedingBondedNbr << " and " << _followingBondedNbr << endl;

					if (hasNbrAtLabel(_followingBondedNbr)) {
						qDebug() << "Sending ack from " << _name << endl;
						nbrAtLabel(_followingBondedNbr)._rotate_status_tail = 2;//Ack
						//nbrAtLabel(_followingBondedNbr).rotate();
						nbrAtLabel(_followingBondedNbr).rotate();
					}
					return;
				}
			}
		}
		if (isExpanded())
			contractTail();
		//if receives Ack
		if (_rotate_status_tail == 2) {
			qDebug() << "Rotate - follower - received ack " << _name << endl;
			//Rotate once
			/*if (isExpanded()) {
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

			}*/

			
			
			//----------------------------- EXPAND CODE ---------------------------------------------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------------------------------------------------------------------------------------
			/*if (isPointedByNextNode(_nextOne, head.x, head.y) != -1 || isPointedByNextNode(_nextTwo, head.x, head.y) != -1) {
				//_rotate_status_tail = 3;
				//Write next or direction somehow
				qDebug() << "pointed by next, sending rotate to prec bonded nbr from " << _name << endl;
					qDebug() << "pbn and fbn at " << _name << " are " << _precedingBondedNbr << " and " << _followingBondedNbr << endl;

					if (hasNbrAtLabel(_precedingBondedNbr)) {
						nbrAtLabel(_precedingBondedNbr)._rotate_status_head = 1;
							nbrAtLabel(_precedingBondedNbr).rotate();
					}
				//return 3;
			}
			*/

			int expandDir = (_followingBondedNbr + 1) % 6;
			qDebug() << "Following bonded nbr = " << _followingBondedNbr << endl;
			qDebug() << "before expanding, has Following bonded nbr = " << hasNbrAtLabel(_followingBondedNbr) << endl;
			qDebug() << "tail dir before expanding = " << tailDir() << endl;
			//int _new_dir = nbrAtLabel(_followingBondedNbr)._precedingBondedNbr != 0 ? abs(nbrAtLabel(_followingBondedNbr)._precedingBondedNbr - 1) % 6 : 5;
			
			int _new_dir = getLabelFromNode(nbrAtLabel(_followingBondedNbr).head.x, nbrAtLabel(_followingBondedNbr).head.y, getNodeFromLabel(head.x, head.y, expandDir)[0], getNodeFromLabel(head.x, head.y, expandDir)[1]);
			qDebug() << "new dir after = " << _new_dir << endl;
			//nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
			vector<int> nbrNode = getNodeFromLabel(head.x, head.y, expandDir);
			//int new_dir_1 = getLabelFromNode();
			vector<int> pBN;
			if (hasNbrAtLabel(_precedingBondedNbr))
				pBN = { nbrAtLabel(_precedingBondedNbr).head.x, nbrAtLabel(_precedingBondedNbr).head.y };
			qDebug() << "pbn =  " << pBN << endl;
			if (canExpand(expandDir))
				expand(expandDir);

			_followingBondedNbr = (_new_dir + 3) % 6;

			//nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
			qDebug() << "tail dir = " << tailDir() << endl;
			qDebug() << "after expandingFollowing bonded nbr = " << hasNbrAtLabel(_followingBondedNbr) << endl;
			qDebug() << "prec bonded nbr = " << _precedingBondedNbr << endl;
			//qDebug() << "prec bonded nbr = " << _precedingBondedNbr << endl;
			if (isExpanded()) {
				//If it is the last particle
				if (_precedingBondedNbr == -1) {

					contractTail();
					_followingBondedNbr = (_new_dir + 3) % 6;
					qDebug() << "after contracting Following bonded nbr = " << hasNbrAtLabel(_followingBondedNbr) << endl;
					nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;



				}
				//If it is not the last particle
				else {
					qDebug() << "entered else " << _precedingBondedNbr << endl;
					//If the transition doesnot change the connectivity
					qDebug() << "check if node is nbr = " << checkIfNodeIsNbr(pBN[0], pBN[1], nbrNode[0], nbrNode[1]) << endl;
					if (checkIfNodeIsNbr(pBN[0], pBN[1], nbrNode[0], nbrNode[1])) { //if prec bonded nbr is a neighbor to new expanded position
						qDebug() << "No need to pull at " << _name << endl;
						if(isExpanded())
							contractTail();
						if (hasNbrAtLabel(_followingBondedNbr))
							nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir; //right particle pBN changes
						_precedingBondedNbr = getLabelFromNode(head.x, head.y, pBN[0], pBN[1]);
						nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (_precedingBondedNbr + 3) % 6;
						_followingBondedNbr = (_new_dir + 3) % 6;
					}
					else {

						//------------------------------------------------------------------------------
						//START FROM HERE
						qDebug() << "Pull mechanism starts at " << _name << endl;
						vector<int> tailNode = getNodeFromLabel(head.x, head.y, tailDir());
						vector<int> pBN;
						vector<int> fBN;
						if (_precedingBondedNbr != -1) {
							
							pBN = getNodeFromLabel(tailNode[0], tailNode[1], _precedingBondedNbr);
							qDebug() << "pbn = " << pBN << endl;
						}
						if (_followingBondedNbr != -1) {
							fBN = getNodeFromLabel(head.x, head.y, _followingBondedNbr);
							qDebug() << "fbn = " << fBN << endl;

						}
						qDebug() << "current pos of " << _name << " = " << head.x << ", " << head.y << endl;
						qDebug() << "tail dir of " << _name << " = " << tailNode[0] << ", " << tailNode[1] << endl;
						qDebug() << "pbn of " << _name << " = " << pBN << endl;
						qDebug() << "fbn of " << _name << " = " << fBN << endl;

						//nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (tailDir() + 3) % 6;
						//_precedingBondedNbr = tailDir();
						//if (canPull(_precedingBondedNbr))
						//int labelToPull = getLabelFromNode(tailNode[0], tailNode[1], pBN[0], pBN[1]);
						//qDebug() << "label to pull " << labelToPull << endl;
						//qDebug() << "can pull?  " << canPull(labelToPull) << endl;


						for (int label : tailLabels()) {
							qDebug() << "label = " << label << endl;
							qDebug() << "_precedingBondedNbr = " << _precedingBondedNbr << endl;
							if(hasNbrAtLabel(label))
								qDebug() << "nbrAtLabel(label)._followingBondedNbr = " << nbrAtLabel(label)._followingBondedNbr << endl;

							if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr == (_precedingBondedNbr+3) %6) {
								qDebug() << "can pull this label: " << canPull(label) << endl;
								if (canPull(label)) {
									_precedingBondedNbr = tailDir();
									pull(label);
									break;
								}
							}
						}
						//_precedingBondedNbr = tailDir();
						qDebug() << "prec bonded nbr becomes " << _precedingBondedNbr << endl;




						//if (canPull(labelToPull))
							//pull(labelToPull);
						nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (_precedingBondedNbr + 3) % 6;
						if (hasNbrAtLabel(_followingBondedNbr))
							nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;


						Caterpillar1Particle *temp = &nbrAtLabel(_precedingBondedNbr);

						while ((temp->_precedingBondedNbr)!=-1) {
							qDebug() << "Pulling continues at " << temp->_name << endl;
							vector<int> tailNode = temp->getNodeFromLabel(temp->head.x, temp->head.y, temp->tailDir());
							vector<int> pBN = temp->getNodeFromLabel(tailNode[0], tailNode[1], temp->_precedingBondedNbr);
							vector<int> fBN = temp->getNodeFromLabel(temp->head.x, temp->head.y, temp->_followingBondedNbr);
							qDebug() << "current pos of " << temp->_name << " = " << temp->head.x << ", " << temp->head.y << endl;
							qDebug() << "tail dir of " << temp->_name << " = " << tailNode[0] << ", " << tailNode[1] << endl;
							qDebug() << "pbn of " << temp->_name << " = " << pBN << endl;
							qDebug() << "fbn of " << temp->_name << " = " << fBN << endl;
							qDebug() << "pbn and fbn are " << temp->_precedingBondedNbr << " " << temp->_followingBondedNbr << endl;


							if (checkIfNodeIsNbr(temp->head.x, temp->head.y, pBN[0], pBN[1])) { //if prec bonded nbr is a neighbor to new expanded position
								temp->contractTail();
								temp->_followingBondedNbr = getLabelFromNode(temp->head.x, temp->head.y, fBN[0], fBN[1]);
								if (temp->hasNbrAtLabel(temp->_followingBondedNbr))
									temp->nbrAtLabel(temp->_followingBondedNbr)._precedingBondedNbr = getLabelFromNode(fBN[0], fBN[1], temp->head.x, temp->head.y); //right particle pBN changes
								temp->_precedingBondedNbr = getLabelFromNode(temp->head.x, temp->head.y, pBN[0], pBN[1]);
								temp->nbrAtLabel(temp->_precedingBondedNbr)._followingBondedNbr = (temp->_precedingBondedNbr + 3) % 6;
							}
							else {
								//temp->nbrAtLabel(temp->_precedingBondedNbr)._followingBondedNbr = (temp->tailDir() + 3) % 6;

								for (int label : temp->tailLabels()) {
									qDebug() << "label = " << label << endl;
									qDebug() << "_precedingBondedNbr = " << temp->_precedingBondedNbr << endl;
									if (temp->hasNbrAtLabel(label))
										qDebug() << "nbrAtLabel(label)._followingBondedNbr = " << temp->nbrAtLabel(label)._followingBondedNbr << " at " << temp->_name << endl;

									if (temp->hasNbrAtLabel(label) && temp->nbrAtLabel(label)._followingBondedNbr == (temp->_precedingBondedNbr + 3) % 6) {
										qDebug() << "can pull this label: " << temp->canPull(label) << ", tail dir is " << temp->tailDir() << endl;
										if (temp->canPull(label)) {
											temp->_precedingBondedNbr = temp->tailDir();
											temp->pull(label);
											qDebug() << "prec bonded nbr becomes (inside for loop, before break stmt): " << temp->_precedingBondedNbr << endl;
										}
										break;
									}
								}
								
								qDebug() << "prec bonded nbr becomes " << temp->_precedingBondedNbr << endl;
								qDebug() << "fBN = " << fBN << " "<<temp->head.x <<" "<< temp->head.y << endl;
								qDebug() << "current pos of " << temp->_name << " = " << temp->head.x << ", " << temp->head.y << endl;

								//temp->_precedingBondedNbr = temp->tailDir();
								//vector<int> tailNode = getNodeFromLabel(temp->head.x, temp->head.y, temp->tailDir());
								//vector<int> fBN = getNodeFromLabel(tailNode[0], tailNode[1], temp->_followingBondedNbr);
								temp->_followingBondedNbr = getLabelFromNode(temp->head.x, temp->head.y, fBN[0], fBN[1]);
								qDebug() << "fBN label becomes = " << temp->_followingBondedNbr << " at " << temp->_name  << endl;

								if (temp->hasNbrAtLabel(temp->_followingBondedNbr)) {

									temp->nbrAtLabel(temp->_followingBondedNbr)._precedingBondedNbr = (temp->_followingBondedNbr + 3) % 6;
								}
								//int labelToPull = getLabelFromNode(tailNode[0], tailNode[1], pBN[0], pBN[1]);
								//if (temp->canPull(labelToPull))
									//temp->pull(labelToPull);
								//if (temp->canPull(temp->_precedingBondedNbr)) {
								//temp->pull(temp->_precedingBondedNbr);
								//}
								temp->nbrAtLabel(temp->_precedingBondedNbr)._followingBondedNbr = (temp->_precedingBondedNbr + 3) % 6;

								temp = &temp->nbrAtLabel(temp->_precedingBondedNbr);


								/*for (int label : tailLabels()) {
									if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr != -1
										&& pointsAtMe(nbrAtLabel(label), nbrAtLabel(label)._followingBondedNbr)) {
										if (canPull(label)) {
											nbrAtLabel(label)._followingBondedNbr =
												dirToNbrDir(nbrAtLabel(label), (tailDir() + 3) % 6);

											pull(label);
										}
										break;
									}
								}*/
							}

						}
						qDebug() << "Pull stops at " << temp->_name << endl;
						if (temp->isExpanded())
							temp->contractTail();
					}

				}
			}
			qDebug() << "Expansion ends at " << _name << endl;
			//----------------------------------- END OF EXPAND ---------------------------------------------------------------------------------------
			//-----------------------------------------------------------------------------------------------------------------------------------------

			//Next or direction
			if (isPointedByNextNode(_nextNodes, head.x, head.y) != 0 || isPointedByNextNode(_directionNodes, head.x, head.y) != 0 ) {
				qDebug() << "isPointedByNextNode(_nextNodes, head.x, head.y): "<< isPointedByNextNode(_nextNodes, head.x, head.y) << endl;
				qDebug() << "isPointedByNextNode(_directionNodes, head.x, head.y): "<< isPointedByNextNode(_directionNodes, head.x, head.y) << endl;
				qDebug() << "current pos: " << head.x << " " <<head.y  << endl;

				int pointLabel = labelPointing(_nextNodes, _directionNodes, head.x, head.y);
				dir_node_to_insert = getNodeFromLabel(head.x, head.y, pointLabel);
				qDebug() << "Inserting dir node " << dir_node_to_insert << " at label " << endl;
				_directionNodes.push_back({ dir_node_to_insert[0], dir_node_to_insert[1], pointLabel });

				//_rotate_status_tail = 3;;
				//int pointingLabel = isPointedByNextNode(_nextOne, head.x, head.y) ? _nextOne[3] : _nextTwo[3];
				//_direction.push_back({})
				//Write next or direction somehow
				qDebug() << "pointed by next, sending rotate to prec bonded nbr from " << _name << endl;
				qDebug() << "pbn and fbn at " << _name << " are " << _precedingBondedNbr << " and " << _followingBondedNbr << endl;

				if (hasNbrAtLabel(_precedingBondedNbr)) {
					nbrAtLabel(_precedingBondedNbr)._rotate_status_head = 1;
					nbrAtLabel(_precedingBondedNbr).rotate();
				}
				//return 3;
			}
			else {
				qDebug() << "not pointed by next, sending ack to following bonded nbr from " << _name << endl;
				qDebug() << "pbn and fbn at " << _name << " are " << _precedingBondedNbr << " and " << _followingBondedNbr << endl;
				if (hasNbrAtLabel(_followingBondedNbr)) {
					nbrAtLabel(_followingBondedNbr)._rotate_status_tail = 2; //Ack
					nbrAtLabel(_followingBondedNbr).rotate();
				}
				return;
			}

		}
		if (isExpanded())
			contractTail();
		//if receives Completed
		if (_rotate_status_tail == 3) { //completed
			qDebug() << "Rotate - follower - received completed " << _name << endl;
			if (hasNbrAtLabel(_followingBondedNbr)) {
				nbrAtLabel(_followingBondedNbr)._rotate_status_tail = 3;
				return;

			}

		}
		if (isExpanded())
			contractTail();
	}

		//return 3;
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

Caterpillar1Particle::Color Caterpillar1Particle::getColor(Caterpillar1Particle::State state) const {
	// Randomly select an integer and return the corresponding color via casting.
	if(state == State::Leader)
		return static_cast<Color>(0);
	else if (state == State::Root)
		return static_cast<Color>(1);
	else if (state == State::Follower)
		return static_cast<Color>(2);
	else if (state == State::Retired)
		return static_cast<Color>(3);
	else if (state == State::Terminated)
		return static_cast<Color>(4);
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
	int nameOfParticle = 1;
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
	leader->_receive_head = -1;
	leader->_receive_tail = -1;
	leader->_rotate_status_head = -1;
	leader->_rotate_status_tail = -1;
	leader->_name = nameOfParticle;
	nameOfParticle++;
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
		follower->_name = nameOfParticle;
		nameOfParticle++;
		if (numParticles - numParticlesAdded == 1) {
			follower->_precedingBondedNbr = -1;
			insert(follower);
			occupied.insert(followerNode);
			follower->_receive_head = -1;
			follower->_receive_tail = -1;
			follower->_rotate_status_head = -1;
			follower->_rotate_status_tail = -1;
			break;
		}
		else {
			follower->_precedingBondedNbr = 3;
		}
		insert(follower);
		follower->_receive_head = -1;
		follower->_receive_tail = -1;
		follower->_rotate_status_head = -1;
		follower->_rotate_status_tail = -1;
		occupied.insert(followerNode);
		prev = followerNode;
		numParticlesAdded++;


	}
}
