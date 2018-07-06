// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include <HttpModule.h>
#include "InfinarioDataTypes.h"
#include "InfinarioSDK.h"
#include <JsonWriter.h>
#include <JsonSerializer.h>
#include "Infinario.generated.h"

class FHttpModule;
class FJsonObject;

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
	void Initialize( const FString& ProjectTokenToSet, const FString& TargetToSet = TEXT( "" ), const FString& AppVersionToSet = TEXT( "" ) );

	/** Set player identity. Also saves the uuid to disk. */
	void Identify( const FString& PlayerIdentityToSet );

	/** Main tracking event */
	void Track( const FString& ActionName, const TMap< FString, FInfinarioData >& Payload, const float TimeStamp = -1.0f );

	/** Shortcut to track Session Start event */
	void TrackSessionStart( TMap< FString, FInfinarioData >& Payload );

	/** Shortcut to track Session End event */
	void TrackSessionEnd( TMap< FString, FInfinarioData >& Payload );

	/** Shortcut to track Virtual Payment event */
	void TrackVirtualPayment( const FVirtualPayment& VirtualPaymentData );

	/** Stores data attached to every call being made. */
	TMap< FString, FInfinarioData > EveryCallPayload;

	/** Create data attached to every call. */
	void CreateEveryCallPayload( const TMap< FString, FInfinarioData >& EveryCallPayloadToSet );

	/**
	 *Getters
	 */

public:
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

	//===------------------------------------------------------------------===//
	// Private
	//===------------------------------------------------------------------===//

private:
	/** Stores Project token. Set by Initialize function. */
	FString ProjectToken;

	/** Stores App version. Set by Initialize function. */
	FString AppVersion;

	/** Stores Target. Default is set to api.infinario.com/bulk. Set by Initialize function. */
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

	/** Helper function */
	void FillProperties( const TMap< FString, FInfinarioData >& PayloadToParse, TSharedRef< FJsonObject > OutProperties );

	/** Returns map of HW specifications for the active platform */
	TMap< FString, FInfinarioData > GetHWPayload( );

	/** Generate new UUID */
	FString GenerateUUID( );

	//===------------------------------------------------------------------===//
	// BP library
	//===------------------------------------------------------------------===//

public:
	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_Initialize( const FString& ProjectTokenToSet, const FString& TargetToSet, const FString& AppVersionToSet );

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_Track( const FString ActionName, const TMap< FString, FInfinarioData >& Payload, const float TimeStamp = -1.0f );

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
	void BP_Track_SessionStart( TMap< FString, FInfinarioData > Payload );

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_Track_SessionEnd( TMap< FString, FInfinarioData > Payload );

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_Track_VirtualPayment( const FVirtualPayment& VirtualPaymentData );

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	void BP_CreateEveryCallPayload( const TMap< FString, FInfinarioData >& EveryCallPayloadToSet );
};
