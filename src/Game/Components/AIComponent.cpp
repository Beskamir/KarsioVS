#include "AIComponent.h"



AIComponent::AIComponent() : Component(AI)
{
    // Currently hard coded waypoints, 4 corners to travel to.
    currentWaypointIndex = 0;
    waypoints.emplace_back(glm::vec3(50, 0, 50));
    waypoints.emplace_back(glm::vec3(-50, 0, 50));
    waypoints.emplace_back(glm::vec3(-50, 0, -50));
    waypoints.emplace_back(glm::vec3(50, 0, -50));
}

glm::vec3 AIComponent::getCurrentWaypoint() {
    return waypoints[currentWaypointIndex];
}

void AIComponent::nextWaypoint() {
    currentWaypointIndex = (currentWaypointIndex + 1) % waypoints.size();
}

AIComponent::~AIComponent()
{
}
