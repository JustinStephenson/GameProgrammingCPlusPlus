#pragma once

#include <vector>

class Actor {
public: 
	// Used to track stat of actor
	enum State {
		EActive,
		EPaused,
		EDead
	};

	// Vector class
	struct Vector2
	{
		float x;
		float y;
	};

	// Constructor / Destructor
	Actor(class Game* game);
	virtual ~Actor();
	
	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// Updates all the components attached to the actor (not overridable)
	void UpdateComponents(float deltaTime);
	// And actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);

	// Getters/setters
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// Actor's state
	State mState;
	// Transform
	Vector2 mPosition;	// Center position of actor
	float mScale;		// Uniforms scale of actor (1.0f for 100%)
	float mRotation;	// Rotation angle (in radians)
	// Components held by this actor
	std::vector<class Component*> mComponents;
	class Game* mGame;
};