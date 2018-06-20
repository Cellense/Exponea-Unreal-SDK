// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#pragma once

#include <UnrealString.h>
#include "InfinarioDefaults.h"

struct FHardwareDataBase
{
	template< typename T >
	void AddNew( FString _Key, T _Val )
	{
		DataMap.Add( _Key, FInfinarioData( _Val ) );
	}

private:
	TMap< FString, FInfinarioData > DataMap;

	bool	bDev		= false;   // True if tracked from non shipping build
	FString SDK			= PLUGIN_SDK;
	FString SDKVersion  = PLUGIN_VERSION;
	FString DeviceModel = UNDEFINED;
	FString DeviceType  = UNDEFINED;
	FString OSName		= UNDEFINED;
	FString OSVersion   = UNDEFINED;
	FString GPUFamily   = UNDEFINED;
	FString GPUVersion  = UNDEFINED;

public:
	FHardwareDataBase( )
	{
	}

	TMap< FString, FInfinarioData >& GetHWPayload( )
	{
		return DataMap;
	}

	void SetbDev( bool Value )
	{
		bDev = Value;
		AddNew( PROPERTY_HW_DEV, Value );
	}

	void SetSDK( FString Value )
	{
		SDK = Value;
		AddNew( PROPERTY_HW_SDK, Value );
	}

	void SetSDKVersion( FString Value )
	{
		SDKVersion = Value;
		AddNew( PROPERTY_HW_SDK_VERSION, Value );
	}

	void SetDeviceModel( FString Value )
	{
		DeviceModel = Value;
		AddNew( PROPERTY_HW_DEVICE_MODEL, Value );
	}

	void SetDeviceType( FString Value )
	{
		OSName = Value;
		AddNew( PROPERTY_HW_DEVICE_TYPE, Value );
	}

	void SetOSName( FString Value )
	{
		OSName = Value;
		AddNew( PROPERTY_HW_OS_NAME, Value );
	}

	void SetOSVersion( FString Value )
	{
		OSVersion = Value;
		AddNew( PROPERTY_HW_OS_VERSION, Value );
	}

	void SetGPUFamily( FString Value )
	{
		GPUFamily = Value;
		AddNew( PROPERTY_HW_GPU_FAMILY, Value );
	}

	void SetGPUVersion( FString Value )
	{
		GPUVersion = Value;
		AddNew( PROPERTY_HW_GPU_VERSION, Value );
	}
};

struct FHardwareDataAndroid : public FHardwareDataBase
{
	FHardwareDataAndroid( )
	{
	}
};

struct FHardwareDataiOS : public FHardwareDataBase
{
	FHardwareDataiOS( )
	{
	}
};

struct FHardwareDataPC : public FHardwareDataBase
{
private:
	FString CPU   = UNDEFINED;
	int32   Cores = -1;

public:
	FHardwareDataPC( )
	{
	}

	void SetCPU( FString Value )
	{
		CPU = Value;
		AddNew( PROPERTY_HW_CPU, Value );
	}

	void SetCores( int32 Value )
	{
		Cores = Value;
		AddNew( PROPERTY_HW_CORES, Value );
	}
};