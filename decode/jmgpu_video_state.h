
#ifndef JM_VIDEO_STATE_H
#define JM_VIDEO_STATE_H

#ifdef __cplusplus
extern "C" {
#endif
#include "jmgpu_types.h"
#include "jmgpu_video_buffer.h"

typedef u32 JmgpuDecoderProfile;

/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_MPEG1                       ((JmgpuDecoderProfile)0)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_MPEG2_SIMPLE                ((JmgpuDecoderProfile)1)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_MPEG2_MAIN                  ((JmgpuDecoderProfile)2)
/** \hideinitializer */
/** \brief MPEG 4 part 10 == H.264 == AVC */
#define JMGPU_DECODER_PROFILE_H264_BASELINE               ((JmgpuDecoderProfile)66)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_H264_MAIN                   ((JmgpuDecoderProfile)77)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_H264_HIGH                   ((JmgpuDecoderProfile)100)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_VC1_SIMPLE                  ((JmgpuDecoderProfile)9)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_VC1_MAIN                    ((JmgpuDecoderProfile)10)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_VC1_ADVANCED                ((JmgpuDecoderProfile)11)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_MPEG4_PART2_SP              ((JmgpuDecoderProfile)12)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_MPEG4_PART2_ASP             ((JmgpuDecoderProfile)13)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_DIVX4_QMOBILE               ((JmgpuDecoderProfile)14)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_DIVX4_MOBILE                ((JmgpuDecoderProfile)15)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_DIVX4_HOME_THEATER          ((JmgpuDecoderProfile)16)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_DIVX4_HD_1080P              ((JmgpuDecoderProfile)17)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_DIVX5_QMOBILE               ((JmgpuDecoderProfile)18)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_DIVX5_MOBILE                ((JmgpuDecoderProfile)19)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_DIVX5_HOME_THEATER          ((JmgpuDecoderProfile)20)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_DIVX5_HD_1080P              ((JmgpuDecoderProfile)21)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_H264_CONSTRAINED_BASELINE   ((JmgpuDecoderProfile)22)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_H264_EXTENDED               ((JmgpuDecoderProfile)23)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_H264_PROGRESSIVE_HIGH       ((JmgpuDecoderProfile)24)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_H264_CONSTRAINED_HIGH       ((JmgpuDecoderProfile)25)
/** \hideinitializer */
/** \brief Support for 8 bit depth only */
#define JMGPU_DECODER_PROFILE_H264_HIGH_444_PREDICTIVE    ((JmgpuDecoderProfile)26)
/** \hideinitializer */
/** \brief MPEG-H Part 2 == H.265 == HEVC */
#define JMGPU_DECODER_PROFILE_HEVC_MAIN                   ((JmgpuDecoderProfile)100)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_HEVC_MAIN_10                ((JmgpuDecoderProfile)101)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_HEVC_MAIN_STILL             ((JmgpuDecoderProfile)102)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_HEVC_MAIN_12                ((JmgpuDecoderProfile)103)
/** \hideinitializer */
#define JMGPU_DECODER_PROFILE_HEVC_MAIN_444               ((JmgpuDecoderProfile)104)

typedef struct JmgpuPictureDesc {
	JmgpuVideoDataFormat profile;
} JmgpuPictureDesc;

typedef struct JmgpuH264Sps {
	u8 level_idc;
	u8 chroma_format_idc;
	u8 separate_colour_plane_flag;
	u8 bit_depth_luma_minus8;
	u8 bit_depth_chroma_minus8;
	u8 seq_scaling_matrix_present_flag;
	u8 ScalingList4x4[6][16];
	u8 ScalingList8x8[6][64];
	u8 log2_max_frame_num_minus4;
	u8 pic_order_cnt_type;
	u8 log2_max_pic_order_cnt_lsb_minus4;
	u8 delta_pic_order_always_zero_flag;
	s32 offset_for_non_ref_pic;
	s32 offset_for_top_to_bottom_field;
	u8 num_ref_frames_in_pic_order_cnt_cycle;
	s32 offset_for_ref_frame[256];
	u8 max_num_ref_frames;
	u8 frame_mbs_only_flag;
	u8 mb_adaptive_frame_field_flag;
	u8 direct_8x8_inference_flag;
} JmgpuH264Sps;

typedef struct JmgpuH264Pps {
	// JmgpuH264Sps *sps;
	u8 entropy_coding_mode_flag;
	u8 bottom_field_pic_order_in_frame_present_flag;
	u8 num_slice_groups_minus1;
	u8 slice_group_map_type;
	u8 slice_group_change_rate_minus1;
	u8 num_ref_idx_l0_default_active_minus1;
	u8 num_ref_idx_l1_default_active_minus1;
	u8 weighted_pred_flag;
	u8 weighted_bipred_idc;
	s8 pic_init_qp_minus26;
	s8 chroma_qp_index_offset;
	u8 deblocking_filter_control_present_flag;
	u8 constrained_intra_pred_flag;
	u8 redundant_pic_cnt_present_flag;
	u8 ScalingList4x4[6][16];
	u8 ScalingList8x8[6][64];
	u8 transform_8x8_mode_flag;
	s8 second_chroma_qp_index_offset;
} JmgpuH264Pps;

typedef struct JmgpuH264PictureDesc {
	JmgpuPictureDesc base;

	JmgpuH264Sps sps;
	JmgpuH264Pps pps;

	/* slice header */
	u32 frame_num;
	u8 field_pic_flag;
	u8 bottom_field_flag;
	u8 num_ref_idx_l0_active_minus1;
	u8 num_ref_idx_l1_active_minus1;

	u32 slice_count;
	s32 field_order_cnt[2];
	int is_reference;
	u8 num_ref_frames;

	int is_long_term[16];
	int top_is_reference[16];
	int bottom_is_reference[16];
	u32 field_order_cnt_list[16][2];
	u32 frame_num_list[16];

	JmgpuVideoBuffer *ref[16];
} JmgpuH264PictureDesc;

#if defined(__cplusplus)
}
#endif

#endif /*  JM_VIDEO_FORMAT_H */
