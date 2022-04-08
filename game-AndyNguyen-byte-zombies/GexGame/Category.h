#pragma once


namespace Category
{
	enum type
	{
		None				= 0,
		SceneAirLayer		= 1 << 0,
		ParticleSystem		= 1 << 1,
		SoundEffect			= 1 << 2,

		Ground				= 1 << 3,
		Turtle				= 1 << 4,
		Pillar				= 1 << 5,
		ScorePillar			= 1 << 6,
		Shark				= 1 << 7,

		SpaceJunk = Pillar | ScorePillar | Shark,
	
	};
}

