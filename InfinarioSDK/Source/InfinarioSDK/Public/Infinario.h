// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include <HttpModule.h>
#include "Private/InfinarioDataTypes.h"
#include "InfinarioSDK.h"
#include <JsonWriter.h>
#include <JsonSerializer.h>
#include "Infinario.generated.h"

class FHttpModule;
class FJsonObject;

UENUM( BlueprintType, Blueprintable )
enum class EDataStatus : uint8
{
	None,
	FString,
	Int,
	Float,
	Bool
};

USTRUCT( BlueprintType, Blueprintable )
struct FInfinarioData
{
	GENERATED_BODY( )

	FInfinarioData( )
	{
	}

	FInfinarioData( int32 Value )
		: FInfinarioData( )
	{
		if( Status == EDataStatus::None )
		{
			In	 = Value;
			Status = EDataStatus::Int;
		}
	}

	FInfinarioData( FString Value )
		: FInfinarioData( )
	{
		if( Status == EDataStatus::None )
		{
			St	 = Value;
			Status = EDataStatus::FString;
		}
	}

	FInfinarioData( float Value )
		: FInfinarioData( )
	{
		if( Status == EDataStatus::None )
		{
			Fl	 = Value;
			Status = EDataStatus::Float;
		}
	}

	FInfinarioData( bool Value )
		: FInfinarioData( )
	{
		if( Status == EDataStatus::None )
		{
			Bo	 = Value;
			Status = EDataStatus::Bool;
		}
	}

	FORCEINLINE EDataStatus GetStatus( ) const
	{
		return Status;
	}

	UPROPERTY( )
	EDataStatus Status = EDataStatus::None;

	UPROPERTY( )
	int32 In;

	UPROPERTY( )
	FString St;

	UPROPERTY( )
	float Fl;

	UPROPERTY( )
	bool Bo;
};

USTRUCT( BlueprintType, Blueprintable )
struct FVirtualPayment
{
	GENERATED_BODY( )

	FVirtualPayment( )
	{
	}

	FVirtualPayment( FInfinarioData CurrencyToSet, FInfinarioData AmountToSet, FInfinarioData ItemNameToSet, FInfinarioData ItemTypeToSet )
	{
		Currency = CurrencyToSet;
		Amount   = AmountToSet;
		ItemName = ItemNameToSet;
		ItemType = ItemTypeToSet;
	}

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Infinario" )
	FInfinarioData Currency;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Infinario" )
	FInfinarioData Amount;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Infinario" )
	FInfinarioData ItemName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Infinario" )
	FInfinarioData ItemType;

	TMap< FString, FInfinarioData > GetMap( ) const
	{
		TMap< FString, FInfinarioData > VirtualCurrencyData;

		VirtualCurrencyData.Add( PROPERTY_CURRENCY, Currency );
		VirtualCurrencyData.Add( PROPERTY_AMOUNT, Amount );
		VirtualCurrencyData.Add( PROPERTY_ITEM_NAME, ItemName );
		VirtualCurrencyData.Add( PROPERTY_ITEM_TYPE, ItemType );

		return VirtualCurrencyData;
	}
};

UCLASS( BlueprintType, Blueprintable ) class INFINARIOSDK_API UInfinario : public UObject
{
	GENERATED_BODY( )

	//===------------------------------------------------------------------===//
	// Lifetime
	//===------------------------------------------------------------------===//

public:
	UInfinario( );
	~UInfinario( );
	static UInfinario* GetInstance( UObject* WorldContext );

protected:
	static UInfinario* Instance;

	//===------------------------------------------------------------------===//
	// API
	//===------------------------------------------------------------------===//

public:
	/** Initialize  */
	void Initialize( const FString& ProjectTokenToSet, const FString& AppVersionToSet = TEXT( "" ), const FString& TargetToSet = TEXT( "" ) );

	/** Set player identity */
	void Identify( const FString& PlayerIdentityToSet );

	/** Main tracking event */
	void Track( const FString ActionName, const TMap< FString, FInfinarioData >& Payload );

	/** Shortcut to track Session Start event */
	void TrackSessionStart( const TMap< FString, FInfinarioData >& Payload );

	/** Shortcut to track Session End event */
	void TrackSessionEnd( const TMap< FString, FInfinarioData >& Payload );

	/** Shortcut to track Virtual Payment event */
	void TrackVirtualPayment( const FVirtualPayment& VirtualPaymentData );

	/** Stores data attached to every call being made. */
	TMap< FString, FInfinarioData > EveryCallPayload;

	/** Create data attached to every call. */
	void CreateEveryCallPayload( const TMap< FString, FInfinarioData >& EveryCallPayloadToSet );

	/**
	 *Getters
	 */

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	FORCEINLINE FString GetProjectToken( ) const
	{
		return ProjectToken;
	}

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	FORCEINLINE FString GetAppVersion( ) const
	{
		return AppVersion;
	}

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	FORCEINLINE FString GetTarget( ) const
	{
		return Target;
	}

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	FORCEINLINE bool GetIsInitialized( ) const
	{
		if( !bIsInitialized )
		{
			INF_LOG_W( TEXT( "Infinario plugin is not initialized. Please call Initialize () first." ) )
		}

		return bIsInitialized;
	}

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	FORCEINLINE FString GetPlayerIdentity( ) const
	{
		return PlayerIdentity;
	}

private:
	/** Stores Project token. Set by Initialize function. */
	FString ProjectToken;

	/** Stores App version. Set by Initialize function. */
	FString AppVersion;

	/** Stores Target. Set by Initialize function. */
	FString Target;

	/** Is true in case plugin is correctly initialized by user.  */
	bool bIsInitialized;

	/** Player identity */
	FString PlayerIdentity;

	/** HTTP Module handling communication */
	FHttpModule* Http;

	/** Process a request and return a status */
	bool DoRequest( TSharedRef< FJsonObject > Payload );

	/** Assign this function to call when the GET request processes successfully */
	void OnResponseReceived( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful );

	/** Returns FString from FJsonObject */
	FORCEINLINE FString GetJSONAsString( const TSharedRef< FJsonObject > JSONObj ) const
	{
		FString outputString;

		TSharedRef< TJsonWriter<> > writer = TJsonWriterFactory<>::Create( &outputString );

		FJsonSerializer::Serialize( JSONObj, writer );

		return outputString;
	}

	void FillProperties( const TMap< FString, FInfinarioData > PayloadToParse, TSharedRef< FJsonObject > OutProperties );

	//===------------------------------------------------------------------===//
	// BP library
	//===------------------------------------------------------------------===//

public:
	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_Initialize( const FString& ProjectTokenToSet, const FString& AppVersionToSet, const FString& TargetToSet );

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_Track( const FString ActionName, const TMap< FString, FInfinarioData >& Payload );

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	FInfinarioData BP_SetIntValue( const int32 Value ) const;

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	FInfinarioData BP_SetFloatValue( const float Value ) const;

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	FInfinarioData BP_SetBoolValue( const bool Value ) const;

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	FInfinarioData BP_SetStringValue( const FString Value ) const;

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_Identify( const FString& PlayerIdentityToSet );

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_Track_SessionStart( const TMap< FString, FInfinarioData >& Payload );

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_Track_SessionEnd( const TMap< FString, FInfinarioData >& Payload );

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_Track_VirtualPayment( const FVirtualPayment& VirtualPaymentData );

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_CreateEveryCallPayload( const TMap< FString, FInfinarioData >& EveryCallPayloadToSet );
};

/*

 {
	"commands": [
		{
			"name": "crm/events",
			"data":
			{
				"project_id": "b9b54c06-3cbd-11e8-811d-b083fed3a8cb",
				"type": "session_end",
				"customer_ids":
				{
					"registered": "924"
				},
				"properties":
				{
					"player_name": "924",
					"coins_balance": 9113,
					"session_number": 31,
					"time_in_game": 387,
					"session_duration": 2,
					"country": "en_US",
					"dev": true,
					"device_model": "GenuineIntel|Intel(R) Core(TM) i5-6600K CPU @ 3.50GHz",
					"device_type": "GenuineIntel|Intel(R) Core(TM) i5-6600K CPU @ 3.50GHz",
					"game_version": "clicker",
					"os_name": "",
					"os_version": "",
					"CPU": 0,
					"RAM": -1,
					"Cores": 1,
					"GPU_family": "GenericGPUBrand",
					"GPU_version": "GenericGPUBrand"
				}
			}
		}
	]
}
*/
