// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mob/Tasks/BTTask_Attack.h"

#include "Characters/Mob/MobBase.h"
#include "Characters/Mob/AIController/MobAIController.h"
#include "Characters/Mob/Component/MobAttackComponent.h"

#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Core/Component/MovementControllerComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	NodeName = TEXT("Mob Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    auto AIController = OwnerComp.GetAIOwner();
    auto Mob = Cast<AMobBase>(AIController->GetPawn());
    AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeys::Mob::Target));

    if (!Mob || !Target)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

	bool bIsAttacking = false;
	if (auto AnimInstance = Mob->GetMesh()->GetAnimInstance())
	{
		// 현재 어떤 몽타주라도 재생 중이라면 true
		bIsAttacking = AnimInstance->IsAnyMontagePlaying();
	}

	if (bIsAttacking)
	{
		return;
	}

    // 1. 방향 계산
    FVector LookDir = (Target->GetActorLocation() - Mob->GetActorLocation()).GetSafeNormal2D();
    FRotator TargetRot = LookDir.Rotation();

    // 2. 회전
    FRotator CurrentRot = Mob->GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, 12.f);
    Mob->SetActorRotation(NewRot);

    // 3. 각도 차이 확인 (Shortest Path 계산)
    FRotator DeltaRot = (TargetRot - CurrentRot).GetNormalized(); // -180 ~ 180 사이로 정규화
	float AngleDiff = FMath::Abs(DeltaRot.Yaw);

    auto AttackComponent = Cast<UMobAttackComponent>(Mob->_AttackComponent);

    // 4. 충분히 돌아봤다면 공격 시작
	if (AttackComponent)
	{
		if (AngleDiff < 5.f)
		{
			AttackComponent->StartAttack();
		}

		if (AttackComponent->GetCanAttack())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
