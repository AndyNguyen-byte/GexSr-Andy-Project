#pragma once


namespace Category
{
	enum type
	{
		None				= 0,
		SceneAirLayer		= 1 << 0,
		PlayerAircraft		= 1 << 1,
		AlliedAircraft		= 1 << 2,
		EnemyAircraft		= 1 << 3,
		Pickup				= 1 << 4,
		AlliedProjectile	= 1 << 5,
		EnemyProjectile		= 1 << 6,
		ParticleSystem		= 1 << 8,
		SoundEffect			= 1 << 9,
		Zombie				= 1 << 10,
		Hero				= 1 << 11,


		PlayerFrog			= 1 << 12,
		BonusFrog			= 1 << 13,
		LilyPad				= 1 << 14,
		Vehicle				= 1 << 15,
		RiverEntities		= 1 << 16,

		Ground				= 1 << 17,
		Turtle				= 1 << 18,
		Pillar				= 1 << 19,
		ScorePillar			= 1 << 20,



		Aircraft = PlayerAircraft | AlliedAircraft | EnemyAircraft,
		Projectile = AlliedProjectile | EnemyProjectile,
		SpaceJunk = Projectile | Pickup | EnemyAircraft | Vehicle | RiverEntities | Pillar | ScorePillar,
	
	};
}

