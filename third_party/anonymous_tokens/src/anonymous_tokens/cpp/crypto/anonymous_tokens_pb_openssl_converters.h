// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ANONYMOUS_TOKENS_CPP_CRYPTO_ANONYMOUS_TOKENS_PB_OPENSSL_CONVERTERS_H_
#define ANONYMOUS_TOKENS_CPP_CRYPTO_ANONYMOUS_TOKENS_PB_OPENSSL_CONVERTERS_H_

#include <string>

#include "absl/status/statusor.h"
#include "anonymous_tokens/proto/anonymous_tokens.pb.h"
#include <openssl/base.h>


namespace anonymous_tokens {

// Generate a message mask. For more details, see
// https://datatracker.ietf.org/doc/draft-irtf-cfrg-rsa-blind-signatures/
absl::StatusOr<std::string> GenerateMask(
    const RSABlindSignaturePublicKey& public_key);

// Converts the AnonymousTokens proto hash type to the equivalent EVP digest.
absl::StatusOr<const EVP_MD*>
ProtoHashTypeToEVPDigest(HashType hash_type);

// Converts the AnonymousTokens proto hash type for mask generation function to
// the equivalent EVP digest.
absl::StatusOr<const EVP_MD*>
ProtoMaskGenFunctionToEVPDigest(MaskGenFunction mgf);

// Converts AnonymousTokens::RSAPrivateKey to bssl::UniquePtr<RSA> without
// public metadata augmentation.
absl::StatusOr<bssl::UniquePtr<RSA>>
AnonymousTokensRSAPrivateKeyToRSA(const RSAPrivateKey& private_key);

// Converts AnonymousTokens::RSAPublicKey to bssl::UniquePtr<RSA> without
// public metadata augmentation.
absl::StatusOr<bssl::UniquePtr<RSA>>
AnonymousTokensRSAPublicKeyToRSA(const RSAPublicKey& public_key);

}  // namespace anonymous_tokens


#endif  // ANONYMOUS_TOKENS_CPP_CRYPTO_ANONYMOUS_TOKENS_PB_OPENSSL_CONVERTERS_H_
