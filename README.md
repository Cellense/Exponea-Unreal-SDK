# Getting started with Infinario Unreal Engine SDK (in 5 minutes)

 Infinario UE4 SDK is available in this Git repository :https://github.com/Cellense/Exponea-Unreal-SDK. It provides tracking capabilities for your application. All functionality is expoed for C++ and Blueprints.
 
 > Note: The plugin is not compiled at the moment and it's not possible to use it within your pure Blueprint project. Compiled version is coming soon.


## Installation

* Close UE4 editor
* Download or clone this repository from your command line: git clone `https://github.com/Cellense/Exponea-Unreal-SDK.git`
* Copy downloaded **InfinarioSDK** folder into **[*YourProject*]/Plugins/**.
* Open UE4 editor and make sure the plugin is installed. In UE4 editor open **Settings/Plugins** and see if the plugin has been enabled.

### Engine compatibility and supported platforms

**Unreal Enigne** 
- 4.19.x

**Platforms** 
* Windows (32bit, 64bit)
* Mac
* Linux
* iOS
* Android
* HTML5


## Usage

### Plugin initialization

```
// GameMode.h

UPROPERTY( BlueprintReadOnly, Category = "Infinario" )
UInfinario* Inf;
    
UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Infinario" )
FString Token = "e79ea4ac-5cdc-11e8-aaaa-b083fed3a8cb";
```

```
// GameMode.cpp

void AMyGameModeBase::BeginPlay( )
{
	Super::BeginPlay( );
    
    Inf = UInfinario::GetInstance( this );

	if( Inf == nullptr )
    {
		return;
	}

	/** Initialize with given project token */
    Inf->Initialize( Token );
}
```

### Identifying Players

```
/** Initialize with a given token */
Inf->Identify( FString( TEXT( "C++ Player" ) ) );
```

### Infinario Data

```
/** Create sample payload */
TMap< FString, FInfinarioData > TestData;
TestData.Add( "Integer Key", FInfinarioData( 3 ) );
TestData.Add( "String Key", FInfinarioData( FString( TEXT( "String value." ) ) ) );
TestData.Add( "Float Key", FInfinarioData( 1.2f ) );
TestData.Add( "Bool Key", FInfinarioData( true ) );
```       
  
### Every Call Data
```
/** Create data attached to every call. This part is optional.*/
TMap< FString, FInfinarioData > EveryCallData;
EveryCallData.Add( "every_call_data_int", FInfinarioData( 1 ) );
Inf->CreateEveryCallPayload( EveryCallData );
```

### Basic Tracking

To start tracking, you need to know your projectToken. To initialize the tracking, simply get an instance of the Infinario class and call just once Initialize:
```
Inf->Track( "my_event", TestData );
```

### Tracking Session Start and Session End Events

```
Inf->TrackSessionStart( TestData );
Inf->TrackSessionEnd( TestData );

```

### Tracking Virtual Payment
```
/**
 * Track with default virtual payment data structure.
 * Note: Use standard Track scenario in case you need to track additional data to the currency payload.
 */
FVirtualPayment VirtualPaymentData = FVirtualPayment( FInfinarioData( TEXT( "Gold" ) ), FInfinarioData( 10.0f ), FInfinarioData( TEXT( "Axe" ) ), FInfinarioData( TEXT( "Weapon" ) ) );
Inf->TrackVirtualPayment( VirtualPaymentData );
```


### C++

First of all, you need to modify `[YourProject].Build.cs` file. Add `InfinarioSDK` to `PrivateDependencyModuleNames`


### Blueprints


## Sample project

Please refer to sample project if something is not clear enough.



## Final Remarks

* It's suggested you make at most one instance of `InfinarioSDK` object during your application lifetime. The good place for instance is the GameMode.
