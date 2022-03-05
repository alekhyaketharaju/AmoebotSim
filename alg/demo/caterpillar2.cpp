
#include "alg/demo/caterpillar2.h"
//#include "stdafx.h"

#include <QDebug>


using namespace std;

std::vector<std::vector<int>> _nextNodes2; //each next node contains (x,y,label) where x and y are the node that is being pointed to (not by) and label is direction
std::vector<std::vector<int>> _firstNode2; //each next node contains (x,y,label) where x and y are the node that is being pointed to (not by) and label is direction
std::vector<std::vector<int>> _directionNodes2; //each next node contains (x,y,label) where x and y are the node that is being pointed to (not by) and label is direction

int layer2 = 1;
std::vector<int> next_node_to_insert2;
std::vector<int> first_node_to_insert2;
std::vector<int> dir_node_to_insert2;

Caterpillar2Particle::Caterpillar2Particle(const Node head,
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

void Caterpillar2Particle::activate() {
	qDebug() << "Layer number = " << layer2 << endl;
	//--------------------- LEADER STATE ---------------------
	if (_state == State::Leader) {
		//Leader state

		qDebug() << "----------LEaDER = " << _name << endl;
		_color = getColor(_state);
		first_node_to_insert2 = getNodeFromLabel(head.x, head.y, 3);
		_firstNode2.push_back({ first_node_to_insert2[0], first_node_to_insert2[1], 3 });
		if (hasNbrAtLabel(_precedingBondedNbr)) {

			nbrAtLabel(_precedingBondedNbr)._receive_head = 0; //root activation
		}

		if (_receive_tail == 0){ //receives terminate{
			_state = State::Terminated;
			_color = getColor(_state);
		}

	}

	//--------------------- ROOT STATE ---------------------
	else if (_state == State::Root) {
		qDebug() << "----------Root = " << _name << endl;



		_color = getColor(_state);


		qDebug() << "Pointed by first node o/p = " << endl;

		qDebug() << isPointedByFirstNode(_firstNode2, head.x, head.y) << endl;
		qDebug() << "layer2 number = " << layer2 << endl;

		//Not pointed by first node or next
		if (isPointedByFirstNode(_firstNode2, head.x, head.y) == -1 && isPointedByNextNode(_nextNodes2, head.x, head.y) == 0) {
			qDebug() << "1" << endl;
			if (isPointedByFirstNode(_firstNode2, head.x, head.y) == -1 && isPointedByNextNode(_nextNodes2, head.x, head.y) == 0) {
				//qDebug() << "1" << endl;

				rotate();
				//qDebug() << "1" << endl;

			}
		}

		//Pointed by firstnode or next
		else {
			if (isPointedByFirstNode(_firstNode2, head.x, head.y) != -1 && layer2 % 2 == 1) {
				next_node_to_insert2 = getNodeFromLabel(head.x, head.y, 1);
				_nextNodes2.push_back({ next_node_to_insert2[0], next_node_to_insert2[1], 1 });

				next_node_to_insert2 = getNodeFromLabel(head.x, head.y, 5);
				_nextNodes2.push_back({ next_node_to_insert2[0], next_node_to_insert2[1], 5 });
			}
			else if (isPointedByFirstNode(_firstNode2, head.x, head.y) != -1 && layer2 % 2 == 0) {
				next_node_to_insert2 = getNodeFromLabel(head.x, head.y, 2);
				_nextNodes2.push_back({ next_node_to_insert2[0], next_node_to_insert2[1], 2 });

				next_node_to_insert2 = getNodeFromLabel(head.x, head.y, 5);
				_nextNodes2.push_back({ next_node_to_insert2[0], next_node_to_insert2[1], 5 });
			}
			else if (isPointedByNextNode(_nextNodes2, head.x, head.y) >= 2) {
				//_firstNode2.clear();
				first_node_to_insert2 = getNodeFromLabel(head.x, head.y, 3);
				_firstNode2.push_back({ first_node_to_insert2[0], first_node_to_insert2[1], 3 });
				//_firstNode2.push_back({ (layer2 + 1) % 4, head.x, head.y, 3 });
				layer2 = (layer2 + 1) % 4;
			}
			else if (isPointedByNextNode(_nextNodes2, head.x, head.y) == 1) {
				for (int label = 0; label <= 5; label++) {

					//qDebug() << "Label = " << label << endl;
					//qDebug() << "hasNbrAtLabel(label): " << hasNbrAtLabel(label) << endl;
					if (label == _precedingBondedNbr) {
						int a = (label + 1) % 6;
						int b = label == 0 ? 5 : label - 1;
						//qDebug() << "a, b = " << a << ", " << b << endl;
						if ((hasNbrAtLabel(a) && (nbrAtLabel(a)._state == State::Retired || nbrAtLabel(a)._state == State::Leader) && a != _followingBondedNbr) || (hasNbrAtLabel(b) && (nbrAtLabel(b)._state == State::Retired || nbrAtLabel(b)._state == State::Leader) && b != _followingBondedNbr)) {
							next_node_to_insert2 = getNodeFromLabel(head.x, head.y, label);
							_nextNodes2.push_back({ next_node_to_insert2[0], next_node_to_insert2[1], label });
							break;
						}
					}

					if (!hasNbrAtLabel(label) && _precedingBondedNbr != -1) {
						int a = (label + 1) % 6;
						int b = label == 0 ? 5 : label - 1;
						//qDebug() << "a, b = " << a << ", " << b << endl;
						if ((hasNbrAtLabel(a) && (nbrAtLabel(a)._state == State::Retired || nbrAtLabel(a)._state == State::Leader) && a != _followingBondedNbr) || (hasNbrAtLabel(b) && (nbrAtLabel(b)._state == State::Retired || nbrAtLabel(b)._state == State::Leader) && b != _followingBondedNbr)) {
							next_node_to_insert2 = getNodeFromLabel(head.x, head.y, label);
							_nextNodes2.push_back({ next_node_to_insert2[0], next_node_to_insert2[1], label });
							break;
						}
					}
				}
			}

			//Forward root activation to preceding bonded nbr
			if (_precedingBondedNbr != -1 && hasNbrAtLabel(_precedingBondedNbr)) {
				//qDebug() << "----------root - forwarding root activation to preceding nbr" << endl;
				nbrAtLabel(_precedingBondedNbr)._receive_head = 0;
				//nbrAtLabel(_precedingBondedNbr)._state = State::Root;
			}
			_state = State::Retired;
			_color = getColor(_state);
		}

	}


	//--------------------- FOLLOWER STATE ---------------------
	else if (_state == State::Follower) {
		_color = getColor(_state);
		if (_receive_head == 0) {
			qDebug() << "Follower " << _name << " becoming root" << endl;
			_state = State::Root;
			_color = getColor(_state);
		}
	}

	//--------------------- RETIRED STATE ---------------------
	else if (_state == State::Retired) {
		_color = getColor(_state);
		//qDebug() << "Retired state = " << _name << endl;
		//qDebug() << "_precedingBondedNbr == -1: " << (_precedingBondedNbr == -1) << " (_followingBondedNbr): " << _followingBondedNbr << endl;
		if (_precedingBondedNbr == -1 && hasNbrAtLabel(_followingBondedNbr)) {
			//qDebug() << "Tail particle retired, sending terminate" << endl;
			nbrAtLabel(_followingBondedNbr)._receive_tail = 0; //terminate
			_state = State::Terminated;
			_color = getColor(_state);
		}

		if (_receive_tail == 0) {
			//qDebug() << "Received termination, forwarding to following bonded nbr" << endl;
			if (hasNbrAtLabel(_followingBondedNbr)) {
				nbrAtLabel(_followingBondedNbr)._receive_tail = 0; //terminate
				
			}
			_state = State::Terminated;
			_color = getColor(_state);
		}
	}
	//--------------------- TERMINATED STATE ---------------------
	else if (_state == State::Terminated) {
		layer2 = 1;
		_firstNode2.clear();
		_nextNodes2.clear();
		_color = getColor(_state);
		//qDebug() << "Terminated state = " << _name << endl;
	}

}





int Caterpillar2Particle::headMarkColor() const {
	switch (_color) {
	case Color::Red:    return 0xff0000;
	case Color::Yellow:	return 0xffff00;
	case Color::Green:	return 0x00ff00;
	case Color::Blue:   return 0x0000ff;
	case Color::Black:	return 0xffffff;
	}

	return -1;
}

int Caterpillar2Particle::headMarkDir() const {
	return _precedingBondedNbr;
}

int Caterpillar2Particle::tailMarkColor() const {
	return headMarkColor();
}

std::vector<int> Caterpillar2Particle::getNodeFromLabel(int x, int y, int label) const {
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

int Caterpillar2Particle::getLabelFromNode(int x_current, int y_current, int x_pre, int y_pre) const { //Current -> pre label
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

bool Caterpillar2Particle::checkIfNodeIsNbr(int x1, int y1, int x2, int y2) const {
	
	if ((x1 == x2 && abs(y1 - y2) == 1) || (y1 == y2 && abs(x1 - x2) == 1) || (x1 - x2 == 1 && y1 - y2 == -1) || (x1 - x2 == -1 && y1 - y2 == 1)) {
		return true;
	}
	return false;
	
}


int Caterpillar2Particle::labelPointing(std::vector<std::vector<int>> nextNode, std::vector<std::vector<int>> directionNode, int x, int y) const {
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


int Caterpillar2Particle::isPointedByFirstNode(std::vector<std::vector<int>> firstNode, int x, int y) const {

	for (int i = 0; i < firstNode.size(); i++) {
		if (firstNode[i][0] == x && firstNode[i][1] == y)
			return firstNode[i][2];
	}
	




	return -1;
}


int Caterpillar2Particle::isPointedByNextNode(std::vector<std::vector<int>> nextNode, int x, int y) const {
	//next i,x,y,label
	int count = 0;
	for (int i = 0; i < nextNode.size(); i++) {
		if (nextNode[i][0] == x && nextNode[i][1] == y)
			count = count + 1;
	}
	
	return count;
}

//Rotate function
//return values: (-1) - ack (not pointed), 1 - completed for next1, 2 - completed for next2
//
void Caterpillar2Particle::rotate() {

	int expandDir = (_followingBondedNbr + 1) % 6;
	//qDebug() << "_followingBondedNbr:" << _followingBondedNbr << endl;
	int _new_dir = getLabelFromNode(nbrAtLabel(_followingBondedNbr).head.x, nbrAtLabel(_followingBondedNbr).head.y, getNodeFromLabel(head.x, head.y, expandDir)[0], getNodeFromLabel(head.x, head.y, expandDir)[1]);
	//qDebug() << "new dir after = " << _new_dir << endl;
	//nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
	vector<int> nbrNode = getNodeFromLabel(head.x, head.y, expandDir);
	qDebug() << "nbrNode:" << nbrNode<<endl;
	//qDebug() << "new dir = " << _new_dir << endl;
	//int new_dir_1 = getLabelFromNode();
	vector<int> pBN;
	//qDebug() << "prec bonded nbr = " << _precedingBondedNbr << endl;
	if (_precedingBondedNbr != -1 && hasNbrAtLabel(_precedingBondedNbr))
		pBN = { nbrAtLabel(_precedingBondedNbr).head.x, nbrAtLabel(_precedingBondedNbr).head.y };
	//qDebug() << "pbn =  " << pBN << endl;
	//qDebug() << "nbrNode:" << nbrNode;endl;

	if (canExpand(expandDir))
		expand(expandDir);

	_followingBondedNbr = (_new_dir + 3) % 6;

	if (isExpanded()) {
		//If it is the last particle
		if (_precedingBondedNbr == -1) {

			contractTail();
			nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
			_followingBondedNbr = (_new_dir + 3) % 6;


		}
		//If it is not the last particle
		else {
			//If the transition doesnot change the connectivity
			//qDebug() << "check if node is nbr = " << checkIfNodeIsNbr(pBN[0], pBN[1], nbrNode[0], nbrNode[1]) << endl;
			//qDebug() << "checkIfNodeIsNbr:" << checkIfNodeIsNbr(pBN[0], pBN[1], nbrNode[0], nbrNode[1]) << endl;

			if (checkIfNodeIsNbr(pBN[0], pBN[1], nbrNode[0], nbrNode[1])) { //if prec bonded nbr is a neighbor to new expanded position
				//qDebug() << "No need to pull at " << _name << endl;
				if (isExpanded())
					contractTail();
				if (hasNbrAtLabel(_followingBondedNbr))
					nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir; //right particle pBN changes
				//qDebug() << "getLabelFromNode(head.x, head.y, pBN[0], pBN[1]) " << getLabelFromNode(head.x, head.y, pBN[0], pBN[1]) << endl;
				//qDebug() << "head.x, head.y = " << head.x << " " << head.y << endl;

				_precedingBondedNbr = getLabelFromNode(head.x, head.y, pBN[0], pBN[1]);
				//qDebug() << "_precedingBondedNbr " << _precedingBondedNbr << endl;

				nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (_precedingBondedNbr + 3) % 6;
				_followingBondedNbr = (_new_dir + 3) % 6;
				//qDebug() << "_followingBondedNbr " << _followingBondedNbr << endl;

			}
			else {

				//------------------------------------------------------------------------------
			//START FROM HERE
				//qDebug() << "Pull mechanism starts at " << _name << endl;
				vector<int> tailNode = getNodeFromLabel(head.x, head.y, tailDir());
				vector<int> pBN;
				vector<int> fBN;
				if (_precedingBondedNbr != -1) {

					pBN = getNodeFromLabel(tailNode[0], tailNode[1], _precedingBondedNbr);
					//qDebug() << "pbn = " << pBN << endl;
				}
				if (_followingBondedNbr != -1) {
					fBN = getNodeFromLabel(head.x, head.y, _followingBondedNbr);
					//qDebug() << "fbn = " << fBN << endl;

				}
				


				for (int label : tailLabels()) {
					//qDebug() << "label = " << label << endl;
					//qDebug() << "_precedingBondedNbr = " << _precedingBondedNbr << endl;
					if (hasNbrAtLabel(label))
						//qDebug() << "nbrAtLabel(label)._followingBondedNbr = " << nbrAtLabel(label)._followingBondedNbr << endl;

						if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr == (_precedingBondedNbr + 3) % 6) {
							//qDebug() << "can pull this label: " << canPull(label) << endl;
							if (canPull(label)) {
								_precedingBondedNbr = tailDir();
								pull(label);
								break;
							}
						}
				}
				//_precedingBondedNbr = tailDir();
				//qDebug() << "prec bonded nbr becomes " << _precedingBondedNbr << endl;




				nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (_precedingBondedNbr + 3) % 6;
				if (hasNbrAtLabel(_followingBondedNbr)) {
					nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
					_followingBondedNbr = (_new_dir + 3) % 6;
				}
				Caterpillar2Particle *temp = &nbrAtLabel(_precedingBondedNbr);

				while ((temp->_precedingBondedNbr) != -1) {
					//qDebug() << "Pulling continues at " << temp->_name << endl;
					vector<int> tailNode = temp->getNodeFromLabel(temp->head.x, temp->head.y, temp->tailDir());
					vector<int> pBN = temp->getNodeFromLabel(tailNode[0], tailNode[1], temp->_precedingBondedNbr);
					vector<int> fBN = temp->getNodeFromLabel(temp->head.x, temp->head.y, temp->_followingBondedNbr);
					


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
							//qDebug() << "label = " << label << endl;
							//qDebug() << "_precedingBondedNbr = " << temp->_precedingBondedNbr << endl;
							if (temp->hasNbrAtLabel(label))
								//qDebug() << "nbrAtLabel(label)._followingBondedNbr = " << temp->nbrAtLabel(label)._followingBondedNbr << " at " << temp->_name << endl;

								if (temp->hasNbrAtLabel(label) && temp->nbrAtLabel(label)._followingBondedNbr == (temp->_precedingBondedNbr + 3) % 6) {
									//qDebug() << "can pull this label: " << temp->canPull(label) << ", tail dir is " << temp->tailDir() << endl;
									if (temp->canPull(label)) {
										temp->_precedingBondedNbr = temp->tailDir();
										temp->pull(label);
									}
									break;
								}
						}

						
						temp->_followingBondedNbr = getLabelFromNode(temp->head.x, temp->head.y, fBN[0], fBN[1]);
						//qDebug() << "fBN label becomes = " << temp->_followingBondedNbr << " at " << temp->_name << endl;

						if (temp->hasNbrAtLabel(temp->_followingBondedNbr)) {

							temp->nbrAtLabel(temp->_followingBondedNbr)._precedingBondedNbr = (temp->_followingBondedNbr + 3) % 6;
						}
						
						temp->nbrAtLabel(temp->_precedingBondedNbr)._followingBondedNbr = (temp->_precedingBondedNbr + 3) % 6;

						temp = &temp->nbrAtLabel(temp->_precedingBondedNbr);


						
					}

				}
				//qDebug() << "Pull stops at " << temp->_name << endl;
				if (temp->isExpanded()) {
					temp->contractTail();
				}
			}

		}

	}
	return;
	
}




QString Caterpillar2Particle::inspectionText() const {
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

Caterpillar2Particle& Caterpillar2Particle::nbrAtLabel(int label) const {
	return AmoebotParticle::nbrAtLabel<Caterpillar2Particle>(label);
}



Caterpillar2Particle::Color Caterpillar2Particle::getColor(Caterpillar2Particle::State state) const {
	// Randomly select an integer and return the corresponding color via casting.
	if (state == State::Leader)
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




Caterpillar2System::Caterpillar2System(unsigned int numParticles) {


	std::set<Node> occupied;
	unsigned int numParticlesAdded = 0;
	int nameOfParticle = 1;
	int x = 2;
	int y = 2;
	Node leaderNode(x, y);
	numParticlesAdded++;
	Caterpillar2Particle* leader =
		new Caterpillar2Particle(leaderNode, -1, 0, *this,
			Caterpillar2Particle::State::Leader);
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
		Caterpillar2Particle* follower =
			new Caterpillar2Particle(followerNode, -1, 0, *this,
				Caterpillar2Particle::State::Follower);
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
