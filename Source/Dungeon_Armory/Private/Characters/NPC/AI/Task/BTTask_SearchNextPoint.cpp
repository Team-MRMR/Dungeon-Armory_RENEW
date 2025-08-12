#include "Characters/NPC/AI/Task/BTTask_SearchNextPoint.h"
#include "Characters/NPC/AI/NPCAIController.h"
#include "Characters/NPC/AI/LocationPoint.h"
#include "Characters/NPC/NPCBase.h"


#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_SearchNextPoint::UBTTask_SearchNextPoint()
{
	bNotifyTick = false;

	NodeName = TEXT("Search Point");
}

EBTNodeResult::Type UBTTask_SearchNextPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	FVector PointLocation= NPC->GetPointLocation();

	if (bIsShopping)
	{
		Blackboard->SetValueAsVector(BBKeys::NPC::ShoppingPoint, PointLocation);
	}
	else
	{
		Blackboard->SetValueAsVector(BBKeys::NPC::RoammingPoint, PointLocation);
	}

	return EBTNodeResult::Succeeded;
}
