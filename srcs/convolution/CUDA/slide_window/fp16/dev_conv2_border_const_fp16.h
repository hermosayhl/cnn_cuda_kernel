/**
*    ---------------------------------------------------------------------
*    Author : Wayne Anderson
*    Date   : 2021.04.16
*    ---------------------------------------------------------------------
*    This is a part of the open source program named "DECX", copyright c Wayne,
*    2021.04.16
*/

#ifndef _DEV_CONV2_BORDER_CONST_FP16_H_
#define _DEV_CONV2_BORDER_CONST_FP16_H_

#include "../../../../core/basic.h"
#include "../../../../classes/core_types.h"
#include "../Conv_utils.h"
#include "../../../../classes/GPU_Matrix.h"
#include "../../rearrangement.h"


using decx::_Matrix;
using decx::alloc::MIF;
using decx::_GPU_Matrix;


namespace decx
{
    /*\
    * 8 x 8
    * This function complete a comand of a convolutipon kernel, which is in type of border-ignored
    * In order to save the device memory, just allocate memory of suitable size
    */
    static void _dev_Conv2_BC_R8x8_fp16(decx::_GPU_Matrix<de::Half>* src, decx::_GPU_Matrix<de::Half>* kernel, decx::_GPU_Matrix<de::Half>* dst, de::DH* handle);


    /*\
    * 8 x 16 (h x w)
    * This function complete a comand of a convolutipon kernel, which is in type of border-ignored
    * In order to save the device memory, just allocate memory of suitable size
    */
    static void _dev_Conv2_BC_R8x16_fp16(decx::_GPU_Matrix<de::Half>* src, decx::_GPU_Matrix<de::Half>* kernel, decx::_GPU_Matrix<de::Half>* dst, de::DH* handle);


    /*\
    * 16 x 8 (h x w)
    * This function complete a comand of a convolutipon kernel, which is in type of border-ignored
    * In order to save the device memory, just allocate memory of suitable size
    */
    static void _dev_Conv2_BC_R16x8_fp16(decx::_GPU_Matrix<de::Half>* src, decx::_GPU_Matrix<de::Half>* kernel, decx::_GPU_Matrix<de::Half>* dst, de::DH* handle);


    /*\
    * 16 x 16
    * This function complete a comand of a convolutipon kernel, which is in type of border-ignored
    * In order to save the device memory, just allocate memory of suitable size
    */
    static void _dev_Conv2_BC_R16x16_fp16(decx::_GPU_Matrix<de::Half>* src, decx::_GPU_Matrix<de::Half>* kernel, decx::_GPU_Matrix<de::Half>* dst, de::DH* handle);



    static void dev_hConv2_border_zero(decx::_GPU_Matrix<de::Half>& src, decx::_GPU_Matrix<de::Half>& kernel, decx::_GPU_Matrix<de::Half>& dst, de::DH* handle);
}




static
void decx::_dev_Conv2_BC_R8x8_fp16(decx::_GPU_Matrix<de::Half>* src, decx::_GPU_Matrix<de::Half>* kernel, decx::_GPU_Matrix<de::Half>* dst, de::DH *handle)
{
    float4* Dsrc,
        *Ddst;

    int2 Dsrc_alloc_dim,
        Ddst_alloc_dim,
        ker_dim;
    
    ker_dim.x = kernel->width;            ker_dim.y = kernel->height;
    // first, allocate the memory according to R8 alignments
    Ddst_alloc_dim.x = decx::utils::ceil<int>(dst->width, 64) * bounded_kernel_R8 * 2;
    Ddst_alloc_dim.y = decx::utils::ceil<int>(dst->height, 16) * bounded_kernel_R8 * 2;

    Dsrc_alloc_dim.x = Ddst_alloc_dim.x + bounded_kernel_R8 / 4;        // bounded_kernel_R8 * 2 / 4
    Dsrc_alloc_dim.y = Ddst_alloc_dim.y + bounded_kernel_R8 * 2;
    
    size_t dev_src_size = static_cast<size_t>(Dsrc_alloc_dim.x) * static_cast<size_t>(Dsrc_alloc_dim.y);
    size_t dev_dst_size = static_cast<size_t>(Ddst_alloc_dim.x) * static_cast<size_t>(Ddst_alloc_dim.y);

    decx::PtrInfo<float4> dev_tmp;
    decx::alloc::_alloc_D(&dev_tmp.block, (dev_src_size + dev_dst_size) * sizeof(float4));
    dev_tmp._sync_type();

    Dsrc = dev_tmp.ptr;
    Ddst = Dsrc + dev_src_size;

    cudaStream_t S;
    checkCudaErrors(cudaStreamCreate(&S));

    uint offset_lin = 0, offset_ker = 0;
    
    for (int i = 0; i < kernel->height; ++i) {
        cudaMemcpyToSymbolAsync(Const_Mem,
            kernel->Mat.ptr + offset_ker,
            kernel->width * sizeof(de::Half),
            offset_lin * sizeof(de::Half), cudaMemcpyDeviceToDevice, S);

        offset_lin += kernel->width;
        offset_ker += kernel->pitch;
    }

    checkCudaErrors(cudaMemcpy2DAsync(
        reinterpret_cast<de::Half*>(Dsrc) + Dsrc_alloc_dim.x * bounded_kernel_R8 * 4 + bounded_kernel_R8,
        Dsrc_alloc_dim.x * sizeof(float4),
        src->Mat.ptr,
        src->pitch * sizeof(de::Half),
        src->width * sizeof(de::Half),
        src->height,
        cudaMemcpyDeviceToDevice,
        S));                            // copy the datas of src from host to device

    if (ker_dim.x == (bounded_kernel_R8 * 2 + 1) && ker_dim.y == (bounded_kernel_R8 * 2 + 1)) {
        decx::hconv2_kernel_exact8x8(Dsrc, Ddst, Dsrc_alloc_dim, Ddst_alloc_dim, ker_dim, &S);
    }
    else {
        int2 src_diff;
        src_diff.x = bounded_kernel_R8 - ker_dim.y / 2;                src_diff.y = bounded_kernel_R8 - ker_dim.x / 2;
        
        decx::hconv2_kernel_within8x8(Dsrc, Ddst, src_diff, Dsrc_alloc_dim, Ddst_alloc_dim, ker_dim, &S);
    }
    
    checkCudaErrors(cudaMemcpy2DAsync(dst->Mat.ptr,        // copy the datas of src from device to host
        dst->pitch * sizeof(de::Half),
        Ddst,
        Ddst_alloc_dim.x * sizeof(float4),
        dst->width * sizeof(de::Half),
        dst->height,
        cudaMemcpyDeviceToDevice,
        S));

    checkCudaErrors(cudaDeviceSynchronize());

    decx::alloc::_dealloc_D(dev_tmp.block);

    checkCudaErrors(cudaStreamDestroy(S));
}




static
void decx::_dev_Conv2_BC_R8x16_fp16(decx::_GPU_Matrix<de::Half>* src, decx::_GPU_Matrix<de::Half>* kernel, decx::_GPU_Matrix<de::Half>* dst, de::DH *handle)
{
    float4* Dsrc,
        *Ddst;

    int2 Dsrc_alloc_dim,
        Ddst_alloc_dim,
        ker_dim;
    
    ker_dim.x = kernel->width;            ker_dim.y = kernel->height;
    // first, allocate the memory according to R8 alignments
    Ddst_alloc_dim.x = decx::utils::ceil<int>(dst->width, 64) * 16;
    Ddst_alloc_dim.y = decx::utils::ceil<int>(dst->height, 16) * 16;

    Dsrc_alloc_dim.x = Ddst_alloc_dim.x + bounded_kernel_R16 / 4;        // bounded_kernel_R16 * 2 / 4
    Dsrc_alloc_dim.y = Ddst_alloc_dim.y + bounded_kernel_R8 * 2;
    
    size_t dev_src_size = static_cast<size_t>(Dsrc_alloc_dim.x) * static_cast<size_t>(Dsrc_alloc_dim.y);
    size_t dev_dst_size = static_cast<size_t>(Ddst_alloc_dim.x) * static_cast<size_t>(Ddst_alloc_dim.y);

    //checkCudaErrors(cudaMalloc(&Dsrc, (dev_src_size + dev_dst_size) * sizeof(float4)));
    decx::PtrInfo<float4> dev_tmp;
    decx::alloc::_alloc_D(&dev_tmp.block, (dev_src_size + dev_dst_size) * sizeof(float4));
    dev_tmp._sync_type();

    Dsrc = dev_tmp.ptr;
    Ddst = Dsrc + dev_src_size;

    cudaStream_t S;
    checkCudaErrors(cudaStreamCreate(&S));

    uint offset_lin = 0, offset_ker = 0;
    
    for (int i = 0; i < kernel->height; ++i) {
        cudaMemcpyToSymbolAsync(Const_Mem,
            kernel->Mat.ptr + offset_ker,
            kernel->width * sizeof(de::Half),
            offset_lin * sizeof(de::Half), cudaMemcpyDeviceToDevice, S);

        offset_lin += kernel->width;
        offset_ker += kernel->pitch;
    }

    checkCudaErrors(cudaMemcpy2DAsync(
        reinterpret_cast<de::Half*>(Dsrc) + bounded_kernel_R8 * Dsrc_alloc_dim.x * 4 + bounded_kernel_R16,
        Dsrc_alloc_dim.x * sizeof(float4),
        src->Mat.ptr,
        src->pitch * sizeof(de::Half),
        src->width * sizeof(de::Half),
        src->height,
        cudaMemcpyDeviceToDevice,
        S));                            // copy the datas of src from host to device

    if (ker_dim.x == (bounded_kernel_R16 * 2 + 1) && ker_dim.y == (bounded_kernel_R8 * 2 + 1)) {
        decx::hconv2_kernel_exact8x16(Dsrc, Ddst, Dsrc_alloc_dim, Ddst_alloc_dim, ker_dim, &S);
    }
    else {
        int2 src_diff;
        src_diff.x = bounded_kernel_R8 - ker_dim.y / 2;                src_diff.y = bounded_kernel_R16 - ker_dim.x / 2;
        
        decx::hconv2_kernel_within8x16(Dsrc, Ddst, src_diff, Dsrc_alloc_dim, Ddst_alloc_dim, ker_dim, &S);
    }
    
    checkCudaErrors(cudaMemcpy2DAsync(dst->Mat.ptr,        // copy the datas of src from device to host
        dst->pitch * sizeof(de::Half),
        Ddst,
        Ddst_alloc_dim.x * sizeof(float4),
        dst->width * sizeof(de::Half),
        dst->height,
        cudaMemcpyDeviceToDevice,
        S));

    checkCudaErrors(cudaDeviceSynchronize());

    decx::alloc::_dealloc_D(dev_tmp.block);

    checkCudaErrors(cudaStreamDestroy(S));
}





static
void decx::_dev_Conv2_BC_R16x8_fp16(decx::_GPU_Matrix<de::Half>* src, decx::_GPU_Matrix<de::Half>* kernel, decx::_GPU_Matrix<de::Half>* dst, de::DH *handle)
{
    float4* Dsrc,
        *Ddst;

    int2 Dsrc_alloc_dim,
        Ddst_alloc_dim,
        ker_dim;
    
    ker_dim.x = kernel->width;            ker_dim.y = kernel->height;
    // first, allocate the memory according to R8 alignments
    Ddst_alloc_dim.x = decx::utils::ceil<int>(dst->width, 64) * 16;
    Ddst_alloc_dim.y = decx::utils::ceil<int>(dst->height, 16) * 16;

    Dsrc_alloc_dim.x = Ddst_alloc_dim.x + bounded_kernel_R8 / 2;        // bounded_kernel_R16 * 2 / 4
    Dsrc_alloc_dim.y = Ddst_alloc_dim.y + bounded_kernel_R16 * 2;
    
    size_t dev_src_size = static_cast<size_t>(Dsrc_alloc_dim.x) * static_cast<size_t>(Dsrc_alloc_dim.y);
    size_t dev_dst_size = static_cast<size_t>(Ddst_alloc_dim.x) * static_cast<size_t>(Ddst_alloc_dim.y);

    decx::PtrInfo<float4> dev_tmp;
    decx::alloc::_alloc_D(&dev_tmp.block, (dev_src_size + dev_dst_size) * sizeof(float4));
    dev_tmp._sync_type();

    Dsrc = dev_tmp.ptr;
    Ddst = Dsrc + dev_src_size;

    cudaStream_t S;
    checkCudaErrors(cudaStreamCreate(&S));

    uint offset_lin = 0, offset_ker = 0;
    
    for (uint i = 0; i < kernel->height; ++i) {
        cudaMemcpyToSymbolAsync(Const_Mem,
            kernel->Mat.ptr + offset_ker,
            kernel->width * sizeof(de::Half),
            offset_lin * sizeof(de::Half), cudaMemcpyDeviceToDevice, S);

        offset_lin += kernel->width;
        offset_ker += kernel->pitch;
    }

    checkCudaErrors(cudaMemcpy2DAsync(
        reinterpret_cast<de::Half*>(Dsrc) + bounded_kernel_R16 * Dsrc_alloc_dim.x * 4 + bounded_kernel_R8,
        Dsrc_alloc_dim.x * sizeof(float4),
        src->Mat.ptr,
        src->pitch * sizeof(de::Half),
        src->width * sizeof(de::Half),
        src->height,
        cudaMemcpyDeviceToDevice,
        S));                            // copy the datas of src from host to device

    if (ker_dim.x == (bounded_kernel_R8 * 2 + 1) && ker_dim.y == (bounded_kernel_R16 * 2 + 1)) {
        decx::hconv2_kernel_exact16x8(Dsrc, Ddst, Dsrc_alloc_dim, Ddst_alloc_dim, ker_dim, &S);
    }
    else {
        int2 src_diff;
        src_diff.x = bounded_kernel_R16 - ker_dim.y / 2;                src_diff.y = bounded_kernel_R8 - ker_dim.x / 2;
        
        decx::hconv2_kernel_within16x8(Dsrc, Ddst, src_diff, Dsrc_alloc_dim, Ddst_alloc_dim, ker_dim, &S);
    }
    
    checkCudaErrors(cudaMemcpy2DAsync(dst->Mat.ptr,        // copy the datas of src from device to host
        dst->pitch * sizeof(de::Half),
        Ddst,
        Ddst_alloc_dim.x * sizeof(float4),
        dst->width * sizeof(de::Half),
        dst->height,
        cudaMemcpyDeviceToDevice,
        S));

    checkCudaErrors(cudaDeviceSynchronize());

    decx::alloc::_dealloc_D(dev_tmp.block);

    checkCudaErrors(cudaStreamDestroy(S));
}




static
void decx::_dev_Conv2_BC_R16x16_fp16(decx::_GPU_Matrix<de::Half>* src, decx::_GPU_Matrix<de::Half>* kernel, decx::_GPU_Matrix<de::Half>* dst, de::DH *handle)
{
    float4* Dsrc,
        * Ddst;

    int2 Dsrc_alloc_dim,
        Ddst_alloc_dim,
        ker_dim;

    ker_dim.x = kernel->width;            ker_dim.y = kernel->height;
    // first, allocate the memory according to R8 alignments
    Ddst_alloc_dim.x = decx::utils::ceil<int>(dst->width, 64) * 16;
    Ddst_alloc_dim.y = decx::utils::ceil<int>(dst->height, 16) * 16;

    Dsrc_alloc_dim.x = Ddst_alloc_dim.x + bounded_kernel_R16 / 2;        // bounded_kernel_R16 * 2 / 4
    Dsrc_alloc_dim.y = Ddst_alloc_dim.y + bounded_kernel_R16 * 2;

    size_t dev_src_size = static_cast<size_t>(Dsrc_alloc_dim.x) * static_cast<size_t>(Dsrc_alloc_dim.y);
    size_t dev_dst_size = static_cast<size_t>(Ddst_alloc_dim.x) * static_cast<size_t>(Ddst_alloc_dim.y);

    decx::PtrInfo<float4> dev_tmp;
    decx::alloc::_alloc_D(&dev_tmp.block, (dev_src_size + dev_dst_size) * sizeof(float4));
    dev_tmp._sync_type();

    Dsrc = dev_tmp.ptr;
    Ddst = Dsrc + dev_src_size;

    cudaStream_t S;
    checkCudaErrors(cudaStreamCreate(&S));

    uint offset_lin = 0, offset_ker = 0;

    for (uint i = 0; i < kernel->height; ++i) {
        cudaMemcpyToSymbolAsync(Const_Mem,
            kernel->Mat.ptr + offset_ker,
            kernel->width * sizeof(de::Half),
            offset_lin * sizeof(de::Half), cudaMemcpyDeviceToDevice, S);

        offset_lin += kernel->width;
        offset_ker += kernel->pitch;
    }

    checkCudaErrors(cudaMemcpy2DAsync(
        reinterpret_cast<de::Half*>(Dsrc) + Dsrc_alloc_dim.x * bounded_kernel_R16 * 4 + bounded_kernel_R16,
        Dsrc_alloc_dim.x * sizeof(float4),
        src->Mat.ptr,
        src->pitch * sizeof(de::Half),
        src->width * sizeof(de::Half),
        src->height,
        cudaMemcpyDeviceToDevice,
        S));                            // copy the datas of src from host to device

    if (ker_dim.x == (bounded_kernel_R16 * 2 + 1) && ker_dim.y == (bounded_kernel_R16 * 2 + 1)) {    
        decx::hconv2_kernel_exact16x16(Dsrc, Ddst, Dsrc_alloc_dim, Ddst_alloc_dim, ker_dim, &S);
    }
    else {
        int2 src_diff;
        src_diff.x = bounded_kernel_R16 - ker_dim.y / 2;                src_diff.y = bounded_kernel_R16 - ker_dim.x / 2;
        
        decx::hconv2_kernel_within16x16(Dsrc, Ddst, src_diff, Dsrc_alloc_dim, Ddst_alloc_dim, ker_dim, &S);
    }

    checkCudaErrors(cudaMemcpy2DAsync(dst->Mat.ptr,        // copy the datas of src from device to host
        dst->pitch * sizeof(de::Half),
        Ddst,
        Ddst_alloc_dim.x * sizeof(float4),
        dst->width * sizeof(de::Half),
        dst->height,
        cudaMemcpyDeviceToDevice,
        S));

    checkCudaErrors(cudaDeviceSynchronize());

    decx::alloc::_dealloc_D(dev_tmp.block);

    checkCudaErrors(cudaStreamDestroy(S));
}




static 
void decx::dev_hConv2_border_zero(decx::_GPU_Matrix<de::Half>& src, decx::_GPU_Matrix<de::Half>& kernel, decx::_GPU_Matrix<de::Half>& dst, de::DH* handle)
{
    int2 half_ker_dim;
    half_ker_dim.x = kernel.width / 2;                half_ker_dim.y = kernel.height / 2;

    const uint2 dst_dim = make_uint2(src.width,
                                     src.height);

    decx::_dev_conv2_dst_rearrangement(&dst, dst_dim);

    if (half_ker_dim.x < bounded_kernel_R8 + 1) {
        if (half_ker_dim.y < bounded_kernel_R8 + 1) {
            decx::_dev_Conv2_BC_R8x8_fp16(&src, &kernel, &dst, handle);
        }
        else {
            decx::_dev_Conv2_BC_R16x8_fp16(&src, &kernel, &dst, handle);
        }
    }
    else {
        if (half_ker_dim.y < bounded_kernel_R8 + 1) {
            decx::_dev_Conv2_BC_R8x16_fp16(&src, &kernel, &dst, handle);
        }
        else {
            decx::_dev_Conv2_BC_R16x16_fp16(&src, &kernel, &dst, handle);
        }
    }
}




#endif        //    #ifndef _DEV_CONV2_BORDER_IGNORED_H_