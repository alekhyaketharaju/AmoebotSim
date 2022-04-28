
#include "alg/demo/caterpillar1.h"
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

		_color = getColor(_state);
		first_node_to_insert = getNodeFromLabel(head.x, head.y, 3);
		_firstNode.push_back({ first_node_to_insert[0], first_node_to_insert[1], 3 });
		if (hasNbrAtLabel(_precedingBondedNbr)) {
			
			nbrAtLabel(_precedingBondedNbr)._receive_msg = 0; //root activation
		}

		if (_receive_msg == 4) { //receives terminate
			_state = State::Terminated;
			_receive_msg = -1;
		}
		_color = getColor(_state);
	}

	//--------------------- ROOT STATE ---------------------
	else if (_state == State::Root) {
		qDebug() << "----------Root = " << _name << endl;

		

		_color = getColor(_state);

		if (isPointedByFirstNode(_firstNode, head.x, head.y) != -1 && layer == 1) {
			next_node_to_insert = getNodeFromLabel(head.x, head.y, 2);

			_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], 2});
			

			_receive_msg = 1; //init rotate
			rotate();
			

			_nextNodes.clear();
			_directionNodes.clear();
			
			next_node_to_insert = getNodeFromLabel(head.x, head.y, 1);
			_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], 1});

			next_node_to_insert = getNodeFromLabel(head.x, head.y, 5);
			_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], 5});

			
			_receive_msg = 1; //init rotate
			rotate();
			_directionNodes.clear();

		}
		
		//pointed by FirstNodei, where i > 1
		
		else if (isPointedByFirstNode(_firstNode, head.x, head.y) != -1 && layer>1) {
			next_node_to_insert = getNodeFromLabel(head.x, head.y, 2);
			_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], 2 });

			next_node_to_insert = getNodeFromLabel(head.x, head.y, 5);
			_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], 5 });

			

			_receive_msg = 1;
			rotate();
			_directionNodes.clear();


		}
		//pointed by only one Nexti
		else if(isPointedByNextNode(_nextNodes, head.x, head.y) == 1) { //pointed only by one nexti
			
			for (int label = 0; label <=5; label++) {
				
				
				if (label == _precedingBondedNbr) {
					int a = (label + 1) % 6;
					int b = label == 0 ? 5 : label - 1;
					if ((hasNbrAtLabel(a) && (nbrAtLabel(a)._state == State::Retired || nbrAtLabel(a)._state == State::Leader) && a != _followingBondedNbr) || (hasNbrAtLabel(b) && (nbrAtLabel(b)._state == State::Retired || nbrAtLabel(b)._state == State::Leader) && b != _followingBondedNbr)) {
						next_node_to_insert = getNodeFromLabel(head.x, head.y, label);
						_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], label });
						break;
					}
				}

				if (!hasNbrAtLabel(label) && _precedingBondedNbr!=-1) {
					int a = (label + 1) % 6;
					int b = label == 0 ? 5 : label - 1;
					if ((hasNbrAtLabel(a) && (nbrAtLabel(a)._state == State::Retired || nbrAtLabel(a)._state == State::Leader) && a != _followingBondedNbr) || (hasNbrAtLabel(b) && (nbrAtLabel(b)._state == State::Retired || nbrAtLabel(b)._state == State::Leader) && b != _followingBondedNbr)) {
						next_node_to_insert = getNodeFromLabel(head.x, head.y, label);
						_nextNodes.push_back({ next_node_to_insert[0], next_node_to_insert[1], label });
						break;
					}
				}
			}

			if (_precedingBondedNbr != -1 && hasNbrAtLabel(_precedingBondedNbr)) {
				if (isPointedByNextNode(_nextNodes, nbrAtLabel(_precedingBondedNbr).head.x, nbrAtLabel(_precedingBondedNbr).head.y) == 0) {
					_receive_msg = 1;
					rotate();
					_directionNodes.clear();

				}
			}
		}
		//pointed by two Nexti
		else {
			
			_firstNode.clear();
			first_node_to_insert = getNodeFromLabel(head.x, head.y, 3);
			_firstNode.push_back({ first_node_to_insert[0], first_node_to_insert[1], 3 });
			layer = (layer + 1);
			_nextNodes.clear();
		}

		//Forward root activation to preceding bonded nbr
		if (_precedingBondedNbr!=-1 && hasNbrAtLabel(_precedingBondedNbr)) {
			nbrAtLabel(_precedingBondedNbr)._receive_msg = 0;
		}
		_state = State::Retired;
		_color = getColor(_state);

	}

	//--------------------- FOLLOWER STATE ---------------------
	else if (_state == State::Follower) {
		_color = getColor(_state);
		if (_receive_msg == 0) {
			_state = State::Root;
			_color = getColor(_state);
		}
	}

	//--------------------- RETIRED STATE ---------------------
	else if (_state == State::Retired) {
		_color = getColor(_state);
		if (_precedingBondedNbr == -1 && hasNbrAtLabel(_followingBondedNbr)) {
			nbrAtLabel(_followingBondedNbr)._receive_msg = 4; //terminate
			_state = State::Terminated;
			_color = getColor(_state);
		}

		if (_receive_msg == 4) {
			_receive_msg = -1;
			if (hasNbrAtLabel(_followingBondedNbr)) {
				nbrAtLabel(_followingBondedNbr)._receive_msg = 4; //terminate
			}
			_state = State::Terminated;
			_color = getColor(_state);
		}
	}
	//--------------------- TERMINATED STATE ---------------------
	else if (_state == State::Terminated) {
		layer = 1;
		_firstNode.clear();
		_nextNodes.clear();

		_color = getColor(_state);
	}

}





int Caterpillar1Particle::headMarkColor() const {
	switch (_color) {
	case Color::Red:    return 0xff0000;
	case Color::Yellow:	return 0xffff00;
	case Color::Green:	return 0x00ff00;
	case Color::Blue:   return 0x0000ff;
	case Color::Black:	return 0xffffff;
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

	if ((x1 == x2 && abs(y1 - y2) == 1) || (y1 == y2 && abs(x1 - x2) == 1) || (x1 - x2 == 1 && y1 - y2 == -1) || (x1 - x2 == -1 && y1 - y2 == 1)) {
		return true;
	}
	return false;

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
		
	return -1;
}


int Caterpillar1Particle::isPointedByNextNode(std::vector<std::vector<int>> nextNode, int x, int y) const {
	//next i,x,y,label
	int count = 0;
	for (int i = 0; i < nextNode.size(); i++) {
		if (nextNode[i][0] == x && nextNode[i][1] == y)
			count = count + 1;
	}
	return count;
}

//Rotate function
//
void Caterpillar1Particle::rotate() {

	//----------------- ROOT STATE -----------------
	if (_state == State::Root) {
		//send rotate to next follower particle
		if (_receive_msg == 1) {
			_receive_msg = -1;
			qDebug() << "Rotate - root state - entered by " << _name << endl;
			if (_precedingBondedNbr == -1)
				return;
			Caterpillar1Particle* temp = &nbrAtLabel(_precedingBondedNbr);
			while (temp->_state != State::Follower) {
				if (temp->hasNbrAtLabel(temp->_precedingBondedNbr))
					temp = &(temp->nbrAtLabel(temp->_precedingBondedNbr));
			}
			temp->_receive_msg = 1; // rotate
			temp->rotate();
			
		}

		//Ack
		if (_receive_msg == 2 && _precedingBondedNbr!=-1 && hasNbrAtLabel(_precedingBondedNbr)) {
			_receive_msg = -1;
			nbrAtLabel(_precedingBondedNbr)._receive_msg = 1; //send rotate
			nbrAtLabel(_precedingBondedNbr).rotate();
		}

		//Completed
		if (_receive_msg == 3) {
			_receive_msg = -1;
			

			return;
		}
		
	}

	//---------------- FOLLOWER STATE ----------------
	else if (_state == State::Follower) {
		
		//Follower receives Rotate
		if (_receive_msg == 1) {
			_receive_msg = -1;
			//if not the tail particle
			if (_precedingBondedNbr != -1 && hasNbrAtLabel(_precedingBondedNbr)) {
				nbrAtLabel(_precedingBondedNbr)._receive_msg = 1;
				nbrAtLabel(_precedingBondedNbr).rotate();
			}
			//f tail particle
			else {

				//Rotate once clockwise
				
				int expandDir = (_followingBondedNbr + 1) % 6;
				int _new_dir = getLabelFromNode(nbrAtLabel(_followingBondedNbr).head.x, nbrAtLabel(_followingBondedNbr).head.y, getNodeFromLabel(head.x, head.y, expandDir)[0], getNodeFromLabel(head.x, head.y, expandDir)[1]);
				vector<int> nbrNode = getNodeFromLabel(head.x, head.y, expandDir);
				vector<int> pBN;
				if (_precedingBondedNbr != -1 && hasNbrAtLabel(_precedingBondedNbr))
					pBN = { nbrAtLabel(_precedingBondedNbr).head.x, nbrAtLabel(_precedingBondedNbr).head.y };
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
						if (checkIfNodeIsNbr(pBN[0], pBN[1], nbrNode[0], nbrNode[1])) { //if prec bonded nbr is a neighbor to new expanded position
							if (isExpanded())
								contractTail();
							if (hasNbrAtLabel(_followingBondedNbr))
								nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir; //right particle pBN changes
							_precedingBondedNbr = getLabelFromNode(head.x, head.y, pBN[0], pBN[1]);
							nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (_precedingBondedNbr + 3) % 6;
							_followingBondedNbr = (_new_dir + 3) % 6;
						}
						else {

							vector<int> tailNode = getNodeFromLabel(head.x, head.y, tailDir());
							vector<int> pBN;
							vector<int> fBN;
							if (_precedingBondedNbr != -1) {

								pBN = getNodeFromLabel(tailNode[0], tailNode[1], _precedingBondedNbr);
								//qDebug() << "pbn = " << pBN << endl;
							}
							if (_followingBondedNbr != -1) {
								fBN = getNodeFromLabel(head.x, head.y, _followingBondedNbr);

							}
							

							for (int label : tailLabels()) {
								
								if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr == (_precedingBondedNbr + 3) % 6) {
									if (canPull(label)) {
										_precedingBondedNbr = tailDir();
										pull(label);
										break;
									}
								}
							}
							
							nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (_precedingBondedNbr + 3) % 6;
							if (hasNbrAtLabel(_followingBondedNbr)) {
								nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
								_followingBondedNbr = (_new_dir + 3) % 6;
							}
							Caterpillar1Particle *temp = &nbrAtLabel(_precedingBondedNbr);

							while ((temp->_precedingBondedNbr) != -1) {
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

									for (int label : temp->tailLabels()) {
										
										if (temp->hasNbrAtLabel(label) && temp->nbrAtLabel(label)._followingBondedNbr == (temp->_precedingBondedNbr + 3) % 6) {
											if (temp->canPull(label)) {
												temp->_precedingBondedNbr = temp->tailDir();
												temp->pull(label);
											}
											break;
										}
									}

									temp->_followingBondedNbr = getLabelFromNode(temp->head.x, temp->head.y, fBN[0], fBN[1]);
									
									if (temp->hasNbrAtLabel(temp->_followingBondedNbr)) {

										temp->nbrAtLabel(temp->_followingBondedNbr)._precedingBondedNbr = (temp->_followingBondedNbr + 3) % 6;
									}
									
									temp->nbrAtLabel(temp->_precedingBondedNbr)._followingBondedNbr = (temp->_precedingBondedNbr + 3) % 6;

									temp = &temp->nbrAtLabel(temp->_precedingBondedNbr);

								}

							}
							if (temp->isExpanded()) {
								temp->contractTail();
							}
						}

					}
				
}

				if (isPointedByNextNode(_nextNodes, head.x, head.y) != 0 || isPointedByNextNode(_directionNodes, head.x, head.y) != 0) {
					
					if (hasNbrAtLabel(_followingBondedNbr)) {
						nbrAtLabel(_followingBondedNbr)._receive_msg = 3;//Complete
						return;
					}
					return;
				}
				else {
					
					if (hasNbrAtLabel(_followingBondedNbr)) {
						nbrAtLabel(_followingBondedNbr)._receive_msg = 2;//Ack
						return;
					}
					return;
				}
			}
		}
		if (isExpanded())
			contractTail();
		//if receives Ack
		if (_receive_msg == 2) {
			_receive_msg = -1;
			//Rotate once
			
			int expandDir = (_followingBondedNbr + 1) % 6;
			
			int _new_dir = getLabelFromNode(nbrAtLabel(_followingBondedNbr).head.x, nbrAtLabel(_followingBondedNbr).head.y, getNodeFromLabel(head.x, head.y, expandDir)[0], getNodeFromLabel(head.x, head.y, expandDir)[1]);
			vector<int> nbrNode = getNodeFromLabel(head.x, head.y, expandDir);
			vector<int> pBN;
			if (hasNbrAtLabel(_precedingBondedNbr))
				pBN = { nbrAtLabel(_precedingBondedNbr).head.x, nbrAtLabel(_precedingBondedNbr).head.y };
			if (canExpand(expandDir))
				expand(expandDir);

			_followingBondedNbr = (_new_dir + 3) % 6;

			if (isExpanded()) {
				//If it is the last particle
				if (_precedingBondedNbr == -1) {

					contractTail();
					_followingBondedNbr = (_new_dir + 3) % 6;
					nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;



				}
				//If it is not the last particle
				else {
					if (checkIfNodeIsNbr(pBN[0], pBN[1], nbrNode[0], nbrNode[1])) { //if prec bonded nbr is a neighbor to new expanded position
						if(isExpanded())
							contractTail();
						if (hasNbrAtLabel(_followingBondedNbr))
							nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir; //right particle pBN changes
						_precedingBondedNbr = getLabelFromNode(head.x, head.y, pBN[0], pBN[1]);
						nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (_precedingBondedNbr + 3) % 6;
						_followingBondedNbr = (_new_dir + 3) % 6;
					}
					else {

						vector<int> tailNode = getNodeFromLabel(head.x, head.y, tailDir());
						vector<int> pBN;
						vector<int> fBN;
						if (_precedingBondedNbr != -1) {
							
							pBN = getNodeFromLabel(tailNode[0], tailNode[1], _precedingBondedNbr);
						}
						if (_followingBondedNbr != -1) {
							fBN = getNodeFromLabel(head.x, head.y, _followingBondedNbr);

						}
						
						for (int label : tailLabels()) {
							
							if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr == (_precedingBondedNbr+3) %6) {
								if (canPull(label)) {
									_precedingBondedNbr = tailDir();
									pull(label);
									break;
								}
							}
						}
						
						nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (_precedingBondedNbr + 3) % 6;
						if (hasNbrAtLabel(_followingBondedNbr))
							nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;


						Caterpillar1Particle *temp = &nbrAtLabel(_precedingBondedNbr);

						while ((temp->_precedingBondedNbr)!=-1) {
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

								for (int label : temp->tailLabels()) {
									
									if (temp->hasNbrAtLabel(label) && temp->nbrAtLabel(label)._followingBondedNbr == (temp->_precedingBondedNbr + 3) % 6) {
										if (temp->canPull(label)) {
											temp->_precedingBondedNbr = temp->tailDir();
											temp->pull(label);
										}
										break;
									}
								}
								
								temp->_followingBondedNbr = getLabelFromNode(temp->head.x, temp->head.y, fBN[0], fBN[1]);
								
								if (temp->hasNbrAtLabel(temp->_followingBondedNbr)) {

									temp->nbrAtLabel(temp->_followingBondedNbr)._precedingBondedNbr = (temp->_followingBondedNbr + 3) % 6;
								}
								
								temp->nbrAtLabel(temp->_precedingBondedNbr)._followingBondedNbr = (temp->_precedingBondedNbr + 3) % 6;

								temp = &temp->nbrAtLabel(temp->_precedingBondedNbr);

							}

						}
						if (temp->isExpanded())
							temp->contractTail();
					}

				}
			}
			
			//Next or direction
			if (isPointedByNextNode(_nextNodes, head.x, head.y) != 0 || isPointedByNextNode(_directionNodes, head.x, head.y) != 0 ) {
				
				int pointLabel = labelPointing(_nextNodes, _directionNodes, head.x, head.y);
				dir_node_to_insert = getNodeFromLabel(head.x, head.y, pointLabel);
				_directionNodes.push_back({ dir_node_to_insert[0], dir_node_to_insert[1], pointLabel });

				
				if (hasNbrAtLabel(_precedingBondedNbr)) {
					nbrAtLabel(_precedingBondedNbr)._receive_msg = 1;
					nbrAtLabel(_precedingBondedNbr).rotate();
				}
			}
			else {
				if (hasNbrAtLabel(_followingBondedNbr)) {
					nbrAtLabel(_followingBondedNbr)._receive_msg = 2; //Ack
					return;
				}
				return;
			}

		}
		if (isExpanded())
			contractTail();
		//if receives Completed
		if (_receive_msg == 3) { //completed
			_receive_msg = -1;
			if (hasNbrAtLabel(_followingBondedNbr)) {
				nbrAtLabel(_followingBondedNbr)._receive_msg = 3;
				return;

			}

		}
		if (isExpanded())
			contractTail();
	}

		
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



Caterpillar1Particle::Color Caterpillar1Particle::getColor(Caterpillar1Particle::State state) const {
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
	leader->_receive_msg = -1;
	
	leader->_name = nameOfParticle;
	nameOfParticle++;
	Node prev = leaderNode;
	occupied.insert(leaderNode);
	while (numParticlesAdded < numParticles) {
		
		int followerDir = 3;
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
			follower->_receive_msg = -1;
			
			break;
		}
		else {
			follower->_precedingBondedNbr = 3;
		}
		insert(follower);
		follower->_receive_msg = -1;
		
		occupied.insert(followerNode);
		prev = followerNode;
		numParticlesAdded++;


	}
}
