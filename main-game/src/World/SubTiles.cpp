#include "Tile.h"
#include "../../external-libs/nlohmann/json.hpp"
#include "../Helper.h"

// ---------------------------------------------- Combat Tile
CombatTile::CombatTile(
    const std::string& tileId,
    int tileX,
    int tileY,
    bool isBlockMovement
) : Tile(tileId, tileX, tileY, isBlockMovement)
{
  // Additional constructor functionality to add to base constructor

  // Rendering values
  // offsetX = 0.8;
  // offsetY = 1.1;
  // spriteWidth = 16;
  // spriteHeight = 14;
  // xScale = 0.8;
  // yScale = 1.2;
  // sprite = Helper::loadTexture("sprite path here");
}

std::pair<std::string, std::string> CombatTile::interact() {

  return Tile::interact();
}

// ---------------------------------------------- NPC Tile
NpcTile::NpcTile(
    const std::string& tileId,
    int tileX,
    int tileY,
    bool isBlockMovement
) : Tile(tileId, tileX, tileY, isBlockMovement)
{
  // Additional constructor functionality to add to base constructor
  const std::string npcFilePath = "./json/npc/";
  const std::string jsonFileType = ".json";
  const std::string fullFilePath = npcFilePath + id + jsonFileType;
  nlohmann::json root;
  std::ifstream jsonFile(fullFilePath);
  if (jsonFile.is_open()) {
    try {
      jsonFile >> root;
      // Parse NPC data
      std::string name = root["name"].get<std::string>();
      std::string text = root["text"].get<std::string>();
      // Add to second output string
      tileText = name + ": " + text;
    }
    catch (const std::exception& e) {
      fprintf(stderr, "%s JSON parsing failed: %s\n", id, e.what());
    }
  }

  // Rendering values
  // offsetX = 0.8;
  // offsetY = 1.1;
  // spriteWidth = 16;
  // spriteHeight = 14;
  // xScale = 0.8;
  // yScale = 1.2;
  // sprite = Helper::loadTexture("sprite path here");
}

std::pair<std::string, std::string> NpcTile::interact() {
  return Tile::interact();
}

// ---------------------------------------------- Chest Tile
ChestTile::ChestTile(
    const std::string& tileId,
    int tileX,
    int tileY,
    bool isBlockMovement
) : Tile(tileId, tileX, tileY, isBlockMovement)
{
  const std::string chestFilePath = "./json/chest/";
  const std::string jsonFileType = ".json";
  const std::string fullFilePath = chestFilePath + id + jsonFileType;
  nlohmann::json root;
  std::ifstream jsonFile(fullFilePath);
  if (jsonFile.is_open()) {
    try {
      jsonFile >> root;

      tileText = "Obtained:";

      int chestKeys = root["keys"];
      inventory.setKeys(chestKeys);
      if (inventory.hasKey()) tileText = tileText + " " + std::to_string(chestKeys) + " key \\\\";

      // Parse Chest
      for (const auto& itemData : root["items"]) {
        std::string itemID = itemData["id"].get<std::string>();
        int quantity = itemData["quantity"].get<int>();
        tileText = tileText + " " + std::to_string(quantity) + " " + itemID.c_str() + " \\\\";

        // TODO: Fill chest inventory here:
        inventory.AddItem(itemID, quantity);
      }
    }
    catch (const std::exception& e) {
      fprintf(stderr, "JSON parsing failed: %s\n", e.what());
    }
  }

  // Rendering values
  offsetX = 0.8;
  offsetY = 1.1;
  spriteWidth = 16;
  spriteHeight = 14;
  xScale = 0.8;
  yScale = 1.2;
  sprite = Helper::loadTexture("chest.png");
}

// ---------------------------------------------- Door Tile
DoorTile::DoorTile(
    const std::string& tileId,
    int tileX,
    int tileY,
    bool isBlockMovement
) : Tile(tileId, tileX, tileY, isBlockMovement)
{
  // Additional constructor functionality to add to base constructor

  // Rendering values
  // offsetX = 0.8;
  // offsetY = 1.1;
  // spriteWidth = 16;
  // spriteHeight = 14;
  // xScale = 0.8;
  // yScale = 1.2;
  // sprite = Helper::loadTexture("sprite path here");
}