#include "World.h"
#include <algorithm>
#include <string>
#include "../Helper.h"


// Should be called whenever entities changes or existing entities change their y position
void World::sortGameObjects() {
  // NOTE: Sort player and tile/object based on their y (depth)
  // THEN: Iterate & call render for player and object
  // NOTE: This is necessary because the sprite of whatever would not
  // fit the 96x80 grid, so the object with lower y (farther away) will
  // render before object closer
  // basically, if player is in front of the chest, the chest sprite would not
  // appear over the player
  // and vice versa,
  // if player is behind, the player sprite would not appear over the chest
  std::sort(
      entities.begin(),
      entities.end(),
      [](const Entity* a, const Entity* b) {
      return a->y < b->y;
      });
}

void World::removeEntity(const std::string& tileId) {
  auto found = [tileId](Entity* entity) {
    return (entity->id == tileId);
  };
  auto foundEntity = std::find_if(entities.begin(), entities.end(), found);
  grid[(*foundEntity)->x][(*foundEntity)->y] = 0;
  entities.erase(foundEntity);
}

Room* World::buildRoom(const std::string& roomId, nlohmann::json source) {
  // variables for constructing new room
  std::string roomInfo = source["roomInfo"];
  std::vector<Tile*> roomTiles;
  std::vector<TransitionTile*> roomTransitions;

  // TODO: rename this
  std::string roomFilePath = "./assets/room/" + roomId;
  Texture2D roomBackground = LoadTexture((roomFilePath + "-bg.png").c_str());

  // Parse Tiles
  for (const auto& tileData : source["specialTiles"]) {
    std::string tileID = tileData[0].get<std::string>();
    bool blocked = tileData[1].get<bool>();
    int tileX = tileData[2].get<int>();
    int tileY = tileData[3].get<int>();

    std::string tileType = Helper::parseGameObjectType(tileID);
    Tile* tile = nullptr;
    if (tileType == "battle") tile = new CombatTile(tileID.c_str(), tileX, tileY, blocked);
    else if (tileType == "npc") tile = new NpcTile(tileID.c_str(), tileX, tileY, blocked);
    else if (tileType == "chest") tile = new ChestTile(tileID.c_str(), tileX, tileY, blocked);
    else if (tileType == "door") tile = new DoorTile(tileID.c_str(), tileX, tileY, blocked);
    else tile = new Tile(tileID.c_str(), tileX, tileY, blocked);

    roomTiles.push_back(tile);
  }

  // Parse transition tiles
  for (const auto& transitionData : source["transitionTiles"]) {
    std::string destinationRoomId = transitionData[0].get<std::string>();
    int tileX = transitionData[1].get<int>();
    int tileY = transitionData[2].get<int>();
    int enterX = transitionData[3].get<int>();
    int enterY = transitionData[4].get<int>();
    TransitionTile* transitionTile = new TransitionTile(tileX, tileY, enterX, enterY, destinationRoomId.c_str());
    roomTransitions.push_back(transitionTile);
  }
  // Construct new room and add it to the rooms vector
  Room* roomToBuild = new Room(roomId, roomInfo, roomTiles, roomTransitions, roomBackground);
  return roomToBuild;
}

void World::setRoom(Room* roomToSet) {
  // Set current room as input
  currentRoom = roomToSet;

  // --------------------- Render/build the room
  // Reinitialize display data
  entities.clear();
  transitionTiles.clear();
  // Refill display data with room data
  setGridFromString(roomToSet->roomInfo);
  for (Tile* tile : roomToSet->tiles) {
    if (tile->isBlockMovement) {
      grid[tile->x][tile->y] = 1;
    }
  }

  std::copy(roomToSet->tiles.begin(), roomToSet->tiles.end(), std::back_inserter(entities));
  std::copy(roomToSet->transitionTiles.begin(), roomToSet->transitionTiles.end(), std::back_inserter(transitionTiles));
  // Add players back to entities vector
  for (Player* pc : players) {
    entities.push_back(pc);
  }
  background = roomToSet->background;
  sortGameObjects();
}

void World::setGridFromString(std::string roomInfo) {
  // Create vector of secion info
  std::vector<std::pair<char, int>> sections;
  std::string::iterator ch = roomInfo.begin();
  while (ch != roomInfo.end()) {
    char kind = *(ch++);
    std::string number = "";
    while (isdigit(*ch)) {
      number += *(ch++);
    }
    int num;
    try {
      num = std::stoi(number);
    }
    catch (const std::exception& e) {
      fprintf(stderr, "Didn't work: %s\n", e.what());
      return;
    }
    sections.push_back(std::make_pair(kind, num));
  }
  int col = 0, row = 0;
  int bitVal;
  for (auto &section : sections) {
    // Get the value that needs to be applied
    if (section.first == 'w') bitVal = 1;
    else bitVal = 0;
    // Set the bitmap position accordingly
    for (int i = 0; i < section.second; ++i) {
      if (row == 12) {
        // About to try to put data into grid[0][12], which does not exist
        fprintf(stderr, "Too much data. Ending loop\n");
        return;
      }
      grid[col++][row] = bitVal;
      if (col == 20) {
        ++row;
        col = 0;
      }
    }
  }
  if (row != 12 && col != 0) fprintf(stderr, "roomInfo did not have complete data. Returning incomplete bitmap\n");
}


Room* World::findRoom(const std::string& roomId) {
  if (rooms.empty()) return nullptr;
  fprintf(stderr, "Room vec is not empty.\n");
  auto found = [roomId](Room* room) {
    return (room->roomId == roomId);
  };
  auto foundRoom = std::find_if(rooms.begin(), rooms.end(), found);
  if (foundRoom == rooms.end()) return nullptr;
  return *foundRoom;
}

void World::initializeWorld(std::string roomId) {
  loadRoom(roomId);

  player = new Player(
      "player-01",
      playerX,
      playerY,
      playerFacing,
      playerInventory
      );
  entities.push_back(player);
  players.push_back(player);
}