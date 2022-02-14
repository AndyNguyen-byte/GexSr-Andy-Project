#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


class TextNode : public SceneNode
{
public:
	TextNode(const FontHolder_t& fonts, const std::string& str);

	void setText(const std::string& str);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	
private:
	sf::Text	sfText;

};

