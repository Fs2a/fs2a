// vim:set ts=4 sw=4 noexpandtab:

#include <cstdint>
#include <map>
#include <string>

#ifdef COOLENUM
#undef COOLENUM_BEGIN
#undef COOLENUM
#undef COOLENUM_END
#endif

#define COOLENUM_BEGIN(NAME, KEYTYPE) \
	enum NAME : KEYTYPE; \
	extern std::map<KEYTYPE, std::string> NAME ## _k2s; \
	extern std::map<std::string, KEYTYPE> NAME ## _s2k; \
	const std::string & NAME ## _ (const KEYTYPE p_key); \
	NAME NAME ## _ (const std::string & p_str); \
	void NAME ## _fillenummaps(); \
	enum NAME : KEYTYPE {
#define COOLENUM(VALUE, INDEX) VALUE = INDEX,
#define COOLENUM_END() };
