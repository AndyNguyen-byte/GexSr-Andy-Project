#include "BloomEffect.h"

#include <string>
#include <cassert>
#include <stdexcept>

BloomEffect::BloomEffect()
	: shaders()
	, brightnessTexture()
	, firstPassTexture()
	, secondPassTexture()
{


	std::unique_ptr<sf::Shader> s(new sf::Shader());
	std::string f1name = "../Media/Shaders/Fullpass.vert";
	std::string f2name = "../Media/Shaders/Brightness.frag";
	if (!(s->loadFromFile("../Media/Shaders/Fullpass.vert", "../Media/Shaders/Brightness.frag")))
		throw std::runtime_error("Shader::load - Failed to load ");

	auto inserted = shaders.insert(std::make_pair(Shaders::BrightnessPass, std::move(s)));
	assert(inserted.second);

	s.reset(new sf::Shader());
	f1name = "../Media/Shaders/Fullpass.vert";
	f2name = "../Media/Shaders/DownSample.frag";
	if (!s->loadFromFile(f1name, f2name))
		throw std::runtime_error("Shader::load - Failed to load " + f1name);
	inserted = shaders.insert(std::make_pair(Shaders::DownSamplePass, std::move(s)));
	assert(inserted.second);

	s.reset(new sf::Shader());
	f1name = "../Media/Shaders/Fullpass.vert";
	f2name = "../Media/Shaders/GuassianBlur.frag";
	if (!s->loadFromFile(f1name, f2name))
		throw std::runtime_error("Shader::load - Failed to load " + f1name);
	inserted = shaders.insert(std::make_pair(Shaders::GaussianBlurPass, std::move(s)));
	assert(inserted.second);

	s.reset(new sf::Shader());
	f1name = "../Media/Shaders/Fullpass.vert";
	f2name = "../Media/Shaders/Add.frag";
	if (!s->loadFromFile(f1name, f2name))
		throw std::runtime_error("Shader::load - Failed to load " + f1name);
	inserted = shaders.insert(std::make_pair(Shaders::AddPass, std::move(s)));
	assert(inserted.second);

}

void BloomEffect::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
	prepareTextures(input.getSize());

	filterBright(input, brightnessTexture);

	downsample(brightnessTexture, firstPassTexture[0]);
	blurMultipass(firstPassTexture);

	downsample(firstPassTexture[0], secondPassTexture[0]);
	blurMultipass(secondPassTexture);

	add(firstPassTexture[0], secondPassTexture[0], firstPassTexture[1]);
	firstPassTexture[1].display();

	add(input, firstPassTexture[1], output);
}

void BloomEffect::prepareTextures(sf::Vector2u size)
{
	if (brightnessTexture.getSize() != size)
	{
		brightnessTexture.create(size.x, size.y);
		brightnessTexture.setSmooth(true);

		firstPassTexture[0].create(size.x / 2, size.y / 2);
		firstPassTexture[0].setSmooth(true);
		firstPassTexture[1].create(size.x / 2, size.y / 2);
		firstPassTexture[1].setSmooth(true);

		secondPassTexture[0].create(size.x / 4, size.y / 4);
		secondPassTexture[0].setSmooth(true);
		secondPassTexture[1].create(size.x / 4, size.y / 4);
		secondPassTexture[1].setSmooth(true);
	}
}

void BloomEffect::filterBright(const sf::RenderTexture& input, sf::RenderTexture& output)
{
	sf::Shader& brightness = *shaders.at(Shaders::BrightnessPass);

	brightness.setUniform("source", input.getTexture());
	applyShader(brightness, output);
	output.display();
}

void BloomEffect::blurMultipass(RenderTextureArray& renderTextures)
{
	sf::Vector2u textureSize = renderTextures[0].getSize();

	for (std::size_t count = 0; count < 2; ++count)
	{
		blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
		blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
	}
}

void BloomEffect::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
{
	sf::Shader& gaussianBlur = *shaders.at(Shaders::GaussianBlurPass);

	gaussianBlur.setUniform("source", input.getTexture());
	gaussianBlur.setUniform("offsetFactor", offsetFactor);
	applyShader(gaussianBlur, output);
	output.display();
}

void BloomEffect::downsample(const sf::RenderTexture& input, sf::RenderTexture& output)
{
	sf::Shader& downSampler = *shaders.at(Shaders::DownSamplePass);

	downSampler.setUniform("source", input.getTexture());
	downSampler.setUniform("sourceSize", sf::Vector2f(input.getSize()));
	applyShader(downSampler, output);
	output.display();
}

void BloomEffect::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output)
{
	sf::Shader& adder = *shaders.at(Shaders::AddPass);

	adder.setUniform("source", source.getTexture());
	adder.setUniform("bloom", bloom.getTexture());
	applyShader(adder, output);
}
