#pragma once
#include "CGameObject.h"

/**
* @class CShip
* @brief Player ship object
*
* Derived from CGameObject.
*
* @author Jass Saini
*
*/
class CShip : public CGameObject
{
public:

    /**
    *
    * @brief Constructor for CShip class.
    *
    * @param board Size of game board
    *
    */
    CShip(cv::Size board);
};