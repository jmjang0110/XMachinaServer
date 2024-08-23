#pragma once
#include <cstdint> 
enum class ScriptKey : uint32_t {
	None,
	Enemy,
	EnemyController,
	Bullet,
	Player,
	Skill,
	Phero,
	PheroDropper,
	BehaviorTree,
	Building,
	Item,
	_count,
};

uint32_t uint32_t_ScriptKey(ScriptKey key);
