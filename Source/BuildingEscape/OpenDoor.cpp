// Copyright Conal Smith 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName())
	}
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() > 30.f)// todo make into a parameter editanywhere for testing
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
	if (!Owner) { return; }
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	if (!Owner) { return; }
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { return TotalMass; }
	// Find all the overlapping actors
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses. (use const as the iterator doesn't change we just receive it).
	for (const auto& Iterator : OverlappingActors)
	{
		TotalMass = TotalMass + Iterator->GetRootPrimitiveComponent()->GetMass();
	}

	return TotalMass;
}
