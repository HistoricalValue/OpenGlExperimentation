#ifndef __MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__H__
#define __MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__H__

#include "my/gl/textures/TextureUnitManager_fwd.h"

#include "PConfigurations.h"					// P_INLINE

#include "my/gl/textures/TextureUnitWrapper.h"	// TextureUnitId
#include "my/gl/textures/TextureUnit.h"			// TextureUnit

#include "PPlaceholder.h"
#include "PTypes.h"								// ArrayOf

#define GL__TEXUTRE_UNIT_MANAGER__MAX_TEXTURE_UNITS		31u

namespace my { namespace gl { namespace textures {

	using TextureUnitWrapper::TextureUnitId;
	class MYUTIL_API TextureUnitManager {
	public:
		P_INLINE										TextureUnitManager (void);
		P_INLINE										~TextureUnitManager (void);

		P_INLINE bool									IsValidId (TextureUnitId const&) const;

		P_INLINE TextureUnit*							GetUnit (TextureUnitId const&);
		P_INLINE TextureUnit*							GetActiveUnit (void);
		
	private:
		struct TextureUnitEntry {
			TextureUnit									unit;
			bool										initialised;
			size_t										index;
		};
		typedef codeshare::utilities::Placeholder<TextureUnitEntry>
														TextureUnitEntryPlaceholder;
		typedef codeshare::utilities::types::ArrayOf<TextureUnitEntryPlaceholder, GL__TEXUTRE_UNIT_MANAGER__MAX_TEXTURE_UNITS>::Array
														TextureUnitEntryPlaceholdersArray;
		typedef TextureUnitEntryPlaceholdersArray		Entries;
		
#pragma warning( push )
#pragma warning( disable: 4251 ) // Placeholder<> (for "entries") needs to have dll-linkage for clients of TextureUnitManager 
		Entries											entries;
		
		P_INLINE void									BlankOutEntries (void);
		P_INLINE void									ConstructEntry (size_t, TextureUnitId const&);
		P_INLINE void									DestructEntry (size_t);
		P_INLINE TextureUnitEntry&						GetEntry (size_t);
		P_INLINE bool									IsValidEntryIndex (size_t) const;
	}; // class TextureUnitManager

}}} // namespace my::gl::textures

#endif // __MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__H__
