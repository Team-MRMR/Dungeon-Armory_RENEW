#include "Characters/NPC/AI/Task/BTTask_SearchPoint.h"
#include "Characters/NPC/AI/LocationPoint.h"
#include "Characters/NPC/NPCBase.h"

#include "Characters/Core/AI/AIControllerBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_SearchPoint::UBTTask_SearchPoint()
{
	bNotifyTick = false;

	NodeName = TEXT("Search Point");
}

EBTNodeResult::Type UBTTask_SearchPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return EBTNodeResult::Failed;

	ANPCBase* NPC = Cast<ANPCBase>(AIController->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;

	bool bIsShopping = NPC->GetIsShopping();
	ALocationPoint* LocationPoint = NPC->GetPoint(bIsShopping);
	if (!LocationPoint)
		return EBTNodeResult::Failed;

	Blackboard->SetValueAsObject(BBKeys::LocationPoint, LocationPoint);

	return EBTNodeResult::Type();
}
