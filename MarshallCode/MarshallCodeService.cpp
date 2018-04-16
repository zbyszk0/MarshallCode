#include "stdafx.h"
#include "MarshallCodeService.h"

std::mutex MarshallCodeService::mutex;
MarshallCodeService *MarshallCodeService::instance = nullptr;
Patch *MarshallCodeService::nullPatch;