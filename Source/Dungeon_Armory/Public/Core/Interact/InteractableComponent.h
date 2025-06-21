// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "Core/Interact/Interface/Interactable.h"

#include "InteractableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DUNGEON_ARMORY_API UInteractableComponent : public UActorComponent, public IInteractable
{
	GENERATED_BODY()

public:
	UInteractableComponent();

/***** Interact *****/
protected:
	UPROPERTY(EditAnywhere)
	bool bIsInteractable = true;

public:
	// 상호작용 여부 판단
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(AActor* Interactor);

	// 상호작용 실행
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Interactor);

	// 아웃라인 제어
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EnableOutline();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void DisableOutline();

};
