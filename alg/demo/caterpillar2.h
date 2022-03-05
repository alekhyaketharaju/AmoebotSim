#ifndef CATERPILLAR2_H
#define CATERPILLAR2_H

#include <QString>

#include "core/amoebotparticle.h"
#include "core/amoebotsystem.h"



class Caterpillar2Particle : public AmoebotParticle {
public:
	enum class State {
		Leader,
		Root,
		Follower,
		Retired,
		Terminated
	};

	enum class Color {
		Red, //leader
		Yellow, //root
		Green, //follower
		Blue, //retired
		Black //terminated
	};

	// Constructs a new particle with a node position for its head, a global
	// compass direction from its head to its tail (-1 if contracted), an offset
	// for its local compass, a system which it belongs to, and an initial state.
	Caterpillar2Particle(const Node head, const int globalTailDir,
		const int orientation, AmoebotSystem& system,
		State _state);

	// Executes one particle activation.
	void activate() override;

	// Functions for altering the particle's color. headMarkColor() (resp.,
	// tailMarkColor()) returns the color to be used for the ring drawn around the
	// particle's head (resp., tail) node. In this demo, the tail color simply
	// matches the head color. headMarkDir returns the label of the port
	// on which the head marker is drawn; in this demo, this points from the
	// follower dance partner to its leader.
	int headMarkColor() const override;
	int headMarkDir() const override;
	int tailMarkColor() const override;

	// Returns the string to be displayed when this particle is inspected; used
	// to snapshot the current values of this particle's memory at runtime.
	QString inspectionText() const override;

	// Gets a reference to the neighboring particle incident to the specified port
	// label. Crashes if no such particle exists at this label; consider using
	// hasNbrAtLabel() first if unsure.
	Caterpillar2Particle& nbrAtLabel(int label) const;
	int isPointedByFirstNode(std::vector<std::vector<int>> firstNode, int x, int y) const;
	int isPointedByNextNode(std::vector<std::vector<int>> nextNode, int x, int y) const;
	int labelPointing(std::vector<std::vector<int>> nextNode, std::vector<std::vector<int>> directionNode, int x, int y) const;
	std::vector<int> getNodeFromLabel(int x, int y, int label) const;
	bool checkIfNodeIsNbr(int x1, int y1, int x2, int y2) const;
	int getLabelFromNode(int x_current, int y_current, int x_pre, int y_pre) const;

protected:
	// Returns a random Color.
	
	Color getColor(State state) const;
	void rotate();

	// Member variables.
	State _state;
	Color _color;
	int _name;
	//int layer2 = 1;
	int _followingBondedNbr;
	int _precedingBondedNbr;
	int _rotate_status_head; //-1 - do nothing, 1 - init rotate, 2 - Ack, 3 - Completed rotate
	int _rotate_status_tail;
	
	int _receive_head; 

	int _receive_tail; 
private:
	friend class Caterpillar2System;
};

class Caterpillar2System : public AmoebotSystem {

public:

	// Constructs a system of the specified number of BallroomDemoParticles in
	// "dance partner" pairs enclosed by a rhombic ring of objects.
	Caterpillar2System(unsigned int numParticles = 30);
};



#endif // CATERPILLAR2_H
