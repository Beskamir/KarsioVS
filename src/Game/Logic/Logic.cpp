//
// Created by Sebastian on 12/23/2017.
//

#include "Logic.h"
#include <glm/gtx/vector_angle.hpp>

Logic::Logic(){
};
void Logic::cameraMovement(Movement* newMovement) {
    moveCamera = newMovement;
}

//void Logic::playerMovement(vehicleInput *newMovement, Entity* targetEnt) {
void Logic::playerMovement(Entity* targetEnt) {
	ControllableComponent* con = static_cast<ControllableComponent*>(targetEnt->getComponent(CONTROLLABLE));
	DriveComponent* temp = static_cast<DriveComponent*>(targetEnt->getComponent(DRIVE));
	//if (temp) temp->setInputs(*newMovement);
	con->getInput();
	temp->setInputs(con->input);
}

void Logic::aiMovement(Entity* entity) {
    // First thing we need to do is see if we are close enough to the waypoint to head toward the next one
    AIComponent* ai = static_cast<AIComponent*>(entity->getComponent(AI));
	
	// populate waypoints
	if (ai->waypoints.empty()) {
		ai->setWaypoints(path);
	}
	
    // Check if the distance is less than some threshold. If so, then we have "arrived at the waypoint", head to the next waypoint.
    if (glm::distance(ai->getCurrentWaypoint(), entity->getCoarsePosition()) < 1.0f) {
        ai->nextWaypoint();
    }	
	
    // Now we need to determine where to steer based on where the point is. Currently does not support reverse.
    DriveComponent* aiDrive = static_cast<DriveComponent*>(entity->getComponent(DRIVE));
    bool steerLeft = false, steerRight = false, accel = true;

    float oangle = glm::orientedAngle(glm::normalize(ai->getCurrentWaypoint() - entity->getCoarsePosition()),entity->getForwardVector(), glm::vec3(0,1,0));
	
	// if at last waypoint, move directly towards goal
	if (ai->currentWaypointIndex == path.size() - 1) {
		oangle = glm::orientedAngle(glm::normalize(ai->getCurrentWaypoint() - entity->getPosition()), entity->getForwardVector(), glm::vec3(0, 1, 0));
	}

	// get speed of vehicle to make it slow down while turning
	glm::vec3 velocity = PhysicsEngine::toglmVec3(static_cast<PhysicsComponent*>(entity->getComponent(PHYSICS))->getRigidBody()->getLinearVelocity());
	float speed = glm::dot(velocity, entity->getForwardVector());

	// TODO this should be refined so that the AI doesn't spin out
    if (oangle > .1f) {
        steerLeft = true;
		
    } else if (oangle < -.1f){
        steerRight = true;
    }
	// slow down when turning
	if (speed > 5 && (steerLeft || steerRight)) {
		accel = false;
	}
					// accel, brake, handbrake
    aiDrive->setInputs(accel, false, false, steerLeft, steerRight, false, false);
}

void Logic::bindCamera(Camera* aCamera) {
    camera = aCamera;
}

glm::vec3 getCoarsePosition(glm::vec3 position) {
	float x = position.x;
	float y = position.y;
	float z = position.z;
	float gridSize = 10; // larger size = larger grid
	x = (x >= 0) ? (x - fmod(x, gridSize)) / gridSize : (x + fmod(x, gridSize)) / gridSize;
	y = (y - fmod(y, gridSize)) / gridSize;
	z = (z >= 0) ? (z - fmod(z, gridSize)) / gridSize : (z + fmod(z, gridSize)) / gridSize;

	return glm::vec3(x, y, z);
}

void Logic::findPath(AStar::Generator* generator, glm::vec3 start, glm::vec3 goal) {
	glm::vec3 coarseStart = getCoarsePosition(start);
	glm::vec3 coarseGoal = getCoarsePosition(goal);
	std::vector<vec2> p = generator->findPath({ vec2(coarseStart.x, coarseStart.z) }, { vec2(coarseGoal.x, coarseGoal.z) });

	// convert back to vec3 ...
	path.resize(p.size() + 1);
	for (int i = p.size() - 1; i > 0; i--) {
		path[i] = vec3(p[i].x, 0, p[i].y);
	}

	// add exact position of goal to the end
	path[0] = vec3(goal.x, 0, goal.z);
}

// rukiya's added stuff
void Logic::findPath(AStar::Generator* generator, Entity* start, Entity* goal) {
	findPath(generator, start->getPosition(), goal->getPosition());
}



Logic::~Logic() = default;