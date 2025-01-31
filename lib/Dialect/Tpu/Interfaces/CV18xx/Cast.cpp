//===----------------------------------------------------------------------===//
//
// Copyright (c) 2020-2030 by Sophgo Technologies Inc. All rights reserved.
//
// Licensed under the Apache License v2.0.
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

#include "tpu_mlir/Dialect/Tpu/IR/TpuOps.h"
#include "tpu_mlir/Backend/CV18xx/CV18xx.h"

#include "tpu_mlir/Support/Module.h"
#include "tpu_mlir/Backend/CV18xx/CV18xx_global_api.h"



using namespace tpu_mlir::backend;

// =========================================
// GlobalGenInterface
// =========================================

// int8
void tpu::CastOp::codegen_global_cv18xx(int64_t layer_id) {
  int64_t n, c, h, w;
  int64_t offset = 0;
  float_t scale = 1.;
  module::getNCHW(getInput(), n, c, h, w);
  cvk_fmt_t from = CV18xx::getDataType(getInput());
  cvk_fmt_t to = CV18xx::getDataType(getOutput());
  gaddr_t ga_input = module::getAddress(getInput());
  gaddr_t ga_output = module::getAddress(getOutput());

  bool qInput = module::isUniformQuantized(getInput());
  bool qOutput = module::isUniformQuantized(getOutput());
  if (qInput || qOutput) {
    if (!qInput && qOutput) {
      auto qtype = module::getUniformQuantizedType(getOutput());
      scale = 1. / qtype.getScale();
    } else {
      auto qtype = module::getUniformQuantizedType(getInput());
      scale = qtype.getScale();
    }
  }
  //  quant to int8
  cvi_backend_tg_quant_kernel(layer_id, from, to, ga_input, ga_output, n, c, h,
                              w, scale, offset);
}

// =========================================
// LocalGenInterface
// =========================================

int64_t tpu::CastOp::getBufferSize_cv18xx(int64_t in_lmem_bytes,
                                          int64_t out_lmem_bytes,
                                          int64_t in_nslice, int64_t in_hslice,
                                          int64_t out_nslice,
                                          int64_t out_hslice) {
  llvm_unreachable("Not supported now");
  return 0;
}

void tpu::CastOp::codegen_local_cv18xx(int64_t n_step, int64_t h_step) {
  llvm_unreachable("Not supported now");
}
