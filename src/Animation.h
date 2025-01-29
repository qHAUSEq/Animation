#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Logger/Logger.h"

namespace graphics {

	class AnimationBuffer {
	private:
		AnimationBuffer() = default;
		AnimationBuffer(const AnimationBuffer&) = delete;
		AnimationBuffer& operator=(const AnimationBuffer&) = delete;

	public:
		static AnimationBuffer& getInstance();
		const sf::Texture& getFrame(const std::string& animationKey, int index) const; // Getting a frame by index
		size_t getFrameCount(const std::string& animationKey) const;

		void addFrame(const std::string& animationKey, const sf::Texture& frame); // Method for adding a frame to the buffer
		void addFrame(const std::string& animationKey, const std::string& basePath);
		void loadFrames(const std::string& animationKey, const std::string& basePath, unsigned int frameCount, unsigned int startFrame = 0); // Frame loading method

	private:
		std::unordered_map<std::string, std::vector<sf::Texture>> frameCache_;
	};

	class AnimationController {
	public:
		struct AnimationState {
			int frameIndex;
			float elapsedTime;
			bool reverse;
		};

	private:
		AnimationController() = default;
		AnimationController(const AnimationController&) = delete;
		AnimationController& operator=(const AnimationController&) = delete;

	public:
		static AnimationController& getInstance(); // Getting a singleton instance of the controller

		void saveState(const std::string& animationKey, int frameIndex, float elapsedTime, bool reverse); // Saving the current state for a specific animation
		AnimationState loadState(const std::string& animationKey) const; // Getting the state for a specific animation, if it exists

	private:
		std::unordered_map<std::string, AnimationState> states_; // Storing states for each animation by a unique key
	};


	class Animation {
	public: 
		enum AnimationType { // Animation type
			None,              // Looped animation
			Reverse,           // Animation to the end, then in reverse order
			Remember,          // Remembers the current frame, using with method Animation::remember()
			RememberReverse,   // Combined Type remember & reverse 
			FrameByFrameMove,  // Frame-by-frame animation + move
			NoFrameMove,	   // No frame animation, only move
		};

	private:
		void updateCurrentFrame();
		void updateReverseAnimation(); // Logic for animation in reverse mode

	public:
		/* Using Animation type:
		1. None (standart) - Looped animation;
		2. Reverse - Animation to the end, then in reverse order;
		3. Remember - Remembers the current frame, using with method Animation::remember(). Inside using animation controller;
		4. RememberReverse - Combined Type remember & reverse.
		*/
		Animation(const std::string& basePath, unsigned int frameCount, float frameTime, const std::string& animationKey, AnimationType type = None, unsigned int startFrame = 0);
		// Animation type: NoFrameMove - No frame animation, only move
		Animation(const std::string& basePath, const std::string& animationKey, sf::Vector2f velocity, float frameTime);
		// Animation type: FrameMove - Frame-by-frame animation + move
		Animation(const std::string& basePath, const std::string& animationKey, sf::Vector2f velocity, unsigned int frameCount, float frameTime, unsigned int startFrame = 0);
		~Animation() = default;

		void setFrameCount(unsigned int frameCount);
		void setFrameTime(float frameTime);
		void setCurrentFrameIndex(unsigned int currentFrameIndex);
		void setElapsedTime(float elapsedTime);
		void setDeltaTime(float deltaTime);
		void setStartFrame(unsigned int startFrame);
		void setVelocity(const sf::Vector2f& velocity);
		void setStartPosition(const sf::Vector2f& pos);

		unsigned int getFrameCount() const;
		float getFrameTime() const;
		unsigned int getCurrentFrameIndex() const;
		float getElapsedTime() const;
		float getDeltaTime() const;
		unsigned int getStartFrame() const;
		sf::Vector2f getVelocity() const;
		sf::Vector2f getStartPosition() const;
		const sf::Sprite& getSprite() const;

		void remember() const;
		void load(const std::string& animationKey, const std::string& basePath);
		void scale();
		void update();
		void restart();
		void draw(sf::RenderWindow& window);

	private:
		unsigned int frameCount_;        // The total number of frames in the animation
		float frameTime_;                // The display time of each frame
		unsigned int currentFrameIndex_;
		float elapsedTime_;              // Elapsed time since the last update
		float deltaTime_;                // Time between frames
		unsigned int startFrame_;
		AnimationType animationType_;
		bool reverseDirection_;          // Flag for tracking the direction (only for Reverse mode & RememberReverse mode)
		std::string animationKey_;       // The animation key is used in buffer & Controller
		sf::Clock clock_;                // Time Tracking Clock

		sf::Vector2f velocity_;			 // The speed of movement of the sprite
		float startPosX;				 // Start position x for move
		float startPosY;				 // Start position y for move

		sf::Sprite sprite_;              // Sprite to display the current frame
	};

}