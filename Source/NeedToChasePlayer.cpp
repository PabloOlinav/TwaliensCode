// All rights reserved.

#include "NeedToChasePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void UNeedToChasePlayer::OnGameplayTaskActivated(UGameplayTask & Task)
{
	
}

void UNeedToChasePlayer::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
	
}

UNeedToChasePlayer::UNeedToChasePlayer()
{
	ForceInstancing(true);
}

bool UNeedToChasePlayer::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	FVector TargetPosition, AgentPosition;
	UBlackboardComponent * blackboard;
	ACharacter * TargetActor;

	if(!AIControllerReference)
	{
		AIControllerReference = OwnerComp.GetAIOwner();
	}

	if (AIControllerReference->GetPawn())
	{
		float Distance;

		AgentPosition = AIControllerReference->GetPawn()->GetActorLocation();

		blackboard = OwnerComp.GetBlackboardComponent();
		TargetActor = Cast<ACharacter>(blackboard->GetValueAsObject(TargetKey.SelectedKeyName));

		if (TargetActor)
		{
			TargetPosition = TargetActor->GetActorLocation();

			if (OverlapDistance)
			{
				ACharacter * agent;
				agent = Cast<ACharacter>(AIControllerReference->GetPawn());
				if (agent)
				{
					Distance = blackboard->GetValueAsFloat(MeleeRange.SelectedKeyName) + TargetActor->GetCapsuleComponent()->GetScaledCapsuleRadius() + agent->GetCapsuleComponent()->GetScaledCapsuleRadius();
				}
			}
			else
			{
				Distance = blackboard->GetValueAsFloat(MeleeRange.SelectedKeyName);
			}

			return FVector::Dist(TargetPosition, AgentPosition) > Distance;
		}
		//Workaround we need to create a interface for targetting TODO - create interface for targetting - a$tuR
		else
		{
			AActor * TauntTarget = Cast<AActor>(blackboard->GetValueAsObject(TargetKey.SelectedKeyName));

			if (TauntTarget)
			{
				TargetPosition = TauntTarget->GetActorLocation();

				if (OverlapDistance)
				{
					ACharacter * agent;
					agent = Cast<ACharacter>(AIControllerReference->GetPawn());
					if (agent)
					{
						Distance = blackboard->GetValueAsFloat(MeleeRange.SelectedKeyName) + 100.f;
					}
				}
				else
				{
					Distance = blackboard->GetValueAsFloat(MeleeRange.SelectedKeyName);
				}

				return FVector::Dist(TargetPosition, AgentPosition) > Distance;
			}
			else
			{
				return false;
			}
		}
			
	}
	else
		return false;
}