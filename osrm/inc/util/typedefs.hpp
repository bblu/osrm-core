#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "strong_typedef.hpp"

#include <limits>
#include <cstddef>
#include <assert.h>

// OpenStreetMap node ids are higher than 2^32
OSRM_STRONG_TYPEDEF(uint64_t, OSMNodeID)
OSRM_STRONG_TYPEDEF(uint32_t, OSMWayID)

static const OSMNodeID SPECIAL_OSM_NODEID = OSMNodeID(std::numeric_limits<uint64_t>::max());
static const OSMWayID SPECIAL_OSM_WAYID = OSMWayID(std::numeric_limits<uint32_t>::max());

static const OSMNodeID MAX_OSM_NODEID = OSMNodeID(std::numeric_limits<uint64_t>::max());
static const OSMNodeID MIN_OSM_NODEID = OSMNodeID(std::numeric_limits<uint64_t>::min());
static const OSMWayID MAX_OSM_WAYID = OSMWayID(std::numeric_limits<uint32_t>::max());
static const OSMWayID MIN_OSM_WAYID = OSMWayID(std::numeric_limits<uint32_t>::min());

using OSMNodeID_weak = uint64_t;
using OSMEdgeID_weak = uint64_t;

using NodeID = unsigned int;
using EdgeID = unsigned int;
using EdgeWeight = int;

static const NodeID SPECIAL_NODEID = std::numeric_limits<unsigned>::max();
static const EdgeID SPECIAL_EDGEID = std::numeric_limits<unsigned>::max();
static const unsigned INVALID_NAMEID = std::numeric_limits<unsigned>::max();
static const unsigned INVALID_COMPONENTID = 0;
static const EdgeWeight INVALID_EDGE_WEIGHT = std::numeric_limits<int>::max();

#endif /* TYPEDEFS_H */
