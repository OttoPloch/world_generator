#pragma once

#include "../../core/common.hpp"
#include "../../graphics/global_animation.hpp"
#include "../../graphics/vertex_group.hpp"
#include "tile_types.hpp"
#include "tags/tile_tag.hpp"
#include "tags/mineable_tag.hpp"
#include "tile_template.hpp"

class Game;
class Chunk;

class Tile
{
public:
    Tile();
    
    Tile(Game* game, Chunk* chunk, sf::Vector2i localPosition, const TileTemplate& t, int z = 0);

    sf::FloatRect getCollRect();

    template<typename T>
    T* getTag()
    {
        for (auto& t : tags)
        {
            if (auto casted = dynamic_cast<T*>(t.get())) return casted;
        }
        
        return nullptr;
    }

    template<typename T, typename... Args>
    T& addTag(Args&&... args)
    {
        T* comp = new T(std::forward<Args>(args)...);
        tags.emplace_back(comp);
        return *comp;
    }

    template<typename T>
    void removeTag()
    {
        for (auto i = tags.begin(); i != tags.end();)
        {
            if (auto casted = dynamic_cast<T*>(i->get()))
            {
                i = tags.erase(i);
                break;
            }
            else
            {
                ++i;
            }
        }
    }

    void update(float dt);

    Chunk* chunk;

    sf::Vector2i localPosition;

    float size;

    TileType type;

    bool collides;

    sf::Vector2f collOffsetFraction;

    sf::Vector2f collSizeFraction;

    std::string colliderName;

    VertexGroup myVerts;

    GlobalAnimation* globalAnimation;
    std::unique_ptr<Animation> animation;
    float animSpeedMult;

    int z;

    std::vector<std::unique_ptr<TileTag>> tags;
    
    Game* game;
};