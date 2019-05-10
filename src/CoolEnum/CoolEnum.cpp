// vim:set ts=4 sw=4 noexpandtab:

#include <stdexcept>
#include <string>

#ifdef COOLENUM
#undef COOLENUM_BEGIN
#undef COOLENUM
#undef COOLENUM_END
#endif

#define COOLENUM_BEGIN(NAME, KEYTYPE) \
	const std::string & NAME ## _ (const KEYTYPE p_key) { \
		NAME ## _fillenummaps(); \
		auto i = NAME ## _k2s.find(p_key); \
		if (i == NAME ## _k2s.end()) { \
			throw std::out_of_range("Requested enum key not found in " #NAME); \
		} \
		return i->second; \
	} \
	NAME NAME ## _ (const std::string & p_str) { \
		NAME ## _fillenummaps(); \
		auto i = NAME ## _s2k.find(p_str); \
		if (i == NAME ## _s2k.end()) { \
			throw std::out_of_range("Requested enum value not found in " #NAME); \
		} \
		return (NAME) i->second; \
	} \
	std::map<KEYTYPE, std::string> NAME ## _k2s; \
	std::map<std::string, KEYTYPE> NAME ## _s2k; \
	void NAME ## _fillenummaps() { \
		std::map<KEYTYPE, std::string> * k2s = & NAME ## _k2s; \
		std::map<std::string, KEYTYPE> * s2k = & NAME ## _s2k; \
		if (k2s->empty()) {

#define COOLENUM(VALUE, INDEX) \
	(*k2s)[INDEX] = #VALUE; \
	(*s2k)[#VALUE] = INDEX;

#define COOLENUM_END() \
	} \
	}
