// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#pragma once

#include "Private/InfinarioDefaults.h"
#include "GameFramework/SaveGame.h"
#include "InfinarioDataTypes.generated.h"

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
