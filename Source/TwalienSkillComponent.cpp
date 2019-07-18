// Fill out your copyright notice in the Description page of Project Settings.

#include "TwalienSkillComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "BasicWarriorController.h"
#include "Twalien.h"

// Sets default values for this component's properties
UTwalienSkillComponent::UTwalienSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	TauntArea = CreateDefaultSubobject<USphereComponent>("TauntArea");
	TauntArea->SetupAttachment(this);
	TauntArea->SetSphereRadius(TauntRadio);

	// ...
}


// Called when the game starts
void UTwalienSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTwalienSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTwalienSkillComponent::TauntSkill() {

	for (AActor * ValidActor : ActorsInArea)
	{
		if (IsEnemy(ValidActor) )
		{
			ABasicWarriorController* CurrentController = Cast<ABasicWarriorController>((Cast<APawn>(ValidActor)->GetController()));
			if (CurrentController) {
				CurrentController->SetAgentTarget(Cast<ATwalien>(GetOwner()));
			}
		}
	}
}

void UTwalienSkillComponent::OnObjectEnterToArea(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//If actor is a enemy and it isn't in the area.
	if (HasActorAnyValidTag(OtherActor) && !ActorsInArea.Contains(OtherActor))
	{
		//Add enemy
		ActorsInArea.Add(OtherActor);
	}
}

bool UTwalienSkillComponent::HasActorAnyValidTag(AActor * Actor)
{
	bool found = false;

	for (FName Tag : Actor->Tags)
	{
		found = AllowedTagsToInteract.Contains(Tag);
		if (found)
		{
			break;
		}
	}

	return found;
}

void UTwalienSkillComponent::OnObjectExitToArea(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//If actor is a coin and it is in Suckarea.
	if (HasActorAnyValidTag(OtherActor) && ActorsInArea.Contains(OtherActor))
	{
		//Remove coin from TArray
		ActorsInArea.Remove(OtherActor);
	}
}
bool UTwalienSkillComponent::IsEnemy(AActor * Actor) {
	return Actor->Tags.Contains("Enemy");
}