/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkFont_DEFINED
#define SkFont_DEFINED

#include "SkFontTypes.h"
#include "SkScalar.h"
#include "SkTypeface.h"

class SkPaint;
class SkPath;
struct SkFontMetrics;

/** \class SkFont
    SkFont controls options applied when drawing and measuring text.
*/
class SK_API SkFont {
public:
    /** Whether edge pixels draw opaque or with partial transparency.
    */
    enum class Edging {
        kAlias,              //!< no transparent pixels on glyph edges
        kAntiAlias,          //!< may have transparent pixels on glyph edges
        kSubpixelAntiAlias,  //!< glyph positioned in pixel using transparency
    };

    /** Amount of font hinting applied to glyph outlines.
    */
    enum Hinting : uint8_t {
        kNo_Hinting     = 0, //!< glyph outlines unchanged
        kSlight_Hinting = 1, //!< minimal modification to improve constrast
        kNormal_Hinting = 2, //!< glyph outlines modified to improve constrast
        kFull_Hinting   = 3, //!< modifies glyph outlines for maximum constrast
    };

    /** Constructs SkFont with default values.

        @return  default initialized SkFont
    */
    SkFont();

    /** Constructs SkFont with default values with SkTypeface and size in points.

        @param typeface  font and style used to draw and measure text
        @param size      typographic height of text
        @return          initialized SkFont
    */
    SkFont(sk_sp<SkTypeface> typeface, SkScalar size);


    /** Constructs SkFont with default values with SkTypeface and size in points,
        horizontal scale, and horizontal skew. Horizontal scale emulates condensed
        and expanded fonts. Horizontal skew emulates oblique fonts.

        @param typeface  font and style used to draw and measure text
        @param size      typographic height of text
        @param scaleX    text horizontal scale
        @param skewX     additional shear on x-axis relative to y-axis
        @return          initialized SkFont
    */
    SkFont(sk_sp<SkTypeface> typeface, SkScalar size, SkScalar scaleX, SkScalar skewX);

    bool operator==(const SkFont&);

    /** If true, instructs the font manager to always hint glyphs.
        Returned value is only meaningful if platform uses FreeType as the font manager.

        @return  true if all glyphs are hinted
    */
    bool isForceAutoHinting() const { return SkToBool(fFlags & kForceAutoHinting_PrivFlag); }

    /** Returns true if font engine may return glyphs from font bitmaps instead of from outlines.

        @return  true if glyphs may be font bitmaps
    */
    bool isEmbeddedBitmaps() const { return SkToBool(fFlags & kEmbeddedBitmaps_PrivFlag); }

    /** Returns true if glyphs at different sub-pixel positions may differ on pixel edge coverage.

        @return  true if glyph positioned in pixel using transparency
    */
    bool isSubpixel() const { return SkToBool(fFlags & kSubpixel_PrivFlag); }

    /** Returns true if text is converted to SkPath before drawing and measuring.

        @return  true glyph hints are never applied
    */
    bool isLinearMetrics() const { return SkToBool(fFlags & kLinearMetrics_PrivFlag); }

    /** Returns true if bold is approximated by increasing the stroke width when creating glyph
        bitmaps from outlines.

        @return  bold is approximated through stroke width
    */
    bool isEmbolden() const { return SkToBool(fFlags & kEmbolden_PrivFlag); }

    /** Sets whether to always hint glyphs.
        If forceAutoHinting is set, instructs the font manager to always hint glyphs.

        Only affects platforms that use FreeType as the font manager.

        @param forceAutoHinting  setting to always hint glyphs
    */
    void setForceAutoHinting(bool forceAutoHinting);

    /** Requests, but does not require, to use bitmaps in fonts instead of outlines.

        @param embeddedBitmaps  setting to use bitmaps in fonts
    */
    void setEmbeddedBitmaps(bool embeddedBitmaps);

    /** Requests, but does not require, that glyphs respect sub-pixel positioning.

        @param subpixel  setting for sub-pixel positioning
    */
    void setSubpixel(bool subpixel);

    /** Requests, but does not require, that glyphs are converted to SkPath
        before drawing and measuring.

        @param linearMetrics  setting for converting glyphs to paths
    */
    void setLinearMetrics(bool linearMetrics);

    /** Increases stroke width when creating glyph bitmaps to approximate a bold typeface.

        @param embolden  setting for bold approximation
    */
    void setEmbolden(bool embolden);

    /** Whether edge pixels draw opaque or with partial transparency.

        @return  one of: Edging::kAlias, Edging::kAntiAlias, Edging::kSubpixelAntiAlias
    */
    Edging getEdging() const { return (Edging)fEdging; }

    /** Requests, but does not require, that edge pixels draw opaque or with
        partial transparency.

        @param edging  one of: Edging::kAlias, Edging::kAntiAlias, Edging::kSubpixelAntiAlias
    */
    void setEdging(Edging edging);

    /** Sets level of glyph outline adjustment.
        Does not check for valid values of hintingLevel.

        @param hintingLevel  one of: SkFontHinting::kNone, SkFontHinting::kSlight,
                                     SkFontHinting::kNormal, SkFontHinting::kFull
    */
    void setHinting(SkFontHinting hintingLevel);

#ifdef SK_SUPPORT_LEGACY_NESTED_HINTINGENUM
    /** Deprecated.
    */
    Hinting getHinting() const { return (Hinting)fHinting; }

    /** Deprecated.
    */
    void setHinting(Hinting hinting) {
        this->setHinting((SkFontHinting)hinting);
    }
#else
    /** Returns level of glyph outline adjustment.

        @return  one of: SkFontHinting::kNone, SkFontHinting::kSlight, SkFontHinting::kNormal,
                         SkFontHinting::kFull
     */
    SkFontHinting getHinting() const { return (SkFontHinting)fHinting; }
#endif

    /** Returns a font with the same attributes of this font, but with the specified size.
        If size is less than zero or non-finite, nullptr will be returned.

        @param size  typographic height of text
        @return      initialized SkFont
     */
    SkFont makeWithSize(SkScalar size) const;

    /** Returns SkTypeface if set, or nullptr.
        Does not alter SkTypeface SkRefCnt.

        @return  SkTypeface if previously set, nullptr otherwise
    */
    SkTypeface* getTypeface() const { return fTypeface.get(); }

    /** Returns text size in points.

        @return  typographic height of text
    */
    SkScalar    getSize() const { return fSize; }

    /** Returns text scale on x-axis.
        Default value is 1.

        @return  text horizontal scale
    */
    SkScalar    getScaleX() const { return fScaleX; }

    /** Returns text skew on x-axis.
        Default value is zero.

        @return  additional shear on x-axis relative to y-axis
    */
    SkScalar    getSkewX() const { return fSkewX; }

    /** Increases SkTypeface SkRefCnt by one.

        @return  SkTypeface if previously set, nullptr otherwise
    */
    sk_sp<SkTypeface> refTypeface() const { return fTypeface; }

    /** Sets SkTypeface to typeface, decreasing SkRefCnt of the previous SkTypeface.
        Pass nullptr to clear SkTypeface and use the default typeface. Increments
        tf SkRefCnt by one.

        @param tf  font and style used to draw text
    */
    void setTypeface(sk_sp<SkTypeface> tf) { fTypeface = tf; }

    /** Sets text size in points.
        Has no effect if textSize is not greater than or equal to zero.

        @param textSize  typographic height of text
    */
    void setSize(SkScalar textSize);

    /** Sets text scale on x-axis.
        Default value is 1.

        @param scaleX  text horizontal scale
    */
    void setScaleX(SkScalar scaleX);

    /** Sets text skew on x-axis.
        Default value is zero.

        @param skewX  additional shear on x-axis relative to y-axis
    */
    void setSkewX(SkScalar skewX);

    /** Converts text into glyph indices.
        Returns the number of glyph indices represented by text.
        SkTextEncoding specifies how text represents characters or glyphs.
        glyphs may be nullptr, to compute the glyph count.

        Does not check text for valid character codes or valid glyph indices.

        If byteLength equals zero, returns zero.
        If byteLength includes a partial character, the partial character is ignored.

        If encoding is kUTF8_TextEncoding and text contains an invalid UTF-8 sequence,
        zero is returned.

        If maxGlyphCount is not sufficient to store all the glyphs, no glyphs are copied.
        The total glyph count is returned for subsequent buffer reallocation.

        @param text          character storage encoded with SkPaint::TextEncoding
        @param byteLength    length of character storage in bytes
        @param encoding      one of: kUTF8_SkTextEncoding, kUTF16_SkTextEncoding,
                             kUTF32_SkTextEncoding, kGlyphID_SkTextEncoding
        @param glyphs        storage for glyph indices; may be nullptr
        @param maxGlyphCount storage capacity
        @return              number of glyphs represented by text of length byteLength
    */
    int textToGlyphs(const void* text, size_t byteLength, SkTextEncoding encoding,
                     SkGlyphID glyphs[], int maxGlyphCount) const;

    /** Returns glyph index for Unicode character.

        @param uni  Unicode character
        @return     glyph index
    */
    uint16_t unicharToGlyph(SkUnichar uni) const {
        return fTypeface->unicharToGlyph(uni);
    }

    /** Returns number of glyphs represented by text.

        @param text          character storage encoded with SkPaint::TextEncoding
        @param byteLength    length of character storage in bytes
        @param encoding      one of: kUTF8_SkTextEncoding, kUTF16_SkTextEncoding,
                             kUTF32_SkTextEncoding, kGlyphID_SkTextEncoding
        @return              number of glyphs represented by text of length byteLength
    */
    int countText(const void* text, size_t byteLength, SkTextEncoding encoding) const {
        return this->textToGlyphs(text, byteLength, encoding, nullptr, 0);
    }

    /** Returns the advance width of text.
        The advance is the normal distance to move before drawing additional text.
        Returns the bounding box of text if bounds is not nullptr.

        @param text        character storage encoded with SkPaint::TextEncoding
        @param byteLength  length of character storage in bytes
        @param encoding    one of: kUTF8_SkTextEncoding, kUTF16_SkTextEncoding,
                           kUTF32_SkTextEncoding, kGlyphID_SkTextEncoding
        @param bounds      returns bounding box relative to (0, 0) if not nullptr
        @return            number of glyphs represented by text of length byteLength
    */
    SkScalar measureText(const void* text, size_t byteLength, SkTextEncoding encoding,
                         SkRect* bounds = nullptr) const;

    /** Retrieves the advance and bounds for each glyph in glyphs.
        Both widths and bounds may be nullptr.
        If widths is not nullptr, widths must be an array of count entries.
        if bounds is not nullptr, bounds must be an array of count entries.

        @param glyphs      array of glyph indices to be measured
        @param count       number of glyphs
        @param widths      returns text advances for each glyph; may be nullptr
        @param bounds      returns bounds for each glyph relative to (0, 0); may be nullptr
    */
    void getWidths(const uint16_t glyphs[], int count, SkScalar widths[],
                   SkRect bounds[] = nullptr) const;

    /** Returns path corresponding to glyph outline.
        If glyph has an outline, copies outline to path and returns true.
        path returned may be empty.
        If glyph is described by a bitmap, returns false and ignores path parameter.

        @param glyphID  index of glyph
        @param path     pointer to existing SkPath
        @return         true if glyphID is described by path
     */
    bool getPath(uint16_t glyphID, SkPath* path) const;


    /** Returns path corresponding to glyph array.

        @param glyphIDs      array of glyph indices
        @param count         number of glyphs
        @param glyphPathProc function returning one glyph description as path
        @param ctx           function context
   */
    void getPaths(const uint16_t glyphIDs[], int count,
                  void (*glyphPathProc)(uint16_t glyphID, const SkPath* pathOrNull, void* ctx),
                  void* ctx) const;

    /** Returns SkFontMetrics associated with SkTypeface.
        The return value is the recommended spacing between lines: the sum of metrics
        descent, ascent, and leading.
        If metrics is not nullptr, SkFontMetrics is copied to metrics.
        Results are scaled by text size but does not take into account
        dimensions required by text scale x, text skew x, fake bold,
        style stroke, and SkPathEffect.

        @param metrics  storage for SkFontMetrics; may be nullptr
        @return         recommended spacing between lines
    */
    SkScalar getMetrics(SkFontMetrics* metrics) const;

    /** Returns the recommended spacing between lines: the sum of metrics
        descent, ascent, and leading.
        Result is scaled by text size but does not take into account
        dimensions required by stroking and SkPathEffect.
        Returns the same result as getMetrics().

        @return  recommended spacing between lines
    */
    SkScalar getSpacing() const { return this->getMetrics(nullptr); }

    /** Deprecated.
    */
    void LEGACY_applyToPaint(SkPaint* paint) const;
    /** Deprecated.
     */
    void LEGACY_applyPaintFlags(uint32_t paintFlags);
    /** Deprecated.
    */
    static SkFont LEGACY_ExtractFromPaint(const SkPaint& paint);

private:
    enum PrivFlags {
        kForceAutoHinting_PrivFlag      = 1 << 0,
        kEmbeddedBitmaps_PrivFlag       = 1 << 1,
        kSubpixel_PrivFlag              = 1 << 2,
        kLinearMetrics_PrivFlag         = 1 << 3,
        kEmbolden_PrivFlag              = 1 << 4,
    };

    static constexpr unsigned kAllFlags = 0x07F;

    sk_sp<SkTypeface> fTypeface;
    SkScalar    fSize;
    SkScalar    fScaleX;
    SkScalar    fSkewX;
    uint8_t     fFlags;
    uint8_t     fEdging;
    uint8_t     fHinting;

    SkScalar setupForAsPaths(SkPaint*);

    friend class SkCanonicalizeFont;
};

#endif
