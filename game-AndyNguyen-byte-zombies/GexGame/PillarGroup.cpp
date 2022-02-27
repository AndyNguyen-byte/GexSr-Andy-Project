#include "PillarGroup.h"

const float TITLE_HEIGHT = 1024 / 16;
const float TITLE_WIDTH = 578 / 15;
const float TITLE_WIDTH_SPEED = 1156 / 15;

PillarGroup::PillarGroup(const TextureHolder_t& textures,int speedInput)
	:Entity(1)
	, speed(speedInput)                                                             
{
	int y = randomInt(4);

 	auto pillarTop = std::make_unique<Pillar>(Pillar::Type::PillarDown, textures);
	pillarTop->setPosition(TITLE_WIDTH * 7, TITLE_HEIGHT * (-5+y));
	pillarTop->setVelocity(TITLE_WIDTH_SPEED * speed, 0.f);
	attachChild(std::move(pillarTop));

	auto pillarInvis = std::make_unique<Pillar>(Pillar::Type::PillarInvisible, textures);
	pillarInvis->setPosition(TITLE_WIDTH * 7, TITLE_HEIGHT * (5.5+y));
	pillarInvis->setVelocity(TITLE_WIDTH_SPEED * speed, 0.f);
	attachChild(std::move(pillarInvis));

	auto pillarBottom = std::make_unique<Pillar>(Pillar::Type::PillarUp, textures);
	pillarBottom->setPosition(TITLE_WIDTH * 7, TITLE_HEIGHT * (10 + y));
	pillarBottom->setVelocity(TITLE_WIDTH_SPEED * speed, 0.f);
	attachChild(std::move(pillarBottom));
}
