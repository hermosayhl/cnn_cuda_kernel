/**
*    ---------------------------------------------------------------------
*    Author : Wayne Anderson
*   Date   : 2021.04.16
*    ---------------------------------------------------------------------
*    This is a part of the open source program named "DECX", copyright c Wayne,
*    2021.04.16
*/


#ifndef _CONV_UTILS_H_
#define _CONV_UTILS_H_

#include "../../../core/basic.h"
#include "conv2.cuh"
#include "../conv_flags.h"


#define N_conv_once 1024 * 1024

// the total bytes (IN BYTE) allowed for kernel to restore in constant memory
#define kernel_in_CM 512


#define conv2_bld 16
#define conv2_tps 4

#define bounded_kernel_R8  8
#define bounded_kernel_R16 16


namespace decx
{

    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void sconv2_kernel_within8x8(float4* Dsrc,
                                               float4* Ddst,
                                               const int2 kernel_shift,
                                               const int2 src_dim,
                                               const int2 dst_dim,
                                               const int2 ker_dim,
                                               cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r8_within << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift);
    }


    /**\
    @param offset : In float
    */
    static inline void sconv2_kernel_within8x8_offset(float4* Dsrc,
                                                      float4* Ddst,
                                                      const int2 kernel_shift,
                                                      const int2 src_dim,
                                                      const int2 dst_dim,
                                                      const int2 ker_dim,
                                                      const size_t offset,
                                                      cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r8_within_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift, offset);
    }



    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void hconv2_kernel_within8x8(float4* Dsrc,
                                               float4* Ddst,
                                               const int2 kernel_shift,
                                               const int2 src_dim,
                                               const int2 dst_dim,
                                               const int2 ker_dim,
                                               cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r8_within << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift);
    }

    static inline void hconv2_kernel_within8x8_offset(float4* Dsrc,
                                                      float4* Ddst,
                                                      const int2 kernel_shift,
                                                      const int2 src_dim,
                                                      const int2 dst_dim,
                                                      const int2 ker_dim,
                                                      const size_t offset,
                                                      cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r8_within_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift, offset);
    }


    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void sconv2_kernel_exact8x8(float4* Dsrc,
                                              float4* Ddst,
                                              const int2 src_dim,
                                              const int2 dst_dim,
                                              const int2 ker_dim,
                                              cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r8_exact << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x);
    }

    static inline void sconv2_kernel_exact8x8_offset(float4* Dsrc,
                                                     float4* Ddst,
                                                     const int2 src_dim,
                                                     const int2 dst_dim,
                                                     const int2 ker_dim,
                                                     const size_t offset,
                                                     cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r8_exact_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, offset);
    }



    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void hconv2_kernel_exact8x8(float4* Dsrc,
            float4* Ddst,
            const int2 src_dim,
            const int2 dst_dim,
            const int2 ker_dim,
            cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r8_exact << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x);
    }

    static inline void hconv2_kernel_exact8x8_offset(float4* Dsrc,
        float4* Ddst,
        const int2 src_dim,
        const int2 dst_dim,
        const int2 ker_dim,
        const size_t offset,
        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r8_exact_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, offset);
    }




    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void sconv2_kernel_exact16x16(float4* Dsrc,
                                                float4* Ddst,
                                                const int2 src_dim,
                                                const int2 dst_dim,
                                                const int2 ker_dim,
                                                cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r16_exact << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x);
    }

    static inline void sconv2_kernel_exact16x16_offset(float4* Dsrc,
        float4* Ddst,
        const int2 src_dim,
        const int2 dst_dim,
        const int2 ker_dim,
        const size_t offset,
        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r16_exact_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, offset);
    }


    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void hconv2_kernel_exact16x16(float4* Dsrc,
                                                float4* Ddst,
                                                const int2 src_dim,
                                                const int2 dst_dim,
                                                const int2 ker_dim,
                                                cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r16_exact << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x);
    }

    static inline void hconv2_kernel_exact16x16_offset(float4* Dsrc,
        float4* Ddst,
        const int2 src_dim,
        const int2 dst_dim,
        const int2 ker_dim,
        const size_t offset,
        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r16_exact_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, offset);
    }



    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void sconv2_kernel_within16x16(float4* Dsrc,
                                                 float4* Ddst,
                                                 const int2 kernel_shift,
                                                 const int2 src_dim,
                                                 const int2 dst_dim,
                                                 const int2 ker_dim,
                                                 cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r16_within << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift);
    }

    static inline void sconv2_kernel_within16x16_offset(float4* Dsrc,
                                                        float4* Ddst,
                                                        const int2 kernel_shift,
                                                        const int2 src_dim,
                                                        const int2 dst_dim,
                                                        const int2 ker_dim,
                                                        const size_t offset,
                                                        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r16_within_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift, offset);
    }




    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void hconv2_kernel_within16x16(float4* Dsrc,
                                                 float4* Ddst,
                                                 const int2 kernel_shift,
                                                 const int2 src_dim,
                                                 const int2 dst_dim,
                                                 const int2 ker_dim,
                                                 cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r16_within << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift);
    }

    static inline void hconv2_kernel_within16x16_offset(float4* Dsrc,
                                                        float4* Ddst,
                                                        const int2 kernel_shift,
                                                        const int2 src_dim,
                                                        const int2 dst_dim,
                                                        const int2 ker_dim,
                                                        const size_t offset,
                                                        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r16_within_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift, offset);
    }




    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void sconv2_kernel_exact8x16(float4* Dsrc,
                                               float4* Ddst,
                                               const int2 src_dim,
                                               const int2 dst_dim,
                                               const int2 ker_dim,
                                               cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r816_exact << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x);
    }

    static inline void sconv2_kernel_exact8x16_offset(float4* Dsrc,
                                                      float4* Ddst,
                                                      const int2 src_dim,
                                                      const int2 dst_dim,
                                                      const int2 ker_dim,
                                                      const size_t offset,
                                                      cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r816_exact_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, offset);
    }




    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void hconv2_kernel_exact8x16(float4* Dsrc,
            float4* Ddst,
            const int2 src_dim,
            const int2 dst_dim,
            const int2 ker_dim,
            cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r816_exact << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x);
    }

    static inline void hconv2_kernel_exact8x16_offset(float4* Dsrc,
        float4* Ddst,
        const int2 src_dim,
        const int2 dst_dim,
        const int2 ker_dim,
        const size_t offset,
        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r816_exact_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, offset);
    }





    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void sconv2_kernel_within8x16(float4* Dsrc,
            float4* Ddst,
            const int2 kernel_shift,
            const int2 src_dim,
            const int2 dst_dim,
            const int2 ker_dim,
            cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r816_within << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift);
    }

    static inline void sconv2_kernel_within8x16_offset(float4* Dsrc,
        float4* Ddst,
        const int2 kernel_shift,
        const int2 src_dim,
        const int2 dst_dim,
        const int2 ker_dim,
        const size_t offset,
        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r816_within_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift, offset);
    }



    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void hconv2_kernel_within8x16(float4* Dsrc,
            float4* Ddst,
            const int2 kernel_shift,
            const int2 src_dim,
            const int2 dst_dim,
            const int2 ker_dim,
            cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r816_within << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift);
    }

    static inline void hconv2_kernel_within8x16_offset(float4* Dsrc,
        float4* Ddst,
        const int2 kernel_shift,
        const int2 src_dim,
        const int2 dst_dim,
        const int2 ker_dim,
        const size_t offset,
        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r816_within_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift, offset);
    }




    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void sconv2_kernel_exact16x8(float4* Dsrc,
            float4* Ddst,
            const int2 src_dim,
            const int2 dst_dim,
            const int2 ker_dim,
            cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r168_exact << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x);
    }

    static inline void sconv2_kernel_exact16x8_offset(float4* Dsrc,
        float4* Ddst,
        const int2 src_dim,
        const int2 dst_dim,
        const int2 ker_dim,
        const size_t offset,
        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r168_exact_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, offset);
    }



    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void hconv2_kernel_exact16x8(float4* Dsrc,
            float4* Ddst,
            const int2 src_dim,
            const int2 dst_dim,
            const int2 ker_dim,
            cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r168_exact << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x);
    }

    static inline void hconv2_kernel_exact16x8_offset(float4* Dsrc,
        float4* Ddst,
        const int2 src_dim,
        const int2 dst_dim,
        const int2 ker_dim,
        const size_t offset,
        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r168_exact_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, offset);
    }



    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void sconv2_kernel_within16x8(float4* Dsrc,
            float4* Ddst,
            const int2 kernel_shift,
            const int2 src_dim,
            const int2 dst_dim,
            const int2 ker_dim,
            cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r168_within << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift);
    }

    static inline void sconv2_kernel_within16x8_offset(float4* Dsrc,
        float4* Ddst,
        const int2 kernel_shift,
        const int2 src_dim,
        const int2 dst_dim,
        const int2 ker_dim,
        const size_t offset,
        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_sConv2_r168_within_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift, offset);
    }



    /**\
    * The conv2 kernel, this function complete one single convolution, regardless
    * of borders
    * @param Dsrc : device ptr of source matrix
    * @param Ddst : device ptr of destination matrix
    * @param src_dim : ~.x : pitch_src (having considered float4); ~.y : Hsrc, in float
    * @param dst_dim : ~.x : pitch_dst (having considered float4); ~.y : Hdst, in float
    */
    static inline void hconv2_kernel_within16x8(float4* Dsrc,
            float4* Ddst,
            const int2 kernel_shift,
            const int2 src_dim,
            const int2 dst_dim,
            const int2 ker_dim,
            cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r168_within << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift);
    }

    static inline void hconv2_kernel_within16x8_offset(float4* Dsrc,
        float4* Ddst,
        const int2 kernel_shift,
        const int2 src_dim,
        const int2 dst_dim,
        const int2 ker_dim,
        const size_t offset,
        cudaStream_t* S)    noexcept
    {
        const dim3 threads(conv2_bld, conv2_bld);
        const dim3 blocks(dst_dim.y / (conv2_bld), dst_dim.x / (conv2_bld));

        cu_hConv2_r168_within_offset << <blocks, threads, 0, *S >> > (
            Dsrc, Ddst, src_dim.x, dst_dim.x, ker_dim.x * ker_dim.y, ker_dim.x, kernel_shift, offset);
    }

}


#endif