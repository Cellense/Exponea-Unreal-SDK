// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#include "InfinarioSaveObject.h"
#include "Kismet/GameplayStatics.h"
#include "InfinarioDefaults.h"

UInfinarioSaveObject::UInfinarioSaveObject( )
{
	UUID = TEXT( "" );
}

void UInfinarioPersistencyManager::SaveUUID( const FString& UUIDToSave )
{
	UInfinarioSaveObject* SaveGameInstance = Cast< UInfinarioSaveObject >( UGameplayStatics::CreateSaveGameObject( UInfinarioSaveObject::StaticClass( ) ) );
	SaveGameInstance->UUID				   = UUIDToSave;
	UGameplayStatics::SaveGameToSlot( SaveGameInstance, SAVE_SLOT_NAME, SAVE_SLOT_IDX );
}

FString UInfinarioPersistencyManager::LoadUUID( )
{
	UInfinarioSaveObject* LoadGameInstance;

	if( UGameplayStatics::DoesSaveGameExist( SAVE_SLOT_NAME, SAVE_SLOT_IDX ) )
	{
		LoadGameInstance = Cast< UInfinarioSaveObject >( UGameplayStatics::LoadGameFromSlot( SAVE_SLOT_NAME, SAVE_SLOT_IDX ) );
	}
	else
	{
		LoadGameInstance = NewObject< UInfinarioSaveObject >( );
		LoadGameInstance = Cast< UInfinarioSaveObject >( UGameplayStatics::CreateSaveGameObject( UInfinarioSaveObject::StaticClass( ) ) );
	}

	return LoadGameInstance->UUID;
}
