// copyright Jacob Lim

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootStepSFXComponent.generated.h"

class UNiagaraSystem;

/**
 * 발이 지면에 닿을 때 SFX 재생 관리 (Sound, VFX 판별) 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FOOTSFX_API UFootStepSFXComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	/** FootStepAnimNotify에서 이 함수를 실행 (여기서 사운드와 파티클효과 재생) */
	void FootStep();

	/**
	 * 현재 캐릭터가 보여주고있는 애니메이션시퀀스를 가져와서 멤버변수로 설정 
	 * @param NewAnimation 캐릭터가 실행중인 시퀀스
	 */
	void SetAnimation(UAnimSequenceBase* NewAnimation);

private:
	/** 캐릭터 레퍼런스 초기화 */
	void GetRefData();

	/** 어떤 파티클을 재생해야하는지 피직스머티리얼로 판별 */
	void SpawnFootStepParticle();

	/**
	 * 소켓 위치로부터 바닥으로 Trace를 실행하고 결과 저장 
	 * @param SocketName 
	 */
	void FootLineTrace(const FName& SocketName);

	UFUNCTION()
	void OnActorHit_Event(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
#pragma region Debug

	/** 디버그 모드 활성화? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FootStep|Debug")
	bool bDebugMode;

	/** 디버그 모드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FootStep|Debug")
	float DebugDuration;

#pragma endregion Debug

#pragma region Footstep

	UPROPERTY(BlueprintReadOnly, Category = "FootStep|SFX")
	int SoundIndex;

	UPROPERTY(BlueprintReadOnly, Category = "FootStep|VFX")
	int ParticleIndex;

	UPROPERTY(BlueprintReadOnly, Category = "FootStep")
	FVector FootStepLocation;

	UPROPERTY(BlueprintReadOnly, Category = "FootStep")
	FName ParticleName;

	UPROPERTY(BlueprintReadOnly, Category = "FootStep")
	FName FootName;

	UPROPERTY(BlueprintReadOnly, Category = "FootStep|SFX")
	USoundBase* FootStep_Sound;

	UPROPERTY(BlueprintReadOnly, Category = "FootStep|Animation")
	UAnimSequenceBase* Animation;

	UPROPERTY(BlueprintReadOnly, Category = "FootStep|GroundInfo")
	UPhysicalMaterial* PhysicalMaterial;

	// TMap<>

#pragma endregion Footstep

#pragma region Data

	UPROPERTY(BlueprintReadOnly, Category = "FootStep|Data")
	ACharacter* CharacterRef;

	UPROPERTY(BlueprintReadOnly, Category = "FootStep|Data")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadOnly, Category = "FootStep|Data")
	UObject* ObjectRef;

	UPROPERTY(BlueprintReadOnly, Category = "FootStep|Data")
	USoundBase* ObjectSound;

#pragma endregion Data

#pragma region Sounds

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|SFX")
	TMap<FName, USoundBase*> SoundMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|SFX")
	USoundBase* WalkDefault_Sound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|SFX")
	USoundBase* RunDefault_Sound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|SFX")
	USoundBase* ObjectDefault_Sound;

#pragma endregion Sounds

#pragma region Niagara

	UPROPERTY(BlueprintReadOnly, Category = "FootStep|VFX")
	UNiagaraSystem* Niagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|VFX")
	TMap<FName, UNiagaraSystem*> NiagaraMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|VFX")
	UNiagaraSystem* Footstep_Default_Niagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|VFX")
	UNiagaraSystem* Object_Default_Niagara;

#pragma endregion Niagara

#pragma region Particle

	UPROPERTY(BlueprintReadOnly, Category = "FootStep|VFX")
	UParticleSystem* Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|VFX")
	TMap<FName, UParticleSystem*> ParticleMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|VFX")
	UParticleSystem* Footstep_Default_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|VFX")
	UParticleSystem* Object_Default_Particle;

#pragma endregion Particle

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	float ParticleScale;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	float NiagaraScale;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	float ObjectImpactThreshold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	float FootStepTraceHeight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	bool bOneStepAtATime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	bool bObjectParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	bool bFootStepParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	bool bObjectNiagara;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	bool bFootStepNiagara;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	bool bSpawnParticleForWalk;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	bool bSpawnParticleForRun;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	FName LeftFootStepSocket;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FootStep|Edit")
	FName RightFootStepSocket;
};
