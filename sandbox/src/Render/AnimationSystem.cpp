#include "pch.h"
#include "Sandbox/Render/AnimationSystem.h"
#include "Sandbox/Render/SpriteRender.h"
#include "Sandbox/Core/Assets.h"

namespace Sandbox
{
	//---Keyframe---

	Keyframe::Keyframe() : timeToNext(0)
	{
	}

	Animation::Animation() : frequency(0.1666f)
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

	//---Animation---

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
		bool signal = false;
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
			keyframe.sendSignal = frame.find("signal") != frame.end() ? frame.at("signal").get<bool>() : false;
			if (keyframe.sendSignal)
				signal = true;
			frames.push_back(keyframe);
		}
		if (signal)
		{
			signalsTemplate.resize(frames.size());
		}
	}


	//---Animator---

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

	void Animator::AddAnimation(String stateName, sptr<Animation> animation, String transition)
	{
		AnimationState state;
		state.animation = animation;
		state.looping = true;
		state.transition = transition;
		state.signals = animation->signalsTemplate;
		animations.insert(std::make_pair(stateName, state));
	}

	void Animator::AddAnimation(String stateName, String animation, String transition)
	{
		AddAnimation(stateName, Assets::Get<Animation>(animation).Ptr(), transition);
	}

	//Animation System
	void AnimationSystem::OnUpdate(Time delta)
	{
		ForeachComponents<Animator, SpriteRender, Transform>([&](Animator& animator, SpriteRender& sprite, Transform& transform)
			{
				if (animator.currentState == nullptr)
					return;

				auto anim = animator.currentState->animation;
				auto* frame = &animator.currentKeyFrame;
				float frameTime = anim->frames[*frame].timeToNext;

				while (animator.accumulator > frameTime)
				{
					//Go to next frame, or go back to frame 0, or stay and last frame
					int frameMax = anim->frames.size() - 1;
					(*frame)++;
					
					if (*frame > frameMax)
					{
						if (animator.currentState->transition.size() > 0)
						{
							animator.SetAnimation(animator.currentState->transition);
							anim = animator.currentState->animation;
							frame = &animator.currentKeyFrame;
						}
						else
						{
							*frame = animator.loop ? 0 : frameMax;
						}
					}
					if (anim->frames[*frame].sendSignal)
					{
						animator.currentState->signals[*frame].SendSignal(KeyframeSignal(animator.currentStateName, *frame));
					}
					//Reset frame timer
					animator.accumulator = std::max(animator.accumulator - frameTime, 0.f);
				}
				sprite.SetSprite(anim->frames[*frame].sprite);
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