// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MKAttributeSet.generated.h"

// Boilerplate for Attributes	(접근 제한자 자동 매크로)
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct FGameplayTag;

/**
 * 게임플레이 관련 부동 소수점 값을 저장, 계산, 수정
 * 이 값은 캐릭터의 남은 생명력, 비히클의 최고 속력, 아이템이 파괴되기 전까지 사용 가능한 횟수 등 소유자의 어떤 특성이든 설명할 수 있음
 * ----> 이러한 값들을 바로 이클래스 Attribute Set에 저장함
 * - 어트리뷰트를 액터의 어빌리티 시스템 컴포넌트에 등록,
 * - 게임플레이 어트리뷰트와 시스템의 나머지 부분 간의 인터랙션을 관리.
 * - 이러한 인터랙션에는 값 범위 제한, 일시적 값 변경을 적용하는 계산 수행, 영구적으로 베이스 값을 변경하는 이벤트에 대한 반응 등
 * --------------------------------------------------------------------------------------
 * Attribute Set에는 Current와 Base의 값이 저장됨
 * Base값은 주로 고정된 값이고 (Level up등으로 값이 변경 될 수 있음)
 * Current는 현재 사용되고 있는 값을 뜻함
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UMKAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	void AdjustAttributeForMaxChange(const FGameplayAttribute& AffectedAttribute, const float OldMaxValue,
									 const float               NewMaxValue) const;

	void CheckMaxReachedForAttribute(const FGameplayAttributeData& MaxAttribute, const FGameplayTag& MaxTag,
									 const float&                  NewValue) const;
	void CheckStatusTagForAttribute(const FGameplayTag& StatusTag, const float& NewValue, const float& OldValue) const;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const {};

};
