// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IToolEuipable.generated.h"

UENUM(BlueprintType)
enum class EToolType : uint8
{
	Other    UMETA(DisplayName = "Other"),

	Weapon   UMETA(DisplayName = "Weapon"),
	Axe      UMETA(DisplayName = "Axe"),
	Pickaxe  UMETA(DisplayName = "Pickaxe")
};

// 도구 액터의 ToolType을 가져오는 인터페이스
UINTERFACE(MinimalAPI)
class UIToolEuipable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEON_ARMORY_API IIToolEuipable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	EToolType GetToolType() const;

	virtual EToolType GetToolType_Implementation() const = 0; // 순수 가상 함수
};
