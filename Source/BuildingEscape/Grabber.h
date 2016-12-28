// Copyright Conal Smith 2017

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


private:
	// How far ahead the player can reach in cm.
	float Reach = 100.f;
		
	// Set to null as these components may not be initialised and we validate they aren't nullptr
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray-case and grab what is in reach.
	void Grab();

	// Release what has already been grabbed.
	void Release();

	// Find attached physics
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached input component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Get the vector of the end of player's reach trace line
	FVector GetReachLineStart();

	// Get the vector of the end of player's reach trace line
	FVector GetReachLineEnd();
};
