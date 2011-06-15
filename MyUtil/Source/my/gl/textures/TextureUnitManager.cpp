#include "stdafx.h"

#include "PPointerUtilities_inl.h"

namespace my { namespace gl { namespace textures {

	namespace TextureUnitManager {
		using TextureUnitUtil::TextureUnitId;
		using codeshare::pointer_utilities::reinterpret_assign;
		
		struct TextureUnitEntry {
			TextureUnit									unit;
			bool										initialised;
			size_t										index;
		};

		static P_INLINE void ConstructEntry (TextureUnitEntry& entry, TextureUnit const& textureUnit) {
			PASSERT(!entry.initialised)

			new(&entry.unit) TextureUnit(textureUnit);
			entry.initialised = true;
		}

		static P_INLINE void DestructEntry (TextureUnitEntry& entry) {
			PASSERT(entry.initialised)

			entry.unit.~TextureUnit();
			entry.initialised = false;
		}

		static size_t const	_entriesLength(GL__TEXUTRE_UNIT_MANAGER__MAX_TEXTURE_UNITS * sizeof(TextureUnitEntry));
		static size_t const	entriesLength(GL__TEXUTRE_UNIT_MANAGER__MAX_TEXTURE_UNITS);
		static char			_entries[_entriesLength];
		static bool			initialised(false);

		P_GLOBAL_STATIC_ASSERT(_entriesLength/sizeof(TextureUnitEntry) == entriesLength)
		
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
					return -0xfffffff;
			}
			P_STATIC_ASSERT(30u < GL__TEXUTRE_UNIT_MANAGER__MAX_TEXTURE_UNITS)
		}
		

		void Initialise (void) {
			PASSERT(!initialised)
			memset(&_entries[0], 0xaa, sizeof(_entries));
			

			{
				TextureUnitEntry* entries(NULL);
				reinterpret_assign(entries, &_entries[0]);
			
				for (TextureUnitEntry* entry(&entries[0]); entry < &entries[entriesLength]; ++entry) {
					entry->initialised = false;
					entry->index = entry - &entries[0];
				}
			}


			initialised = true;
		}

		void CleanUp (void) {
			PASSERT(initialised)


			{
				TextureUnitEntry* entries(NULL);
				reinterpret_assign(entries, &_entries[0]);

				for (TextureUnitEntry* entry(&entries[0]); entry < &entries[entriesLength]; ++entry)
					if (entry->initialised) {
						PASSERT(entry->index == entry - &entries[0])
						DestructEntry(*entry);
					}
			}


			memset(&_entries[0], 0xaa, sizeof(_entries));

			initialised = false;
		}

		TextureUnit& GetUnit (TextureUnitId const& textureUnitId) {
			size_t const unitIndex(TranslateIndex(textureUnitId));
			PASSERT(unitIndex < entriesLength)

			TextureUnitEntry* entries(NULL);
			reinterpret_assign(entries, &_entries[0]);

			TextureUnitEntry& entry(entries[unitIndex]);
			PASSERT(entry.index == &entry - &entries[0])

			if (!entry.initialised)
				ConstructEntry(entry, TextureUnit(textureUnitId));

			return entry.unit;
		}

		TextureUnit& GetActiveUnit (void) {
			TextureUnitEntry* entries(NULL);
			reinterpret_assign(entries, &_entries[0]);

			return GetUnit(TextureUnitUtil::GetActive());
		}

	}

}}} // namespace my::gl::textures