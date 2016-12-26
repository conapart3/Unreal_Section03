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
	AActor* Owner = GetOwner();
	FRotator Rotator = Owner->GetActorRotation();
	UE_LOG(LogTemp, Warning, TEXT("Rotation is at %s"), *Rotator.ToString());

	FRotator NewRotation = FRotator(0.f, -60.f, 0.f);
	Owner->SetActorRotation(NewRotation);
	//UE_LOG(LogTemp, Warning, TEXT("NEW Rotation is at %s"), *Rotator.ToString());
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

