#include "PillarGroup.h"

const float TITLE_HEIGHT = 1024 / 16;
const float TITLE_WIDTH = 578 / 15;
const float TITLE_WIDTH_SPEED = 1156 / 15;

PillarGroup::PillarGroup(const TextureHolder_t& textures)
	:Entity(1)
{
	int x = randomInt(4);

 	auto pillarTop = std::make_unique<Pillar>(Pillar::Type::PillarDown, textures);
	pillarTop->setPosition(TITLE_WIDTH * 7, TITLE_HEIGHT * (-5+x));
	pillarTop->setVelocity(-TITLE_WIDTH_SPEED * 3, 0.f);
	attachChild(std::move(pillarTop));

	auto pillarInvis = std::make_unique<Pillar>(Pillar::Type::PillarInvisible, textures);
	pillarInvis->setPosition(TITLE_WIDTH * 7, TITLE_HEIGHT * (5.5+x));
	pillarInvis->setVelocity(-TITLE_WIDTH_SPEED * 3, 0.f);
	attachChild(std::move(pillarInvis));

	auto pillarBottom = std::make_unique<Pillar>(Pillar::Type::PillarUp, textures);
	pillarBottom->setPosition(TITLE_WIDTH * 7, TITLE_HEIGHT * (10 + x));
	pillarBottom->setVelocity(-TITLE_WIDTH_SPEED * 3, 0.f);
	attachChild(std::move(pillarBottom));
}
