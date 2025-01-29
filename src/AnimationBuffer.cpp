#include "Animation.h"

namespace graphics {

	AnimationBuffer& AnimationBuffer::getInstance() {
		static AnimationBuffer instance;
		return instance;
	}
	const sf::Texture& AnimationBuffer::getFrame(const std::string& animationKey, int index) const {
		return frameCache_.at(animationKey).at(index);
	}

	size_t AnimationBuffer::getFrameCount(const std::string& animationKey) const {
		return frameCache_.at(animationKey).size();
	}

	void AnimationBuffer::addFrame(const std::string& animationKey, const sf::Texture& frame) {
		frameCache_[animationKey].push_back(frame);
	}
	void AnimationBuffer::addFrame(const std::string& animationKey, const std::string& basePath) {
		sf::Texture texture;
		if (texture.loadFromFile(basePath + ".png")) {
			frameCache_[animationKey].push_back(texture);
		}
		else {
			std::cout << "Failed to load frame: " + basePath << std::endl; // Edit this for your logger.
		}
	}
	void AnimationBuffer::loadFrames(const std::string& animationKey, const std::string& basePath, unsigned int frameCount, unsigned int startFrame) {
		if (frameCache_.find(animationKey) != frameCache_.end()) {
			return;
		}

		std::vector<sf::Texture> frames;
		for (unsigned int index = 0, j = startFrame; index < frameCount; ++index, ++j) {
			sf::Texture texture;
			if (texture.loadFromFile(basePath + std::to_string(j) + ".png")) {
				frames.push_back(std::move(texture));
			}
			else {
				std::cout << "Failed to load frame: " << std::to_string(index) << std::endl; // Edit this for your logger.
			}
		}
		frameCache_[animationKey] = std::move(frames);
	}

}