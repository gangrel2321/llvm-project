//===- TypeTranslation.h - Translate types between MLIR & LLVM --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the type translation function going from MLIR LLVM dialect
// to LLVM IR and back.
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_TARGET_LLVMIR_TYPETRANSLATION_H
#define MLIR_TARGET_LLVMIR_TYPETRANSLATION_H

#include <memory>

namespace llvm {
class DataLayout;
class LLVMContext;
class Type;
} // namespace llvm

namespace mlir {

class MLIRContext;

namespace LLVM {

class LLVMType;

namespace detail {
class TypeToLLVMIRTranslatorImpl;
class TypeFromLLVMIRTranslatorImpl;
} // namespace detail

/// Utility class to translate MLIR LLVM dialect types to LLVM IR. Stores the
/// translation state, in particular any identified structure types that can be
/// reused in further translation.
class TypeToLLVMIRTranslator {
public:
  TypeToLLVMIRTranslator(llvm::LLVMContext &context);
  ~TypeToLLVMIRTranslator();

  /// Returns the perferred alignment for the type given the data layout. Note
  /// that this will perform type conversion and store its results for future
  /// uses.
  // TODO: this should be removed when MLIR has proper data layout.
  unsigned getPreferredAlignment(LLVM::LLVMType type,
                                 const llvm::DataLayout &layout);

  /// Translates the given MLIR LLVM dialect type to LLVM IR.
  llvm::Type *translateType(LLVM::LLVMType type);

private:
  /// Private implementation.
  std::unique_ptr<detail::TypeToLLVMIRTranslatorImpl> impl;
};

/// Utility class to translate LLVM IR types to the MLIR LLVM dialect. Stores
/// the translation state, in particular any identified structure types that are
/// reused across translations.
class TypeFromLLVMIRTranslator {
public:
  TypeFromLLVMIRTranslator(MLIRContext &context);
  ~TypeFromLLVMIRTranslator();

  /// Translates the given LLVM IR type to the MLIR LLVM dialect.
  LLVM::LLVMType translateType(llvm::Type *type);

private:
  /// Private implementation.
  std::unique_ptr<detail::TypeFromLLVMIRTranslatorImpl> impl;
};

} // namespace LLVM
} // namespace mlir

#endif // MLIR_TARGET_LLVMIR_TYPETRANSLATION_H
