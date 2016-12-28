// Copyright Conal Smith 2017

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}


void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for attached Physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component."), *GetOwner()->GetName());
	}
}


void UGrabber::SetupInputComponent()
{
	/// Look for the attached Input component (Only appears at runtime)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component."), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// Move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}


void UGrabber::Grab()
{
	/// LINE TRACE and reach any actors with Physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit) 
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None, // no skeleton bones to grab
			ComponentToGrab->GetOwner()->GetActorLocation() // grab at base
		);
	}
}


void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}



const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Set up query parameters
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Ray-cast out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}


FVector UGrabber::GetReachLineStart()
{
	// Get the player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()
		->GetFirstPlayerController()
		->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	// The vector of the point at the end of the reach
	return PlayerViewPointLocation;
}


FVector UGrabber::GetReachLineEnd()
{
	// Get the player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()
		->GetFirstPlayerController()
		->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	// The vector of the point at the end of the reach
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
