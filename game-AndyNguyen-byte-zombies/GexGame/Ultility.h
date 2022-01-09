#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "Animation.h"
namespace sf {
	class Sprite;
	class Text;
}

//random
int randomInt(int exclusiveMax);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(Animation& animation);

float length(sf::Vector2f v);

sf::Vector2f normalizeVector(sf::Vector2f v);

//trig helper
const float PI = 3.14159265f;

inline float radToDeg(float rad) {
	return rad * 180.f / PI;
}

inline float degToRad(float deg) {
	return deg * PI / 180.f;
}

// invert, or flip an IntRect for a frame
sf::IntRect flip(const sf::IntRect& rec);

inline float bering(sf::Vector2f v) {
	return 180 - radToDeg(std::atan2(v.x, v.y));
}

inline float bering(float x, float y) {
	return 180 - radToDeg(std::atan2(x,y));
}

namespace gex {
	inline float	sin(float deg) { return std::sin(degToRad(deg)); }
	inline float	cos(float deg) { return std::cos(degToRad(deg)); }
	inline float	atan2(float opp, float adj) { return radToDeg(std::atan2(opp, adj)); }
}