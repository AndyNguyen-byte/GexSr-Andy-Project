#include "TextNode.h"
#include "Utility.h"

TextNode::TextNode(const FontHolder_t& fonts, const std::string& str)
{
	sfText.setFont(fonts.get(FontID::Main));
	sfText.setCharacterSize(20);
	setText(str);
}

void TextNode::setText(const std::string& str)
{
	sfText.setString(str);
	centerOrigin(sfText);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sfText, states);
}
