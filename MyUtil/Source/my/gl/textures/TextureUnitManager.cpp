#include "stdafx.h"

#include "PPointerUtilities_inl.h"


#define FOR_EACH_ENTRY															\
			for (																\
					TextureUnitEntryPlaceholder* entry_plc(&entries[0]);		\
					entry_plc < &entries[sizeof(entries)/sizeof(entries[0])];	\
					++entry_plc													\
			)																	\
				for (bool done(false); !done; )									\
					for (														\
							TextureUnitEntry* entry(entry_plc->GetInternal());	\
							!done;												\
							done = true											\
					)															\

namespace my { namespace gl { namespace textures {

	using TextureUnitWrapper::TextureUnitId;
	using codeshare::utilities::pointer_utilities::reinterpret_assign;

	static P_INLINE size_t TranslateIndex (TextureUnitId const& textureUnitId) {
		switch (textureUnitId) {
			case GL_TEXTURE0: return 0u;
			case GL_TEXTURE1: return 1u;
			case GL_TEXTURE2: return 2u;
			case GL_TEXTURE3: return 3u;
			case GL_TEXTURE4: return 4u;
			case GL_TEXTURE5: return 5u;
			case GL_TEXTURE6: return 6u;
			case GL_TEXTURE7: return 7u;
			case GL_TEXTURE8: return 8u;
			case GL_TEXTURE9: return 9u;
			case GL_TEXTURE10: return 10u;
			case GL_TEXTURE11: return 11u;
			case GL_TEXTURE12: return 12u;
			case GL_TEXTURE13: return 13u;
			case GL_TEXTURE14: return 14u;
			case GL_TEXTURE15: return 15u;
			case GL_TEXTURE16: return 16u;
			case GL_TEXTURE17: return 17u;
			case GL_TEXTURE18: return 18u;
			case GL_TEXTURE19: return 19u;
			case GL_TEXTURE20: return 20u;
			case GL_TEXTURE21: return 21u;
			case GL_TEXTURE22: return 22u;
			case GL_TEXTURE23: return 23u;
			case GL_TEXTURE24: return 24u;
			case GL_TEXTURE25: return 25u;
			case GL_TEXTURE26: return 26u;
			case GL_TEXTURE27: return 27u;
			case GL_TEXTURE28: return 28u;
			case GL_TEXTURE29: return 29u;
			case GL_TEXTURE30: return 30u;
			default:
				PASSERT(false)
				return 0xfffffff;
		}
		P_STATIC_ASSERT(30u < GL__TEXUTRE_UNIT_MANAGER__MAX_TEXTURE_UNITS)
	}
	
	P_INLINE
	bool TextureUnitManager::IsValidEntryIndex (size_t const i) const {
		return i < sizeof(entries)/sizeof(entries[0]);
	}
	
	P_INLINE
	bool TextureUnitManager::IsValidId (TextureUnitId const& textureUnitId) const {
		return IsValidEntryIndex(TranslateIndex(textureUnitId));
	}
	
	P_INLINE
	TextureUnitManager::TextureUnitEntry& TextureUnitManager::GetEntry (size_t const entryIndex) {
		PASSERT(IsValidEntryIndex(entryIndex))

		TextureUnitEntryPlaceholder& entry_plc(entries[entryIndex]);
		TextureUnitEntry& entry(*entry_plc.GetInternal());
		PASSERT(entry.index == &entry_plc - &entries[0])
		
		return entry;
	}
	
	P_INLINE
	void TextureUnitManager::ConstructEntry (size_t const entryIndex, TextureUnitId const& textureUnitId) {
		TextureUnitEntry& entry(GetEntry(entryIndex));
		PASSERT(!entry.initialised)

		new(&entry.unit) TextureUnit(textureUnitId);
		entry.initialised = true;
	}

	P_INLINE
	void TextureUnitManager::DestructEntry (size_t const entryIndex) {
		TextureUnitEntry& entry(GetEntry(entryIndex));
		PASSERT(entry.initialised)

		entry.unit.~TextureUnit();
		entry.initialised = false;
	}

	P_INLINE
	void TextureUnitManager::BlankOutEntries (void) {
		memset(&entries[0], 0xaa, sizeof(entries));
	}

	TextureUnitManager::TextureUnitManager (void):
#pragma warning( push )
#pragma warning( disable: 4351 ) // elements of "entries" will be default initialised
		entries() {
#pragma warning( pop )
		
		BlankOutEntries();
		
		FOR_EACH_ENTRY {
			entry->initialised = false;
			entry->index = entry_plc - &entries[0];
		}
	}

	TextureUnitManager::~TextureUnitManager (void) {
		FOR_EACH_ENTRY
			if (entry->initialised) {
				PASSERT(entry->index == entry_plc - &entries[0])
				DestructEntry(entry->index);
			}

		BlankOutEntries();
	}

	TextureUnit* TextureUnitManager::GetUnit (TextureUnitId const& textureUnitId) {
		TextureUnitEntry& entry(GetEntry(TranslateIndex(textureUnitId)));

		if (!entry.initialised)
			ConstructEntry(entry.index, textureUnitId);

		return &entry.unit;
	}

	TextureUnit* TextureUnitManager::GetActiveUnit (void) {
		return GetUnit(TextureUnitWrapper::GetActive());
	}

}}} // namespace my::gl::textures
