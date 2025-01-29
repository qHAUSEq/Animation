#include "Animation.h"

namespace graphics {

	void Animation::updateCurrentFrame() {
		sprite_.setTexture(AnimationBuffer::getInstance().getFrame(animationKey_, currentFrameIndex_));
	}
	void Animation::updateReverseAnimation() {
		if (animationType_ == Reverse || animationType_ == RememberReverse) {
			if (reverseDirection_) {
				if (currentFrameIndex_ == 0) {
					reverseDirection_ = false;
					currentFrameIndex_++;
				}
				else {
					currentFrameIndex_--;
				}
			}
			else {
				if (currentFrameIndex_ == frameCount_ - 1) {
					reverseDirection_ = true;
					currentFrameIndex_--;
				}
				else {
					currentFrameIndex_++;
				}
			}
		}
	}

	// Constructor for frame-by-frame animation
	Animation::Animation(const std::string& basePath, unsigned int frameCount, float frameTime, const std::string& animationKey, AnimationType type, unsigned int startFrame)
		: frameCount_(frameCount), frameTime_(frameTime), animationType_(type), animationKey_(animationKey), startFrame_(startFrame),
		reverseDirection_(false), currentFrameIndex_(0), elapsedTime_(0), deltaTime_(0)
	{
		this->frameCount_ -= startFrame;
		AnimationBuffer::getInstance().loadFrames(animationKey_, basePath, frameCount_, startFrame_);
		// If the type is Remember || RememberReverse, load the saved state from the AnimationController
		if (animationType_ == Remember || animationType_ == RememberReverse) {
			this->currentFrameIndex_ = AnimationController::getInstance().loadState(animationKey_).frameIndex;
			this->elapsedTime_ = AnimationController::getInstance().loadState(animationKey_).elapsedTime;
			this->reverseDirection_ = AnimationController::getInstance().loadState(animationKey_).reverse;
		}
		this->updateCurrentFrame();  // Initializing the initial frame
	}
	// Constructor for no frame animation with move
	Animation::Animation(const std::string& basePath, const std::string& animationKey, sf::Vector2f velocity, float frameTime) :
		animationKey_(animationKey), velocity_(velocity), frameTime_(frameTime), 
		currentFrameIndex_(0), elapsedTime_(0), deltaTime_(0), animationType_(NoFrameMove)
	{
		AnimationBuffer::getInstance().addFrame(animationKey_, basePath); 
		this->updateCurrentFrame();
	}
	// Constructor for frame-by-frame animation with move
	Animation::Animation(const std::string& basePath, const std::string& animationKey, sf::Vector2f velocity, unsigned int frameCount, float frameTime, unsigned int startFrame) :
		animationKey_(animationKey), velocity_(velocity), frameCount_(frameCount), frameTime_(frameTime), startFrame_(startFrame), 
		currentFrameIndex_(0), elapsedTime_(0), deltaTime_(0), reverseDirection_(false), animationType_(FrameByFrameMove)
	{
		this->frameCount_ -= startFrame;
		AnimationBuffer::getInstance().loadFrames(animationKey_, basePath, frameCount_, startFrame_);
		this->updateCurrentFrame();  // Initializing the initial frame
	}

	void Animation::setFrameCount(unsigned int frameCount) {
		this->frameCount_ = frameCount;
	}
	void Animation::setFrameTime(float frameTime) {
		this->frameTime_ = frameTime;
	}
	void Animation::setCurrentFrameIndex(unsigned int currentFrameIndex) {
		this->currentFrameIndex_ = currentFrameIndex;
	}
	void Animation::setElapsedTime(float elapsedTime) {
		this->elapsedTime_ = elapsedTime;
	}
	void Animation::setDeltaTime(float deltaTime) {
		this->deltaTime_ = deltaTime;
	}
	void Animation::setStartFrame(unsigned int startFrame) {
		this->startFrame_ = startFrame;
		this->frameCount_ -= startFrame_;
	}
	void Animation::setVelocity(const sf::Vector2f& velocity) {
		this->velocity_ = velocity;
	}
	void Animation::setStartPosition(const sf::Vector2f& pos) {
		this->sprite_.setPosition(pos);
		this->startPosX = pos.x;
		this->startPosY = pos.y;
	}

	unsigned int Animation::getFrameCount() const {
		return this->frameCount_;
	}
	float Animation::getFrameTime() const {
		return this->frameTime_;
	}
	unsigned int Animation::getCurrentFrameIndex() const {
		return this->currentFrameIndex_;
	}
	float Animation::getElapsedTime() const {
		return this->elapsedTime_;
	}
	float Animation::getDeltaTime() const {
		return this->deltaTime_;
	}
	unsigned int Animation::getStartFrame() const {
		return this->startFrame_;
	}
	sf::Vector2f Animation::getVelocity() const {
		return this->velocity_;
	}
	sf::Vector2f Animation::getStartPosition() const {
		return sf::Vector2f();
	}
	const sf::Sprite& Animation::getSprite() const {
		return this->sprite_;
	}

	// Save the current state for future use through the controller
	void Animation::remember() const {
		if (animationType_ == Remember || animationType_ == RememberReverse) {
			AnimationController::getInstance().saveState(animationKey_, currentFrameIndex_, elapsedTime_, reverseDirection_);
		}
	}
	void Animation::load(const std::string& animationKey, const std::string& basePath) {
		// NOT USE, TODO
		AnimationBuffer::getInstance().loadFrames(animationKey, basePath, frameCount_, startFrame_);
		updateCurrentFrame();  // Initializing the initial frame
	}
	void Animation::scale() {
		// Calculating the scaling factors. TODO
		float ScaleX = sf::VideoMode::getDesktopMode().width / sprite_.getLocalBounds().width; // Edit this method for your resolution
		float ScaleY = sf::VideoMode::getDesktopMode().height / sprite_.getLocalBounds().height; // Edit this method for your resolution

		//sprite_.setScale(ScaleX, ScaleY);
	}
	void Animation::update() {
		deltaTime_ = clock_.restart().asSeconds();
		elapsedTime_ += deltaTime_;

		if (elapsedTime_ >= frameTime_) {
			elapsedTime_ = 0.0f;
			switch (animationType_) {
			case None:
				currentFrameIndex_ = (currentFrameIndex_ + 1) % frameCount_;
				break;
			case Reverse:
				updateReverseAnimation();
				break;
			case Remember:
				currentFrameIndex_ = (currentFrameIndex_ + 1) % frameCount_;
				break;
			case RememberReverse:
				updateReverseAnimation();
				break;
			case FrameByFrameMove:
				currentFrameIndex_ = (currentFrameIndex_ + 1) % frameCount_;
				sprite_.move(velocity_ * deltaTime_);
				break;
			case NoFrameMove:
				sprite_.move(velocity_ * deltaTime_);
				// Check whether the sprite has gone beyond the window border
				if (sprite_.getPosition().x > sf::VideoMode::getDesktopMode().width) { // Edit this method for your resolution
					sprite_.setPosition(this->startPosX, -this->startPosY);
				}
				break;
			}
			updateCurrentFrame();
		}
	}
	void Animation::restart() {
		currentFrameIndex_ = 0;
		elapsedTime_ = 0.0f;
		updateCurrentFrame();  // Switching to the first frame
	}
	void Animation::draw(sf::RenderWindow& window) {
		this->scale();
		window.draw(sprite_);
	}

}