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
		sptr<Animation> animation = nullptr;
		bool looping = false;
		String transition = "";
	};
	struct Animator
	{
		void SetAnimation(String animation);
		void AddAnimation(String stateName, sptr<Animation> animation, String transition = "");
		void AddAnimation(String stateName, String animation, String transition = "");
		std::map<String, AnimationState> animations;
		AnimationState* currentState = nullptr;
		String currentStateName;
		float accumulator = 0;
		int currentKeyFrame = 0;
		float nextFrame = 0;
		float speed = 1;
		bool loop = true;
	};
	class AnimationSystem : public System
	{
	public:
		void OnUpdate(Time delta) override;
		int GetUsedMethod() override;

		Keyframe EvaluateTransform(Animator& animator);

	};
}