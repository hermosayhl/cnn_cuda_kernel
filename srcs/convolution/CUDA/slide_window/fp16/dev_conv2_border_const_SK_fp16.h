/**
*    ---------------------------------------------------------------------
*    Author : Wayne Anderson
*    Date   : 2021.04.16
*    ---------------------------------------------------------------------
*    This is a part of the open source program named "DECX", copyright c Wayne,
*    2021.04.16
*/

#ifndef _DEV_CONV2_MC_BORDER_CONST_SK_FP16_H_
#define _DEV_CONV2_MC_BORDER_CONST_SK_FP16_H_


#include "../../../../core/basic.h"
#include "../../../../classes/GPU_Matrix.h"
#include "../../../../classes/GPU_MatrixArray.h"
#include "../Conv3_macros.h"
#include "../Conv_utils.h"


using decx::_GPU_Matrix;
using decx::alloc::MIF;
using decx::_GPU_MatrixArray;


namespace decx
{
    static void dev_main_loop_hconv2_sk_within8x8_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S);



    static void dev_main_loop_hconv2_sk_exact8x8_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S);



    static void dev_main_loop_hconv2_sk_within8x16_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S);


    static void dev_main_loop_hconv2_sk_exact8x16_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S);


    static void dev_main_loop_hconv2_sk_within16x8_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S);


    static void dev_main_loop_hconv2_sk_exact16x8_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S);


    static void dev_main_loop_hconv2_sk_within16x16_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S);


    static void dev_main_loop_hconv2_sk_exact16x16_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S);


    // single kernel
    static void dev_Conv2_BC_R8x8_SK(_GPU_MatrixArray<de::Half>* src, _GPU_Matrix<de::Half>* kernel, _GPU_MatrixArray<de::Half>* dst);


    // single kernel
    static void dev_Conv2_BC_R8x16_SK(_GPU_MatrixArray<de::Half>* src, _GPU_Matrix<de::Half>* kernel, _GPU_MatrixArray<de::Half>* dst);


    // single kernel
    static void dev_Conv2_BC_R16x8_SK(_GPU_MatrixArray<de::Half>* src, _GPU_Matrix<de::Half>* kernel, _GPU_MatrixArray<de::Half>* dst);


    // single kernel
    static void dev_Conv2_BC_R16x16_SK(_GPU_MatrixArray<de::Half>* src, _GPU_Matrix<de::Half>* kernel, _GPU_MatrixArray<de::Half>* dst);


    static void dev_hConv2_border_zero_sk(_GPU_MatrixArray<de::Half>* src, _GPU_Matrix<de::Half>* kernel, _GPU_MatrixArray<de::Half>* dst, de::DH* handle);
}




static void decx::dev_main_loop_hconv2_sk_within8x8_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S)
{
    int2 src_diff;
    // copy the first part to device memory
    src_diff.x = bounded_kernel_R8 - ker_dim->y / 2;                src_diff.y = bounded_kernel_R8 - ker_dim->x / 2;

    // strat the main loop
    for (int i = 0; i < src->ArrayNumber; ++i) {
        checkCudaErrors(cudaMemcpy2DAsync(
            reinterpret_cast<de::Half*>(Dsrc) + Dsrc_alloc_dim->x * bounded_kernel_R8 * 8 + bounded_kernel_R8,
            Dsrc_alloc_dim->x * sizeof(float4),
            src->MatptrArr.ptr[i],
            src->pitch * sizeof(de::Half),
            src->width * sizeof(de::Half),
            src->height,
            cudaMemcpyDeviceToDevice,
            S));                            // copy the datas of src from host to device

        hconv2_kernel_within8x8(Dsrc, Ddst, src_diff, *Dsrc_alloc_dim, *Ddst_alloc_dim, *ker_dim, &S);

        checkCudaErrors(cudaMemcpy2DAsync(dst->MatptrArr.ptr[i],
            dst->pitch * sizeof(de::Half), Ddst, Ddst_alloc_dim->x * sizeof(float4), dst->width * sizeof(de::Half),
            dst->height, cudaMemcpyDeviceToDevice, S));
    }
}




static void decx::dev_main_loop_hconv2_sk_exact8x8_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S)
{
    // strat the main loop
    for (int i = 0; i < src->ArrayNumber; ++i) {
        checkCudaErrors(cudaMemcpy2DAsync(
            reinterpret_cast<de::Half*>(Dsrc) + Dsrc_alloc_dim->x * bounded_kernel_R8 * 8 + bounded_kernel_R8,
            Dsrc_alloc_dim->x * sizeof(float4),
            src->MatptrArr.ptr[0],
            src->pitch * sizeof(de::Half),
            src->width * sizeof(de::Half),
            src->height,
            cudaMemcpyDeviceToDevice,
            S));                            // copy the datas of src from host to device

        hconv2_kernel_exact8x8(Dsrc, Ddst, *Dsrc_alloc_dim, *Ddst_alloc_dim, *ker_dim, &S);

        checkCudaErrors(cudaMemcpy2DAsync(dst->MatptrArr.ptr[i],
            dst->pitch * sizeof(de::Half), Ddst, Ddst_alloc_dim->x * sizeof(float4), dst->width * sizeof(de::Half),
            dst->height, cudaMemcpyDeviceToDevice, S));
    }
}





static void decx::dev_main_loop_hconv2_sk_within8x16_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S)
{
    int2 src_diff;
    // copy the first part to device memory
    src_diff.x = bounded_kernel_R8 - ker_dim->y / 2;                src_diff.y = bounded_kernel_R16 - ker_dim->x / 2;

    // strat the main loop
    for (int i = 0; i < src->ArrayNumber; ++i) {
        checkCudaErrors(cudaMemcpy2DAsync(
            reinterpret_cast<de::Half*>(Dsrc) + Dsrc_alloc_dim->x * bounded_kernel_R8 * 4 + bounded_kernel_R16,
            Dsrc_alloc_dim->x * sizeof(float4),
            src->MatptrArr.ptr[i],
            src->pitch * sizeof(de::Half),
            src->width * sizeof(de::Half),
            src->height,
            cudaMemcpyDeviceToDevice,
            S));                            // copy the datas of src from host to device

        hconv2_kernel_within8x16(Dsrc, Ddst, src_diff, *Dsrc_alloc_dim, *Ddst_alloc_dim, *ker_dim, &S);

        checkCudaErrors(cudaMemcpy2DAsync(dst->MatptrArr.ptr[i],
            dst->pitch * sizeof(de::Half), Ddst, Ddst_alloc_dim->x * sizeof(float4), dst->width * sizeof(de::Half),
            dst->height, cudaMemcpyDeviceToDevice, S));
    }
}




static void decx::dev_main_loop_hconv2_sk_exact8x16_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S)
{
    // strat the main loop
    for (int i = 0; i < src->ArrayNumber; ++i) {
        checkCudaErrors(cudaMemcpy2DAsync(
            reinterpret_cast<de::Half*>(Dsrc) + Dsrc_alloc_dim->x * bounded_kernel_R8 * 8 + bounded_kernel_R16,
            Dsrc_alloc_dim->x * sizeof(float4),
            src->MatptrArr.ptr[i],
            src->pitch * sizeof(de::Half),
            src->width * sizeof(de::Half),
            src->height,
            cudaMemcpyDeviceToDevice,
            S));                            // copy the datas of src from host to device

        hconv2_kernel_exact8x16(Dsrc, Ddst, *Dsrc_alloc_dim, *Ddst_alloc_dim, *ker_dim, &S);

        checkCudaErrors(cudaMemcpy2DAsync(dst->MatptrArr.ptr[i],
            dst->pitch * sizeof(de::Half), Ddst, Ddst_alloc_dim->x * sizeof(float4), dst->width * sizeof(de::Half),
            dst->height, cudaMemcpyDeviceToDevice, S));
    }
}




static void decx::dev_main_loop_hconv2_sk_within16x8_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S)
{
    int2 src_diff;
    // copy the first part to device memory
    src_diff.x = bounded_kernel_R16 - ker_dim->y / 2;                src_diff.y = bounded_kernel_R8 - ker_dim->x / 2;

    // strat the main loop
    for (int i = 0; i < src->ArrayNumber; ++i) {
        checkCudaErrors(cudaMemcpy2DAsync(
            reinterpret_cast<de::Half*>(Dsrc) + Dsrc_alloc_dim->x * bounded_kernel_R16 * 8 + bounded_kernel_R8,
            Dsrc_alloc_dim->x * sizeof(float4),
            src->MatptrArr.ptr[i],
            src->pitch * sizeof(de::Half),
            src->width * sizeof(de::Half),
            src->height,
            cudaMemcpyDeviceToDevice,
            S));                            // copy the datas of src from host to device

        hconv2_kernel_within16x8(Dsrc, Ddst, src_diff, *Dsrc_alloc_dim, *Ddst_alloc_dim, *ker_dim, &S);

        checkCudaErrors(cudaMemcpy2DAsync(dst->MatptrArr.ptr[i],
            dst->pitch * sizeof(de::Half), Ddst, Ddst_alloc_dim->x * sizeof(float4), dst->width * sizeof(de::Half),
            dst->height, cudaMemcpyDeviceToDevice, S));
    }
}




static void decx::dev_main_loop_hconv2_sk_exact16x8_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S)
{
    // strat the main loop
    for (int i = 0; i < src->ArrayNumber; ++i) {
        checkCudaErrors(cudaMemcpy2DAsync(
            reinterpret_cast<de::Half*>(Dsrc) + Dsrc_alloc_dim->x * bounded_kernel_R16 * 8 + bounded_kernel_R8,
            Dsrc_alloc_dim->x * sizeof(float4),
            src->MatptrArr.ptr[i],
            src->pitch * sizeof(de::Half),
            src->width * sizeof(de::Half),
            src->height,
            cudaMemcpyDeviceToDevice,
            S));                            // copy the datas of src from host to device

        hconv2_kernel_exact8x16(Dsrc, Ddst, *Dsrc_alloc_dim, *Ddst_alloc_dim, *ker_dim, &S);

        checkCudaErrors(cudaMemcpy2DAsync(dst->MatptrArr.ptr[i],
            dst->pitch * sizeof(de::Half), Ddst, Ddst_alloc_dim->x * sizeof(float4), dst->width * sizeof(de::Half),
            dst->height, cudaMemcpyDeviceToDevice, S));
    }
}





static void decx::dev_main_loop_hconv2_sk_within16x16_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S)
{
    int2 src_diff;
    // copy the first part to device memory
    src_diff.x = bounded_kernel_R16 - ker_dim->y / 2;                src_diff.y = bounded_kernel_R16 - ker_dim->x / 2;

    // strat the main loop
    for (int i = 0; i < src->ArrayNumber; ++i) {
        checkCudaErrors(cudaMemcpy2DAsync(
            reinterpret_cast<de::Half*>(Dsrc) + Dsrc_alloc_dim->x * bounded_kernel_R16 * 8 + bounded_kernel_R16,
            Dsrc_alloc_dim->x * sizeof(float4),
            src->MatptrArr.ptr[i],
            src->pitch * sizeof(de::Half),
            src->width * sizeof(de::Half),
            src->height,
            cudaMemcpyDeviceToDevice,
            S));                            // copy the datas of src from host to device

        hconv2_kernel_within16x8(Dsrc, Ddst, src_diff, *Dsrc_alloc_dim, *Ddst_alloc_dim, *ker_dim, &S);

        checkCudaErrors(cudaMemcpy2DAsync(dst->MatptrArr.ptr[i],
            dst->pitch * sizeof(de::Half), Ddst, Ddst_alloc_dim->x * sizeof(float4), dst->width * sizeof(de::Half),
            dst->height, cudaMemcpyDeviceToDevice, S));
    }
}




static void decx::dev_main_loop_hconv2_sk_exact16x16_BC(
        int2* Dsrc_alloc_dim,                int2* Ddst_alloc_dim,                int2* ker_dim,
        _GPU_MatrixArray<de::Half>* src,     _GPU_Matrix<de::Half>* kernel,       _GPU_MatrixArray<de::Half>* dst,
        float4* Dsrc,                        float4* Ddst,                        cudaStream_t S)
{
    // strat the main loop
    for (int i = 0; i < src->ArrayNumber; ++i) {
        checkCudaErrors(cudaMemcpy2DAsync(
            reinterpret_cast<de::Half*>(Dsrc) + Dsrc_alloc_dim->x * bounded_kernel_R16 * 8 + bounded_kernel_R16,
            Dsrc_alloc_dim->x * sizeof(float4),
            src->MatptrArr.ptr[i],
            src->pitch * sizeof(de::Half),
            src->width * sizeof(de::Half),
            src->height,
            cudaMemcpyDeviceToDevice,
            S));                            // copy the datas of src from host to device

        hconv2_kernel_exact16x16(Dsrc, Ddst, *Dsrc_alloc_dim, *Ddst_alloc_dim, *ker_dim, &S);

        checkCudaErrors(cudaMemcpy2DAsync(dst->MatptrArr.ptr[i],
            dst->pitch * sizeof(de::Half), Ddst, Ddst_alloc_dim->x * sizeof(float4), dst->width * sizeof(de::Half),
            dst->height, cudaMemcpyDeviceToDevice, S));
    }
}



// **************************************************************************************************************************


// single kernel
static void decx::dev_Conv2_BC_R8x8_SK(_GPU_MatrixArray<de::Half>* src, _GPU_Matrix<de::Half>* kernel, _GPU_MatrixArray<de::Half>* dst)
{
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

    decx::PtrInfo<float4> dev_buffer;
    if (decx::alloc::_device_malloc(&dev_buffer, (dev_src_size + dev_dst_size) * sizeof(float4))) {
        Print_Error_Message(4, ALLOC_FAIL);
        return;
    }

    float4* Dsrc = dev_buffer.ptr;
    float4* Ddst = dev_buffer.ptr + dev_src_size;

    cudaStream_t S;
    checkCudaErrors(cudaStreamCreate(&S));

    uint offset_lin = 0, offset_ker = 0;

    for (uint k = 0; k < kernel->height; ++k) {
        cudaMemcpyToSymbolAsync(Const_Mem, kernel->Mat.ptr + offset_ker,
            kernel->width * sizeof(de::Half), offset_lin * sizeof(de::Half), cudaMemcpyDeviceToDevice, S);
        offset_lin += kernel->width;
        offset_ker += kernel->pitch;
    }
    // strat the main loop
    if (ker_dim.x == (bounded_kernel_R8 * 2 + 1) && ker_dim.y == (bounded_kernel_R8 * 2 + 1)) {
        decx::dev_main_loop_hconv2_sk_exact8x8_BC(
            &Dsrc_alloc_dim, &Ddst_alloc_dim, &ker_dim, src, kernel, dst, Dsrc, Ddst, S);
    }
    else {
        decx::dev_main_loop_hconv2_sk_within8x8_BC(
            &Dsrc_alloc_dim, &Ddst_alloc_dim, &ker_dim, src, kernel, dst, Dsrc, Ddst, S);
    }

    decx::alloc::_device_dealloc(&dev_buffer);

    checkCudaErrors(cudaDeviceSynchronize());

    checkCudaErrors(cudaStreamDestroy(S));
}




// single kernel
static void decx::dev_Conv2_BC_R8x16_SK(_GPU_MatrixArray<de::Half>* src, _GPU_Matrix<de::Half>* kernel, _GPU_MatrixArray<de::Half>* dst)
{
    int2 Dsrc_alloc_dim,
        Ddst_alloc_dim,
        ker_dim;

    ker_dim.x = kernel->width;            ker_dim.y = kernel->height;
    // first, allocate the memory according to R8 alignments
    Ddst_alloc_dim.x = decx::utils::ceil<int>(dst->width, 64) * bounded_kernel_R8 * 2;
    Ddst_alloc_dim.y = decx::utils::ceil<int>(dst->height, 16) * bounded_kernel_R8 * 2;

    Dsrc_alloc_dim.x = Ddst_alloc_dim.x + bounded_kernel_R16 / 4;        // bounded_kernel_R8 * 2 / 4
    Dsrc_alloc_dim.y = Ddst_alloc_dim.y + bounded_kernel_R8 * 2;

    size_t dev_src_size = static_cast<size_t>(Dsrc_alloc_dim.x) * static_cast<size_t>(Dsrc_alloc_dim.y);
    size_t dev_dst_size = static_cast<size_t>(Ddst_alloc_dim.x) * static_cast<size_t>(Ddst_alloc_dim.y);

    decx::PtrInfo<float4> dev_buffer;
    if (decx::alloc::_device_malloc(&dev_buffer, (dev_src_size + dev_dst_size) * sizeof(float4))) {
        Print_Error_Message(4, ALLOC_FAIL);
        return;
    }

    float4* Dsrc = dev_buffer.ptr;
    float4* Ddst = dev_buffer.ptr + dev_src_size;

    cudaStream_t S;
    checkCudaErrors(cudaStreamCreate(&S));

    uint offset_lin = 0, offset_ker = 0;

    for (uint k = 0; k < kernel->height; ++k) {
        cudaMemcpyToSymbolAsync(Const_Mem, kernel->Mat.ptr + offset_ker,
            kernel->width * sizeof(de::Half), offset_lin * sizeof(de::Half), cudaMemcpyDeviceToDevice, S);
        offset_lin += kernel->width;
        offset_ker += kernel->pitch;
    }
    // strat the main loop
    if (ker_dim.x == (bounded_kernel_R16 * 2 + 1) && ker_dim.y == (bounded_kernel_R8 * 2 + 1)) {
        decx::dev_main_loop_hconv2_sk_exact8x16_BC(
            &Dsrc_alloc_dim, &Ddst_alloc_dim, &ker_dim, src, kernel, dst, Dsrc, Ddst, S);
    }
    else {
        decx::dev_main_loop_hconv2_sk_within8x16_BC(
            &Dsrc_alloc_dim, &Ddst_alloc_dim, &ker_dim, src, kernel, dst, Dsrc, Ddst, S);
    }

    decx::alloc::_device_dealloc(&dev_buffer);

    checkCudaErrors(cudaDeviceSynchronize());

    checkCudaErrors(cudaStreamDestroy(S));
}



// single kernel
static void decx::dev_Conv2_BC_R16x8_SK(_GPU_MatrixArray<de::Half>* src, _GPU_Matrix<de::Half>* kernel, _GPU_MatrixArray<de::Half>* dst)
{
    MIF<float4> Dmem1, Dmem2,    // for src
        Dmem3, Dmem4;            // for dst

    int2 Dsrc_alloc_dim,
        Ddst_alloc_dim,
        ker_dim;

    ker_dim.x = kernel->width;            ker_dim.y = kernel->height;
    // first, allocate the memory according to R8 alignments
    Ddst_alloc_dim.x = decx::utils::ceil<int>(dst->width, 64) * bounded_kernel_R8 * 2;
    Ddst_alloc_dim.y = decx::utils::ceil<int>(dst->height, 16) * bounded_kernel_R8 * 2;

    Dsrc_alloc_dim.x = Ddst_alloc_dim.x + bounded_kernel_R8 / 4;        // bounded_kernel_R8 * 2 / 4
    Dsrc_alloc_dim.y = Ddst_alloc_dim.y + bounded_kernel_R16 * 2;

    size_t dev_src_size = static_cast<size_t>(Dsrc_alloc_dim.x) * static_cast<size_t>(Dsrc_alloc_dim.y);
    size_t dev_dst_size = static_cast<size_t>(Ddst_alloc_dim.x) * static_cast<size_t>(Ddst_alloc_dim.y);

    decx::PtrInfo<float4> dev_buffer;
    if (decx::alloc::_device_malloc(&dev_buffer, (dev_src_size + dev_dst_size) * sizeof(float4))) {
        Print_Error_Message(4, ALLOC_FAIL);
        return;
    }

    float4* Dsrc = dev_buffer.ptr;
    float4* Ddst = dev_buffer.ptr + dev_src_size;

    cudaStream_t S;
    checkCudaErrors(cudaStreamCreate(&S));

    uint offset_lin = 0, offset_ker = 0;

    for (int k = 0; k < kernel->height; ++k) {
        cudaMemcpyToSymbolAsync(Const_Mem, kernel->Mat.ptr + offset_ker,
            kernel->width * sizeof(de::Half), offset_lin * sizeof(de::Half), cudaMemcpyDeviceToDevice, S);
        offset_lin += kernel->width;
        offset_ker += kernel->pitch;
    }
    // strat the main loop
    if (ker_dim.x == (bounded_kernel_R8 * 2 + 1) && ker_dim.y == (bounded_kernel_R16 * 2 + 1)) {
        decx::dev_main_loop_hconv2_sk_exact16x8_BC(
            &Dsrc_alloc_dim, &Ddst_alloc_dim, &ker_dim, src, kernel, dst, Dsrc, Ddst, S);
    }
    else {
        decx::dev_main_loop_hconv2_sk_within16x8_BC(
            &Dsrc_alloc_dim, &Ddst_alloc_dim, &ker_dim, src, kernel, dst, Dsrc, Ddst, S);
    }

    decx::alloc::_device_dealloc(&dev_buffer);

    checkCudaErrors(cudaDeviceSynchronize());

    checkCudaErrors(cudaStreamDestroy(S));
}





// single kernel
static void decx::dev_Conv2_BC_R16x16_SK(_GPU_MatrixArray<de::Half>* src, _GPU_Matrix<de::Half>* kernel, _GPU_MatrixArray<de::Half>* dst)
{
    MIF<float4> Dmem1, Dmem2,    // for src
        Dmem3, Dmem4;            // for dst

    int2 Dsrc_alloc_dim,
        Ddst_alloc_dim,
        ker_dim;

    ker_dim.x = kernel->width;            ker_dim.y = kernel->height;
    // first, allocate the memory according to R8 alignments
    Ddst_alloc_dim.x = decx::utils::ceil<int>(dst->width, 64) * bounded_kernel_R8 * 2;
    Ddst_alloc_dim.y = decx::utils::ceil<int>(dst->height, 16) * bounded_kernel_R8 * 2;

    Dsrc_alloc_dim.x = Ddst_alloc_dim.x + bounded_kernel_R16 / 4;        // bounded_kernel_R8 * 2 / 4
    Dsrc_alloc_dim.y = Ddst_alloc_dim.y + bounded_kernel_R16 * 2;

    size_t dev_src_size = static_cast<size_t>(Dsrc_alloc_dim.x) * static_cast<size_t>(Dsrc_alloc_dim.y);
    size_t dev_dst_size = static_cast<size_t>(Ddst_alloc_dim.x) * static_cast<size_t>(Ddst_alloc_dim.y);

    decx::PtrInfo<float4> dev_buffer;
    if (decx::alloc::_device_malloc(&dev_buffer, (dev_src_size + dev_dst_size) * sizeof(float4))) {
        Print_Error_Message(4, ALLOC_FAIL);
        return;
    }

    float4* Dsrc = dev_buffer.ptr;
    float4* Ddst = dev_buffer.ptr + dev_src_size;

    cudaStream_t S;
    checkCudaErrors(cudaStreamCreate(&S));

    uint offset_lin = 0, offset_ker = 0;

    for (int k = 0; k < kernel->height; ++k) {
        cudaMemcpyToSymbolAsync(Const_Mem, kernel->Mat.ptr + offset_ker,
            kernel->width * sizeof(de::Half), offset_lin * sizeof(de::Half), cudaMemcpyDeviceToDevice, S);
        offset_lin += kernel->width;
        offset_ker += kernel->pitch;
    }
    // strat the main loop
    if (ker_dim.x == (bounded_kernel_R16 * 2 + 1) && ker_dim.y == (bounded_kernel_R16 * 2 + 1)) {
        decx::dev_main_loop_hconv2_sk_exact16x16_BC(
            &Dsrc_alloc_dim, &Ddst_alloc_dim, &ker_dim, src, kernel, dst, Dsrc, Ddst, S);
    }
    else {
        decx::dev_main_loop_hconv2_sk_within16x16_BC(
            &Dsrc_alloc_dim, &Ddst_alloc_dim, &ker_dim, src, kernel, dst, Dsrc, Ddst, S);
    }

    decx::alloc::_device_dealloc(&dev_buffer);

    checkCudaErrors(cudaDeviceSynchronize());

    checkCudaErrors(cudaStreamDestroy(S));
}



// ******************************************************************************************************************************

static void decx::dev_hConv2_border_zero_sk(_GPU_MatrixArray<de::Half>* src, _GPU_Matrix<de::Half>* kernel, _GPU_MatrixArray<de::Half>* dst, de::DH* handle)
{
    int2 half_ker_dim;
    half_ker_dim.x = kernel->width / 2;                half_ker_dim.y = kernel->height / 2;

    /*if (dst->width != src->width || dst->height != src->height) {
        decx::MDim_Not_Matching(handle);
        return;
    }*/

    const uint3 dst_dim = make_uint3(src->width, 
                                     src->height,
                                     src->ArrayNumber);

    decx::_dev_conv2_dst_rearrangement(dst, dst_dim);

    if (half_ker_dim.x < bounded_kernel_R8 + 1) {
        if (half_ker_dim.y < bounded_kernel_R8 + 1) {
            decx::dev_Conv2_BC_R8x8_SK(src, kernel, dst);
        }
        else {
            decx::dev_Conv2_BC_R16x8_SK(src, kernel, dst);
        }
    }
    else {
        if (half_ker_dim.y < bounded_kernel_R8 + 1) {
            decx::dev_Conv2_BC_R8x16_SK(src, kernel, dst);
        }
        else {
            decx::dev_Conv2_BC_R16x16_SK(src, kernel, dst);
        }
    }
}


#endif