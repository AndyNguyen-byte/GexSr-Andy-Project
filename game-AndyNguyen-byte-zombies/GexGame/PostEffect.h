#pragma once

namespace sf
{
	class RenderTarget;
	class RenderTexture;
	class Shader;
}



// Abstract class
// override apply() in derived class with shader effect to be applied

class PostEffect
{
public:
							PostEffect();
							PostEffect(const PostEffect&) = delete; // non-copyable
	virtual					~PostEffect() = default;


	virtual void			apply(const sf::RenderTexture& input, sf::RenderTarget& output) = 0;

	static bool				isSupported();  // does the graphics card support post effects


protected:
	static void				applyShader(const sf::Shader& shader, sf::RenderTarget& output);
};
