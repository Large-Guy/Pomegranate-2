#ifndef POMEGRANATEENGINE_ECS_TYPEDEFS_H
#define POMEGRANATEENGINE_ECS_TYPEDEFS_H
#include <vector>
#include <unordered_set>
#include <unordered_map>

struct ArchetypeRecord;
typedef unsigned int ComponentID;
typedef unsigned int EntityID;
typedef unsigned int ArchetypeID;
typedef unsigned int GroupID;
typedef std::unordered_set<ComponentID> Type;
typedef std::unordered_map<ArchetypeID, ArchetypeRecord> ArchetypeMap;


#endif //POMEGRANATEENGINE_ECS_TYPEDEFS_H
