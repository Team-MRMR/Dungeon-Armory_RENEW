#include "Characters/Mob/Tasks/BTTask_Stun.h"

#include "Characters/Mob/BossBase.h"

#include "Characters/Mob/AIController/MobAIController.h"

#include "Characters/Mob/Component/BossAttackComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Stun::UBTTask_Stun()
{
	bNotifyTick = false;
	NodeName = TEXT("Mob Stun");
}

EBTNodeResult::Type UBTTask_Stun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    ABossBase* Boss = (AIController) ? Cast<ABossBase>(AIController->GetPawn()) : nullptr;

    if (!Boss)
        return EBTNodeResult::Failed;

    if (auto StunMontage = Cast<UBossAttackComponent>(Boss->_AttackComponent)->StunMontage)
    {
        Boss->PlayAnimMontage(StunMontage);
    }

    FTimerHandle TimerHandle;
    Boss->GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UBTTask_Stun::OnStunFinished, &OwnerComp), StunDuration, false);

    return EBTNodeResult::InProgress;
}

void UBTTask_Stun::OnStunFinished(UBehaviorTreeComponent* OwnerComp)
{
    AAIController* AIController = OwnerComp->GetAIOwner();
    ABossBase* Boss = (AIController) ? Cast<ABossBase>(AIController->GetPawn()) : nullptr;

    if (!Boss)
    {
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
        return;
    }

    Boss->StopAnimMontage();

    FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
