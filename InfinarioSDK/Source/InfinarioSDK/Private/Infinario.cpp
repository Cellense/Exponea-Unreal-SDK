// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#include "Infinario.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include <IHttpRequest.h>
#include <SharedPointer.h>
#include "InfinarioSDK.h"
#include <JsonObject.h>
#include "InfinarioDataTypes.h"
#include "Private/InfinarioDefaults.h"
#include "HAL/PlatformMisc.h"
#include "Private/HardwareSpec.h"

UInfinario* UInfinario::Instance;

UInfinario::UInfinario( )
{
	// When the object is constructed, Get the HTTP module
	Http = &FHttpModule::Get( );
}

UInfinario* UInfinario::GetInstance( UObject* WorldContext )
{
	if( Instance == nullptr )
	{
		Instance = NewObject< UInfinario >( WorldContext );
	}

	return Instance;
}

void UInfinario::Initialize( const FString& ProjectTokenToSet, const FString& AppVersionToSet /*= TEXT( "" )*/, const FString& TargetToSet /*= TEXT( "" ) */ )
{
	INF_LOG_L( TEXT( "Initialization started." ) )

	ProjectToken = ProjectTokenToSet;
	AppVersion   = AppVersionToSet;
	Target		 = TargetToSet;

	/** Set default name for player */
	Identify( DEFAULT_PLAYER_IDENTITY );

	bIsInitialized = true;
	INF_LOG_L( TEXT( "Initialization completed." ) )
}

void UInfinario::Identify( const FString& PlayerIdentityToSet )
{
	/** Do not set if the user fill the same name */
	if( PlayerIdentityToSet.Equals( PlayerIdentity ) )
	{
		return;
	}

	/** Assign default name if empty */
	PlayerIdentity = PlayerIdentityToSet.IsEmpty( ) ? DEFAULT_PLAYER_IDENTITY : PlayerIdentityToSet;
}

void UInfinario::Track( const FString ActionName, const TMap< FString, FInfinarioData >& Payload, const float TimeStamp /*= -1.0f */ )
{
	//	https://forums.unrealengine.com/development-discussion/c-gameplay-programming/121818-need-help-to-get-json-with-cpp
	//	http://www.wraiyth.com/?p=198
	//	https://answers.unrealengine.com/questions/341767/how-to-add-an-array-of-json-objects-to-a-json-obje.html

	/** Do not execute when plugin is not initialized */
	if( !GetIsInitialized( ) )
	{
		return;
	}

	TSharedPtr< FJsonObject > payloadToSend = MakeShareable( new FJsonObject( ) );

	TArray< TSharedPtr< FJsonValue > > commandsArray;
	TSharedPtr< FJsonObject >		   commands		= MakeShareable( new FJsonObject( ) );
	TSharedPtr< FJsonObject >		   data			= MakeShareable( new FJsonObject( ) );
	TSharedPtr< FJsonObject >		   customer_ids = MakeShareable( new FJsonObject( ) );
	TSharedPtr< FJsonObject >		   properties   = MakeShareable( new FJsonObject( ) );

	commands->SetStringField( "name", "crm/events" );   // #todo Handle as a parameter
	customer_ids->SetStringField( "registered", GetPlayerIdentity( ) );
	data->SetStringField( "project_id", ProjectToken );
	data->SetStringField( "type", ActionName );
	data->SetObjectField( "customer_ids", customer_ids );
	data->SetObjectField( "properties", properties );

	/** Fill tracking payload */
	FillProperties( Payload, properties.ToSharedRef( ) );

	/** Fill every call data if present. */
	if( EveryCallPayload.Num( ) > 0 )
	{
		FillProperties( EveryCallPayload, properties.ToSharedRef( ) );
	}

	/** Append custom user timestamp if present. */
	if( TimeStamp > 0.0f )
	{
		properties->SetNumberField( PROPERTY_TIMESTAMP, TimeStamp );
	}

	commands->SetObjectField( "data", data );

	//////////////////////////////////////////////////////////////////////////

	TSharedPtr< FJsonValueObject > commandsValObj = MakeShareable( new FJsonValueObject( commands ) );

	commandsArray.Add( commandsValObj );

	payloadToSend->SetArrayField( "commands", commandsArray );

	DoRequest( payloadToSend.ToSharedRef( ) );
}

void UInfinario::TrackSessionStart( TMap< FString, FInfinarioData >& Payload )
{
	Payload.Append( GetHWPayload( ) );
	Track( EVENT_SESSION_START, Payload );
}

void UInfinario::TrackSessionEnd( TMap< FString, FInfinarioData >& Payload )
{
	Payload.Append( GetHWPayload( ) );
	Track( EVENT_SESSION_END, Payload );
}

void UInfinario::TrackVirtualPayment( const FVirtualPayment& VirtualPaymentData )
{
	TMap< FString, FInfinarioData > payload = VirtualPaymentData.GetMap( );
	Track( EVENT_VIRTUAL_PAYMENT, payload );
}

void UInfinario::BP_Initialize( const FString& ProjectTokenToSet, const FString& AppVersionToSet, const FString& TargetToSet )
{
	Initialize( ProjectTokenToSet, AppVersionToSet, TargetToSet );
}

void UInfinario::BP_Track( const FString ActionName, const TMap< FString, FInfinarioData >& Payload, const float TimeStamp /* = -1.0f*/ )
{
	Track( ActionName, Payload, TimeStamp );
}

FInfinarioData UInfinario::BP_SetFloatValue( const float Value ) const
{
	return FInfinarioData( Value );
}

FInfinarioData UInfinario::BP_SetIntValue( const int32 Value ) const
{
	return FInfinarioData( Value );
}

FInfinarioData UInfinario::BP_SetBoolValue( const bool Value ) const
{
	return FInfinarioData( Value );
}

FInfinarioData UInfinario::BP_SetStringValue( const FString Value ) const
{
	return FInfinarioData( Value );
}

void UInfinario::BP_Identify( const FString& PlayerIdentityToSet )
{
	Identify( PlayerIdentityToSet );
}

void UInfinario::BP_Track_SessionStart( TMap< FString, FInfinarioData > Payload )
{
	TrackSessionStart( Payload );
}

void UInfinario::BP_Track_SessionEnd( TMap< FString, FInfinarioData > Payload )
{
	TrackSessionEnd( Payload );
}

void UInfinario::BP_Track_VirtualPayment( const FVirtualPayment& VirtualPaymentData )
{
	TrackVirtualPayment( VirtualPaymentData );
}

void UInfinario::BP_CreateEveryCallPayload( const TMap< FString, FInfinarioData >& EveryCallPayloadToSet )
{
	CreateEveryCallPayload( EveryCallPayloadToSet );
}

bool UInfinario::DoRequest( TSharedRef< FJsonObject > Payload )
{
	INF_LOG( TEXT( "Request sent with payload" ), GetJSONAsString( Payload ) )

	//	Create request

	TSharedRef< IHttpRequest > Request = Http->CreateRequest( );
	Request->OnProcessRequestComplete( ).BindUObject( this, &UInfinario::OnResponseReceived );

	// This is the url on which to process the request

	Request->SetURL( "api.infinario.com/bulk" );   // #todo Replace?
	Request->SetVerb( "POST" );
	Request->SetHeader( TEXT( "User-Agent" ), "X-UnrealEngine-Agent" );
	Request->SetHeader( "Content-Type", TEXT( "application/json" ) );
	Request->SetContentAsString( GetJSONAsString( Payload ) );
	Request->ProcessRequest( );

	return true;
}

void UInfinario::OnResponseReceived( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful )
{
	// Create a pointer to hold the json serialized data
	TSharedPtr< FJsonObject > JsonObject;

	// Create a reader pointer to read the json data
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create( Response->GetContentAsString( ) );

	INF_LOG( TEXT( "Response from server" ), Response->GetContentAsString( ) )
}

void UInfinario::FillProperties( const TMap< FString, FInfinarioData >& PayloadToParse, TSharedRef< FJsonObject > OutProperties )
{
	for( auto item : PayloadToParse )
	{
		FInfinarioData& fobj = item.Value;
		if( fobj.GetStatus( ) == EDataStatus::Float )
		{
			OutProperties->SetNumberField( item.Key, fobj.Fl );
			continue;
		}
		else if( fobj.GetStatus( ) == EDataStatus::Int )
		{
			OutProperties->SetNumberField( item.Key, fobj.In );
			continue;
		}
		else if( fobj.GetStatus( ) == EDataStatus::Bool )
		{
			OutProperties->SetBoolField( item.Key, fobj.Bo );
			continue;
		}
		else if( fobj.GetStatus( ) == EDataStatus::FString )
		{
			OutProperties->SetStringField( item.Key, fobj.St );
			continue;
		}
	}
}


TMap< FString, FInfinarioData > UInfinario::GetHWPayload( )
{

#if PLATFORM_ANDROID

	FHardwareDataAndroid data;
	data.SetbDev( UE_BUILD_DEVELOPMENT );
	data.SetDeviceModel( FAndroidMisc::DeviceModel );
	data.SetDeviceType( FAndroidMisc::DeviceMake );   // FAndroidMisc::GetDeviceId( );   // ?
	data.SetOSName( TEXT( "Android" ) );
	data.SetOSVersion( FAndroidMisc::GetOSVersion( ) );
	data.SetGPUFamily( FAndroidMisc::GetGPUFamily( ) );
	data.SetGPUVersion( FAndroidMisc::GetGLVersion( ) );

#elif PLATFORM_IOS

	// #todo

#elif PLATFORM_WINDOWS

	FHardwareDataPC data;
	data.SetbDev( UE_BUILD_DEVELOPMENT );
	data.SetDeviceModel( FGenericPlatformMisc::GetDeviceMakeAndModel( ) );
	data.SetDeviceType( FGenericPlatformMisc::GetDeviceMakeAndModel( ) );
	data.SetOSName( TEXT( "Windows" ) );
	data.SetOSVersion( FGenericPlatformMisc::GetOSVersion( ) );
	data.SetGPUVersion( FGenericPlatformMisc::GetPrimaryGPUBrand( ) );
	data.SetGPUFamily( FGenericPlatformMisc::GetPrimaryGPUBrand( ) );
	data.SetCores( FGenericPlatformMisc::NumberOfCores( ) );
	data.SetCPU( FGenericPlatformMisc::GetCPUVendor( ) );

#elif PLATFORM_MAC

	// #todo

#endif

	return data.GetHWPayload( );
}
void UInfinario::CreateEveryCallPayload( const TMap< FString, FInfinarioData >& EveryCallPayloadToSet )
{
	UE_LOG( LogTemp, Warning, TEXT( "Address of to Set is %d" ), &EveryCallPayloadToSet );
	EveryCallPayload = EveryCallPayloadToSet;
	UE_LOG( LogTemp, Warning, TEXT( "Address of to every call property is is %d" ), &EveryCallPayload );
}

UInfinario::~UInfinario( )
{
	Instance = nullptr;
}
