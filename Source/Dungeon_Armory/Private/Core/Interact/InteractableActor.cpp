// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Interact/InteractableActor.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::Interact_Implementation(AActor* Caller)
{
	UE_LOG(LogTemp, Warning, TEXT("UInteractableComponent::Interact Called"));
	// 실제 로직은 여기서 구현
}

bool AInteractableActor::CanInteract_Implementation(AActor* Interactor)
{
	return bIsInteractable;
}

void AInteractableActor::EnableOutline_Implementation()
{
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);

	for (auto* MeshComp : MeshComponents)
	{
		if (MeshComp)
		{
			MeshComp->SetRenderCustomDepth(true);
			MeshComp->SetCustomDepthStencilValue(1); // 예: 하이라이트용 스텐실
		}
	}
}

void AInteractableActor::DisableOutline_Implementation()
{
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);

	for (auto* MeshComp : MeshComponents)
	{
		if (MeshComp)
		{
			MeshComp->SetRenderCustomDepth(false);
		}
	}
}
