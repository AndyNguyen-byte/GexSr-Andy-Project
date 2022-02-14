#pragma once

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

#include <SFML/Graphics.hpp>

enum class TextureID { Landscape, Airplain, Missile };

class TextureHolder
{
public:
	TextureHolder(const TextureHolder&) = delete;

	void load(TextureID id, const std::string& filename) {
		std::unique_ptr<sf::Texture> texture(new sf::Texture());
		
		if (!texture->loadFromFile(filename)) {
			throw std::runtime_error("load failed file " + filename);
		}

		auto inserted = textureMap.insert(std::make_pair(id, std::move(texture)));
		assert(inserted.second);
	}

	sf::Texture& get(TextureID id) {
		auto found = textureMap.find(id);
		return *found->second;
	}

	const sf::Texture& get(TextureID id) const {
		auto found = textureMap.find(id);
		return *found->second;
	}
private:
	std::map < TextureID, std::unique_ptr<sf::Texture>> textureMap;
};

