// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#pragma once

#include <UnrealString.h>

/**
 * SDK
 */
const FString PLUGIN_SDK	 = "Unreal Engine SDK";
const FString PLUGIN_VERSION = "0.0.1";

/**
 * Tracking ids, events and properties
 */
const FString ID_REGISTERED = "registered";
const FString ID_COOKIE		= "cookie";

const FString EVENT_SESSION_START   = "session_start";
const FString EVENT_SESSION_END		= "session_end";
const FString EVENT_IDENTIFICATION  = "identification";
const FString EVENT_VIRTUAL_PAYMENT = "virtual_payment";

const FString PROPERTY_APP_VERSION				= "app_version";
const FString PROPERTY_DURATION					= "duration";
const FString PROPERTY_REGISTRATION_ID			= "registration_id";
const FString PROPERTY_CURRENCY					= "currency";
const FString PROPERTY_AMOUNT					= "amount";
const FString PROPERTY_TIMESTAMP				= "timestamp";
const FString PROPERTY_ITEM_NAME				= "item_name";
const FString PROPERTY_ITEM_TYPE				= "item_type";
const FString PROPERTY_SESSION_START_TIMESTAMP  = "session_start_timestamp";
const FString PROPERTY_SESSION_START_PROPERTIES = "session_start_properties";
const FString PROPERTY_SESSION_END_TIMESTAMP	= "session_end_timestamp";
const FString PROPERTY_SESSION_END_PROPERTIES   = "session_end_properties";
const FString PROPERTY_HW_DEV					= "dev";
const FString PROPERTY_HW_SDK					= "sdk";
const FString PROPERTY_HW_SDK_VERSION			= "sdk_version";
const FString PROPERTY_HW_DEVICE_MODEL			= "device_model";
const FString PROPERTY_HW_DEVICE_TYPE			= "device_type";
const FString PROPERTY_HW_OS_NAME				= "os_name";
const FString PROPERTY_HW_OS_VERSION			= "os_version";
const FString PROPERTY_HW_GPU_FAMILY			= "gpu_family";
const FString PROPERTY_HW_GPU_VERSION			= "gpu_version";
const FString PROPERTY_HW_CPU					= "cpu";
const FString PROPERTY_HW_RAM					= "ram";
const FString PROPERTY_HW_CORES					= "cores";


const FString DEFAULT_PLAYER_IDENTITY = "Default Player";
const FString UNDEFINED				  = "Undefined";


/**
 * Sending
 */
const int32 BULK_LIMIT			   = 49;
const int32 BULK_TIMEOUT_MS		   = 10000;
const int32 BULK_INTERVAL_MS	   = 1000;
const int32 BULK_MAX_RETRIES	   = 20;
const int32 BULK_MAX_RETRY_WAIT_MS = 60 * 1000;
const float BULK_MAX			   = 60 * 20;

const double SESSION_TIMEOUT = 60;

const FString DEFAULT_TARGET = TEXT( "api.infinario.com" );
const FString BULK_URL		 = TEXT( "/bulk" );

const FString ENDPOINT_UPDATE = "crm/customers";
const FString ENDPOINT_TRACK  = "crm/events";
