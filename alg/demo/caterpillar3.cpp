
#include "alg/demo/caterpillar3.h"
#include <QDebug>


using namespace std;

int initialization = 1;
std::vector<int> dirToMove = { 1, 0, 4, 4, 0, 1};
std::vector<int> count2 = { 0,1,3,5 };
Caterpillar3Particle::Caterpillar3Particle(const Node head,
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

void Caterpillar3Particle::activate() {
    //--------------------- LEADER STATE ---------------------
    if (_state == State::Leader) {
        //qDebug() << "----------LEaDER = " << _name << endl;
        _color = getColor(_state);
        if(initialization == 1){
            qDebug() << "init: " << initialization << endl;
            //qDebug() << "dir to move size: " << dirToMove.size() << endl;
            //qDebug() << "dir to move size: " << dirToMove << endl;

            if (dirToMove.size() != 0) {
                zigzag(dirToMove[0]);
                dirToMove.erase(dirToMove.begin());
                qDebug() << "dir to move: " << dirToMove << endl;

            }
            else {
                dirToMove = { 1, 0, 4, 4, 0, 1 };
                initialization = -1;
                qDebug() << "dir to move: " << dirToMove << endl;

            }
        }

        else if (_receive_msg == 1) {
			//_receive_msg = -1;
			initialization = 0;
            if (dirToMove.size() != 0) {
                zigzag(dirToMove[0]);
                dirToMove.erase(dirToMove.begin());
            }
            else {
                dirToMove = { 1, 0, 4, 4, 0, 1 };
				initialization = -1;
				_receive_msg = -1;
            }
        }

        nbrAtLabel(_precedingBondedNbr)._receive_msg = 3; //round anymore?

        if (_receive_msg == 0) //receives terminate
			//_receive_msg = -1;
			
            _state = State::Terminated;
		_color = getColor(_state);
    }


    //--------------------- FOLLOWER STATE ---------------------
    else if (_state == State::Follower && initialization==-1) {
        _color = getColor(_state);
        if (_receive_msg == 3) { //received round anymore?
			qDebug() << "Particle " << _name << "recived the msg: " << _receive_msg << endl;
			_receive_msg = -1;
			

            if ((_precedingBondedNbr == -1 || (_precedingBondedNbr == 3 && nbrAtLabel(_precedingBondedNbr)._state == State::Follower)) && (_followingBondedNbr == 0)) {
				qDebug() << "Ack being sent to following bonded nb from " << _name << endl;
                if (_followingBondedNbr != -1 && nbrAtLabel(_followingBondedNbr)._state == State::SemiRetired)
                    nbrAtLabel(_followingBondedNbr)._receive_msg = 1;
                else if (_precedingBondedNbr != -1 && nbrAtLabel(_precedingBondedNbr)._state == State::SemiRetired)
                    nbrAtLabel(_precedingBondedNbr)._receive_msg = 1;
            }

            else {
				
                if (_precedingBondedNbr != -1) {
					qDebug() << "Forwarding round anymore to preceding bonded nbr from " << _name << endl;
                    nbrAtLabel(_precedingBondedNbr)._receive_msg = 3;
                    _state = State::SemiRetired;
					_color = getColor(_state);

                }
                else {
					qDebug() << "Sending terminate from " << _name << endl;
                    nbrAtLabel(_followingBondedNbr)._receive_msg = 0;
                    _state = State::Terminated;
					_color = getColor(_state);

                }
            }

        }

        if (_receive_msg == 4) {//p10
			_receive_msg = -1;

			zigzag(1);
            zigzag(0);
            if (hasNbrAtLabel(_followingBondedNbr)) {
                nbrAtLabel(_followingBondedNbr)._receive_msg = 2;
                //nbrAtLabel(_sender)._sender = (_sender + 3) % 6;
            }
        }

        if (_receive_msg == 5) {//p10p1
			_receive_msg = -1;

            zigzag(1);
            zigzag(0);
            if (hasNbrAtLabel(_precedingBondedNbr)) {
                nbrAtLabel(_precedingBondedNbr)._receive_msg = 6;
                //nbrAtLabel(_precedingBondedNbr)._sender = (_precedingBondedNbr + 3) % 6;
            }
        }

        if (_receive_msg == 6) {//p1
            zigzag(1);
            if (hasNbrAtLabel(_followingBondedNbr)) {
                nbrAtLabel(_followingBondedNbr)._receive_msg = 2;
                //nbrAtLabel(_sender)._sender = (_sender + 3) % 6;
            }
        }

        if (_receive_msg == 7) {//p4
			_receive_msg = -1;

            zigzag(4);
            if (hasNbrAtLabel(_followingBondedNbr)) {
                nbrAtLabel(_followingBondedNbr)._receive_msg = 2;
                //nbrAtLabel(_sender)._sender = (_sender + 3) % 6;
            }
        }

        if (_receive_msg == 2) {
			_receive_msg = -1;

            if (hasNbrAtLabel(_followingBondedNbr))
                nbrAtLabel(_followingBondedNbr)._receive_msg = 2;
        }
    }

    //--------------------- SEMI-RETIRED STATE ---------------------
    else if (_state == State::SemiRetired && initialization==-1) {
        _color = getColor(_state);
		if (_name == 2) {
			qDebug() << "_name = " << _name << endl;
			qDebug() << "received msg = "<< _receive_msg << endl;
		}
        if (_receive_msg == 1) {
			_receive_msg = -1;
			qDebug() << "Received Ack at " << _name << endl;
			qDebug() << "checkForCornerNode(): " << checkForCornerNode() << endl;
			qDebug() << hasNbrAtLabel(4) << endl;
			//qDebug() << nbrAtLabel(4)._state << endl;
			qDebug() << hasNbrAtLabel(0) << endl;
			//qDebug() << nbrAtLabel(0)._state << endl;

			//qDebug() << "conditions: " << hasNbrAtLabel(4) << " , " << nbrAtLabel(4)._state << endl;
			//qDebug() << " , " << hasNbrAtLabel(0) << " , " << nbrAtLabel(0)._state << endl;
            if (checkForCornerNode() && hasNbrAtLabel(4) && nbrAtLabel(4)._state == State::Follower && hasNbrAtLabel(0) && nbrAtLabel(0)._state == State::SemiRetired) {
				qDebug() << "corner node, SW and E, " << _name << endl;
                zigzag(1);
                zigzag(0);
                if (hasNbrAtLabel(_followingBondedNbr))
                    nbrAtLabel(_followingBondedNbr)._receive_msg = 1; //Ack
                _state = State::Follower;
				_color = getColor(_state);

            }

            else if (checkForCornerNode() && hasNbrAtLabel(4) && nbrAtLabel(4)._state == State::Follower && hasNbrAtLabel(5) && nbrAtLabel(5)._state == State::SemiRetired) {
				qDebug() << "corner node, SW and SE, " << _name << endl;

                zigzag(0);
                if (hasNbrAtLabel(_precedingBondedNbr))
                    nbrAtLabel(_precedingBondedNbr)._receive_msg = 4; //p10
            }

            else if (checkForCornerNode() && hasNbrAtLabel(1) && nbrAtLabel(1)._state == State::Follower && hasNbrAtLabel(0) && nbrAtLabel(0)._state == State::SemiRetired) {
				qDebug() << "corner node, NE and E, " << _name << endl;

				zigzag(5);
				if (_followingBondedNbr!=-1) {
					qDebug() << "sending ack to _followingBondedNbr: " << _followingBondedNbr << ", hasNbrAtLabel(_followingBondedNbr):" << hasNbrAtLabel(_followingBondedNbr) << endl;
					nbrAtLabel(_followingBondedNbr)._receive_msg = 1; //Ack
					qDebug() << "nbrAtLabel(_followingBondedNbr)._receive_msg: " << nbrAtLabel(_followingBondedNbr)._receive_msg << endl;
					qDebug() << "nbrAtLabel(_followingBondedNbr)._name: " << nbrAtLabel(_followingBondedNbr)._name << endl;
				}
                _state = State::Follower;
				_color = getColor(_state);

            }

            else if (!checkForCornerNode() && hasNbrAtLabel(4) && nbrAtLabel(4)._state == State::Follower && hasNbrAtLabel(5) && nbrAtLabel(5)._state == State::SemiRetired) {
				qDebug() << "non-corner node, SW and SE, " << _name << endl;

				zigzag(0);
                if (hasNbrAtLabel(_precedingBondedNbr))
                    nbrAtLabel(_precedingBondedNbr)._receive_msg = 5; //p10p1
            }

            else if (!checkForCornerNode() && hasNbrAtLabel(1) && nbrAtLabel(1)._state == State::Follower && hasNbrAtLabel(0) && nbrAtLabel(0)._state == State::SemiRetired) {
				qDebug() << "non-corner node, NE and E, " << _name << endl;

				zigzag(5);
                if (hasNbrAtLabel(_precedingBondedNbr))
                    nbrAtLabel(_precedingBondedNbr)._receive_msg = 7; //
            }
			else {
				if (hasNbrAtLabel(_followingBondedNbr))
					nbrAtLabel(_followingBondedNbr)._receive_msg = 1; //Ack
				_state = State::Follower;
				_color = getColor(_state);

			}
        }

        if (_receive_msg == 2) {
			_receive_msg = -1;
            if (hasNbrAtLabel(_followingBondedNbr))
                nbrAtLabel(_followingBondedNbr)._receive_msg = 1; //Ack
            _state = State::Follower;
			_color = getColor(_state);

        }

        if (_receive_msg == 0) {
			_receive_msg = -1;

            if (hasNbrAtLabel(_followingBondedNbr))
                nbrAtLabel(_followingBondedNbr)._receive_msg = 1; //Terminate
            _state = State::Terminated;
			_color = getColor(_state);

        }
    }
    //--------------------- TERMINATED STATE ---------------------
    else if (_state == State::Terminated && initialization==-1) {
    _color = getColor(_state);
    }

}


bool Caterpillar3Particle::checkForCornerNode() {
    std::vector<int> count;

    for (int i = 0;i < 6;i++) {
		//qDebug() << "i: " << i << " , hasNbrAtLabel(i): " << hasNbrAtLabel(i) << endl;
        if (hasNbrAtLabel(i))
            count.push_back(i);
    }
	//qDebug() << "Checking corner count: " << count << endl;
    if (count.size() <= 3)
        return true;
	if (count == count2 )
		return true;
    return false;
}


int Caterpillar3Particle::headMarkColor() const {
    switch (_color) {
    case Color::Red:    return 0xff0000;
    case Color::Green:	return 0x00ff00;
    case Color::Blue:   return 0x0000ff;
    case Color::White:	return 0xffffff;
    }

    return -1;
}

int Caterpillar3Particle::headMarkDir() const {
    return _precedingBondedNbr;
}

int Caterpillar3Particle::tailMarkColor() const {
    return headMarkColor();
}

std::vector<int> Caterpillar3Particle::getNodeFromLabel(int x, int y, int label) const {
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

int Caterpillar3Particle::getLabelFromNode(int x_current, int y_current, int x_pre, int y_pre) const { //Current -> pre label
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

bool Caterpillar3Particle::checkIfNodeIsNbr(int x1, int y1, int x2, int y2) const {

    if ((x1 == x2 && abs(y1 - y2) == 1) || (y1 == y2 && abs(x1 - x2) == 1) || (x1 - x2 == 1 && y1 - y2 == -1) || (x1 - x2 == -1 && y1 - y2 == 1)) {
        return true;
    }
    return false;

}


int Caterpillar3Particle::labelPointing(std::vector<std::vector<int>> nextNode, std::vector<std::vector<int>> directionNode, int x, int y) const {
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


int Caterpillar3Particle::isPointedByFirstNode(std::vector<std::vector<int>> firstNode, int x, int y) const {

    for (int i = 0; i < firstNode.size(); i++) {
        if (firstNode[i][0] == x && firstNode[i][1] == y)
            return firstNode[i][2];
    }



    return -1;
}


int Caterpillar3Particle::isPointedByNextNode(std::vector<std::vector<int>> nextNode, int x, int y) const {
    //next i,x,y,label
    int count = 0;
    for (int i = 0; i < nextNode.size(); i++) {
        if (nextNode[i][0] == x && nextNode[i][1] == y)
            count = count + 1;
    }

    return count;
}


void Caterpillar3Particle::zigzag(int _dir){

    int expandDir = _dir;
    int _new_dir = -1;
    qDebug() << "expand dir: " << expandDir << endl;
    if(_followingBondedNbr!=-1 && hasNbrAtLabel(_followingBondedNbr)){
        _new_dir = getLabelFromNode(nbrAtLabel(_followingBondedNbr).head.x, nbrAtLabel(_followingBondedNbr).head.y, getNodeFromLabel(head.x, head.y, expandDir)[0], getNodeFromLabel(head.x, head.y, expandDir)[1]);
        qDebug() << "_new_dir: " << _new_dir << endl;
    }
    vector<int> nbrNode = getNodeFromLabel(head.x, head.y, expandDir);
    qDebug() << "nbrNode:" << nbrNode<<endl;
    vector<int> pBN;
	if (_precedingBondedNbr != -1 && hasNbrAtLabel(_precedingBondedNbr)) {

		pBN = { nbrAtLabel(_precedingBondedNbr).head.x, nbrAtLabel(_precedingBondedNbr).head.y };
		qDebug() << "_precedingBondedNbr:" << _precedingBondedNbr << endl;
	}
	qDebug() << "canExpand(expandDir):" << canExpand(expandDir) << endl;

    if (canExpand(expandDir))
        expand(expandDir);
	if (_followingBondedNbr != -1) {
		_followingBondedNbr = (_new_dir + 3) % 6;
		qDebug() << "_followingBondedNbr:" << _followingBondedNbr << endl;

	}
    if (isExpanded()) {
        //If it is the last particle
        if (_precedingBondedNbr == -1) {

            contractTail();
            if(_followingBondedNbr!=-1){
                nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
                _followingBondedNbr = (_new_dir + 3) % 6;
				qDebug() << "_followingBondedNbr:" << _followingBondedNbr << endl;

            }

        }
        //If it is not the last particle
        else {

            vector<int> tailNode = getNodeFromLabel(head.x, head.y, tailDir());
            vector<int> pBN;
            vector<int> fBN;
            if (_precedingBondedNbr != -1) {

                pBN = getNodeFromLabel(tailNode[0], tailNode[1], _precedingBondedNbr);
            }
            if (_followingBondedNbr != -1) {
                fBN = getNodeFromLabel(head.x, head.y, _followingBondedNbr);
				qDebug() << "_followingBondedNbr:" << _followingBondedNbr << endl;

            }

			for (int label : tailLabels()) {
				if (hasNbrAtLabel(label)) {
					qDebug() << "label:" << label << endl;
					qDebug() << "nbrAtLabel(label)._followingBondedNbr: " << nbrAtLabel(label)._followingBondedNbr << endl;
					if (hasNbrAtLabel(label) && nbrAtLabel(label)._followingBondedNbr == (_precedingBondedNbr + 3) % 6 && pointsAtMe(nbrAtLabel(label), nbrAtLabel(label)._followingBondedNbr)) {
						if (canPull(label)) {
							qDebug() << "can pull label:" << canPull(label) << endl;
							_precedingBondedNbr = tailDir();
							qDebug() << "pull starting" << endl;
							pull(label);
							qDebug() << "pull complete" << endl;
							break;
						}
					}
				}
			}
            nbrAtLabel(_precedingBondedNbr)._followingBondedNbr = (_precedingBondedNbr + 3) % 6;
			qDebug() << "nbrAtLabel(_precedingBondedNbr)._followingBondedNbr: " << nbrAtLabel(_precedingBondedNbr)._followingBondedNbr << endl;
            if (_followingBondedNbr!=-1) {
                nbrAtLabel(_followingBondedNbr)._precedingBondedNbr = _new_dir;
                _followingBondedNbr = (_new_dir + 3) % 6;
				qDebug() << "_followingBondedNbr:" << _followingBondedNbr << endl;

            }
            Caterpillar3Particle *temp = &nbrAtLabel(_precedingBondedNbr);
			qDebug() << "temp pulling starts" << endl;
            while ((temp->_precedingBondedNbr) != -1) {
                vector<int> tailNode = temp->getNodeFromLabel(temp->head.x, temp->head.y, temp->tailDir());
                vector<int> pBN = temp->getNodeFromLabel(tailNode[0], tailNode[1], temp->_precedingBondedNbr);
                vector<int> fBN = temp->getNodeFromLabel(temp->head.x, temp->head.y, temp->_followingBondedNbr);
				qDebug() << "temp name: " << temp->_name << endl;
				for (int label : temp->tailLabels()) {
					if (temp->hasNbrAtLabel(label)) {
						//qDebug() << "label:" << label << endl;

						if (temp->hasNbrAtLabel(label) && temp->nbrAtLabel(label)._followingBondedNbr == (temp->_precedingBondedNbr + 3) % 6 && temp->pointsAtMe(temp->nbrAtLabel(label), temp->nbrAtLabel(label)._followingBondedNbr)) {
							if (temp->canPull(label)) {
								temp->_precedingBondedNbr = temp->tailDir();
								temp->pull(label);
							}
							break;
						}
					}
				}

                temp->_followingBondedNbr = getLabelFromNode(temp->head.x, temp->head.y, fBN[0], fBN[1]);

                if (temp->_followingBondedNbr!=-1) {

                    temp->nbrAtLabel(temp->_followingBondedNbr)._precedingBondedNbr = (temp->_followingBondedNbr + 3) % 6;
                }

                temp->nbrAtLabel(temp->_precedingBondedNbr)._followingBondedNbr = (temp->_precedingBondedNbr + 3) % 6;
				qDebug() << "temp _preceedingBondedNbr: " << temp->_precedingBondedNbr << endl;
				qDebug() << "temp _followingBondedNbr: " << temp->_followingBondedNbr << endl;
				if(temp->hasNbrAtLabel(2))
					qDebug() << "temp->nbrAtLabel(2): " << temp->nbrAtLabel(2)._name << endl;

				qDebug() << "temp->nbrAtLabel(temp->_precedingBondedNbr): " << temp->nbrAtLabel(temp->_precedingBondedNbr)._name << endl;
                temp = &temp->nbrAtLabel(temp->_precedingBondedNbr);

            }
            if (temp->isExpanded()) {
                temp->contractTail();
            }
        }

    }
	qDebug() << "_preceedingBondedNbr: " << _precedingBondedNbr << endl;
	qDebug() << "_followingBondedNbr: " << _followingBondedNbr << endl;


}



QString Caterpillar3Particle::inspectionText() const {
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

Caterpillar3Particle& Caterpillar3Particle::nbrAtLabel(int label) const {
    return AmoebotParticle::nbrAtLabel<Caterpillar3Particle>(label);
}

Caterpillar3Particle::Color Caterpillar3Particle::getRandColor() const {
    // Randomly select an integer and return the corresponding color via casting.
    return static_cast<Color>(2);
}

Caterpillar3Particle::Color Caterpillar3Particle::getColor(Caterpillar3Particle::State state) const {
    // Randomly select an integer and return the corresponding color via casting.
    if(state == State::Leader)
        return static_cast<Color>(0);
    \
    else if (state == State::Follower)
        return static_cast<Color>(1);
    else if (state == State::SemiRetired)
        return static_cast<Color>(2);
    else if (state == State::Terminated)
        return static_cast<Color>(3);
}



Caterpillar3System::Caterpillar3System(unsigned int numParticles) {


    std::set<Node> occupied;
    unsigned int numParticlesAdded = 0;
    int nameOfParticle = 1;
    int x = 2;
    int y = 2;
    Node leaderNode(x, y);
    numParticlesAdded++;
    Caterpillar3Particle* leader =
        new Caterpillar3Particle(leaderNode, -1, 0, *this,
            Caterpillar3Particle::State::Leader);
    insert(leader);
    leader->_followingBondedNbr = -1;
    leader->_precedingBondedNbr = 3;
    leader->_receive_msg = -1;
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
        Caterpillar3Particle* follower =
            new Caterpillar3Particle(followerNode, -1, 0, *this,
                Caterpillar3Particle::State::Follower);
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
