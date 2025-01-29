#include "Animation.h"

namespace graphics {

	AnimationController& AnimationController::getInstance() {
		static AnimationController instance;
		return instance;
	}
	void AnimationController::saveState(const std::string& animationKey, int frameIndex, float elapsedTime, bool reverse) {
		states_[animationKey] = { frameIndex, elapsedTime, reverse };
	}
	AnimationController::AnimationState AnimationController::loadState(const std::string& animationKey) const {
		auto it = states_.find(animationKey);
		if (it != states_.end()) {
			return it->second;
		}
		return { 0, 0.0f, false };
	}

}