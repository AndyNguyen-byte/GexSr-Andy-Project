#pragma once

#include "PostEffect.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>
#include <map>
#include <array>

class BloomEffect : public PostEffect
{
public:

	enum class Shaders
	{
		BrightnessPass,
		DownSamplePass,
		GaussianBlurPass,
		AddPass,
	};


public:
				BloomEffect();

	void		apply(const sf::RenderTexture& input, sf::RenderTarget& output) override;


private:
	typedef std::array<sf::RenderTexture, 2> RenderTextureArray;


private:
	void				prepareTextures(sf::Vector2u size);

	void				filterBright(const sf::RenderTexture& input, sf::RenderTexture& output);
	void				blurMultipass(RenderTextureArray& renderTextures);
	void				blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor);
	void				downsample(const sf::RenderTexture& input, sf::RenderTexture& output);
	void				add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output);


private:
	std::map<Shaders, std::unique_ptr<sf::Shader>>	shaders;

	sf::RenderTexture								brightnessTexture;
	RenderTextureArray								firstPassTexture;
	RenderTextureArray								secondPassTexture;
};
