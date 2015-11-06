/*
 * gstvp9parser.h
 *
 *  Copyright (C) 2013-2014 Intel Corporation
 *  Copyright (C) 2015 Intel Corporation
 *    Author: XuGuangxin<Guangxin.Xu@intel.com>
 *    Author: Sreerenj Balachandran<sreerenj.balachandran@intel.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef GST_VP9_PARSER_H
#define GST_VP9_PARSER_H

#ifndef GST_USE_UNSTABLE_API
#warning "The VP9 parsing library is unstable API and may change in future."
#warning "You can define GST_USE_UNSTABLE_API to avoid this warning."
#endif

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_VP9_FRAME_MARKER 0x02
#define GST_VP9_SYNC_CODE 0x498342

#define GST_VP9_MAX_LOOP_FILTER    63
#define GST_VP9_MAX_PROB           255

#define GST_VP9_REFS_PER_FRAME     3
#define GST_VP9_REF_FRAMES_LOG2    3
#define GST_VP9_REF_FRAMES         (1 << GST_VP9_REF_FRAMES_LOG2)

#define GST_FRAME_CONTEXTS_LOG2    2

#define GST_VP9_MAX_LOOP_FILTER    63
#define GST_VP9_MAX_SHARPNESS      7

#define GST_VP9_MAX_REF_LF_DELTAS  4
#define GST_VP9_MAX_MODE_LF_DELTAS 2

#define GST_VP9_SEGMENT_DELTADATA  0
#define GST_VP9_SEGMENT_ABSDATA    1

#define GST_VP9_MAX_SEGMENTS       8
#define GST_VP9_SEG_TREE_PROBS     (GST_VP9_MAX_SEGMENTS-1)

#define GST_VP9_PREDICTION_PROBS   3

typedef struct _GstVp9Parser               GstVp9Parser;
typedef struct _GstVp9FrameHdr             GstVp9FrameHdr;
typedef struct _GstVp9LoopFilter           GstVp9LoopFilter;
typedef struct _GstVp9QuantIndices         GstVp9QuantIndices;
typedef struct _GstVp9Segmentation         GstVp9Segmentation;
typedef struct _GstVp9SegmentationInfo     GstVp9SegmentationInfo;
typedef struct _GstVp9SegmentationInfoData GstVp9SegmentationInfoData;

/**
 * GstVp9ParseResult:
 * @GST_VP9_PARSER_OK: The parsing went well
 * @GST_VP9_PARSER_BROKEN_DATA: The data to parse is broken
 * @GST_VP9_PARSER_NO_PACKET_ERROR: An error accured durint the parsing
 *
 * Result type of any parsing function.
 */
typedef enum
{
  GST_VP9_PARSER_OK,
  GST_VP9_PARSER_BROKEN_DATA,
  GST_VP9_PARSER_ERROR,
} GstVp9ParserResult;

/**
 * GstVp9Profile: Bitstream profiles indicated by 2-3 bits in the uncompressed header
 * @GST_VP9_PROFILE_0: Profile 0, 8-bit 4:2:0 only.
 * @GST_VP9_PROFILE_1: Profile 1, 8-bit 4:4:4, 4:2:2, and 4:4:0.
 * @GST_VP9_PROFILE_2: Profile 2, 10-bit and 12-bit color only, with 4:2:0 sampling.
 * @GST_VP9_PROFILE_3: Profile 3, 10-bit and 12-bit color only, with 4:2:2/4:4:4/4:4:0 sampling.
 * @GST_VP9_PROFILE_UNDEFINED: Undefined profile
 *
 * VP9 Profiles
 */
typedef enum {
  GST_VP9_PROFILE_0,
  GST_VP9_PROFILE_1,
  GST_VP9_PROFILE_2,
  GST_VP9_PROFILE_3,
  GST_VP9_PROFILE_UNDEFINED
} GstVP9Profile;

typedef enum {
  GST_VP9_KEY_FRAME   = 0,
  GST_VP9_INTER_FRAME = 1
} GstVp9FrameType;

typedef enum {
  GST_VP9_BIT_DEPTH_8  = 8,
  GST_VP9_BIT_DEPTH_10 = 10,
  GST_VP9_BIT_DEPTH_12 = 12
} GstVp9BitDepth;

typedef enum {
  GST_VP9_CS_UNKNOWN               = 0,
  GST_VP9_CS_BT_601                = 1,
  GST_VP9_CS_BT_709                = 2,
  GST_VP9_CS_SMPTE_170             = 3,
  GST_VP9_CS_SMPTE_240             = 4,
  GST_VP9_CS_RESERVED_1            = 5,
  GST_VP9_CS_RESERVED_2            = 6,
  GST_VP9_CS_SRGB                  = 7
} GstVp9ColorSpace;

/**
 * GstVp9ColorRange:
 * @GST_VP9_CR_LIMITED: Y range is [16-235], UV range is [16-240]
 * @GST_VP9_CR_FULL: Full range for Y,U and V [0-255]
 *
 * Possible color value ranges
 */
typedef enum {
  GST_VP9_CR_LIMITED,
  GST_VP9_CR_FULL
} GstVp9ColorRange;

typedef enum {
  GST_VP9_INTERP_FILTER_EIGHTTAP        = 0,
  GST_VP9_INTERP_FILTER_EIGHTTAP_SMOOTH = 1,
  GST_VP9_INTERP_FILTER_EIGHTTAP_SHARP  = 2,
  GST_VP9_INTERP_FILTER_BILINEAR        = 3,
  GST_VP9_INTERP_FILTER_SWITCHABLE      = 4
} GstVp9InterpFilter;

typedef enum {
  GST_VP9_REF_FRAME_INTRA  = 0,
  GST_VP9_REF_FRAME_LAST   = 1,
  GST_VP9_REF_FRAME_GOLDEN = 2,
  GST_VP9_REF_FRAME_ALTREF = 3,
  GST_VP9_REF_FRAME_MAX    = 4
} GstVp9RefFrameType;

/**
 * GstVp9QuantIndices:
 * @y_ac_qi: indicates the dequantization table index used for the
 *   luma AC coefficients
 * @y_dc_delta: indicates the delta value that is added to the
 *   baseline index to obtain the luma DC coefficient dequantization
 *   index
 * @uv_dc_delta: indicates the delta value that is added to the
 *   baseline index to obtain the chroma DC coefficient dequantization
 *   index
 * @uv_ac_delta: indicates the delta value that is added to the
 *   baseline index to obtain the chroma AC coefficient dequantization
 *   index
 *
 * Dequantization indices.
 */
struct _GstVp9QuantIndices
{
  guint8 y_ac_qi;
  gint8 y_dc_delta;
  gint8 uv_dc_delta;
  gint8 uv_ac_delta;
};

struct _GstVp9LoopFilter {
  gboolean filter_level;
  gboolean sharpness_level;

  gboolean mode_ref_delta_enabled;
  gboolean mode_ref_delta_update;
  gboolean update_ref_deltas[GST_VP9_MAX_REF_LF_DELTAS];
  gint8 ref_deltas[GST_VP9_MAX_REF_LF_DELTAS];
  gboolean update_mode_deltas[GST_VP9_MAX_MODE_LF_DELTAS];
  gint8 mode_deltas[GST_VP9_MAX_MODE_LF_DELTAS];
};

struct _GstVp9SegmentationInfoData {
  /* SEG_LVL_ALT_Q */
  gboolean alternate_quantizer_enabled;
  gint16 alternate_quantizer;

  /* SEG_LVL_ALT_LF */
  gboolean alternate_loop_filter_enabled;
  gint8 alternate_loop_filter;

  /* SEG_LVL_REF_FRAME */
  gboolean reference_frame_enabled;
  gboolean reference_frame;

  gboolean reference_skip;
};

struct _GstVp9SegmentationInfo {
  /* enable in setup_segmentation*/
  gboolean  enabled;
  /* update_map in setup_segmentation*/
  gboolean update_map;
  /* tree_probs exist or not*/
  gboolean update_tree_probs[GST_VP9_SEG_TREE_PROBS];
  gboolean tree_probs[GST_VP9_SEG_TREE_PROBS];
  /* pred_probs exist or not*/
  gboolean update_pred_probs[GST_VP9_PREDICTION_PROBS];
  gboolean pred_probs[GST_VP9_PREDICTION_PROBS];

  /* abs_delta in setup_segmentation */
  gboolean abs_delta;
  /* temporal_update in setup_segmentation */
  gboolean temporal_update;

  /* update_data in setup_segmentation*/
  gboolean update_data;
  GstVp9SegmentationInfoData data[GST_VP9_MAX_SEGMENTS];
};

struct _GstVp9FrameHdr
{
  guint profile;
  gboolean show_existing_frame;
  gboolean  frame_to_show;
  guint frame_type;
  gboolean show_frame;
  gboolean error_resilient_mode;
  gboolean subsampling_x;
  gboolean subsampling_y;
  guint32 width;
  guint32 height;
  gboolean display_size_enabled;
  guint32 display_width;
  guint32 display_height;
  gboolean frame_context_idx;

  guint bit_depth;
  guint color_space;
  guint color_range;

  gboolean intra_only;
  gboolean reset_frame_context;
  gboolean refresh_frame_flags;

  gboolean ref_frame_indices[GST_VP9_REFS_PER_FRAME];
  gboolean ref_frame_sign_bias[GST_VP9_REFS_PER_FRAME];
  gboolean allow_high_precision_mv;
  GstVp9InterpFilter mcomp_filter_type;

  gboolean refresh_frame_context;
  /* frame_parallel_decoding_mode in vp9 code*/
  gboolean frame_parallel_decoding_mode;

  GstVp9LoopFilter loopfilter;
  GstVp9QuantIndices quant_indices;
  GstVp9SegmentationInfo segmentation;

  gboolean log2_tile_rows;
  gboolean log2_tile_columns;

  guint32 first_partition_size;
  guint32 frame_header_length_in_bytes;
};

struct _GstVp9Segmentation
{
  guint8 filter_level[4][2];
  gint16 luma_ac_quant_scale;
  gint16 luma_dc_quant_scale;
  gint16 chroma_ac_quant_scale;
  gint16 chroma_dc_quant_scale;

  gboolean reference_frame_enabled;
  guint8 reference_frame;

  gboolean reference_skip;
};

struct _GstVp9Parser
{
  /* private stuct for tracking state variables across frames */
  void *priv;

  gboolean lossless_flag;
  guint8 mb_segment_tree_probs[GST_VP9_SEG_TREE_PROBS];
  guint8 segment_pred_probs[GST_VP9_PREDICTION_PROBS];
  GstVp9Segmentation segmentation[GST_VP9_MAX_SEGMENTS];
};

GstVp9Parser *     gst_vp9_parser_new (void);

GstVp9ParserResult gst_vp9_parser_parse_frame_header (GstVp9Parser* parser, GstVp9FrameHdr * frame_hdr, const guint8 * data, gsize size);

void               gst_vp9_parser_free (GstVp9Parser * parser);

G_END_DECLS

#endif /* GST_VP9_PARSER_H */
