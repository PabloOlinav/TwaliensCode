// All rights reserved.

#include "LineOfSight.h"
#include "Twalien.h"
#include "AIController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

ULineOfSight::ULineOfSight()
{
	ForceInstancing(true);
}

void ULineOfSight::OnGameplayTaskActivated(UGameplayTask & Task)
{
}

void ULineOfSight::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
}

bool ULineOfSight::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	TArray<FHitResult> OutHit;
	FVector Start, End;
	FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_Pawn));;
	FCollisionQueryParams QueryParams;
	const UBlackboardComponent * Blackboard = OwnerComp.GetBlackboardComponent();
	const AActor * Player = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	const APawn * Agent = OwnerComp.GetAIOwner()->GetPawn();
	const UWorld * World = GetWorld();


	Start = Agent->GetActorLocation();
	Start.Z += Agent->BaseEyeHeight;

	End = Player->GetActorLocation();
	//fast fix, we need to create a interface for targetting TODO - create interface for targetting - a$tuR
	//End.Z += Player->BaseEyeHeight;

	QueryParams.AddIgnoredActor(Agent);

	if(World->LineTraceMultiByObjectType(OutHit, Start, End, ObjectQueryParams, QueryParams))
	{
		return IsThereLineOfSight(OutHit, Player);
	}
	else 
		return false;
}

bool ULineOfSight::IsThereLineOfSight(TArray<FHitResult> Hits, const AActor * Target) const
{
	for (const FHitResult Element : Hits)
	{
		if(!ActorTagsAreInIgnoreList(Element.Actor->Tags))
		{
			return Element.Actor == Target;
		}
	}

	return false;
}

bool ULineOfSight::ActorTagsAreInIgnoreList(TArray<FName> Tags) const
{
	bool ignore = false;

	for (int i = 0; i < TagsToIgnore.Num() && !ignore; i++)
	{
		ignore = Tags.Contains(TagsToIgnore[i]);
	}

	return ignore;
}