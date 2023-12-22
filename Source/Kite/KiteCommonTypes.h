﻿#pragma once

// #include "KiteCommonTypes.generated.h"

// ----------------------------------------------------------------------------------------------------------------
// This header is for enums and structs used by classes and blueprints accross the game
// Collecting these in a single header helps avoid problems with recursive header includes
// It's also a good place to put things like data table row structs
// ----------------------------------------------------------------------------------------------------------------

KITE_API DECLARE_LOG_CATEGORY_EXTERN(KiteLog, Display, All);

// 简单的调试输出宏
#define kprint(Format, ...) UE_LOG(KiteLog, Display, TEXT(Format), ##__VA_ARGS__)
#define kwarn(Format, ...) UE_LOG(KiteLog, Warning, TEXT(Format), ##__VA_ARGS__)
#define kerror(Format, ...) UE_LOG(KiteLog, Error, TEXT(Format), ##__VA_ARGS__)
