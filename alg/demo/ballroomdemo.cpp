#include "alg/demo/ballroomdemo.h"

BallroomDemoParticle::BallroomDemoParticle(const Node head,
	const int globalTailDir,
	const int orientation,
	AmoebotSystem &system,
	State state)
	: AmoebotParticle(head, globalTailDir, orientation, system),
	_state(state),
	_partnerHead(-1),
	_partnerTail(-1) {
	_color = getRandColor();
}

void BallroomDemoParticle::activate() {
	_color = getLeaderColor();
	//int dir = randDir();
	//bool flag = true;
	if (_state == State::Leader) {
		_color = getLeaderColor();
		if (isContracted()) {
			// Attempt to expand into an random adjacent position.
			int expandDir = randDir();

			if (canExpand(expandDir)) {
				//cout << "Expanding leader" << endl;
				expand(expandDir);

			}
		}
		else {//Leader is expanded
			// Find the follower partner and pull it, if possible.
			for (int label : tailLabels()) {
				if (hasNbrAtLabel(label) && nbrAtLabel(label)._partnerHead != -1
					&& pointsAtMe(nbrAtLabel(label), nbrAtLabel(label)._partnerHead)) {
					if (canPull(label)) {
						nbrAtLabel(label)._partnerHead =
							dirToNbrDir(nbrAtLabel(label), (tailDir() + 3) % 6);
						pull(label);
					}
					break;
				}
			}



		}
		//_color = getRandColor();
	}
	else {  // _state == State::Follower.
		_color = getRandColor();
		if (isExpanded()) {
			if (_partnerTail == -1)
			{
				contractTail();
			}
			else {
				for (int label : tailLabels()) {
					if (hasNbrAtLabel(label) && nbrAtLabel(label)._partnerHead != -1
						&& pointsAtMe(nbrAtLabel(label), nbrAtLabel(label)._partnerHead)) {
						if (canPull(label)) {
							nbrAtLabel(label)._partnerHead =
								dirToNbrDir(nbrAtLabel(label), (tailDir() + 3) % 6);
							pull(label);
						}
						break;
					}

				}
			}

		}

	}
	/*(else { //If follower is in contracted position
		if (canPush(_partnerHead)) {
			// Update the pair's color.
			auto leader = nbrAtLabel(_partnerHead);
			// Push the leader and update the partner direction label.
			int leaderContractDir = nbrDirToDir(leader, (leader.tailDir() + 3) % 6);
			push(_partnerHead);
			_partnerHead = leaderContractDir;
		}

	}*/
	_color = getRandColor();
}



int BallroomDemoParticle::headMarkColor() const {
	switch (_color) {
	case Color::Red:    return 0xff0000;
	case Color::Green:	return 0x00ff00;
	case Color::Blue:   return 0x0000ff;

	}

	return -1;
}

int BallroomDemoParticle::headMarkDir() const {
	return _partnerTail;
}

int BallroomDemoParticle::tailMarkColor() const {
	return headMarkColor();
}

QString BallroomDemoParticle::inspectionText() const {
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
	text += "  partnerTail: " + QString::number(_partnerTail);

	return text;
}

BallroomDemoParticle& BallroomDemoParticle::nbrAtLabel(int label) const {
	return AmoebotParticle::nbrAtLabel<BallroomDemoParticle>(label);
}

BallroomDemoParticle::Color BallroomDemoParticle::getRandColor() const {
	// Randomly select an integer and return the corresponding color via casting.
	return static_cast<Color>(randInt(1, 3));
}

BallroomDemoParticle::Color BallroomDemoParticle::getLeaderColor() const {
	// Randomly select an integer and return the corresponding color via casting.
	return static_cast<Color>(0);
}


BallroomDemoSystem::BallroomDemoSystem(unsigned int numParticles) {


	std::set<Node> occupied;
	unsigned int numParticlesAdded = 0;
	int x = 2;
	int y = 2;
	Node leaderNode(x, y);
	numParticlesAdded++;
	BallroomDemoParticle* leader =
		new BallroomDemoParticle(leaderNode, -1, randDir(), *this,
			BallroomDemoParticle::State::Leader);
	insert(leader);
	leader->_partnerHead = -1;
	leader->_partnerTail = 0;
	//leader->_color = BallroomDemoParticle::Color BallroomDemoParticle::getLeaderColor();
	Node prev = leaderNode;
	occupied.insert(leaderNode);
	while (numParticlesAdded < numParticles) {
		// Choose an (x,y) position within the rhombus for the Leader and a random
		// adjacent node for its Follower partner.

		int followerDir = 0;
		//occupied.back()->_partnerTail = followerDir;
		Node followerNode = prev.nodeInDir(followerDir);
		BallroomDemoParticle* follower =
			new BallroomDemoParticle(followerNode, -1, randDir(), *this,
				BallroomDemoParticle::State::Follower);
		follower->_partnerHead = follower->globalToLocalDir((followerDir + 3) % 6);
		if (numParticles - numParticlesAdded == 1) {
			follower->_partnerTail = -1;
			insert(follower);
			occupied.insert(followerNode);
			break;
		}
		else {
			follower->_partnerTail = 0;
		}
		insert(follower);
		occupied.insert(followerNode);
		prev = followerNode;
		numParticlesAdded++;


	}
}