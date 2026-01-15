#include "Characters/Mob/Tasks/BTTask_Skill.h"

#include "Characters/Mob/BossBase.h"

#include "Characters/Mob/AIController/MobAIController.h"

#include "Characters/Mob/Component/BossAttackComponent.h"

UBTTask_Skill::UBTTask_Skill()
{
	bNotifyTick = false;
	NodeName = TEXT("Mob Skill");
}

EBTNodeResult::Type UBTTask_Skill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    ABossBase* Boss = (AIController) ? Cast<ABossBase>(AIController->GetPawn()) : nullptr;

    if (!Boss)
        return EBTNodeResult::Failed;

    if (auto RoarMontage = Cast<UBossAttackComponent>(Boss->_AttackComponent)->RoarMontage)
    {
        Boss->PlayAnimMontage(RoarMontage);
        FTimerHandle TimerHandle;
        Boss->GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            FTimerDelegate::CreateUObject(this, &UBTTask_Skill::OnSkillFinished, &OwnerComp),
            RoarMontage->GetPlayLength(),
            false
        );
    }

    return EBTNodeResult::InProgress;
}

void UBTTask_Skill::OnSkillFinished(UBehaviorTreeComponent* OwnerComp)
{
    if (OwnerComp)
    {
        FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);

        Cast<AMobAIController>(OwnerComp->GetAIOwner())->SetSkillPhase(false);
    }
}
