#include "collision_attribute.hpp"

CollisionAttribute::CollisionAttribute(Entity* myEntity, GamePosition position, sf::Vector2f size, std::vector<Entity>* entities, bool active) : Attribute("collision")
{
    this->myEntity = myEntity;

    rect.init(position, size);

    this->entities = entities;

    this->active = active;
}

void CollisionAttribute::tick()
{
    for (int i = 0; i < entities->size(); i++)
    {
        Entity* entity = &(*entities)[i];

        if (entity->getID() != myEntity->getID())
        {
            if (entity->getCollision() != nullptr)
            {
                CollisionRect* other = &entity->getCollision()->rect;
                
                if (rect.left() < other->right() && rect.right() > other->left() && rect.top() < other->bottom() && rect.bottom() > other->top())
                {   
                    float leftDiff = abs(rect.center().x - other->left());
                    float rightDiff = abs(rect.center().x - other->right());
                    float topDiff = abs(rect.center().y - other->top());
                    float bottomDiff = abs(rect.center().y - other->bottom());

                    float angle = getAngleRadians(rect.center(), other->center());

                    int xDirection = 1;

                    if (abs(rect.center().x - rect.lastPosition.x) > .01f) xDirection = toInt((rect.center().x - rect.lastPosition.x) / (-1 * abs(rect.center().x - rect.lastPosition.x)));

                    // as in rise / run, run is 1
                    float rise = roundToPlace(sin(getAngleRadians(rect.center(), rect.lastPosition)), 5) * xDirection;

                    float yIntercept = rect.center().y - (rise * rect.center().x);
                    float xIntercept = (-1 * yIntercept) / rise;
 
                    float xIfSnapLeft = other->left() - rect.halfLength();
                    float xIfSnapRight = other->right() + rect.halfLength();
                    float yIfSnapTop = other->top() - rect.halfWidth();
                    float yIfSnapBottom = other->bottom() + rect.halfWidth();

                    float yOnLineIfSnapLeft = rise * xIfSnapLeft + yIntercept;
                    float yOnLineIfSnapRight = rise * xIfSnapRight + yIntercept;
                    float xOnLineIfSnapTop = rise * yIfSnapTop + xIntercept;
                    float xOnLineIfSnapBottom = rise * yIfSnapBottom + xIntercept;

                    bool canSnapLeft = false;
                    bool canSnapRight = false;
                    
                    if (abs(rise) != 1)
                    {
                        canSnapLeft = (abs(yOnLineIfSnapLeft - other->center().y) < rect.halfWidth() + other->halfWidth());
                        canSnapRight = (abs(yOnLineIfSnapRight - other->center().y) < rect.halfWidth() + other->halfWidth());

                        std::cout << yOnLineIfSnapLeft << " - " << other->center().y << " < " << rect.halfWidth() << " + " << other->halfWidth() << '\n';
                        std::cout << yOnLineIfSnapRight << " - " << other->center().y << " < " << rect.halfWidth() << " + " << other->halfWidth() << '\n';
                    }
                    
                    bool canSnapTop = false;
                    bool canSnapBottom = false;
                    
                    if (!std::isinf(xIntercept))
                    {
                        canSnapTop = (abs(xOnLineIfSnapTop - other->center().x) < rect.halfLength() + other->halfLength());
                        canSnapBottom = (abs(xOnLineIfSnapBottom - other->center().x) < rect.halfLength() + other->halfLength());

                        std::cout << xOnLineIfSnapTop << " - " << other->center().x << " < " << rect.halfLength() << " + " << other->halfLength() << '\n';
                        std::cout << xOnLineIfSnapBottom << " - " << other->center().x << " < " << rect.halfLength() << " + " << other->halfLength() << '\n';
                    }
                    
                    int candidates = canSnapLeft + canSnapRight + canSnapTop + canSnapBottom;

                    std::cout << "STEP 1 COMPLETE:\n";
                    std::cout << "position: " << rect.center().x << ", " << rect.center().y << "; last position: " << rect.lastPosition.x << ", " << rect.lastPosition.y << ";\n"; 
                    std::cout << "differences: " << leftDiff << "; " << rightDiff << "; " << topDiff << "; " << bottomDiff << ";\n";
                    std::cout << "angle: " << angle << "; xDirection: " << xDirection << "; rise: " << rise << "; yIntercept: " << yIntercept << "; xIntercept: " << xIntercept << ";\n";
                    std::cout << "first snap values: " << xIfSnapLeft << "; " << xIfSnapRight << "; " << yIfSnapTop << "; " << yIfSnapBottom << ";\n";
                    std::cout << "second snap values: " << yOnLineIfSnapLeft << "; " << yOnLineIfSnapRight << "; " << xOnLineIfSnapTop << "; " << xOnLineIfSnapBottom << ";\n";
                    std::cout << "candidates: " << canSnapLeft << "; " << canSnapRight << "; " << canSnapTop << "; " << canSnapBottom << ";\n"; 
                    std::cout << "END OF STEP 1\n";

                    if (candidates == 1)
                    {
                        if (canSnapLeft) rect.setPosition({xIfSnapLeft, yOnLineIfSnapLeft});
                        if (canSnapRight) rect.setPosition({xIfSnapRight, yOnLineIfSnapRight});
                        if (canSnapTop) rect.setPosition({xOnLineIfSnapTop, yIfSnapTop});
                        if (canSnapBottom) rect.setPosition({xOnLineIfSnapBottom, yIfSnapBottom});

                        std::cout << "1\n";
                    }
                    else if (candidates > 1) // should only be 2 at most i think
                    {
                        sf::Vector2f snapLeft(xIfSnapLeft, yOnLineIfSnapLeft);
                        sf::Vector2f snapRight(xIfSnapRight, yOnLineIfSnapRight);
                        sf::Vector2f snapTop(xOnLineIfSnapTop, yIfSnapTop);
                        sf::Vector2f snapBottom(xOnLineIfSnapBottom, yIfSnapBottom);

                        float snapLeftDist = getDistance(rect.lastPosition, snapLeft);
                        float snapRightDist = getDistance(rect.lastPosition, snapRight);
                        float snapTopDist = getDistance(rect.lastPosition, snapTop);
                        float snapBottomDist = getDistance(rect.lastPosition, snapBottom);
                        
                        std::cout << "STEP 2: MORE THAN ONE CANDIDATE:\n";
                        std::cout << "snap positions: " << snapLeft.x << ", " << snapLeft.y << "; " << snapRight.x << ", " << snapRight.y << "; " << snapTop.x << ", " << snapTop.y << "; " << snapBottom.x << ", " << snapBottom.y << ";\n";
                        std::cout << "distances: " << snapLeftDist << "; " << snapRightDist << "; " << snapTopDist << "; " << snapBottomDist << ";\n";

                        bool solved = false;

                        if (canSnapLeft)
                        {
                            if (snapLeftDist < snapRightDist || !canSnapRight)
                            {
                                if (snapLeftDist < snapTopDist || !canSnapTop)
                                {
                                    if (snapLeftDist < snapBottomDist || !canSnapBottom)
                                    {
                                        std::cout << "snapLeft is the winner\n";

                                        rect.setPosition(snapLeft);

                                        solved = true;
                                    }
                                }
                            }
                        }
                        
                        if (canSnapRight)
                        {
                            if (snapRightDist < snapLeftDist || !canSnapLeft)
                            {
                                if (snapRightDist < snapTopDist || !canSnapTop)
                                {
                                    if (snapRightDist < snapBottomDist || !canSnapBottom)
                                    {
                                        std::cout << "snapRight is the winner\n";

                                        rect.setPosition(snapRight);

                                        solved = true;
                                    }
                                }
                            }
                        }

                        if (canSnapTop)
                        {
                            if (snapTopDist < snapLeftDist || !canSnapLeft)
                            {
                                if (snapTopDist < snapRightDist || !canSnapRight)
                                {
                                    if (snapTopDist < snapBottomDist || !canSnapBottom)
                                    {
                                        std::cout << "snapTop is the winner\n";

                                        rect.setPosition(snapTop);

                                        solved = true;
                                    }
                                }
                            }
                        }

                        if (canSnapBottom)
                        {
                            if (snapBottomDist < snapLeftDist || !canSnapLeft)
                            {
                                if (snapBottomDist < snapRightDist || !canSnapRight)
                                {
                                    if (snapBottomDist < snapTopDist || !canSnapTop)
                                    {
                                        std::cout << "snapBottom is the winner\n";

                                        rect.setPosition(snapBottom);

                                        solved = true;
                                    }
                                }
                            }
                        }
                        
                        if (!solved)
                        {
                            std::cout << "Error with collision resolution, no one candidate distance is smaller than all the rest or none are valid. Distances l,r,t,b are: ";
                            std::cout << snapLeftDist << ", " << snapRightDist << ", " << snapTopDist << ", " << snapBottomDist << ". Can snaps are: ";
                            std::cout << canSnapLeft << ", " << canSnapRight << ", " << canSnapTop << ", " << canSnapBottom << ".\n";
                        }
                            
                        std::cout << candidates << '\n';
                    }
                    else
                    {
                        std::cout << "Error with collision resolution, no candidates for snapping to one of (other->rect)'s sides.\n";
                    }


                    std::cout << "/////////////////////////////////////////////////////////\n";

                }
            }
        }
    }

    rect.lastPosition = rect.center();
}