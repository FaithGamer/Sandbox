#include "pch.h"
#include "Sandbox/Render/AnimationSystem.h"
#include "Sandbox/Render/SpriteRender.h"
#include "Sandbox/Core/Assets.h"


namespace Sandbox
{
	//Keyframe
	Keyframe::Keyframe() : timeToNext(0)
	{
	}

	Animation::Animation() : frequency(0.1666)
	{
	}

	Animation::Animation(Serialized& config)
	{
		Deserialize(config);
	}

	float Animation::GetTime()
	{
		return frequency * frames.size();
	}

	//Animation
	Serialized Animation::Serialize()
	{
		LOG_WARN("Cannot serialize animation.");
		return Serialized();
	}

	void Animation::Deserialize(Serialized& config)
	{
		frequency = 1.f / config["frame_per_second"].get<float>();

		int currentFrame = 0;
		sptr<Sprite> lastSprite;
		for (auto& frame : config["frames"])
		{
			sptr<Sprite> sprite;

			//Set sprite if any
			auto sprite_it = frame.find("sprite");
			if (sprite_it != frame.end())
			{
				sprite = Assets::Get<Sprite>(sprite_it->get<String>()).Ptr();
			}
		
			float timeToNext = 1.f * frequency;

			//Set custom time if any
			auto time_it = frame.find("time");
			if (time_it != frame.end())
			{
				timeToNext = time_it->get<float>() * frequency;
			}

			Keyframe keyframe;
			keyframe.sprite = sprite;
			keyframe.timeToNext = timeToNext;
			frames.push_back(keyframe);
		}
	}

	//Animator
	Animator::Animator() : accumulator(0), currentState(nullptr), nextFrame(0), currentKeyFrame(0), speed(1)
	{

	}

	void Animator::SetAnimation(String animation)
	{
		auto find_it = animations.find(animation);
		if (find_it != animations.end())
		{
			currentStateName = animation;
			currentState = &find_it->second;
			accumulator = 0;
			currentKeyFrame = 0;
		}
		else
		{
			LOG_WARN("Cannot find animation with name: {0}", animation);
		}
	}

	void Animator::AddAnimation(String stateName, String animation)
	{
		AnimationState state;
		state.animation = Assets::Get<Animation>(animation).Ptr();
		state.looping = true;
		state.transition = stateName;
		animations.insert(std::make_pair(stateName, state.animation));
	}

	//Animation System
	void AnimationSystem::OnUpdate(Time delta)
	{
		ForeachComponents<Animator, SpriteRender, Transform>([&](Animator& animator, SpriteRender& sprite, Transform& transform)
			{

				if (animator.currentState == nullptr)
					return;
			
				auto& anim = animator.currentState->animation;
				auto& frame = animator.currentKeyFrame;
				float frameTime = anim->frames[frame].timeToNext;

				if (animator.accumulator > frameTime)
				{
					//Go to next frame, or go back to frame 0
					frame = frame >= anim->frames.size() - 1 ? 0 : frame + 1;
					//Reset frame timer
					animator.accumulator -= frameTime;
				}

				sprite.SetSprite(anim->frames[frame].sprite);
				animator.accumulator += (float)delta * animator.speed;
				//to do, evaluate transform
			});
	}

	int AnimationSystem::GetUsedMethod()
	{
		return System::Method::Update;
	}

	Keyframe AnimationSystem::EvaluateTransform(Animator& animator)
	{
		return Keyframe();
	}
}