#pragma once
#include "Sandbox/ECS/System.h"
#include "Sandbox/Core/Serialization.h"
#include "Sandbox/Render/Sprite.h"

namespace Sandbox
{
	struct Keyframe
	{
		Keyframe();
		sptr<Sprite> sprite;
		float timeToNext;
	};
	struct Animation : public Serializable
	{
		Animation();
		Animation(Serialized& config);
		float GetTime();
		Serialized Serialize() override;
		void Deserialize(Serialized& config);

		std::vector<Keyframe> frames;
		float frequency; // 1/fps
	};
	struct AnimationState
	{
		sptr<Animation> animation;
		bool looping;
		String transition;
	};
	struct Animator
	{
		Animator();
		void SetAnimation(String animation);
		void AddAnimation(String stateName, String animation);
		std::map<String, AnimationState> animations;
		AnimationState* currentState;
		String currentStateName;
		float accumulator;
		int currentKeyFrame;
		float nextFrame;
	};
	class AnimationSystem : public System
	{
	public:
		void OnUpdate(Time delta) override;
		int GetUsedMethod() override;

		Keyframe EvaluateTransform(Animator& animator);

	};
}