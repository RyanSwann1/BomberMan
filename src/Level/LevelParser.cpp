#include "LevelParser.h"
#include "Level.h"
#include <tile\TileSheet.h>
#include <Locators\TileSheetManagerLocator.h>
#include <Managers\TileSheetManager.h>
#include <Managers\EntityManager.h>
#include <Utilities\tinyxml.h>
#include <Utilities\Base64.h>
#include <assert.h>

std::vector<TileLayer> parseTileLayers(const TiXmlElement& rootElement, const LevelDetails& levelDetails);
CollisionLayer parseCollisionLayer(const TiXmlElement& rootElement);
LevelDetails parseLevelDetails(const TiXmlElement& rootElement);
std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, const LevelDetails& levelDetails);
void parseTileSheets(const TiXmlElement& rootElement);
void parseEntities(const TiXmlElement& rootElement, EntityManager& entityManager);

void parseTileSheets(const TiXmlElement& rootElement)
{
	for (const auto* tileSheetElement = rootElement.FirstChildElement(); 
		tileSheetElement != nullptr; tileSheetElement = tileSheetElement->NextSiblingElement())
	{
		if (tileSheetElement->Value() != std::string("tileset"))
		{
			continue;
		}

		int tileSheetFirstGID = 0;
		tileSheetElement->Attribute("firstgid", &tileSheetFirstGID);

		auto& tileSheetManager = TileSheetManagerLocator::getTileSheetManager();
		if (tileSheetManager.hasTileSheet(tileSheetFirstGID))
		{
			continue;
		}
		
		std::string tileSheetName = tileSheetElement->Attribute("name");
		sf::Vector2i tileSetSize;
		int spacing = 0, margin = 0, tileSize = 0, firstGID = 0;
		tileSheetElement->FirstChildElement()->Attribute("width", &tileSetSize.x);
		tileSheetElement->FirstChildElement()->Attribute("height", &tileSetSize.y);
		tileSheetElement->Attribute("tilewidth", &tileSize);
		tileSheetElement->Attribute("spacing", &spacing);
		tileSheetElement->Attribute("firstgid", &firstGID);
		tileSheetElement->Attribute("margin", &margin);
		const int columns = tileSetSize.x / (tileSize + spacing);
		const int rows = tileSetSize.y / (tileSize + spacing);
		tileSheetManager.addTileSheet(tileSheetFirstGID, TileSheet(std::move(tileSheetName), tileSize, columns, rows, firstGID, margin, spacing));
	}
}

void parseEntities(const TiXmlElement & rootElement, EntityManager& entityManager)
{
	for (const auto* entityElementRoot = rootElement.FirstChildElement(); entityElementRoot != nullptr; entityElementRoot = entityElementRoot->NextSiblingElement())
	{
		if (entityElementRoot->Value() != std::string("objectgroup"))
		{
			continue;
		}

		
		for (const auto* entityElement = entityElementRoot->FirstChildElement(); entityElement != nullptr; entityElement = entityElement->NextSiblingElement())
		{
			int xPosition = 0, yPosition = 0;
			entityElement->Attribute("x", &xPosition);
			entityElement->Attribute("y", &yPosition);

			entityManager.addEntity(xPosition, yPosition);
		}
	}
}

std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, const LevelDetails& levelDetails)
{
	std::vector<std::vector<int>> tileData;

	std::string decodedIDs; //Base64 decoded information
	const TiXmlElement* dataNode = nullptr; //Store our node once we find it
	for (const TiXmlElement* e = tileLayerElement.FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("data"))
		{
			dataNode = e;
		}
	}
	assert(dataNode);

	Base64 base64;
	const TiXmlText* text = dataNode->FirstChild()->ToText();
	const std::string t = text->Value();
	decodedIDs = base64.base64_decode(t);

	const std::vector<int> layerColumns(levelDetails.m_size.x);
	for (int i = 0; i < levelDetails.m_size.y; ++i)
	{
		tileData.push_back(layerColumns);
	}

	for (int rows = 0; rows < levelDetails.m_size.y; ++rows)
	{
		for (int cols = 0; cols < levelDetails.m_size.x; ++cols)
		{
			tileData[rows][cols] = *((int*)decodedIDs.data() + rows * levelDetails.m_size.x + cols) - 1;
		}
	}

	return tileData;
}

LevelDetails parseLevelDetails(const TiXmlElement& rootElement)
{
	int width = 0, height = 0, tileSize = 0;
	rootElement.Attribute("width", &width);
	rootElement.Attribute("height", &height);
	rootElement.Attribute("tilewidth", &tileSize);

	return LevelDetails(sf::Vector2i(width, height), tileSize);
}

std::unique_ptr<Level> LevelParser::parseLevel(const std::string& fileDirectory, const std::string& levelName, EntityManager& entityManager)
{
	TiXmlDocument mapFile;
	const bool mapLoaded = mapFile.LoadFile(fileDirectory);
	assert(mapLoaded);

	const auto& rootElement = mapFile.RootElement();
	const auto levelDetails = parseLevelDetails(*rootElement);
	parseTileSheets(*rootElement);
	parseEntities(*rootElement, entityManager);

	return std::make_unique<Level>(parseTileLayers(*rootElement, levelDetails), parseCollisionLayer(*rootElement), levelDetails, levelName);
}

std::vector<TileLayer> parseTileLayers(const TiXmlElement & rootElement, const LevelDetails& levelDetails)
{
	std::vector<TileLayer> tileLayers;
	for (const auto* tileLayerElement = rootElement.FirstChildElement(); tileLayerElement != nullptr; tileLayerElement = tileLayerElement->NextSiblingElement())
	{
		if (tileLayerElement->Value() != std::string("layer"))
		{
			continue;
		}
		
		auto tileMap = decodeTileLayer(*tileLayerElement, levelDetails);
		tileLayers.emplace_back(std::move(tileMap), levelDetails);
	}

	assert(!tileLayers.empty());
	return tileLayers;
}

CollisionLayer parseCollisionLayer(const TiXmlElement & rootElement)
{
	std::vector<sf::Vector2i> collidablePositions;
	for (const auto* collisionLayerElement = rootElement.FirstChildElement(); collisionLayerElement != nullptr; 
		collisionLayerElement = collisionLayerElement->NextSiblingElement())
	{
		if (collisionLayerElement->Value() != std::string("objectgroup"))
		{
			continue;
		}

		if (collisionLayerElement->Attribute("name") != std::string("Collision Layer"))
		{
			continue;
		}

		for (const auto* collisionElement = collisionLayerElement->FirstChildElement(); 
			collisionElement != nullptr; collisionElement = collisionElement->NextSiblingElement())
		{
			int xPosition = 0, yPosition = 0;
			collisionElement->Attribute("x", &xPosition);
			collisionElement->Attribute("y", &yPosition);
			collidablePositions.emplace_back(xPosition, yPosition);
		}
	}

	assert(!collidablePositions.empty());
	return CollisionLayer(std::move(collidablePositions));
}