#include "SpatialHash.h"
#include <glm/gtx/hash.hpp>

u_int64_t SpatialHash::add(std::shared_ptr<Entity> &entity) {
    auto hash = getHash(entity);
    spatialIndex[hash][entity->id] = entity;

    entity->binHash = hash;
    return hash;
}

glm::ivec2 SpatialHash::getBinCoordinates(const glm::vec2 &position) {
    float x = std::floor(position.x / (float) binSize) * (float) binSize;
    float y = std::floor(position.y / (float) binSize) * (float) binSize;

    return {(int) x, (int) y + binSize};
}

u_int64_t SpatialHash::getHash(std::shared_ptr<Entity> &entity) {
    auto c = entity->position + glm::vec2{0.0, -1.0};
    return getHash(getBinCoordinates(c));
}


u_int64_t SpatialHash::modify(u_int64_t previousBinHash, std::shared_ptr<Entity> &entity) {
    auto hash = getHash(entity);

    if (previousBinHash == hash) {
        return hash;
    }

    drop(previousBinHash, entity);
    add(entity);
    entity->binHash = hash;

    return hash;
}


std::unordered_map<u_int64_t, std::shared_ptr<Entity>> SpatialHash::getBin(const glm::ivec2 &binCoordinates) {
    return spatialIndex[getHash(binCoordinates)];
}

void SpatialHash::clear() {
    spatialIndex.clear();
}

u_int64_t SpatialHash::getHash(const glm::ivec2 &binCoordinates) {
    return std::hash<glm::vec2>()(binCoordinates);
}

u_int64_t SpatialHash::getHashByWC(const glm::vec2 &worldCoordinates) {
    return std::hash<glm::vec2>()(getBinCoordinates(worldCoordinates));
}

std::unordered_map<u_int64_t, std::shared_ptr<Entity> > SpatialHash::getBin(std::shared_ptr<Entity> &entity) {
    return this->getBin(getBinCoordinates(entity->position));
}

u_int64_t SpatialHash::bins() {
    return spatialIndex.size();
}

void SpatialHash::drop(u_int64_t binHash, const std::shared_ptr<Entity> &entity) {
    spatialIndex[binHash].erase(entity->id);
}

void SpatialHash::drop(u_int64_t binHash, u_int64_t id) {
    spatialIndex[binHash].erase(id);
}

SpatialHash::SpatialHash(int binSize) : binSize(binSize) {}

std::unordered_map<u_int64_t, std::shared_ptr<Entity>> SpatialHash::getBin(u_int64_t hash) {
    return spatialIndex[hash];
}


LocalArea SpatialHash::getLocalArea(std::shared_ptr<Entity> &playerEntity, int radiusSizeBins) {
    LocalArea area;

    //top left coord
    auto currentBinCoords = getBinCoordinates(playerEntity->position);
    int totalBins = radiusSizeBins * 2 + 1;

    glm::ivec2 bottomLeft = {currentBinCoords.x - (radiusSizeBins * binSize), currentBinCoords.y - (radiusSizeBins * binSize)};

    for (int i = 0; i < totalBins; i++) {
        for (int j = 0; j < totalBins; j++) {
            glm::ivec2 offset = {i * binSize, j * binSize};
            glm::ivec2 bcoord = bottomLeft + offset;

            auto hash = getHash(bcoord);
            auto entities = getBin(hash);

            area.entities.insert(entities.begin(), entities.end());
            area.bins.emplace_back(hash);
        }
    }

    return area;
}

std::unordered_map<u_int64_t, std::shared_ptr<Entity> > SpatialHash::getBinByWC(const glm::vec2 &worldCoordinates) {
    return getBin(getHashByWC(worldCoordinates));
}



