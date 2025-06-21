// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ManagerBase.generated.h"

/**
 * UManagerBase: 서브시스템의 기본 클래스
 */
UCLASS(Abstract) // 직접 인스턴스화 방지
class DUNGEON_ARMORY_API UManagerBase : public UGameInstanceSubsystem
{
	GENERATED_BODY()
};

/**
 * TManagerBase<T>: 제네릭 싱글톤 서브시스템 관리 템플릿
 * (템플릿 클래스는 선언과 정의를 분리할 수 없음)
 */
template <typename T>
class TManagerBase
{
private:
	static T* Instance;

public:
	/** 초기화 (UManagerBase에서 호출) */
	static void InitializeInstance(T* NewInstance)
	{
		if (NewInstance)
		{
			Instance = NewInstance;
		}
	}

	/** 해제 (UManagerBase에서 호출) */
	static void DestroyInstance()
	{
		Instance = nullptr;
	}

	/** 싱글톤 반환 */
	static T* GetInstance()
	{
		// 이미 캐싱된 인스턴스가 있다면 즉시 반환
		if (Instance)
		{
			return Instance;
		}

		// 캐싱된 값이 없다면, 다시 찾고 캐싱
		if (GEngine)
		{
			// 현재 활성화된 월드 찾기
			for (const FWorldContext& Context : GEngine->GetWorldContexts())
			{
				if (Context.World())
				{
					if (UGameInstance* GameInstance = Context.World()->GetGameInstance())
					{
						Instance = GameInstance->GetSubsystem<T>();
						return Instance;
					}
				}
			}
		}
		return nullptr;
	}
};

// 정적 변수 정의 (이 부분은 템플릿이 정의된 헤더에 포함되어야 합니다)
template <typename T>
T* TManagerBase<T>::Instance = nullptr;