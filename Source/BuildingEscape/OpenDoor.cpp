// Copyright Conal Smith 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Find the owning component
	Owner = GetOwner();
	// Find the player controller
	// Player controller will exist after play has begun
	// Pawn is an actor therefore use getPawn method.
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the trigger volume every frame
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))// if the ActorThatOpens is in the volume
	{
		OpenDoor();// open the door
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it is time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}

}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}