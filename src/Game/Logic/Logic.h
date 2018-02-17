//
// Created by Sebastian on 12/23/2017.
//
#pragma once
#ifndef RENDERENGINE_LOGIC_H
#define RENDERENGINE_LOGIC_H
#include "../../Engine/Entity.h"
#include "../../Engine/ComponentManager.h"
#include "../Components/DriveComponent.h"
#include "../../Main/initOpenGL/OpenGL_Deps.h"
#include "../../Main/initOpenGL/OpenGL.h"
#include "../Camera.h"

#include "AStar.h"

class Logic {
Camera * camera;
Movement * moveCamera;
Entity * player;
vehicleInput * movePlayer;
DriveComponent * playerDrive;
public:        
    Logic();
    void cameraMovement(Movement* newMovement);
    void playerMovement(vehicleInput* newMovement, Entity* targetEnt);
    void aiMovement(Entity* entity);
    void bindCamera(Camera* aCamera);
	void findPath(AStar::Generator* map, Entity* player, Entity* goal);
   ~Logic();
};


#endif //RENDERENGINE_LOGIC_H
