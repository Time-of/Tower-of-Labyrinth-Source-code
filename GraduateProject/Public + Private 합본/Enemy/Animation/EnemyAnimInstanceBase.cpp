
#include "Enemy/Animation/EnemyAnimInstanceBase.h"

#include "Enemy/EnemyCharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"



UEnemyAnimInstanceBase::UEnemyAnimInstanceBase() : Super(),
	GroundMoveSpeed(0.0f), bIsFalling(false),
	MoveDirection(0.0f), YawDelta(0.0f)
{

}



void UEnemyAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwnerEnemy))
	{
		OwnerEnemy = Cast<AEnemyCharacterBase>(TryGetPawnOwner());
	}
	else
	{
		const FVector Velocity = OwnerEnemy->GetVelocity();
		const FRotator Rotation = OwnerEnemy->GetActorRotation();

		GroundMoveSpeed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
		bIsFalling = OwnerEnemy->GetCharacterMovement()->IsFalling();
		MoveDirection = CalculateDirection(Velocity, Rotation);
		YawDelta = UKismetMathLibrary::NormalizedDeltaRotator(OwnerEnemy->GetControlRotation(), Rotation).Yaw;
	}
}
