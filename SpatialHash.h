#pragma once

#include <base.h>
#include <ECS/Entity.h>

class SpatialHash {
public:
    int binSize = 5;

    //hash of bin -> uid -> entity
    std::unordered_map<u_int64_t, std::unordered_map<u_int64_t, std::shared_ptr<Entity>>> spatialIndex;

    u_int64_t add(std::shared_ptr<Entity> &entity);

    u_int64_t modify(u_int64_t previousBinHash, std::shared_ptr<Entity> &entity);

    void drop(u_int64_t binHash, const std::shared_ptr<Entity> &entity);

    void drop(u_int64_t binHash, u_int64_t id);

    u_int64_t bins();

    void clear();

    glm::ivec2 getBinCoordinates(const glm::vec2 &position);

    std::unordered_map<u_int64_t, std::shared_ptr<Entity> > getBin(const glm::ivec2 &binCoordinates);

    std::unordered_map<u_int64_t, std::shared_ptr<Entity> > getBin(std::shared_ptr<Entity> &entity);

    std::unordered_map<u_int64_t, std::shared_ptr<Entity> > getBin(u_int64_t hash);

    std::unordered_map<u_int64_t, std::shared_ptr<Entity> > getBinByWC(const glm::vec2 &worldCoordinates);

    LocalArea getLocalArea(std::shared_ptr<Entity> &entity, int radiusSizeBins = 1);

    u_int64_t getHash(const glm::ivec2 &binCoordinates);

    u_int64_t getHashByWC(const glm::vec2 &worldCoordinates);

    u_int64_t getHash(std::shared_ptr<Entity> &entity);

    explicit SpatialHash(int binSize);
};