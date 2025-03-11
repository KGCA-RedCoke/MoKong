// CopyRight KGCA - Team RedCoke


#include "MokongBlueprintFunctionLibrary.h"

#include "Data/MKAbilitySystemData.h"

EHitReactDirection UMokongBlueprintFunctionLibrary::GetHitDirection(AActor* Target, const FVector& ImpactPoint)
{
	const FVector& ActorLocation = Target->GetActorLocation();
	// PointPlaneDist is super cheap - 1 vector subtraction, 1 dot product.
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, Target->GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, Target->GetActorForwardVector());


	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		// Determine if Front or Back

		// Can see if it's left or right of Left/Right plane which would determine Front or Back
		if (DistanceToRightLeftPlane >= 0)
		{
			return EHitReactDirection::Forward;
		}
		else
		{
			return EHitReactDirection::Backward;
		}
	}
	else
	{
		// Determine if Right or Left

		if (DistanceToFrontBackPlane >= 0)
		{
			return EHitReactDirection::Right;
		}
		else
		{
			return EHitReactDirection::Left;
		}
	}
}
