/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SKLAND_GRAPHIC_INTERNAL_SHADER_META_HPP_
#define SKLAND_GRAPHIC_INTERNAL_SHADER_META_HPP_

#include "SkShader.h"

namespace skland {

/**
 * @ingroup graphic_intern
 * @brief Structure to encapsulate a sk_sp<SkShader> object
 */
struct ShaderMeta {

  ShaderMeta() {}

  ShaderMeta(const sk_sp<SkShader> &shader)
      : sk_shader(shader) {}

  ShaderMeta(const ShaderMeta &other)
      : sk_shader(other.sk_shader) {}

  ShaderMeta &operator=(const ShaderMeta &other) {
    sk_shader = other.sk_shader;
    return *this;
  }

  ShaderMeta &operator=(const sk_sp<SkShader> &shader) {
    sk_shader = shader;
    return *this;
  }

  sk_sp<SkShader> sk_shader;

};

}

#endif  // SKLAND_GRAPHIC_INTERNAL_SHADER_META_HPP_
