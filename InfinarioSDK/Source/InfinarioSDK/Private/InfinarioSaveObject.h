// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "InfinarioSaveObject.generated.h"

UCLASS( )
class UInfinarioSaveObject : public USaveGame
{
	GENERATED_BODY( )

public:
	UPROPERTY( )
	FString UUID;

	UInfinarioSaveObject( );
};

UCLASS()
class UInfinarioPersistencyManager : public UObject
{
	GENERATED_BODY()

public:
	/** Save UUID to disk */
	static void SaveUUID( const FString& UUIDToSave );

	/** Load and return saved UUID */
	static FString LoadUUID( );
};
