// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Core/Animation/ReceiveInputNotify.h"

#include "Characters/Mannequin/Manny.h"

#include "Characters/Mannequin/Interface/IToolEuipable.h"
#include "Characters/Mannequin/Component/GatherComponent.h"
#include "Characters/Mannequin/Component/PlayerAttackComponent.h"

void UReceiveInputNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
		return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
		return;

	auto Player = Cast<AManny>(Owner);
	auto IToolEuipable = Cast<IIToolEuipable>(Owner);
	if (!Player || !IToolEuipable)
		return;

	EToolType ToolType = IToolEuipable->Execute_GetToolType(Owner);
	if (ToolType == EToolType::Weapon)
	{
		auto AttackComponent = Player->FindComponentByClass<UPlayerAttackComponent>();
		if (!AttackComponent)
			return;

		AttackComponent->ReceiveInput();
	}
	else if (ToolType == EToolType::Axe || ToolType == EToolType::Pickaxe)
	{
		auto GatherComponent = Player->FindComponentByClass<UGatherComponent>();
		if (!GatherComponent)
			return;

		GatherComponent->ReceiveInput();
	}

	return;
}
