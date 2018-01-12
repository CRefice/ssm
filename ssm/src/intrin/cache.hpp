#pragma once

namespace ssm
{
inline void sse_prefetch(void* item) {
	_mm_prefetch((const char*)item, _MM_HINT_NTA);
}
}
