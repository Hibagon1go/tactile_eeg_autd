// File: backend_cuda.cpp
// Project: cuda
// Created Date: 13/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 18/10/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#include "autd3/gain/backend_cuda.hpp"

#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4102 26439 26478 26495 26812)
#endif
#include <cublas_v2.h>
#include <cuda_runtime_api.h>
#include <cusolverDn.h>

#include "./kernel.h"
#if _MSC_VER
#pragma warning(pop)
#endif

namespace autd3::gain::holo {

namespace {
cublasOperation_t convert(const TRANSPOSE trans) {
  switch (trans) {
    case TRANSPOSE::NO_TRANS:
      return CUBLAS_OP_N;
    case TRANSPOSE::CONJ_TRANS:
      return CUBLAS_OP_C;
    case TRANSPOSE::TRANS:
      return CUBLAS_OP_T;
  }
  return CUBLAS_OP_N;
}
}  // namespace

#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26812)
#endif

class BufferPool final {
 public:
  BufferPool() = default;
  ~BufferPool() { clear(); }
  BufferPool(const BufferPool& v) noexcept = default;
  BufferPool& operator=(const BufferPool& obj) = default;
  BufferPool(BufferPool&& obj) = default;
  BufferPool& operator=(BufferPool&& obj) = default;

  void* get(const VectorXc& v) {
    const auto key = reinterpret_cast<std::uintptr_t>(v.data());
    if (_pool.find(key) != _pool.end()) return _pool[key];

    void* dp;
    cudaMalloc(&dp, sizeof(complex) * v.size());
    cudaMemcpy(dp, v.data(), sizeof(complex) * v.size(), cudaMemcpyHostToDevice);
    _pool.emplace(key, dp);
    return dp;
  }

  void* get(const MatrixXc& v) {
    const auto key = reinterpret_cast<std::uintptr_t>(v.data());
    if (_pool.find(key) != _pool.end()) return _pool[key];

    void* dp;
    cudaMalloc(&dp, sizeof(complex) * v.size());
    cudaMemcpy(dp, v.data(), sizeof(complex) * v.size(), cudaMemcpyHostToDevice);
    _pool.emplace(key, dp);
    return dp;
  }

  void* get(const VectorXd& v) {
    const auto key = reinterpret_cast<std::uintptr_t>(v.data());
    if (_pool.find(key) != _pool.end()) return _pool[key];

    void* dp;
    cudaMalloc(&dp, sizeof(double) * v.size());
    cudaMemcpy(dp, v.data(), sizeof(double) * v.size(), cudaMemcpyHostToDevice);
    _pool.emplace(key, dp);
    return dp;
  }

  void* get(const MatrixXd& v) {
    const auto key = reinterpret_cast<std::uintptr_t>(v.data());
    if (_pool.find(key) != _pool.end()) return _pool[key];

    void* dp;
    cudaMalloc(&dp, sizeof(double) * v.size());
    cudaMemcpy(dp, v.data(), sizeof(double) * v.size(), cudaMemcpyHostToDevice);
    _pool.emplace(key, dp);
    return dp;
  }

  void clear() {
    for (const auto& [_, p] : _pool) cudaFree(p);
    _pool.clear();
  }

 private:
  std::unordered_map<std::uintptr_t, void*> _pool;
};

class CUDABackendImpl final : public CUDABackend {
 public:
  explicit CUDABackendImpl(const int device_idx) {
    cudaSetDevice(device_idx);
    cublasCreate(&_handle);
    cusolverDnCreate(&_handle_s);
  }
  ~CUDABackendImpl() override {
    cublasDestroy(_handle);
    cusolverDnDestroy(_handle_s);
  }
  CUDABackendImpl(const CUDABackendImpl& v) = default;
  CUDABackendImpl& operator=(const CUDABackendImpl& obj) = default;
  CUDABackendImpl(CUDABackendImpl&& obj) = default;
  CUDABackendImpl& operator=(CUDABackendImpl&& obj) = default;

  void init() override { _pool.clear(); }

  void to_host(VectorXc& dst) override {
    const auto dst_p = _pool.get(dst);
    cudaMemcpy(dst.data(), dst_p, sizeof(complex) * dst.size(), cudaMemcpyDeviceToHost);
  }
  void to_host(MatrixXc& dst) override {
    const auto dst_p = _pool.get(dst);
    cudaMemcpy(dst.data(), dst_p, sizeof(complex) * dst.size(), cudaMemcpyDeviceToHost);
  }
  void to_host(VectorXd& dst) override {
    const auto dst_p = _pool.get(dst);
    cudaMemcpy(dst.data(), dst_p, sizeof(double) * dst.size(), cudaMemcpyDeviceToHost);
  }
  void to_host(MatrixXd& dst) override {
    const auto dst_p = _pool.get(dst);
    cudaMemcpy(dst.data(), dst_p, sizeof(double) * dst.size(), cudaMemcpyDeviceToHost);
  }

  void copy_to(const MatrixXc& src, MatrixXc& dst) override {
    const auto src_p = _pool.get(src);
    const auto dst_p = _pool.get(dst);
    cudaMemcpy(dst_p, src_p, sizeof(complex) * src.size(), cudaMemcpyDeviceToDevice);
  }

  void copy_to(const MatrixXd& src, MatrixXd& dst) override {
    const auto src_p = _pool.get(src);
    const auto dst_p = _pool.get(dst);
    cudaMemcpy(dst_p, src_p, sizeof(double) * src.size(), cudaMemcpyDeviceToDevice);
  }
  void copy_to(const VectorXd& src, VectorXd& dst) override {
    const auto src_p = _pool.get(src);
    const auto dst_p = _pool.get(dst);
    cudaMemcpy(dst_p, src_p, sizeof(double) * src.size(), cudaMemcpyDeviceToDevice);
  }
  void copy_to(const VectorXc& src, VectorXc& dst) override {
    const auto src_p = _pool.get(src);
    const auto dst_p = _pool.get(dst);
    cudaMemcpy(dst_p, src_p, sizeof(complex) * src.size(), cudaMemcpyDeviceToDevice);
  }

  void abs(const VectorXc& src, VectorXd& dst) override {
    const auto size = static_cast<uint32_t>(src.size());
    const auto src_p = static_cast<cuDoubleComplex*>(_pool.get(src));
    const auto dst_p = static_cast<double*>(_pool.get(dst));
    cu_abs(src_p, size, 1, dst_p);
  }

  void abs(const VectorXc& src, VectorXc& dst) override {
    const auto size = static_cast<uint32_t>(src.size());
    const auto src_p = static_cast<cuDoubleComplex*>(_pool.get(src));
    const auto dst_p = static_cast<cuDoubleComplex*>(_pool.get(dst));
    cu_abs(src_p, size, 1, dst_p);
  }
  void sqrt(const VectorXd& src, VectorXd& dst) override {
    const auto size = static_cast<uint32_t>(src.size());
    const auto src_p = static_cast<double*>(_pool.get(src));
    const auto dst_p = static_cast<double*>(_pool.get(dst));
    cu_sqrt(src_p, size, 1, dst_p);
  }
  void conj(const VectorXc& src, VectorXc& dst) override {
    const auto size = static_cast<uint32_t>(src.size());
    const auto src_p = static_cast<cuDoubleComplex*>(_pool.get(src));
    const auto dst_p = static_cast<cuDoubleComplex*>(_pool.get(dst));
    cu_conj(src_p, size, 1, dst_p);
  }
  void arg(const VectorXc& src, VectorXc& dst) override {
    const auto size = static_cast<uint32_t>(src.size());
    const auto src_p = static_cast<cuDoubleComplex*>(_pool.get(src));
    const auto dst_p = static_cast<cuDoubleComplex*>(_pool.get(dst));
    cu_arg(src_p, size, 1, dst_p);
  }
  void reciprocal(const VectorXc& src, VectorXc& dst) override {
    const auto size = static_cast<uint32_t>(src.size());
    const auto src_p = static_cast<cuDoubleComplex*>(_pool.get(src));
    const auto dst_p = static_cast<cuDoubleComplex*>(_pool.get(dst));
    cu_reciprocal(src_p, size, 1, dst_p);
  }
  void exp(const VectorXc& src, VectorXc& dst) override {
    const auto size = static_cast<uint32_t>(src.size());
    const auto src_p = static_cast<cuDoubleComplex*>(_pool.get(src));
    const auto dst_p = static_cast<cuDoubleComplex*>(_pool.get(dst));
    cu_exp(src_p, size, 1, dst_p);
  }
  void pow(const VectorXd& src, const double p, VectorXd& dst) override {
    const auto size = static_cast<uint32_t>(src.size());
    const auto src_p = static_cast<double*>(_pool.get(src));
    const auto dst_p = static_cast<double*>(_pool.get(dst));
    cu_pow(src_p, p, size, 1, dst_p);
  }

  void real(const MatrixXc& src, MatrixXd& re) override {
    const auto row = static_cast<uint32_t>(src.rows());
    const auto col = static_cast<uint32_t>(src.cols());
    const auto sp = static_cast<cuDoubleComplex*>(_pool.get(src));
    const auto rp = static_cast<double*>(_pool.get(re));
    cu_real(sp, row, col, rp);
  }
  void imag(const MatrixXc& src, MatrixXd& im) override {
    const auto row = static_cast<uint32_t>(src.rows());
    const auto col = static_cast<uint32_t>(src.cols());
    const auto sp = static_cast<cuDoubleComplex*>(_pool.get(src));
    const auto ip = static_cast<double*>(_pool.get(im));
    cu_imag(sp, row, col, ip);
  }

  void make_complex(const VectorXd& re, const VectorXd& im, VectorXc& dst) override {
    const auto row = static_cast<uint32_t>(dst.rows());
    const auto col = static_cast<uint32_t>(dst.cols());
    const auto rp = static_cast<double*>(_pool.get(re));
    const auto ip = static_cast<double*>(_pool.get(im));
    const auto dp = static_cast<cuDoubleComplex*>(_pool.get(dst));
    cu_make_complex(rp, ip, row, col, dp);
  }

  void create_diagonal(const VectorXc& src, MatrixXc& dst) override {
    const auto row = static_cast<uint32_t>(dst.rows());
    const auto col = static_cast<uint32_t>(dst.cols());
    const auto src_p = static_cast<cuDoubleComplex*>(_pool.get(src));
    const auto dst_p = static_cast<cuDoubleComplex*>(_pool.get(dst));
    cu_set_diagonal(src_p, row, col, dst_p);
  }

  void get_diagonal(const MatrixXc& src, VectorXc& dst) override {
    const auto row = static_cast<uint32_t>(src.rows());
    const auto col = static_cast<uint32_t>(src.cols());
    const auto src_p = static_cast<cuDoubleComplex*>(_pool.get(src));
    const auto dst_p = static_cast<cuDoubleComplex*>(_pool.get(dst));
    cu_get_diagonal(src_p, row, col, dst_p);
  }
  void get_diagonal(const MatrixXd& src, VectorXd& dst) override {
    const auto row = static_cast<uint32_t>(src.rows());
    const auto col = static_cast<uint32_t>(src.cols());
    const auto src_p = static_cast<double*>(_pool.get(src));
    const auto dst_p = static_cast<double*>(_pool.get(dst));
    cu_get_diagonal(src_p, row, col, dst_p);
  }

  void set(const size_t i, const complex value, VectorXc& dst) override {
    const auto dst_p = static_cast<complex*>(_pool.get(dst));
    cudaMemcpy(dst_p + i, &value, sizeof(complex), cudaMemcpyHostToDevice);
  }

  void set_row(VectorXc& src, const size_t i, const size_t begin, const size_t end, MatrixXc& dst) override {
    const auto row = static_cast<int>(dst.rows());
    const auto src_p = static_cast<cuDoubleComplex*>(_pool.get(src));
    const auto dst_p = static_cast<cuDoubleComplex*>(_pool.get(dst));
    cublasZcopy(_handle, static_cast<int>(end - begin), src_p + begin, 1, dst_p + i + begin * row, row);
  }

  void set_col(VectorXc& src, const size_t i, const size_t begin, const size_t end, MatrixXc& dst) override {
    const auto row = dst.rows();
    const auto src_p = static_cast<complex*>(_pool.get(src));
    const auto dst_p = static_cast<complex*>(_pool.get(dst));
    cudaMemcpy(dst_p + i * row + begin, src_p + begin, (end - begin) * sizeof(complex), cudaMemcpyDeviceToDevice);
  }

  void get_col(const MatrixXc& src, const size_t i, VectorXc& dst) override {
    const auto row = src.rows();
    const auto src_p = static_cast<complex*>(_pool.get(src));
    const auto dst_p = static_cast<complex*>(_pool.get(dst));
    cudaMemcpy(dst_p, src_p + i * row, row * sizeof(complex), cudaMemcpyDeviceToDevice);
  }

  complex max_abs_element(const VectorXc& src) override {
    const auto src_p = _pool.get(src);
    VectorXc tmp(src.size());
    cudaMemcpy(tmp.data(), src_p, sizeof(complex) * src.size(), cudaMemcpyDeviceToHost);
    Eigen::Index idx = 0;
    tmp.cwiseAbs2().maxCoeff(&idx);
    return tmp(idx);
  }

  double max_element(const VectorXd& src) override {
    const auto src_p = _pool.get(src);
    VectorXd tmp(src.size());
    cudaMemcpy(tmp.data(), src_p, sizeof(double) * src.size(), cudaMemcpyDeviceToHost);
    return tmp.maxCoeff();
  }

  void scale(const complex value, VectorXc& dst) override {
    const auto dst_p = static_cast<complex*>(_pool.get(dst));
    cublasZscal(_handle, static_cast<int>(dst.size()), reinterpret_cast<const cuDoubleComplex*>(&value), reinterpret_cast<cuDoubleComplex*>(dst_p),
                1);
  }

  void scale(const double value, VectorXd& dst) override {
    const auto dst_p = static_cast<double*>(_pool.get(dst));
    cublasDscal(_handle, static_cast<int>(dst.size()), &value, dst_p, 1);
  }

  complex dot(const VectorXc& a, const VectorXc& b) override {
    complex d;
    const auto a_p = static_cast<cuDoubleComplex*>(_pool.get(a));
    const auto b_p = static_cast<cuDoubleComplex*>(_pool.get(b));
    cublasZdotc(_handle, static_cast<int>(a.size()), a_p, 1, b_p, 1, reinterpret_cast<cuDoubleComplex*>(&d));
    return d;
  }

  double dot(const VectorXd& a, const VectorXd& b) override {
    double d;
    const auto a_p = static_cast<double*>(_pool.get(a));
    const auto b_p = static_cast<double*>(_pool.get(b));
    cublasDdot(_handle, static_cast<int>(a.size()), a_p, 1, b_p, 1, &d);
    return d;
  }

  void add(const double alpha, const MatrixXd& a, MatrixXd& b) override {
    const auto a_p = static_cast<double*>(_pool.get(a));
    const auto b_p = static_cast<double*>(_pool.get(b));
    cublasDaxpy(_handle, static_cast<int>(a.size()), &alpha, a_p, 1, b_p, 1);
  }

  void add(const double alpha, const VectorXd& a, VectorXd& b) override {
    const auto a_p = static_cast<double*>(_pool.get(a));
    const auto b_p = static_cast<double*>(_pool.get(b));
    cublasDaxpy(_handle, static_cast<int>(a.size()), &alpha, a_p, 1, b_p, 1);
  }

  void add(const complex alpha, const MatrixXc& a, MatrixXc& b) override {
    const auto a_p = static_cast<cuDoubleComplex*>(_pool.get(a));
    const auto b_p = static_cast<cuDoubleComplex*>(_pool.get(b));
    cublasZaxpy(_handle, static_cast<int>(a.size()), reinterpret_cast<const cuDoubleComplex*>(&alpha), a_p, 1, b_p, 1);
  }

  void add(const complex alpha, const VectorXc& a, VectorXc& b) override {
    const auto a_p = static_cast<cuDoubleComplex*>(_pool.get(a));
    const auto b_p = static_cast<cuDoubleComplex*>(_pool.get(b));
    cublasZaxpy(_handle, static_cast<int>(a.size()), reinterpret_cast<const cuDoubleComplex*>(&alpha), a_p, 1, b_p, 1);
  }

  void mul(const TRANSPOSE trans_a, const TRANSPOSE trans_b, const complex alpha, const MatrixXc& a, const MatrixXc& b, const complex beta,
           MatrixXc& c) override {
    const auto m = static_cast<int>(c.rows());
    const auto n = static_cast<int>(c.cols());
    const auto k = trans_a == TRANSPOSE::NO_TRANS ? static_cast<int>(a.cols()) : static_cast<int>(a.rows());
    const auto lda = static_cast<int>(a.rows());
    const auto ldb = static_cast<int>(b.rows());
    const auto ldc = static_cast<int>(c.rows());
    const auto a_p = static_cast<cuDoubleComplex*>(_pool.get(a));
    const auto b_p = static_cast<cuDoubleComplex*>(_pool.get(b));
    const auto c_p = static_cast<cuDoubleComplex*>(_pool.get(c));
    cublasZgemm(_handle, convert(trans_a), convert(trans_b), m, n, k, reinterpret_cast<const cuDoubleComplex*>(&alpha), a_p, lda, b_p, ldb,
                reinterpret_cast<const cuDoubleComplex*>(&beta), c_p, ldc);
  }

  void mul(const TRANSPOSE trans_a, const complex alpha, const MatrixXc& a, const VectorXc& b, const complex beta, VectorXc& c) override {
    const auto m = static_cast<int>(a.rows());
    const auto n = static_cast<int>(a.cols());
    const auto lda = m;
    const auto a_p = static_cast<cuDoubleComplex*>(_pool.get(a));
    const auto b_p = static_cast<cuDoubleComplex*>(_pool.get(b));
    const auto c_p = static_cast<cuDoubleComplex*>(_pool.get(c));
    cublasZgemv(_handle, convert(trans_a), m, n, reinterpret_cast<const cuDoubleComplex*>(&alpha), a_p, lda, b_p, 1,
                reinterpret_cast<const cuDoubleComplex*>(&beta), c_p, 1);
  }

  void mul(const TRANSPOSE trans_a, const TRANSPOSE trans_b, const double alpha, const MatrixXd& a, const MatrixXd& b, const double beta,
           MatrixXd& c) override {
    const auto m = static_cast<int>(c.rows());
    const auto n = static_cast<int>(c.cols());
    const auto k = trans_a == TRANSPOSE::NO_TRANS ? static_cast<int>(a.cols()) : static_cast<int>(a.rows());
    const auto lda = static_cast<int>(a.rows());
    const auto ldb = static_cast<int>(b.rows());
    const auto ldc = static_cast<int>(c.rows());
    const auto a_p = static_cast<double*>(_pool.get(a));
    const auto b_p = static_cast<double*>(_pool.get(b));
    const auto c_p = static_cast<double*>(_pool.get(c));
    cublasDgemm(_handle, convert(trans_a), convert(trans_b), m, n, k, &alpha, a_p, lda, b_p, ldb, &beta, c_p, ldc);
  }
  void mul(const TRANSPOSE trans_a, const double alpha, const MatrixXd& a, const VectorXd& b, const double beta, VectorXd& c) override {
    const auto m = static_cast<int>(a.rows());
    const auto n = static_cast<int>(a.cols());
    const auto lda = m;
    const auto a_p = static_cast<double*>(_pool.get(a));
    const auto b_p = static_cast<double*>(_pool.get(b));
    const auto c_p = static_cast<double*>(_pool.get(c));
    cublasDgemv(_handle, convert(trans_a), m, n, &alpha, a_p, lda, b_p, 1, &beta, c_p, 1);
  }

  void hadamard_product(const VectorXc& a, const VectorXc& b, VectorXc& c) override {
    const auto m = static_cast<uint32_t>(a.size());
    const auto a_p = static_cast<cuDoubleComplex*>(_pool.get(a));
    const auto b_p = static_cast<cuDoubleComplex*>(_pool.get(b));
    const auto c_p = static_cast<cuDoubleComplex*>(_pool.get(c));
    cu_hadamard_product(a_p, b_p, m, 1, c_p);
  }

  void hadamard_product(const MatrixXc& a, const MatrixXc& b, MatrixXc& c) override {
    const auto m = static_cast<uint32_t>(a.rows());
    const auto n = static_cast<uint32_t>(a.cols());
    const auto a_p = static_cast<cuDoubleComplex*>(_pool.get(a));
    const auto b_p = static_cast<cuDoubleComplex*>(_pool.get(b));
    const auto c_p = static_cast<cuDoubleComplex*>(_pool.get(c));
    cu_hadamard_product(a_p, b_p, m, n, c_p);
  }

  void concat_col(const MatrixXc& a, const MatrixXc& b, MatrixXc& dst) override {
    const auto a_p = static_cast<cuDoubleComplex*>(_pool.get(a));
    const auto b_p = static_cast<cuDoubleComplex*>(_pool.get(b));
    const auto c_p = static_cast<cuDoubleComplex*>(_pool.get(dst));
    cudaMemcpy(c_p, a_p, a.size() * sizeof(complex), cudaMemcpyDeviceToDevice);
    cudaMemcpy(c_p + a.size(), b_p, b.size() * sizeof(complex), cudaMemcpyDeviceToDevice);
  }
  void concat_row(const MatrixXc& a, const MatrixXc& b, MatrixXc& dst) override {
    const auto a_p = static_cast<cuDoubleComplex*>(_pool.get(a));
    const auto b_p = static_cast<cuDoubleComplex*>(_pool.get(b));
    const auto c_p = static_cast<cuDoubleComplex*>(_pool.get(dst));
    for (Eigen::Index i = 0; i < a.cols(); i++) {
      cudaMemcpy(c_p + i * (a.rows() + b.rows()), a_p + i * a.rows(), a.rows() * sizeof(complex), cudaMemcpyDeviceToDevice);
      cudaMemcpy(c_p + i * (a.rows() + b.rows()) + a.rows(), b_p + i * b.rows(), b.rows() * sizeof(complex), cudaMemcpyDeviceToDevice);
    }
  }
  void concat_row(const VectorXc& a, const VectorXc& b, VectorXc& dst) override {
    const auto a_p = static_cast<cuDoubleComplex*>(_pool.get(a));
    const auto b_p = static_cast<cuDoubleComplex*>(_pool.get(b));
    const auto c_p = static_cast<cuDoubleComplex*>(_pool.get(dst));
    cudaMemcpy(c_p, a_p, a.size() * sizeof(complex), cudaMemcpyDeviceToDevice);
    cudaMemcpy(c_p + a.size(), b_p, b.size() * sizeof(complex), cudaMemcpyDeviceToDevice);
  }

  void max_eigen_vector(MatrixXc& src, VectorXc& dst) override {
    const auto size = src.cols();
    const auto src_p = static_cast<complex*>(_pool.get(src));
    const auto dst_p = static_cast<complex*>(_pool.get(dst));

    double* d_w = nullptr;
    cudaMalloc(reinterpret_cast<void**>(&d_w), sizeof(double) * size);

    size_t workspace_in_bytes_on_device;
    size_t workspace_in_bytes_on_host;
    cusolverDnXsyevd_bufferSize(_handle_s, nullptr, CUSOLVER_EIG_MODE_VECTOR, CUBLAS_FILL_MODE_UPPER, size, CUDA_C_64F, src_p, size, CUDA_R_64F, d_w,
                                CUDA_C_64F, &workspace_in_bytes_on_device, &workspace_in_bytes_on_host);

    void* workspace_buffer_on_device = nullptr;
    void* workspace_buffer_on_host = nullptr;
    cudaMalloc(&workspace_buffer_on_device, workspace_in_bytes_on_device);
    if (workspace_in_bytes_on_host > 0) workspace_buffer_on_host = malloc(workspace_in_bytes_on_host);

    int* info = nullptr;
    cudaMalloc(reinterpret_cast<void**>(&info), sizeof(int));
    cusolverDnXsyevd(_handle_s, nullptr, CUSOLVER_EIG_MODE_VECTOR, CUBLAS_FILL_MODE_UPPER, size, CUDA_C_64F, src_p, size, CUDA_R_64F, d_w, CUDA_C_64F,
                     workspace_buffer_on_device, workspace_in_bytes_on_device, workspace_buffer_on_host, workspace_in_bytes_on_host, info);
    cudaFree(d_w);
    cudaFree(info);
    cudaFree(workspace_buffer_on_device);
    free(workspace_buffer_on_host);

    cudaMemcpy(dst_p, src_p + size * (size - 1), size * sizeof(complex), cudaMemcpyDeviceToDevice);
  }

  void pseudo_inverse_svd(MatrixXc& src, const double alpha, MatrixXc& u, MatrixXc& s, MatrixXc& vt, MatrixXc& buf, MatrixXc& dst) override {
    const auto nc = src.cols();
    const auto nr = src.rows();

    const auto m = static_cast<int>(nr);
    const auto n = static_cast<int>(nc);

    const auto src_p = static_cast<complex*>(_pool.get(src));
    const auto u_p = static_cast<complex*>(_pool.get(u));
    const auto v_p = static_cast<complex*>(_pool.get(vt));
    const auto s_p = static_cast<cuDoubleComplex*>(_pool.get(s));

    const auto lda = m;
    const auto ldu = m;
    const auto ldv = n;

    const auto s_size = std::min(nr, nc);
    double* d_s = nullptr;
    cudaMalloc(reinterpret_cast<void**>(&d_s), sizeof(double) * s_size);

    size_t workspace_in_bytes_on_device;
    size_t workspace_in_bytes_on_host;

    cusolverDnXgesvdp_bufferSize(_handle_s, nullptr, CUSOLVER_EIG_MODE_VECTOR, 0, m, n, CUDA_C_64F, src_p, lda, CUDA_R_64F, d_s, CUDA_C_64F, u_p, ldu,
                                 CUDA_C_64F, v_p, ldv, CUDA_C_64F, &workspace_in_bytes_on_device, &workspace_in_bytes_on_host);
    void* workspace_buffer_on_device = nullptr;
    void* workspace_buffer_on_host = nullptr;
    cudaMalloc(&workspace_buffer_on_device, workspace_in_bytes_on_device);
    if (workspace_in_bytes_on_host > 0) workspace_buffer_on_host = malloc(workspace_in_bytes_on_host);

    int* info = nullptr;
    cudaMalloc(reinterpret_cast<void**>(&info), sizeof(int));
    double h_err_sigma;
    cusolverDnXgesvdp(_handle_s, nullptr, CUSOLVER_EIG_MODE_VECTOR, 0, m, n, CUDA_C_64F, src_p, lda, CUDA_R_64F, d_s, CUDA_C_64F, u_p, ldu,
                      CUDA_C_64F, v_p, ldv, CUDA_C_64F, workspace_buffer_on_device, workspace_in_bytes_on_device, workspace_buffer_on_host,
                      workspace_in_bytes_on_host, info, &h_err_sigma);

    cu_calc_singular_inv(d_s, static_cast<uint32_t>(n), static_cast<uint32_t>(m), alpha, s_p);

    mul(TRANSPOSE::NO_TRANS, TRANSPOSE::CONJ_TRANS, ONE, s, u, ZERO, buf);
    mul(TRANSPOSE::NO_TRANS, TRANSPOSE::NO_TRANS, ONE, vt, buf, ZERO, dst);

    cudaFree(d_s);
    cudaFree(info);
    cudaFree(workspace_buffer_on_device);
    free(workspace_buffer_on_host);
  }

  void pseudo_inverse_svd(MatrixXd& src, const double alpha, MatrixXd& u, MatrixXd& s, MatrixXd& vt, MatrixXd& buf, MatrixXd& dst) override {
    const auto nc = src.cols();
    const auto nr = src.rows();

    const auto m = static_cast<int>(nr);
    const auto n = static_cast<int>(nc);

    const auto src_p = static_cast<double*>(_pool.get(src));
    const auto u_p = static_cast<double*>(_pool.get(u));
    const auto v_p = static_cast<double*>(_pool.get(vt));
    const auto s_p = static_cast<double*>(_pool.get(s));

    const auto lda = m;
    const auto ldu = m;
    const auto ldv = n;

    const auto s_size = std::min(nr, nc);
    double* d_s = nullptr;
    cudaMalloc(reinterpret_cast<void**>(&d_s), sizeof(double) * s_size);

    size_t workspace_in_bytes_on_device;
    size_t workspace_in_bytes_on_host;

    cusolverDnXgesvdp_bufferSize(_handle_s, nullptr, CUSOLVER_EIG_MODE_VECTOR, 0, m, n, CUDA_R_64F, src_p, lda, CUDA_R_64F, d_s, CUDA_R_64F, u_p, ldu,
                                 CUDA_R_64F, v_p, ldv, CUDA_R_64F, &workspace_in_bytes_on_device, &workspace_in_bytes_on_host);
    void* workspace_buffer_on_device = nullptr;
    void* workspace_buffer_on_host = nullptr;
    cudaMalloc(&workspace_buffer_on_device, workspace_in_bytes_on_device);
    if (workspace_in_bytes_on_host > 0) workspace_buffer_on_host = malloc(workspace_in_bytes_on_host);

    int* info = nullptr;
    cudaMalloc(reinterpret_cast<void**>(&info), sizeof(int));
    double h_err_sigma;
    cusolverDnXgesvdp(_handle_s, nullptr, CUSOLVER_EIG_MODE_VECTOR, 0, m, n, CUDA_R_64F, src_p, lda, CUDA_R_64F, d_s, CUDA_R_64F, u_p, ldu,
                      CUDA_R_64F, v_p, ldv, CUDA_R_64F, workspace_buffer_on_device, workspace_in_bytes_on_device, workspace_buffer_on_host,
                      workspace_in_bytes_on_host, info, &h_err_sigma);

    cu_calc_singular_inv(d_s, static_cast<uint32_t>(n), static_cast<uint32_t>(m), alpha, s_p);

    mul(TRANSPOSE::NO_TRANS, TRANSPOSE::TRANS, 1.0, s, u, 0.0, buf);
    mul(TRANSPOSE::NO_TRANS, TRANSPOSE::NO_TRANS, 1.0, vt, buf, 0.0, dst);

    cudaFree(d_s);
    cudaFree(info);
    cudaFree(workspace_buffer_on_device);
    free(workspace_buffer_on_host);
  }

  void solvet(MatrixXd& a, VectorXd& b) override {
    const auto n = static_cast<int>(a.cols());
    const auto lda = static_cast<int>(a.rows());
    const auto ldb = static_cast<int>(b.rows());

    const auto ap = static_cast<double*>(_pool.get(a));
    const auto bp = static_cast<double*>(_pool.get(b));

    size_t workspace_in_bytes_on_device;
    size_t workspace_in_bytes_on_host;
    cusolverDnXpotrf_bufferSize(_handle_s, nullptr, CUBLAS_FILL_MODE_UPPER, n, CUDA_R_64F, ap, lda, CUDA_R_64F, &workspace_in_bytes_on_device,
                                &workspace_in_bytes_on_host);

    void* workspace_buffer_on_device = nullptr;
    void* workspace_buffer_on_host = nullptr;
    cudaMalloc(&workspace_buffer_on_device, workspace_in_bytes_on_device);
    if (workspace_in_bytes_on_host > 0) workspace_buffer_on_host = malloc(workspace_in_bytes_on_host);

    int* info = nullptr;
    cudaMalloc(reinterpret_cast<void**>(&info), sizeof(int));
    cusolverDnXpotrf(_handle_s, nullptr, CUBLAS_FILL_MODE_UPPER, n, CUDA_R_64F, ap, lda, CUDA_R_64F, workspace_buffer_on_device,
                     workspace_in_bytes_on_device, workspace_buffer_on_host, workspace_in_bytes_on_host, info);
    cusolverDnXpotrs(_handle_s, nullptr, CUBLAS_FILL_MODE_UPPER, n, 1, CUDA_R_64F, ap, lda, CUDA_R_64F, bp, ldb, info);

    cudaFree(info);
    cudaFree(workspace_buffer_on_device);
    free(workspace_buffer_on_host);
  }

  void solveh(MatrixXc& a, VectorXc& b) override {
    const auto n = static_cast<int>(a.cols());
    const auto lda = static_cast<int>(a.rows());
    const auto ldb = static_cast<int>(b.rows());

    const auto ap = static_cast<cuDoubleComplex*>(_pool.get(a));
    const auto bp = static_cast<cuDoubleComplex*>(_pool.get(b));

    size_t workspace_in_bytes_on_device;
    size_t workspace_in_bytes_on_host;
    cusolverDnXpotrf_bufferSize(_handle_s, nullptr, CUBLAS_FILL_MODE_UPPER, n, CUDA_C_64F, ap, lda, CUDA_C_64F, &workspace_in_bytes_on_device,
                                &workspace_in_bytes_on_host);

    void* workspace_buffer_on_device = nullptr;
    void* workspace_buffer_on_host = nullptr;
    cudaMalloc(&workspace_buffer_on_device, workspace_in_bytes_on_device);
    if (workspace_in_bytes_on_host > 0) workspace_buffer_on_host = malloc(workspace_in_bytes_on_host);

    int* info = nullptr;
    cudaMalloc(reinterpret_cast<void**>(&info), sizeof(int));
    cusolverDnXpotrf(_handle_s, nullptr, CUBLAS_FILL_MODE_UPPER, n, CUDA_C_64F, ap, lda, CUDA_C_64F, workspace_buffer_on_device,
                     workspace_in_bytes_on_device, workspace_buffer_on_host, workspace_in_bytes_on_host, info);
    cusolverDnXpotrs(_handle_s, nullptr, CUBLAS_FILL_MODE_UPPER, n, 1, CUDA_C_64F, ap, lda, CUDA_C_64F, bp, ldb, info);

    cudaFree(info);
    cudaFree(workspace_buffer_on_device);
    free(workspace_buffer_on_host);
  }

  void reduce_col(const MatrixXd& a, VectorXd& b) override {
    const auto m = static_cast<uint32_t>(a.rows());
    const auto n = static_cast<uint32_t>(a.cols());
    const auto a_p = static_cast<double*>(_pool.get(a));
    const auto b_p = static_cast<double*>(_pool.get(b));
    double* buffer = nullptr;
    cudaMalloc(reinterpret_cast<void**>(&buffer), static_cast<size_t>(m) * BLOCK_SIZE / 2 * sizeof(double));
    cu_reduce_col(a_p, m, n, b_p, buffer);
    cudaFree(buffer);
  }

 private:
  BufferPool _pool;
  cublasHandle_t _handle = nullptr;
  cusolverDnHandle_t _handle_s = nullptr;
};

#if _MSC_VER
#pragma warning(pop)
#endif

BackendPtr CUDABackend::create(const int device_idx) { return std::make_shared<CUDABackendImpl>(device_idx); }

}  // namespace autd3::gain::holo
